default: chttp

chttp.o: chttp.c 
	gcc -c chttp.c -o chttp.o 

chttp: chttp.o 
	gcc chttp.o -o chttp 

chttp-rumprun: chttp.c
	${CC} chttp.c -o chttp-rumprun

chttp.spt: chttp-rumprun
	rumprun-bake solo5_spt chttp.spt chttp-rumprun

chttp.hvt: chttp-rumprun
	rumprun-bake solo5_hvt chttp.hvt chttp-rumprun

clean:
	-rm -f chttp.o 
	-rm -f chttp
	-rm -f chttp-rumprun
	-rm -f chttp.spt
	-rm -f chttp.hvt
