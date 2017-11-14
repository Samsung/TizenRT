# QEMU

QEMU is a generic and open source machine emulator and virtualizer.

## Information

Targets Supported:

1. lm3s6963-ek  
NOTE:  
Tizen RT supports two memory options for lm3s6963-ek board;
- 64KB RAM requirement configuration which matches with real memory of lm3s6963-ek board
- 16MB RAM requirement which can be run on QEMU only with the patch in TizenRT/build/configs/qemu/qemu-2.10.0-rc2_increase_ram_size.patch

## Environment Set-up
### QEMU installation

```
wget http://download.qemu-project.org/qemu-2.10.0-rc2.tar.xz
tar xvJf qemu-2.10.0-rc2.tar.xz
cd qemu-2.10.0-rc2
/* Copy qemu-2.10.0-rc2_increase_ram_size.patch from TizenRT/build/configs/qemu */
patch -p1 < qemu-2.10.0-rc2_increase_ram_size.patch
./configure --target-list=arm-softmmu
make -j 4
cd arm-softmmu/
sudo ln qemu-system-arm /usr/local/bin/qemu-system-arm
```
## How to program a binary

There are two methods, using QEMU command or make command.  
After building Tizen RT, follow below steps at $TIZENRT_BASEDIR/os folder.  
TIZENRT_BASEDIR was set at [[Getting the sources]](../../../README.md#getting-the-sources) tab of Quick Start.

### Using QEMU command

```
qemu-system-arm -M lm3s6965evb -kernel ../build/output/bin/tinyara -nographic -gdb tcp::3333
```

### Using make command

```
make download
```

## How to debug

To debug Tizen RT on QEMU, GDB should be connected through below commands.

```
arm-none-eabi-gdb
(gdb) file ../build/output/bin/tinyara
(gdb) target remote:3333
```

## Configuration Sets
### tash_64k
for running tash under 256KB flash and 64KB sram

### tash_16m
for running tash under 128MB flash and 16MB sram

## APPENDIX
### How to change memory size
If you want to set your sram to 64KB or 16MB, follow these steps.  
`make menuconfig`  
Enter `Board Selection` -> `Build for qemu hardware`  
Select `NONE`(64KB) or `Build for SRAM increased QEMU Hardware`(16MB)  
Enter `Chip Selection` -> `Boot Memory Configuration`  
Set `Primary RAM size` to `65536`(64KB) or `16777216`(16MB)
