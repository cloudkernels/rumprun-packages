include ../Makefile.inc

ifeq ($(RUMPRUN_WATCHDOG),)
all: bin/hello.spt bin/hello.hvt
else
all: watchdog bin/hello.spt bin/hello.hvt
endif

ifeq ($(RUMPRUN_WATCHDOG),)
bin/hello.spt: build/hello.spt
	mkdir -p bin
	cp $< $@

bin/hello.hvt: build/hello.hvt
	mkdir -p bin
	cp $< $@
else
bin/hello.spt: build/hello-rumprun
	mkdir -p bin
	rumprun-bake solo5_spt bin/hello.spt  $(RUMPRUN_PKGS_DIR)/lib/watchdog.o build/hello-rumprun

bin/hello.hvt: build/hello-rumprun
	mkdir -p bin
	rumprun-bake solo5_hvt bin/hello.hvt  $(RUMPRUN_PKGS_DIR)/lib/watchdog.o build/hello-rumprun

build/hello-rumprun: build/configure
	CC=${RUMPRUN_CC} $(MAKE) -C build hello-rumprun

endif

build/hello.spt: build/configure
	CC=${RUMPRUN_CC} $(MAKE) -C build hello.spt

build/hello.hvt: build/configure
	CC=${RUMPRUN_CC} $(MAKE) -C build hello.hvt

build/configure:
	mkdir -p build
	cp src/* build

.PHONY: clean
clean:
	-$(MAKE) -C build clean
	rm -rf bin build

include ../Makefile.deps
