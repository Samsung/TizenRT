/****************************************************************************
 * arch/arm/src/AMEBASMART/hardware/AMEBASMART_memorymap.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_AMEBASMART_HARDWARE_AMEBASMART_MEMORYMAP_H
#define __ARCH_ARM_SRC_AMEBASMART_HARDWARE_AMEBASMART_MEMORYMAP_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <arch/amebasmart/chip.h>

/* AmebaSmart Virtual (mapped) Memory Map
 *
 * board_memorymap.h contains special mappings that are needed when a ROM
 * memory map is used.  It is included in this odd location because it
 * depends on some the virtual address definitions provided above.
 */

// #include <arch/board/board_memorymap.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Decimal configuration values may exceed 2Gb and, hence, overflow to
 * negative values unless we force them to unsigned long:
 */

#define __CONCAT(a,b) a ## b
#define MKULONG(a) __CONCAT(a,ul)

/* Overview *****************************************************************
 *
 *  AmebaSmart Physical (unmapped) Memory Map
 *  - AmebaSmart System PSECTIONS
 *  Sizes of memory regions in bytes
 *  Sizes of memory regions in sections
 *  Section MMU Flags
 *  AmebaSmart Virtual (mapped) Memory Map
 *  MMU Page Table Location (Need to check)
 *  Page table start addresses (Need to check)
 *  Base address of the interrupt vector table
 *
 ****************************************************************************/

/* AmebaSmart Physical (unmapped) Memory Map *************************************/
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
/* AmebaSmart System PSECTIONS */

#define AMEBASMART_ROMCP_PSECTION       0x00000000  /* 0x00000000 - 0x00100000  1MB Boot ROM */
#define AMEBASMART_FLASH_PSECTION       0x08000000  /* 0x08000000 - 0x10000000  128MB Flash */
#define AMEBASMART_SRAM_PSECTION        0x20000000  /* 0x20000000 - 0x30000000  256MB NS SRAM */
#define AMEBASMART_MMAP_DEV0_PSECTION   0x40000000  /* 0x40000000 - 0x50000000  256MB NS Memory mapped device */
//#define AMEBASMART_NS_PSECTION          (uint64_t)((int)__text_start__)  /* __text_start__ - __text_end__  MB NS code */
//#define AMEBASMART_NON_CACHE_PSECTION   (uint64_t)((int)__ram_nocache_start__)  /* __ram_nocache_start__ - __ram_nocache_end__  MB Non-cache code */
#define AMEBASMART_DRAM_PSECTION        0x60000000  /* 0x60000000 - 0x70000000  256MB NS Other DRAM */
#define AMEBASMART_MMAP_DEV1_PSECTION   0x80000000  /* 0x80000000 - 0xFFFFFFFF  2048MB Memory mapped device */

/* Sizes of memory regions in bytes.
 *
 * These sizes exclude the undefined addresses at the end of the memory
 * region.  The implemented sizes of the EBI CS0-3 and DDRCS regions
 * are not known apriori and must be specified with configuration settings.
 */

#define AMEBASMART_ROMCP_SECSIZE          (1024*1024) /* 0x00000000 - 0x00100000  1MB Boot ROM */
#define AMEBASMART_FLASH_SECSIZE          MKULONG(128*1024*1024) /* 0x08000000 - 0x10000000  128MB Flash */
#define AMEBASMART_SRAM_SECSIZE           MKULONG(256*1024*1024) /* 0x20000000 - 0x40000000  256MB SRAM */
#define AMEBASMART_MMAP_DEV0_SECSIZE      MKULONG(256*1024*1024) /* 0x40000000 - 0x60000000  256MB Memory mapped device */
//#define AMEBASMART_NS_SECSIZE             (size_t)__text_end__ - (size_t)__text_start__ /* __text_start__ - __text_end__  MB NS code */
//#define AMEBASMART_NON_CACHE_SECSIZE      (size_t)__ram_nocache_end__ - (size_t)__ram_nocache_start__ /* __ram_nocache_start__ - __ram_nocache_end__  MB Non-cache code */
#define AMEBASMART_DRAM_SECSIZE           MKULONG(256*1024*1024)  /* 0x60000000 - 0x80000000  256MB Other DRAM */
#define AMEBASMART_MMAP_DEV1_SECSIZE      MKULONG(2048*1024*1024) /* 0x80000000 - 0xFFFFFFFF  2048MB Memory mapped device */

