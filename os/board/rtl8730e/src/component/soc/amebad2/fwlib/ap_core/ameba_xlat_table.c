/*
 * Copyright (C) 2020 Realtek Semiconductor Corp.  All Rights Reserved.
 */

#include "xlat_tables_v2.h"
#include "basic_types.h"

#if defined ( __ICCARM__ )
SECTION(".data") u8 *__bss_start__ = 0;
SECTION(".data") u8 *__bss_end__ = 0;
SECTION(".data") u8 *__ram_nocache_start__ = 0;
SECTION(".data") u8 *__ram_nocache_end__ = 0;
SECTION(".data") u8 *__text_start__ = 0;
SECTION(".data") u8 *__text_end__ = 0;
SECTION(".data") u8 *__data_start__ = 0;

extern unsigned char IMAGEBSS$$Base[];
extern unsigned char IMAGEBSS$$Limit[];
extern unsigned char IMAGETEXT$$Base[];
extern unsigned char IMAGETEXT$$Limit[];
extern unsigned char IMAGENCSRAM$$Base[];
extern unsigned char IMAGENCSRAM$$Limit[];
#else
extern unsigned char __ram_nocache_start__[];
extern unsigned char __ram_nocache_end__[];
extern unsigned char __bss_start__[];
extern unsigned char __bss_end__[];
extern unsigned char __text_start__[];
extern unsigned char __text_end__[];
extern unsigned char __data_start__[];
#endif

void mmap_add_region(unsigned long long base_pa, uintptr_t base_va, size_t size,
					 unsigned int attr);

void *memset(void *dst, int val, size_t count);

/*|                       		| Memory Range            	| Definition in Translation Table   |
*|----------------------|-------------------------|------------------------------------------------|
*|Boot ROM				| 0x00000000 - 0x00100000-1 | Cacheable, write-back, write-allocate, RO, EXECUTE  	|
*|Flash					| 0x08000000 - 0x10000000-1 | Cacheable, write-back, write-allocate, RO, EXECUTE		|
*|SRAM					| 0x20000000 - 0x40000000-1 | Cacheable, write-back, write-allocate, RW, XN			|
*|Memory mapped device	| 0x40000000 - 0x60000000-1 | Device Memory									|
*|NS code				| __text_start__ - __text_end__ | Cacheable, write-back, write-allocate, RO, EXECUTE	|
*|Non-cache memory		| __ram_nocache_start__ - __ram_nocache_end__ | Non-cacheable, RW				|
*|Other DRAM			| 0x60000000 - 0x80000000-1 | Cacheable, write-back, write-allocate, RW, XN			|
*|Memory mapped device	| 0x80000000 - 0xFFFFFFFF   | Device Memory										|
*
******************************************************************************/

void mmap_section_init(void)
{
#if defined ( __ICCARM__ )
	__text_start__				= IMAGETEXT$$Base;
	__text_end__					= IMAGETEXT$$Limit;
	__bss_start__				= IMAGEBSS$$Base;
	__bss_end__					= IMAGEBSS$$Limit;
	__ram_nocache_start__			= IMAGENCSRAM$$Base;
	__ram_nocache_end__			=  IMAGENCSRAM$$Limit;
	__ram_nocache_end__ = (u8 *)(((((u32)__ram_nocache_end__ - 1) >> 5) + 1) << 5); //32-byte aligned
#endif
}

void setupMMUTable(int coreID)
{
	if (coreID == 1) {
		mmap_section_init();
		/* core0: clear bss, create MMU table */
		memset((void *)__bss_start__, 0, (size_t)__bss_end__ - (size_t)__bss_start__);

		/* NOTE: For RW Normal Memory, default XN. For RO Normal Memory, it can be EXC/XN */
		mmap_add_region(0x00000000, 0x00000000, 0x00100000, MT_CODE | MT_NS);
		mmap_add_region(0x08000000, 0x08000000, 0x10000000 - 0x08000000, MT_CODE | MT_NS);
		mmap_add_region(0x20000000, 0x20000000, 0x40000000 - 0x20000000, MT_MEMORY | MT_RW | MT_NS);
		mmap_add_region(0x40000000, 0x40000000,	0x60000000 - 0x40000000, MT_DEVICE | MT_RW | MT_NS);

		mmap_add_region((uint64_t)((int)__text_start__), (uintptr_t)__text_start__, (size_t)__text_end__ - (size_t)__text_start__,  \
						MT_CODE | MT_NS);
		mmap_add_region((uint64_t)((int)__ram_nocache_start__), (uintptr_t)__ram_nocache_start__, (size_t)__ram_nocache_end__ - (size_t)__ram_nocache_start__,  \
						MT_NON_CACHEABLE | MT_RW | MT_NS);
		mmap_add_region(0x60000000, 0x60000000,	0x80000000 - 0x60000000, MT_MEMORY | MT_RW | MT_NS);

		mmap_add_region(0x80000000, 0x80000000,	0x80000000, MT_DEVICE | MT_RW | MT_NS);

		init_xlat_tables();

	}
	/* core0 & 1: set BBT0 */
	enable_mmu_svc_mon(0);
}
