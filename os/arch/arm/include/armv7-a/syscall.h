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
/****************************************************************************
 * arch/arm/include/syscall.h
 *
 *   Copyright (C) 2011 Gregory Nutt. All rights reserved.
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

/* This file should never be included directed but, rather, only indirectly
 * through include/syscall.h or include/sys/sycall.h
 */

#ifndef __ARCH_ARM_INCLUDE_ARMV7A_SYSCALL_H
#define __ARCH_ARM_INCLUDE_ARMV7A_SYSCALL_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

/* Include ARM architecture-specific syscall macros */
#include <tinyara/config.h>

#ifndef __ASSEMBLY__
#include <stdint.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define SYS_syscall 0x00

/* Configuration ********************************************************************/
/* This logic uses three system calls {0,1,2} for context switching and one for the
 * syscall return.  So a minimum of four syscall values must be reserved.  If
 * CONFIG_BUILD_PROTECTED is defined, then four more syscall values must be reserved.
 * If TRUSTZONE is enabled, then three more syscall values must be reserved.
 */

#ifdef CONFIG_LIB_SYSCALL

#ifndef CONFIG_SYS_RESERVED
#error "CONFIG_SYS_RESERVED must be defined to have the value"
#endif //CONFIG_SYS_RESERVED

#ifdef CONFIG_BUILD_PROTECTED

#ifdef CONFIG_ARMV7A_TRUSTZONE
#if CONFIG_SYS_RESERVED != 11
#error "CONFIG_SYS_RESERVED must have the value 11"
#endif
#else
#if CONFIG_SYS_RESERVED != 8
#error "CONFIG_SYS_RESERVED must have the value 8"
#endif
#endif //CONFIG_ARMV7A_TRUSTZONE

#else //CONFIG_BUILD_PROTECTED

#ifdef CONFIG_ARMV7A_TRUSTZONE
#if CONFIG_SYS_RESERVED != 7
#error "CONFIG_SYS_RESERVED must have the value 7"
#endif
#else
#if CONFIG_SYS_RESERVED != 4
#error "CONFIG_SYS_RESERVED must have the value 4"
#endif
#endif //CONFIG_ARMV7A_TRUSTZONE

#endif //CONFIG_BUILD_PROTECTED

#endif //CONFIG_LIB_SYSCALL

/* ARM System calls ***********************************************************/

/* SYS call 0:
 *
 * int arm_saveusercontext(uint32_t *saveregs);
 */

#define SYS_save_context          (0)

/* SYS call 1:
 *
 * void arm_fullcontextrestore(uint32_t *restoreregs) noreturn_function;
 */

#define SYS_restore_context       (1)

/* SYS call 2:
 *
 * void arm_switchcontext(uint32_t *saveregs, uint32_t *restoreregs);
 */

#define SYS_switch_context        (2)

#ifdef CONFIG_LIB_SYSCALL
/* SYS call 3:
 *
 * void up_syscall_return(void);
 */

#define SYS_syscall_return        (3)

#ifdef CONFIG_BUILD_PROTECTED
/* SYS call 4:
 *
 * void up_task_start(main_t taskentry, int argc, FAR char *argv[])
 *        noreturn_function;
 */

#define SYS_task_start            (4)

/* SYS call 5:
 *
 * void up_pthread_start(pthread_startroutine_t entrypt, pthread_addr_t arg)
 *        noreturn_function
 */

#define SYS_pthread_start         (5)

/* SYS call 6:
 *
 * void signal_handler(_sa_sigaction_t sighand, int signo, FAR siginfo_t *info,
 *                     FAR void *ucontext);
 */

#define SYS_signal_handler        (6)

/* SYS call 7:
 *
 * void signal_handler_return(void);
 */

#define SYS_signal_handler_return (7)

#ifdef CONFIG_ARMV7A_TRUSTZONE
/* SYS call 3:
 *
 * void SecureContext_Init(void);
 */

#define SYS_init_securecontext        (8)

/* SYS call 4:
 *
 * SecureContextHandle_t SecureContext_AllocateContext(uint32_t ulR0);
 */

#define SYS_allocate_securecontext        (9)

/* SYS call 5:
 *
 * void SecureContext_FreeContext( ( SecureContextHandle_t ) ulR0 );
 */

#define SYS_free_securecontext        (10)
#endif					/* CONFIG_BUILD_PROTECTED */
#else
#ifdef CONFIG_ARMV7A_TRUSTZONE
/* SYS call 3:
 *
 * void SecureContext_Init(void);
 */

#define SYS_init_securecontext        (4)

/* SYS call 4:
 *
 * SecureContextHandle_t SecureContext_AllocateContext(uint32_t ulR0);
 */

#define SYS_allocate_securecontext        (5)

/* SYS call 5:
 *
 * void SecureContext_FreeContext( ( SecureContextHandle_t ) ulR0 );
 */

#define SYS_free_securecontext        (6)
#endif

#endif
#else
#ifdef CONFIG_ARMV7A_TRUSTZONE
/* SYS call 3:
 *
 * void SecureContext_Init(void);
 */

#define SYS_init_securecontext        (3)

/* SYS call 4:
 *
 * SecureContextHandle_t SecureContext_AllocateContext(uint32_t ulR0);
 */

#define SYS_allocate_securecontext        (4)

/* SYS call 5:
 *
 * void SecureContext_FreeContext( ( SecureContextHandle_t ) ulR0 );
 */

#define SYS_free_securecontext        (5)
#endif
#endif					/* CONFIG_LIB_SYSCALL */

