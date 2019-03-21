# How to partition a flash

This document describes the method to partition flash memory using TizenRT's menuconfig. 

## How to configure CONFIG

Flash partition is provided by configurations, FLASH_PART_SIZE, FLASH_PART_TYPE and FLASH_PART_NAME. 

Flash partition can be configured the following three parameters:  

 1. FLASH_PART_SIZE -- represents a partition size  
 2. FLASH_PART_TYPE -- represents interface between flash and upper layer  
 3. FLASH_PART_NAME -- represents partition name  

All partition information is separated by comma and the last must end with a comma.  
Note that the values for FLASH_PART_SIZE, FLASH_PART_TYPE and FLASH_PART_NAME must be specified in that order, as shown in the example below:  

FLASH_PART_SIZE "16,48,192,32,512,2400,1536,1536,1400,8,512,"  
FLASH_PART_TYPE "none,none,none,none,none,none,none,none,smartfs,config,none,"  
FLASH_PART_NAME "bl1,sssro,bl2,sssfw,wlanfw,os,factory,ota,user,nvram,sssrw,"  

name    | type    | size
------- | ------- | --------
bl1     | none    | 16
sssro   | none    | 48
bl2     | none    | 192
sssfw   | none    | 512
wlanfw  | none    | 2400
os      | none    | 1536
factory | none    | 1536
user    | smartfs | 1400
nvram   | config  | 8
sssrw   | none    | 512

In the following, we describe how to set the parameters using menuconfig.  

### FLASH_PART_SIZE

The unit of partition size is KB.   
You can find and modify it using menuconfig as shown below:  

```
	Board Selection -> Flash partition size
```

### FLASH_PART_TYPE

There are none, ftl, config, smartfs and romfs.  
You can find and modify it using menuconfig as shown below:  

```
	Board Selection -> Flash partition type list
```

### FLASH_PART_NAME

This is set to mtd name. so you can name it anything you want.  
You can find and modify it using menuconfig as shown below:  

```
	Board Selection -> Flash partition name
```

## APPENDIX
### How to set romfs partition

Here is an example to show how to set romfs partition on ARTIK05x.  
  
1. Modify partition configs and enable the automount config through *menuconfig*  
    Below steps creates ROMFS partition with size 400KB at next of user partition. 

    1. Split user partition size from (1400) to (1000, 400) in FLASH_PART_SIZE

        ```bash
        Hardware Configuration -> Board Selection -> change values at Flash partition size list (in KBytes)
        ```
    
    2. Append "romfs" at next of *smartfs* to FLASH_PART_TYPE

        ```bash
        Hardware Configuration -> Board Selection -> append string at Flash partition type list
        ```

    3. Append "rom" at next of *user* to FLASH_PART_NAME

        ```bash
        Hardware Configuration -> Board Selection -> append string at Flash partition name list
        ```

    4. Enable the automount config for romfs

        ```bash
        Hardware Configuration -> Board Selection -> Automount partitions -> [*] Automount romfs partition
        ```
