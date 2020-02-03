# Protected Build Guide

## Contents
- [Overview of Flat Build](#overview-of-flat-build)
- [Overview of Protected Build](#overview-of-protected-build)
- [Steps for suporting protected build in a new chipset](#steps-for-supporting-protected-build-in-a-new-chipset)
	- [Make.defs](#makedefs)
	- [Linker scripts](#linker-scripts)
	- [Userspace Initialization](#userspace-initialization)
	- [MPU Initialization](#mpu-initialization)
	- [Allocation of kernel and user heaps](#allocation-of-kernel-and-user-heaps)


## Overview of Flat Build
- In TizenRT flat build, a single binary file will be generated which will contain all the user and kernel code.
- In fact, there is no user - kernel distinction in the flat build.
- The flat build provides a flat address space in which any part of code can access any address in the memory map.
- When TizenRT is being ported to a new chipset, the porting activity is performed for the flat build scenario.
- The document [How to add new board](HowToAddnewBoard.md) provides the instructions for porting TizenRT flat build for a new chipset.
- Once flat build has been successfully verified, the details given in this document can be used to support protected build on the chipset.


## Overview of Protected Build
- In protected build, two separate binaries namely kernel binary and user binary will be created.
- After the build process, the kernel binary will be created under the name - tinyara.bin and user binary as - tinyara_user.bin at path <build/output/bin>
- The kernel binary will consist of the following modules - architecture(ARCH), core Kernel Module(SCHED), C-Lib and memory manager (MM), file system (FS) and the drivers.
- The user binary will include the following modules  - Applications, C-Lib and Memory Manager (MM), Framework and External modules.
- C-lib and Memory Manager are duplicated in the kernel space and the user space.
- As for the memory, the code memory (Flash) region and data memory (RAM) region will be split into two parts each, for use by user and kernel.
- The kernel and user binaries will be placed in separate code memory regions and during runtime they will use separate data memory regions.
- If the architecture supports MPU(Memory Protection Unit), the kernel logic runs in the privileged mode and the user logic runs in the unprivileged mode. MPU prohibits access to all kernel memory regions in un-privileged mode.
- When the processor runs in previleged mode, it will have access to all the memory regions. When the processor is running in un-previleged mode, it has access only to user-space memory areas which are configured by the kernel during boot up.
- User-space code will be able to access the kernel services only through System Calls (downward communication)
- Information about user-space such as memory regions, and some user-space API's needs to be shared to kernel. This information will be stored in a special data structure called as the User Space Object which will be linked to the kernel binary during build time. The kernel can use the function pointers to user-space APIs available in the user space object to invoke required user space functionality.


## Steps for Supporting Protected Build in a new chipset
The following sections give detailed explaination regarding some of the steps / code changes involved in supporting protected build on a new Chipset.

### Make.defs
- The make.defs file provides several definitions which are used during the build process. One of the definitions is (ARCHSCRIPT) for the linker script to be used to link the kernel binary.
- In case of flat build, a single linker script such as ld.script is used. The same is specified in the make.defs file.
- For the case of protected build, we need to provide two linker scripts:
	1. memory.ld : Provides the overall memory layout
	2. kernel-space.ld : Provides the rules to link the kernel binary
- Make.defs file needs to be modified to use these two linker scripts.


### Linker Scripts
- In case of flat build, the single linker script ld.script describes the location and size of memory regions in the target, and also describes the placement and linking of different sections of binary into the different memory regions.
- In case of protected build three linker scripts are needed to perform this functionality:
	- memory.ld : This describes the location and size of memory regions in the target.
	- kernel-space.ld : This is used to assign sections of kernel binary to particular memory regions.
	- user-space.ld : This is used to assign sections of user binary to particular memory regions.
- The kernel-space.ld and user-space.ld make use of the memory regions defined in memory.ld.
- All these three files should be created for protected build and placed in build/configs/CHIPSET/scripts folder.

#### Memory.ld
- This script defines the code and data memory regions to be used by the kernel and user linker scripts.
- Generally a board will contain two types of memory. First is the flash memory which will contain the code and second is the RAM memory to be used for data, stack and heap during execution.
- The developer needs to find out the start address and size of these memories. This information can be found from either of the following sources: datasheets, configuration files, download and linker scripts of flat build, or memory map defined in the codebase of the CHIPSET.
- The code memory should be partitioned into kernel and user code memory. Similarly, the data memory should be partitioned into kernel and user data memory.
- It is recommended that both these memories be partitioned between user and kernel on a 50:50 sharing basis.
- In due course, the size of the various memory regions can be adjusted according to the size requirements of the user and kernel.
- Having made such partitions, the developer should define four memory regions as follows, along with appropriate attributes in the memory.ld file:
	- kflash (for kernel binary)
	- uflash (for user binary)
	- ksram (for kernel binary)
	- usram (for user binary)
- The snippet below shows contents of the memory.ld file for a board having total flash of 2048K and ram of 640K bytes. This can be used as a reference to create the 'memory.ld' script for the intended CHIPSET.

```
/* 2048Kb FLASH */

	kflash (rx)      : ORIGIN = 0x08000000, LENGTH = 1024K
	uflash (rx)      : ORIGIN = 0x080FA000, LENGTH = 1024K

/* 640Kb of contiguous SRAM */

	ksram (rwx)      : ORIGIN = 0x20000000, LENGTH = 320K
	usram (rwx)      : ORIGIN = 0x2004E200, LENGTH = 320K

        __kflash_segment_start__  = ORIGIN(kflash);
        __kflash_segment_size__   = LENGTH(kflash);
        __uflash_segment_start__  = ORIGIN(uflash);
        __uflash_segment_size__   = LENGTH(uflash);
        __ksram_segment_start__   = ORIGIN(ksram);
        __ksram_segment_size__    = LENGTH(ksram);
        __usram_segment_start__   = ORIGIN(usram);
        __usram_segment_size__    = LENGTH(usram);

```

- It is mandatory to define __uflash_segment_start__ linker variable. This will be used in the code to obtain the address of the uerspace object.
- The user can also enable the CONFIG_AUTOGEN_MEMORY_LDSCRIPT config, which will auto-generate the memory.ld file based on the Flash partition information in CONFIG_FLASH_PART_XXXX configs.

#### user-space.ld
- This linker script is used while building the user binary. A reference to this script can be found in os/board/common/userspace/Makefile. 
- This script has two major additions as compared to kernel-space.ld.
1. UserSpace Object - This should be placed at the starting of the userspace binary. This is accomplished by using the attribute ```.userspace```. A new section in introduced before the text section in the user-space.ld file for this purpose as shown in the snippet below:

```
.userspace : {
	*(.userspace)
} > uflash
```
2. A pointer to the user space heap should be placed at the start of the text section. This will be used by the MM(memory manager) module to make allocations. Hence the user heap pointer (.usrheapptr) should be added in the text section of the user memory as follows:

```
.text : {
	_stext = ABSOLUTE(.);
	*(.usrheapptr)		/* User heap pointer added in the .text user section */
	....
	_etext = ABSOLUTE(.);
	} > uflash
```

### Userspace Initialization
- Userspace initialization is performed during the boot up stage to initialize the  user space data and bss memory regions.
- The userpsace data section is copied from the flash memory to the designated RAM memory region.
- The bss section is initialized with zeros.
- If the flash does not support XIP, then the code section also needs to be copied from the flash to the designated area of RAM.
- The start address and sizes of the code, data and bss sections can be found either in the USERSPACE object.

### MPU Initialization
- This step will configure the MPU to provide default access to complete memory map in the privileged mode.
- It will also configure the user space code and data regions and enable the MPU.
- The MPU region configuration will make use of one of the predefined API's in the arch specific code.
- For example, mpu_userflash, mpu_userintsram APIs may be used to configure the user flash and ram regions.
- These and several other APIs are provided in the mpu.h file in arch/arm/src/armv7m/ folder.
- Depending on the type of memory to be configured, different APIs can be used from this file.
- The MPU region configuration requires the start and size of each region and the region number to which it should be assigned.


### Allocation of kernel and user heaps
- Kernel and User heaps are initialized by calling up_allocate_kheap and up_allocate_heap APIs during the OS start phase.
- Default implementations for these APIs are provided in os/arch/arm/src/common/up_allocateheap.c.
- The kernel heap generally starts at the end of the idle stack and spans the entire remaining space in the kernel ram region.
- The user heap starts at the end of the user bss section and spans the entire remaining space in the user ram region.
- However, if the chipset has a memory configuration that is different from the default one or if the vendor would like to initialize the kernel and user heaps at different locations or sizes, then the vendor can implement new versions of the APIs in a chipset specific file such as CHIPSET_allocateheap.c and build this new file instead of up_allocate.c file.
- The vendor needs to take care that the prototypes of the APIs are not changed in their new implementation.
