# QEMU MPS2 AN505 Platform (qemu-mps2-an505)

This configuration runs TizenRT on the QEMU `mps2-an505` machine for the ARM Cortex-M33 profile. It is used to emulate the BK7239N execution model in QEMU without trying to reproduce the full SoC peripheral set.

## Overview

### Key Features
- **Cortex-M33 Execution Model**: Single-core ARMv8-M profile aligned with the BK7239N software model
- **Fast QEMU Validation**: Boot, shell smoke, and testcase validation without hardware
- **Protected and XIP Coverage**: Supports flat, loadable ELF, and loadable XIP ELF flows in one board family

### Configurations

This directory contains the following pre-defined configurations for the `qemu-mps2-an505` board:

| Configuration | Build Model | CPU | Binary Format | Description |
|---------------|-------------|-----|---------------|-------------|
| `flat` | Flat (`CONFIG_BUILD_FLAT=y`) | Cortex-M33 | Raw/XIP kernel | Kernel and applications are linked into one image and staged through `./dbuild.sh download` |
| `loadable_elf` | Protected (`CONFIG_BUILD_PROTECTED=y`) | Cortex-M33 | ELF loadable apps | Kernel runs separately and `common` / `app1` are loaded from staged runtime images |
| `loadable_xip_elf` | Protected (`CONFIG_BUILD_PROTECTED=y`) | Cortex-M33 | XIP-ELF loadable apps | Kernel runs separately and `common` / `app1` execute from the staged flash/XIP layout |

## Quick Start Guide

### Step 1: Configure and Build

```bash
cd os
./dbuild.sh configure qemu-mps2-an505 loadable_xip_elf
./dbuild.sh build
```

For the other configurations:

```bash
./dbuild.sh configure qemu-mps2-an505 flat
./dbuild.sh build

./dbuild.sh configure qemu-mps2-an505 loadable_elf
./dbuild.sh build
```

The default Docker build image is `tizenrt/tizenrt:2.0.1`.

### Step 2: Stage Runtime Images

```bash
./dbuild.sh download
```

This generates `qemu_mps2_an505_images/` in the project root and copies `run_qemu.sh` to the project root for the next step. The staging directory includes `kernel-runtime.elf`, `flash-runtime.bin`, and, when applicable, `common.trpk`, `app1.trpk`, and `bootparam.bin`.

### Step 3: Run QEMU

```bash
cd ..
./run_qemu.sh
```

To override the runtime image tag:

```bash
QEMU_RUNTIME_DOCKER_IMAGE=tizenrt-qemu-test:ci ./run_qemu.sh
```

The default runtime image is built from `tools/qemu_test/Dockerfile`.

## Runtime Validation Scope

The shared runtime harness validates the following on `qemu-mps2-an505`:

- Boot to `TASH>>`
- `help`, `free`, `ps`, `pwd`
- `/mnt` file I/O smoke
- `drivers_tc`
- `filesystem_tc`
- `kernel_tc`

Local Docker runtime validation has been completed for `flat`, `loadable_elf`, and `loadable_xip_elf`.

## CI

GitHub Actions workflow `qemu-runtime-tests.yml` validates all three `qemu-mps2-an505` defconfigs together with the `qemu-virt` defconfigs in a single 5-config matrix. Each run uploads transcript, JUnit XML, and summary artifacts.

For the full RTL8730E / BK7239N emulation expansion plan and phase log, refer to `260409_qemu_emulation_plan_KOR.md`.

## Limitations

- This port targets the BK7239N execution model, not the full BK7239N SoC peripheral set.
- TF-M secure world, vendor Wi-Fi/BLE blocks, and board-specific signing flows are out of scope.
- `virtio_blk_test` is not part of the `qemu-mps2-an505` runtime suite.
