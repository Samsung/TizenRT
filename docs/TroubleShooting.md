# Trouble Shooting

## Contents
> [Common](#common)  
> [Board-Specific](#board-specific)

## Common
### Issues on GNU toolchain
When 64 bit machine tries to use 32 bit package like GDB, someone meets below:
```
'Launching XXX' has envountered a problem.
Could not determine GDB version after sending: /home/.../arm-none-eabi-gdb --version
```
```
bash: .../gcc-arm-none-eabi-4_9-2015q3/bin/arm-none-eabi-gcc: No such file or directory
```

Installing *lib32ncurses5* package resolves it.
```bash
sudo apt-get install lib32ncurses5
```

### Issues on Kconfig-frontend
When ```make menuconfig``` excutes after installing Kconfig-frontend, someone meets below:
```
kconfig-mconf: error while loading shared libraries: libkconfig-parser-x.xx.0.so: cannot open shared object file: No such file or directory
Makefile.unix:579: recipe for target 'menuconfig' failed
make: *** [menuconfig] Error 127
```
To resolve:
```
cd <Kconfig-frontend_package_PATH>
./configure --prefix=/usr
make
sudo make install
```

## Board-Specific
### ARTIK
#### Issues on Programming
When USB connection is not established, someone meets below:
```
[Command] make download ALL
Generating partition map ... Done
Open On-Chip Debugger 0.10.0-dirty (2017-09-02-08:32)
Licensed under GNU GPL v2
For bug reports, read
http://openocd.org/doc/doxygen/bugs.html
adapter speed: 2000 kHz
Info : auto-selecting first available session transport "jtag". To override use 'transport select '.
force hard breakpoints
trst_and_srst separate srst_gates_jtag trst_push_pull srst_push_pull connect_deassert_srst
adapter_nsrst_assert_width: 50
adapter_nsrst_delay: 100
debug_level: -1

Makefile.unix:543: recipe for target 'download' failed
make: *** [download] Error 1
[STATUS] FAILED
```

To resolve,  
1. See [USB device rules](https://github.com/Samsung/TizenRT/blob/master/build/configs/artik053/README.md#add-usb-device-rules).
2. Change or re-plugin USB cable and confirm USB connection.
