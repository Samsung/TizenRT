# How to add(port) a new board

To support a new board, you should make changes on three folders, *arch*, *board* and *configs*.  

> 1. **arch** folder includes *CPU* and *chip* architectures.
> 2. **board** folder includes board support package(BSP).
> 3. **configs** folder includes build options and flash operations.

## Contents

- [Folder structure](#folder-structure)
- [Architecture](#architecture)
- [Board](#board)
- [How to include headers](#how-to-include-arch-and-board-header-files)
- [Configs](#configs)

## Folder structure

```
	.
	|- os .- arch - <cpu-familyname> .- include .- <cpu-archname>
	|     |                    |          |
	|     |                    |          `- <chip-name>
	|     |                    |
	|     |                    `- src .- <cpu-archname>
	|     |                           |
	|     |                           `- <chip-name>
	|     `- board - <board-name> .- include
	|                             |
	|                             `- src
	|
	`- build - configs - <board-name>
```

## Architecture

This includes *CPU* and *chip* architecture inside.  

### CPU

```
	.
	`- os - arch - <cpu-familyname> .- include - <cpu-archname>
	                          |
	                          `- src - <cpu-archname>
```
**CPU** has *\<cpu-familyname\>* along with *\<cpu-archname\>* in the path.  
- cpu-family : arm, mips (not supported yet), ...
- cpu-architecture : armv7-r, armv7-m, common, ...

After adding source codes under *os/arch/\<cpu-familyname\>*,  
to provide selection of new architecture, **Kconfig** on *os/arch* also need to be change like below:
```
choice
	prompt "CPU Architecture"
	default ARCH_ARM

config ARCH_ARM
	bool "ARM"
	---help---
		The ARM architectures

config ARCH_<CPU-FAMILYNAME>
	bool "<CPU-FAMILYNAME>"
	---help---
		The <CPU-FAMILYNAME> architecture

endchoice

config ARCH
	string
	default "arm"        if ARCH_ARM
	default "<cpu-familyname>  if ARCH_<CPU-FAMILYNAME>

source arch/arm/Kconfig
source arch/<cpu-familyname>/Kconfig
```

>**Note**
>Value of `config ARCH` should be same as folder name of `<cpu-familyname>`.
>Currently, TizenRT supports only ARM architecture with *armv7-m*, *armv7-r* and *common* folders.

### Chip

```
	.
	`- os - arch - <cpu-familyname> .- include .- <cpu-archname>
	                          |          |
	                          |          `- <chip-name>
	                          |
	                          `- src .- <cpu-archname>
	                                 |
	                                 `- <chip-name>
```

Currently, TizenRT includes *bcm4390x*, *s5j* and *tiva* folders for chip architecture.  
You must add new chip folders under *src* and *include* folders.  
The `<chip-name>` folders under *src* should include **Make.defs** and **Kconfig** to provide configuration and compilation.

Kconfig of *os/arch/\<cpu-familyname\>* supports selection of chip among TizenRT supported like CPU.
```
choice
	prompt "<CPU-FAMILYNAME> chip selection"
	default ARCH_CHIP_S5J

config ARCH_CHIP_S5J
	bool "Samsung S5J"
	---help---
		Samsung IoT SoC architectures (ARM Cortex R)

config ARCH_CHIP_<CHIP-NAME>
	bool "xxxxx"

endchoice

config ARCH_CHIP
	string
	default "s5j"              if ARCH_CHIP_S5J
	default "<chip-name>"      if ARCH_CHIP_<CHIP-NAME>

if ARCH_CHIP_S5J
source arch/<cpu-familyname>/src/s5j/Kconfig
endif
if ARCH_CHIP_<CHIP-NAME>
source arch/<cpu-familyname>/src/<CHIP-NAME>/Kconfig
```

Like \<cpu-familyname\>, `config ARCH_CHIP` value also should be same as folder name.

## Board

```
	.
	|- os .- arch
	      |
	      `- board - <board-name> .- include
	                              |
	                              `- src
```

After making `<board-name>` folder, put headers into *include* folder and put source codes into *src* folder.  
Same as CPU and chip selection, new board will be selected by **Kconfig** of *os/board*.

```
choice
	prompt "Select target board"
	default ARCH_BOARD_SIDK_S5JT200

config ARCH_BOARD_LM3S6965EK
	bool "Stellaris LM3S6965 Evaluation Kit"

config ARCH_BOARD_<BOARD-NAME>
	bool "XXX"

endchoice

config ARCH_BOARD
	string
	default "lm3s6965-ek"        if ARCH_BOARD_LM3S6965EK
	default "<board-name>"       if ARCH_BOARD_<BOARD-NAME>

if ARCH_BOARD_LM3S6965EK
source board/lm3s6965-ek/Kconfig
endif
if ARCH_BOARD_<BOARD-NAME>
source board/<board-name>/Kconfig
endif
```

## How to include arch and board header files

At build time, Makefile dynamically links selected chip and board folders into selected cpu folder.  
After building, you can find *chip* and *board* folders at *os/arch/\<cpu-familyname\>/src* folder.

Additionally, at that time, *include* folder is linked into *os/include/arch* folder to expose their public APIs and definitions.

For example, to use *bcm4390x* chip API which is defined in *os/arch/arm/include/bcm4390x/chip.h*, `#include <arch/chip/chip.h>` is necessary.  
For *os/board/artik05x/artik055_alc5658_i2c.h*, `#include <arch/board/artik055_alc5658_i2c.h>` can be used.

>**Note**
>If you want to unlink the chip and board folders from the linked cpu folder, you can use the *make distclean* command.

## Configs

Changes in *configs* folder includes build options and flash operations.

1. Add a new folder with board name.
	```
	mkdir build/configs/<board-name>
	```

2. Add the *scripts* folder and add the linker script file.
	```
	mkdir build/configs/<board-name>/scripts
	```

3. Add a new folder with representing main program name under \<board-name\> folder.
	```
	mkdir build/configs/<board-name>/<program-name>
	```

4. Put *defconfig* and *Make.defs* inside \<program-name\>.

### defconfig

This is a configurations set of representing folder name.  
This is moved to .config at executing configure.sh and used to build TizenRT.

### Make.defs

This is a build options set like setting of including path, ARM build options, linker script name and so on.  
There is the DOWNLOAD definition to support "make download xx" command too.

### Linker Variables

Linker script has memory configuration information.  
file path : *build/configs/<BOARD_NAME>/scripts/<SCRIPTS_NAME>.ld* 

In addition to the SECTION information, add the following variables as well to mark the following:  
1. `_sbss`: Start of .bss section
2. `_ebss`: End+1 of .bss section
3. `_sidle_stack`: Start of idle stack
4. `__sint_heap_start`: Start of heap in internal RAM region
5. `__sext_heap_start`: Start of heap in external RAM region

For example,  

1. The remaining RAM region after bss region can be set as idle thread stack and internal heap regions(`build/configs/sidk_s5jt200/scripts/ld_s5jt200_flash.script`):
```
	.bss : {
		_sbss = ABSOLUTE(.);
		*(.bss .bss.*)
		*(.gnu.linkonce.b.*)
		*(COMMON)
		. = ALIGN(4);
		_ebss = ABSOLUTE(.);
		_sidle_stack = ABSOLUTE(.);
		. = . + CONFIG_IDLETHREAD_STACKSIZE ;
		/* Heap start address in internal RAM */
		_sint_heap_start = ABSOLUTE(.);
	} > sram
```
2. Idle thread stack and internal heap regions should be set at a different region (other than end of bss) as per the memory layout (`build/configs/rtl8720e/scripts/rlx8720e_img2.ld`):
```
	.ram_image2.bss (NOLOAD):
	{
		__bss_start__ = .;
		*(.bss*)
		*(COMMON)
		__bss_end__ = .;
	} > KM4_BD_RAM

	.psram_heap.start (NOLOAD):
	{
		/* Heap start address in external RAM */
		_sext_heap_start = ABSOLUTE(.);
	} > KM4_BD_PSRAM

	/* Heap start address in internal RAM */
	_sint_heap_start = ABSOLUTE(ORIGIN(KM4_HEAP_EXT));
	_sidle_stack = ABSOLUTE(ORIGIN(KM4_MSP_RAM_NS) + LENGTH(KM4_MSP_RAM_NS)) - CONFIG_IDLETHREAD_STACKSIZE;
```

In addition to the above information, export the CONFIG_IDLETHREAD_STACKSIZE to the linker script using below statement in the board `Make.defs` for all configurations (`build/configs/sidk_s5jt200/hello/Make.defs`):  
```
LDFLAGS += --defsym=CONFIG_IDLETHREAD_STACKSIZE=$(CONFIG_IDLETHREAD_STACKSIZE)
```
