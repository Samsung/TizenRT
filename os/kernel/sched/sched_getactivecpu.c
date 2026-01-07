/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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
 * kernel/sched/sched_getactivecpu.c
 *
 * Description:
 *   This file implements the CPU control's fetch CPU mask function 
 *	 which returns CPU (each bit represents active(1)/inactive(0) cpu) 
 *	 bit mask.
 *
 ****************************************************************************/
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include "sched/sched.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sched_getactivecpu
 *
 * Description:
 *   Request to fetch a CPU mask indicating current state of all CPUs.
 *
 * Input Parameters:
 *   void
 *
 * Returned Value:
 *   CPU mask
 *
 * Preconditions(Checks):
 *	 None
 ****************************************************************************/

cpu_set_t sched_getactivecpu(void)
{
	return g_active_cpus_mask;
}
