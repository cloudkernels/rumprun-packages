PYTHON_VER=python3.7
PKG_NAME=scipy
UPSTREAM=https://github.com/scipy/scipy/releases/download/v1.3.1/scipy-1.3.1.tar.gz
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
freeze_python/frozen.c: FREEZE_MODULES+=$$(for i in $$(for i in `find $(shell pwd)/python -name "__init__.py"`; do dirname "$$i"; done); \
                                                do find $$i -mindepth 1 -maxdepth 1 -name "*.py" | \
                                                awk -F'site-packages/$(PKG_NAME)/|.py' '$$(NF-1) !~ "__init__" { gsub("/",".",$$(NF-1)); \
						print "$(PKG_NAME)."$$(NF-1) }'; done)

$(EXAMPLES_DIR)/pythonRun: BUILDP_OBJECTS=$$(echo $$(find $(shell pwd)/build/build -name \*.o \
						-not -path "*scipy/fftpack/src/dfftpack/*" \
						-not -name "d1mach.o" -not -name "xsetf.o" -not -name "xsetun.o" -not -name "nnls.o") \
						| sed -e "s/ /\n/g" | sort -t. -r -k2 | rev | sed "s/\./?/2g" | sed "s/\//./" | rev | \
						sort -u -t. -k2 | rev | sed "s/\./\//2" | sed "s/?/./g" | rev)
$(EXAMPLES_DIR)/pythonRun: BUILDP_LINKER=$(RUMPRUN_CXX)

.PHONY: run_python
run_python: $(EXAMPLES_DIR)/pythonRun.spt $(EXAMPLES_DIR)/pythonRun.hvt $(EXAMPLES_DIR)/fs.iso
	cd $(EXAMPLES_DIR); \
	$(SOLO5_SPT) --disk=fs.iso --net=tap100 pythonRun.spt \
		'{"cmdline":"./pythonRun.spt /python/test.py","env":"PYTHONPATH=/python/lib/$(PYTHON_VER)/site-packages","blk":{"source":"etfs","path":"ld0a","fstype":"blk","mountpoint":"/python"}}'; \
	$(SOLO5_SPT) --disk=fs.iso --net=tap100 pythonRun.spt \
		'{"cmdline":"./pythonRun.spt /python/testApp.py","env":"PYTHONPATH=/python/lib/$(PYTHON_VER)/site-packages","blk":{"source":"etfs","path":"ld0a","fstype":"blk","mountpoint":"/python"}}'

.PHONY: clean
clean: clean_dist clean_hostdist
