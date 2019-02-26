# TizenRT

[![License](https://img.shields.io/badge/licence-Apache%202.0-brightgreen.svg?style=flat)](LICENSE)
[![Build Status](https://travis-ci.org/Samsung/TizenRT.svg?branch=master)](https://travis-ci.org/Samsung/TizenRT)

TizenRT is lightweight RTOS-based platform to support low-end IoT devices.  
Please find project details at [Wiki](https://github.com/Samsung/TizenRT/wiki) especially **[documentations page](https://github.com/Samsung/TizenRT/wiki/Documentations)**.

## Contents

> [Environment Setup](#environment-setup)  
> [How to Build](#how-to-build)  
> [Supported Board / Emulator](#supported-board--emulator)  

## Environment Setup

TizenRT provides the easiest way to build with the use of [Docker](https://www.docker.com/).  
There is no need to install the required libraries and toolchains since the provided Docker container already includes everything required for TizenRT development.  
However, if your development systems are not eligible for running the Docker container, all libraries and toolchains should be manually installed.  
Please refer to [Manual Setup Build Environment](docs/HowToSetEnv.md).

For more information of libraries in the TizenRT Docker Image, see https://hub.docker.com/r/tizenrt/tizenrt/.

Especially For Windows environment, configuring CRLF is required as below.
```
git config --global core.autocrlf input
```

### 1. Installing Docker

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

## How to build

TizenRT provides an interactive tool (*dbuild.sh*) where you are prompted to select a option among multiple choices.  
According to your selection, it consecutively provides next-step options.  
When you become familiar to the TizenRT build system, you may use the *dbuild.sh* script with a specific build option.

As the build script is running based on Docker, it requires *sudo* for root permission.  
To run Docker without *sudo*, refer to https://docs.docker.com/install/linux/linux-postinstall/.

### Using an interactive tool

To get started, use the *dbuild.sh* script with the *menu* option as follows:
```bash
cd os
./dbuild.sh menu
```

This command shows you the complete list of supported boards first as shown below:
```bash
======================================================
  "Select Board"
======================================================
  "1. artik053"
  "2. cy4390x"
      ...
  "x. EXIT"
======================================================
```

After the board selection, you are prompted to select configuration of the given board:
```bash
======================================================
  "Select Configuration of artik053"
======================================================
  "1. hello"
  "2. tc"
      ...
  "x. EXIT"
======================================================
```

Finally, you are prompted to select a build option as shown below:
```bash
======================================================
  "Select build Option"
======================================================
  "1. Build with Current Configurations"
  "2. Re-configure"
  "3. Menuconfig"
  "4. Build Clean"
  "5. Build Dist-Clean"
  "d. Download"
  "x. Exit"
======================================================
```

Once the board and configuration selection is finished,  
you are prompted to select a build option repeatedly until you remove configuration by the *Re-configure* or *Build Dist-Clean* option.

### Using specific build options

#### 1. Configuration

```bash
cd os
./tools/configure.sh <board>/<configuration_set>
```

This command retrieves the specific pre-configured file named defconfig and Make.defs according to \<board\>/\<configuration_set\>.  
Once the configuration is done, you can skip this step next time unless you want to change your configuration.  
You can see collection of all configuration files at *$TIZENRT_BASEDIR/build/configs*.  
To check all pre-defined configurations, type as follows:

```bash
./configure.sh --help
```

##### 1.1 Additional Configuration (optional)

After basic configuration by [1. Configuration](#1-configuration), you can additionally modify your configuration with *menuconfig*.

```bash
./dbuild.sh menuconfig
```

**Note**: In Docker environment, ```make menuconfig``` command from other README files should be replaced with this command.  
          ```make menuconfig``` applies only in [Manual Setup Build Environment](docs/HowToSetEnv.md).

#### 2. Compilation

```bash
./dbuild.sh
```

Built binaries are located in *$TIZENRT_BASEDIR/build/output/bin*.

##### 2.1 Clean

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

#### 3. Programming

```bash
./dbuild.sh download [OPTION]
```

TizenRT supports *download* command to program a binary into a board.  
You might be required to set up USB driver. For more information, please refer to [Supported board / Emulator](#supported-board--emulator).

```OPTION``` designates which flash partitions are flashed.  
For example, ```ALL``` means programming all of binaries. This also depends on each board.

#### Advanced

You can give multiple build options to the *dbuild.sh* script as mentioned below:
```bash
./dbuild.sh distclean configure artik053 hello build download all
```
This executes sequentially multiple commands including
1. Execute distclean
2. Configure with artik053/hello
3. Build
4. Program all of binaries

## Supported Board / Emulator

TizenRT supports multiple boards as well as QEMU.  
The linked page for each board includes board-specific environments, programming method, and board information.

ARTIK053 [[details]](build/configs/artik053/README.md)

ARTIK053S [[details]](build/configs/artik053s/README.md)

ARTIK055S [[details]](build/configs/artik055s/README.md)

CY4390X [[details]](build/configs/cy4390x/README.md)

SIDK_S5JT200 [[details]](build/configs/sidk_s5jt200/README.md)

STM32F407-DISC1 [[details]](build/configs/stm32f407-disc1/README.md)

STM32F429I-DISCO [[details]](build/configs/stm32f429i-disco/README.md)

QEMU [[details]](build/configs/qemu/README.md)
