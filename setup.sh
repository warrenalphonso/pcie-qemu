cd qemu
./configure --target-list="x86_64-softmmu" --enable-debug
cd ..

mkdir initramfs
cd initramfs
curl -O https://www.busybox.net/downloads/binaries/1.35.0-x86_64-linux-musl/busybox
chmod +x busybox
find busybox init.sh -print0 | cpio --null -H newc -o | gzip -9 > ../initramfs.gz
cd ..

# Steal Alpine's vmlinuz-virt kernel
curl -O https://dl-cdn.alpinelinux.org/alpine/v3.19/releases/x86_64/alpine-virt-3.19.1-x86_64.iso
7z e alpine-virt-3.19.1-x86_64.iso boot/vmlinuz-virt