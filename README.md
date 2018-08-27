# TizenRT

[![License](https://img.shields.io/badge/licence-Apache%202.0-brightgreen.svg?style=flat)](LICENSE)
[![Build Status](https://travis-ci.org/Samsung/TizenRT.svg?branch=master)](https://travis-ci.org/Samsung/TizenRT)

TizenRT is lightweight RTOS-based platform to support low-end IoT devices.  
Please find project details at [Wiki](https://github.com/Samsung/TizenRT/wiki) especially **[documentations page](https://github.com/Samsung/TizenRT/wiki/Documentations)**.

## Contents

> [Quick Start](#quick-start)  
> [Supported Board / Emulator](#supported-board--emulator)  

## Quick Start

TizenRT provides the easiest way to build with the use of [Docker](https://www.docker.com/).  
There is no need to install the required libraries and toolchains since the provided Docker container already includes everything required for TizenRT development.  
However, if your development systems are not eligible for running the Docker container, all libraries and toolchains should be manually installed.  
Please refer to [Manual Setup Build Environment](docs/HowToSetEnv.md).

For more infomation of libraries in the TizenRT Docker Image, see https://hub.docker.com/r/tizenrt/tizenrt/.

### 1. Install Docker

To install OS specific Docker engines, see https://docs.docker.com/install/linux/docker-ce/ubuntu/.  
If you already have a Docker engine, please skip this step.

### 2. Getting TizenRT source code

```bash
git clone https://github.com/Samsung/TizenRT.git
cd TizenRT
TIZENRT_BASEDIR="$PWD"
```
**Note**: To contribute in this community, you need to clone your forked private repository instead.  
          Github guides this by [working-with-forks](https://help.github.com/articles/working-with-forks).

### 3. Configuration

```bash
cd os
./tools/configure.sh <board>/<configuration_set>
```

This command retrieves the specific pre-configured file named defconfig and Make.defs according to \<board\>/\<configuration_set\>.  
You can see collection of all configuration files at *$TIZENRT_BASEDIR/build/configs*.  
To check all pre-defined configurations, type as follows:

```bash
./configure.sh --help
```

#### 3.1 Additional Configuration

After basic configuration by [3. Configuration](#3-configuration), you can customize configurations optionally with **menuconfig**.

```bash
./dbuild.sh menuconfig
```

This command might require ```sudo``` for root permission.  
To run Docker without ```sudo```, refer to https://docs.docker.com/install/linux/linux-postinstall/.

### 4. Compilation

```bash
./dbuild.sh
```

Built binaries are located in *$TIZENRT_BASEDIR/build/output/bin*.

#### 4.1 Clean

There are two types of clean commands, clean and distclean.

```bash
./dbuild.sh clean
```

This command removes built files including objects, libraries, .depend, Make.dep, etc.  
After modifying configuration with menuconfig, this command is required.

```bash
./dbuild.sh distclean
```

This command includes the *clean* option and additionally removes configured files including .config, Make.defs and linked folders / files.  
Before changing basic configuration with ```./configure.sh``` command, this command is required to delete pre-set configurations.

### 5. Programming

```bash
./dbuild.sh download [OPTION]
```

TizenRT supports *download* command to program a binary into a board.  
You might be required to set up usb driver. For more information, please refer to [Supported board / Emulator](#supported-board--emulator).

```OPTION``` designates which flash partitions are flashed.  
For example, ```ALL``` means programming all of binaries. This also depends on each board.

## Supported Board / Emulator

TizenRT supports multiple boards as well as QEMU.  
The linked page for each board includs board-specific environments, programming method, and board information.

ARTIK053 [[details]](build/configs/artik053/README.md)

ARTIK053S [[details]](build/configs/artik053s/README.md)

ARTIK055S [[details]](build/configs/artik055s/README.md)

CY4390X [[details]](build/configs/cy4390x/README.txt)

SIDK_S5JT200 [[details]](build/configs/sidk_s5jt200/README.md)

QEMU [[details]](build/configs/qemu/README.md)
