/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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
#ifndef __BINFMT_ARCH_APIS_H__
#define __BINFMT_ARCH_APIS_H__
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <errno.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/kmalloc.h>
#include <tinyara/mm/mm.h>
#ifdef CONFIG_ARM_MPU
#include <tinyara/mpu.h>
#endif
#ifdef CONFIG_ARCH_USE_MMU
#include <tinyara/mmu.h>
#endif
#include <tinyara/binfmt/binfmt.h>

/****************************************************************************
* Pre-processor Definitions
 ****************************************************************************/
#ifndef CONFIG_HEAP_INDEX_LOADED_APP
#define CONFIG_HEAP_INDEX_LOADED_APP 0
#endif

#if CONFIG_HEAP_INDEX_LOADED_APP >= CONFIG_KMM_NHEAPS
#error "Heap index for loading apps must be less than total number of heaps"
#endif

/****************************************************************************
 * Public functions
 ****************************************************************************/
#ifdef CONFIG_ARCH_USE_MMU
void binfmt_setup_app_pgtable(struct binary_s *binp);
#endif

void *binfmt_arch_allocate_section(uint32_t size);
uint32_t binfmt_arch_align_mem(uint32_t val);
void binfmt_arch_init_mem_protect(struct binary_s *binp);
void binfmt_arch_deinit_mem_protect(struct binary_s *binp);

#endif
