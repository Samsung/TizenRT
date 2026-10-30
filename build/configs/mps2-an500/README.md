# mps2-an500

TizenRT on the QEMU `mps2-an500` machine (Arm MPS2 FPGA image AN500,
Cortex-M7 with FPU) with a CMSDK UART console.  The board comes from
`boards/arm/mps/mps2-an500` of NuttX 12.13.0 (`os/board/mps2-an500`,
`os/arch/arm/src/mps`); it uses the `armv7-m` core.  FLAT build only.

Together with `lm3s6965-ek` (Cortex-M3, no FPU) it checks the `armv7-m`
core on QEMU: here with the FPU (`CONFIG_ARCH_FPU`, ostest FPU test) and
the Cortex-M7 code paths.

## Configurations

| Config | What it does |
|---|---|
| `ostest` | Runs the NuttX OS test (`apps/examples/testcase/ostest`) as the entry point (`CONFIG_INIT_ENTRYPOINT="ostest_main"`). |
| `tash` | Starts TASH (`CONFIG_INIT_ENTRYPOINT="preapp_start"`, `apps/system/init`) with the system and FS commands and the `ostest` command. |

The configurations are the ones of `mps2-an521` with the chip and board
values of the NuttX `mps2-an500` `nsh` configuration: SysTick
(`ARMV7M_SYSTICK`), CMSDK UART0 at 0x40004000 (TX 17, RX 16, overflow
28), RAM at 0x60000000.  The board has no `board_power_off()` in NuttX
12.13.0, so QEMU keeps running after the test.

## Build

```bash
docker build -t tizenrt/tizenrt:2.0.1-qemu tools/docker
cd os
./tools/configure.sh mps2-an500/ostest    # or mps2-an500/tash
./dbuild.sh build
```

## Run

```bash
docker run --rm -it -v $PWD:/root/tizenrt tizenrt/tizenrt:2.0.1-qemu \
	/root/tizenrt/build/configs/mps2-an500/qemu_run.sh
```

`qemu_run.sh` runs:

```bash
qemu-system-arm -M mps2-an500 -nographic \
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
