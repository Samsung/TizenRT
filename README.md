# TizenRT for ARTIK05x

[![License](https://img.shields.io/github/license/SamsungARTIK/TizenRT.svg)](LICENSE)
[![Github Release](https://img.shields.io/github/release/SamsungARTIK/TizenRT.svg)](https://github.com/SamsungARTIK/TizenRT/releases)
[![GitHub Release Date](https://img.shields.io/github/release-date/SamsungARTIK/TizenRT.svg)](https://github.com/SamsungARTIK/TizenRT/releases)
[![Build Status](https://travis-ci.org/SamsungARTIK/TizenRT.svg?branch=artik)](https://travis-ci.org/SamsungARTIK/TizenRT)

lightweight RTOS-based platform to support low-end IoT devices.

This project is forking [TizenRT](https://github.com/Samsung/TizenRT) from [SamsungOpenSource](https://github.com/Samsung). It is intended to support ARTIK05x board stably.

## Quick Start
### Ubuntu Development Setting

Install the most basic packages for using TizenRT.
```bash
$ sudo apt-get update
$ sudo apt-get upgrade
$ sudo apt-get install build-essential openocd git gcc-arm-none-eabi gdb-arm-none-eabi minicom
```

### Getting the toolchain

Get the build in binaries and libraries, [gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar.bz2](https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q3-update/+download/gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar.bz2)

Untar the archive file.
```bash
$ tar -tvf gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar.bz2
```

and export the path like.
```bash
export PATH=<Your Toolchain PATH>:$PATH
```

### Set kconfig-frontends

This package is used by the TizenRT to configure the build. You can change build environment, what is enabled as peripherals from the SoC and from the board.
```bash
$ sudo apt-get install gperf libncurses5-dev flex bison
$ git clone https://bitbucket.org/nuttx/tools.git tools
$ cd tools/kconfig-frontends
$ ./configure --enable-mconf --disable-gconf --disable-qconf --prefix=/usr
$ make
$ sudo make install
```

### Getting the sources

The main branch is `artik`.
```bash
$ git clone https://github.com/SamsungARTIK/TizenRT.git
$ cd TizenRT
```
Since then, `TIZENRT_BASEDIR` means the path where the source was cloned.

### How to Build

Configure the build from `TIZENRT_BASEDIR/os/tools` directory
```bash
$ cd os/tools
$ ./configure.sh <board>/<configuration_set>
```
For list of boards and configuration set supported, refer belows.

Above copies the canned configuration-set for the particular board, into the `TIZENRT_BASEDIR/os` directory.

Configuration can be modified through make menuconfig from `TIZENRT_BASEDIR/os`.
```bash
$ cd os
$ make menuconfig
```

Finally, initiate build by make from `TIZENRT_BASEDIR/os`.
```bash
$ cd os
$ make
```

Built binaries are in `TIZENRT_BASEDIR/build/output/bin`.
```bash
$ cd build/output/bin
$ ls
System.map  tinyara  tinyara.bin  tinyara_head.bin  tinyara.map  tinyara_memstats.txt
```

## Supported Board

> * [ARTIK053](build/configs/artik053/README.md)
> * [ARTIK053S](build/configs/artik053s/README.md)
> * [ARTIK055S](build/configs/artik055s/README.md)
> * [sidk_s5jt200](build/configs/sidk_s5jt200/README.md)

## Configuration Sets

To build a TizenRT application, use the default configuration files named `defconfig` under `TIZENRT_BASEDIR/build/configs/<board>/` folder.

To customize your application with specific configuration settings, using the menuconfig tool is recommended at os folder as shown:
```bash
$ cd os
$ make menuconfig
```
Please keep in mind that we are actively working on board configurations, and will be posting our updates on the README files under each config

## How to enable the ARTIK SDK

If you want to use the `ARTIK SDK`, you need the following settings in menuconfig.
 * External Functions > Enable "ARTIK SDK"
 * Save and exit
 * Launch the build

If you enable ARTIK SDK and then build it, it will automatically download the ARTIK SDK sources from github and build it. You do not need to download additional files.

In addition, to enable the `ARTIK SDK examples`, set the following in menuconfig:
 * Application Configuration > Examples > Enable "ARTIK SDK examples"
 * Press Enter then select all the examples you want to include in the image
