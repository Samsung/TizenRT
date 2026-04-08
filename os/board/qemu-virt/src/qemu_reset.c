/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * os/board/qemu-virt/src/qemu_reset.c
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

#include <tinyara/board.h>
#include <psci.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* PSCI v0.2 function ID for System Reset */
#define PSCI_SYSTEM_RESET 0x84000009

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/* Issue a PSCI call with given function ID */
static inline int32_t psci_call(uint32_t function_id)
{
	register uint32_t r0 __asm__("r0") = function_id;

	__asm__ volatile(
		"hvc #0"            /* Hypervisor Call */
		: "=r"(r0)          /* output */
		: "r"(r0)           /* input  */
		: "memory");

	return r0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int board_reset(int status)
{
	(void)status;                  /* Unused */

	psci_sys_reset();

	/* If PSCI reset returns (should never happen), spin forever */
	for (;;);

	/* Not reachable */
	return 0;
}