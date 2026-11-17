# qemu-armv7a

TizenRT on the QEMU `virt` machine with a Cortex-A7 CPU, a GICv2 and a
PL011 UART console.  The board comes from
`boards/arm/qemu/qemu-armv7a` of NuttX 12.13.0
(`os/board/qemu-armv7a`, `os/arch/arm/src/qemu`).  FLAT build only.

## Configurations

| Config | What it does |
|---|---|
| `ostest` | Runs the NuttX OS test (`apps/examples/testcase/ostest`) as the entry point (`CONFIG_INIT_ENTRYPOINT="ostest_main"`) and powers QEMU off with the test result. |
| `tash` | Starts TASH (`CONFIG_INIT_ENTRYPOINT="preapp_start"`, `apps/system/init`) with the FS commands, the `ostest` command, the `smp` command (`apps/examples/smp`) and the `helloxx` command (`apps/examples/helloxx`, toolchain C++ library with exceptions). |
| `net` | `tash` with the network stack (`CONFIG_NET`, lwIP with netmgr): loopback and virtio-net interfaces, DHCP client and `ifconfig`/`ping`. |

All configurations use SMP with 2 CPUs (`CONFIG_SMP=y`,
`CONFIG_SMP_NCPUS=2`), like `rtl8730e/loadable_ext_*`.  CPU0 starts
CPU1 with PSCI `CPU_ON` (`up_cpu_start()` in `qemu_boot.c`).  With SMP,
ostest also runs its `smp_call` test.

## Build

The `tizenrt/tizenrt:2.0.1-qemu` docker image adds QEMU to the TizenRT
build image (see `tools/docker`).  `CONFIG_DOCKER_VERSION="2.0.1-qemu"`
makes `dbuild.sh` use it.

```bash
docker build -t tizenrt/tizenrt:2.0.1-qemu tools/docker
cd os
./tools/configure.sh qemu-armv7a/ostest    # or qemu-armv7a/tash, qemu-armv7a/net
./dbuild.sh build
```

## Run

```bash
docker run --rm -it -v $PWD:/root/tizenrt tizenrt/tizenrt:2.0.1-qemu \
	/root/tizenrt/build/configs/qemu-armv7a/qemu_run.sh
```

`qemu_run.sh` runs:

```bash
qemu-system-arm -cpu cortex-a7 -smp 2 -nographic \
	-machine virt,virtualization=off,gic-version=2 -net none \
	-chardev stdio,id=con,mux=on -serial chardev:con \
	-mon chardev=con,mode=readline -kernel build/output/bin/tinyara
```

`-smp` is `CONFIG_SMP_NCPUS` of `os/.config` (1 without SMP).

The `ostest` configuration passes when the log ends with

```
user_main: Exiting
ostest_main: Exiting with status 0
```

and QEMU exits (`boardctl(BOARDIOC_POWEROFF)`).

With the `tash` configuration, type `ostest` at the `TASH>>` prompt; the
same lines show the result.  `smp` runs the SMP example (barrier threads
on both CPUs).  Quit QEMU with `Ctrl-a x`.

## Network

The `net` configuration registers two interfaces: `lo` (lwIP loopback)
and `eth0`, a virtio-net device on the virtio-mmio transport of the
`virt` machine (`os/arch/arm/src/qemu/qemu_virtio_net.c`,
`CONFIG_QEMU_VIRTIO_NET`).  The driver scans the 32 virtio-mmio slots
from 0x0a000000 and uses the first network device.  Give QEMU a NIC with
the extra arguments of `qemu_run.sh`:

```bash
docker run --rm -it -v $PWD:/root/tizenrt tizenrt/tizenrt:2.0.1-qemu \
	/root/tizenrt/build/configs/qemu-armv7a/qemu_run.sh \
	-netdev user,id=net0 \
	-device virtio-net-device,netdev=net0
```

Without these arguments only `lo` is registered.

At the `TASH>>` prompt:

```
ifconfig                          # lo and eth0
ifup eth0
ifconfig eth0 dhcp                # 10.0.2.15 from the QEMU user network
ping -c 4 10.0.2.2                # the host
```

10.0.2.2 is the machine that runs QEMU, which is the container with the
command above.
