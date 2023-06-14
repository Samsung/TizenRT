/****************************************************************************
 * arch/arm/src/amebasmart/amebasmart_boot.c
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <assert.h>
#include <debug.h>

#ifdef CONFIG_PAGING
#  include <tinyara/page.h>
#endif

#include <arch/board/board.h>

#include "chip.h"
#include "arm.h"
#include "mmu.h"
#include "scu.h"
#include "arm_internal.h"
#include "amebasmart_config.h"
#include "amebasmart_clockconfig.h"
#include "amebasmart_memorymap.h"
#include "amebasmart_boot.h"
#include "sctlr.h"
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/


/****************************************************************************
 * Public Data
 ****************************************************************************/

/* Symbols defined via the linker script */

extern uint8_t _vector_start[]; /* Beginning of vector block */
extern uint8_t _vector_end[];   /* End+1 of vector block */

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: amebasmart_setupmappings
 *
 * Description:
 *   Map all of the initial memory regions defined in g_section_mapping[]
 *
 ****************************************************************************/

extern unsigned char __ram_nocache_start__[];
extern unsigned char __ram_nocache_end__[];
extern unsigned char __text_start__[];
extern unsigned char __text_end__[];

#ifndef CONFIG_ARCH_ROMPGTABLE
static inline void amebasmart_setupmappings(void)
{
#if 1
//FENG: tmp fix, set dynamic sections here. 
  g_section_mapping[4].physbase = (uint64_t)((int)__text_start__);
  g_section_mapping[4].virtbase = (uint64_t)((int)__text_start__);
  g_section_mapping[4].nsections = (((size_t)__text_end__ - (size_t)__text_start__)+0x000fffff) >> 20;

  g_section_mapping[5].physbase = (uint64_t)((int)__ram_nocache_start__);
  g_section_mapping[5].virtbase = (uint64_t)((int)__ram_nocache_start__);
  g_section_mapping[5].nsections = (((size_t)__ram_nocache_end__ - (size_t)__ram_nocache_start__)+0x000fffff) >> 20;
#endif
  mmu_l1_map_regions(g_section_mapping, g_num_mappings);
}
#else
#  define amebasmart_setupmappings()
#endif

/****************************************************************************
 * Name: amebasmart_remap
 *
 * Description:
 *   Map all of the final memory regions defined in g_operational_mapping[]
 *
 ****************************************************************************/

#ifdef NEED_SDRAM_REMAPPING
static inline void amebasmart_remap(void)
{
  mmu_l1_map_regions(g_operational_mapping, g_num_opmappings);
}
#endif

/****************************************************************************
 * Name: amebasmart_vectorpermissions
 *
 * Description:
 *   Set permissions on the vector mapping.
 *
 ****************************************************************************/

#if !defined(CONFIG_ARCH_ROMPGTABLE) && defined(CONFIG_ARCH_LOWVECTORS) && \
     defined(CONFIG_PAGING)
static void amebasmart_vectorpermissions(uint32_t mmuflags)
{
  /* The PTE for the beginning of OCRAM is at the base of the L2 page table */

  uint32_t pte = mmu_l2_getentry(PG_L2_VECT_VADDR, 0);

  /* Mask out the old MMU flags from the page table entry.
   *
   * The pte might be zero the first time this function is called.
   */

  if (pte == 0)
    {
      pte = PG_VECT_PBASE;
    }
  else
    {
      pte &= PG_L1_PADDRMASK;
    }

  /* Update the page table entry with the MMU flags and save */

  mmu_l2_setentry(PG_L2_VECT_VADDR, pte, 0, mmuflags);
}
#endif

/****************************************************************************
 * Name: amebasmart_vectorsize
 *
 * Description:
 *   Return the size of the vector data
 *
 ****************************************************************************/

static inline size_t amebasmart_vectorsize(void)
{
  return _vector_end - _vector_start;
}

/****************************************************************************
 * Name: amebasmart_vectormapping
 *
 * Description:
 *   Setup a special mapping for the interrupt vectors when the interrupt
 *   vectors are located at the high address, 0xffff0000.
 *
 ****************************************************************************/

