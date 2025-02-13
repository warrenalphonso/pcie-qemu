From: https://blog.davidv.dev/posts/learning-pcie/

cd initramfs
curl -LO https://www.busybox.net/downloads/binaries/1.35.0-x86_64-linux-musl/busybox
chmod +x busybox
find busybox init.sh -print0 | cpio --null -H newc -o | gzip -9 > ../initramfs.gz

can list with 7z l alpine-virt-3.19.1-x86_64.iso 

now extract:
7z e alpine-virt-3.19.1-x86_64.iso boot/vmlinuz-virt

Now this shoudl at least run, will hit some file system error: 
./qemu/build/qemu-system-x86_64 -kernel vmlinuz-virt -m 256 -chardev stdio,id=char0 -serial chardev:char0 -append 'console=ttyS0 quiet panic=-1'

[    3.518361] Kernel panic - not syncing: VFS: Unable to mount root fs on unknown-block(0,0)
[    3.520939] CPU: 0 PID: 1 Comm: swapper/0 Not tainted 6.6.14-0-virt #1-Alpine
[    3.521301] Hardware name: QEMU Standard PC (i440FX + PIIX, 1996), BIOS rel-1.16.3-0-ga6ed6b701f0a-prebuilt.qemu.org 04/01/2014
[    3.522036] Call Trace:
[    3.522340]  <TASK>
[    3.523307]  dump_stack_lvl+0x47/0x70
[    3.524492]  panic+0x17b/0x330
[    3.524624]  mount_root_generic+0x234/0x4d0
[    3.524827]  prepare_namespace+0xc9/0x4a0
[    3.525002]  kernel_init_freeable+0x510/0x5d0
[    3.525235]  ? __pfx_kernel_init+0x10/0x10
[    3.525359]  kernel_init+0x15/0x1c0
[    3.525458]  ret_from_fork+0x2f/0x50
[    3.525574]  ? __pfx_kernel_init+0x10/0x10
[    3.525686]  ret_from_fork_asm+0x1b/0x30
[    3.525962]  </TASK>
[    3.527456] Kernel Offset: 0x19000000 from 0xffffffff81000000 (relocation range: 0xffffffff80000000-0xffffffffbfffffff)
qemu-system-x86_64: terminating on signal 2 from pid 3349 (<unknown process>)

./qemu/build/qemu-system-x86_64 -kernel vmlinuz-virt -initrd initramfs.gz -m 256 -chardev stdio,id=char0 -serial chardev:char0 -append 'console=ttyS0 rdinit=/init.sh quiet'