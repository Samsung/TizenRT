# How to add(port) a new board

There are three kinds of folders, core, chip architecture and board.  
For example, armv7-r is a core architecture, s5j is a chip architecture and artik05x is a board.  

Changes in *[arch](#arch)*, *[board](#board)* and *[configs](#configs)* folders are mandatory to support a new board.  

core and chip architecture must be added in *[arch](#arch)*.  
board must be added in *[board](#board)* and *[configs](#configs)*.  

Adding static library or expanding driver will be provided later.

## Arch

Changes in *os/arch* directory for a new board includes core and chip architecture.  
Currently, TizenRT supports only ARM architectures.  
But if new board is of another architecture, new core architecture folder must be added.  

chip architecture sources must be added in *os/arch/arm/src/chip-name*.  
chip architecture headers must be added in *os/arch/arm/include/chip-name*.  

Example: *os/arch/arm/src/s5j*, *os/arch/arm/include/s5j*  

Refer *[kconfig](#kconfig)* for adding Kconfig entries

## Board

Board sources must be added in *os/board/board-name/src/*  
Board headers must be added in *os/board/board-name/include/*  
Example: *os/board/artik05x/src*,  *os/board/artik05x/include*  

Refer *[kconfig](#kconfig)* for adding Kconfig entries

### Kconfig

Modifications on below two Kconfigs show new board configurations on menuconfig.

#### Chip
Modify *os/arch/arm/Kconfig* file.

1. Add new configurations as belows.
```
choice
	prompt "ARM chip selection"

config ARCH_CHIP_<CHIP_NAME>
	bool "XXX"
	select ARCH_HAVE_YYY
endchoice

config ARCH_CHIP
	string
	default "<CHIP_NAME>"       if ARCH_CHIP_<CHIP_NAME>
```

2. Include Kconfig as belows.
```
if ARCH_CHIP_<CHIP_NAME>
source arch/arm/src/<CHIP_NAME>/Kconfig
endif
```

#### Board
Modify *os/board/Kconfig* file.

1. Add new configurations as belows.
```
choice
	prompt "Select target board"

config ARCH_BOARD_<BOARD_NAME>
	bool "XXX Starter Kit"
	depends on ARCH_CHIP_<CHIP_NAME>
	select YYY
endchoice

config ARCH_BOARD
	string
	default "<BOARD_NAME>"           if ARCH_BOARD_<BOARD_NAME>
```

2. Include Kconfig as belows.
```
comment "Board-Specific Options"

if ARCH_BOARD_<BOARD_NAME>
source board/<BOARD_NAME>/Kconfig
endif
```

## Configs

Changes in *configs* folder includes build options and flash operations.

1. Add a new folder with board name.
```
mkdir build/configs/<board-name>
```

2. Add the *script* folder and add the linker script file.
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

