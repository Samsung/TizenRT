# How to Enable and use SmartFS

## Contents
> [About SmartFS](#about-smartfs)  
> [SmartFS Code Layering](#smartfs-code-layering)  
> [How to Enable SmartFS on New Board](#how-to-enable-smartfs-on-new-board)  

## About SmartFS
SmartFS is a FileSystem used for user partition in TizenRT.  

SmartFS stands for Sector Mapped Allocation for Really Tiny (SMART) flash.  
It is a filesystem that has been designed to work primary with small,
serial NOR type flash parts that are 1M byte to 16M byte in size
(though this is not a limitation).

The filesystem operates by segmenting the flash (or flash partition)
into “logical sectors” of equal size and then managing them (allocating,
mapping, chaining, releasing, etc.) to build files and directories.

This guide explains the steps and details for enabling SmartFS on a new board.

## SmartFS Code Layering

SmartFS consists of two layers built on top of standard MTD Driver.  
Below diagram shows the dependent layers part of Smart FS.

```
+----------------------------------------------+
| SMART FS (Smart FileSystem Layer)            |
+----------------------------------------------+
| SMART MTD Driver (Smart Block Driver Layer)  |
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

On top of the SMART MTD layer is the Smart Filesystem Layer.  
The SmartFS code uses the logical sector services of the SMART MTD layer to
provide file and directory level management, such as creating new files,
chaining logical sectors together to create files, creating directories and
 file / directory search and management routines.

## How to Enable SmartFS on New Board
1. add *[FLASH Driver](#flash-driver)* for the flash present in the SoC/Board.  
2. enable *[MTD Driver](#mtd-driver)* corresponding to flash.  
3. enable *[SMART MTD Driver](#smart-mtd-driver)*.  
4. enable *[SMART FS](#smart-fs)*

## FLASH Driver
Flash driver must provide low-level read/write/erase capability for the FLASH HW.  
Example of flash driver is present at *os/arch/arm/src/s5j/s5j_sflash.c*

During bootup flash driver must be initialized similar to below example snippet.
```
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
## MTD Driver
MTD Driver must be enabled to access the underlying FLASH driver.  
Example of MTD driver is at *os/fs/driver/mtd/mtd_progmem.c*.  

Below configurations must be enabled for using MTD Driver.
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
4. Enable CONFIG_MTD_PROGMEM  
```
File Systems -> Memory Technology Device (MTD) Support -> Enable on-chip program FLASH MTD device = y
```

Refer to *os/board/artik05x/src/artik05x_tash.c* for example of MTD driver initialization
```
artik05x_configure_partitions() -> progmem_initialize ();
```

## SMART MTD Driver
Smart MTD layer can be enabled by enabling CONFIG_MTD_SMART config
```
File Systems -> Memory Technology Device (MTD) Support -> Sector Mapped Allocation for Really Tiny (SMART) Flash support = y
```

Refer *os/fs/driver/mtd/smart.c* for smart mtd layer code.

smart device can be initialized as shown below,
```
#if defined(CONFIG_MTD_SMART) && defined(CONFIG_FS_SMARTFS)
                if (!strncmp(types, "smartfs,", 8)) {
                        char partref[4];

                        snprintf(partref, sizeof(partref), "p%d", partno);
                        smart_initialize(CONFIG_ARTIK05X_FLASH_MINOR, mtd_part, partref);
                }
#endif

```
The above snippet creates smart device node similar to */dev/smart0p8*.  
For smart device initialization, refer *os/board/artik05x/src/artik05x_tash.c*

## SMART FS
Smart FS layer can be enabled by enabling below configurations.  
1. Enable CONFIG_FS_SMARTFS
```
File Systems -> SMART file system = y
```
2. Set CONFIG_SMARTFS_ERASEDSTATE
```
File Systems -> SMART file system -> SMARTFS options -> FLASH erased state = 0xff
```
3. set CONFIG_SMARTFS_MAXNAMLEN=32
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
2.CONFIG_SMARTFS_BAD_SECTOR
```
File Systems -> SMART file system -> SMARTFS options -> Bad Sector Management
```
3.CONFIG_SMARTFS_DYNAMIC_HEADER
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
Before mounting, smart device is formatted to smartfs using mksmartfs tool.  
Example snippet for formatting and mounting SmartFS:
```
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
