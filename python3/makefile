include ../Makefile.inc


UPSTREAM=https://www.python.org/ftp/python/3.5.2/Python-3.5.2.tar.xz
TARBALL=$(notdir $(UPSTREAM))
ARCH=$(shell $(HOST_CC) -dumpmachine)
HOSTINCLUDE=$(shell dirname $(shell gcc -v 2>&1  | grep COLLECT_LTO_WRAPPER | cut -f2 -d'='))/include


all: libssl $(RUMPRUN_PKGS_DIR)/lib/libsqlite.a $(RUMPRUN_PKGS_DIR)/lib/libffi.a  images/python.iso

.NOTPARALLEL: $(RUMPRUN_PKGS_DIR)/lib/libsqlite.a
$(RUMPRUN_PKGS_DIR)/lib/libsqlite.a:
	$(MAKE) -C ../sqlite

.NOTPARALLEL: $(RUMPRUN_PKGS_DIR)/lib/libffi.a
$(RUMPRUN_PKGS_DIR)/lib/libffi.a:
	$(MAKE) -C ../libffi

build/python: build/Makefile libssl $(RUMPRUN_PKGS_DIR)/lib/libsqlite.a $(RUMPRUN_PKGS_DIR)/lib/libffi.a 
	$(MAKE) -C build
	$(MAKE) -C build install

PYTHON_CONF_ENV += \
	LDFLAGS="-static -static-libgcc -L${RUMPRUN_PKGS_DIR}/lib" \
	CPPFLAGS="$(CPPFLAGS) -static -I${RUMPRUN_PKGS_DIR}/include" \
	CFLAGS="$(CFLAGS) -static" \
	CC=$(RUMPRUN_CC) \
	PYTHON_FOR_BUILD=$(shell pwd)/build/hostdist/bin/python3 \
	CONFIG_SITE=config.site

PYTHON_CONF_OPTS += \
	--prefix=$(shell pwd)/build/pythondist \
	--disable-shared \
	--host=$(RUMPRUN_TOOLCHAIN_TUPLE) \
	--build $(ARCH) \
	--enable-ipv6 \
	--with-openssl=${RUMPRUN_PKGS_DIR} \
	--without-ensurepip

dl/$(TARBALL):
	mkdir -p dl
	../scripts/fetch.sh ${UPSTREAM} dl/$(TARBALL)

build/hostpython: | dl/$(TARBALL)
	mkdir -p build
	(cd build && tar -x --strip-components 1 -f ../dl/$(TARBALL))
	cp Setup.local build/Modules/
	(cd build; ./configure --prefix=$(shell pwd)/build/hostdist; make && make install)
	mv build/python build/hostpython

build/Parser/hostpgen: build/hostpython
	cp build/Parser/pgen build/Parser/hostpgen

build/Programs/host_freeze_importlib: build/Parser/hostpgen
	cp build/Programs/_freeze_importlib build/Programs/host_freeze_importlib

build/configure: build/Programs/host_freeze_importlib
	(cd build; mv hostdist ..; make distclean; mv ../hostdist .)

build/stamp_patch: build/configure patches/*
	cp config.site build/
	(cd build && ../../scripts/apply-patches.sh ./ ../patches/*)
	cp -R files/* build/Modules/
	touch $@
	cp -r build/ Python-3.5.2/

build/Makefile: build/stamp_patch
	(cd build; $(PYTHON_CONF_ENV) ./configure $(PYTHON_CONF_OPTS))

images/python.iso: build/python
	mkdir -p images
	$(RUMPRUN_GENISOIMAGE) -o images/python.iso build/pythondist/lib/python3.5

build_numpy: build/python
	wget https://github.com/numpy/numpy/archive/v1.7.2.tar.gz && tar -zxf v1.7.2.tar.gz
	(cd numpy-1.7.2; CC=$(RUMPRUN_CC) CFLAGS=-I$(HOSTINCLUDE)  ../build/hostdist/bin/python3 setup.py  install --prefix $(shell pwd)/build/pythondist/)

freeze_python: build_numpy
	rm -rf freezeOutput
	mkdir freezeOutput
	(cd freezeOutput; echo "import encodings.aliases" >> freezeInputScript.py; echo "import encodings.ascii" >> freezeInputScript.py; echo "import numpy" >> freezeInputScript.py)
	PYTHONHOME=$(shell pwd)/build/pythondist/  PYTHONPATH=$(shell pwd)/build/pythondist/lib/python3.5/site-packages/ CC=$(RUMPRUN_CC) CFLAGS=-I$(HOSTINCLUDE) build/hostdist/bin/python3 -S build/Tools/freeze/freeze.py -o freezeOutput -p $(shell pwd)/Python-3.5.2/ freezeOutput/freezeInputScript.py
	(cd freezeOutput; numberOfLines=$(shell cat frozen.c | wc -l);  numberOfLinesToKeep=$$(($$numberOfLines - 9)); head -n $$numberOfLinesToKeep frozen.c > frozen.h)

python.hvt: build/python
	rumprun-bake solo5_hvt python.hvt build/python

python.spt: build/python
	rumprun-bake solo5_spt python.spt build/python

### build hello world example ###
python_hello.iso: examples/hello.py
	rm -rf python
	mkdir -p python/lib/
	cp -r build/pythondist/lib/python3.5 python/lib/.
	cp examples/hello.py python/lib/python3.5/site-packages/.
	$(RUMPRUN_GENISOIMAGE) -o python_hello.iso python

### build requests example ###
python/lib/python3.5/site-packages/requests_main.py: requests_main.py
	rm -rf requests-env
	pyvenv-3.5 requests-env
	bash -c "source requests-env/bin/activate; pip install requests; deactivate"
	rm -rf python
	mkdir -p python/lib
	cp -r build/pythondist/lib/python3.5 python/lib/.
	cp -r requests-env/lib/python3.5/site-packages/* python/lib/python3.5/site-packages/.
	cp requests_main.py python/lib/python3.5/site-packages/.

python_requests.iso: python/lib/python3.5/site-packages/requests_main.py
	$(RUMPRUN_GENISOIMAGE) -o python_requests.iso python

.PHONY: clean
clean:
	-$(MAKE) -C build clean
	rm -f bin/*
	rm -f images/python.iso
	rm -f examples/hw.c examples/*.bin examples/hw
	rm -rf build
	rm -rf *.ukvm
	rm -rf *.bin
	rm -f *.fs

.PHONY: distclean
distclean: clean
	rm -rf build

include ../Makefile.deps
