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
 * kernel/sched/sched.h
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

#ifndef __SCHED_SCHED_SCHED_H
#define __SCHED_SCHED_SCHED_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdbool.h>
#include <queue.h>
#include <sched.h>
#ifdef CONFIG_SCHED_CPULOAD
#include <tinyara/clock.h>
#endif
#include <tinyara/kmalloc.h>
#include <tinyara/spinlock.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Although task IDs can take the (positive, non-zero)
 * range of pid_t, the number of tasks that will be supported
 * at any one time is (artificially) limited by the CONFIG_MAX_TASKS
 * configuration setting. Limiting the number of tasks speeds certain
 * OS functions (this is the only limitation in the number of
 * tasks built into the design).
 */

#if CONFIG_MAX_TASKS & (CONFIG_MAX_TASKS - 1)
#error "Max number of tasks(CONFIG_MAX_TASKS) should be power of 2"
#endif

/* These are macros to access the current CPU and the current task on a CPU.
 * These macros are intended to support a future SMP implementation.
 */
#ifdef CONFIG_SMP
#define current_task(cpu)	((FAR struct tcb_s *)g_assignedtasks[cpu].head)
#define this_cpu()		up_cpu_index()
#else
#define current_task(cpu)	((FAR struct tcb_s *)g_readytorun.head)
#define this_cpu()		(0)
#define this_task()		(current_task(this_cpu()))
#endif

#define TLIST_ATTR_PRIORITIZED   (1 << 0) /* Bit 0: List is prioritized */
#define TLIST_ATTR_INDEXED       (1 << 1) /* Bit 1: List is indexed by CPU */
#define TLIST_ATTR_RUNNABLE      (1 << 2) /* Bit 2: List includes running task */

#define __TLIST_HEAD(s)			(FAR dq_queue_t *)g_tasklisttable[s].list
#define __TLIST_HEADINDEXED(s,c)	(&(__TLIST_HEAD(s))[c])

#ifdef CONFIG_SMP
#define TLIST_HEAD(s, c) 	__TLIST_HEADINDEXED(s, c)
#else
#define TLIST_HEAD(s)		__TLIST_HEAD(s)
#endif
#define TLIST_BLOCKED(s)	__TLIST_HEAD(s)

/****************************************************************************
 * Public Type Definitions
 ****************************************************************************/

/* This structure defines the format of the hash table that is used to (1)
 * determine if a task ID is unique, and (2) to map a process ID to its
 * corresponding TCB.
 *
 * NOTE also that CPU load measurement data is retained in his table vs. in
 * the TCB which would seem to be the more logic place.  It is place in the
 * hash table, instead, to facilitate CPU load adjustments on all threads
 * during timer interrupt handling. sched_foreach() could do this too, but
 * this would require a little more overhead.
 */

struct pidhash_s {
	FAR struct tcb_s *tcb;		/* TCB assigned to this PID */
	pid_t pid;					/* The full PID value */
#ifdef CONFIG_SCHED_CPULOAD
	uint32_t ticks[SCHED_NCPULOAD];				/* Number of ticks on this thread */
#endif
};

/* This structure defines an element of the g_tasklisttable[].
 * This table is used to map a task_state enumeration to the
 * corresponding task list.
 */

struct tasklist_s {
	DSEG volatile dq_queue_t *list;	/* Pointer to the task list */
	bool prioritized;			/* true if the list is prioritized */
};

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/* Declared in os_start.c ***************************************************/

/* The state of a task is indicated both by the task_state field of the TCB
 * and by a series of task lists.  All of these tasks lists are declared
 * below. Although it is not always necessary, most of these lists are
 * prioritized so that common list handling logic can be used (only the
 * g_readytorun, the g_pendingtasks, and the g_waitingforsemaphore lists need
 * to be prioritized).
 */

/* This is the list of all tasks that are ready to run.  The head of this
 * list is the currently active task; the tail of this list is always the
 * IDLE task.
 */

extern volatile dq_queue_t g_readytorun;

