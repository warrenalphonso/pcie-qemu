#!/busybox sh
/busybox mkdir /sys
/busybox mkdir /proc
/busybox mount -t proc null /proc
/busybox mount -t sysfs null /sys
/busybox mknod /dev/mem c 1 1
/lspci -vv
exec /busybox sh
