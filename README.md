# Tizen RT

[![License](https://img.shields.io/badge/licence-Apache%202.0-brightgreen.svg?style=flat)](LICENSE)

lightweight RTOS-based platform to support low-end IoT devices.

Please find project details on our [Tizen wiki](https://wiki.tizen.org/wiki/Tizen_RT).

## Quick Start
### Ubuntu Development Setting

```bash
~$ sudo apt-get update
~$ sudo apt-get upgrade
~$ sudo apt-get install build-essential openocd git gcc-arm-none-eabi gdb-arm-none-eabi minicom
```

### Getting the toolchain

Get the build in binaries and libraries, [gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar](https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q3-update)

Untar the gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar and export the path like

```bash
~$ tar xvf gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar
export PATH=<Your Toolchain PATH>:$PATH
```

### Getting the sources

```bash
~$ git clone https://github.com/SamsungARTIK/TizenRT.git
~$ cd TizenRT
TIZENRT_BASEDIR="$PWD"
```

### Set kconfig-frontends

```bash
~$ sudo apt-get install gperf libncurses5-dev flex bison
~$ git clone https://patacongo@bitbucket.org/nuttx/tools.git tools
~$ cd tools/kconfig-frontends
~/tools/kconfig-frontends$ ./configure --enable-mconf --disable-gconf --disable-qconf --prefix=/usr
~/tools/kconfig-frontends$ make
~/tools/kconfig-frontends$ sudo make install
```

### How to Build

Configure the build from $TIZENRT_BASEDIR/os/tools directory
```bash
~/TizenRT$ cd os/tools
~/TizenRT/os/tools$ ./configure.sh <board>/<configuration_set>
```
For list of boards and configuration set supported, refer belows.

Above copies the canned configuration-set for the particular board, into the $TIZENRT_BASEDIR/os directory.

Configuration can be modified through make menuconfig from $TIZENRT_BASEDIR/os.
```bash
~/TizenRT/os/tools$ cd ..
~/TizenRT/os$ make menuconfig
```

Finally, initiate build by make from $TIZENRT_BASEDIR/os
```bash
~/TizenRT/os$ make
```

Built binaries are in $TIZENRT_BASEDIR/build/output/bin.

## Supported Board

ARTIK053 [[details]](build/configs/artik053/README.md)

ARTIK053S [[details]](build/configs/artik053s/README.md)

ARTIK055S [[details]](build/configs/artik055s/README.md)

sidk_s5jt200 [[details]](build/configs/sidk_s5jt200/README.md)

## Configuration Sets

To build a Tizen RT application, use the default configuration files named 'defconfig' under 'build/configs/<board>/' folder.

To customize your application with specific configuration settings, using the menuconfig tool is recommended at os folder as shown:
```bash
~/TizenRT/os$ make menuconfig
```
Please keep in mind that we are actively working on board configurations, and will be posting our updates on the README files under each config