#ifdef CONFIG_SMP
/* In order to support SMP, the function of the g_readytorun list changes,
 * The g_readytorun is still used but in the SMP case it will contain only:
 *
 *  - Only tasks/threads that are eligible to run, but not currently running,
 *    and
 *  - Tasks/threads that have not been assigned to a CPU.
 *
 * Otherwise, the TCB will be retained in an assigned task list,
 * g_assignedtasks.  As its name suggests, on 'g_assignedtasks queue for CPU
 * 'n' would contain only tasks/threads that are assigned to CPU 'n'.  Tasks/
 * threads would be assigned a particular CPU by one of two mechanisms:
 *
 *  - (Semi-)permanently through an RTOS interfaces such as
 *    pthread_attr_setaffinity(), or
 *  - Temporarily through scheduling logic when a previously unassigned task
 *    is made to run.
 *
 * Tasks/threads that are assigned to a CPU via an interface like
 * pthread_attr_setaffinity() would never go into the g_readytorun list, but
 * would only go into the g_assignedtasks[n] list for the CPU 'n' to which
 * the thread has been assigned.  Hence, the g_readytorun list would hold
 * only unassigned tasks/threads.
 *
 * Like the g_readytorun list in in non-SMP case, each g_assignedtask[] list
 * is prioritized:  The head of the list is the currently active task on this
 * CPU.  Tasks after the active task are ready-to-run and assigned to this
 * CPU. The tail of this assigned task list, the lowest priority task, is
 * always the CPU's IDLE task.
 */

extern volatile dq_queue_t g_assignedtasks[CONFIG_SMP_NCPUS];
#endif

/* g_running_tasks[] holds a references to the running task for each cpu.
 * It is valid only when up_interrupt_context() returns true.
 */

extern FAR struct tcb_s *g_running_tasks[CONFIG_SMP_NCPUS];

extern volatile dq_queue_t g_pendingtasks;

/* This is the list of all tasks that are blocked waiting for a semaphore */

extern volatile dq_queue_t g_waitingforsemaphore;

/* This is the list of all tasks that are blocked waiting for a signal */

#ifndef CONFIG_DISABLE_SIGNALS
extern volatile dq_queue_t g_waitingforsignal;
#endif

/* This is the list of all tasks that are blocked waiting for a message
 * queue to become non-empty.
 */

#ifndef CONFIG_DISABLE_MQUEUE
extern volatile dq_queue_t g_waitingformqnotempty;
#endif

/* This is the list of all tasks that are blocked waiting for a message
 * queue to become non-full.
 */

#ifndef CONFIG_DISABLE_MQUEUE
extern volatile dq_queue_t g_waitingformqnotfull;
#endif

/* This is the list of all tasks that are blocking waiting for a page fill */

#ifdef CONFIG_PAGING
extern volatile dq_queue_t g_waitingforfill;
#endif

/* This the list of all tasks that have been initialized, but not yet
 * activated. NOTE:  This is the only list that is not prioritized.
 */

extern volatile dq_queue_t g_inactivetasks;

/* This gives number of alive tasks at any point of time in the system.
 * If the system is already running CONFIG_MAX_TASKS, Creating new
 * task is not supported.
 */
extern volatile uint8_t g_alive_taskcount;

/* These are lists of dayed memory deallocations that need to be handled
 * within the IDLE loop or worker thread.  These deallocations get queued
 * by sched_kufree and sched_kfree() if the OS needs to deallocate memory
 * while it is within an interrupt handler.
 */

extern volatile sq_queue_t g_delayed_kufree;

extern volatile sq_queue_t g_delayed_kfree;

/* This is the value of the last process ID assigned to a task */

extern volatile pid_t g_lastpid;

/* The following hash table is used for two things:
 *
 * 1. This hash table greatly speeds the determination of a new unique
 *    process ID for a task, and
 * 2. Is used to quickly map a process ID into a TCB.
 *
 * It has the side effects of using more memory and limiting the number
 * of tasks to CONFIG_MAX_TASKS.
 */

#ifdef CONFIG_SMP
extern struct pidhash_s g_pidhash[CONFIG_SMP_NCPUS][CONFIG_MAX_TASKS];
extern volatile int g_npidhash;
#else
extern struct pidhash_s g_pidhash[CONFIG_MAX_TASKS];
#endif

/* This is a table of task lists.  This table is indexed by the task state
 * enumeration type (tstate_t) and provides a pointer to the associated
 * static task list (if there is one) as well as a boolean indication as to
 * if the list is an ordered list or not.
 */

extern const struct tasklist_s g_tasklisttable[NUM_TASK_STATES];

