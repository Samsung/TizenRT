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
 *
 *   Copyright (C) 2013 Gregory Nutt. All rights reserved.
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

#ifndef __INCLUDE_USERSPACE_H
#define __INCLUDE_USERSPACE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <signal.h>
#include <pthread.h>

#ifdef CONFIG_BUILD_PROTECTED

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/****************************************************************************
 * Public Type Definitions
 ****************************************************************************/
/* Every user-space blob starts with a header that provides information about
 * the blob.  The form of that header is provided by struct userspace_s.  An
 * instance of this structure is expected to reside at __uflash_segment_start__.
 */

struct userspace_s {
	/* Task/thread startup routines */

	void (*task_startup)(main_t entrypt, int argc, FAR char *argv[])
	noreturn_function;
#ifndef CONFIG_DISABLE_PTHREAD
	void (*pthread_startup)(pthread_startroutine_t entrypt, pthread_addr_t arg);
#endif

	/* Signal handler trampoline */

#ifndef CONFIG_DISABLE_SIGNALS
	void (*signal_handler)(_sa_sigaction_t sighand, int signo, FAR siginfo_t *info, FAR void *ucontext);
#endif

#ifdef CONFIG_XIP_ELF
	/* data, bss and heap info used for loading them into ram */
	void * text_start;
	void * bss_start;
	void * bss_end;
	void * data_start_in_ram;
	void * data_end_in_ram;
	void * data_start_in_flash;
	void * heap_start;
	void * heap_end;
	void * sctors;
	void * ectors;
	void * flash_end;
	void * ram_start;
	void * ram_end;
	void (*entry)(int argc, char **argv);
#endif
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: task_startup
 *
 * Description:
 *   This function is the user-space, task startup function.  It is called
 *   from up_task_start() in user-mode.
 *
 * Inputs:
 *   entrypt - The user-space address of the task entry point
 *   argc and argv - Standard arguments for the task entry point
 *
 * Return:
 *   None.  This function does not return.
 *
 ****************************************************************************/

#if defined(CONFIG_BUILD_PROTECTED) && !defined(__KERNEL__)
void task_startup(main_t entrypt, int argc, FAR char *argv[]) noreturn_function;
#endif

/****************************************************************************
 * Name: pthread_startup
 *
 * Description:
 *   This function is the user-space, pthread startup function.  It is called
 *   from up_pthread_start() in user-mode.
 *
 * Inputs:
 *   entrypt - The user-space address of the pthread entry point
 *   arg     - Standard argument for the pthread entry point
 *
 * Return:
 *   None.  This function does not return.
 *
 ****************************************************************************/

#if defined(CONFIG_BUILD_PROTECTED) && !defined(__KERNEL__) && !defined(CONFIG_DISABLE_PTHREAD)
void pthread_startup(pthread_startroutine_t entrypt, pthread_addr_t arg);
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* CONFIG_BUILD_PROTECTED */
#endif							/* __INCLUDE_USERSPACE_H */
