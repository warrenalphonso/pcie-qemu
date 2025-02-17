cd src/linux/pciutils
docker build -t pciutils .
docker create --name pciutils pciutils
docker cp pciutils:/build/pciutils-master/lspci ../
docker rm pciutils
docker image rm pciutils
cd ../../..

cd src/linux/driver
docker build -t driver .
docker create --name driver driver
docker cp driver:/build/pcie_test.ko ../
docker rm driver
docker image rm driver
cd ../../..

cd src/linux
curl -O https://www.busybox.net/downloads/binaries/1.35.0-x86_64-linux-musl/busybox
chmod +x busybox
find init.sh busybox lspci pcie_test.ko -print0 | cpio --null -H newc -o | gzip -9 > initramfs.gz
cd ../..

# Steal Alpine's vmlinuz-virt kernel
cd src/linux/kernel
docker build -t kernel .
docker create --name kernel kernel
docker cp kernel:/boot/vmlinuz-virt ../vmlinuz-virt
docker rm kernel
docker image rm kernel
cd ../../..

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
ln -sf $REPOSITORY_DIR/src/qemu/hw/misc/$DEVICE_NAME/ $REPOSITORY_DIR/qemu/hw/misc/

cd qemu
./configure --target-list="x86_64-softmmu" --enable-debug
cd ..