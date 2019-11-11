# How to Enable and use SmartFS
SmartFS is a file system used for user partitions in TizenRT. This document provides details on SmartFS in TizenRT, notably on its architecture, and how to enable it on a 
new target board.

## Contents
- [About SmartFS](#about-smartfs)  
- [SmartFS Code Layering](#smartfs-code-layering)  
- [How to Enable SmartFS on New Board](#how-to-enable-smartfs-on-new-board)  

## About SmartFS

SmartFS stands for Sector Mapped Allocation for Really Tiny (SMART) flash.  
It is a file system that has been designed to work primary with small and
serial NOR type flash parts that are 1M byte to 16M byte in size
(though this is not a limitation).

The file system operates by segmenting the flash (or flash partition)
into “logical sectors” of equal size and then managing them (allocating,
mapping, chaining, releasing, etc.) to build files and directories.

This guide explains the steps and details for enabling SmartFS on a new board.

## SmartFS Code Layering

SmartFS consists of two layers built on top of a standard MTD Driver.  
Below diagram shows code layers regarding the two SmartFS layers.

```
+----------------------------------------------+
| Smart FS (Smart File System Layer)           |
+----------------------------------------------+
| Smart MTD Driver (Smart Block Driver Layer)  |
+----------------------------------------------+
| MTD Driver  (MTD Block Driver Layer)         |
+----------------------------------------------+
| FLASH Driver (Flash Driver)                  |
+----------------------------------------------+
```

The code directly above the MTD Driver is the SMART MTD layer.  
This interfaces with the MTD (flash driver) layer and handles low-level
media operations such as logical sector allocation, freeing and management,
erase block management, low-level formatting, wear leveling, etc.

On top of the SMART MTD layer is the Smart File System Layer.  
The SmartFS code uses the logical sector services of the SMART MTD layer to
provide file and directory level management, such as creating new files,
chaining logical sectors together to create files, and creating directories/files
with management routines.

## How to Enable SmartFS on New Board
1. add *[FLASH Driver](#flash-driver)* for the flash present in the SoC/Board.  
2. enable *[MTD Driver](#mtd-driver)* corresponding to flash.  
3. enable *[SMART MTD Driver](#smart-mtd-driver)*.  
4. enable *[SMART FS](#smart-fs)*.

### FLASH Driver
Flash driver must provide low-level read/write/erase capability for the FLASH HW.  
Example of flash driver is present at *os/arch/arm/src/s5j/s5j_sflash.c*

During bootup flash driver must be initialized similar to the following example.
```c
/* In case of s5j */
void arm_boot(void)
{
	...
	s5j_board_initialize();
	...
}

void s5j_board_initialize(void)
{
#ifdef CONFIG_S5J_SFLASH
        s5j_sflash_init();
#endif
}
```
```c
/* In case of imxrt */
void __start(void)
{
	...
	/* Initialize onboard resources */
	imxrt_boardinitialize();
	...
}

void imxrt_boardinitialize(void)
{
	...
	imxrt_flash_init();
}
```
### MTD Driver
MTD Driver must be enabled to access the underlying FLASH driver.  
The MTD driver initialization, however, varies by target board.

Below configurations must be enabled to use MTD Driver.
1. Enable CONFIG_MTD  
	```
	File Systems -> Memory Technology Device (MTD) Support = y
	```
2. Enable CONFIG_MTD_PARTITION  
	```
	File Systems -> Memory Technology Device (MTD) Support -> Support MTD partitions = y
	```
3. Enable CONFIG_MTD_PARTITION_NAMES  
	```
	File Systems -> Memory Technology Device (MTD) Support -> Support MTD partition  naming = y
	```

Refer to *os/board/common/partitions.c* for example of MTD driver initialization.
```c
#ifdef CONFIG_MTD_PROGMEM
	mtd = progmem_initialize();
	if (!mtd) {
		lldbg("ERROR: progmem_initialize failed\n");
		return;
	}

	if (mtd->ioctl(mtd, MTDIOC_GEOMETRY, (unsigned long)&geo) < 0) {
		lldbg("ERROR: mtd->ioctl failed\n");
		return;
	}
#else
	mtd = up_flashinitialize();
	if (!mtd) {
		lldbg("ERROR : up_flashinitializ failed\n");
		return;
	}

	if (mtd->ioctl(mtd, MTDIOC_GEOMETRY, (unsigned long)&geo) < 0) {
		lldbg("ERROR: mtd->ioctl failed\n");
		return;
	}
#endif
```

For the board such as s5j that supports progmem operations (refer *os/fs/driver/mtd/mtd_progmem.c*),  
CONFIG_MTD_PROGMEM should be enabled first.  
```
File Systems -> Memory Technology Device (MTD) Support -> Enable on-chip program FLASH MTD device = y
```

For the board such as imxrt, CONFIG_MTD_PROGMEM is disabled and up_flashinitialize needs to be implemented.
(refer *os/arch/arm/src/imxrt/imxrt_norflash.c*)


### SMART MTD Driver
Smart MTD layer can be enabled via the CONFIG_MTD_SMART config parameter.
```
File Systems -> Memory Technology Device (MTD) Support -> Sector Mapped Allocation for Really Tiny (SMART) Flash support = y
```

Refer *os/fs/driver/mtd/smart.c* for smart MTD driver code.

And a smart device can be initialized as shown below. (Refer *os/board/common/partitions.c*)
```c
#ifdef CONFIG_MTD_CONFIG
		if (!strncmp(types, "config,", 7)) {
			mtdconfig_register(mtd_part);
		} else
#endif
#if defined(CONFIG_MTD_SMART) && defined(CONFIG_FS_SMARTFS)
		if (!strncmp(types, "smartfs,", 8)) {
			char partref[4];

			snprintf(partref, sizeof(partref), "p%d", partno);
			smart_initialize(CONFIG_FLASH_MINOR, mtd_part, partref);
		} else
#endif
		{
		}

```
The above code snippet creates a smart device node similar to */dev/smart0p[partno]*.  


### SMART FS
Smart FS layer can be enabled by enabling below configurations.  
1. Enable CONFIG_FS_SMARTFS
	```
	File Systems -> SMART file system = y
	```
2. Set CONFIG_SMARTFS_ERASEDSTATE
	```
	File Systems -> SMART file system -> SMARTFS options -> FLASH erased state = 0xff
	```
3. Set CONFIG_SMARTFS_MAXNAMLEN=32
	```
	File Systems -> SMART file system -> SMARTFS options -> Maximum file name length = 32
	```
4. Enable CONFIG_SMARTFS_ALIGNED_ACCESS
	```
	File Systems -> SMART file system -> SMARTFS options -> Ensure 16 and 32 bit accesses are aligned = y
	```

Below are the optional features configurations. Refer *os/fs/smartfs/Kconfig* for more details.  
1. CONFIG_SMARTFS_MULTI_ROOT_DIRS
```
File Systems -> SMART file system -> SMARTFS options -> Support multiple Root Directories/Mount points
```
2. CONFIG_SMARTFS_BAD_SECTOR
```
File Systems -> SMART file system -> SMARTFS options -> Bad Sector Management
```
3. CONFIG_SMARTFS_DYNAMIC_HEADER
```
File Systems -> SMART file system -> SMARTFS options -> Dynamic Header
```
4. CONFIG_SMARTFS_JOURNALING
```
File Systems -> SMART file system -> SMARTFS options -> Enable filesystem journaling for smartfs
```
5. CONFIG_SMARTFS_VERIFY_JOURNALING
```
File Systems -> SMART file system -> SMARTFS options -> Enable filesystem journaling for smartfs -> Verify Journal logging
```
6. CONFIG_SMARTFS_SECTOR_RECOVERY
```
File Systems -> SMART file system -> SMARTFS options -> Enable recovery of lost sectors in Filesystem
```
SmartFS filesystem can be mounted on smart mtd device.  
Before mounting, smart device needs to formatted as smartfs using a mksmartfs function.  
Sample code for formatting and mounting SmartFS:
```c
#ifdef CONFIG_ARTIK05X_AUTOMOUNT_USERFS
	/* Initialize and mount user partition (if we have) */
	ret = mksmartfs(ARTIK05X_AUTOMOUNT_USERFS_DEVNAME, false);
	if (ret != OK) {
		lldbg("ERROR: mksmartfs on %s failed\n",
				ARTIK05X_AUTOMOUNT_USERFS_DEVNAME);
	} else {
		ret = mount(ARTIK05X_AUTOMOUNT_USERFS_DEVNAME,
				CONFIG_ARTIK05X_AUTOMOUNT_USERFS_MOUNTPOINT,
				"smartfs", 0, NULL);
		if (ret != OK) {
			lldbg("ERROR: mounting '%s' failed\n",
					ARTIK05X_AUTOMOUNT_USERFS_DEVNAME);
		}
	}
#endif /* CONFIG_ARTIK05X_AUTOMOUNT_USERFS */
```
Smart FS can also be mounted in TASH Shell as shown below:  
1. Format the partition using mksmartfs command
	```
	Usage   : mksmartfs <device name>
	Example : mksmartfs /dev/smart0p8
	```
2. Mount smartfs using mount command
	```
	Usage   : mount -t <fs-type> <source/device name> <target/ logical mount path>
	Example : mount -t smartfs /dev/smart0p8 /mnt
	```
