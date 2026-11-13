# esp32c6-devkitc

TizenRT on the Espressif ESP32-C6-DevKitC-1 (ESP32-C6-WROOM-1, RV32IMAC,
4 MB flash).  The chip and the board come from `arch/risc-v/src/esp32c6`,
`arch/risc-v/src/common/espressif` and `boards/risc-v/esp32c6` of NuttX
12.13.0 (`os/arch/risc-v/src/esp32c6`,
`os/arch/risc-v/src/common/espressif`, `os/board/esp32c6-devkitc`,
`os/board/esp32c6-common`).  FLAT build, UART0 console, simple boot (the
ROM of the chip loads the image, no second stage bootloader).

The configurations are checked on the `esp32c6` machine of the Espressif
QEMU only.  That machine models the CPU, the UART, the interrupt matrix,
the system timer, the timer groups, the SPI flash, the cache/MMU and the
eFuse; Wi-Fi, BLE, 802.15.4 and most of the other peripherals are not
modeled.  Passing in QEMU does not show that the real board works.

## Configurations

| Config | What it does |
|---|---|
| `ostest` | Runs the NuttX OS test (`apps/examples/testcase/ostest`) as the entry point (`CONFIG_INIT_ENTRYPOINT="ostest_main"`). |
| `tash` | Starts TASH (`CONFIG_INIT_ENTRYPOINT="preapp_start"`, `apps/system/init`) with the FS commands, the `ostest` command and the `helloxx` command (`apps/examples/helloxx`, toolchain C++ library with exceptions). |

Both are the configurations of `rv-virt` with the chip, the board and the
console replaced and with these differences:

- `CONFIG_DEBUG_FULLOPT=y` (`-Os`), as the `nsh` configuration of NuttX
  builds the Espressif HAL.
- `CONFIG_RAW_BINARY` is off: `tinyara.bin` is made by esptool from the
  ELF file, not by objcopy.
- `CONFIG_ESPRESSIF_MERGE_BINS=y` makes `tinyara.merged.bin`, the image
  of the whole 4 MB flash that QEMU needs.
- No power off (`CONFIG_BOARDCTL_POWEROFF`): the chip has none, so
  `ostest` returns instead of stopping QEMU.
- No oneshot timer and arch alarm (`CONFIG_ONESHOT`,
  `CONFIG_ALARM_ARCH`): the system tick is the SYSTIMER interrupt of
  `esp_timerisr.c`.

Not enabled: Wi-Fi, BLE, MCUboot, the SPI flash storage, the LP core and
the peripheral drivers other than UART0.  Only `esp32c6-devkitc` has a
build configuration; `os/board/esp32c6-devkitm` and `os/board/esp32c6-xiao`
are the board sources of NuttX without one.

## Espressif HAL

The chip support of NuttX is built on the Espressif HAL
(`esp-hal-3rdparty`), which is not in this repository.  As in NuttX, the
first `make` (the `context` step) clones
`https://github.com/espressif/esp-hal-3rdparty.git` into
`os/arch/risc-v/src/esp32c6/esp-hal-3rdparty` (ignored by git) and checks
out the commit that `os/arch/risc-v/src/common/espressif/Make.defs` pins
(`ESP_HAL_3RDPARTY_VERSION`), so the first build needs the network, and
`make distclean` removes the directory again.

`Make.defs` sets `DISABLE_GIT_DEPTH`: the git of the build image (2.7.4)
cannot fetch the commit of the mbedtls submodule with a shallow clone.

Two ways to avoid the download:

- Create the directory `nxtmpdir` at the top of the repository (beside
  `os`).  As in NuttX, the HAL is then cloned into
  `nxtmpdir/esp-hal-3rdparty` once and linked from the source tree, and
  `make distclean` removes only the link.
- Put a checkout of the pinned commit (with the
  `components/mbedtls/mbedtls` submodule) in
  `os/arch/risc-v/src/esp32c6/esp-hal-3rdparty` before the build, for
  example when git cannot reach GitHub from the build container: the
  clone is skipped when the directory exists.

