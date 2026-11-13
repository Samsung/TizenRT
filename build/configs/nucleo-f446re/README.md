# Nucleo-F446RE

The STM32F446RE is a 180MHz Cortex-M4 microcontroller with 512 Kbytes of Flash memory and 128 Kbytes SRAM.
(Note: TizenRT currently clocks this board at 168MHz to match standard PLL multiplier requirements for stable UART baud rates off the 16MHz internal HSI oscillator).

## Contents

> [Information](#information)  
> [Environment Set-up](#environment-set-up)  
> [Install STLINK Tool](#install-stlink-tool)  
> [How to program a binary](#how-to-program-a-binary)  
> [Configuration Sets](#configuration-sets)  

## Information
The board features:  
	- On-board ST-LINK/V2-1 debugger/programmer with SWD connector,  
	- Arduino Uno V3 connectivity support,  
	- ST morpho extension pin headers for full access to all STM32 I/Os,  
	- One user LED (LD2) on PA5,  
	- One user push-button (B1) on PC13,  
	- Virtual Com port and Mass storage support.  

## Environment Set-up
This section covers board-specific environment set-up.  
Please set TizenRT common environment, [quick start](https://github.com/Samsung/TizenRT#quick-start), first before doing below.

## Install STLINK / OpenOCD Tool
This section covers installing the ST-LINK/V2 or OpenOCD tool on Linux.  
OpenOCD is required for programming and debugging the board via `stlink-v2-1`.  
Install via apt: `sudo apt install openocd`

## How to program a binary

After building TizenRT, execute the OpenOCD command from the root directory:

```bash
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program build/output/bin/tinyara.bin 0x08000000 verify reset exit"
```

## Configuration Sets

#### [hello](hello/README.md)
This provides a simple hello world application including the kernel, a shell (TASH), and the LED test application.
