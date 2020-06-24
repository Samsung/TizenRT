# Protected Build Guide

## Contents
- [Overview of Flat Build](#overview-of-flat-build)
- [Overview of Protected Build](#overview-of-protected-build)
- [Steps for supporting protected build in a new chipset](#steps-for-supporting-protected-build-in-a-new-chipset)
	- [Prerequisites](#prerequisites)
	- [Linker scripts](#linker-scripts)
	- [Userspace Initialization](#userspace-initialization)
	- [MPU Initialization](#mpu-initialization)
	- [Allocation of kernel and user heaps](#allocation-of-kernel-and-user-heaps)
	- [Makefile changes](#makefile-changes)
	- [Creating a new configuration for Protected Build](#creating-a-new-configuration-for-protected-build)
	- [Binary download](#binary-download)
- [Available APIs](#available-apis)


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
- When the processor runs in privileged mode, it will have access to all the memory regions. When the processor is running in un-privileged mode, it has access only to user-space memory areas which are configured by the kernel during boot up.
- User-space code will be able to access the kernel services only through System Calls (downward communication)
- Information about user-space such as memory regions, and some user-space API's needs to be shared to kernel. This information will be stored in a special data structure called as the User Space Object which will be linked to the kernel binary during build time. The kernel can use the function pointers to user-space APIs available in the user space object to invoke required user space functionality.


## Steps for Supporting Protected Build in a new chipset
The following sections give detailed explanation regarding some of the steps / code changes involved in supporting protected build on a new Chipset.

### Prerequisites
- TizenRT flat build needs to be successfully ported and tested on the target CHIPSET board.
- The developer should have information about the types of memories supported on the board, along with their start addresses and sizes. The partition information of the flash memory should also be known. These details can be found in either the chipset's data sheet, project configuration file, linker or download scripts or in the memory map mentioned as part of the chip's source code.

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

- It is mandatory to define __uflash_segment_start__ linker variable. This will be used in the code to obtain the address of the userspace object.
- The user can also enable the CONFIG_AUTOGEN_MEMORY_LDSCRIPT config, which will auto-generate the memory.ld file based on the Flash partition information in CONFIG_FLASH_PART_XXXX configs.

#### kernel-space.ld
- Create this file by making a copy of the ld.script file which is used for flat build.
- After making a copy, remove any memory region definitions and retain only the 'section' definitions in the kernel-space.ld file.
- Replace all occurrences of "flash" memory region by "kflash" and "ram" or "sram" memory region by "ksram".
- The below snippet shows a sample where text section is assigned to kflash and data section is assigned to ksram region:

```
SECTIONS
{
	.text : {
		_stext = ABSOLUTE(.);
		....
		....
		_etext = ABSOLUTE(.);
	} > kflash
	/* Here in comparison to ld.script for flat build, 'flash' memory region should be replaced with 'kflash' memory region. */
	....
	.data : {
		_sdata = ABSOLUTE(.);
		....
		_edata = ABSOLUTE(.);
	} > ksram
	/* Here in comparison to ld.script for flat build, 'sram' memory region should be replaced with 'ksram' memory region. */
}
```

#### user-space.ld
- This linker script is used while building the user binary. A reference to this script can be found in os/board/common/userspace/Makefile. 
- Replace all occurrences of "flash" memory region by "uflash" and "ram" or "sram" or "ksram" memory region by "usram".
- This script has two major additions as compared to kernel-space.ld.
1. UserSpace Object - This should be placed at the starting of the userspace binary. This is accomplished by using the attribute ```.userspace```. A new section is introduced before the text section in the user-space.ld file for this purpose as shown in the snippet below:

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
- Create a file "CHIPSET_userspace.c" in the folder path <os/arch/arm/src/CHIPSET> and define an API as shown below in the file.
- A header file "CHIPSET_userspace.h" should also be created at path <os/arch/arm/src/CHIPSET> and declare the below given API.
- The functionality of this API should be to clear all of user-space .bss and initialize all of user-space .data.
- The userspace data section is copied from the flash memory to the designated RAM memory region.
- If the flash does not support XIP, then the code section also needs to be copied from the flash to the designated area of RAM.
- The start address and sizes of the code, data and bss sections can be found either in the USERSPACE object.
- Call the CHIPSET_userspace() API from '__start()' API present in the "CHIPSET_start.c" (path : <os/arch/arm/src/CHIPSET> ).

```
	void CHIPSET_userspace(void) {
		  ....
		/* Clear all of user-space .bss */

		dest = (uint8_t *)USERSPACE->us_bssstart;	/*STORING THE START ADDRESS OF .bss section */
		end  = (uint8_t *)USERSPACE->us_bssend;		/*STORING THE END ADDRESS OF .bss section */

		while (dest != end) {
		  *dest++ = 0;					/*Initialization*/
		}

		/* Initialize all of user-space .data */

		src  = (uint8_t *)USERSPACE->us_datasource;	/*STORING THE VALUE WITH WHICH .data section is to be initialized with */
		dest = (uint8_t *)USERSPACE->us_datastart;	/*STORING THE START ADDRESS OF .data section */
		end  = (uint8_t *)USERSPACE->us_dataend;	/*STORING THE END ADDRESS OF .data section */

		while (dest != end) {
		  *dest++ = *src++;				/*Initialization */
		}
		....
	}
```

### MPU Initialization
- This step will configure the MPU to provide default access to complete memory map in the privileged mode.
- It will also configure the user space code and data regions and enable the MPU.
- The MPU region configuration will make use of one of the predefined API's in the arch specific code.
- For example, mpu_userflash, mpu_userintsram APIs may be used to configure the user flash and ram regions.
- These and several other APIs are provided in the mpu.h file in arch/arm/src/armv7m/ folder.
- Depending on the type of memory to be configured, different APIs can be used from this file.
- The MPU region configuration requires the start and size of each region and the region number to which it should be assigned.
- A file 'CHIPSET_mpuinit.c' should be created at folder path <os/arch/arm/src/CHIPSET>.
- A header file 'CHIPSET_mpuinit.h' should also be created at path <os/arch/arm/src/CHIPSET> to declare the API(s) used.
- In the file, first define the user space memory regions as an array of mpu_region_info structures. The following code snippet shows a sample definition:

```
	#if defined(CONFIG_BUILD_PROTECTED)
	const struct mpu_region_info regions_info[] = {
	{
		&mpu_userflash, (uintptr_t)__uflash_segment_start__, (uintptr_t)__uflash_segment_size__, MPU_REG_USER_CODE,
	},
	{
		&mpu_userintsram, (uintptr_t)__usram_segment_start__, (uintptr_t)__usram_segment_size__, MPU_REG_USER_DATA,
	},
	};
	#endif
```
- Secondly, an API to initialize and configure the MPU should be written. A sample is shown in the code snippet below :

```
void CHIPSET_mpuinitialize(void)
{
	....
	for (i = 0; i < (sizeof(regions_info) / sizeof(struct mpu_region_info)); i++) {
		lldbg("Region = %u base = 0x%x size = %u\n", regions_info[i].rgno, regions_info[i].base, regions_info[i].size);
		regions_info[i].call(regions_info[i].rgno, regions_info[i].base, regions_info[i].size);
	}
	....

	mpu_control(true, false, true);		/*ENABLE the MPU : this enables priviledged access to default memory map */
}
```

- Call the CHIPSET_mpuinitialize API from '__start()' present in the 'CHIPSET_start.c'.

### Allocation of kernel and user heaps
- Kernel and User heaps are initialized by calling up_allocate_kheap and up_allocate_heap APIs during the OS start phase.
- Default implementations for these APIs are provided in os/arch/arm/src/common/up_allocateheap.c.
- The kernel heap generally starts at the end of the idle stack and spans the entire remaining space in the kernel ram region.
- The user heap starts at the end of the user bss section and spans the entire remaining space in the user ram region.
- However, if the chipset has a memory configuration that is different from the default one or if the vendor would like to initialize the kernel and user heaps at different locations or sizes, then the vendor can implement new versions of the APIs in a chipset specific file such as CHIPSET_allocateheap.c and build this new file instead of up_allocate.c file.
- The vendor needs to take care that the prototypes of the APIs are not changed in their new implementation.
- For example, the below given API is written to for kernel-space heap allocation:

```
	#if defined(CONFIG_BUILD_PROTECTED) && defined(CONFIG_MM_KERNEL_HEAP)
	void up_allocate_kheap(FAR void **heap_start, size_t *heap_size){
		//The kernel heap will start at the end of the idle stack
		*heap_start = (FAR void *)(g_idle_topstack & ~(0x7));

		//The kernel heap will extend from the end of idle stack to the start of user ram region
		//The start address of the user ram region is obtained from the variables defined in memory.ld linker script
		*heap_size = (uint32_t)((uintptr_t)__usram_segment_start__) - (uint32_t)(*heap_start);
	}
	#endif
```

- The API for user-space heap allocation should be modified as follows:

```
	void up_allocate_heap(FAR void **heap_start, size_t *heap_size){
	#if defined(CONFIG_BUILD_PROTECTED) && defined(CONFIG_MM_KERNEL_HEAP)

		//The user heap will extend from the end of bss section to the end of user ram region
		// The start address and the size of the user ram region is stored in variables defined in memory.ld script
		uintptr_t user_end = (uint32_t)__usram_segment_start__ + (uint32_t)__usram_segment_size__;
		//The user heap will start at the end of the .bss section
		uintptr_t ubase = (uintptr_t)USERSPACE->us_bssend;
		size_t usize = (uint32_t)user_end - ubase;
		....
	#endif
	}
```

### Makefile changes
- The following additions should be made in the Make.defs at path <os/arch/arm/src/CHIPSET/Make.defs>

```
	ifeq ($(CONFIG_BUILD_PROTECTED),y)
	CHIP_CSRCS += CHIPSET_userspace.c
	endif
	ifeq ($(CONFIG_ARMV7M_MPU),y)
	CHIP_CSRCS += CHIPSET_mpuinit.c
	endif
```

### Creating a new configuration for Protected Build
- Every chipset has a hello configuration under path build/config/CHIPSET_NAME.
- Copy the contents of the "hello" folder into a new folder called “hello_protected”
- After modifying below configs using menuconfig, copy os/.config file to configs folder with defconfig named.

#### Menuconfig changes
- Enable CONFIG_BUILD_PROTECTED through ```build setup -> build configuration -> Memory organization -> TinyAra protected buil```
- Enable CONFIG_BUILD_2PASS through ```build setup -> build configuration -> Two pass build``` 
- Set CONFIG_PASS1_TARGET to ```all``` through ```build setup -> build configuration -> Pass one target```
- Set CONFIG_PASS1_OBJECT to ```null``` through ```build setup -> build configuration -> Pass one object```
- Select CONFIG_ARMV7M_MPU through ```Chip Selection -> MPU support```
- Set CONFIG_ARMV7M_MPU_NREGIONS to ```8``` through ```Chip Selection -> Number of MPU regions```
- Select CONFIG_MM_KERNEL_HEAP through ```Memory Management -> Support a protected, kernel heap```
- Select CONFIG_LIB_SYSCALL through ```System Call -> System call support```
- Set CONFIG_SYS_RESERVED to ```8``` through ```System Call -> System call support -> Number of reserved system calls```
- Set CONFIG_SYS_NNEST to ```2``` through ```System Call -> System call support -> Number of nested system calls```

#### Make.defs changes
- The make.defs file at path <build/config/CHIPSET_NAME/hello_protected> provides several definitions which are used during the build process. One of the definitions is (ARCHSCRIPT) for the linker script to be used to link the kernel binary.
- Modify this file to use below two linker scripts for protected build support.

```
MEM_LDSCRIPT = memory.ld
KSPACE_LDSCRIPT = kernel-space.ld
....
ARCHSCRIPT = -T$(TOPDIR)/../build/configs/$(CONFIG_ARCH_BOARD)/scripts/$(MEM_LDSCRIPT)
ARCHSCRIPT += -T$(TOPDIR)/../build/configs/$(CONFIG_ARCH_BOARD)/scripts/$(KSPACE_LDSCRIPT)
```

### Binary download
- Different chipsets have different configs, scripts or different GUI based tools for download purposes.
- In protected build, two binaries (kernel and user) need to be downloaded. So, the download process should be modified to accommodate this.
- The download process should ensure that the user binary is flashed in the "app" partition given by CONFIG_FLASH_PART_NAME defined in 'defconfig' file.

## Available APIs
Because the main purpose of Protected build is kernel protection from user by separation, user application can't use some Kernel APIs which manage kernel internal operation like sched_gettcb API.
The platform APIs user can use with protected build are defined in syscall, libc, framework and external folders.  
Please find the lists to check whether API is available or not on Protected build.

- [List of libc APIs](https://github.com/Samsung/TizenRT/blob/master/lib/libc/libc.csv)
- [List of Math APIs](https://github.com/Samsung/TizenRT/blob/master/lib/libc/math.csv)
- [List of syscall APIs](https://github.com/Samsung/TizenRT/blob/master/os/syscall/syscall.csv)
- [Headers of framework](https://github.com/Samsung/TizenRT/tree/master/framework/include)
- [Headers of external](https://github.com/Samsung/TizenRT/tree/master/external/include)