/* Convert size in bytes to number of sections (in Mb). */

#define _NSECTIONS(b)            (((b)+0x000fffff) >> 20)

/* Sizes of memory regions in sections.
 *
 * The boot logic in sam_boot.c, will select 1Mb level 1 MMU mappings to
 * span the entire physical address space.  The definitions below specify
 * the number of 1Mb entries that are required to span a particular address
 * region.
 */

#define AMEBASMART_ROMCP_NSECTIONS      _NSECTIONS(AMEBASMART_ROMCP_SECSIZE)
#define AMEBASMART_FLASH_NSECTIONS      _NSECTIONS(AMEBASMART_FLASH_SECSIZE)
#define AMEBASMART_SRAM_NSECTIONS       _NSECTIONS(AMEBASMART_SRAM_SECSIZE)
#define AMEBASMART_MMAP_DEV0_NSECTIONS  _NSECTIONS(AMEBASMART_MMAP_DEV0_SECSIZE)
//#define AMEBASMART_NS_NSECTIONS         _NSECTIONS(AMEBASMART_NS_SECSIZE)
//#define AMEBASMART_NON_CACHE_NSECTIONS  _NSECTIONS(AMEBASMART_NON_CACHE_SECSIZE)
#define AMEBASMART_DRAM_NSECTIONS       _NSECTIONS(AMEBASMART_DRAM_SECSIZE)
#define AMEBASMART_MMAP_DEV1_NSECTIONS  _NSECTIONS(AMEBASMART_MMAP_DEV1_SECSIZE)

/* Section MMU Flags
 *
 * DRAM is a special case because it requires non-cached access of its
 * initial configuration, then cached access thereafter.
 */

#define AMEBASMART_ROMCP_MMUFLAGS       MMU_ROMFLAGS
#define AMEBASMART_FLASH_MMUFLAGS       MMU_MEMFLAGS
#define AMEBASMART_SRAM_MMUFLAGS        MMU_MEMFLAGS
#define AMEBASMART_MMAP_DEV0_MMUFLAGS   MMU_IOFLAGS
#define AMEBASMART_NS_MMUFLAGS          MMU_MEMFLAGS
#define AMEBASMART_NON_CACHE_MMUFLAGS   MMU_MEMFLAGS
#define AMEBASMART_DRAM_MMUFLAGS        MMU_MEMFLAGS
#define AMEBASMART_MMAP_DEV1_MMUFLAGS   MMU_IOFLAGS

/* AmebaSmart Virtual (mapped) Memory Map.  These are the mappings that will
 * be created if the page table lies in RAM.  If the platform has another,
 * read-only, pre-initialized page table (perhaps in ROM), then the board.h
 * file must provide these definitions.
 */

#ifndef CONFIG_ARCH_ROMPGTABLE

/* The default mappings are a simple 1-to-1 mapping */

#  define AMEBASMART_ROMCP_VSECTION     AMEBASMART_ROMCP_PSECTION       /*  1MB Boot ROM  */
#  define AMEBASMART_FLASH_VSECTION     AMEBASMART_FLASH_PSECTION       /*  128MB Flash  */
#  define AMEBASMART_SRAM_VSECTION      AMEBASMART_SRAM_PSECTION        /*  512MB SRAM  */
#  define AMEBASMART_MMAP_DEV0_VSECTION AMEBASMART_MMAP_DEV0_PSECTION   /*  512MB Memory mapped device  */
//#  define AMEBASMART_NS_VSECTION        AMEBASMART_NS_PSECTION          /*  MB NS code  */
//#  define AMEBASMART_NON_CACHE_VSECTION AMEBASMART_NON_CACHE_PSECTION   /*  MB Non-cache code  */
#  define AMEBASMART_DRAM_VSECTION      AMEBASMART_DRAM_PSECTION        /*  512MB Other DRAM  */
#  define AMEBASMART_MMAP_DEV1_VSECTION AMEBASMART_MMAP_DEV1_PSECTION   /*  2048MB Memory mapped device  */
#endif /* CONFIG_ARCH_ROMPGTABLE */

