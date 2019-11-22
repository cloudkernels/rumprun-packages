PYTHON_VER=python3.7
PKG_NAME=numpy
UPSTREAM=https://github.com/numpy/numpy/archive/v1.17.3.tar.gz
OPENBLAS=$(shell pwd)/../openblas

INSTALL_DEPS=$(OPENBLAS) build_prepare | build/stamp_patch
INSTALL_HOSTDEPS=| build_host/stamp_patch

include ../$(PYTHON_VER)/Makefile.pypkg.inc

SETUP_EXTRAENV=NPY_NOSMP=1

%/openblas:
	make -C $@

.PHONY: build_prepare
build_prepare: | build/stamp_patch 
	cd build; \
		echo "[openblas]\nlibraries = openblas\nlibrary_dirs = $(RUMPRUN_PKGS_DIR)/lib" > site.cfg && \
		echo "include_dirs = $(RUMPRUN_PKGS_DIR)/include\nruntime_library_dirs = $(RUMPRUN_PKGS_DIR)/lib\n" >> site.cfg && \
		echo "[lapack]\nlibraries = openblas\nlibrary_dirs = $(RUMPRUN_PKGS_DIR)/lib" >> site.cfg && \
		echo "include_dirs = $(RUMPRUN_PKGS_DIR)/include\nruntime_library_dirs = $(RUMPRUN_PKGS_DIR)/lib\n" >> site.cfg

freeze_python/frozen.c: FREEZE_MODULES+=$(PKG_NAME)

$(EXAMPLES_DIR)/pythonRun: BUILDP_OBJECTS=$(shell find $(shell pwd)/build/ -name \*.o | grep -v lapack_lite/python_xerbla)

.PHONY: run_python
run_python: $(EXAMPLES_DIR)/pythonRun.spt $(EXAMPLES_DIR)/pythonRun.hvt $(EXAMPLES_DIR)/fs.iso
	cd $(EXAMPLES_DIR);\
	$(SOLO5_SPT) --disk=fs.iso --net=tap100 pythonRun.spt \
		'{"cmdline":"./pythonRun.spt /python/test.py","env":"PYTHONPATH=/python/lib/$(PYTHON_VER)/site-packages","blk":{"source":"etfs","path":"ld0a","fstype":"blk","mountpoint":"/python"}}'; \
	$(SOLO5_SPT) --disk=fs.iso --net=tap100 pythonRun.spt \
		'{"cmdline":"./pythonRun.spt /python/testApp.py","env":"PYTHONPATH=/python/lib/$(PYTHON_VER)/site-packages","blk":{"source":"etfs","path":"ld0a","fstype":"blk","mountpoint":"/python"}}'

.PHONY: clean
clean: clean_dist clean_hostdist
