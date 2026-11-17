# esp_wrover_kit

Espressif ESP-WROVER-KIT is a highly integrated ultra-low-power development board, 
achieving great performance with a 4.5 MB RAM and a dual-core 240 MHz CPU. Create 
Internet cameras, smart displays or Internet radios by connecting LCDs, microphones 
and codecs to it.

## Contents

> [Information](#information)  
> [Environment Set-up](#environment-set-up)  
> [How to use GDB](#how-to-use-gdb)  

## Information

will be updated

## Manual Environment Set-up

### 1. Getting the toolchain
Get the build in binaries and libraries

64-bit Linux：

[xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz](https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz)

32-bit Linux：

[xtensa-esp32-elf-linux32-1.22.0-80-g6c4433a-5.2.0.tar.gz](https://dl.espressif.com/dl/xtensa-esp32-elf-linux32-1.22.0-80-g6c4433a-5.2.0.tar.gz)

```bash
tar xvf xtensa-esp32-elf-[linux*]-1.22.0-80-g6c4433a-5.2.0.tar.gz
export PATH=<Your Toolchain PATH>:$PATH
```

### 2. Getting TizenRT source code

```bash
git clone https://github.com/Samsung/TizenRT.git
cd TizenRT
TIZENRT_BASEDIR="$PWD"
```
**Note**: To contribute in this community, you need to clone your forked private repository instead.  
          Github guides this by [working-with-forks](https://help.github.com/articles/working-with-forks).

### 3. How to Build

Configure the build from $TIZENRT_BASEDIR/os/tools directory

```bash
cd os
./tools/configure.sh <board>/<configuration_set>
```

This command retrieves the specific pre-configured file named defconfig and Make.defs according to \<board\>/\<configuration_set\>.  
You can see collection of all configuration files at *$TIZENRT_BASEDIR/build/configs*.  
To check all pre-defined configurations, type as follows:

```bash
./configure.sh --help
```

After configuring above, configuration can be modified through *make menuconfig* from *$TIZENRT_BASEDIR/os*.

```bash
cd ..
make menuconfig
```

Refer [kconfig-frontend installation](docs/HowtoInstallKconfigFrontend.md) to use *menuconfig*.

Export your toolchain:

```bash
cd os
source setenv.sh <Your Toolchain PATH>
```
Or, 

```bash
export PATH=<Your Toolchain PATH>:$PATH
```

Finally, initiate build by make from *$TIZENRT_BASEDIR/os*.
```bash
make
```

### 4. How to burn images

After compiling,  "tinyara.bin" are built in the $TIZENRT_BASEDIR/build/ directory.
Burn the image by performing the following commands in $TIZENRT_BASEDIR/os/:

Burn ALL the images:

 ```bash
cd $TIZENRT_BASEDIR/os
make download ALL
```

Burn the bootloader:

 ```bash
cd $TIZENRT_BASEDIR/os
make download BOOTLOADER
```

Burn the TizenRT image:

 ```bash
cd $TIZENRT_BASEDIR/os
make download APP
```

## How to use GDB

1. Run OpenOCD daemon.

    ```bash
    @ubuntu:~/TizenRT/os$ ../build/tools/openocd/linux64/openocd-esp32 -s ../build/configs/esp_wrover_kit/scripts -f esp32.cfg
    Open On-Chip Debugger  v0.10.0-esp32-20191008 (2019-10-08-14:50)
    Licensed under GNU GPL v2
    For bug reports, read
            http://openocd.org/doc/doxygen/bugs.html
    none separate
    adapter speed: 20000 kHz
    Info : Configured 2 cores
    Info : Listening on port 6666 for tcl connections
    Info : Listening on port 4444 for telnet connections
    Error: type 'esp32' is missing virt2phys
    Info : ftdi: if you experience problems at higher adapter clocks, try the command "ftdi_tdo_sample_edge falling"
    Info : clock speed 20000 kHz
    Info : JTAG tap: esp32.cpu0 tap/device found: 0x120034e5 (mfg: 0x272 (Tensilica), part: 0x2003, ver: 0x1)
    Info : JTAG tap: esp32.cpu1 tap/device found: 0x120034e5 (mfg: 0x272 (Tensilica), part: 0x2003, ver: 0x1)
    Info : esp32: Debug controller 0 was reset.
    Info : esp32: Core 0 was reset.
    Info : Listening on port 3333 for gdb connections
    ```

2. Run GDB at another terminal.  
    Please find ```<-- COMMAND``` at below logs.

    ```bash
    @ubuntu:~/TizenRT/os$ xtensa-esp32-elf-gdb ../build/output/bin/tinyara.elf   <-- COMMAND
    GNU gdb (crosstool-NG crosstool-ng-1.22.0-80-g6c4433a) 7.10
    Copyright (C) 2015 Free Software Foundation, Inc.
    License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
    This is free software: you are free to change and redistribute it.
    There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
    and "show warranty" for details.
    This GDB was configured as "--host=x86_64-build_pc-linux-gnu --target=xtensa-esp32-elf".
    Type "show configuration" for configuration details.
    For bug reporting instructions, please see:
    <http://www.gnu.org/software/gdb/bugs/>.
    Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.
    For help, type "help".
    Type "apropos word" to search for commands related to "word"...
    Reading symbols from ../build/output/bin/tinyara.elf...done.
    (gdb) set pagination off    <-- COMMAND
    (gdb) target remote :3333   <-- COMMAND
    Remote debugging using :3333
    __start () at chip/esp32_start.c:101
    101     chip/esp32_start.c: No such file or directory.
    (gdb) set remote hardware-watchpoint-limit 2   <-- COMMAND
    (gdb) mon reset halt   <-- COMMAND
    JTAG tap: esp32.cpu0 tap/device found: 0x120034e5 (mfg: 0x272 (Tensilica), part: 0x2003, ver: 0x1)
    JTAG tap: esp32.cpu1 tap/device found: 0x120034e5 (mfg: 0x272 (Tensilica), part: 0x2003, ver: 0x1)
    cpu0: Debug controller 0 was reset.
    cpu0: Core 0 was reset.
    cpu0: Target halted, PC=0x500000CF, debug_reason=00000000
    esp32: Core 0 was reset.
    esp32: Debug controller 1 was reset.
    esp32: Core 1 was reset.
    Target halted. CPU0: PC=0x40000400 (active)
    Target halted. CPU1: PC=0x40000400
    (gdb) flushregs    <-- COMMAND
    Register cache flushed.
    (gdb) thb __start
    Hardware assisted breakpoint 1 at 0x400808d4: file chip/esp32_start.c, line 101.
    (gdb) c
    Continuing.
    Target halted. CPU0: PC=0x400808D4 (active)
    Target halted. CPU1: PC=0x400076DD

    Temporary breakpoint 1, __start () at chip/esp32_start.c:101
    101     in chip/esp32_start.c
    (gdb)
    ```

    ```help``` will help you to find GDB commands.
