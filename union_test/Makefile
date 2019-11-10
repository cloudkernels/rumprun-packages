include ../Makefile.inc

all: bin/union_test.spt bin/union_test.hvt

bin/union_test.spt: build/union_test.spt
	mkdir -p bin
	cp $< $@

bin/union_test.hvt: build/union_test.hvt
	mkdir -p bin
	cp $< $@

build/union_test.spt: build/configure
	CC=${RUMPRUN_CC} $(MAKE) -C build union_test.spt

build/union_test.hvt: build/configure
	CC=${RUMPRUN_CC} $(MAKE) -C build union_test.hvt

build/configure:
	mkdir -p build
	cp src/* build

.PHONY: clean
clean:
	-$(MAKE) -C build clean
	rm -rf bin build

