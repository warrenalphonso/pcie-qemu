FROM --platform=linux/amd64 alpine:3.19.1
RUN apk add --no-cache build-base linux-headers pciutils-dev
WORKDIR /build

RUN wget https://github.com/pciutils/pciutils/archive/master.tar.gz 
RUN tar xf master.tar.gz

WORKDIR /build/pciutils-master
RUN make ZLIB=no DNS=no SHARED=no LIBKMOD=no HWDB=no STATIC=yes LDFLAGS="-static"