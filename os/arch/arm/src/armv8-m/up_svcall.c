/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/armv8-m/up_svcall.c
 *
 *   Copyright (C) 2009, 2011-2014 Gregory Nutt. All rights reserved.
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
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <debug.h>

#include <arch/irq.h>
#include <tinyara/sched.h>
#include <tinyara/userspace.h>

#ifdef CONFIG_LIB_SYSCALL
#include <syscall.h>
#endif

#ifdef CONFIG_ARMV8M_TRUSTZONE
#include <tinyara/tz_context.h>
#endif

#include "svcall.h"
#include "exc_return.h"
#include "up_internal.h"
#ifdef CONFIG_ARMV8M_MPU
#include "mpu.h"
#include <tinyara/mpu.h>
#endif
#define INDEX_ERROR (-1)
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/

/* Debug ********************************************************************/
/* Debug output from this file may interfere with context switching!  To get
 * debug output you must enabled the following in your TinyAra configuration:
 *
 * - CONFIG_DEBUG and CONFIG_DEBUG_SYSCALL (shows only syscalls)
 * - CONFIG_DEBUG and CONFIG_DEBUG_SVCALL  (shows everything)
 */

#if defined(CONFIG_DEBUG_SYSCALL) || defined(CONFIG_DEBUG_SVCALL)
#define svcdbg(format, ...) lldbg(format, ##__VA_ARGS__)
#else
#define svcdbg(...)
#endif

#ifdef CONFIG_BINMGR_RECOVERY
extern uint32_t g_assertpc;
#endif
#ifdef CONFIG_SUPPORT_COMMON_BINARY
extern uint32_t *g_umm_app_id;
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 *   Call the stub function corresponding to the system call.  NOTE the non-
 *   standard parameter passing:
 *
 *     R0 = SYS_ call number
 *     R1 = parm0
 *     R2 = parm1
 *     R3 = parm2
 *     R4 = parm3
 *     R5 = parm4
 *     R6 = parm5
 *
 *   The values of R4-R5 may be preserved in the proxy called by the user
 *   code if they are used (but otherwise will not be).
 *
 *   Register usage:
 *
 *     R0 - Need not be preserved.
 *     R1-R3 - Need to be preserved until the stub is called.  The values of
 *       R0 and R1 returned by the stub must be preserved.
 *     R4-R11 must be preserved to support the expectations of the user-space
 *       callee.  R4-R6 may have been preserved by the proxy, but don't know
 *       for sure.
 *     R12 - Need not be preserved
 *     R13 - (stack pointer)
 *     R14 - Need not be preserved
 *     R15 - (PC)
 *
 ****************************************************************************/

