include ../Makefile.inc

all: bin/chttp-rumprun

bin/chttp-rumprun: build/chttp-rumprun
	mkdir -p bin
	cp $< $@

build/chttp-rumprun: build/configure
	CC=${RUMPRUN_CC} $(MAKE) -C build chttp.spt

build/configure:
	mkdir -p build
	cp src/* build

.PHONY: clean
clean:
	-$(MAKE) -C build clean
	rm -rf bin build

