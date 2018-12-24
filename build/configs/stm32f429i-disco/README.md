# STM32F429I-DISCO

The STM32F429ZIT6 is a 180MHz Cortex-M4 operation with 2Mbit Flash memory and 256kbytes

## Contents

> [Information](#information)  
> [Environment Set-up](#environment-set-up)  
> [Install STLINK Tool](#install-stlink-tool)  
> [How to program a binary](#how-to-program-a-binary)  
> [Configuration Sets](#configuration-sets)  

## Information
The board features:  
	- On-board ST-LINK/V2 for programming and debugging,  
	- On-board 64 Mbits (8 Mbytes) External SDRAM (1 Mbit x 16-bit x 4-bank)  
	- L3GD20, ST MEMS motion sensor, 3-axis digital output gyroscope,  
	- TFT 2.4" LCD, 262K color RGB, 240 x 320 pixels  
	- Touchscreen controller  
	- Two user LEDs and two push-buttons,  
	- USB OTG FS with micro-AB connector, and  
	- Easy access to most MCU pins.  

## Environment Set-up
This section covers board-specific environment set-up.  
Please set TizenRT common environment, [quick start](https://github.com/Samsung/TizenRT#quick-start), first before doing below.

## Install STLINK Tool
This section covers installing ST-LINK/V2 tool on Linux.  
ST-LINK/V2 tool is required for programming and debugging.  
Please follow, (https://fishpepper.de/2016/09/16/installing-using-st-link-v2-to-flash-stm32-on-linux/)

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
## Configuration Sets

#### [hello](hello/README.md)
This provides simple hello world application including kernel.
#### [tc](tc/README.md)
This can be used to test kernel functionality of TizenRT
