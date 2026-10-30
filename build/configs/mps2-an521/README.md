# mps2-an521

TizenRT on the QEMU `mps2-an521` machine (Arm MPS2 FPGA image AN521,
Cortex-M33) with a CMSDK UART console.  The board comes from
`boards/arm/mps/mps2-an521` of NuttX 12.13.0 (`os/board/mps2-an521`,
`os/arch/arm/src/mps`).  FLAT build only.

## Configurations

| Config | What it does |
|---|---|
| `ostest` | Runs the NuttX OS test (`apps/examples/testcase/ostest`) as the entry point (`CONFIG_INIT_ENTRYPOINT="ostest_main"`). |
| `tash` | Starts TASH (`CONFIG_INIT_ENTRYPOINT="preapp_start"`, `apps/system/init`) with the FS commands and the `ostest` command. |

The board has no `board_power_off()` in NuttX 12.13.0, so QEMU keeps
running after the test.  The OS options follow the NuttX `nsh`
configuration of the board: there is no FPU test and no priority
inheritance test.

The CMSDK UART0 interrupts are TX 48, RX 49 and overflow 63, as in
NuttX 12.13.0; checked with QEMU 8.2.

## Build

The `tizenrt/tizenrt:2.0.1-qemu` docker image adds QEMU to the TizenRT
build image (see `tools/docker`).  `CONFIG_DOCKER_VERSION="2.0.1-qemu"`
makes `dbuild.sh` use it.

```bash
docker build -t tizenrt/tizenrt:2.0.1-qemu tools/docker
cd os
./tools/configure.sh mps2-an521/ostest    # or mps2-an521/tash
./dbuild.sh build
```

## Run

```bash
docker run --rm -it -v $PWD:/root/tizenrt tizenrt/tizenrt:2.0.1-qemu \
	/root/tizenrt/build/configs/mps2-an521/qemu_run.sh
```

`qemu_run.sh` runs:

```bash
qemu-system-arm -M mps2-an521 -nographic \
	-chardev stdio,id=con,mux=on -serial chardev:con \
	-mon chardev=con,mode=readline -kernel build/output/bin/tinyara
```

The test passes when the log shows

```
user_main: Exiting
ostest_main: Exiting with status 0
```

With the `tash` configuration, type `ostest` at the `TASH>>` prompt.
Quit QEMU with `Ctrl-a x`.
