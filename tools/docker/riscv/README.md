# TizenRT build image for the RISC-V boards

`Dockerfile` extends the public `tizenrt/tizenrt:2.0.1` image with what
the NuttX-based RISC-V boards need:

| Board            | Needs                                               |
|------------------|-----------------------------------------------------|
| rv-virt          | RISC-V toolchain, `qemu-system-riscv32 -M virt`     |
| esp32c6-devkitc  | RISC-V toolchain, esptool (flash image)             |

- **Toolchain:** Espressif `riscv32-esp-elf` GCC 14.2.0
  (`esp-14.2.0_20241119`) in `/opt/riscv32-esp-elf`.  The base image is
  Ubuntu 16.04 with glibc 2.23; the xPack `riscv-none-elf-gcc` that the
  NuttX documentation recommends needs glibc 2.27 and does not run there.
  The Espressif toolchain has the `rv32imac_zicsr_zifencei/ilp32`
  multilib that both boards use.  The board `Make.defs` set
  `CROSSDEV = riscv32-esp-elf-`.
- **QEMU:** `qemu-system-riscv32` 8.2, built statically from the Ubuntu
  24.04 source package in a first stage and copied to `/opt/qemu`, like
  `tools/docker/Dockerfile` does for `qemu-system-arm`.  It has the
  `virt` machine only; the `esp32c6` machine is in the QEMU of Espressif
  (see `build/configs/esp32c6-devkitc`).
- **esptool** 4.8.1 with pyserial and intelhex.  The Python 3.8 of the
  base image has no `ssl` module, so the wheels are fetched in a build
  stage and installed without the network.

## Build

```bash
docker build -t tizenrt/tizenrt:2.0.1-qemu-riscv tools/docker/riscv
```

Behind a proxy that re-signs TLS, pass the proxy and the CA bundle of the
host:

```bash
docker build --build-arg http_proxy --build-arg https_proxy \
	--secret id=ca_bundle,src=/etc/ssl/certs/ca-certificates.crt \
	-t tizenrt/tizenrt:2.0.1-qemu-riscv tools/docker/riscv
```

## Use

`os/dbuild.sh` picks a local image when its tag matches
`CONFIG_DOCKER_VERSION` of the configuration, so the rv-virt
configurations set:

```
CONFIG_DOCKER_VERSION="2.0.1-qemu-riscv"
```

QEMU can also be run directly in the image:

```bash
docker run --rm -it -v $PWD:/root/tizenrt -w /root/tizenrt tizenrt/tizenrt:2.0.1-qemu-riscv \
	build/configs/rv-virt/qemu_run.sh
```