/////////////// Temporary Added, Need to modify ///////////////
/////////////// L2CC_PL310 is an external cache controller for A9 processor, not specified in A32
/////////////// defined a temporary address to prevent compile error for l2cc_pl310.h
#define AMEBASMART_DRAM_OFFSET         0x00300000
#define AMEBASMART_GIC_OFFSET          0x20100000  /* A0100000-A0107FFF  32 KB GIC register */
#define AMEBASMART_GIC_PBASE           (AMEBASMART_MMAP_DEV1_PSECTION + AMEBASMART_GIC_OFFSET)
#define AMEBASMART_GIC_VBASE           (AMEBASMART_MMAP_DEV1_VSECTION + AMEBASMART_GIC_OFFSET)
// #define AMEBASMART_PL310_OFFSET        0x20140000  /* Not used */
// #define AMEBASMART_PL310_PBASE         (AMEBASMART_MMAP_DEV1_PSECTION + AMEBASMART_PL310_OFFSET)  /* Not used */
// #define AMEBASMART_PL310_VBASE         (AMEBASMART_MMAP_DEV1_VSECTION + AMEBASMART_PL310_OFFSET)  /* Not used */
#define AMEBASMART_DRAM_PBASE          (AMEBASMART_DRAM_PSECTION + AMEBASMART_DRAM_OFFSET) // Starting base from PSRAM for BL33 (0x60300000)
#define AMEBASMART_DRAM_VBASE          (AMEBASMART_DRAM_VSECTION + AMEBASMART_DRAM_OFFSET) // Starting base from PSRAM for BL33 (0x60300000)

#ifdef CONFIG_AMEBASMART_BOOT_NOR

/* Some sanity checks.  If we are running from FLASH, then one of the
 * external chip selects must be configured to boot from NOR flash.
 * And, if so, then its size must agree with the configured size.
 */

#  if defined(CONFIG_AMEBASMART_EIM) && defined(CONFIG_AMEBASMART_EIM_NOR)
#    if CONFIG_AMEBASMART_EIM_SIZE != CONFIG_FLASH_SIZE
#      error EIM FLASH size disagreement
#    endif
#  else
#    error CONFIG_AMEBASMART_BOOT_NOR=y, but no bootable NOR flash defined
#  endif

  /* Set up the NOR FLASH region as the TIZENRT .text region */

#  define TIZENRT_TEXT_VADDR       (CONFIG_FLASH_VSTART & 0xfff00000)
#  define TIZENRT_TEXT_PADDR       (CONFIG_FLASH_START_ADDR & 0xfff00000)
#  define TIZENRT_TEXT_PEND        ((CONFIG_FLASH_END + 0x000fffff) & 0xfff00000)
#  define TIZENRT_TEXT_SIZE        (TIZENRT_TEXT_PEND - TIZENRT_TEXT_PADDR)

  /* The "primary" RAM is the DRAM or SRAM used for .bss and .data */

#  define TIZENRT_RAM_VADDR        (CONFIG_RAM_VSTART & 0xfff00000)
#  define TIZENRT_RAM_PADDR        (CONFIG_RAM_START & 0xfff00000)
#  define TIZENRT_RAM_PEND         ((CONFIG_RAM_END + 0x000fffff) & 0xfff00000)
#  define TIZENRT_RAM_SIZE         (TIZENRT_RAM_PEND - TIZENRT_RAM_PADDR)

#else /* CONFIG_AMEBASMART_BOOT_NOR */

  /* Must be
   * CONFIG_AMEBASMART_BOOT_SRAM ||
   * CONFIG_AMEBASMART_BOOT_DRAM ||
   */

  /* Otherwise we are running from some kind of RAM (SRAM, DRAM).
   * Setup the RAM region as the TIZENRT .txt, .bss, and .data region.
   */

#  define TIZENRT_TEXT_VADDR       (CONFIG_RAM_VSTART & 0xfff00000)
#  define TIZENRT_TEXT_PADDR       (CONFIG_RAM_START & 0xfff00000)
#  define TIZENRT_TEXT_PEND        ((CONFIG_RAM_END + 0x000fffff) & 0xfff00000)
#  define TIZENRT_TEXT_SIZE        (TIZENRT_TEXT_PEND - TIZENRT_TEXT_PADDR)

