/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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

#ifndef __INCLUDE_TINYARA_MEM_PROTECTTEST_H
#define __INCLUDE_TINYARA_MEM_PROTECTTEST_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/
#ifdef CONFIG_EXAMPLES_MEM_PROTECT_TEST
enum {
	MEM_PROTECTTEST_KERNEL_CODE,
	MEM_PROTECTTEST_KERNEL_DATA,
	MEM_PROTECTTEST_APP_ADDR,
};
#endif

struct mem_protecttest_arg_s {
	uint32_t type;
	volatile uint32_t *addr;
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#endif							/* __INCLUDE_TINYARA_MEM_PROTECTTEST_H */