#ifdef CONFIG_SMP
/* In the multiple CPU, SMP case, disabling context switches will not give a
 * task exclusive access to the (multiple) CPU resources (at least without
 * stopping the other CPUs): Even though pre-emption is disabled, other
 * threads will still be executing on the other CPUS.
 *
 * There are additional rules for this multi-CPU case:
 *
 * 1. There is a global lock count 'g_cpu_lockset' that includes a bit for
 *    each CPU: If the bit is '1', then the corresponding CPU has the
 *    scheduler locked; if '0', then the CPU does not have the scheduler
 *    locked.
 * 2. Scheduling logic would set the bit associated with the cpu in
 *    'g_cpu_lockset' when the TCB at the head of the g_assignedtasks[cpu]
 *    list transitions has 'lockcount' > 0. This might happen when
 *    sched_lock() is called, or after a context switch that changes the
 *    TCB at the head of the g_assignedtasks[cpu] list.
 * 3. Similarly, the cpu bit in the global 'g_cpu_lockset' would be cleared
 *    when the TCB at the head of the g_assignedtasks[cpu] list has
 *    'lockcount' == 0. This might happen when sched_unlock() is called, or
 *    after a context switch that changes the TCB at the head of the
 *    g_assignedtasks[cpu] list.
 * 4. Modification of the global 'g_cpu_lockset' must be protected by a
 *    spinlock, 'g_cpu_schedlock'. That spinlock would be taken when
 *    sched_lock() is called, and released when sched_unlock() is called.
 *    This assures that the scheduler does enforce the critical section.
 *    NOTE: Because of this spinlock, there should never be more than one
 *    bit set in 'g_cpu_lockset'; attempts to set additional bits should
 *    be cause the CPU to block on the spinlock.  However, additional bits
 *    could get set in 'g_cpu_lockset' due to the context switches on the
 *    various CPUs.
 * 5. Each the time the head of a g_assignedtasks[] list changes and the
 *    scheduler modifies 'g_cpu_lockset', it must also set 'g_cpu_schedlock'
 *    depending on the new state of 'g_cpu_lockset'.
 * 6. Logic that currently uses the currently running tasks lockcount
 *    instead uses the global 'g_cpu_schedlock'. A value of SP_UNLOCKED
 *    means that no CPU has pre-emption disabled; SP_LOCKED means that at
 *    least one CPU has pre-emption disabled.
 */

extern volatile spinlock_t g_cpu_schedlock;

/* Used to keep track of which CPU(s) hold the IRQ lock. */

extern volatile spinlock_t g_cpu_locksetlock;
extern volatile cpu_set_t g_cpu_lockset;

/* Used to lock tasklist to prevent from concurrent access */

extern volatile spinlock_t g_cpu_tasklistlock;

#endif /* CONFIG_SMP */

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

bool sched_addreadytorun(FAR struct tcb_s *rtrtcb);
bool sched_removereadytorun(FAR struct tcb_s *rtrtcb);
bool sched_addprioritized(FAR struct tcb_s *newTcb, DSEG dq_queue_t *list);
bool sched_mergepending(void);
void sched_addblocked(FAR struct tcb_s *btcb, tstate_t task_state);
void sched_removeblocked(FAR struct tcb_s *btcb);
int sched_setpriority(FAR struct tcb_s *tcb, int sched_priority);

#ifdef CONFIG_PRIORITY_INHERITANCE
int sched_reprioritize(FAR struct tcb_s *tcb, int sched_priority);
#else
#define sched_reprioritize(tcb, sched_priority) \
		sched_setpriority(tcb, sched_priority)
#endif

#ifdef CONFIG_SCHED_TICKLESS
unsigned int sched_timer_cancel(void);
void sched_timer_resume(void);
void sched_timer_reassess(void);
#else
#define sched_timer_cancel() (0)
#define sched_timer_resume()
#define sched_timer_reassess()
#endif

#ifdef CONFIG_SCHED_CPULOAD
#ifndef CONFIG_SCHED_CPULOAD_EXTCLK
void weak_function sched_process_cpuload(void);
#endif
void sched_clear_cpuload(pid_t pid);
#endif

#ifdef CONFIG_SMP
FAR struct tcb_s *this_task(void);

int  sched_select_cpu(cpu_set_t affinity);
int  sched_pause_cpu(FAR struct tcb_s *tcb);

#  define sched_islocked_global() spin_islocked(&g_cpu_schedlock)
#  define sched_islocked_tcb(tcb) sched_islocked_global()

#else
#  define sched_select_cpu(a)     (0)
#  define sched_pause_cpu(t)      (-38)  /* -ENOSYS */
#  define sched_islocked_tcb(tcb) ((tcb)->lockcount > 0)
#endif

bool sched_verifytcb(FAR struct tcb_s *tcb);
int sched_releasetcb(FAR struct tcb_s *tcb, uint8_t ttype);

#endif							/* __SCHED_SCHED_SCHED_H */
