include ../Makefile.inc

all: build/watchdog.o
	cp build/watchdog.o $(RUMPRUN_PKGS_DIR)/lib

build/watchdog.o: build/configure
	AR=$(RUMPRUN_AR) CC=${RUMPRUN_CC} $(MAKE) -C build watchdog.o

build/configure:
	mkdir -p build
	cp src/* build

.PHONY: clean
clean:
	-$(MAKE) -C build clean
	rm -rf build

