PYTHON_VER=python3.7
PKG_NAME=sklearn
UPSTREAM=https://github.com/scikit-learn/scikit-learn/archive/0.21.3.tar.gz
PKG_NUMPY=$(shell pwd)/../python3-numpy
PKG_SCIPY=$(shell pwd)/../python3-scipy
PKG_JOBLIB=$(shell pwd)/../python3-joblib
PKG_DEPS=$(PKG_NUMPY) $(PKG_SCIPY) $(PKG_JOBLIB)

INSTALL_DEPS=build_prepare | build/stamp_patch
INSTALL_HOSTDEPS=| build_host

include ../$(PYTHON_VER)/Makefile.pypkg.inc

SETUP_EXTRAENV=NPY_DISTUTILS_APPEND_FLAGS=1 SKLEARN_NO_OPENMP=1 
SETUP_HOSTEXTRAENV=LDFLAGS="-lm" NPY_DISTUTILS_APPEND_FLAGS=1 SKLEARN_NO_OPENMP=1

.PHONY: build_prepare
build_prepare: | build/stamp_patch
	cd build; \
		echo "[openblas]\nlibraries = openblas\nlibrary_dirs = $(RUMPRUN_PKGS_DIR)/lib" > site.cfg && \
		echo "include_dirs = $(RUMPRUN_PKGS_DIR)/include\nruntime_library_dirs = $(RUMPRUN_PKGS_DIR)/lib\n" >> site.cfg && \
		echo "[lapack]\nlibraries = openblas\nlibrary_dirs = $(RUMPRUN_PKGS_DIR)/lib" >> site.cfg && \
		echo "include_dirs = $(RUMPRUN_PKGS_DIR)/include\nruntime_library_dirs = $(RUMPRUN_PKGS_DIR)/lib\n" >> site.cfg

freeze_python/frozen.c: FREEZE_MODULES+=$(PKG_NAME)
freeze_python/frozen.c: FREEZE_MODULES+=$$(for i in $$(for i in `find $(shell pwd)/python -name "__init__.py"`; do dirname "$$i"; done); \
                                                do find $$i -mindepth 1 -maxdepth 1 -name "*.py" | \
                                                awk -F'site-packages/$(PKG_NAME)/|.py' '$$(NF-1) !~ "__init__" { gsub("/",".",$$(NF-1)); \
						print "$(PKG_NAME)."$$(NF-1) }'; done)

$(EXAMPLES_DIR)/pythonRun: BUILDP_LINKER=$(RUMPRUN_CXX)

.PHONY: run_python
run_python: $(EXAMPLES_DIR)/pythonRun.spt $(EXAMPLES_DIR)/pythonRun.hvt $(EXAMPLES_DIR)/fs.iso
	cd $(EXAMPLES_DIR); \
	$(SOLO5_SPT) --disk=fs.iso --net=tap100 pythonRun.spt \
		'{"cmdline":"./pythonRun.spt /python/test.py","env":"PYTHONPATH=/python/lib/$(PYTHON_VER)/site-packages","blk":{"source":"etfs","path":"ld0a","fstype":"blk","mountpoint":"/python"}}'; \
	$(SOLO5_SPT) --disk=fs.iso --net=tap100 --mem=2048 pythonRun.spt \
		'{"cmdline":"./pythonRun.spt /python/train_sgd_batch_256.py","env":"PYTHONPATH=/python/lib/$(PYTHON_VER)/site-packages","blk":{"source":"etfs","path":"ld0a","fstype":"blk","mountpoint":"/python"}}'

.PHONY: clean
clean: clean_dist clean_hostdist
