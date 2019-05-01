# ARTIK055S

The ARTIK055S is a SOC for Wi-Fi™ IoT solutions. The ARTIK055S has a Wi-Fi subsystem, security subsystem, and application subsystem.

## Contents

> [Information](#information)  
> [Configuration Sets](#configuration-sets)  
> [Environment Set-up](#environment-set-up)  
> [How to program a binary](#how-to-program-a-binary)  
> [ROMFS](#romfs)  
> [How to use GDB](#how-to-use-gdb)  
> [Appendix](#appendix)

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
            XADC0 <- A0     -----------+     5 -> XPWMTOUT_1
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

### MemoryMap

8MB is allocated to the SPI Flash area. 1280 KB is prepared for operation in SRAM. Here is the physical memory address, see [[here]](../artik05x/README.md#memory-map-artik05x).

## Configuration Sets

#### [audio](audio/README.md)
This can be used to test audio functionality.
#### [nettest](nettest/README.md)
This can be used to test network functionality.

## Environment Set-up
This section covers board-specific environment set-up.  
Please set TizenRT common environment, [quick start](https://github.com/Samsung/TizenRT#quick-start), first before doing below.

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

After building TizenRT, execute below command at $TIZENRT_BASEDIR/os folder.  
See [[Getting the sources]](https://github.com/Samsung/TizenRT#getting-the-sources) for how to set *TIZENRT_BASEDIR*.

```bash
./dbuild.sh download [Programming Option]
```

For example, *ALL* option makes complete set of binaries programmed.
```bash
./dbuild.sh download ALL
```
See programming options list which is pre-defined at [.flashSpec.xml](.flashSpec.xml).

Refer [How to program using OpenOCD](#how-to-program-using-openocd) to make user-specific programming seqeunce.

### Factory Reset
If you can not boot normally, you can change os to the initial version. This is possible if there is an initialization binary in memory.

#### How to Download the Initialization Binaries
You can download it using OpenOCD. You compress the compiled firmware and download it to the board.

```bash
gzip -c tinyara_head.bin-signed > factoryimage.gz
openocd -f artik05x.cfg -s ../build/configs/artik05x/scripts -c ' \
    flash erase_part ota ; \
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

## ROMFS

>[How to set romfs partition](https://github.com/Samsung/TizenRT/blob/master/os/board/common/README.md#how-to-set-romfs-partition)

## How to use GDB
1. Run OpenOCD daemon.
    ```bash
    @ubuntu:~/TizenRT/os$ ../build/tools/openocd/linux64/openocd -f artik05x.cfg -s ../build/configs/artik05x/scripts/
    Open On-Chip Debugger 0.10.0-dirty (2017-09-02-08:32)
    Licensed under GNU GPL v2
    For bug reports, read
    	http://openocd.org/doc/doxygen/bugs.html
    adapter speed: 2000 kHz
    Info : auto-selecting first available session transport "jtag". To override use 'transport select <transport>'.
    force hard breakpoints
    trst_and_srst separate srst_gates_jtag trst_push_pull srst_push_pull connect_deassert_srst
    adapter_nsrst_assert_width: 50
    adapter_nsrst_delay: 100
    debug_level: -1
    ```
2. Run GDB at another terminal.  
    Please find ```<-- COMMAND``` at below logs.
    ```bash
    @ubuntu:~/TizenRT/os$ arm-none-eabi-gdb ../build/output/bin/tinyara   <-- COMMAND
    GNU gdb (GNU Tools for ARM Embedded Processors) 7.8.0.20150604-cvs
    Copyright (C) 2014 Free Software Foundation, Inc.
    License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
    This is free software: you are free to change and redistribute it.
    There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
    and "show warranty" for details.
    This GDB was configured as "--host=i686-linux-gnu --target=arm-none-eabi".
    Type "show configuration" for configuration details.
    For bug reporting instructions, please see:
    <http://www.gnu.org/software/gdb/bugs/>.
    Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.
    For help, type "help".
    Type "apropos word" to search for commands related to "word"...
    Reading symbols from TizenRT/build/output/bin/tinyara...done.
    (gdb) target remote:3333   <-- COMMAND
    Remote debugging using :3333
    0x00003834 in ?? ()
    (gdb) monitor reset halt   <-- COMMAND
    target halted in ARM state due to debug-request, current mode: Supervisor
    cpsr: 0x60000153 pc: 0x040db8a8
    D-Cache: enabled, I-Cache: enabled
    (gdb) monitor cortex_r4 maskisr on   <-- COMMAND
    cortex_a interrupt mask on
    (gdb) b hello_main
    Breakpoint 1 at 0x40d30fc: file hello_main.c, line 196.
    (gdb) b up_assert
    Breakpoint 2 at 0x40c9364: file armv7-r/arm_assert.c, line 945.
    (gdb) c
    Continuing.

    Breakpoint 1, hello_main (argc=1, argv=0x202fd1c) at hello_main.c:196
    196	{
    (gdb) s
    197		sleep(20);
    (gdb) s
    sleep (seconds=seconds@entry=20) at unistd/lib_sleep.c:155
    155		if (seconds) {
    (gdb) c
    Continuing.

    Breakpoint 2, up_assert (filename=filename@entry=0x40ea757 "hello_main.c", 
      lineno=lineno@entry=198) at armv7-r/arm_assert.c:945
    945		lldbg("Assertion failed at file:%s line: %d task: %s\n", filename, lineno, this_task()->name);
    (gdb) 
    ```
```help``` will help you to find GDB commands.


## Appendix
### On Chip Debugger installation

OpenOCD is used to program and debug.

OpenOCD v0.10.0 is recommended and can be installed like below,
but **pre-built OpenOCD binaray on tools/openocd/linux64(or 32) can be used without installing**.
```bash
sudo apt-get build-dep openocd
git clone --depth 1 -b v0.10.0 https://git.code.sf.net/p/openocd/code openocd-code
cd openocd-code
./bootstrap
./configure
make
sudo make install
```

### How to program using OpenOCD
```bash
../build/configs/artik05x/tools/codesigner/artik05x_AppCodesigner ../build/configs/artik05x/tools/codesigner/rsa_private.key ../build/output/bin/tinyara_head.bin

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
