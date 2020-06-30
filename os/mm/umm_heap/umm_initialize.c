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
/************************************************************************
 * mm/umm_heap/umm_initialize.c
 *
 *   Copyright (C) 2013-2014 Gregory Nutt. All rights reserved.
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
 ************************************************************************/

/************************************************************************
 * Included Files
 ************************************************************************/

#include <tinyara/config.h>
#include <assert.h>
#include <tinyara/mm/mm.h>

/************************************************************************
 * Pre-processor definition
 ************************************************************************/

/************************************************************************
 * Private Types
 ************************************************************************/

/************************************************************************
 * Public Data
 ************************************************************************/
#ifdef CONFIG_SUPPORT_COMMON_BINARY
/* g_cur_app will contain the id of the currently executing application.
 * It will be used to index into the app_heap_table to get the heap pointer
 * for the currently executing application. This variable will be placed at
 * the first byte of the data section. The value contained in this variable
 * will be used only by the common binary. The cur_app will be updated by
 * the kernel during every context switch.
 */
uint32_t g_cur_app __attribute__((section(".curapp")));

/* The app_heap_table will contain pointers to the heap of each application.
 * These pointers will be populated by the loader at the time of loading the
 * application and creating its heap.
 */
struct mm_heap_s *g_app_heap_table[CONFIG_NUM_APPS + 1] __attribute__((section(".appheaptable")));
#endif

/************************************************************************
 * Private Functions
 ************************************************************************/

/************************************************************************
 * Public Functions
 ************************************************************************/

/************************************************************************
 * Name: umm_initialize
 *
 * Description:
 *   This is a simple wrapper for the mm_initialize() function.  This
 *   function will initialize the user heap.
 *
 *   CONFIG_BUILD_FLAT:
 *     There is only kernel mode "blob" containing both containing both
 *     kernel and application code.  There is only one heap that use is
 *     used by both the kernel and application logic.
 *
 *     In this configuration, this function is called early in os_start()
 *     to initialize the common heap.
 *
 *   CONFIG_BUILD_PROTECTED
 *     In this configuration, there are two "blobs", one containing
 *     protected kernel logic and one containing unprotected application
 *     logic.  Depending upon the setting of CONFIG_MM_KERNEL_HEAP there
 *     may be only a signal shared heap, much as with CONFIG_BUILD_FLAT.
 *     Or there may be separate protected/kernel and unprotected/user
 *     heaps.
 *
 *     In either case, this function is still called early in os_start()
 *     to initialize the user heap.
 *
 *   CONFIG_BUILD_KERNEL
 *     In this configuration there are multiple user heaps, one for each
 *     user process.  Furthermore, each heap is initially empty; memory
 *     is added to each heap dynamically via sbrk().  The heap data
 *     structure was set to zero when the address environment was created.
 *     Otherwise, the heap is uninitialized.
 *
 *     This function is not called at all.  Rather, this function is called
 *     when each user process is created before the first allocation is made.
 *
 * Parameters:
 *   heap_start - Address of the beginning of the (initial) memory region
 *   heap_size  - The size (in bytes) if the (initial) memory region.
 *
 * Return Value:
 *   None
 *
 ************************************************************************/

void umm_initialize(FAR void *heap_start, size_t heap_size)
{
	mm_initialize(BASE_HEAP, heap_start, heap_size);
}
