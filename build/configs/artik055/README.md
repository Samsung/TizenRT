# ARTIK055S

The ARTIK055S is a SOC for Wi-Fi™ IoT solutions. The ARTIK055S has a Wi-Fi subsystem, security subsystem, and application subsystem.

## Information

The Samsung ARTIK™ 055 Module is a highly integrated module for secure Internet of Things (IoT) devices that require Wi-Fi®. It is based on an ARM® Cortex® R4 core with on-chip memories, a complete 2.4GHz Wi-Fi® Phy, MAC layer processing, a large complement of standard digital buses, a PUF-based security system and power management. The module is packaged with additional external Flash memory, a hardware Secure Element and a single integrated 2.4GHz structural antenna.
Aimed especially at power-sensitive devices needing Wi-Fi®, the ARTIK055S Module provides excellent performance in a variety of environments, with a feature set tailored specifically for IoT end nodes.

## MemoryMap

8MB is allocated to the SPI Flash area. 1280 KB is prepared for operation in SRAM. If you want to know the physical memory address, see [[here]](scripts/README.md).

## Configuration Sets

After configuration, you can add / remove the configuration you want via menuconfig. If you want to run menuconfig, you need to install `kconfig-frontends`. kconfig-frontends was set at [[kconfig-frontends]](../../../README.md#set-kconfig-frontends) tab of Quick Start.

#### nettestsecure

This is the basic configuration of ARTIK055S products. If you have an ARTIK055S product, use this config.

```bash
~/TizenRT$ cd os/tools
~/TizenRT/os/tools$ ./configure.sh artik055/nettestsecure
```

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
export OPENOCD_SCRIPTS=$TIZENRT_BASEDIR/build/tools/openocd
```

At first, programming the complete set of binaries are needed.

#### ARTIK055S
```bash
../build/configs/artik05x/tools/codesigner/artik05x_codesigner -sign ../build/output/bin/tinyara_head.bin

openocd -f artik05x.cfg -s ../build/configs/artik05x/scripts -c ' \
    flash_write bl1    ../build/configs/artik055/bin/bl1.bin;  \
    flash_write bl2    ../build/configs/artik055/bin/bl2.bin;         \
    flash_write sssfw  ../build/configs/artik055/bin/sssfw.bin;       \
    flash_write wlanfw ../build/configs/artik055/bin/wlanfw.bin;      \
    flash_write os     ../build/output/bin/tinyara_head.bin-signed;   \
    exit'
```

Once the complete binaries are successfully programmed, each partition can be updated separately with new one.

```bash
openocd -f artik05x.cfg -s ../build/configs/artik05x/scripts -c ' \
    flash_write os ../build/output/bin/tinyara_head.bin-signed; exit'
```

### Factory Reset
If you can not boot normally, you can change os to the initial version. This is possible if there is an initialization binary in memory.

#### How to Download the Initialization Binaries
You can download it using OpenOCD. You compress the compiled firmware and download it to the board.

```bash
gzip -c tinyara_head.bin > factoryimage.gz
openocd -f artik05x.cfg -s ../build/configs/artik05x/scripts -c ' \
    flash_write factory    ../build/configs/artik055/bin/factoryimage.gz;      \
    exit'
```

#### How to enter initialization mode
When you press the RESET button (SW700) to reboot the Starter Kit, press and hold the 'ARDUINO RESET' button (SW701) for 10 seconds. Enter initialization mode as follows.
```
.....
Factory reset.
Erasing boot partitions...
....................................e
Erased 600 sectors
Flashing factory image...
Uncompressed size: 1258496 = 0x133400
resetting ...

........ <RESET>.....................
U-Boot 2017
.....
```
