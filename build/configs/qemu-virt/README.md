# QEMU Virtual Platform (qemu-virt)

This configuration is for running TizenRT on the QEMU `virt` machine for the ARM architecture. It allows for testing and development of the OS without needing physical hardware.

## Overview

### Key Features
- **Virtual Environment**: Complete development environment using software only, no physical hardware required
- **Fast Test Cycle**: Rapid testing and debugging without hardware reboot
- **DRAM Boot (dramboot)**: All configurations boot from DRAM, where only the kernel is loaded into RAM and applications run using XIP (Execute-In-Place)

### Configurations

This directory contains the following pre-defined configurations for the `qemu-virt` board:

| Configuration | Build Model | CPU | Binary Format | Description |
|---------------|-------------|-----|---------------|-------------|
| `flat` | Flat (`CONFIG_BUILD_FLAT=y`) | Dual Core (SMP 2) | - | Kernel and applications are linked into a single image and downloaded as `bl1/kernel/bootparam` |
| `loadable_xip_elf` | Protected (`CONFIG_BUILD_PROTECTED=y`) | Dual Core (SMP 2) | XIP-ELF (`CONFIG_XIP_ELF=y`) | Kernel runs from DRAM while `common` and `app1` execute from flash and download as `bl1/kernel/common/app1/bootparam` |


## Quick Start Guide

### Step 1: Configure and Build

```bash
# Navigate to the os directory
cd os

# Configure for XIP (recommended for beginners)
./dbuild.sh configure qemu-virt loadable_xip_elf

# Build the system
./dbuild.sh build
```

For the flat configuration, use:

```bash
./dbuild.sh configure qemu-virt flat
./dbuild.sh build
```

### Step 2: Download Binaries to QEMU Flash Image

```bash
# Run in os directory
# download to Partition A
./dbuild.sh download bl1 kernel common app1 bootparam
# or interactive partition selection
./dbuild.sh download ALL
```

For `flat`, use the flat image layout:

```bash
./dbuild.sh download bl1 kernel bootparam
```

If needed, `qemu_flash.bin` will be created in project root directory.
Download process copies `run_qemu.sh` that required in next step to project root directory.
For repeated local runtime runs, regenerate `qemu_flash.bin` with `./dbuild.sh download ...` before rerunning filesystem testcases so the flash-backed userfs starts clean.

### Step 3: Run QEMU

```bash
# Run in project root directory
./run_qemu.sh

# Override the default repo-local runtime image
QEMU_VIRT_DOCKER_IMAGE=tizenrt-qemu-virt-runtime:ci ./run_qemu.sh
```

The default runtime image is built from `tools/qemu_test/Dockerfile`. It compiles official QEMU source in-repo and applies the 4 KiB pflash geometry required by `qemu-virt`, so no external `pcs1265/qemu-virt-flash-4k` image is required.

### Step 4: Run the Virtio Smoke Command

```bash
virtio_blk_test
```

`virtio_blk_test` prints `VIRTIO_BLK_TEST: PASS` on success.

## Machine Components

### Hardware Components

#### PL011 UART Controller
**Description**: ARM PrimeCell PL011 UART controller for serial communication.

**Specifications**:
- **Base Address**: 0x09000000
- **Interrupt**: IRQ 33
- **Clock Frequency**: 24MHz
- **Function**: Serial console communication and bootloader output

#### Virtio-MMIO Devices
**Description**: Virtual I/O framework for efficient paravirtualized device access with customizable peripheral support.

**Specifications**:
- **Base Address**: 0x0A000000
- **Device Spacing**: 0x200 bytes
- **Maximum Devices**: 32
- **Supported Devices**: Virtio Block Device (ID: 2), Network Device, Console, RNG, and more
- **Function**: High-performance device emulation with flexible peripheral configuration
- **Customization**: Users can add various virtio devices for testing different hardware scenarios

### Memory Architecture

The qemu-virt platform uses the following memory map:

| Region | Address Range | Size | Description |
|--------|--------------|------|-------------|
| Flash 0 | 0x00000000-0x04000000 | 64MB | Primary boot and application storage |
| Flash 1 | 0x04000000-0x08000000 | 64MB | Secondary boot and application storage |
| I/O | 0x08000000-0x0e000000 | 96MB | PL011 UART, Virtio devices |
| Secure Memory | 0x0e000000-0x0f000000 | 16MB | Secure world memory |
| PCIe | 0x10000000-0x40000000 | 768MB | PCIe device space |
| DDR | 0x40000000-0x50000000 | 256MB | System RAM |

