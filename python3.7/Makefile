PYTHON_VER=python3.7

include Makefile.common

PYTHON_UPSTREAM=https://www.python.org/ftp/python/3.7.5/Python-3.7.5.tar.xz
PYTHON_TARBALL=$(notdir $(PYTHON_UPSTREAM))
CYTHON_UPSTREAM=https://github.com/cython/cython/archive/0.29.13.tar.gz
CYTHON_TARBALL=$(notdir $(CYTHON_UPSTREAM))
ARCH=$(shell $(HOST_CC) -dumpmachine)

.PHONY: all
all: $(INSTALL_DIR) $(INSTALL_HOSTDIR)

.NOTPARALLEL: $(RUMPRUN_PKGS_DIR)/lib/libsqlite.a
$(RUMPRUN_PKGS_DIR)/lib/libsqlite.a:
	$(MAKE) -C ../sqlite

.NOTPARALLEL: $(RUMPRUN_PKGS_DIR)/lib/libffi.a
$(RUMPRUN_PKGS_DIR)/lib/libffi.a:
	$(MAKE) -C ../libffi

dl/$(PYTHON_TARBALL):
	mkdir -p dl
	../scripts/fetch.sh $(PYTHON_UPSTREAM) dl/$(PYTHON_TARBALL)

dl/$(CYTHON_TARBALL):
	mkdir -p dl
	../scripts/fetch.sh $(CYTHON_UPSTREAM) dl/$(CYTHON_TARBALL)

build_host: | dl/$(PYTHON_TARBALL)
	mkdir -p build_host
	(cd build_host && tar -x --strip-components 1 -f ../dl/$(PYTHON_TARBALL))