#ifndef CONFIG_ARCH_LOWVECTORS
static void amebasmart_vectormapping(void)
{
  uint32_t vector_paddr = AMEBASMART_VECTOR_PADDR & PTE_SMALL_PADDR_MASK;
  uint32_t vector_vaddr = AMEBASMART_VECTOR_VADDR & PTE_SMALL_PADDR_MASK;
  uint32_t vector_size  = _vector_end - _vector_start;
  uint32_t end_paddr    = AMEBASMART_VECTOR_PADDR + vector_size;

  /* REVISIT:  Cannot really assert in this context */

  DEBUGASSERT (vector_size <= VECTOR_TABLE_SIZE);

  while (vector_paddr < end_paddr)
    {
      mmu_l2_setentry(VECTOR_L2_VBASE,  vector_paddr, vector_vaddr,
                      MMU_L2_VECTORFLAGS);
      vector_paddr += 4096;
      vector_vaddr += 4096;
    }

  /* Now set the level 1 descriptor to refer to the level 2 page table. */

  mmu_l1_setentry(VECTOR_L2_PBASE & PMD_PTE_PADDR_MASK,
                  AMEBASMART_VECTOR_VADDR & PMD_PTE_PADDR_MASK,
                  MMU_L1_VECTORFLAGS);
}
#else
  /* No vector remap */

#  define amebasmart_vectormapping()
#endif

/****************************************************************************
 * Name: amebasmart_copyvectorblock
 *
 * Description:
 *   Copy the interrupt block to its final destination.  Vectors are already
 *   positioned at the beginning of the text region and only need to be
 *   copied in the case where we are using high vectors or where the
 *   beginning of the text region cannot be remapped to address zero.
 *
 ****************************************************************************/

#ifndef CONFIG_ARCH_LOWVECTORS
static void amebasmart_copyvectorblock(void)
{
  uint32_t *src;
  uint32_t *end;
  uint32_t *dest;

#ifdef CONFIG_PAGING
  /* If we are using re-mapped vectors in an area that has been marked
   * read only, then temporarily mark the mapping write-able (non-buffered).
   */

  amebasmart_vectorpermissions(MMU_L2_VECTRWFLAGS);
#endif

  /* Copy the vectors into OCRAM at the address that will be mapped to the
   * vector address:
   *
   *   AMEBASMART_VECTOR_PADDR - Unmapped, physical address of vector table in OCRAM
   *   AMEBASMART_VECTOR_VSRAM - Virtual address of vector table in OCRAM
   *   AMEBASMART_VECTOR_VADDR - Virtual address of vector table (0x00000000 or
   *                      0xffff0000)
   */

  src  = (uint32_t *)_vector_start;
  end  = (uint32_t *)_vector_end;
  dest = (uint32_t *)AMEBASMART_VECTOR_VSRAM;

  while (src < end)
    {
      *dest++ = *src++;
    }

#if !defined(CONFIG_ARCH_LOWVECTORS) && defined(CONFIG_PAGING)
  /* Make the vectors read-only, cacheable again */

  amebasmart_vectorpermissions(MMU_L2_VECTORFLAGS);

#else
  /* Flush the DCache to assure that the vector data is in physical RAM */

  up_clean_dcache((uintptr_t)AMEBASMART_VECTOR_VSRAM,
                  (uintptr_t)AMEBASMART_VECTOR_VSRAM + amebasmart_vectorsize());
#endif
}

#else
/* Don't copy the vectors */

#  define amebasmart_copyvectorblock()
#endif

/****************************************************************************
 * Name: amebasmart_wdtdisable
 *
 * Description:
 *
 ****************************************************************************/

#ifndef CONFIG_AMEBASMART_WDT
static inline void amebasmart_wdtdisable(void)
{
  /* REVISIT: WDT initialization */
}
#else
#  define amebasmart_wdtdisable()
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: arm_boot
 *
 * Description:
 *   Complete boot operations started in arm_head.S
 *
 *
 ****************************************************************************/

