# How to add(port) a new board

Changes in *[arch](#arch)* and *[configs](#configs)* folders are mandantory to support a new board.  
Adding static library or expanding driver will be provided later.

## Arch

Changes in *arch* directory for a new board includes chip architecture and BSP for board.  
Currently, TizenRT supports only ARM architecture.  
But if new board is another architecture, new architecture folder should be added in this path.

### BSP

There are three kinds of folders, core, chip architecture and board.  
For example, armv7-r is a core architecture, s5j is a chip architecture and artik053 is a board.  

At least, new board folder should be added.

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
Modify *os/arch/Kconfig.board* file.

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
source arch/arm/src/<BOARD_NAME>/Kconfig
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

