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
 * apps/examples/testcase/ostest/ostest.h
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

#ifndef __APPS_TESTING_OSTEST_OSTEST_H
#define __APPS_TESTING_OSTEST_OSTEST_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdbool.h>
#include <inttypes.h>
#include <signal.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The task_create task size can be specified in the defconfig file */

#ifdef CONFIG_TESTING_OSTEST_STACKSIZE
#  define STACKSIZE CONFIG_TESTING_OSTEST_STACKSIZE
#else
#  define STACKSIZE 8192
#endif

/* The number of times to execute the test can be specified in the defconfig
 * file.
 */

#ifndef CONFIG_TESTING_OSTEST_LOOPS
#  define CONFIG_TESTING_OSTEST_LOOPS 1
#endif

/* This is the number of threads that are created in the barrier test.
 * A smaller number should be selected on systems without sufficient memory
 * to start so many threads.
 */

#ifndef CONFIG_TESTING_OSTEST_NBARRIER_THREADS
#  define CONFIG_TESTING_OSTEST_NBARRIER_THREADS 8
#endif

/* Priority inheritance */

#if defined(CONFIG_DEBUG_FEATURES) && defined(CONFIG_PRIORITY_INHERITANCE) && defined(CONFIG_SEM_PHDEBUG)
#  define dump_nfreeholders(s) printf(s " nfreeholders: %d\n", sem_nfreeholders())
#else
#  define dump_nfreeholders(s)
#endif

/* If CONFIG_STDIO_LINEBUFFER is defined, the STDIO buffer will be flushed
 * on each new line.  Otherwise, STDIO needs to be explicitly flushed to
 * see the output in context.
 */

#ifndef CONFIG_STDIO_BUFFER_SIZE
#  define CONFIG_STDIO_BUFFER_SIZE 0
#endif

#if defined(CONFIG_FILE_STREAM) && CONFIG_STDIO_BUFFER_SIZE > 0 && \
    !defined(CONFIG_STDIO_LINEBUFFER)
#  define FFLUSH() fflush(stdout)
#else
#  define FFLUSH()
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/* getopt.c *****************************************************************/

int getopt_test(void);

/* libc_memmem.c ************************************************************/

int memmem_test(void);

/* setvbuf.c ****************************************************************/

#ifndef CONFIG_STDIO_DISABLE_BUFFERING
int setvbuf_test(void);
#endif

/* dev_null.c ***************************************************************/

#ifdef CONFIG_DEV_NULL
int dev_null_test(void);
#endif

/* multiuser.c **************************************************************/

#if defined(CONFIG_TESTING_OSTEST_MULTIUSER) && defined(CONFIG_SCHED_USER_IDENTITY)
int multiuser_test(void);
#endif

/* fpu.c ********************************************************************/

void fpu_test(void);

/* aio.c ********************************************************************/

#ifdef CONFIG_TESTING_OSTEST_AIO
void aio_test(void);
#endif

/* restart.c ****************************************************************/

#ifndef CONFIG_BUILD_KERNEL
void restart_test(void);
#endif

/* waitpid.c ****************************************************************/

#ifdef CONFIG_SCHED_WAITPID
int waitpid_test(void);
#endif

/* wqueue.c *****************************************************************/

#if defined(CONFIG_SCHED_LPWORK) || defined(CONFIG_SCHED_HPWORK)
void wqueue_test(void);
#endif

/* mutex.c ******************************************************************/

void mutex_test(void);

/* timedmutex.c *************************************************************/

void timedmutex_test(void);

/* rmutex.c *****************************************************************/

void recursive_mutex_test(void);

/* sem.c ********************************************************************/

void sem_test(void);

/* semtimed.c ***************************************************************/

void semtimed_test(void);

/* nsem.c *******************************************************************/

void nsem_test(void);

/* cond.c *******************************************************************/

void cond_test(void);

/* specific.c ***************************************************************/

void specific_test(void);

/* mqueue.c *****************************************************************/

void mqueue_test(void);

/* timedmqueue.c ************************************************************/

void timedmqueue_test(void);

/* cancel.c *****************************************************************/

void cancel_test(void);

/* robust.c *****************************************************************/

#ifndef CONFIG_PTHREAD_MUTEX_UNSAFE
void robust_test(void);
#endif

/* timedwait.c **************************************************************/

void timedwait_test(void);

/* sigprocmask.c ************************************************************/

void sigprocmask_test(void);

/* sighelper.c **************************************************************/

bool sigset_isequal(FAR const sigset_t *left, FAR const sigset_t *right);

/* sighand.c ****************************************************************/

void sighand_test(void);

/* signest.c ****************************************************************/

void signest_test(void);

/* suspend.c ****************************************************************/

void suspend_test(void);

/* wdog.c *******************************************************************/

void wdog_test(void);

/* hrtimer.c ****************************************************************/

#ifdef CONFIG_HRTIMER
void hrtimer_test(void);
#endif

/* posixtimers.c ************************************************************/

void timer_test(void);
void sigev_thread_test(void);

/* roundrobin.c *************************************************************/

void rr_test(void);

/* sporadic.c ***************************************************************/

void sporadic_test(void);

/* sporadic2.c **************************************************************/

void sporadic2_test(void);

/* tls.c ********************************************************************/

void tls_test(void);

/* sched_thread_local.c *****************************************************/

void sched_thread_local_test(void);

/* pthread_rwlock.c *********************************************************/

void pthread_rwlock_test(void);

/* pthread_rwlock_cancel.c **************************************************/

void pthread_rwlock_cancel_test(void);

/* pthread_exit.c ***********************************************************/

#ifdef CONFIG_SCHED_WAITPID
void pthread_exit_test(void);
#endif

/* pthread_cleanup.c ********************************************************/

void pthread_cleanup_test(void);

/* barrier.c ****************************************************************/

void barrier_test(void);

/* prioinherit.c ************************************************************/

void priority_inheritance(void);

/* schedlock.c **************************************************************/

void sched_lock_test(void);

/* vfork.c ******************************************************************/

#ifdef CONFIG_ARCH_HAVE_VFORK
int vfork_test(void);
#endif

/* fork.c *******************************************************************/

#ifdef CONFIG_ARCH_HAVE_FORK
int fork_test(void);
#endif

/* setjmp.c *****************************************************************/

void setjmp_test(void);

/* smp_call.c ***************************************************************/

#ifdef CONFIG_SMP
void smp_call_test(void);
#endif

/* spinlock.c ***************************************************************/

void spinlock_test(void);

/* perf_gettime.c ***********************************************************/

#if defined(CONFIG_ARCH_HAVE_PERF_EVENTS) && defined(CONFIG_BUILD_FLAT)
void perf_gettime_test(void);
#endif

/* APIs exported (conditionally) by the OS specifically for testing of
 * priority inheritance
 */

#if defined(CONFIG_DEBUG_FEATURES) && defined(CONFIG_PRIORITY_INHERITANCE) && defined(CONFIG_SEM_PHDEBUG)
void sem_enumholders(FAR sem_t *sem);
int sem_nfreeholders(void);
#else
#  define sem_enumholders(sem)
#  define sem_nfreeholders()
#endif

/* nxevent.c ****************************************************************/

#if defined(CONFIG_SCHED_EVENTS) && defined(CONFIG_BUILD_FLAT)
void nxevent_test(void);
#endif

#endif /* __APPS_TESTING_OSTEST_OSTEST_H */