void arm_boot(void)
{
#if defined(CONFIG_ARCH_RAMFUNCS)
  const uint32_t *src;
#endif
#if defined(CONFIG_ARCH_RAMFUNCS) || defined(CONFIG_SMP) && defined(SMP_INTERCPU_NONCACHED)
  uint32_t *dest;
#endif

  /* __start provided the basic MMU mappings for OCRAM.  Now provide mappings
   * for all IO regions (Including the vector region).
   */

  amebasmart_setupmappings();

  /* Make sure that all other CPUs are in the disabled state.  This is a
   * formality because the other CPUs are actually running then we have
   * probably already crashed.
   */

  amebasmart_cpu_disable();

#ifdef CONFIG_SMP
  /* Enable SMP cache coherency for CPU0 */

  arm_enable_smp(0);
#endif

  /* Provide a special mapping for the OCRAM interrupt vector positioned in
   * high memory.
   */

  amebasmart_vectormapping();

#ifdef CONFIG_ARCH_RAMFUNCS
  /* Copy any necessary code sections from FLASH to RAM.  The correct
   * destination in OCRAM is given by _sramfuncs and _eramfuncs.  The
   * temporary location is in flash after the data initialization code
   * at _framfuncs
   */

  for (src = (const uint32_t *)_framfuncs,
       dest = (uint32_t *)_sramfuncs; dest < (uint32_t *)_eramfuncs;
      )
    {
      *dest++ = *src++;
    }

  /* Flush the copied RAM functions into physical RAM so that will
   * be available when fetched into the I-Cache.
   */

  up_clean_dcache((uintptr_t)_sramfuncs, (uintptr_t)_eramfuncs)
#endif

  /* Setup up vector block.  _vector_start and _vector_end are exported from
   * arm_vector.S
   */

  amebasmart_copyvectorblock();

  /* Disable the watchdog timer */

  amebasmart_wdtdisable();

  /* Initialize clocking to settings provided by board-specific logic */

  amebasmart_clockconfig();

  /* Initialize the FPU */

  arm_fpuconfig();

  /* Perform board-specific memory initialization,  This must include
   * initialization of board-specific memory resources (e.g., SDRAM)
   *
   * NOTE: We must use caution prior to this point to make sure that
   * the logic does not access any global variables that might lie
   * in SDRAM.
   */

  amebasmart_memory_initialize();

#ifdef NEED_SDRAM_REMAPPING
  /* SDRAM was configured in a temporary state to support low-level
   * initialization.  Now that the SDRAM has been fully initialized,
   * we can reconfigure the SDRAM in its final, fully cache-able state.
   */

  amebasmart_remap();
#endif

#ifdef CONFIG_BOOT_SDRAM_DATA
  /* If .data and .bss reside in SDRAM, then initialize the data sections
   * now after SDRAM has been initialized.
   */

  arm_data_initialize();
#endif

  /* Perform board-specific device initialization. This would include
   * configuration of board specific resources such as GPIOs, LEDs, etc.
   */

  /* Done in os_start->os_bringup()->os_start_application()->os_do_appstart()->board_initialize()*/
  // board_initialize();

  /* Perform common, low-level chip initialization (might do nothing) */

  // amebasmart_lowsetup();

#ifdef USE_EARLYSERIALINIT
  /* Perform early serial initialization if we are going to use the serial
   * driver.
   */

  // amebasmart_serialinit();
#endif

  /* Now we can enable all other CPUs.  The enabled CPUs will start execution
   * at __cpuN_start and, after very low-level CPU initialization has been
   * performed, will branch to arm_cpu_boot()
   * (see arch/arm/src/armv7-a/smp.h)
   */

  //SMP pending for Tizen Confirmation//
  cp15_wrvbar((uint32_t)_vector_start);
  amebasmart_cpu_enable();

  // TBD: Complete the rest of the step in app_start, prvSetupHardware will be done in os_start()
  app_start();
}

