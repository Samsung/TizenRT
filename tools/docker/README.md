# TizenRT build image with QEMU

`Dockerfile` extends the public `tizenrt/tizenrt:2.0.1` image with
`qemu-system-arm` 8.2, which is needed to run the NuttX-based QEMU boards:

| Board        | QEMU machine                  | CPU        |
|--------------|-------------------------------|------------|
| qemu-armv7a  | `-M virt -cpu cortex-a7`      | Cortex-A7  |
| mps2-an521   | `-M mps2-an521`               | Cortex-M33 |

The QEMU of the base image (Ubuntu 16.04) is too old for these machines, so
QEMU is built statically from the Ubuntu 24.04 source package in a first
stage and copied to `/opt/qemu` of the final image.  The toolchain of the
base image (arm-none-eabi-gcc 10.3.1) is used as it is.

## Build

```bash
docker build -t tizenrt/tizenrt:2.0.1-qemu tools/docker
```

## Use

`os/dbuild.sh` picks a local image when its tag matches
`CONFIG_DOCKER_VERSION` of the configuration, so the qemu-armv7a and
mps2-an521 configurations set:

```
CONFIG_DOCKER_VERSION="2.0.1-qemu"
```

QEMU can also be run directly in the image:

```bash
docker run --rm -it -v $PWD:/root/tizenrt -w /root/tizenrt tizenrt/tizenrt:2.0.1-qemu \
	qemu-system-arm -M mps2-an521 -nographic -kernel build/output/bin/tinyara
```