build_host/stamp_patch: config.site | build_host patches/host/*
	cp config.site build_host/
	(cd build_host && ../../scripts/apply-patches.sh ./ ../patches/host/*)
	touch $@

$(INSTALL_HOSTDIR): Setup.local.host | build_host/stamp_patch
	cp Setup.local.host build_host/Modules/Setup.local
	(cd build_host; \
		CPPFLAGS='' LDFLAGS='' ./configure --with-ensurepip=install --prefix=$(PYTHON_HOSTDIST) 2>&1 |tee $(shell pwd)/build_host.log; \
		CPPFLAGS='' LDFLAGS='' make -j$(NCORES) python Parser/pgen Programs/_freeze_importlib 2>&1 |tee -a $(shell pwd)/build_host.log && \
		CPPFLAGS='' LDFLAGS='' make install 2>&1 |tee -a $(shell pwd)/build_host.log)

build_cython/build: build_host/python | dl/$(CYTHON_TARBALL)
	mkdir -p build_cython
	(cd build_cython && tar -x --strip-components 1 -f ../dl/$(CYTHON_TARBALL); \
		CPPFLAGS='' LDFLAGS='' $(PYTHON_VER) setup.py install --prefix=$(PYTHON_HOSTDIST) 2>&1 |tee $(shell pwd)/build_cython.log)

PYTHON_CONF_ENV += \
	LDFLAGS="-static -static-libgcc -L$(RUMPRUN_PKGS_DIR)/lib" \
	CPPFLAGS="$(CPPFLAGS) -static -I$(RUMPRUN_PKGS_DIR)/include" \
	CFLAGS="$(CFLAGS) -static" \
	CC=$(RUMPRUN_CC) \
	PATH=$(PYTHON_HOSTDIST)/bin:$$PATH \
	CONFIG_SITE=config.site

PYTHON_CONF_OPTS += \
	--prefix=$(PYTHON_DIST) \
	--disable-shared \
	--host=$(RUMPRUN_TOOLCHAIN_TUPLE) \
	--build $(ARCH) \
	--enable-ipv6 \
	--with-openssl=$(RUMPRUN_PKGS_DIR) \
	--without-ensurepip

build: $(INSTALL_HOSTDIR) build_cython/build
	mkdir -p build
	(cd build && tar -x --strip-components 1 -f ../dl/$(PYTHON_TARBALL))

build/Parser/hostpgen: build_host build
	cp build_host/Parser/pgen build/Parser/hostpgen

build/Programs/host_freeze_importlib: build_host build
	cp build_host/Programs/_freeze_importlib build/Programs/host_freeze_importlib

build/stamp_patch: config.site | build/Programs/host_freeze_importlib build/Parser/hostpgen patches/dist*
	cp config.site build/
	(cd build && ../../scripts/apply-patches.sh ./ ../patches/dist/*)
	touch $@

build/Makefile: Setup.local.dist | build/stamp_patch
	cp Setup.local.dist build/Modules/Setup.local
	(cd build; $(PYTHON_CONF_ENV) ./configure $(PYTHON_CONF_OPTS) 2>&1 |tee $(shell pwd)/build.log)

$(INSTALL_DIR): build/Makefile libssl $(RUMPRUN_PKGS_DIR)/lib/libsqlite.a $(RUMPRUN_PKGS_DIR)/lib/libffi.a 
	$(MAKE) -C build 2>&1 |tee -a $(shell pwd)/build.log; \
	$(MAKE) -C build install 2>&1 |tee -a $(shell pwd)/build.log

freeze_python/frozen.c: $(INSTALL_DIR)
	rm -rf freeze_python
	mkdir freeze_python
	(cd freeze_python; echo "import encodings.aliases\nimport encodings.ascii" >> freezeInputScript.py)
	PYTHONHOME=$(PYTHON_DIST) PYTHONPATH=$(PYTHON_DIST)/lib/$(PYTHON_VER)/site-packages CC=$(RUMPRUN_CC) \
	$(PYTHON_VER) -S $(PYTHON_SRC)/Tools/freeze/freeze.py -o freeze_python -p $(PYTHON_SRC) freeze_python/freezeInputScript.py
	(cd freeze_python; numberOfLines=$$(cat frozen.c | wc -l); \
		numberOfLinesToKeep=$$(($$numberOfLines - 9)); head -n $$numberOfLinesToKeep frozen.c > frozen.h)

$(EXAMPLES_DIR)/pythonRun: freeze_python/frozen.c
	(cd $(EXAMPLES_DIR);\
	frozenSources="$(shell find $(shell pwd)/freeze_python/ -name M_\*.c)";\
	objects="";\
	$(RUMPRUN_CC) -g -Xlinker -export-dynamic -o pythonRun pythonRun.c -I$(PYTHON_DIST)/include/$(PYTHON_VER)m \
		-L$(RUMPRUN_PKGS_DIR)/lib -I$(RUMPRUN_PKGS_DIR)/include -I$(shell pwd)/freeze_python $$frozenSources $$objects \
		$(PYTHON_DIST)/lib/lib$(PYTHON_VER)m.a -lpthread -lm -lutil -lsqlite3 -lz -lssl -ltls -lcrypto -lffi)
	touch $@

$(EXAMPLES_DIR)/pythonRun.%t: $(EXAMPLES_DIR)/pythonRun
	cd $(EXAMPLES_DIR);\
	rumprun-bake solo5_$*t pythonRun.$*t pythonRun

$(EXAMPLES_DIR)/fs.iso: $(EXAMPLES_DIR)/*.py
	cd $(EXAMPLES_DIR);\
	rm -rf fs; mkdir -p fs/lib;\
	cp *.py fs/;\
	cp -r $(PYTHON_DIST)/lib/$(PYTHON_VER) fs/lib/;\
	$(RUMPRUN_GENISOIMAGE) -o fs.iso fs

.PHONY: run_python
run_python: $(EXAMPLES_DIR)/pythonRun.spt $(EXAMPLES_DIR)/pythonRun.hvt $(EXAMPLES_DIR)/fs.iso
	cd $(EXAMPLES_DIR);\
	$(SOLO5_SPT) --disk=fs.iso --net=tap100 pythonRun.spt \
		'{"cmdline":"./pythonRun.spt /python/hello.py","env":"PYTHONPATH=/python/lib/$(PYTHON_VER)/site-packages","blk":{"source":"etfs","path":"ld0a","fstype":"blk","mountpoint":"/python"}}';\
	$(SOLO5_SPT) --disk=fs.iso --net=tap100 pythonRun.spt \
		'{"cmdline":"./pythonRun.spt /python/testPython.py","env":"PYTHONPATH=/python/lib/$(PYTHON_VER)/site-packages","blk":{"source":"etfs","path":"ld0a","fstype":"blk","mountpoint":"/python"}}'

.PHONY: clean
clean:
	(cd $(EXAMPLES_DIR); rm -rf *.spt *.hvt fs fs.iso pythonRun)
	rm -rf build build_host
	rm -rf build_cython freeze_python
	rm -rf $(INSTALL_DIR) $(INSTALL_HOSTDIR)

include ../Makefile.deps
