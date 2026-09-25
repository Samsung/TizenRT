/****************************************************************************
 * kernel/mqueue/mq_verifydesg.c
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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <mqueue.h>
#include <errno.h>
#include <sched.h>
#include <debug.h>

#include <tinyara/sched.h>
#include <tinyara/irq.h>

#include "sched/sched.h"
#include "mqueue/mqueue.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

#ifdef CONFIG_LOG_DUMP
/* Cached log_dump mqueue descriptor, set in log_dump() after mq_open()
 * and cleared when freed in mq_desclose_group().
 */
static mqd_t g_log_dump_mqdes;
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mq_verifydesg_set_logdump_mqdes
 *
 * Description:
 *   Cache or clear the log_dump mqueue descriptor. Called from log_dump()
 *   after mq_open() to cache the descriptor, and from mq_desclose_group()
 *   when it is freed.
 *
 ****************************************************************************/

#ifdef CONFIG_LOG_DUMP
void mq_verifydesg_set_logdump_mqdes(mqd_t mqdes)
{
	g_log_dump_mqdes = mqdes;
}

mqd_t mq_verifydesg_get_logdump_mqdes(void)
{
	return g_log_dump_mqdes;
}
#endif

/****************************************************************************
 * Name: mq_verifydesg
 *
 * Description:
 *   Verify that the given message queue descriptor (mqdes) belongs to the
 *   calling task's group, preventing forged descriptor attacks where a
 *   malicious userspace task passes an arbitrary mqd_t pointer into mqueue
 *   syscalls to access or mutate kernel state it does not own.
 *
 *   Verification steps:
 *     1. Interrupt context and kernel threads are trusted callers and
 *        bypass the check (they do not enter via the syscall interface
 *        and may legitimately share descriptors across groups).
 *     2. Under CONFIG_LOG_DUMP, the log_dump mqueue descriptor is
 *        whitelisted because it is shared across groups. The descriptor
 *        is cached in g_log_dump_mqdes (set in log_dump after mq_open,
 *        cleared in mq_desclose_group).
 *     3. Otherwise, walk the current task group's tg_msgdesq list and
 *        return OK only if mqdes is found. The walk uses
 *        enter_critical_section() for SMP safety (sched_lock alone only
 *        prevents context switches on the local CPU).
 *
 * Parameters:
 *   mqdes - Message queue descriptor to verify (must be non-NULL)
 *
 * Return Value:
 *   OK (0) if the descriptor belongs to the caller's task group.
 *   ERROR (-1) with errno set to EPERM if not owned by the caller,
 *   or EBADF if the caller's TCB/group is invalid.
 *
 ****************************************************************************/

int mq_verifydesg(mqd_t mqdes)
{
	FAR struct tcb_s *rtcb;
	FAR struct task_group_s *group;
	mqd_t mqdes_ptr;
	irqstate_t flags;

	DEBUGASSERT(mqdes != NULL);

	rtcb = this_task();
	if (rtcb == NULL) {
		set_errno(EBADF);
		return ERROR;
	}

	/* Interrupt context and kernel threads are trusted callers */

	if (up_interrupt_context()) {
		return OK;
	}

	if ((rtcb->flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_KERNEL) {
		return OK;
	}

	if (rtcb->group == NULL) {
		set_errno(EBADF);
		return ERROR;
	}

	group = rtcb->group;

#ifdef CONFIG_LOG_DUMP
	/* Whitelist: log_dump descriptor shared across groups */

	if (mqdes == g_log_dump_mqdes) {
		return OK;
	}
#endif
	/* Walk group's descriptor list (SMP-safe via critical section) */

	flags = enter_critical_section();

	mqdes_ptr = (mqd_t)sq_peek(&group->tg_msgdesq);
	while (mqdes_ptr) {
		if (mqdes_ptr == mqdes) {
			leave_critical_section(flags);
			return OK;
		}
		mqdes_ptr = (mqd_t)sq_next(mqdes_ptr);
	}

	leave_critical_section(flags);
#ifdef HAVE_GROUP_MEMBERS
	lldbg("mqdes=%p not owned by gid=%d pgid=%d pid=%d\n",
		mqdes, group->tg_gid, group->tg_pgid, rtcb->pid);
#else
	lldbg("mqdes=%p not owned by group, task pid=%d\n", mqdes, rtcb->pid);
#endif
	set_errno(EPERM);
	return ERROR;
}
