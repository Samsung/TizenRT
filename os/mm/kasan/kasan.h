/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __KASAN_KASAN_H
#define __KASAN_KASAN_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sys/types.h>
#include <debug.h>		/* for lldbg func */

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

#ifdef CONFIG_KASAN

extern unsigned long datastart;
extern unsigned long dataend;
extern unsigned long bssstart;
extern unsigned long bssend;
extern unsigned long idlestackstart;
extern unsigned long idlestackend;
extern unsigned long heapstart;
extern unsigned long heapend;

extern unsigned long idlestackstart_shadow;
extern unsigned long idlestackend_shadow;

/**
 * @brief
 *  Prefix for output strings
 */
#define KASAN_PREFIX "KASan: "

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Inline Functions
 ****************************************************************************/

/**
 * @brief
 *  Map memory address to related shadow address.
 *
 * @param addr memory address
 *
 * @return
 *  Shadow memory address
 *
 */
static inline unsigned long mem_to_shadow(unsigned long addr)
{
	return (addr >> KASAN_SHADOW_SCALE_SHIFT)
		+ KASAN_SHADOW_OFFSET;
}

/**
 * @brief
 *  Map shadow address to related memory address.
 *
 * @param shadow_addr shadow memory address
 *
 * @return
 *  Memory address
 *
 */
static inline unsigned long shadow_to_mem(unsigned long shadow_addr)
{
	return (shadow_addr - KASAN_SHADOW_OFFSET)
		<< KASAN_SHADOW_SCALE_SHIFT;
}

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/**
 * @brief
 *  Return shadow value associated with address
 *
 * @param addr memory address
 *
 * @return
 *  Shadow value
 *
 */
signed char kasan_shadow_val(unsigned long addr);

/**
 * @brief
 *  Print KASan report prologue and disables IRQ.
 *
 * @param flags variable for saving irq flags
 *
 * @return
 *   None
 *
 */
void kasan_report_prologue(unsigned long addr, const char *err,
			   irqstate_t *flags);

/**
 * @brief
 *  Print KASan report epilogue and enables IRQ.
 *
 * @param flags irq flags saved in prologue function
 *
 * @return
 *   None
 *
 */
void kasan_report_epilogue(irqstate_t *flags);

/**
 * @brief
 *  Print shadow memory region related to memory address
 *  with some scope around
 *
 * @param addr memory address
 *
 * @return
 *   None
 *
 */
void kasan_print_shadow_scope(unsigned long addr);

/**
 * @brief
 *  Print KASan error report.
 *
 * @param addr address of memory region with redzone
 * @param size size of memory region
 * @param write type of access (true for write and false for read)
 * @param ret_ip address of the instruction which caused the error
 *
 * @return
 *  None
 *
 */
void kasan_report(unsigned long addr, size_t size, bool write,
		  unsigned long ret_ip);

#endif  /* CONFIG_KASAN */
#endif	/* __KASAN_KASAN_H */
