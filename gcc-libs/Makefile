include ../Makefile.inc

UPSTREAM_VERSION=$(shell $(RUMPRUN_CC) -dumpversion)
UPSTREAM_MAJORVER=$(word 1,$(subst ., ,$(UPSTREAM_VERSION)))
UPSTREAM=https://github.com/gcc-mirror/gcc/archive/gcc-$(subst .,_,$(UPSTREAM_VERSION))-release.tar.gz
TARBALL=$(notdir $(UPSTREAM))
PKGSRC_DIR=pkgsrc/lang/gcc$(UPSTREAM_MAJORVER)/patches
ARCH=$(shell $(HOST_CC) -dumpmachine)
TARGET_MACHINE=$(word 1,$(ARCH))--netbsd
NCORES=$(shell nproc)
INSTALL_DIR=$(RUMPRUN_PKGS_DIR)

export CC=$(RUMPRUN_CC)
export CXX=$(RUMPRUN_CXX)
export FC=$(RUMPRUN_FC)
export GFORTRAN=$(RUMPRUN_FC)


.PHONY: all
all: build build/libgfortran/.libs

dl/$(TARBALL):
	mkdir -p dl
	../scripts/fetch.sh ${UPSTREAM} dl/$(TARBALL)

build: | dl/$(TARBALL)
	mkdir -p build
	(cd build && tar -x --strip-components 1 -f ../dl/$(TARBALL))

patches/$(PKGSRC_DIR):
	(cd patches && cvs -q -z2 -d anoncvs@anoncvs.NetBSD.org:/cvsroot checkout -r HEAD -P $(PKGSRC_DIR))
	perl -pi -e 's/\@GCC_TARGET_MACHINE@/$(TARGET_MACHINE)/g' patches/$(PKGSRC_DIR)/patch-*

build/stamp_patch: build patches/*.patch | patches/$(PKGSRC_DIR)
	(cd patches/pkgsrc && ../../../scripts/apply-patches.sh ./ ../pkgsrc-patches/gcc$(UPSTREAM_MAJORVER)-*.patch)
	(cd build && ../../scripts/apply-patches.sh -p 0 ./ ../patches/$(PKGSRC_DIR)/patch-* && \
		 ../../scripts/apply-patches.sh ./ ../patches/*.patch)
	touch $@

build/%/Makefile: build/stamp_patch
	cp config.site build/$*/
	cd build/$*; chmod +x configure;\
	CONFIG_SITE=$(shell pwd)/build/$*/config.site LDFLAGS='$(LDFLAGS) -L$(INSTALL_DIR)/lib' \
	CFLAGS='$(CFLAGS) -I$(INSTALL_DIR)/lib/gcc/$(RUMPRUN_TOOLCHAIN_TUPLE)/$(UPSTREAM_VERSION)/include -fPIC' \
	./configure --prefix=$(INSTALL_DIR) \
		--disable-shared --disable-multilib --with-target-subdir="." \
		--host=$(RUMPRUN_TOOLCHAIN_TUPLE) 2>&1 | tee $(shell pwd)/config_$*.log

build/libbacktrace/.libs: build/libbacktrace/Makefile
	cd build/libbacktrace; \
	make -j$(NCORES) 2>&1 | tee $(shell pwd)/build_libbacktrace.log && \
	make install 2>&1 | tee -a $(shell pwd)/build_libbacktrace.log

build/libquadmath/.libs: build/libquadmath/Makefile
	cd build/libquadmath; \
	make -j$(NCORES) 2>&1 | tee $(shell pwd)/build_libquadmath.log && \
	make install 2>&1 | tee -a $(shell pwd)/build_libquadmath.log
	cp $(INSTALL_DIR)/lib/libquadmath.a $(RUMPRUN_SYSROOT)/lib/

build/libgfortran/.libs: build/libgfortran/Makefile build/libgcc/Makefile build/libbacktrace/.libs build/libquadmath/.libs
	cd build/libgfortran; \
	make -j$(NCORES) 2>&1 | tee $(shell pwd)/build_libgfortran.log && \
	make install 2>&1 | tee -a $(shell pwd)/build_libgfortran.log
	cp $(INSTALL_DIR)/lib/libgfortran.a $(RUMPRUN_SYSROOT)/lib/

.PHONY: clean
clean:
	-(cd build/libbacktrace; make uninstall)
	-(cd build/libquadmath; make uninstall)
	-(cd build/libgfortran; make uninstall)
	rm -f $(RUMPRUN_SYSROOT)/lib/libquadmath.a $(RUMPRUN_SYSROOT)/lib/libgfortran.a
	rm -rf build
	rm -rf patches/pkgsrc/*
