/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
 * libs/libc/machine/arm/gnu_unwind_find_exidx.c
 *
 * SPDX-License-Identifier: Apache-2.0
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
#include <unwind.h>
#include <stdint.h>

/****************************************************************************
 * Private Data
 ****************************************************************************/

struct eit_entry_s {
	unsigned long fnoffset;
	unsigned long content;
};

typedef struct eit_entry_s eit_entry_t;

extern uint32_t *__exidx_start;
extern uint32_t *__exidx_end;
extern uint32_t *_eronly;

/* array to store exidx start and end for app binaries */
eit_entry_t *__exidx_start_elf[CONFIG_NUM_APPS];
eit_entry_t *__exidx_end_elf[CONFIG_NUM_APPS];

/* array to store text start and end for app binaries */
uint32_t text_start_elf[CONFIG_NUM_APPS];
uint32_t text_end_elf[CONFIG_NUM_APPS];

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int up_init_exidx(_Unwind_Ptr start, _Unwind_Ptr end, void * text_start, void * text_end, int bin_idx)
{
	__exidx_start_elf[bin_idx - 1] = (eit_entry_t *)start;
	__exidx_end_elf[bin_idx - 1]   = (eit_entry_t *)end;

	text_start_elf[bin_idx - 1] = (uint32_t)text_start;
	text_end_elf[bin_idx - 1] = (uint32_t)text_end;

	return 0;
}

/****************************************************************************
 * Name:  __gnu_Unwind_Find_exidx
 *
 * Description:
 *    This function is called (if exists) by the gcc generated unwind
 *    run-time in order to retrieve an alternative .ARM.exidx Exception
 *    index section.
 *
 ****************************************************************************/
volatile _Unwind_Ptr __gnu_Unwind_Find_exidx(_Unwind_Ptr return_address, int *nrecp)
{
	eit_entry_t * __exidx_start_common = (eit_entry_t *)(&__exidx_start);
	eit_entry_t * __exidx_end_common = (eit_entry_t *)(&__exidx_end);

	for (int i = 0; i < CONFIG_NUM_APPS; i++) {
		if (text_start_elf[i] <= (uint32_t)return_address && (uint32_t)return_address <= text_end_elf[i]) {
			*nrecp = __exidx_end_elf[i] - __exidx_start_elf[i];
			return (_Unwind_Ptr)__exidx_start_elf[i];
		}
	}

	*nrecp = __exidx_end_common - __exidx_start_common;
	return (_Unwind_Ptr)&__exidx_start;
}
