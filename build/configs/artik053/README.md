# ARTIK 053

The ARTIK 053 is a SOC for Wi-Fi™ IoT solutions. The ARTIK 053 has a Wi-Fi subsystem, security subsystem, and application subsystem.

## Information

will be updated

## Environment Set-up
### On Chip Debugger installation

OpenOCD is used to program and debug.

OpenOCD v0.10.0 is recommended and can be installed like below,
but pre-built OpenOCD binaray on tools/openocd/linux64(or 32) can be used without installing.
```bash
sudo apt-get build-dep openocd
git clone --depth 1 -b v0.10.0 https://git.code.sf.net/p/openocd/code openocd-code
cd openocd-code
./bootstrap
./configure
make
sudo make install
```

Set the OPENOCD_SCRIPTS to environment variable.
This step is needed to program a binary at os folder.
```bash
export OPENOCD_SCRIPTS=$TIZENRT_BASEDIR/build/configs/artik053/tools/openocd
```
TIZENRT_BASEDIR was set at 'Getting the sources' tab of [[Quick Start]](../../../README.md).

## How to program a binary

At first, programming the complete set of binaries are needed.
After buiding a Tizen RT, execute as follows at os folder.
```bash
openocd -f artik053.cfg -c ' \
    flash_write bl1    ../build/configs/artik053/bin/bl1.bin;      \
    flash_write bl2    ../build/configs/artik053/bin/bl2.bin;      \
    flash_write sssfw  ../build/configs/artik053/bin/sssfw.bin;    \
    flash_write wlanfw ../build/configs/artik053/bin/wlanfw.bin;   \
    flash_write os     ../build/output/bin/tinyara_head.bin;       \
    exit'
```

Once the complete binaries are successfully programmed, each partition can be updated seperately with new one.
```bash
openocd -f artik053.cfg -c ' \
    flash_write os ../build/output/bin/tinyara_head.bin; exit'
```

## Configuration Sets

will be updated

#### nettest

#### tash

