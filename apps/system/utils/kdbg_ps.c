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
 * apps/system/utils/kdbg_ps.c
 *
 *   Copyright (C) 2007-2009, 2011-2012, 2014 Gregory Nutt. All rights reserved.
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

#include <tinyara/config.h>
#include <stdio.h>
#include <tinyara/sched.h>

static const char *kdbg_statenames[] = {
	"INVALID ",
	"PENDING ",
	"READY   ",
	"RUNNING ",
	"INACTIVE",
	"WAITSEM ",
#ifndef CONFIG_DISABLE_SIGNALS
	"WAITSIG ",
#endif
#ifndef CONFIG_DISABLE_MQUEUE
	"MQNEMPTY",
	"MQNFULL "
#endif
};

static const char *kdbg_ttypenames[4] = {
	"TASK   ",
	"PTHREAD",
	"KTHREAD",
	"--?--  "
};

static void kdbg_pseach(FAR struct tcb_s *tcb, FAR void *arg)
{
	printf("%5d | %4d | %4s | %7s | %c%c | %8s", tcb->pid, tcb->sched_priority, tcb->flags & TCB_FLAG_ROUND_ROBIN ? "RR  " : "FIFO", kdbg_ttypenames[(tcb->flags & TCB_FLAG_TTYPE_MASK) >> TCB_FLAG_TTYPE_SHIFT], tcb->flags & TCB_FLAG_NONCANCELABLE ? 'N' : ' ', tcb->flags & TCB_FLAG_CANCEL_PENDING ? 'P' : ' ', kdbg_statenames[tcb->task_state]);
#if CONFIG_TASK_NAME_SIZE > 0
	printf(" | %s", tcb->name);
#endif
	printf("\n");
}

int kdbg_ps(int argc, char **args)
{
#if CONFIG_TASK_NAME_SIZE > 0
	printf("\n");
	printf("  PID | PRIO | FLAG |  TYPE   | NP |  STATUS  | NAME\n");
	printf("------|------|------|---------|----|----------|----------\n");
#else
	printf("\n");
	printf("  PID | PRIO | FLAG |  TYPE   | NP |  STATUS\n");
	printf("------|------|------|---------|----|--------\n");
#endif
	sched_foreach(kdbg_pseach, NULL);
	return 0;
}
