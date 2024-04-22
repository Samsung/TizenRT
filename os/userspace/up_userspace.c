/****************************************************************************
 * os/userspace/up_userspace.c
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/userspace.h>
#include <tinyara/init.h>
#include <tinyara/arch.h>

#if CONFIG_XIP_ELF
extern void * _stext_flash;
extern void * _sbss;
extern void * _ebss;
extern void * _sdata;
extern void * _edata;
extern void * _sdata_app;
extern void * _sapp_heap;
extern void * _eapp_heap;
extern void * _my_entry;
extern void * _sctors;
extern void * _ectors;
extern void * _eapp_flash;
extern void * _sapp_ram;
extern void * _eapp_ram;

extern int main(int argc, char **argv);
#endif

#if defined(CONFIG_BUILD_PROTECTED) && !defined(__KERNEL__)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/
const struct userspace_s userspace __attribute__((section(".userspace"))) = {
	/* Task/thread startup routines */
	.task_startup = task_startup,
#ifndef CONFIG_DISABLE_PTHREAD
	.pthread_startup = pthread_startup,
#endif
	/* Signal handler trampoline */
#ifndef CONFIG_DISABLE_SIGNALS
	.signal_handler = up_signal_handler,
#endif

#if CONFIG_XIP_ELF
	.text_start = &_stext_flash,
	.bss_start = &_sbss,
	.bss_end = &_ebss,
	.data_start_in_ram = &_sdata,
	.data_end_in_ram = &_edata,
	.data_start_in_flash = &_sdata_app,
	.heap_start = &_sapp_heap,
	.heap_end = &_eapp_heap,
	.sctors = &_sctors,
        .ectors = &_ectors,
	.flash_end = &_eapp_flash,
	.ram_start = &_sapp_ram,
	.ram_end = &_eapp_ram,
#ifndef __COMMON_BINARY__
	.entry = main,
#endif
#endif
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#endif							/* CONFIG_BUILD_PROTECTED && !__KERNEL__ */
