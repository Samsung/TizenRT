# TizenRT

[![License](https://img.shields.io/badge/licence-Apache%202.0-brightgreen.svg?style=flat)](LICENSE)
[![Build Status](https://travis-ci.org/Samsung/TizenRT.svg?branch=master)](https://travis-ci.org/Samsung/TizenRT)

TizenRT is lightweight RTOS-based platform to support low-end IoT devices.  
Please find project details at [Wiki](https://github.com/Samsung/TizenRT/wiki) especially **[documentations page](https://github.com/Samsung/TizenRT/wiki/Documentations)**.

## Contents

> [Quick Start](#quick-start)  
> [Supported Board / Emulator](#supported-board--emulator)  
> [Configuration Sets](#configuration-sets)

## Quick Start
### Getting the toolchain

Get the build in binaries and libraries, [gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar.bz2](https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q3-update)  
Untar the gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar.bz2 and export the path like

```bash
tar xvjf gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar.bz2
export PATH=<Your Toolchain PATH>:$PATH
```
Be aware that recommanded toolchain is fully working on 32bits machine. 64bits machine needs additional packages.

### Getting the sources

```bash
git clone https://github.com/Samsung/TizenRT.git
cd TizenRT
TIZENRT_BASEDIR="$PWD"
```

### How to Build

Configure the build from *$TIZENRT_BASEDIR/os/tools* directory
```bash
cd os/tools
./configure.sh <board>/<configuration_set>
```
The configuration file is named *defconfig*,  
and resides under the relative path \<board\>/\<configuration_set\> rooted at *build/configs*.  
To check the different \<board\>/\<configuration_set\> combinations supported, type below:
```bash
./configure.sh --help
```

After configuring above, configuration can be modified through *make menuconfig* from *$TIZENRT_BASEDIR/os*.
```bash
cd ..
make menuconfig
```

Refer [kconfig-frontend installation](docs/HowtoInstallKconfigFrontend.md) to use *menuconfig*.

Finally, initiate build by make from *$TIZENRT_BASEDIR/os*.
```bash
make
```

Built binaries are in *$TIZENRT_BASEDIR/build/output/bin*.

See [Trouble Shooting](docs/TroubleShooting.md) to resolve any issue on TizenRT usages.

## Supported Board / Emulator
Here are supported boards and emulator list.  
Refer belows to know board-specifc environments, programming method and board information.

ARTIK053 [[details]](build/configs/artik053/README.md)

ARTIK053S [[details]](build/configs/artik053s/README.md)

ARTIK055S [[details]](build/configs/artik055s/README.md)

SIDK_S5JT200 [[details]](build/configs/sidk_s5jt200/README.md)

QEMU [[details]](build/configs/qemu/README.md)

## Configuration Sets

To build a TizenRT application, use the default configuration files named *defconfig* under *build/configs/\<board\>/\<configuration_set\>* folder.  
To customize your application with specific configuration settings, using the menuconfig tool is recommended at *os* folder as shown:
```bash
make menuconfig
```
Please keep in mind that we are actively working on board configurations, and will be posting our updates on the README files under each config.
