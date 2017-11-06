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

### ARTIK053 Starter Kit PinOut
#### CON710 CON711 CON708 CON709
```
                 XGPIO13(gpio42) <- SW702 | SW703 -> XGPIO15(gpio44)
                XGPIO16(gpio45) <- LED703 | LED702 -> XGPIO20(gpio49)
                                             [CON708]
                           |           |     * -> XI2C0_SCL
                           |           |     * -> XI2C0_SDA
                           |           |     * -> AREF
              [CON710]     |           |     * -> GND
               NC <- *     |           |    13 -> XSPI1_CLK
               NC <- *     |           |    12 -> XSPI1_MISO
            RESET <- *     |           |    11 -> XSPI1_MOSI
             3.3V <- *     | ARTIK053  |    10 -> XSPI1_CSN
               NC <- *     |           |     9 -> XPWMTOUT_4
              GND <- *     |           |     8 -> XGPIO21(gpio50)
              GND <- *     |           |     [CON709]
              Vin <- *     |           |     7 -> XGPIO19(gpio48)
              [CON711]     |           |     6 -> XPWMTOUT_2
            XADC0 <- A0    +-----------+     5 -> XPWMTOUT_1
            XADC1 <- A1                      4 -> XGPIO18(gpio47)
            XADC2 <- A2                      3 -> XPWMTOUT_0
            XADC3 <- A3                      2 -> XGPIO17(gpio46)
            XADC4 <- A4                      1 -> XUART0_TX
            XADC5 <- A5                      0 -> XUART0_RX

                   [CON703]
                   2 4 6 8 10 12 14 16 18 20 22 24
                   1 3 5 7  9 11 13 15 17 19 21 23
                   [CON704]
                   2 4 6 8 10 12 14 16 18 20 22 24
                   1 3 5 7  9 11 13 15 17 19 21 23
```
#### CON703
```
                 XPWMTOUT_1 <-  1 |  2 -> VCC_EXT3P3
                 XPWMTOUT_2 <-  3 |  4 -> XADC6
                 XPWMTOUT_3 <-  5 |  6 -> XADC7
                 XPWMTOUT_0 <-  7 |  8 -> XI2C1_SCL
                 XUART1_RXD <-  9 | 10 -> XI2C1_SDA
                 XUART1_TXD <- 11 | 12 -> GND
            XGPIO26(gpio55) <- 13 | 14 -> VCC_EXT3P3
            XGPIO25(gpio54) <- 15 | 16 -> XSPI0_CLK
            XGPIO24(gpio53) <- 17 | 18 -> XSPI0_CSN
            XGPIO23(gpio52) <- 19 | 20 -> XSPI0_MISO
            XGPIO22(gpio51) <- 21 | 22 -> XSPI0_MOSI
             XEINT0(gpio57) <- 23 | 24 -> GND
```
#### CON704
```
                 XPWMTOUT_4 <-  1 |  2 -> VCC_EXT3P3
                 XPWMTOUT_5 <-  3 |  4 -> XUART2_RXD
             XEINT2(gpio59) <-  5 |  6 -> XUART2_TXD
             XEINT1(gpio58) <-  7 |  8 -> XUART3_RXD
            XGPIO12(gpio41) <-  9 | 10 -> XUART3_TXD
            XGPIO10(gpio39) <- 11 | 12 -> GND
             XGPIO9(gpio38) <- 13 | 14 -> VCC_EXT3P3
            XGPIO11(gpio40) <- 15 | 16 -> XGPIO4(gpio33)/XSPI2_CLK
             XGPIO8(gpio37) <- 17 | 18 -> XGPIO5(gpio34)/XSPI2_CSN
             XGPIO2(gpio31) <- 19 | 20 -> XGPIO6(gpio35)/XSPI2_MISO
             XGPIO1(gpio30) <- 21 | 22 -> XGPIO7(gpio36)/XSPI2_MOSI
             XGPIO3(gpio32) <- 23 | 24 -> GND
```

### MemoryMap

8MB is allocated to the SPI Flash area. 1280 KB is prepared for operation in SRAM. Here is the physical memory address, see [[here]](scripts/README.md).

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

### Add USB device Rules

This is an optional environment.  
But as ARTIK is connected through USB, some operation like programming of binary can't be worked without this configuration.

1. Make a file named 99-\<anyname\>.rules.
2. Add below contents at above file.
```
SUBSYSTEMS=="usb",ATTRS{idVendor}=="0403",ATTRS{idProduct}=="6010",MODE="0666" RUN+="/sbin/modprobe ftdi_sio" RUN+="/bin/sh -c 'echo 0403 6010 > /sys/bus/usb-serial/drivers/ftdi_sio/new_id'"
```
3. Place the above file in /etc/udev/rules.d folder with sudo permission.
4. Reboot your system.


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

Before executing below steps, execute [generic steps](../../../tools/fs/README_ROMFS.md), step 1 and step 2.  
When you use artik053/iotivity config, you can execute only step 4. But note that if you want to resize the rom partition,  
you must modify partition_map.cfg matching the sizes in ARTIK053_FLASH_PART_LIST.

3. Modify partition configs  
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
    sh $TIZENRT_BASEDIR/../tools/fs/mkromfsimg.sh
    ```
6. Modify partition map to *$TIZENRT_BASEDIR/../build/configs/artik053/tools/openocd/partition_map.cfg*
    ```bash
    user	{ "USER R/W"		0x04620000	0x000FA000  0 }
    rom  	{ "ROM FS"  		0x0471A000	0x00064000  0 }
    ```
7. Program a ROM image
    ```bash
    flash_write rom ../bin/romfs.img;    \
    ```
8. Mount on device
    ```bash
    mount -t romfs /dev/smart4rom9 /rom
    ```

## Configuration Sets

#### [audio](audio/README.md)
This can be used to test audio functionality.
#### [extra](extra/README.md)
This can be used to use full functionality of Tizen RT on ARTIK053.
#### [hello](hello/README.md)
This provides simple hello world application including kernel.
#### [iotivity](iotivity/README.md)
This can be used to use Iotivity (OCF).
#### [iotjs](iotjs/README.md)
This can be used to use IoT.js
#### [kernel_sample](kernel_sample/README.md)
This can be used to test kernel functionality.
#### [minimal](minimal/README.md)
This can be used to use the minimal functionality of Tizen RT on ARTIK053.
#### [nettest](nettest/README.md)
This can be used to test network functionality.
#### [st_things](st_things/README.md)
This can be used to test Smart Things Things SDK functionality.
#### [tash](tash/README.md)
This can be used to use Tizen RT Shell(TASH).
#### [tc](tc/README.md)
This can be used to test database, file system, kernel, network functionality of Tizen RT on ARTIK053.
#### [typical](typical/README.md)
This can be used to use typical functionality of Tizen RT on ARTIK053.
#### [wifi_test](wifi_test/README.md)
This can be used to test wifi functionality of Tizen RT on ARTIK053.
