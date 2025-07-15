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
 * kernel/pthread/pthread.h
 *
 *   Copyright (C) 2007-2009, 2011, 2013-2014 Gregory Nutt. All rights reserved.
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

#ifndef __SCHED_PTHREAD_PTHREAD_H
#define __SCHED_PTHREAD_PTHREAD_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <sched.h>

#include <tinyara/compiler.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Type Declarations
 ****************************************************************************/

/* The following defines an entry in the pthread logic's local data set.
 * Note that this structure is used to implemented a singly linked list.
 * This structure is used (instead of, say, a binary search tree) because
 * the data set will be searched using the pid as a key -- a process IDs will
 * always be created in a montonically increasing fashion.
 */

struct join_s {
	FAR struct join_s *next;	/* Implements link list */
	uint8_t crefs;				/* Reference count */
	bool started;				/* true: pthread started. */
	bool detached;				/* true: pthread_detached'ed */
	bool terminated;			/* true: detach'ed+exit'ed */
	pthread_t thread;			/* Includes pid */
	sem_t exit_sem;				/* Implements join */
	sem_t data_sem;				/* Implements join */
	pthread_addr_t exit_value;	/* Returned data */
};

/****************************************************************************
 * Public Variables
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

struct pthread_tcb_s;			/* Forward reference */
struct task_group_s;			/* Forward reference */

void weak_function pthread_initialize(void);
int pthread_schedsetup(FAR struct pthread_tcb_s *tcb, int priority, start_t start, pthread_startroutine_t entry);
#ifdef CONFIG_PTHREAD_CLEANUP
void pthread_cleanup_popall(FAR struct pthread_tcb_s *tcb);
#endif
int pthread_completejoin(pid_t pid, FAR void *exit_value);
void pthread_destroyjoin(FAR struct task_group_s *group, FAR struct join_s *pjoin);
FAR struct join_s *pthread_findjoininfo(FAR struct task_group_s *group, pid_t pid);
int pthread_join_internal(pthread_t thread, FAR pthread_addr_t *pexit_value, bool blocking);
void pthread_release(FAR struct task_group_s *group);
int pthread_sem_take(sem_t *sem);
#ifdef CONFIG_PTHREAD_MUTEX_UNSAFE
int pthread_sem_trytake(sem_t *sem);
#endif
int pthread_sem_give(sem_t *sem);

#ifndef CONFIG_PTHREAD_MUTEX_UNSAFE
int pthread_mutex_take(FAR struct pthread_mutex_s *mutex);
int pthread_mutex_trytake(FAR struct pthread_mutex_s *mutex);
int pthread_mutex_give(FAR struct pthread_mutex_s *mutex);
void pthread_mutex_inconsistent(FAR struct pthread_tcb_s *tcb);
#else
#define pthread_mutex_take(m) pthread_sem_take(&(m)->sem)
#define pthread_mutex_trytake(m) pthread_sem_trytake(&(m)->sem)
#define pthread_mutex_give(m)   pthread_sem_give(&(m)->sem)
#endif

#if defined(CONFIG_CANCELLATION_POINTS) && !defined(CONFIG_PTHREAD_MUTEX_UNSAFE)
uint16_t pthread_disable_cancel(void);
void pthread_enable_cancel(uint16_t oldstate);
#else
#  define pthread_disable_cancel() (0)
#  define pthread_enable_cancel(s) UNUSED(s)
#endif

#ifdef CONFIG_PTHREAD_MUTEX_TYPES
int pthread_mutexattr_verifytype(int type);
#endif

#if defined(CONFIG_NPTHREAD_KEYS) && CONFIG_NPTHREAD_KEYS > 0
void pthread_key_destroy(struct pthread_tcb_s *tcb);
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __SCHED_PTHREAD_PTHREAD_H */
