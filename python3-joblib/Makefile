PYTHON_VER=python3.7
PKG_NAME=joblib
UPSTREAM=https://github.com/joblib/joblib/archive/0.14.0.tar.gz
PKG_NUMPY=$(shell pwd)/../python3-numpy
PKG_DEPS=$(PKG_NUMPY)

INSTALL_DEPS=build_prepare | build/stamp_patch
INSTALL_HOSTDEPS=| build_host

include ../$(PYTHON_VER)/Makefile.pypkg.inc

SETUP_EXTRAENV=NPY_DISTUTILS_APPEND_FLAGS=1
SETUP_HOSTEXTRAENV=LDFLAGS="-lm" NPY_DISTUTILS_APPEND_FLAGS=1

.PHONY: build_prepare
build_prepare: | build/stamp_patch
	cd build; \
		echo "[openblas]\nlibraries = openblas\nlibrary_dirs = $(RUMPRUN_PKGS_DIR)/lib" > site.cfg && \
		echo "include_dirs = $(RUMPRUN_PKGS_DIR)/include\nruntime_library_dirs = $(RUMPRUN_PKGS_DIR)/lib\n" >> site.cfg && \
		echo "[lapack]\nlibraries = openblas\nlibrary_dirs = $(RUMPRUN_PKGS_DIR)/lib" >> site.cfg && \
		echo "include_dirs = $(RUMPRUN_PKGS_DIR)/include\nruntime_library_dirs = $(RUMPRUN_PKGS_DIR)/lib\n" >> site.cfg

freeze_python/frozen.c: FREEZE_MODULES+=$(PKG_NAME)

.PHONY: run_python
run_python: $(EXAMPLES_DIR)/pythonRun.spt $(EXAMPLES_DIR)/pythonRun.hvt $(EXAMPLES_DIR)/fs.iso
	cd $(EXAMPLES_DIR); \
	$(SOLO5_SPT) --disk=fs.iso --net=tap100 pythonRun.spt \
		'{"cmdline":"./pythonRun.spt /python/test.py","env":"PYTHONPATH=/python/lib/$(PYTHON_VER)/site-packages","blk":{"source":"etfs","path":"ld0a","fstype":"blk","mountpoint":"/python"}}'; \
	$(SOLO5_SPT) --disk=fs.iso --net=tap100 pythonRun.spt \
		'{"cmdline":"./pythonRun.spt /python/testApp.py","env":"PYTHONPATH=/python/lib/$(PYTHON_VER)/site-packages","blk":{"source":"etfs","path":"ld0a","fstype":"blk","mountpoint":"/python"}}'

.PHONY: clean
clean: clean_dist clean_hostdist
