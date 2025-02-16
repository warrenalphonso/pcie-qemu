We want the full `lspci` script in our VM. We use busybox for most
utilities, but their version of `lspci` (`/busybox lspci`) is too
minimal. 

Instead, we'll statically build `lspci` and include it in `initramfz.gz`. 

```bash
docker build -t lspci-static-build .
docker run --name lspci-static-build lspci-static-build
docker cp lspci-static-build:/build/pciutils-master/lspci ../initramfs/lspci
docker rm lspci-static-build 
docker image rm lspci-static-build
```