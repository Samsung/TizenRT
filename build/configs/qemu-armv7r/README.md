# qemu-armv7r

TizenRT on the QEMU `virt` machine with a Cortex-R5F CPU, a GICv2 and a
PL011 UART console.  The board comes from `boards/arm/qemu/qemu-armv7r`
of NuttX 12.13.0 (`os/board/qemu-armv7r`); it uses the NuttX `qemu`
chip (`os/arch/arm/src/qemu`, `ARCH_CHIP_QEMU_CORTEXR5`) and the
`armv7-r` core.  FLAT build only.

## QEMU

Upstream QEMU accepts only A-profile CPUs on `virt`
(`qemu-system-arm -M virt -cpu cortex-r5f` fails with "CPU type
cortex-r5f-arm-cpu not supported").  As NuttX (see the NuttX
qemu-armv7r documentation), this board runs on the OpenVela QEMU fork
(`open-vela/platform_external_qemu`), which adds Cortex-R support to
`virt`.

`Dockerfile` in this directory builds the fork (QEMU 10.0.0 based,
fixed commit `df9bdfe3`) statically for `arm-softmmu` and adds it as
`/opt/qemu-vela` to `tizenrt/tizenrt:2.0.1-qemu` (see `tools/docker`).
The upstream QEMU 8.2 of that image stays the default
`qemu-system-arm`.

## Configurations

| Config | What it does |
|---|---|
| `ostest` | Runs the NuttX OS test (`apps/examples/testcase/ostest`) as the entry point (`CONFIG_INIT_ENTRYPOINT="ostest_main"`) and powers QEMU off with the test result (semihosting, `ARM_SEMIHOSTING_POWEROFF`). |
| `tash` | Starts TASH (`CONFIG_INIT_ENTRYPOINT="preapp_start"`, `apps/system/init`) with the system and FS commands and the `ostest` command. |

One CPU (Cortex-R5 has no SMP on this board).  The system tick is the
Arm generic timer (`ARMV7R_HAVE_PTM`, `CONFIG_ALARM_ARCH`).

## Build

The build uses `tizenrt/tizenrt:2.0.1-qemu`
(`CONFIG_DOCKER_VERSION="2.0.1-qemu"`); only running needs the OpenVela
image.

```bash
docker build -t tizenrt/tizenrt:2.0.1-qemu tools/docker
docker build -t tizenrt/tizenrt:2.0.1-qemu-armv7r build/configs/qemu-armv7r
cd os
./tools/configure.sh qemu-armv7r/ostest    # or qemu-armv7r/tash
./dbuild.sh build
```

## Run

```bash
docker run --rm -it -v $PWD:/root/tizenrt tizenrt/tizenrt:2.0.1-qemu-armv7r \
	/root/tizenrt/build/configs/qemu-armv7r/qemu_run.sh
```

`qemu_run.sh` runs:

```bash
/opt/qemu-vela/bin/qemu-system-arm -M virt -cpu cortex-r5f -semihosting \
	-nographic -net none \
	-chardev stdio,id=con,mux=on -serial chardev:con \
	-mon chardev=con,mode=readline -kernel build/output/bin/tinyara
```

`QEMU_VELA` selects another installation of the fork.

The `ostest` configuration passes when the log ends with

```
user_main: Exiting
ostest_main: Exiting with status 0
```

and QEMU exits.  With the `tash` configuration, type `ostest` at the
`TASH>>` prompt.  Quit QEMU with `Ctrl-a x`.
