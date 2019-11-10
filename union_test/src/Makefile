default: union_test

union_test.o: union_test.c 
	gcc -c -ggdb union_test.c -o union_test.o 

union_test: union_test.o 
	gcc -ggdb union_test.o -o union_test 

union_test-rumprun: union_test.c
	${CC} -ggdb union_test.c -o union_test-rumprun

union_test.spt: union_test-rumprun
	rumprun-bake solo5_spt union_test.spt union_test-rumprun

union_test.hvt: union_test-rumprun
	rumprun-bake solo5_hvt union_test.hvt union_test-rumprun

clean:
	-rm -f union_test.o 
	-rm -f union_test
	-rm -f union_test-rumprun
	-rm -f union_test.spt
	-rm -f union_test.hvt