/* The SYS_signal_handler_return is executed here... its value is not always
 * available in this context and so is assumed to be 7.
 */

#ifndef SYS_signal_handler_return
#define SYS_signal_handler_return (7)
#elif SYS_signal_handler_return != 7
#error "SYS_signal_handler_return was assumed to be 7"
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Inline functions
 ****************************************************************************/

#ifndef __ASSEMBLY__

/* SVC with SYS_ call number and no parameters */

static inline uintptr_t sys_call0(unsigned int nbr)
{
	register long reg0 __asm__("r0") = (long)(nbr);

	__asm__ __volatile__
	(
		"svc %1"
		: "=r"(reg0)
		: "i"(SYS_syscall), "r"(reg0)
		: "memory", "r14"
	);

	return reg0;
}

/* SVC with SYS_ call number and one parameter */

static inline uintptr_t sys_call1(unsigned int nbr, uintptr_t parm1)
{
	register long reg0 __asm__("r0") = (long)(nbr);
	register long reg1 __asm__("r1") = (long)(parm1);

	__asm__ __volatile__
	(
		"svc %1"
		: "=r"(reg0)
		: "i"(SYS_syscall), "r"(reg0), "r"(reg1)
		: "memory", "r14"
	);

	return reg0;
}

/* SVC with SYS_ call number and two parameters */

static inline uintptr_t sys_call2(unsigned int nbr, uintptr_t parm1, uintptr_t parm2)
{
	register long reg0 __asm__("r0") = (long)(nbr);
	register long reg2 __asm__("r2") = (long)(parm2);
	register long reg1 __asm__("r1") = (long)(parm1);

	__asm__ __volatile__
	(
		"svc %1"
		: "=r"(reg0)
		: "i"(SYS_syscall), "r"(reg0), "r"(reg1), "r"(reg2)
		: "memory", "r14"
	);

	return reg0;
}

/* SVC with SYS_ call number and three parameters */

static inline uintptr_t sys_call3(unsigned int nbr, uintptr_t parm1, uintptr_t parm2, uintptr_t parm3)
{
	register long reg0 __asm__("r0") = (long)(nbr);
	register long reg3 __asm__("r3") = (long)(parm3);
	register long reg2 __asm__("r2") = (long)(parm2);
	register long reg1 __asm__("r1") = (long)(parm1);

	__asm__ __volatile__
	(
		"svc %1"
		: "=r"(reg0)
		: "i"(SYS_syscall), "r"(reg0), "r"(reg1), "r"(reg2), "r"(reg3)
		: "memory", "r14"
	);

	return reg0;
}

/* SVC with SYS_ call number and four parameters */

static inline uintptr_t sys_call4(unsigned int nbr, uintptr_t parm1, uintptr_t parm2, uintptr_t parm3, uintptr_t parm4)
{
	register long reg0 __asm__("r0") = (long)(nbr);
	register long reg4 __asm__("r4") = (long)(parm4);
	register long reg3 __asm__("r3") = (long)(parm3);
	register long reg2 __asm__("r2") = (long)(parm2);
	register long reg1 __asm__("r1") = (long)(parm1);

	__asm__ __volatile__
	(
		"svc %1"
		: "=r"(reg0)
		: "i"(SYS_syscall), "r"(reg0), "r"(reg1), "r"(reg2), "r"(reg3), "r"(reg4)
		: "memory", "r14"
	);

	return reg0;
}

/* SVC with SYS_ call number and five parameters */

static inline uintptr_t sys_call5(unsigned int nbr, uintptr_t parm1, uintptr_t parm2, uintptr_t parm3, uintptr_t parm4, uintptr_t parm5)
{
	register long reg0 __asm__("r0") = (long)(nbr);
	register long reg5 __asm__("r5") = (long)(parm5);
	register long reg4 __asm__("r4") = (long)(parm4);
	register long reg3 __asm__("r3") = (long)(parm3);
	register long reg2 __asm__("r2") = (long)(parm2);
	register long reg1 __asm__("r1") = (long)(parm1);

	__asm__ __volatile__
	(
		"svc %1"
		: "=r"(reg0)
		: "i"(SYS_syscall), "r"(reg0), "r"(reg1), "r"(reg2), "r"(reg3), "r"(reg4), "r"(reg5)
		: "memory", "r14"
	);

	return reg0;
}

/* SVC with SYS_ call number and six parameters */

static inline uintptr_t sys_call6(unsigned int nbr, uintptr_t parm1, uintptr_t parm2, uintptr_t parm3, uintptr_t parm4, uintptr_t parm5, uintptr_t parm6)
{
	register long reg0 __asm__("r0") = (long)(nbr);
	register long reg6 __asm__("r6") = (long)(parm6);
	register long reg5 __asm__("r5") = (long)(parm5);
	register long reg4 __asm__("r4") = (long)(parm4);
	register long reg3 __asm__("r3") = (long)(parm3);
	register long reg2 __asm__("r2") = (long)(parm2);
	register long reg1 __asm__("r1") = (long)(parm1);

	__asm__ __volatile__
	(
		"svc %1"
		: "=r"(reg0)
		: "i"(SYS_syscall), "r"(reg0), "r"(reg1), "r"(reg2), "r"(reg3), "r"(reg4), "r"(reg5), "r"(reg6)
		: "memory", "r14"
	);

	return reg0;
}
#endif

/****************************************************************************
 * Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Inline functions
 ****************************************************************************/

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifndef __ASSEMBLY__
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif

#endif							/* __ARCH_ARM_INCLUDE_ARMV7A_SYSCALL_H */
