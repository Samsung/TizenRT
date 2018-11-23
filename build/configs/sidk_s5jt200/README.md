# sidk_s5jt200

Samsung IoT Development Kit for S5JT200 chipset.

## Contents

> [Information](#information)  
> [Environment Set-up](#environment-set-up)  
> [How to program a binary](#how-to-program-a-binary)  
> [ROMFS](#romfs)  
> [Using GDB](#using-gdb)  
> [Configuration Sets](#configuration-sets)  
> [Board Configuration](#board-configuration)

## Information

will be updated

## Environment Set-up
This section covers board-specific environment set-up.  
Please set TizenRT common environment, [quick start](https://github.com/Samsung/TizenRT#quick-start), first before doing below.

### On Chip Debugger installation

on Ubuntu 13.10 ~ 14.xx version
```bash
sudo apt-get install -y lib32z1 lib32ncurses5 lib32bz2-1.0
```

on Ubuntu over 16.xx version
```bash
sudo dpkg --add-architecture i386
sudo apt-get update
sudo apt-get install libc6:i386 libncurses5:i386 libstdc++6:i386 lib32z1
```

### FTDI installation

Install the package for usb
```bash
sudo apt-get install libusb-dev
```

Get the package, [libftdi-0.19](http://www.intra2net.com/en/developer/libftdi/download/libftdi-0.19.tar.gz)

Untar the downloaded package
```bash
tar -zxvf libftdi-0.19.tar.gz
```

Build and install the package for libftdi
```bash
cd libftdi-0.19
./configure
make
sudo make install
```

Link the built files
```bash
cd /usr/lib
sudo ln -s /usr/local/lib/libftdi.a libftdi.a
sudo ln -s /usr/local/lib/libftdi.la libftdi.la
sudo ln -s /usr/local/lib/libftdi.so.1.19.0 libftdi.so.1.19.0
sudo ln -s /usr/local/lib/libftdi.so.1.19.0 libftdi.so
sudo ln -s /usr/local/lib/libftdi.so.1.19.0 libftdi.so.1
```

### FT2232 interface driver installation

Get the package, [libftd2xxx1.0.4](https://github.com/psi46/HDItest/blob/master/FTDI-1.0.4/libftd2xx1.0.4.tar.gz)

Untar the downloaded package
```bash
tar -zxvf libftd2xx1.0.4.tar.gz
```

Copy and link the files
```bash
cd libftd2xx1.0.4
sudo cp ftd2xx.h /usr/include
sudo cp WinTypes.h /usr/include
sudo cp build/x86_64/libftd2xx.so.1.0.4 /usr/local/lib
cd /usr/local/include
sudo ln -s /usr/include/ftd2xx.h ftd2xx.h
sudo ln -s /usr/include/WinTypes.h WinTypes.h
cd /usr/local/lib
sudo ln -s libftd2xx.so.1.0.4 libftd2xx.so
cd /usr/lib
sudo ln -s /usr/local/lib/libftd2xx.so.1.0.4 libftd2xx.so
```

### Add USB device Rules

This is an optional environment.  
But as this board is connected through USB, some operation like programming of binary can't be worked without this configuration.

1. Make a file named 99-\<anyname\>.rules.
2. Add below contents at above file.
```
SUBSYSTEMS=="usb",ATTRS{idVendor}=="0403",ATTRS{idProduct}=="6010",MODE="0666" RUN+="/sbin/modprobe ftdi_sio" RUN+="/bin/sh -c 'echo 0403 6010 > /sys/bus/usb-serial/drivers/ftdi_sio/new_id'"
```
3. Place the above file in /etc/udev/rules.d folder with sudo permission.
4. Reboot your system.

## How to program a binary

After building a TizenRT, execute below at $TIZENRT_BASEDIR/os folder.  
See [[Getting the sources]](https://github.com/Samsung/TizenRT#getting-the-sources) for how to set *TIZENRT_BASEDIR*.
```bash
./dbuild.sh download ALL
```

## ROMFS

Before executing below steps, execute [generic steps](../../../tools/fs/README_ROMFS.md), step 1 and step 2.

3. Modify partition configs.  
    Below steps creates ROMFS partition with size 256KB at the end,  
    where romfs device is a ftl mtd block device and romfs filesystem will be mounted on ftl mtd device "/dev/mtdblockX"
    1. Split last partition size to (256, 256) in SIDK_S5JT200_FLASH_PART_LIST
        ```bash
        Hardware Configuration -> Board Selection -> change values at Flash partition size list (in KBytes)
        ```
    2. Append "romfs," at end to SIDK_S5JT200_FLASH_PART_TYPE
        ```bash
        Hardware Configuration -> Board Selection -> append string at Flash partition type list
        ```
    3. Append "rom," at end to SIDK_S5JT200_FLASH_PART_NAME
        ```bash
        Hardware Configuration -> Board Selection -> append string at FLash partition name list
        ```
4. Enable auto-mount config.
    ```bash
    Hardware Configuration -> Board Selection -> Automount partitions -> [*] Automount ROM read only partition
    ```
5. Build TizenRT and flash a binary.  
    Refer [How to program a binary](#how-to-program-a-binary).

## Using GDB
1. Build TizenRT and program a TizenRT binary through above [method](#how-to-program-a-binary)

2. Run GDB server by running openocd with gdb cfg
    ```bash
    cd $TIZENRT_BASEDIR/build/configs/sidk_s5jt200/tools/openocd/
    ./linux64/openocd -f s5jt200_attach_gdb.cfg
    ```
3. Run GDB client from another terminal
    ```bash
    cd $TIZENRT_BASEDIR/os/
    arm-none-eabi-gdb -ex "target remote :3333" $TIZENRT_BASEDIR/build/output/bin/tinyara
    ```
    1. To run tinyara from beginning, set entrypoint to pc register in gdb.
    ```bash
    (gdb) set $pc = entry_addr
    ```
    2. entry_addr can be obtained by
    ```bash
    arm-none-eabi-readelf -h $TIZENRT_BASEDIR/build/output/bin/tinyara
    ```

## Configuration Sets

There are three configuration sets for sidk_s5jt200, including 'hello_with_tash', 'kernel_sample', and 'tc'.

#### hello_with_tash
 for running a hello example with shell enabled

#### kernel_sample
 for running kernel functions

#### tc
 for running unit test cases

#### sidk_tash_aws
 for running AWS IoT SDK examples.


## Board Configuration

### Board Layout
![SIDK_S5JT200 Layout](media/SIDK_Layout.png)

### JTAG selection
There are two JTAG ports, one is OpenOCD through USB port and the other is normal JTAG interface like Lauterbach Trace-32.

![J3 : JTAG Selection](media/J3.png)

J3 Header pin is used to select JTAG port.

Pin  | OpenOCD | Normal JTAG
---  | ------- | --------
1-2  | Connect | Open
3-4  | Connect | Open
5-6  | Connect | Open
7-8  | Connect | Open
9-10 | Connect | Open

### UART source selection for CON3
One of UART0, UART1, UART2, UART3, or UARTDBG channel could be connected to CON3 through the J2 configuration.

![J2 : CON3 source selection](media/J2.png)

Pin   | UART0   | UART1   | UART2   | UART3   | UARTDBG<br>(CON3) | UARTDBG<br>(CON4)
---   | -----   | -----   | -----   | -----   | ------- | ---------------
1-2   | Open    | Open    | Open    | Open    | Connect | Open
3-4   | Open    | Open    | Open    | OPen    | Connect | Open
7-8   | Connect | Open    | Open    | OPen    | Open    | Open
9-10  | Connect | Open    | Open    | OPen    | Open    | Open
11-12 | Open    | Connect | Open    | OPen    | Open    | Open
13-14 | Open    | Connect | Open    | OPen    | Open    | Open
15-16 | Open    | Open    | Connect | OPen    | Open    | Open
17-18 | Open    | Open    | Connect | OPen    | Open    | Open
19-20 | Open    | Open    | Open    | Connect | Open    | Open
21-22 | Open    | Open    | Open    | Connect | Open    | Open

### UART source selection for CON5
UART source for CON5 could be selected through J7 configuration.

![J7 : CON5 source selection](media/J7.png)

Pin   | UART0   | UART1   | UART2   | UART3
---   | -----   | -----   | -----   | -----
1-2   | Connect | Open    | Open    | OPen
3-4   | Connect | Open    | Open    | OPen
5-6   | Open    | Connect | Open    | OPen
7-8   | Open    | Connect | Open    | OPen
9-10  | Open    | Open    | Connect | OPen
11-12 | Open    | Open    | Connect | OPen
13-14 | Open    | Open    | Open    | Connect
15-16 | Open    | Open    | Open    | Connect

### Console Terminal
UARTDBG channel is being used as Console and it could be connected to CON3 or CON4 with J2 and J4 pin configuration.


![J4 : CON4 source selection](media/J4.png)

Pin      | UARTDBG<br>on CON3<br>(SERIAL) | UARTDBT<br>on CON4<br>(USB)
-------- | --------------- | ---------------
J2 : 1-2 | Connect         | Open
J2 : 3-4 | Connect         | Open
J4 : 1-2 | Open            | Connect
J4 : 3-4 | Open            | Connect


