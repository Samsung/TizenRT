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

///@file tinyara/mmu.h

#ifndef __INCLUDE_TINYARA_MMU_H
#define __INCLUDE_TINYARA_MMU_H

/********************************************************************************
 * Included Files
 ********************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>

/********************************************************************************
 * Pre-processor Definitions
 ********************************************************************************/

#define SMALL_PAGE_SIZE		(4096)	/* 4KB small page */
#define MMU_ALIGNMENT_BYTES	SMALL_PAGE_SIZE
#define MMU_ALIGN_UP(a)		(((a) + MMU_ALIGNMENT_BYTES - 1) & ~(MMU_ALIGNMENT_BYTES - 1))

#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
/* Separate three MPU regions (text, ro and rw) to optimize reloading time */
#define NUM_APP_REGIONS     3
#else
/* Just a MPU region for all of section data */
#define NUM_APP_REGIONS     1
#endif


/********************************************************************************
 * Public Function Prototypes
 ********************************************************************************/

#endif
