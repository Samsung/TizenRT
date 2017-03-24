# Tizen RT

[![License](https://img.shields.io/badge/licence-Apache%202.0-brightgreen.svg?style=flat)](LICENSE)

lightweight RTOS-based platform to support low-end IoT devices.

You can find project details on our [wiki](https://github.com/Samsung/TizenRT/wiki).

## Quick Start
### Getting the toolchain

Get the build in binaries and libraries, [gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar](https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q3-update)

Untar the gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar and export the path like

```bash
tar xvf gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar
export PATH=<Your Toolchain PATH>:$PATH
```

### Getting the sources

```bash
git clone https://github.com/Samsung/TizenRT.git
cd TizenRT
TIZENRT_BASEDIR="$PWD"
```

### How to Build

Configure the build from $TIZENRT_BASEDIR/os/tools directory
```bash
cd os/tools
./configure.sh <board>/<configuration_set>
```
For list of boards and configuration set supported, refer belows.
Above copies the canned configuration-set for the particular board, into the $TIZENRT_BASEDIR/os directory.

Configuration can be modified through make menuconfig from $TIZENRT_BASEDIR/os.
```bash
cd ..
make menuconfig
```

Finally, initiate build by make from $TIZENRT_BASEDIR/os
```bash
make
```

Built binaries are in $TIZENRT_BASEDIR/build/output/bin.

## Board
Tizen RT currently supports one board called sidk_s5jt200. However, this board is not available in public markets till now. sidk_s5jt200 or other boards for Tizen RT will be coming soon.
  sidk_s5jt200 stands for Samsung IoT Development Kit for S5JT200 chipset.

## Configuration Sets
There are three configuration sets for sidk_s5jt200, including 'tc', 'kernel_sample', and 'hello_with_tash'.
'tc' is a configuration set for runnig unit test cases, 'kernel_sample' for running kernel functions, and 'hello_with_tash' for running a hello example.
You can modify the configuration by using the menuconfig tool on the 'os' folder, but all configuration combinations are not fully tested yet.
The IPv4 network stack based on LWIP is included, but Wi-Fi related codes such as wpa_supplicant are not included.
Wi-Fi will be added in 2017.
