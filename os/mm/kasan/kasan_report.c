/****************************************************************************
 *
 * Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/mm/kasan.h>
#include <tinyara/mm/kasan_error.h>
#include <tinyara/mm/mm.h>
#include <stdbool.h>
#include <debug.h>
#include <stdio.h>

#include "kasan.h"

#ifdef CONFIG_KASAN

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/**
 * @brief
 *  Determines how many shadow bytes will be printed before and after error address.
 *
 */

#ifdef CONFIG_KASAN_SHADOW_SCOPE
#define SHADOW_SCOPE CONFIG_KASAN_SHADOW_SCOPE
#else
#define SHADOW_SCOPE 8
#endif

#define ERR_BUF_SIZE 128

/****************************************************************************
 * Type Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/**
 * @brief
 *  String representation of redzones
 */
static char *redzone[] = {
	"unknown",
	"heap",
	"heap (use-after-free)",
	"global variable",
	"stack",
	"stack (use-after-scope)"
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/**
 * @brief
 *  Return string which describes redzone type.
 *
 * @param shadow_addr addr to shadow reigon with redzone
 *
 * @return
 *  String which describes redzone type.
 *
 */
static char *redzone_type(unsigned long shadow_addr, bool write)
{
	unsigned char shadow_val;
	int ind;

	if ((*(unsigned char *)shadow_addr) < 8) {
		shadow_addr++;
	}

	shadow_val = *(unsigned char *)shadow_addr;

	switch (shadow_val) {
	case KASAN_REDZONE:
		kasan_set_error(write, KASAN_HEAP_WRITE, KASAN_HEAP_READ);
		ind = 1;
		break;

	case KASAN_QUARANTINE:
		kasan_set_error(write, KASAN_HEAP_UAF_WRITE, KASAN_HEAP_UAF_READ);
		ind = 2;
		break;

	case KASAN_GLOBAL_REDZONE:
		kasan_set_error(write, KASAN_GLOBAL_WRITE, KASAN_GLOBAL_READ);
		ind = 3;
		break;

	case KASAN_STACK_LEFT:
	case KASAN_STACK_MID:
	case KASAN_STACK_RIGHT:
	case KASAN_STACK_PARTIAL:
		kasan_set_error(write, KASAN_STACK_WRITE, KASAN_STACK_READ);
		ind = 4;
		break;

	case KASAN_USE_AFTER_SCOPE:
		kasan_set_error(write, KASAN_STACK_UAS_WRITE, KASAN_STACK_UAS_READ);
		ind = 5;
		break;

	default:
		ind = 0;
		kasan_set_error(write, KASAN_UNKNOWN, KASAN_UNKNOWN);
	}

	return redzone[ind];
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

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
			   irqstate_t *flags)
{
	*flags = irqsave();

	lldbg("~~~~~~~~~~~~~~< KASAN REPORT >~~~~~~~~~~~~~~\n");
	lldbg("error: %s\n", err);
	lldbg("addr: 0x%lx\n", addr);
	lldbg("aligned: %d\n", MM_IS_ALIGNED(addr, KASAN_SHADOW_SCALE_SIZE));
}

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
void kasan_report_epilogue(irqstate_t *flags)
{
#ifdef CONFIG_ARCH_STACKDUMP
	lldbg("stack trace:\n");
	dump_stack();
#endif

	lldbg("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	irqrestore(*flags);
}

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
void kasan_print_shadow_scope(unsigned long addr)
{
	unsigned long i, shadow_addr, shadow_start, shadow_end;

	if (IS_SHAD_ADDR(addr))
		shadow_addr = addr;
	else
		shadow_addr = mem_to_shadow(addr);

	shadow_start = shadow_addr - SHADOW_SCOPE;
	if (shadow_start < KASAN_SHADOW_START)
		shadow_start = KASAN_SHADOW_START;

	shadow_end = shadow_addr + SHADOW_SCOPE + 1;
	if (shadow_end > KASAN_SHADOW_END)
		shadow_end = KASAN_SHADOW_END;

	lldbg("shadow region (0x%lx - 0x%lx):\n", shadow_start, shadow_end - 1);

	for (i = shadow_start; i < shadow_end; ++i)
		lldbg("%s 0x%02lX\n",
			i == shadow_addr ? ">" : " ", *(unsigned char *)i);
}

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
		  unsigned long ret_ip)
{
	irqstate_t flags;
	char err[ERR_BUF_SIZE];
	unsigned long shadow_addr;

	shadow_addr = mem_to_shadow(addr);

	kasan_unpoison((unsigned long)&err, ERR_BUF_SIZE);

	snprintf(err, ERR_BUF_SIZE, "%s %s",
		 write ? "write to" : "read from",
		 redzone_type(shadow_addr, write));

	kasan_report_prologue(addr, err, &flags);

	lldbg("size: %d\n", size);
	lldbg("ret_ip: 0x%lx\n", ret_ip);
	kasan_print_shadow_scope(shadow_addr);

	kasan_report_epilogue(&flags);
}

/****************************************************************************
 * Following doxygen comments relates to funcitons defined through DEFINE_ASAN_REPORT_LOAD_STORE macro.
 * Generate comments inside macro isn't possible because C preprocessor removes comments during macro expansion.
 ****************************************************************************/

/**
 * @fn void __asan_report_load1_noabort(unsigned long addr)
 *
 * @brief
 *  Print KASan report about incorrect load for region with size 1 (instrumented by compiler).
 *
 * @param addr address of memory region with redzone
 *
 * @return
 *  None
 *
 */

/**
 * @fn void __asan_report_load2_noabort(unsigned long addr)
 *
 * @brief
 *  Print KASan report about incorrect load for region with size 2 (instrumented by compiler).
 *
 * @param addr address of memory region with redzone
 *
 * @return
 *  None
 *
 */

/**
 * @fn void __asan_report_load4_noabort(unsigned long addr)
 *
 * @brief
 *  Print KASan report about incorrect load for region with size 4 (instrumented by compiler).
 *
 * @param addr address of memory region with redzone
 *
 * @return
 *  None
 *
 */

/**
 * @fn void __asan_report_load8_noabort(unsigned long addr)
 *
 * @brief
 *  Print KASan report about incorrect load for region with size 8 (instrumented by compiler).
 *
 * @param addr address of memory region with redzone
 *
 * @return
 *  None
 *
 */

/**
 * @fn void __asan_report_load16_noabort(unsigned long addr)
 *
 * @brief
 *  Print KASan report about incorrect load for region with size 16 (instrumented by compiler).
 *
 * @param addr address of memory region with redzone
 *
 * @return
 *  None
 *
 */

/**
 * @fn void __asan_report_store1_noabort(unsigned long addr)
 *
 * @brief
 *  Print KASan report about incorrect store for region with size 1 (instrumented by compiler).
 *
 * @param addr address of memory region with redzone
 *
 * @return
 *  None
 *
 */

/**
 * @fn void __asan_report_store2_noabort(unsigned long addr)
 *
 * @brief
 *  Print KASan report about incorrect store for region with size 2 (instrumented by compiler).
 *
 * @param addr address of memory region with redzone
 *
 * @return
 *  None
 *
 */

/**
 * @fn void __asan_report_store4_noabort(unsigned long addr)
 *
 * @brief
 *  Print KASan report about incorrect store for region with size 4 (instrumented by compiler).
 *
 * @param addr address of memory region with redzone
 *
 * @return
 *  None
 *
 */

/**
 * @fn void __asan_report_store8_noabort(unsigned long addr)
 *
 * @brief
 *  Print KASan report about incorrect store for region with size 8 (instrumented by compiler).
 *
 * @param addr address of memory region with redzone
 *
 * @return
 *  None
 *
 */

/**
 * @fn void __asan_report_store16_noabort(unsigned long addr)
 *
 * @brief
 *  Print KASan report about incorrect store for region with size 16 (instrumented by compiler).
 *
 * @param addr address of memory region with redzone
 *
 * @return
 *  None
 *
 */

/**
 * @brief
 *  Defined __asan_report_load*_noabort and __asan_report_store*_noabort
 *  functions for compiler instrumentation.
 */
#define DEFINE_ASAN_REPORT_LOAD_STORE(size)				\
	void __asan_report_load##size##_noabort(unsigned long addr)	\
	{								\
		kasan_report(addr, size, false, (unsigned long)RET_IP);	\
	}								\
									\
	void __asan_report_store##size##_noabort(unsigned long addr)	\
	{								\
		kasan_report(addr, size, true, (unsigned long)RET_IP);	\
	}

DEFINE_ASAN_REPORT_LOAD_STORE(1);
DEFINE_ASAN_REPORT_LOAD_STORE(2);
DEFINE_ASAN_REPORT_LOAD_STORE(4);
DEFINE_ASAN_REPORT_LOAD_STORE(8);
DEFINE_ASAN_REPORT_LOAD_STORE(16);

/**
 * @brief
 *  Print KASan error report (instrumented by compiler).
 *
 * @param addr address of memory region with redzone
 * @param size size of memory region
 *
 * @return
 *  None
 *
 */
void __asan_report_load_n_noabort(unsigned long addr, size_t size)
{
	kasan_report(addr, size, false, (unsigned long)RET_IP);
}

/**
 * @brief
 *  Print KASan error report (instrumented by compiler).
 *
 * @param addr address of memory region with redzone
 * @param size size of memory region
 *
 * @return
 *  None
 *
 */
void __asan_report_store_n_noabort(unsigned long addr, size_t size)
{
	kasan_report(addr, size, true, (unsigned long)RET_IP);
}

#endif	/* CONFIG_KASAN */