#endif /* CONFIG_AMEBASMART_BOOT_NOR */

/* MMU Page Table Location
 *
 * Determine the address of the MMU page table.  We will always attempt to
 * use the bottom 16KB of RAM (SRAM or DRAM) for the page table, but there
 * are a few conditions that affect this:
 *
 * If CONFIG_ARCH_ROMPGTABLE, then the page table resides in ROM and we will
 * not use any page table in RAM, and in that case the user must specify the
 * address of the page table explicitly by defining PGTABLE_BASE_VADDR and
 * PGTABLE_BASE_PADDR in the board.h file.
 */

#undef PGTABLE_IN_HIGHSRAM
#undef PGTABLE_IN_LOWSRAM
#undef ARMV7A_PGTABLE_MAPPING /* We do not remap the page table */

/* Check if the user has configured the page table address */

#if !defined(PGTABLE_BASE_PADDR) || !defined(PGTABLE_BASE_VADDR)

  /* Sanity check.. if one is undefined, both should be undefined */

#  if defined(PGTABLE_BASE_PADDR) || defined(PGTABLE_BASE_VADDR)
#    error "Only one of PGTABLE_BASE_PADDR or PGTABLE_BASE_VADDR is defined"
#  endif

/* A sanity check, if the configuration says that the page table is read-only
 * and pre-initialized (maybe ROM), then it should have also defined both of
 * the page table base addresses.
 */

#  ifdef CONFIG_ARCH_ROMPGTABLE
#    error "CONFIG_ARCH_ROMPGTABLE defined; PGTABLE_BASE_P/VADDR not defined"
#  endif

/* We must declare the page table at the bottom or at the top of SRAM.
 * Yes.. do the vectors lie in low memory?
 */

#  ifdef CONFIG_ARCH_LOWVECTORS

/* In this case, page table must lie at the top 16Kb * ncpus of SRAM. */

#    define PGTABLE_BASE_PADDR    (AMEBASMART_DRAM_PBASE + AMEBASMART_DRAM_SIZE - ALL_PGTABLE_SIZE)
#    define PGTABLE_BASE_VADDR    (AMEBASMART_DRAM_VBASE + AMEBASMART_DRAM_SIZE - ALL_PGTABLE_SIZE)
#    define PGTABLE_IN_HIGHSRAM   1

/* We will force the IDLE stack to precede the page table */

#    define IDLE_STACK_PBASE      (PGTABLE_BASE_PADDR - CONFIG_IDLETHREAD_STACKSIZE)
#    define IDLE_STACK_VBASE      (PGTABLE_BASE_VADDR - CONFIG_IDLETHREAD_STACKSIZE)

#  else /* CONFIG_ARCH_LOWVECTORS */

/* Otherwise, the vectors lie at another location (perhaps in NOR FLASH,
 * perhaps elsewhere in SRAM).  The page table will then be positioned
 * at the first 16Kb * ncpus of SRAM.
 */

#    define PGTABLE_BASE_PADDR    AMEBASMART_DRAM_PBASE
#    define PGTABLE_BASE_VADDR    AMEBASMART_DRAM_VBASE
#    define PGTABLE_IN_LOWSRAM    1

/* We will force the IDLE stack to follow the page table */

#    define IDLE_STACK_PBASE      (PGTABLE_BASE_PADDR + ALL_PGTABLE_SIZE)
#    define IDLE_STACK_VBASE      (PGTABLE_BASE_VADDR + ALL_PGTABLE_SIZE)

#  endif /* CONFIG_ARCH_LOWVECTORS */

  /* In either case, the page table lies in SRAM.  If SRAM is not the
   * primary RAM region, then we will need to set-up a special mapping for
   * the page table at boot time.
   */

#  if defined(CONFIG_BOOT_RUNFROMFLASH)
  /* If we are running from FLASH, then the primary memory region is
   * given by TIZENRT_RAM_PADDR.
   */

#    if TIZENRT_RAM_PADDR != SAM_OCRAM_PSECTION
#      define ARMV7A_PGTABLE_MAPPING 1
#    endif

