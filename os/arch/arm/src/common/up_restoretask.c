/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>
#include <sched.h>
#ifdef CONFIG_SUPPORT_COMMON_BINARY
#include <stdint.h>
#endif
#ifdef CONFIG_TASK_MONITOR
#include <stdbool.h>
#endif
#ifdef CONFIG_TASK_SCHED_HISTORY
#include <tinyara/debug/sysdbg.h>
#endif
#ifdef CONFIG_ARMV8M_TRUSTZONE
#include <tinyara/tz_context.h>
#endif
#ifdef CONFIG_ARM_MPU
#include <tinyara/mpu.h>
#include "mpu.h"
#endif
#include <tinyara/arch.h>

#include "up_internal.h"
#include "sched/sched.h"

/****************************************************************************
 * Private Data
 ****************************************************************************/
#ifdef CONFIG_SUPPORT_COMMON_BINARY
extern uint32_t *g_umm_app_id;
#endif

/************************************************************************************
 * Name: up_restoretask
 *
 * Description:
 *   Restore and set data needed for context switching from tcb before context switching.
 *
 *   This function is called only from the TinyAra scheduling
 *   logic.
 *
 * Inputs:
 *   tcb: Refers to the tcb which will be scheduled.
 *     This tcb is at the head of the g_readytorun task lists.
 *
 ************************************************************************************/
void up_restoretask(struct tcb_s *tcb)
{
	if (tcb) {
#ifdef CONFIG_TASK_SCHED_HISTORY
		/* Save the task name which will be scheduled */
		save_task_scheduling_status(tcb);
#endif

		/* Restore the MPU registers in case we are switching to an application task */
#ifdef CONFIG_ARM_MPU
#ifdef CONFIG_APP_BINARY_SEPARATION
		/* Condition check : Update MPU registers only if this is not a kernel thread. */

		if ((tcb->flags & TCB_FLAG_TTYPE_MASK) != TCB_FLAG_TTYPE_KERNEL) {
			for (int i = 0; i < MPU_REG_NUMBER * MPU_NUM_REGIONS; i += MPU_REG_NUMBER) {
				up_mpu_set_register(&tcb->mpu_regs[i]);
			}
		}

#ifdef CONFIG_SUPPORT_COMMON_BINARY
		if (g_umm_app_id) {
			*g_umm_app_id = tcb->app_id;
		}
#endif
#endif
#ifdef CONFIG_MPU_STACK_OVERFLOW_PROTECTION
		up_mpu_set_register(tcb->stack_mpu_regs);
#endif
#endif

#ifdef CONFIG_TASK_MONITOR
		/* Update tcb active flag for monitoring. */
		tcb->is_active = true;
#endif

#ifdef CONFIG_ARMV8M_TRUSTZONE
		if (tcb->tz_context) {
			TZ_LoadContext_S(tcb->tz_context);
		}
#endif
	}
}
