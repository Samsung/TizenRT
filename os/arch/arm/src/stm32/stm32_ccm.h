/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/common/stm32_ccm.h
 *
 *   Copyright (C) 2013 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *           Based on a prototype by Petteri Aimonen
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

#ifndef __ARCH_ARM_SRC_STM32_STM32_CCM_H
#define __ARCH_ARM_SRC_STM32_STM32_CCM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <tinyara/mm/mm.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/
/* Assume that we can support the CCM heap */

#define HAVE_CCM_HEAP 1

/* Only the STM32 F2, F3, and F4 have CCM memory */

#if defined(CONFIG_STM32_STM32F30XX)
#define CCM_START 0x10000000
#define CCM_END   0x10002000
#elif defined(CONFIG_STM32_STM32F20XX) || defined(CONFIG_STM32_STM32F40XX)
#define CCM_START 0x10000000
#define CCM_END   0x10010000
#else
#undef HAVE_CCM_HEAP
#endif

/* In order to use the CCM heap, it had to have been excluded from the main
 * heap.
 */

#ifndef CONFIG_STM32_CCMEXCLUDE
#undef HAVE_CCM_HEAP
#endif

/* Can we support the CCM heap? */

#ifdef HAVE_CCM_HEAP

/* ccm_initialize must be called early in initialization in order to
 * initialize the CCM heap.
 */

#define ccm_initialize() mm_initialize(&g_ccm_heap, (uintptr_t)CCM_START, CCM_END-CCM_START)

/* The ccm_addregion interface could be used if, for example, you want to
 * add some other memory region to the CCM heap.  I don't really know why
 * you might want to do that, but the functionality is essentially free.
 */

#define ccm_addregion(b, s) mm_addregion(&g_ccm_heap, b, s);

/* Then, once g_ccm_heap has been setup by ccm_initialize(), these memory
 * allocators can be used just like the standard memory allocators.
 */

#define ccm_malloc(s)      malloc_user_at(&g_ccm_heap, s)
#define ccm_zalloc(s)      zalloc_user_at(&g_ccm_heap, s)
#define ccm_calloc(n, s)   calloc_user_at(&g_ccm_heap, n, s)
#define ccm_free(p)        free_user_at(&g_ccm_heap, p)
#define ccm_realloc(p, s)  realloc_user_at(&g_ccm_heap, p, s)
#define ccm_memalign(a, s) memalign_user_at(&g_ccm_heap, a, s)

/****************************************************************************
 * Public Types
 ****************************************************************************/

#ifndef __ASSEMBLY__

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

EXTERN struct mm_heap_s g_ccm_heap;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __ASSEMBLY__ */
#endif							/* HAVE_CCM_HEAP */
#endif							/* __ARCH_ARM_SRC_STM32_STM32_CCM_H */
