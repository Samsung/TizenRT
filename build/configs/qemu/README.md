# QEMU

QEMU is a generic and open source machine emulator and virtualizer.

## Information

Targets Supported:

- **lm3s6963-ek**

 ***NOTE:*** TizenRT supports three memory options for lm3s6963-ek board.

  - 64KB RAM requirement configuration which matches with real memory of ***lm3s6963-ek*** board

  - 1MB RAM requirement which can be run on QEMU only with the [qemu-2.12.0-rc1_1m_ram_size.patch](https://github.com/Samsung/TizenRT/blob/master/build/configs/qemu/qemu-2.12.0-rc1_1m_ram_size.patch)

  - 16MB RAM requirement which can be run on QEMU only with the [qemu-2.12.0-rc1_16m_ram_size.patch](https://github.com/Samsung/TizenRT/blob/master/build/configs/qemu/qemu-2.12.0-rc1_16m_ram_size.patch)

## Environment Set-up
This section covers board-specific environment set-up.  
Please set TizenRT common environment, [quick start](https://github.com/Samsung/TizenRT#quick-start), first before doing below.

### QEMU installation

#### Qemu version used 2.12.0-rc1

```
wget https://download.qemu.org/qemu-2.12.0-rc1.tar.xz

tar xvJf qemu-2.12.0-rc1.tar.xz

cd qemu-2.12.0-rc1
```

To Run **QEMU with 1MB RAM**, copy **qemu-2.12.0-rc1_1m_ram_size.patch** from **TizenRT/build/configs/qemu** to **PWD**

```
patch -p1 < qemu-2.12.0-rc1_1m_ram_size.patch
```
To Run **QEMU with 16MB RAM**, copy **qemu-2.12.0-rc1_16m_ram_size.patch** from **TizenRT/build/configs/qemu** to **PWD**
```
patch -p1 < qemu-2.12.0-rc1_16m_ram_size.patch
```
```
./configure --target-list=arm-softmmu

make -j 4

cd arm-softmmu/

sudo ln qemu-system-arm /usr/local/bin/qemu-system-arm
```
## How to program a binary

There are two methods,
- Using **qemu** command

- Using **make** command.  

After building TizenRT, follow below steps at $TIZENRT_BASEDIR/os folder.

See [[Getting the sources]](https://github.com/Samsung/TizenRT#getting-the-sources) for how to set *TIZENRT_BASEDIR*.

### Using QEMU command

```
qemu-system-arm -M lm3s6965evb -kernel ../build/output/bin/tinyara -nographic -gdb tcp::3333
```

### Using make command

```
./dbuild.sh download
```

## How to debug

To debug TizenRT on QEMU, GDB should be connected through below commands.

```
arm-none-eabi-gdb
(gdb) file ../build/output/bin/tinyara
(gdb) target remote:3333
```

## Configuration Sets
### tc_64k
for running tc under 256KB flash and 64KB sram

### tc_1m
for running tc under 32MB flash and 1MB sram

### tc_16m
for running tc under 128MB flash and 16MB sram

## APPENDIX
### How to change memory size
If you want to set your sram to 64KB or 16MB, follow these steps.  
`make menuconfig`

Enter `Board Selection` -> `Build for qemu hardware`  
Select `NONE`(64KB) or `Build for SRAM increased QEMU Hardware`(16MB)

Enter `Chip Selection` -> `Boot Memory Configuration`  
Set `Primary RAM size` to `65536`(64KB) or `16777216`(16MB)

### How to run network stack on QEMU
To run the network stack on QEMU please refer [How to run network stack on Qemu](HowToRunNetworkStackOnQemu.md).