/* Otherwise, we are running from RAM and that RAM is also the primary
 * RAM.  If that is not SRAM, then we will need to create a mapping
 * for the SRAM at start-up.
 */

#  elif !defined(CONFIG_AMEBASMART_BOOT_SRAM)
#    define ARMV7A_PGTABLE_MAPPING 1
#  endif

#else /* !PGTABLE_BASE_PADDR || !PGTABLE_BASE_VADDR */

/* Sanity check.. if one is defined, both should be defined */

#  if !defined(PGTABLE_BASE_PADDR) || !defined(PGTABLE_BASE_VADDR)
#    error "One of PGTABLE_BASE_PADDR or PGTABLE_BASE_VADDR is undefined"
#  endif

/* The page table then lies at the beginning of the OSSRAM and
 * the IDLE stack follows immediately.
 */

#    define PGTABLE_BASE_PADDR    AMEBASMART_DRAM_PBASE
#    define PGTABLE_BASE_VADDR    AMEBASMART_DRAM_VBASE
#    define PGTABLE_IN_LOWSRAM    1

/* We will force the IDLE stack to follow the page table */

#    define IDLE_STACK_PBASE      (PGTABLE_BASE_PADDR + ALL_PGTABLE_SIZE)
#    define IDLE_STACK_VBASE      (PGTABLE_BASE_VADDR + ALL_PGTABLE_SIZE)

#endif /* !PGTABLE_BASE_PADDR || !PGTABLE_BASE_VADDR */

/* Level 2 Page table start addresses.
 *
 * The maximum size of the L1 page table is:
 *
 *  (4GB address range / 1 MB per section ) * 4 bytes per entry = 16KB
 *
 * The maximum size of the L2 page table is:
 *
 *  (4GB address range / 4 KB per page ) * 4 bytes per entry = 4MB
 *
 * 16KB of memory is reserved hold the page table for the virtual mappings.
 * A portion of this table is not accessible in the virtual address space
 * (for normal operation with a one-to-one address mapping).   There is this
 * large hole in the physcal address space for which there will never be
 * level 1 mappings:
 *
 *   0x80000000-0xefffffff: Undefined (1.75 GB)
 *
 * That is the offset where the main L2 page tables will be positioned.  This
 * corresponds to page table offsets 0x00002000 up to 0x00003c00.  That
 * is 1792 entries, each mapping 4KB of address for a total of 7MB of virtual
 * address space)
 *
 * Up to two L2 page tables may be used:
 *
 * 1) One mapping the vector table.  However, L2 page tables must be aligned
 *    to 1KB address boundaries, so the minimum L2 page table size is then
 *    1KB, mapping up a full megabyte of virtual address space.
 *
 *    This L2 page table is only allocated if CONFIG_ARCH_LOWVECTORS is *not*
 *    defined.  The AmebaSmart boot-up logic will map the beginning of the boot
 *    memory to address 0x0000:0000 using both the MMU and the AXI matrix
 *    REMAP register.  So no L2 page table is required.
 *
 * 2) If on-demand paging is supported (CONFIG_PAGING=y), than an additional
 *    L2 page table is needed.  This page table will use the remainder of
 *    the address space.
 */

#ifndef CONFIG_ARCH_LOWVECTORS
  /* Memory map
   * VIRTUAL ADDRESS RANGE L1 PG TABLE L2 PG TABLE  DESCRIPTION
   * START      END        OFFSET      SIZE
   * ---------- ---------- ------------ ----------------------------
   * 0x80000000 0x803fffff 0x000002000 0x000000400  Vectors (1MiB)
   * 0x80100000 0x806fffff 0x000002400 0x000001800  Paging  (6MiB)
   */

  /* Vector L2 page table offset/size */

#  define VECTOR_L2_OFFSET        0x000002000
#  define VECTOR_L2_SIZE          0x000000400

  /* Vector L2 page table base addresses */

#  define VECTOR_L2_PBASE         (PGTABLE_BASE_PADDR + VECTOR_L2_OFFSET)
#  define VECTOR_L2_VBASE         (PGTABLE_BASE_VADDR + VECTOR_L2_OFFSET)

  /* Vector L2 page table end addresses */

