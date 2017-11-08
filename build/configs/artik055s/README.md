# ARTIK055S

The ARTIK055S is a SOC for Wi-Fi™ IoT solutions. The ARTIK055S has a Wi-Fi subsystem, security subsystem, and application subsystem.

## Information

The Samsung ARTIK™ 055S Module is a highly integrated module for secure Internet of Things (IoT) devices that require Wi-Fi®. It is based on an ARM® Cortex® R4 core with on-chip memories, a complete 2.4GHz Wi-Fi® Phy, MAC layer processing, a large complement of standard digital buses, a PUF-based security system and power management. The module is packaged with additional external Flash memory, a hardware Secure Element and a single integrated 2.4GHz structural antenna.
Aimed especially at power-sensitive devices needing Wi-Fi®, the ARTIK055S Module provides excellent performance in a variety of environments, with a feature set tailored specifically for IoT end nodes.

### ARTIK055S Starter Kit PinOut
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
             3.3V <- *     | ARTIK055S |    10 -> XSPI1_CSN
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
  XGPIO26(gpio55)/XI2S0_SDO <- 13 | 14 -> VCC_EXT3P3
 XGPIO25(gpio54)/XI2S0_LRCK <- 15 | 16 -> XSPI0_CLK
            XGPIO24(gpio53) <- 17 | 18 -> XSPI0_CSN
  XGPIO27(gpio56)/XI2S0_SDI <- 19 | 20 -> XSPI0_MISO
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

## MemoryMap

8MB is allocated to the SPI Flash area. 1280 KB is prepared for operation in SRAM. If you want to know the physical memory address, see [[here]](../artik05x/README.md#memory-map-artik05x).

## Configuration Sets

After configuration, you can add / remove the configuration you want via menuconfig. If you want to run menuconfig, you need to install `kconfig-frontends`. kconfig-frontends was set at [[kconfig-frontends]](../../../README.md#set-kconfig-frontends) tab of Quick Start.

#### nettest

This is the basic configuration of ARTIK055S products. If you have an ARTIK055S product, use this config.

```bash
~/TizenRT$ cd os/tools
~/TizenRT/os/tools$ ./configure.sh artik055s/nettest
```

#### onboard

This is the configuration for the production phase of ARTIK055S product. If you have an ARTIK055S product, use this config.

```bash
~/TizenRT$ cd os/tools
~/TizenRT/os/tools$ ./configure.sh artik055s/onboard
```

#### using ARTIK-IDE

These configurations are intended for use in the ARTIK-IDE. The detailed functions are as follows.

* minimal
  * OS kernel
  * Customized C Library
  * Security(TLS)
  * BSP(boot loader)
  * Power
  * Networking support (LWIP, DHCP Client, Wi-Fi)

* typical
  * OS kernel
  * Customized C Library
  * Security(TLS)
  * BSP(boot loader)
  * Power
  * Networking support(LWIP, DHCP Client, DHCP Server, Wi-Fi)
  * Watchdog
  * ARTIK-SDK
  * JSON
  * Tash
  * System IO(I2C, UART, GPIO, PWM, SPI)
  * Network Utilities(FTP Client, Websock, Webclient, Webserver, NTP Client)

* extra
  * OS kernel
  * Customized C Library
  * Security(TLS)
  * BSP(boot loader)
  * Power
  * Networking support (LWIP, DHCP Client, DHCP Server, Wi-Fi)
  * Watchdog
  * ARTIK-SDK
  * JSON
  * Tash
  * I2S
  * DMA
  * System IO(I2C, UART, GPIO, PWM, SPI)
  * Network Utilities(FTP Client, Websock, Webclient, Webserver, NTP Client, CoAP, MQTT)

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
    flash_write bl1    ../build/configs/artik055s/bin/bl1.bin;  \
    flash_write bl2    ../build/configs/artik055s/bin/bl2.bin;         \
    flash_write sssfw  ../build/configs/artik055s/bin/sssfw.bin;       \
    flash_write wlanfw ../build/configs/artik055s/bin/wlanfw.bin;      \
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
gzip -c tinyara_head.bin-signed > factoryimage.gz
openocd -f artik05x.cfg -s ../build/configs/artik05x/scripts -c ' \
    flash_write factory    ../build/configs/artik055s/bin/factoryimage.gz;      \
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
