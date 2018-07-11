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
 * kernel/task/task.h
 *
 *   Copyright (C) 2007-2014 Gregory Nutt. All rights reserved.
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

#ifndef __SCHED_TASK_TASK_H
#define __SCHED_TASK_TASK_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/compiler.h>

#include <sys/types.h>
#include <stdbool.h>

#include <tinyara/sched.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Type Definitions
 ****************************************************************************/

#if defined(CONFIG_SCHED_ATEXIT) && !defined(CONFIG_SCHED_ONEXIT)
struct atexit_s {
	struct atexit_s *flink;
	struct atexit_s *blink;
	atexitfunc_t atexitfunc;
};
#endif

#ifdef CONFIG_SCHED_ONEXIT
struct onexit_s {
	struct onexit_s *flink;
	struct onexit_s *blink;
	onexitfunc_t onexitfunc;
	void *onexitarg;
};
#endif

/****************************************************************************
 * Global Variables
 ****************************************************************************/
/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/* Task start-up */

void task_start(void);
int task_schedsetup(FAR struct task_tcb_s *tcb, int priority, start_t start, main_t main, uint8_t ttype);
int task_argsetup(FAR struct task_tcb_s *tcb, FAR const char *name, FAR char *const argv[]);

/* Task exit */

int task_exit(void);
int task_terminate(pid_t pid, bool nonblocking);
void task_exithook(FAR struct tcb_s *tcb, int status, bool nonblocking);
void task_recover(FAR struct tcb_s *tcb);
#if defined(CONFIG_SCHED_ATEXIT) && !defined(CONFIG_SCHED_ONEXIT)
void task_atexit(FAR struct tcb_s *tcb);
#else
#define task_atexit(tcb)
#endif
#ifdef CONFIG_SCHED_ONEXIT
void task_onexit(FAR struct tcb_s *tcb, int status);
#else
#define task_onexit(tcb, status)
#endif
/* Misc. */

bool sched_addreadytorun(FAR struct tcb_s *rtrtcb);

#ifdef CONFIG_CANCELLATION_POINTS
void notify_cancellation(FAR struct tcb_s *tcb);
#endif

#endif							/* __SCHED_TASK_TASK_H */
