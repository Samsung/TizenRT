# ARTIK 053

The ARTIK 053 is a SOC for Wi-Fi™ IoT solutions. The ARTIK 053 has a Wi-Fi subsystem, security subsystem, and application subsystem.

## Contents

> [Information](#information)  
> [Environment Set-up](#environment-set-up)  
> [How to program a binary](#how-to-program-a-binary)  
> [ROMFS](#romfs)  
> [Configuration Sets](#configuration-sets)

## Information

The Samsung ARTIK™ 053 Module is a highly integrated module for secure Internet of Things (IoT) devices that require Wi-Fi®. It is based on an ARM® Cortex® R4 core with on-chip memories, a complete 2.4GHz Wi-Fi® Phy, MAC layer processing, a large complement of standard digital buses, a PUF-based security system and power management. The module is packaged with additional external Flash memory, a hardware Secure Element and a single integrated 2.4GHz structural antenna.
Aimed especially at power-sensitive devices needing Wi-Fi®, the ARTIK 053 Module provides excellent performance in a variety of environments, with a feature set tailored specifically for IoT end nodes.

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

## How to program a binary

There are two methods, using OpenOCD or script.

After building Tizen RT, follow below steps at $TIZENRT_BASEDIR/os folder.

TIZENRT_BASEDIR was set at [[Getting the sources]](../../../README.md#getting-the-sources) tab of Quick Start.

### Using download script

```bash
make download ALL
```
This makes complete set of binaries programmed.

### Using OpenOCD

This is used to program a partial binary.

Export 'OPENOCD_SCRIPTS' to environment variable.

```bash
export OPENOCD_SCRIPTS=$TIZENRT_BASEDIR/build/configs/artik053/tools/openocd
```

At first, programming the complete set of binaries are needed.

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

## ROMFS

Before executing below steps, execute [generic steps](../../../external/contents/README.md), step 1 and step 2.

3. Modify partition configs.  
    Below steps creates ROMFS partition with size 400KB at next of user partition.  
    1. Split user partition size from (1400) to (1000, 400) in ARTIK053_FLASH_PART_LIST
        ```bash
        Board Selection -> change values at Flash partition size list (in KBytes)
        ```
    2. Append "romfs" at next of *smartfs* to ARTIK053_FLASH_PART_TYPE
        ```bash
        Board Selection -> append string at Flash partition type list
        ```
    3. Append "rom" at next of *user* to ARTIK053_FLASH_PART_NAME
        ```bash
        Board Selection -> append string at FLash partition name list
        ```
4. Build Tizen RT  
5. Prepare ROM image
    ```bash
    sh $TIZENRT_BASEDIR/../apps/tools/mkromfsimg.sh
    ```
6. Modify partition map to *$TIZENRT_BASEDIR/../build/configs/artik053/tools/openocd/partition_map.cfg*
    ```bash
    user	{ "USER R/W"		0x04620000	0x000FA000  0 }
    rom  	{ "ROM FS"  		0x0471A000	0x00064000  0 }
    ```
7. Program a ROM image
    ```bash
    flash_write rom ../bin/rom.img;    \
    ```
8. Mount on device
    ```bash
    mount -t romfs /dev/smart4rom9 /rom
    ```

## Configuration Sets

will be updated

#### nettest

#### tash

