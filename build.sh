cd qemu
make -j8
cd ..

./qemu/build/qemu-system-x86_64 \
    -kernel src/linux/vmlinuz-virt \
    -initrd src/linux/initramfs.gz \
    -m 256 -chardev stdio,id=char0 -serial chardev:char0 \
    -append 'console=ttyS0 rdinit=/init.sh quiet' \
    -device gpu