#ifdef CONFIG_LIB_SYSCALL
static void dispatch_syscall(void) naked_function;
static void dispatch_syscall(void)
{
	__asm__ __volatile__
	(
		" sub sp, sp, #16\n"	/* Create a stack frame to hold 3 parms + lr */
		" str r4, [sp, #0]\n"	/* Move parameter 4 (if any) into position */
		" str r5, [sp, #4]\n"	/* Move parameter 5 (if any) into position */
		" str r6, [sp, #8]\n"	/* Move parameter 6 (if any) into position */
		" str lr, [sp, #12]\n"	/* Save lr in the stack frame */
		" ldr ip, =g_stublookup\n"	/* R12=The base of the stub lookup table */
		" ldr ip, [ip, r0, lsl #2]\n"	/* R12=The address of the stub for this syscall */
		" blx ip\n"	/* Call the stub (modifies lr) */
		" ldr lr, [sp, #12]\n"	/* Restore lr */
		" add sp, sp, #16\n"	/* Destroy the stack frame */
		" mov r2, r0\n"	/* R2=Save return value in R2 */
		" mov r0, #3\n"	/* R0=SYS_syscall_return */
		" svc 0"	/* Return from the syscall */
	);
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_svcall
 *
 * Description:
 *   This is SVCall exception handler that performs context switching
 *
 ****************************************************************************/

int up_svcall(int irq, FAR void *context, FAR void *arg)
{
	uint32_t *regs = (uint32_t *)context;
	struct tcb_s *rtcb = sched_self();
	uint32_t cmd;

	DEBUGASSERT(regs && regs == current_regs);
	cmd = regs[REG_R0];

	/* The SVCall software interrupt is called with R0 = system call command
	 * and R1..R7 =  variable number of arguments depending on the system call.
	 */
#ifdef CONFIG_BINMGR_RECOVERY
	g_assertpc = regs[REG_R14];
#endif

#if defined(CONFIG_DEBUG_SYSCALL) || defined(CONFIG_DEBUG_SVCALL)
#ifndef CONFIG_DEBUG_SVCALL
	if (cmd > SYS_switch_context)
#endif
	{
		svcdbg("SVCALL Entry: regs: %p cmd: %d\n", regs, cmd);
		svcdbg("  R0: %08x %08x %08x %08x %08x %08x %08x %08x\n",
			   regs[REG_R0], regs[REG_R1], regs[REG_R2], regs[REG_R3],
			   regs[REG_R4], regs[REG_R5], regs[REG_R6], regs[REG_R7]);
		svcdbg("  R8: %08x %08x %08x %08x %08x %08x %08x %08x\n",
			   regs[REG_R8], regs[REG_R9], regs[REG_R10], regs[REG_R11],
			   regs[REG_R12], regs[REG_R13], regs[REG_R14], regs[REG_R15]);
#ifdef REG_EXC_RETURN
		svcdbg(" PSR: %08x EXC_RETURN: %08x\n",
			   regs[REG_XPSR], regs[REG_EXC_RETURN]);
#else
		svcdbg(" PSR: %08x\n", regs[REG_XPSR]);
#endif
	}
#endif

	/* Handle the SVCall according to the command in R0 */

	switch (cmd) {
	/* R0=SYS_save_context:  This is a save context command:
	 *
	 *   int up_saveusercontext(uint32_t *saveregs);
	 *
	 * At this point, the following values are saved in context:
	 *
	 *   R0 = SYS_save_context
	 *   R1 = saveregs
	 *
	 * In this case, we simply need to copy the current regsters to the
	 * save register space references in the saved R1 and return.
	 */

	case SYS_save_context: {
		DEBUGASSERT(regs[REG_R1] != 0);
		memcpy((uint32_t *)regs[REG_R1], regs, XCPTCONTEXT_SIZE);
#if defined(CONFIG_ARCH_FPU) && !defined(CONFIG_ARM_CMNVECTOR)
		up_savefpu((uint32_t *)regs[REG_R1]);
#endif
	}
	break;

	/* R0=SYS_restore_context:  This a restore context command:
	 *
	 *   void up_fullcontextrestore(uint32_t *restoreregs) noreturn_function;
	 *
	 * At this point, the following values are saved in context:
	 *
	 *   R0 = SYS_restore_context
	 *   R1 = restoreregs
	 *
	 * In this case, we simply need to set current_regs to restore register
	 * area referenced in the saved R1. context == current_regs is the normal
	 * exception return.  By setting current_regs = context[R1], we force
	 * the return to the saved context referenced in R1.
	 */

	case SYS_restore_context: {
		DEBUGASSERT(regs[REG_R1] != 0);
		current_regs = (uint32_t *)regs[REG_R1];

		/* Restore the MPU registers in case we are switching to an application task */
#if (defined(CONFIG_ARMV8M_MPU) && defined(CONFIG_APP_BINARY_SEPARATION))
		/* Condition check : Update MPU registers only if this is not a kernel thread. */
		if ((rtcb->flags & TCB_FLAG_TTYPE_MASK) != TCB_FLAG_TTYPE_KERNEL) {
			for (int i = 0; i < MPU_REG_NUMBER * MPU_NUM_REGIONS; i += MPU_REG_NUMBER) {
				up_mpu_set_register(&rtcb->mpu_regs[i]);
			}
		}
#ifdef CONFIG_MPU_STACK_OVERFLOW_PROTECTION
		up_mpu_set_register(rtcb->stack_mpu_regs);
#endif
#endif
#ifdef CONFIG_SUPPORT_COMMON_BINARY
			if (g_umm_app_id) {
				*g_umm_app_id = rtcb->app_id;
			}
#endif

#ifdef CONFIG_TASK_MONITOR
		/* Update tcb active flag for monitoring. */
		rtcb->is_active = true;
#endif
	}
	break;

	/* R0=SYS_switch_context:  This a switch context command:
	 *
	 *   void up_switchcontext(uint32_t *saveregs, uint32_t *restoreregs);
	 *
	 * At this point, the following values are saved in context:
	 *
	 *   R0 = SYS_switch_context
	 *   R1 = saveregs
	 *   R2 = restoreregs
	 *
	 * In this case, we do both: We save the context registers to the save
	 * register area reference by the saved contents of R1 and then set
	 * current_regs to to the save register area referenced by the saved
	 * contents of R2.
	 */

	case SYS_switch_context: {
		DEBUGASSERT(regs[REG_R1] != 0 && regs[REG_R2] != 0);
		memcpy((uint32_t *)regs[REG_R1], regs, XCPTCONTEXT_SIZE);
#if defined(CONFIG_ARCH_FPU) && !defined(CONFIG_ARM_CMNVECTOR)
		up_savefpu((uint32_t *)regs[REG_R1]);
#endif
		current_regs = (uint32_t *)regs[REG_R2];

		/* Restore the MPU registers in case we are switching to an application task */
#if (defined(CONFIG_ARMV8M_MPU) && defined(CONFIG_APP_BINARY_SEPARATION))
		/* Condition check : Update MPU registers only if this is not a kernel thread. */
		if ((rtcb->flags & TCB_FLAG_TTYPE_MASK) != TCB_FLAG_TTYPE_KERNEL) {
			for (int i = 0; i < MPU_REG_NUMBER * MPU_NUM_REGIONS; i += MPU_REG_NUMBER) {
				up_mpu_set_register(&rtcb->mpu_regs[i]);
			}
		}
#ifdef CONFIG_MPU_STACK_OVERFLOW_PROTECTION
		up_mpu_set_register(rtcb->stack_mpu_regs);
#endif
#endif
#ifdef CONFIG_SUPPORT_COMMON_BINARY
			if (g_umm_app_id) {
				*g_umm_app_id = rtcb->app_id;
			}
#endif

#ifdef CONFIG_TASK_MONITOR
		/* Update tcb active flag for monitoring. */
		rtcb->is_active = true;
#endif
#ifdef CONFIG_ARMV8M_TRUSTZONE
		if (rtcb->tz_context)
			TZ_LoadContext_S(rtcb->tz_context);
#endif
	}
	break;

		/* R0=SYS_syscall_return:  This a syscall return command:
		 *
		 *   void up_syscall_return(void);
		 *
		 * At this point, the following values are saved in context:
		 *
		 *   R0 = SYS_syscall_return
		 *
		 * We need to restore the saved return address and return in
		 * unprivileged thread mode.
		 */

#ifdef CONFIG_LIB_SYSCALL
	case SYS_syscall_return: {
		int index = (int)rtcb->xcp.nsyscalls - 1;

		/* Make sure that there is a saved syscall return address. */

		DEBUGASSERT(index >= 0);
		DEBUGASSERT(index < CONFIG_SYS_NNEST);
		if (index < 0 || index >= CONFIG_SYS_NNEST) {
			return INDEX_ERROR;
		}

		/* Setup to return to the saved syscall return address in
		 * the original mode.
		 */

		regs[REG_PC] = rtcb->xcp.syscall[index].sysreturn;
#if defined(CONFIG_BUILD_PROTECTED)
		regs[REG_EXC_RETURN] = rtcb->xcp.syscall[index].excreturn;
#endif
		rtcb->xcp.nsyscalls = index;

		/* The return value must be in R0-R1.  dispatch_syscall() temporarily
		 * moved the value for R0 into R2.
		 */

		regs[REG_R0] = regs[REG_R2];
	}
	break;
#endif

		/* R0=SYS_task_start:  This a user task start
		 *
		 *   void up_task_start(main_t taskentry, int argc, FAR char *argv[]) noreturn_function;
		 *
		 * At this point, the following values are saved in context:
		 *
		 *   R0 = SYS_task_start
		 *   R1 = taskentry
		 *   R2 = argc
		 *   R3 = argv
		 */

#ifdef CONFIG_BUILD_PROTECTED
	case SYS_task_start: {
		/* Set up to return to the user-space task start-up function in
		 * unprivileged mode.
		 */
		DEBUGASSERT(rtcb->uspace);

		/* While starting loadable apps, we cannot go through the
		* USERSPACE->task_startup method. Instead we pick the PC value
		* from the app's userspace object stored in its tcb.
		*
		* Here, we check if this task is a loadable app (non-zero uspace)
		*/
		regs[REG_PC] = (uint32_t)((struct userspace_s *)(rtcb->uspace))->task_startup;

		regs[REG_EXC_RETURN] = EXC_RETURN_UNPRIVTHR;

		/* Change the parameter ordering to match the expectation of struct
		 * userpace_s task_startup:
		 */

		regs[REG_R0] = regs[REG_R1];	/* Task entry */
		regs[REG_R1] = regs[REG_R2];	/* argc */
		regs[REG_R2] = regs[REG_R3];	/* argv */

	}
	break;
#endif

		/* R0=SYS_pthread_start:  This a user pthread start
		 *
		 *   void up_pthread_start(pthread_startroutine_t entrypt, pthread_addr_t arg) noreturn_function;
		 *
		 * At this point, the following values are saved in context:
		 *
		 *   R0 = SYS_pthread_start
		 *   R1 = entrypt
		 *   R2 = arg
		 */

#if defined(CONFIG_BUILD_PROTECTED) && !defined(CONFIG_DISABLE_PTHREAD)
	case SYS_pthread_start: {
		/* Set up to return to the user-space pthread start-up function in
		 * unprivileged mode.
		 */
		DEBUGASSERT(rtcb->uspace);

		/* While starting loadable apps, we cannot go through the
		* USERSPACE->task_startup method. Instead we pick the PC value
		* from the app's userspace object stored in its tcb.
		*
		* Here, we check if this task is a loadable app (non-zero uspace)
		*/
		regs[REG_PC] = (uint32_t)((struct userspace_s *)(rtcb->uspace))->pthread_startup;

		regs[REG_EXC_RETURN] = EXC_RETURN_UNPRIVTHR;

		/* Change the parameter ordering to match the expectation of struct
		 * userpace_s pthread_startup:
		 */

		regs[REG_R0] = regs[REG_R1];	/* pthread entry */
		regs[REG_R1] = regs[REG_R2];	/* arg */
	}
	break;
#endif

		/* R0=SYS_signal_handler:  This a user signal handler callback
		 *
		 * void signal_handler(_sa_sigaction_t sighand, int signo,
		 *                     FAR siginfo_t *info, FAR void *ucontext);
		 *
		 * At this point, the following values are saved in context:
		 *
		 *   R0 = SYS_signal_handler
		 *   R1 = sighand
		 *   R2 = signo
		 *   R3 = info
		 *        ucontext (on the stack)
		 */

#if defined(CONFIG_BUILD_PROTECTED) && !defined(CONFIG_DISABLE_SIGNALS)
	case SYS_signal_handler: {

		/* Remember the caller's return address */

		DEBUGASSERT(rtcb->xcp.sigreturn == 0);
		DEBUGASSERT(rtcb->uspace);
		rtcb->xcp.sigreturn = regs[REG_PC];

		/* Set up to return to the user-space pthread start-up function in
		 * unprivileged mode.
		 */

		/* While starting loadable apps, we cannot go through the
		* USERSPACE->task_startup method. Instead we pick the PC value
		* from the app's userspace object stored in its tcb.
		*
		* Here, we check if this task is a loadable app (non-zero uspace)
		*/
		regs[REG_PC] = (uint32_t)((struct userspace_s *)(rtcb->uspace))->signal_handler;

		regs[REG_EXC_RETURN] = EXC_RETURN_UNPRIVTHR;

		/* Change the parameter ordering to match the expectation of struct
		 * userpace_s signal_handler.
		 */

		regs[REG_R0] = regs[REG_R1];	/* sighand */
		regs[REG_R1] = regs[REG_R2];	/* signal */
		regs[REG_R2] = regs[REG_R3];	/* info */

		/* The last parameter, ucontext, is trickier.  The ucontext
		 * parameter will reside at an offset of 4 from the stack pointer.
		 */

		regs[REG_R3] = *(uint32_t *)(regs[REG_SP] + 4);
	}
	break;
#endif

		/* R0=SYS_signal_handler_return:  This a user signal handler callback
		 *
		 *   void signal_handler_return(void);
		 *
		 * At this point, the following values are saved in context:
		 *
		 *   R0 = SYS_signal_handler_return
		 */

#if defined(CONFIG_BUILD_PROTECTED) && !defined(CONFIG_DISABLE_SIGNALS)
	case SYS_signal_handler_return: {

		/* Set up to return to the kernel-mode signal dispatching logic. */

		DEBUGASSERT(rtcb->xcp.sigreturn != 0);

		regs[REG_PC] = rtcb->xcp.sigreturn;
		regs[REG_EXC_RETURN] = EXC_RETURN_PRIVTHR;
		rtcb->xcp.sigreturn = 0;
	}
	break;
#endif

#ifdef CONFIG_ARMV8M_TRUSTZONE
	case SYS_init_securecontext: {
		/* Initialize the secure context management system. */
		TZ_InitContextSystem_S();
	}
	break;

	case SYS_allocate_securecontext: {
		uint32_t ulR1;
		ulR1 = regs[REG_R1];

		/* Allocate and load a context for the secure task. */
		rtcb->tz_context = TZ_AllocModuleContext_S(ulR1);

		ASSERT(rtcb->tz_context != 0);
		TZ_LoadContext_S(rtcb->tz_context);
	}
	break;

	case SYS_free_securecontext: {
		/* Free the secure context. */
		TZ_FreeModuleContext_S(rtcb->tz_context);
		rtcb->tz_context = 0;
	}
	break;
#endif
	/* This is not an architecture-specific system call.  If TinyAra is built
	 * as a standalone kernel with a system call interface, then all of the
	 * additional system calls must be handled as in the default case.
	 */

	default: {
#ifdef CONFIG_LIB_SYSCALL
		int index = rtcb->xcp.nsyscalls;

		/* Verify that the SYS call number is within range */

		DEBUGASSERT(cmd >= CONFIG_SYS_RESERVED && cmd < SYS_maxsyscall);

		/* Make sure that there is a no saved syscall return address.  We
		 * cannot yet handle nested system calls.
		 */

		DEBUGASSERT(index < CONFIG_SYS_NNEST);
		if (index >= CONFIG_SYS_NNEST) {
			return INDEX_ERROR;
		}

		/* Setup to return to dispatch_syscall in privileged mode. */

		rtcb->xcp.syscall[index].sysreturn = regs[REG_PC];
#if defined(CONFIG_BUILD_PROTECTED)
		rtcb->xcp.syscall[index].excreturn = regs[REG_EXC_RETURN];
#endif
		rtcb->xcp.nsyscalls = index + 1;

		regs[REG_PC] = (uint32_t)dispatch_syscall;
#if defined(CONFIG_BUILD_PROTECTED)
		regs[REG_EXC_RETURN] = EXC_RETURN_PRIVTHR;
#endif

		/* Offset R0 to account for the reserved values */

		regs[REG_R0] -= CONFIG_SYS_RESERVED;
#else
		slldbg("ERROR: Bad SYS call: %d\n", regs[REG_R0]);
#endif
	}
	break;
	}

	/* Report what happened.  That might difficult in the case of a context switch */

#if defined(CONFIG_DEBUG_SYSCALL) || defined(CONFIG_DEBUG_SVCALL)
#ifndef CONFIG_DEBUG_SVCALL
	if (cmd > SYS_switch_context)
#else
	if (regs != current_regs)
#endif
	{
		svcdbg("SVCall Return:\n");
		svcdbg("  R0: %08x %08x %08x %08x %08x %08x %08x %08x\n",
			   current_regs[REG_R0], current_regs[REG_R1],
			   current_regs[REG_R2], current_regs[REG_R3],
			   current_regs[REG_R4], current_regs[REG_R5],
			   current_regs[REG_R6], current_regs[REG_R7]);
		svcdbg("  R8: %08x %08x %08x %08x %08x %08x %08x %08x\n",
			   current_regs[REG_R8], current_regs[REG_R9],
			   current_regs[REG_R10], current_regs[REG_R11],
			   current_regs[REG_R12], current_regs[REG_R13],
			   current_regs[REG_R14], current_regs[REG_R15]);
#ifdef REG_EXC_RETURN
		svcdbg(" PSR: %08x EXC_RETURN: %08x\n",
			   current_regs[REG_XPSR], current_regs[REG_EXC_RETURN]);
#else
		svcdbg(" PSR: %08x\n", current_regs[REG_XPSR]);
#endif
	}
#ifdef CONFIG_DEBUG_SVCALL
	else {
		svcdbg("SVCall Return: %d\n", regs[REG_R0]);
	}
#endif
#endif

	return OK;
}
