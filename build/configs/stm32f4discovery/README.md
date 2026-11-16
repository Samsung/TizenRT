# STM32F407-DISC1

The STM32F407VG is a 168MHz Cortex-M4 operation with 1Mbit Flash memory and 128kbytes

## Contents

> [Information](#information)  
> [Environment Set-up](#environment-set-up)  
> [Install STLINK Tool](#install-stlink-tool)  
> [How to program a binary](#how-to-program-a-binary)  
> [Configuration Sets](#configuration-sets)  

## Information
The board features:  
	- On-board ST-LINK/V2-A for programming and debugging,  
	- LIS3DSH, ST MEMS motion sensor, 3-axis digital output accelerometer,  
	- MP45DT02, ST MEMS audio sensor, omni-directional digital microphone,  
	- CS43L22, audio DAC with integrated class D speaker driver,  
	- Four user LEDs and two push-buttons,  
	- USB OTG FS with micro-AB connector, and  
	- Easy access to most MCU pins.  
	- Virtual Com port and Mass storage  

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
