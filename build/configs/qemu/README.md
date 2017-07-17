# QEMU

QEMU is a generic and open source machine emulator and virtualizer.

## Information

QEMU uses lm3s6963-ek machine.

## Environment Set-up
### QEMU installation

```
wget http://download.qemu-project.org/qemu-2.0.0.tar.xz
tar xvjf qemu-2.0.0.tar.xz
cd qemu-2.0.0
./configure --target-list=arm-softmmu
make -j 4
Check executable in "arm-softmmu/qemu-system-arm" 
```
## How to program a binary

There are two methods, using QEMU command or make command.  
After building Tizen RT, follow below steps at $TIZENRT_BASEDIR/os folder.  
TIZENRT_BASEDIR was set at [[Getting the sources]](../../../README.md#getting-the-sources) tab of Quick Start.

### Using QEMU command

```
qemu-system-arm -M lm3s6965evb -kernel ../build/output/bin/tinyara -nographic
```

### Using make command

```
make download
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
