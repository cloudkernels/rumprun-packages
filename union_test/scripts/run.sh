set -e

if [ $# -ne 2 ]; then
    echo "usage: ./run.sh [ffs|iso] [layers]"
    exit 1
fi

RR_BASE="../../rumprun"
TENDER="${RR_BASE}/solo5/tenders/spt/solo5-spt"
IMAGE="bin/union_test.spt"
RR_MAKEFS="./scripts/rumprun-makefs.sh"
TMP_DIR=`mktemp -d`
LAYER_DIR="./layers"
ROOTFS="${TMP_DIR}/rootfs"
EMPTY="${TMP_DIR}/empty"
FS="$1"
LAYERS="$2"

cleanup() {
    ret=$?
    if [ $ret -ne 0 ]; then
        rm -rf "${LAYER_DIR}"
    fi

    rm -rf "${TMP_DIR}"

    exit $ret
}
trap cleanup EXIT

echo "Creating ${LAYER_DIR}"
rm -rf "${LAYER_DIR}" # delete layers from previous runs
mkdir "${LAYER_DIR}"
echo

echo "Creating the rootfs layer"
mkdir "${ROOTFS}"
# Make sure rootfs has files that are not shadowed by the upper layers,
# for testing purposes
ROOTFS_FILES=$(($LAYERS * 2))
for i in `seq 1 $ROOTFS_FILES`; do
    echo "from rootfs.layer" > "${ROOTFS}/$i"
done
ls "${ROOTFS}"
$RR_MAKEFS -t "$FS" "${LAYER_DIR}/rootfs.layer" "${ROOTFS}"
echo

echo "Creatig an empty layer"
mkdir "${EMPTY}"
ls "${EMPTY}"
$RR_MAKEFS -t "$FS" "${LAYER_DIR}/empty.layer" "${EMPTY}"
echo

for i in `seq 1 $LAYERS`; do
    TMP_LAYER="${TMP_DIR}/layer.$i"
    echo "Creating intermediate layer no. $i"
    mkdir "${TMP_LAYER}"
    echo "from $i.layer" > "${TMP_LAYER}/$i"
    ls "${TMP_LAYER}"
    $RR_MAKEFS -t "$FS" "${LAYER_DIR}/$i.layer" "${TMP_LAYER}"
done
echo

echo "Building the image"
make "${IMAGE}"
echo

echo "Will now run"
TENDER_CLI="--net=tap100 --disk=${LAYER_DIR}/rootfs.layer"
for i in `seq 1 $LAYERS`; do
    TENDER_CLI="${TENDER_CLI},${LAYER_DIR}/$i.layer"
done
TENDER_CLI="${TENDER_CLI},${LAYER_DIR}/empty.layer"

RUMP_CONFIG='{"blk": ['

for i in `seq 0 $LAYERS`;do
    BLK_ENTRY="{\"source\": \"etfs\",\"path\":\"hda$i\",\"fstype\":\"blk\",\"mountpoint\":\"/var/blk/hda$i\"},"
    RUMP_CONFIG="${RUMP_CONFIG}${BLK_ENTRY}"
done
LAST=$((1 + $LAYERS))
BLK_ENTRY="{\"source\": \"etfs\",\"path\":\"hda$LAST\",\"fstype\":\"blk\",\"mountpoint\":\"/var/blk/hda$LAST\"}"
RUMP_CONFIG="${RUMP_CONFIG}${BLK_ENTRY}],\"cmdline\": \"./a.out $ROOTFS_FILES\"}"

"${TENDER}" ${TENDER_CLI} "${IMAGE}" "${RUMP_CONFIG}"
