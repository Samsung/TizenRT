# lm3s6965-ek

TizenRT on the QEMU `lm3s6965evb` machine: TI Stellaris LM3S6965
evaluation board (Cortex-M3, 256 KB flash, 64 KB SRAM), UART0 console.
The chip and the board come from NuttX 12.13.0 (`arch/arm/src/tiva`,
`boards/arm/tiva/lm3s6965-ek`; `os/arch/arm/src/tiva`,
`os/board/lm3s6965-ek`); they use the `armv7-m` core.  FLAT build only.
The board is used only on QEMU.

This directory was `build/configs/qemu` before the NuttX 12.13.0 port.

## Configurations

| Config | What it does |
|---|---|
| `ostest` | Runs the NuttX OS test (`apps/examples/testcase/ostest`) as the entry point (`CONFIG_INIT_ENTRYPOINT="ostest_main"`). |
| `tash` | Starts TASH (`CONFIG_INIT_ENTRYPOINT="preapp_start"`, `apps/system/init`) with the system and FS commands and the `ostest` command. |

Both use the memory of the real board, which upstream QEMU emulates:
256 KB flash and 64 KB SRAM.  To fit, they are built with
`CONFIG_DEBUG_FULLOPT=y` (`-Os`, as the NuttX lm3s6965-ek
configurations) and with the default stack sizes of NuttX (2 KB task
stacks, 1 KB IDLE stack, one high priority work queue thread).  In
`tash`, `CONFIG_TESTING_OSTEST_STACKSIZE=4096`: with TASH running, the
8 KB stacks of the ostest tasks do not fit in the heap.

The board has no `board_power_off()`, so QEMU keeps running after the
test.  QEMU prints "Timer with period zero, disabling" at boot (its
timer model); this has no effect.

## Build

The `tizenrt/tizenrt:2.0.1-qemu` docker image adds QEMU to the TizenRT
build image (see `tools/docker`).  `CONFIG_DOCKER_VERSION="2.0.1-qemu"`
makes `dbuild.sh` use it.

```bash
docker build -t tizenrt/tizenrt:2.0.1-qemu tools/docker
cd os
./tools/configure.sh lm3s6965-ek/ostest    # or lm3s6965-ek/tash
./dbuild.sh build
```

## Run

```bash
docker run --rm -it -v $PWD:/root/tizenrt tizenrt/tizenrt:2.0.1-qemu \
	/root/tizenrt/build/configs/lm3s6965-ek/qemu_run.sh
```

`qemu_run.sh` runs:

```bash
qemu-system-arm -M lm3s6965evb -nographic \
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

## Old configurations

`build_test`, `smartfs`, `tc_1m`, `tc_16m`, `tc_16m_grpc` and `tc_64k`
are the configurations of the old TizenRT kernel.  They are kept but not
updated for NuttX 12.13.0 and do not build with it.  `tc_1m`, `tc_16m`
and `tc_16m_grpc` also need QEMU 2.12.0-rc1 with the RAM size patches of
this directory (`qemu-2.12.0-rc1_1m_ram_size.patch`,
`qemu-2.12.0-rc1_16m_ram_size.patch`) and the linker scripts
`scripts/flash-sram-qemu-memory-increased.ld` and
`scripts/flash-sdram-qemu-memory-increased.ld`, which the old
`QEMU_SRAM`/`QEMU_SDRAM` board options selected.  Those options are gone
with the NuttX 12.13.0 board Kconfig.  Running the old network stack on
QEMU is described in [HowToRunNetworkStackOnQemu.md](HowToRunNetworkStackOnQemu.md).
