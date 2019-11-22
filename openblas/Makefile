include ../Makefile.inc

UPSTREAM=https://github.com/xianyi/OpenBLAS/archive/v0.3.7.tar.gz
TARBALL=$(notdir $(UPSTREAM))
NCORES=$(shell nproc)
GCC_LIBS=$(shell pwd)/../gcc-libs

.PHONY: all
all: build/libopenblas.a

dl/$(TARBALL):
	mkdir -p dl
	../scripts/fetch.sh $(UPSTREAM) dl/$(TARBALL)

$(RUMPRUN_PKGS_DIR)/lib/libgfortran.a:
	make -C $(GCC_LIBS)

build: $(RUMPRUN_PKGS_DIR)/lib/libgfortran.a | dl/$(TARBALL)
	mkdir -p build
	cd build && tar -zx --strip-components 1 -f ../dl/$(TARBALL)

build/stamp_patch: build patches/*
	(cd build && ../../scripts/apply-patches.sh ./ ../patches/*)
	touch $@

build/libopenblas.a: build/stamp_patch
	cd build; \
	make CC=$(RUMPRUN_CC) FC=$(RUMPRUN_FC) HOSTCC=$(CC) NUM_THREADS=$(NCORES) \
		LDFLAGS="-L$(RUMPRUN_PKGS_DIR)/lib" \
		CFLAGS="-I$(RUMPRUN_SYSROOT)/include/gcc-6" NO_SHARED=1 2>&1 | tee $(shell pwd)/build.log; \
	make install CC=$(RUMPRUN_CC) FC=$(RUMPRUN_FC) HOSTCC=$(CC) NUM_THREADS=$(NCORES) \
		LDFLAGS="-L$(RUMPRUN_PKGS_DIR)/lib" PREFIX=$(RUMPRUN_PKGS_DIR) \
		CFLAGS="-I$(RUMPRUN_SYSROOT)/include/gcc-6" NO_SHARED=1 2>&1 | tee -a $(shell pwd)/build.log; \

run_test: build/libopenblas.a
	cd build/utest && make CC=$(RUMPRUN_CC) HOSTCC=$(CC) NUM_THREADS=$(NCORES) \
		CFLAGS="-I$(RUMPRUN_SYSROOT)/include/gcc-6" NO_SHARED=1 2>&1 |tee -a ../build.log
	rumprun-bake solo5_spt openblas_utest.spt build/utest/openblas_utest ;\
	dd if=/dev/zero of=dummy bs=512 count=2;\
	$(shell pwd)/../../rumprun/solo5/tenders/spt/solo5-spt --disk=dummy --net=tap100 openblas_utest.spt
	
.PHONY: clean
clean:
	rm -rf build
	rm -rf dummy *.spt
