mkdir -p initramfs
cd initramfs
curl -O https://www.busybox.net/downloads/binaries/1.35.0-x86_64-linux-musl/busybox
chmod +x busybox
find busybox init.sh -print0 | cpio --null -H newc -o | gzip -9 > ../initramfs.gz
cd ..

# Steal Alpine's vmlinuz-virt kernel
curl -O https://dl-cdn.alpinelinux.org/alpine/v3.19/releases/x86_64/alpine-virt-3.19.1-x86_64.iso
yes | 7z e alpine-virt-3.19.1-x86_64.iso boot/vmlinuz-virt

# Copying from https://github.com/luizinhosuraty/pciemu/blob/master/setup.sh
REPOSITORY_DIR=$(git rev-parse --show-toplevel)
DEVICE_NAME=pcie-test

KCONFIG_FILE="qemu/hw/misc/Kconfig"
KCONFIG_LINE="source $DEVICE_NAME/Kconfig"
if ! grep -q "$KCONFIG_LINE" "$KCONFIG_FILE"; then
    echo "$KCONFIG_LINE" >> "$KCONFIG_FILE"
fi

MESON_FILE="qemu/hw/misc/meson.build"
MESON_LINE="subdir('$DEVICE_NAME')"
if ! grep -q "$MESON_LINE" "$MESON_FILE"; then
    echo "$MESON_LINE" >> "$MESON_FILE"
fi

# Create symbolic links to device files
ln -sf $REPOSITORY_DIR/src/hw/$DEVICE_NAME/ $REPOSITORY_DIR/qemu/hw/misc/

# Create symbolic link to the pciemu_hw.h include file
# This will avoid changing the meson files to be able to find this include
# ln -s $REPOSITORY_DIR/include/hw/pciemu_hw.h $REPOSITORY_DIR/src/hw/$DEVICE_NAME/pciemu_hw.h

cd qemu
./configure --target-list="x86_64-softmmu" --enable-debug
cd ..