The HAL includes the NuttX headers as `<nuttx/...>`.  The build gives it
`os/include/tinyara` under that name through the link
`os/arch/risc-v/src/esp32c6/nuttx_compat/nuttx`.

## Build

The build image is the one of `rv-virt`, `tizenrt/tizenrt:2.0.1-qemu-riscv`
(`tools/docker/riscv`): it has the Espressif `riscv32-esp-elf` GCC 14.2.0
and esptool 4.8.1, which the build needs for the image
(`esptool.py elf2image`, `merge_bin`).

```bash
docker build -t tizenrt/tizenrt:2.0.1-qemu-riscv tools/docker/riscv
cd os
./tools/configure.sh esp32c6-devkitc/ostest    # or esp32c6-devkitc/tash
./dbuild.sh build
```

Outputs in `build/output/bin`:

| File | What it is |
|---|---|
| `tinyara` | ELF file |
| `tinyara.bin` | Image for the ROM loader (`esptool.py elf2image --ram-only-header`), to flash at offset 0 |
| `tinyara.merged.bin` | Image of the whole flash (4 MB), for QEMU or to flash at offset 0 |

`Make.defs` differs from the one of NuttX in two build options:

- `-ffunction-sections -fdata-sections` and `--gc-sections` are always on
  (`CONFIG_DEBUG_OPT_UNUSED_SECTIONS` of NuttX, which TizenRT does not
  have): the HAL does not link without them.
- `-fstrict-volatile-bitfields`, as ESP-IDF builds the HAL: the register
  bit-fields are then read and written with 32 bit accesses.  The
  peripheral models of the Espressif QEMU decode only those, and the
  flash initialization of the HAL asserts without the option.

The linker scripts in `scripts` are the ones of
`boards/risc-v/esp32c6/common/scripts` of NuttX with one change:
`esp32c6_sections.ld` ends the table of the C++ static constructors
(`_einit`) at the end of `.init_array`.

## Run in QEMU

Upstream QEMU has no ESP32-C6 machine.  The `Dockerfile` in this directory
builds the Espressif QEMU (release `esp-develop-9.2.2-20261109`) on top of
the build image and installs it in `/opt/qemu-esp`:

```bash
docker build -t tizenrt/tizenrt:2.0.1-qemu-esp32c6 build/configs/esp32c6-devkitc
docker run --rm -it -v $PWD:/root/tizenrt tizenrt/tizenrt:2.0.1-qemu-esp32c6 \
	/root/tizenrt/build/configs/esp32c6-devkitc/qemu_run.sh
```

The `Dockerfile` applies `qemu-esp32c6-uart.patch` to that release.  Its
ESP32-C6 UART model decodes the registers as the ESP32-C3 has them; on
the ESP32-C6 the FIFO thresholds of `UART_CONF1_REG` are 8 bits wide and
the RX timeout is in `UART_TOUT_CONF_SYNC_REG` (0x64).  Without the patch
the model never raises the RX interrupts that the serial driver waits
for, and the console prints but takes no input.  The patch is needed for
the `tash` configuration only.

`qemu_run.sh` runs:

```bash
/opt/qemu-esp/bin/qemu-system-riscv32 -nographic -machine esp32c6 \
	-drive file=build/output/bin/tinyara.merged.bin,if=mtd,format=raw,snapshot=on
```

The ROM prints its boot log (the `SHA-256 comparison failed` lines are
normal for an image with the RAM-only header), then the start-up code
prints `*** Booting NuttX ***`.

The `ostest` configuration passes when the log ends with

```
user_main: Exiting
ostest_main: Exiting with status 0
```

QEMU keeps running after that (no power off); quit with `Ctrl-a x`.

With the `tash` configuration, type `ostest` at the `TASH>>` prompt; the
same lines show the result.  `ps`, `free` and `helloxx` are the other
commands that were checked.
