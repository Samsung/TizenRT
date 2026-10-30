# qemu-armv7a

TizenRT on the QEMU `virt` machine with a Cortex-A7 CPU, a GICv2 and a
PL011 UART console.  The board comes from
`boards/arm/qemu/qemu-armv7a` of NuttX 12.13.0
(`os/board/qemu-armv7a`, `os/arch/arm/src/qemu`).  FLAT build only.

## Configurations

| Config | What it does |
|---|---|
| `ostest` | Runs the NuttX OS test (`apps/examples/testcase/ostest`) as the entry point (`CONFIG_INIT_ENTRYPOINT="ostest_main"`) and powers QEMU off with the test result. |
| `tash` | Starts TASH (`CONFIG_INIT_ENTRYPOINT="preapp_start"`, `apps/system/init`) with the FS commands, the `ostest` command and the `smp` command (`apps/examples/smp`). |

Both configurations use SMP with 2 CPUs (`CONFIG_SMP=y`,
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
./tools/configure.sh qemu-armv7a/ostest    # or qemu-armv7a/tash
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
