# Steps for Supporting Protected Build in a new chipset

## Introduction
This guide explains the changes to be made to support Protected Build for a new Chipset. The user can refer to [Protected Build Guide](ProtectedBuildGuide.md) for detailed descriptions of the steps involved.

### Prerequisites
- TizenRT flat build needs to be successfully ported and tested on the target CHIPSET board.
- The developer should have information about the types of memories supported on the board, along with their start addresses and sizes. The partition information of the flash memory should also be known. These details can be found in either the chipset's data sheet, project configuration file, linker or download scripts or in the memory map mentioned as part of the chip's source code.

## Creating a new configuration for Protected Build
- Every chipset has a hello configuration under path build/config/CHIPSET_NAME.
- Copy the contents of the "hello" folder into a new folder called “hello_protected”
- Modify the defconfig and Make.defs as follows:

### Defconfig
- The following configs need to be set or unset in the defconfig file.

```
CONFIG_BUILD_FLAT is not set
CONFIG_BUILD_PROTECTED=y
CONFIG_BUILD_2PASS=y
CONFIG_PASS1_TARGET="all"
CONFIG_PASS1_OBJECT=""
CONFIG_ARM_HAVE_MPU_UNIFIED=y
CONFIG_ARMV7M_MPU=y
CONFIG_ARMV7M_MPU_NREGIONS=8
CONFIG_MM_KERNEL_HEAP=y
CONFIG_LIB_SYSCALL=y
CONFIG_SYS_RESERVED=8
CONFIG_SYS_NNEST=2
```

### Make.defs
- Modify the make.defs to include below lines:

```
MEM_LDSCRIPT = memory.ld
KSPACE_LDSCRIPT = kernel-space.ld
....
ARCHSCRIPT = -T$(TOPDIR)/../build/configs/$(CONFIG_ARCH_BOARD)/scripts/$(MEM_LDSCRIPT)
ARCHSCRIPT += -T$(TOPDIR)/../build/configs/$(CONFIG_ARCH_BOARD)/scripts/$(KSPACE_LDSCRIPT)
```


## Linker Scripts
- Create memory.ld, kernel-space.ld and user-space.ld in build/configs/CHIPSET/scripts folder.

### Memory.ld
- This file should provide the definition for 4 memory regions named as kflash, uflash, ksram and usram.
- Also, define linker variables for the start address and size of each region.
- Refer to [Protected Build Guide](ProtectedBuildGuide.md) for details about the memory regions.
- The snippet below shows contents of the memory.ld file for a board having total flash of 2048K and ram of 640K bytes.

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

### kernel-space.ld
- Create this file by making a copy of the ld.script file which is used for flat build.
- After making a copy, remove any memory region definitions and retain only the 'section' definitions in the kernel-space.ld file.
- Replace all occurrences of "flash" memory region by "kflash" and "ram" or "sram" memory region by "ksram"
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

### user-space.ld
- Create this file by making a copy of the already created kernel-space.ld script.
- Add the following lines at the start of the file.

```
EXTERN(umm_initialize)
EXTERN(umm_addregion)
EXTERN(umm_trysemaphore)
EXTERN(umm_givesemaphore)

EXTERN(malloc)
EXTERN(realloc)
EXTERN(zalloc)
EXTERN(free)

.userspace : {
	*(.userspace)
} > uflash
```
- A pointer to the user space heap (.usrheapptr) should be placed at the start of the text section. Modify the text section as follows:
```
.text : {
	_stext = ABSOLUTE(.);
	*(.usrheapptr)		/* User heap pointer added in the .text user section */
	....
	_etext = ABSOLUTE(.);
	} > uflash
```
- Replace all occurrences of ```kflash``` by ```uflash``` and ```ksram``` by ```usram```.


## Userspace Initialization
- Create a file "CHIPSET_userspace.c" in the folder path <os/arch/arm/src/CHIPSET> and define an API as shown below in the file.
- A header file "CHIPSET_userspace.h" should also be created at path <os/arch/arm/src/CHIPSET> and declare the below given API.
- The functionality of this API should be to clear all of user-space .bss and initialize all of user-space .data.
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

## MPU Initialization
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


## Allocation of kernel and user heaps
- The file 'CHIPSET_allocateheap.c' (path <os/arch/arm/src/CHIPSET>) needs to be modified for the kernel and user heap allocation.
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
- It is mandatory that API signature shown in the above snippets should not be changed.



## Changes in Make files
- The following additions should be made in the Make.defs at path <os/arch/arm/src/CHIPSET/Make.defs>

```
	ifeq ($(CONFIG_BUILD_PROTECTED),y)
	CHIP_CSRCS += CHIPSET_userspace.c CHIPSET_mpuinit.c
	endif
```

## Changes in the download method
- Different chipsets have different configs, scripts or different GUI based tools for download purposes.
- In protected build, two binaries (kernel and user) need to be downloaded. So, the download process should be modified to accomodate this.
- The download process should ensure that the user binary is flashed in the "app" partition given by CONFIG_FLASH_PART_NAME defined in 'defconfig' file.
