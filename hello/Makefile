include ../Makefile.inc

all: bin/hello-rumprun

bin/hello-rumprun: build/hello-rumprun
	mkdir -p bin
	cp $< $@

build/hello-rumprun: build/configure
	CC=${RUMPRUN_CC} $(MAKE) -C build hello.spt

build/configure:
	mkdir -p build
	cp src/* build

.PHONY: clean
clean:
	-$(MAKE) -C build clean
	rm -rf bin build

