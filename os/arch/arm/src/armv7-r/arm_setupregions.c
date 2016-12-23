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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <sched.h>
#include <debug.h>

#include <tinyara/kmalloc.h>
#include <tinyara/arch.h>
#include <arch/board/board.h>
#include <arch/irq.h>

#include "up_arch.h"
#include "up_internal.h"
#include "mpu.h"

/****************************************************************************
 * Pre-processor Macros
 ****************************************************************************/
/* Configuration */

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Global Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_setup_regions
 *
 * Description:
 *   Allocate a stack for a new thread and setup up stack-related information
 *   in the TCB.
 *
 *   The following TCB fields must be initialized by this function:
 *
 *   - adj_stack_size: Stack size after adjustment for hardware, processor,
 *     etc.  This value is retained only for debug purposes.
 *   - stack_alloc_ptr: Pointer to allocated stack
 *   - adj_stack_ptr: Adjusted stack_alloc_ptr for HW.  The initial value of
 *     the stack pointer.
 *
 * Inputs:
 *   - tcb: The TCB of new task
 *   - stack_size:  The requested stack size.  At least this much
 *     must be allocated.
 *   - ttype:  The thread type.  This may be one of following (defined in
 *     include/tinyara/sched.h):
 *
 *       TCB_FLAG_TTYPE_TASK     Normal user task
 *       TCB_FLAG_TTYPE_PTHREAD  User pthread
 *       TCB_FLAG_TTYPE_KERNEL   Kernel thread
 *
 *     This thread type is normally available in the flags field of the TCB,
 *     however, there are certain contexts where the TCB may not be fully
 *     initialized when up_create_stack is called.
 *
 *     If either CONFIG_BUILD_PROTECTED or CONFIG_BUILD_KERNEL are defined,
 *     then this thread type may affect how the stack is allocated.  For
 *     example, kernel thread stacks should be allocated from protected
 *     kernel memory.  Stacks for user tasks and threads must come from
 *     memory that is accessible to user code.
 *
 ****************************************************************************/
FAR int up_setup_regions(FAR struct tcb_s *tcb, uint8_t ttype)
{
	FAR struct pthread_tcb_s *ptcb;
	uint32_t idx;
	uint32_t *regs = tcb->xcp.regs;
	uint32_t offset = 0;

	/* Allocate regions for the user requested configurations */
	for (idx = 0; idx < MPU_TOTAL_USER_REG; idx++) {
		offset = REG_USR_CFG1 + idx * 4;
		regs[offset] = idx + 2;	/* start of user configuration till stack region */
		regs[offset + 1] = 0;
		regs[offset + 2] = 0;
		regs[offset + 3] = 0;
	}
	if (ttype == TCB_FLAG_TTYPE_KERNEL) {
		if (tcb->adj_stack_size >= 32) {
			regs[REG_RNUM_STK] = MPU_REG_TASK_STACK;
		}
		regs[REG_RBASE_STK] = ((uint32_t)tcb->stack_alloc_ptr & MPU_RBAR_ADDR_MASK);
		regs[REG_RSIZE_STK] = MPU_RASR_RSIZE_LOG2(mpu_log2regionceil(tcb->adj_stack_size)) | MPU_RASR_ENABLE;
		regs[REG_RATTR_STK] = MPU_RACR_B | MPU_RACR_TEX(5) | MPU_RACR_AP_RWNO | MPU_RACR_XN;
	} else if (ttype == TCB_FLAG_TTYPE_TASK) {
		if (tcb->adj_stack_size >= 32) {
			regs[REG_RNUM_STK] = MPU_REG_TASK_STACK;
		}
		regs[REG_RBASE_STK] = ((uint32_t)tcb->stack_alloc_ptr & MPU_RBAR_ADDR_MASK);
		regs[REG_RSIZE_STK] = MPU_RASR_RSIZE_LOG2(mpu_log2regionceil(tcb->adj_stack_size)) | MPU_RASR_ENABLE;
		regs[REG_RATTR_STK] = MPU_RACR_B | MPU_RACR_TEX(5) | MPU_RACR_AP_RWRW | MPU_RACR_XN;
	} else if (ttype == TCB_FLAG_TTYPE_PTHREAD) {
		ptcb = (FAR struct pthread_tcb_s *)tcb;
		struct pthread_region_s *regn = (struct pthread_region_s *)ptcb->region;
		/*
		 * Allocate region for the task stack, the stack is adjusted during allocation,
		 * so use that instead of the size requested by the user space
		 */
		if (tcb->adj_stack_size >= 32) {
			regs[REG_RNUM_STK] = MPU_REG_TASK_STACK;
			regs[REG_RBASE_STK] = ((uint32_t)tcb->stack_alloc_ptr & MPU_RBAR_ADDR_MASK);
			regs[REG_RSIZE_STK] = MPU_RASR_RSIZE_LOG2(mpu_log2regionceil(tcb->adj_stack_size)) | MPU_RASR_ENABLE;
			regs[REG_RATTR_STK] = MPU_RACR_B | MPU_RACR_TEX(5) | MPU_RACR_AP_RWRW | MPU_RACR_XN;
		}

		/* Allocate regions for the user requested configurations */
		for (idx = 0; idx < MPU_TOTAL_USER_REG - 1; idx++) {
			offset = REG_USR_CFG0 - idx * 4;
			if (regn[idx].size >= 32) {
				regs[offset] = (MPU_REG_USER_CONFIG0 - idx);
				regs[offset + 1] = ((uint32_t)regn[idx].address & MPU_RBAR_ADDR_MASK);
				regs[offset + 2] = MPU_RASR_RSIZE_LOG2(mpu_log2regionceil(regn[idx].size)) | MPU_RASR_ENABLE;
				regs[offset + 3] = MPU_RACR_B | MPU_RACR_TEX(5);
				regs[offset + 3] |= (regn[idx].attributes == 0) ? MPU_RACR_AP_RWRO : MPU_RACR_AP_RWRW;
			}
		}
	}
	return OK;
}