### Software Components

#### S1-boot (BL1)
**Description**: QEMU-virt specific bootloader, the first bootloader executed during system boot.

**Role**: This bootloader handles:
- Hardware initialization
- Boot parameter and kernel partition checksum verification
- RAM initialization
- Kernel loading and execution

#### Virtio Driver Stack
**Description**: Virtio implementation for qemu-virt board.

**Components**:
- **virtio-mmio**: Memory-mapped Virtio device interface
- **virtio-blk**: Block device driver implementation
- **virtio-queue**: Virtqueue management for efficient I/O

#### PL011 UART Driver
**Description**: Serial communication driver for qemu-virt platform.

**Features**:
- FIFO-based transmission and reception
- Configurable baud rates and data formats
- Interrupt-driven I/O operations
- Console support for system interaction

## Debugging

This section explains how to debug TizenRT on QEMU using Visual Studio Code.

### 1. Prerequisites

- **Visual Studio Code**: Install from the [official website](https://code.visualstudio.com/).
- **Cortex-Debug Extension**: Install the `Cortex-Debug` extension from the Visual Studio Code Marketplace. You can search for `marlonbaeten.cortex-debug` in the Extensions view (Ctrl+Shift+X).
- **ARM Toolchain or gdb-multiarch**: The `gdb-multiarch` debugger is required. Ensure `gdb-multiarch` is installed and accessible in your system's PATH. You may need to install it via your system's package manager (e.g., `sudo apt install gdb-multiarch` on Debian/Ubuntu).
- **Docker**: The QEMU environment is run inside a Docker container. Ensure Docker is installed and running on your system.

### 2. Setup

1. **Create `.vscode` directory**: At the root of your TizenRT project, create a directory named `.vscode`.

    ```bash
    mkdir -p .vscode
    ```

2. **Copy `launch.json`**: Copy the debugger configuration file to the newly created directory.

    ```bash
    cp build/configs/qemu-virt/tools/launch.json .vscode/launch.json
    ```

    *Note: Remember to edit `.vscode/launch.json` if your `arm-none-eabi-gdb` path is different.*

### 3. Debugging Session

1. **Start QEMU with GDB Server**:
    Run the `run_qemu.sh` script. This script will start QEMU inside a Docker container and automatically start a GDB server listening on port `1234`, which is exposed to your host machine.

    ```bash
    ./run_qemu.sh
    ```

2. **Start Debugging in VS Code**:
    - Open your TizenRT project folder in Visual Studio Code.
    - Go to the "Run and Debug" view on the left-hand side (or press Ctrl+Shift+D).
    - From the dropdown menu at the top, select the desired debug configuration:
      - **"QEMU(S1-Boot)"**: Debug the bootloader
      - **"QEMU(Kernel)"**: Debug the kernel only (Flat)
      - **"QEMU(XIPELF)"**: Debug the kernel and XIP applications
      - **"QEMU(ELF)"**: Debug the kernel and standard ELF applications (Load symbols automatically : check `build/configs/qemu-virt/tools/auto_symbol_loader.py`)
    - Press the "Start Debugging" button (the green play icon) or press F5.

The VS Code debugger should now attach to the QEMU session, and you can start debugging your code.

## CI

GitHub Actions workflow `qemu-runtime-tests.yml` validates the full 5-config emulation matrix:

- `qemu-virt/flat`
- `qemu-virt/loadable_xip_elf`
- `qemu-mps2-an505/flat`
- `qemu-mps2-an505/loadable_elf`
- `qemu-mps2-an505/loadable_xip_elf`

For the `qemu-virt` subset, the workflow builds both supported defconfigs, generates flash images, boots QEMU, runs shell smoke checks, executes `virtio_blk_test`, and then runs `drivers_tc`, `filesystem_tc`, and `kernel_tc`.

Local Docker validation has also been completed for both `loadable_xip_elf` and `flat`.

For the full RTL8730E / BK7239N emulation expansion plan and phase log, refer to `260409_qemu_emulation_plan_KOR.md`.

## Limitations
   - Only `virtio-blk` is covered in the current automation scope.
   - `virtio-net` and network validation are out of scope.
   - The `qemu-virt` workflow coverage is limited to `flat` and `loadable_xip_elf`.
