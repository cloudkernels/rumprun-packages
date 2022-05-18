FROM debian:buster as builder

# install prerequisites
RUN apt-get update && apt-get install -y build-essential git \
	libz-dev bin86 bison flex wget bc libelf-dev libssl-dev \
	init udev kmod genisoimage gcc-7 libseccomp-dev && apt-get clean && \
	rm /usr/bin/gcc /usr/bin/g++ && \
	ln -s /usr/bin/gcc-7 /usr/bin/gcc && \
	ln -s /usr/bin/g++-7 /usr/bin/g++

# build rumprun
RUN git clone https://github.com/cloudkernels/rumprun.git -b watchdog && \
	git clone https://github.com/cloudkernels/rumprun-packages.git -b watchdog && \
	cd rumprun && \
	git submodule update --init && \
	make && \
	export PATH=$PATH:${PWD}/rumprun-solo5/bin/ && \
	cd /rumprun-packages && \
	cp config.mk.dist config.mk && \
	cd hello && \
	make
	
FROM scratch as artifacts
COPY --from=builder /rumprun/solo5/tenders/hvt/solo5-hvt /
COPY --from=builder /rumprun-packages/hello/bin/hello.hvt /
