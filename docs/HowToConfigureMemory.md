# How to configure Memory

## Contents
> [Configuration](#configuration)  
> [Linker Scripts](#linker-scripts)  
> [Multi-Heap Support](#multi-heap-support)

## Configuration
There are two configurations to configure memory, CONFIG_RAM_REGIONx_START and CONFIG_RAM_REGIONx_SIZE.  
Menuconfig helps to change them as shown below:
```
cd $TIZENRT_BASEDIR
cd os
make menuconfig
```
Find configs with following step:
```
Hardware Configuration -> Chip selection -> Boot Memory Configuration
```
Set CONFIG_RAM_REGIONx_START as hexa value and set CONFIG_RAM_REGIONx_SIZE as decimal value in bytes.
```
CONFIG_RAM_REGIONx_START=0x02023800
CONFIG_RAM_REGIONx_SIZE=968704
```
Heap allocation will use these values for setting heap start and end address.

## Linker Scripts
Linker script has RAM start and size information also.  
file path : *build/configs/<BOARD_NAME>/scripts/<SCRIPTS_NAME>.ld*  

```
MEMORY
{
...
	SRAM	(rwx)	: ORIGIN = 0x02023800, LENGTH = 946K
...
}
```
These value should be matched with TizenRT configuration file.

## Multi-Heap Support
TizenRT can support multi-heap, if there is another RAM partition in H/W with discontinuous address to previous one.  
Please refer [How to use multiple memory map as a heap](HowToUseMultiHeap.md).


