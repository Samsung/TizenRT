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

#include <tinyara/config.h>
#include <tinyara/sched.h>
#include <debug.h>
#include "semaphore/semaphore.h"

#define SEM_INFO_MAX_ENTRIES 128

static unsigned int sem_print_flag_names(uint8_t flags)
{
	unsigned int count = 0;

	if (flags & PRIOINHERIT_FLAGS_DISABLE) {
		lldbg_noarg("NOPI");
		count++;
	}
	if (flags & FLAGS_INITIALIZED) {
		if (count) {
			lldbg_noarg("|");
		}
		lldbg_noarg("INIT");
		count++;
	}
	if (flags & FLAGS_SIGSEM) {
		if (count) {
			lldbg_noarg("|");
		}
		lldbg_noarg("SIG");
		count++;
	}
	if (flags & FLAGS_SEM_MUTEX) {
		if (count) {
			lldbg_noarg("|");
		}
		lldbg_noarg("MUTEX");
		count++;
	}

	return count;
}

static void sem_print_sem(FAR sem_t *sem)
{
	lldbg_noarg("SEM %p [count=%d, ", sem, sem->semcount);
	if (sem_print_flag_names(sem->flags) == 0) {
		lldbg_noarg("none");
	}
	lldbg_noarg("]");
}

#ifdef CONFIG_PRIORITY_INHERITANCE
static void sem_print_holder(FAR sem_t *sem, int pid, int16_t refs)
{
	sem_print_sem(sem);
	lldbg_noarg(" --HOLDER--> PID %d [refs=%d]\n", pid, refs);
}
#endif

static void sem_print_taskinfo(FAR struct tcb_s *tcb, FAR void *arg)
{
#ifdef CONFIG_PRIORITY_INHERITANCE
	FAR struct semholder_s *holder;
	unsigned int count;
#endif

	if (tcb->waitsem != NULL) {
#ifdef CONFIG_PRIORITY_INHERITANCE
#if defined(CONFIG_SEM_PREALLOCHOLDERS) && CONFIG_SEM_PREALLOCHOLDERS > 0
		holder = tcb->waitsem->hhead;
#else
		holder = (tcb->waitsem->holder.htcb != NULL || tcb->waitsem->holder.counts != 0) ? &tcb->waitsem->holder : NULL;
#endif
		for (count = 0; holder != NULL && count < SEM_INFO_MAX_ENTRIES; count++) {
			lldbg_noarg("PID %d --WAIT--> ", tcb->pid);
			sem_print_sem(tcb->waitsem);
			if (holder->htcb != NULL) {
				lldbg_noarg(" --HOLDER--> PID %d [refs=%d]\n", holder->htcb->pid, holder->counts);
			} else {
				lldbg_noarg(" --HOLDER--> status=invalid [refs=%d]\n", holder->counts);
			}
#if defined(CONFIG_SEM_PREALLOCHOLDERS) && CONFIG_SEM_PREALLOCHOLDERS > 0
			holder = holder->flink;
#else
			holder = NULL;
#endif
		}
		if (count == 0) {
			lldbg_noarg("PID %d --WAIT--> ", tcb->pid);
			sem_print_sem(tcb->waitsem);
			lldbg_noarg("\n");
		}
		if (holder != NULL) {
			lldbg_noarg("TRUNCATED list=holders sem=%p limit=%u\n", tcb->waitsem, SEM_INFO_MAX_ENTRIES);
		}
#else /* CONFIG_PRIORITY_INHERITANCE */
		lldbg_noarg("PID %d --WAIT--> ", tcb->pid);
		sem_print_sem(tcb->waitsem);
		lldbg_noarg("\n");
#endif /* CONFIG_PRIORITY_INHERITANCE */
	}

#ifdef CONFIG_PRIORITY_INHERITANCE
	for (holder = tcb->holdsem, count = 0; holder != NULL && count < SEM_INFO_MAX_ENTRIES; holder = holder->tlink, count++) {
		if (holder->sem != NULL) {
			if (holder->sem->semcount >= 0) {
				sem_print_holder(holder->sem, tcb->pid, holder->counts);
			}
		} else {
			lldbg_noarg("SEM status=invalid --HOLDER--> PID %d [refs=%d]\n", tcb->pid, holder->counts);
		}
	}
	if (holder != NULL) {
		lldbg_noarg("TRUNCATED list=held_sems PID=%d limit=%u\n", tcb->pid, SEM_INFO_MAX_ENTRIES);
	}
#endif /* CONFIG_PRIORITY_INHERITANCE */
}

/****************************************************************************
 * Name: sem_showinfo
 *
 * Description:
 *   Print semaphore relationships during assertion handling, with tasks
 *   stopped by the caller. Do not allocate memory or acquire semaphore locks.
 ****************************************************************************/

void sem_showinfo(void)
{
	lldbg_noarg("===========================================================\n");
	lldbg_noarg("Semaphore info\n");
	lldbg_noarg("===========================================================\n");
#ifndef CONFIG_PRIORITY_INHERITANCE
	lldbg_noarg("CONFIG_PRIORITY_INHERITANCE is not set. Holder tracking not available.\n");
#endif
	sched_foreach(sem_print_taskinfo, NULL);
}
