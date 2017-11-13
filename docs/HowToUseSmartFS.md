# How to use SmartFS

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

Smart FS consists of two layers built on top of standard MTD Driver.  
Below diagram shows the dependant layers part of Smart FS.

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

The code directly above the MTD driver is the SMART MTD layer.  
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
2. enable *[MTD driver](#mtd-driver)* corresponding to flash.  
3. enable *[SMART MTD Driver](#smart-mtd-driver)*.  
4. enable *[SMART FS](#smart-fs)*

## FLASH Driver
Flash driver must provide low-level read/write/erase capability for the FLASH HW.  
Example driver is at *os/arch/arm/src/s5j/s5j_sflash.c*

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
MTD driver must be enabled to access the underlying FLASH driver.  
Example driver is at *os/fs/driver/mtd/mtd_progmem.c*.  

Below configuration must be enabled for using mtd driver.
1. Enable CONFIG_MTD  
```
File  Systems->Memory Technology Device (MTD) Support to y
```
2. Enable CONFIG_MTD_PARTITION  
```
File  Systems->Memory Technology Device (MTD) Support->Support MTD partitions to y
```
3. Enable CONFIG_MTD_PARTITION_NAMES  
```
File  Systems->Memory Technology Device (MTD) Support->Support MTD partition  naming to y
```
4. Enable CONFIG_MTD_PROGMEM  
```
File  Systems->Memory Technology Device (MTD) Support->Enable on-chip program  FLASH MTD device to y
```

Example of initialization is as shown below,refer *os/arch/arm/src/sidk_s5jt200/src/s5jt200_tash.c*
```
artik053_configure_partitions() -> progmem_initialize ();
```

## SMART MTD Driver
Smart MTD layer can be enabled by enabling CONFIG_MTD_SMART config
```
File  Systems->Memory Technology Device (MTD) Support-> Sector Mapped  Allocation for Really Tiny (SMART) Flash support) to y
```

Refer *os/fs/driver/mtd/smart.c* for smart mtd layer code.

smart device can be initialized using as shown below,
```
#if defined(CONFIG_MTD_SMART) && defined(CONFIG_FS_SMARTFS)
                if (!strncmp(types, "smartfs,", 8)) {
                        char partref[4];
                        snprintf(partref, sizeof(partref), "p%d", partno);
                        smart_initialize(CONFIG_SIDK_S5JT200_FLASH_MINOR,
                                        mtd_part, partref);
                }
#endif
```
The above snippet creates smart device node similar to */dev/smartp0x*.  
For smart device initialization, refer *os/arch/arm/src/sidk_s5jt200/src/s5jt200_tash.c*

## SMART FS
Smart FS layer can be enabled by enabling below configurations.  
1. Enable CONFIG_FS_SMARTFS
```
File  Systems->SMART file system
```
2. Set CONFIG_SMARTFS_ERASEDSTATE
```
File  Systems->SMART file system ->SMARTFS options->FLASH erased state  (0xff) to 0xFF
```
3. set CONFIG_SMARTFS_MAXNAMLEN=32
```
File  Systems->SMART file system ->SMARTFS options->Maximum file length  (32) to 32
```
4. Enable CONFIG_SMARTFS_ALIGNED_ACCESS
```
File  Systems->SMART file system ->SMARTFS options->Ensure 16 and 32 bit  accesses are aligned to y
```

Below are the optional features configurations. Refer *os/fs/smartfs/Kconfig* for more details.  
1. CONFIG_SMARTFS_MULTI_ROOT_DIRS
```
File  Systems->SMART file system ->SMARTFS options->Support multiple Root  Directories/Mount points
```
2.CONFIG_SMARTFS_BAD_SECTOR
```
File  Systems->SMART file system ->SMARTFS options->Bad Sector Management
```
3.CONFIG_SMARTFS_DYNAMIC_HEADER
```
File  Systems->SMART file system ->SMARTFS options->Dynamic Header
```
4. CONFIG_SMARTFS_JOURNALING
```
File  Systems->SMART file system ->SMARTFS options->Enable journaling for  smartfs
```
5. CONFIG_SMARTFS_VERIFY_JOURNALING
```
File  Systems->SMART file system ->SMARTFS options->Enable journaling for  smartfs->verify Journal logging
```
6. CONFIG_SMARTFS_SECTOR_RECOVERY
```
File  Systems->SMART file system ->SMARTFS options-> Enable recovery of  lost sectors in File system
```
Smart FS can be mounted on smart mtd device.  
Before mounting, smart device formatted to smartfs using mksmartfs tool.  
Example snippet for formatting and mounting smart fs:
```
#ifdef CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS
        /* Initialize and mount user partition (if we have) */
        ret = mksmartfs(CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME, false);
        if (ret != OK) {
                lldbg("ERROR: mksmartfs on %s failed",
                                CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME);
        } else {
                ret = mount(CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME,
                                CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS_MOUNTPOINT, 
                                "smartfs", 0, NULL);
                if (ret != OK)
                        lldbg("ERROR: mounting '%s' failed\n",
                                CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS_DEVNAME);
        }
#endif /* CONFIG_SIDK_S5JT200_AUTOMOUNT_USERFS */
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
Example : mount -t smartfs /dev/smartp0p8 /mnt
```
