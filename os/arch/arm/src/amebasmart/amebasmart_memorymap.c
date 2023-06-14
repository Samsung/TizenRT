/****************************************************************************
 * arch/arm/src/amebasmart/amebasmart_memorymap.c
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

#include "mmu.h"

#include "hardware/amebasmart_memorymap.h"
#include "amebasmart_memorymap.h"

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifndef CONFIG_ARCH_ROMPGTABLE
/* This table describes how to map a set of 1Mb pages to space the physical
 * address space of the amebaSmart.
 */

struct section_mapping_s g_section_mapping[] =
{
  /* AmebaSmart Address Sections Memories */

  /* If CONFIG_ARCH_LOWVECTORS is defined, then the vectors located at the
   * beginning of the .text region must appear at address at the address
   * specified in the VBAR.  There are two ways to accomplish this:
   *
   *   1. By explicitly mapping the beginning of .text region with a page
   *      table entry so that the virtual address zero maps to the beginning
   *      of the .text region.  VBAR == 0x0000:0000.
   *
   *   2. Set the Cortex-A5 VBAR register so that the vector table address
   *      is moved to a location other than 0x0000:0000.
   *
   *      This is the method used by this logic.
   *
   * The system always boots from the ROM memory at address 0x0.  After
   * reset, and until the Remap command is performed, the OCRAM is accessible
   * at address 0x0090 0000.
   *
   * If we are executing from external SDRAM, then a secondary bootloader
   * must have loaded us into SDRAM.  In this case, simply set the VBAR
   * register to the address of the vector table (not necessary at the
   * beginning or SDRAM).
   */

  { AMEBASMART_ROMCP_PSECTION,     AMEBASMART_ROMCP_VSECTION,       /* Boot ROM (ROMCP) */
    AMEBASMART_ROMCP_MMUFLAGS,     AMEBASMART_ROMCP_NSECTIONS
  },
  { AMEBASMART_FLASH_PSECTION,     AMEBASMART_FLASH_VSECTION,       /* Flash */
    AMEBASMART_FLASH_MMUFLAGS,     AMEBASMART_FLASH_NSECTIONS
  },
  { AMEBASMART_SRAM_PSECTION,      AMEBASMART_SRAM_VSECTION,        /* SRAM */
    AMEBASMART_SRAM_MMUFLAGS,      AMEBASMART_SRAM_NSECTIONS
  },
  { AMEBASMART_MMAP_DEV0_PSECTION, AMEBASMART_MMAP_DEV0_VSECTION,   /* Memory Mapped Device */
    AMEBASMART_MMAP_DEV0_MMUFLAGS, AMEBASMART_MMAP_DEV0_NSECTIONS
  },
  { 0,        0,          /* NS code */
    AMEBASMART_NS_MMUFLAGS,        0
  },
  { 0, 0,   /* Non-cache code */
    AMEBASMART_NON_CACHE_MMUFLAGS, 0
  },
  { AMEBASMART_DRAM_PSECTION,      AMEBASMART_DRAM_VSECTION,        /* Other DRAM */
    AMEBASMART_DRAM_MMUFLAGS,      AMEBASMART_DRAM_NSECTIONS
  },
  { AMEBASMART_MMAP_DEV1_PSECTION, AMEBASMART_MMAP_DEV1_VSECTION,   /* Memory Mapped Device */
    AMEBASMART_MMAP_DEV1_MMUFLAGS, AMEBASMART_MMAP_DEV1_NSECTIONS
  },
};

/* The number of entries in the mapping table */

#define NMAPPINGS \
  (sizeof(g_section_mapping) / sizeof(struct section_mapping_s))

const size_t g_num_mappings = NMAPPINGS;

#endif /* CONFIG_ARCH_ROMPGTABLE */

/* AmebaSmart External SDRAM Memory.  Final configuration.  The SDRAM was
 * configured in a temporary state to support low-level ininitialization.
 * After the SDRAM has been fully initialized, this structure is used to
 * set the SDRM in its final, fully cache-able state.
 */

#ifdef NEED_SDRAM_REMAPPING
const struct section_mapping_s g_operational_mapping[] =
{
  /* This entry reprograms the SDRAM entry, making it cacheable and
   * bufferable.
   */

  { AMEBASMART_MMDCDDR_PSECTION,   AMEBASMART_MMDCDDR_VSECTION,   /* MMDC-DDR Controller */
    AMEBASMART_MMDCDDR_MMUFLAGS,   AMEBASMART_MMDCDDR_NSECTIONS
  },

  /* LCDC Framebuffer.  This entry reprograms a part of one of the above
   * regions, making it non-cacheable and non-buffereable.
   */

#ifdef CONFIG_AMEBASMART_LCDC
  {CONFIG_AMEBASMART_LCDC_FB_PBASE, CONFIG_AMEBASMART_LCDC_FB_VBASE,
    MMU_IOFLAGS, AMEBASMART_LCDC_FBNSECTIONS
  },
#endif
};

/* The number of entries in the operational mapping table */

#define NREMAPPINGS \
  (sizeof(g_operational_mapping) / sizeof(struct section_mapping_s))

const size_t g_num_opmappings = NREMAPPINGS;

#endif /* NEED_SDRAM_REMAPPING */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

