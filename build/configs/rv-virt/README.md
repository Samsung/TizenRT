# rv-virt

TizenRT on the QEMU RISC-V `virt` machine with an RV32IMAC CPU in M-mode,
the ACLINT timer, the PLIC and a 16550 UART console.  The board comes from
`boards/risc-v/qemu-rv/rv-virt` of NuttX 12.13.0 (`os/board/rv-virt`,
`os/arch/risc-v/src/qemu-rv`).  FLAT build, single core, no FPU.

The board is here to run the common RISC-V code of NuttX 12.13.0
(`os/arch/risc-v/src/common`, `os/libs/libc/machine/risc-v`) under QEMU;
esp32c6 uses the same code with the same instruction set.

## Configurations

| Config | What it does |
|---|---|
| `ostest` | Runs the NuttX OS test (`apps/examples/testcase/ostest`) as the entry point (`CONFIG_INIT_ENTRYPOINT="ostest_main"`) and powers QEMU off with the test result. |
| `tash` | Starts TASH (`CONFIG_INIT_ENTRYPOINT="preapp_start"`, `apps/system/init`) with the FS commands, the `ostest` command and the `helloxx` command (`apps/examples/helloxx`, toolchain C++ library with exceptions). |

Not enabled: SMP, the FPU (`CONFIG_ARCH_FPU`), S-mode and the kernel and
protected builds, virtio, the device tree and the semihosting hostfs.
Their linker scripts (`ld-kernel.script`, `ld-protected.script`, ...) are
not taken.

## Build

The `tizenrt/tizenrt:2.0.1-qemu-riscv` docker image adds the RISC-V
toolchain (Espressif `riscv32-esp-elf` GCC 14.2.0) and
`qemu-system-riscv32` to the TizenRT build image (see
`tools/docker/riscv`).  `CONFIG_DOCKER_VERSION="2.0.1-qemu-riscv"` makes
`dbuild.sh` use it.  `Make.defs` sets `CROSSDEV = riscv32-esp-elf-`.

```bash
docker build -t tizenrt/tizenrt:2.0.1-qemu-riscv tools/docker/riscv
cd os
./tools/configure.sh rv-virt/ostest    # or rv-virt/tash
./dbuild.sh build
```

## Run

```bash
docker run --rm -it -v $PWD:/root/tizenrt tizenrt/tizenrt:2.0.1-qemu-riscv \
	/root/tizenrt/build/configs/rv-virt/qemu_run.sh
```

`qemu_run.sh` runs:

```bash
qemu-system-riscv32 -M virt,aclint=on -cpu rv32 -smp 1 -bios none \
	-nographic -kernel build/output/bin/tinyara
```

The `ostest` configuration passes when the log ends with

```
user_main: Exiting
ostest_main: Exiting with status 0
```

and QEMU exits (`boardctl(BOARDIOC_POWEROFF)`).

With the `tash` configuration, type `ostest` at the `TASH>>` prompt; the
same lines show the result.  `ps`, `free` and `helloxx` are the other
commands that were checked.  Quit QEMU with `Ctrl-a x`.