#  define VECTOR_L2_END_PADDR     (VECTOR_L2_PBASE + VECTOR_L2_SIZE)
#  define VECTOR_L2_END_VADDR     (VECTOR_L2_VBASE + VECTOR_L2_SIZE)

  /* Paging L2 page table offset/size */

#  define PGTABLE_L2_OFFSET       0x000002400
#  define PGTABLE_L2_SIZE         0x000001800

#else
  /* Memory map
   * VIRTUAL ADDRESS RANGE L1 PG TABLE L2 PG TABLE  DESCRIPTION
   * START      END        OFFSET      SIZE
   * ---------- ---------- ------------ ----------------------------
   * 0x80000000 0x806fffff 0x000002000 0x000001c00  Paging  (7MiB)
   */

  /* Paging L2 page table offset/size */

#  define PGTABLE_L2_OFFSET       0x000002000
#  define PGTABLE_L2_SIZE         0x000001c00

#endif

/* Paging L2 page table base addresses
 *
 * NOTE: If CONFIG_PAGING is defined, mmu.h will re-assign the virtual
 * address of the page table.
 */

#define PGTABLE_L2_PBASE          (PGTABLE_BASE_PADDR + PGTABLE_L2_OFFSET)
#define PGTABLE_L2_VBASE          (PGTABLE_BASE_VADDR + PGTABLE_L2_OFFSET)

/* Paging L2 page table end addresses */

#define PGTABLE_L2_END_PADDR      (PGTABLE_L2_PBASE + PGTABLE_L2_SIZE)
#define PGTABLE_L2_END_VADDR      (PGTABLE_L2_VBASE + PGTABLE_L2_SIZE)

/* Base address of the interrupt vector table.
 *
 *   AMEBASMART_VECTOR_PADDR - Unmapped, physical address of vector table in SRAM
 *   AMEBASMART_VECTOR_VSRAM - Virtual address of vector table in SRAM
 *   AMEBASMART_VECTOR_VADDR - Virtual address of vector table
 *                      (0x00000000 or 0xffff0000)
 */

#define VECTOR_TABLE_SIZE         0x00010000

/* REVISIT: These definitions are not used:  The vector table is at some
 * arbitrary (but aligned) position in RAM or NOR FLASH and is positioned
 * using the VBAR register.
 */

#ifdef CONFIG_ARCH_LOWVECTORS  /* Vectors located at 0x0000:0000  */
/* Vectors will always lie at the beginning of SRAM
 *
 * SRAM Memory Map:
 * ---------- ---------- ---------------------------
 * START      END        CONTENT
 * ---------- ---------- ---------------------------
 * 0x00000000 0x00010000 Vectors (VECTOR_TABLE_SIZE)
 * 0x00010000 0x0003c000 Unused
 * 0x0003c000 0x00004000 Page table (PGTABLE_SIZE)
 */

#  define AMEBASMART_VECTOR_PADDR        AMEBASMART_DRAM_PBASE
#  define AMEBASMART_VECTOR_VSRAM        AMEBASMART_DRAM_VBASE
#  define AMEBASMART_VECTOR_VADDR        0x00000000

#else  /* Vectors located at 0xffff:0000 -- this probably does not work */
/* SRAM Memory Map:
 * ---------- ---------- ---------------------------
 * START      END        CONTENT
 * ---------- ---------- ---------------------------
 * 0x00000000 0x00004000 Page table (PGTABLE_SIZE)
 * 0x00004000 0x00030000 Unused
 * 0x00030000 0x00010000 Vectors (VECTOR_TABLE_SIZE)
 */

#  define AMEBASMART_VECTOR_PADDR        (AMEBASMART_DRAM_PBASE + AMEBASMART_DRAM_SIZE - VECTOR_TABLE_SIZE)
#  define AMEBASMART_VECTOR_VSRAM        (AMEBASMART_DRAM_VBASE + AMEBASMART_DRAM_SIZE - VECTOR_TABLE_SIZE)
#  define AMEBASMART_VECTOR_VADDR        0xffff0000

#endif

#endif /* __ARCH_ARM_SRC_AMEBASMART_HARDWARE_AMEBASMART_MEMORYMAP_H */

