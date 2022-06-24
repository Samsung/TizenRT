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
/************************************************************************************
 * arch/arm/src/armv8-m/exc_return.h
 *
 *   Copyright (C) 2011-2012 Gregory Nutt. All rights reserved.
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
 ************************************************************************************/

#ifndef __ARCH_ARM_SRC_ARMV8_M_EXC_RETURN_H
#define __ARCH_ARM_SRC_ARMV8_M_EXC_RETURN_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* The processor saves an EXC_RETURN value to the LR on exception entry. The
 * exception mechanism relies on this value to detect when the processor has
 * completed an exception handler.
 *
 * Bits [31:28] of an EXC_RETURN value are always 1.  When the processor loads a
 * value matching this pattern to the PC it detects that the operation is a not
 * a normal branch operation and instead, that the exception is complete.
 * Therefore, it starts the exception return sequence.
 *
 * Bits[4:0] of the EXC_RETURN value indicate the required return stack and eventual
 * processor mode.  The remaining bits of the EXC_RETURN value should be set to 1.
 */

/* EXC_RETURN_BASE: Bits that are always set in an EXC_RETURN value. */

/* In case of ARMv8 with TZ, following bits have special meaning. 
 * BIT 6 :	Set if secure stack is used, else reset
 * BIT 5 : 	Set if default rules used for stacking callee registers
 * BIT 4 :	Set if space for FP registers is not allocated
 * BIT 3 :	Set for Thread mode, reset for Handler mode
 * BIT 2 : 	Set for using PSP, reset for using MSP
 * BIT 0 :	Set for secure state, reset for non-secure state
 *
 * Here we are setting the default values assuming non secure state of
 * execution. Bits 6, 5 and 0 are especially important, because they
 * differ from the ARMv7 implementation
 */

#define EXC_RETURN_BASE          0xffffffa0

/* EXC_RETURN_EXC_SECURE: Exception Secure.  The security domain the
 * exception was taken to.  If this bit is clear non-secure, else secure.
 */

#define EXC_RETURN_EXC_SECURE    (1 << 0)

/* EXC_RETURN_PROCESS_STACK: The exception saved (and will restore) the hardware
 * context using the process stack pointer (if not set, the context was saved
 * using the main stack pointer)
 */

#define EXC_RETURN_PROCESS_STACK (1 << 2)

/* EXC_RETURN_THREAD_MODE: The exception will return to thread mode (if not set,
 * return stays in handler mode)
 */

#define EXC_RETURN_THREAD_MODE   (1 << 3)

/* EXC_RETURN_STD_CONTEXT: The state saved on the stack does not include the
 * volatile FP registers and FPSCR.  If this bit is clear, the state does include
 * these registers.
 */

#define EXC_RETURN_STD_CONTEXT   (1 << 4)

/* EXC_RETURN_DEF_STACKING: Default callee register stacking (DCRS).
 * Indicates whether the default stacking rules apply, or whether the callee
 * registers are already on the stack.  The possible values of this bit are:
 * 0 - Stacking of the callee saved registers skipped.
 * 1 - Default rules for
 * stacking the callee registers followed.
 */

#define EXC_RETURN_DEF_STACKING  (1 << 5)

/* EXC_RETURN_SECURE_STACK: Secure or Non-secure stack.  Indicates whether a
 * Secure or Non-secure stack is used to restore stack frame on exception
 * return.  The possible values of this bit are: 
 * 0 -  Non-secure stack used.
 * 1 - Secure stack used.
 */

#define EXC_RETURN_SECURE_STACK  (1 << 6)

/* EXC_RETURN_HANDLER: Return to handler mode. Exception return gets state
 * from the main stack. Execution uses MSP after return.
 */

#define EXC_RETURN_HANDLER       (EXC_RETURN_BASE | EXC_RETURN_DEF_STACKING | \
                                  EXC_RETURN_STD_CONTEXT)

/* EXC_RETURN_PRIVTHR: Return to privileged thread mode. Exception return gets
 * state from the main stack. Execution uses MSP after return.
 */

#if defined(CONFIG_ARM_CMNVECTOR) && defined(CONFIG_ARCH_FPU)
#define EXC_RETURN_PRIVTHR     (EXC_RETURN_BASE | EXC_RETURN_THREAD_MODE)
#else
#define EXC_RETURN_PRIVTHR     (EXC_RETURN_BASE | EXC_RETURN_STD_CONTEXT | \
								EXC_RETURN_THREAD_MODE)
#endif

/* EXC_RETURN_UNPRIVTHR: Return to unprivileged thread mode. Exception return gets
 * state from the process stack. Execution uses PSP after return.
 */

#if defined(CONFIG_ARM_CMNVECTOR) && defined(CONFIG_ARCH_FPU)
#define EXC_RETURN_UNPRIVTHR   (EXC_RETURN_BASE | EXC_RETURN_THREAD_MODE | \
								EXC_RETURN_PROCESS_STACK)
#else
#define EXC_RETURN_UNPRIVTHR   (EXC_RETURN_BASE | EXC_RETURN_STD_CONTEXT | \
								EXC_RETURN_THREAD_MODE | EXC_RETURN_PROCESS_STACK)
#endif

/************************Th************************************************************
 * Inline Functions
 ************************************************************************************/

#endif							/* __ARCH_ARM_SRC_ARMV8_M_EXC_RETURN_H */
