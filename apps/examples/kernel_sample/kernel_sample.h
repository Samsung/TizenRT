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
 * examples/kernel_sample/kernel_sample.h
 *
 *   Copyright (C) 2007-2009, 2011-2012 Gregory Nutt. All rights reserved.
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

#ifndef __APPS_EXAMPLES_KERNEL_SAMPLE_KERNEL_SAMPLE_H
#define __APPS_EXAMPLES_KERNEL_SAMPLE_KERNEL_SAMPLE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/

/* The task_create task size can be specified in the defconfig file */

#ifdef CONFIG_EXAMPLES_KERNEL_SAMPLE_STACKSIZE
#  define STACKSIZE CONFIG_EXAMPLES_KERNEL_SAMPLE_STACKSIZE
#else
#  define STACKSIZE 8192
#endif

/* The number of times to execute the test can be specified in the defconfig
 * file.
 */

#ifndef CONFIG_EXAMPLES_KERNEL_SAMPLE_LOOPS
#  define CONFIG_EXAMPLES_KERNEL_SAMPLE_LOOPS 1
#endif

/* This is the number of threads that are created in the barrier test.
 * A smaller number should be selected on systems without sufficient memory
 * to start so many threads.
 */

#ifndef CONFIG_EXAMPLES_KERNEL_SAMPLE_NBARRIER_THREADS
#  define CONFIG_EXAMPLES_KERNEL_SAMPLE_NBARRIER_THREADS 8
#endif

/* Priority inheritance */

#if defined(CONFIG_DEBUG) && defined(CONFIG_PRIORITY_INHERITANCE) && defined(CONFIG_SEM_PHDEBUG)
#  define dump_nfreeholders(s) printf(s " nfreeholders: %d\n", sem_nfreeholders())
#else
#  define dump_nfreeholders(s)
#endif

/* If CONFIG_STDIO_LINEBUFFER is defined, the STDIO buffer will be flushed
 * on each new line.  Otherwise, STDIO needs to be explicitly flushed to
 * see the output in context.
 */

#if CONFIG_NFILE_DESCRIPTORS > 0 && CONFIG_NFILE_STREAMS > 0 && \
	CONFIG_STDIO_BUFFER_SIZE > 0 && !defined(CONFIG_STDIO_LINEBUFFER)
#  define FFLUSH() fflush(stdout)
#else
#  define FFLUSH()
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/* dev_null.c ***************************************************************/

int dev_null(void);

/* fpu.c ********************************************************************/

void fpu_test(void);

/* aio.c ********************************************************************/

#ifdef CONFIG_FS_AIO
void aio_test(void);
#endif

/* restart.c ****************************************************************/

void restart_test(void);

/* waitpid.c ****************************************************************/

#ifdef CONFIG_SCHED_WAITPID
int waitpid_test(void);
#endif

/* mutex.c ******************************************************************/

void mutex_test(void);

/* rmutex.c ******************************************************************/

void recursive_mutex_test(void);

/* sem.c ********************************************************************/

void sem_test(void);

/* semtimed.c ***************************************************************/

void semtimed_test(void);

/* nsem.c *******************************************************************/

void nsem_test(void);

/* cond.c *******************************************************************/

void cond_test(void);

/* mqueue.c *****************************************************************/

void mqueue_test(void);

/* timedmqueue.c ************************************************************/

void timedmqueue_test(void);

/* cancel.c *****************************************************************/

void cancel_test(void);

/* timedwait.c **************************************************************/

void timedwait_test(void);

/* sighand.c ****************************************************************/

void sighand_test(void);

/* posixtimers.c ************************************************************/

void timer_test(void);

/* roundrobin.c *************************************************************/

void rr_test(void);

/* barrier.c ****************************************************************/

void barrier_test(void);

/* prioinherit.c ************************************************************/

void priority_inheritance(void);

/* vfork.c ******************************************************************/

#if defined(CONFIG_ARCH_HAVE_VFORK) && defined(CONFIG_SCHED_WAITPID) && \
	!defined(CONFIG_DISABLE_SIGNALS)
int vfork_test(void);
#endif

/* workqueue.c **************************************************************/
#ifdef CONFIG_SCHED_WORKQUEUE
int workqueue_test(void);
#endif

/* pthread.c **************************************************************/
void pthread_test(void);

/* APIs exported (conditionally) by the OS specifically for testing of
 * priority inheritance
 */

#if defined(CONFIG_DEBUG) && defined(CONFIG_PRIORITY_INHERITANCE) && defined(CONFIG_SEM_PHDEBUG)
void sem_enumholders(FAR sem_t *sem);
int sem_nfreeholders(void);
#else
#  define sem_enumholders(sem)
#  define sem_nfreeholders()
#endif

#endif /* __APPS_EXAMPLES_KERNEL_SAMPLE_KERNEL_SAMPLE_H */
