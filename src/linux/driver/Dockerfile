FROM --platform=linux/amd64 alpine:3.19.1
RUN apk update
RUN apk add --no-cache build-base linux-headers linux-virt-dev kmod cpio

WORKDIR /build

COPY pcie_test.c .
COPY Makefile .

RUN make KERNELDIR=/usr/src/linux-headers-6.6.76-0-virt
