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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <debug.h>
#include <sched.h>

#include <tinyara/sched.h>
#include <tinyara/kernel_test_drv.h>

#include "sched/sched.h"
#ifdef CONFIG_SCHED_HAVE_PARENT
#include "group/group.h"
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define TEST_STACK_SIZE (2048)
#define TEST_PRIORITY   (100)
#define TEST_TASK_NAME  ("test_taskinit")

/****************************************************************************
 * Private Function
 ****************************************************************************/

static int test_task_reparent(unsigned long arg)
{
#ifdef CONFIG_SCHED_HAVE_PARENT
	int ret;
	int pid;
	int before_parent_id = 0;
	int after_parent_id = 0;
	struct tcb_s *child_tcb;

	pid = getpid();
	child_tcb = sched_gettcb(pid);
	if (child_tcb == NULL) {
		return ERROR;
	}
#ifdef HAVE_GROUP_MEMBERS
	before_parent_id = child_tcb->group->tg_pgid;
#else
	before_parent_id = child_tcb->group->tg_ppid;
#endif

	ret = task_reparent((int)arg, pid);
	if (ret != OK) {
		return ERROR;
	}

#ifdef HAVE_GROUP_MEMBERS
	after_parent_id = child_tcb->group->tg_pgid;
#else
	after_parent_id = child_tcb->group->tg_ppid;
#endif
	if (before_parent_id == after_parent_id) {
		return ERROR;
	}
#endif
	return OK;
}

static int test_task_init(main_t entry)
{
	struct task_tcb_s *tcb;
	uint32_t *stack;
	int ret;

	tcb = (struct task_tcb_s *)kmm_zalloc(sizeof(struct task_tcb_s));
	if (!tcb) {
		berr("Failed: no memory for tcb\n");
		return -ENOMEM;
	}

	stack = (uint32_t *)kumm_malloc(TEST_STACK_SIZE);
	if (!stack) {
		berr("Failed: no memory for stack\n");
		ret = -ENOMEM;
		goto errout_with_tcb;
	}

	/* positive test */

	ret = task_init((struct tcb_s *)tcb, (const char *)TEST_TASK_NAME, (int)TEST_PRIORITY, stack, TEST_STACK_SIZE, entry, NULL);
	if (ret < 0) {
		berr("Failed: task_init %d\n", ret);
		ret = -get_errno();
		goto errout_with_stack;
	}

	/* Check the TCB values */

	if ((tcb->cmn.sched_priority != TEST_PRIORITY) || (tcb->cmn.stack_alloc_ptr != stack) || (tcb->cmn.entry.main != entry)) {
		berr("Failed: set values, %d, %x, %x %d\n", tcb->cmn.pid, tcb->cmn.stack_alloc_ptr, tcb->cmn.entry.main);
		ret = -ENXIO;
		goto errout_with_task;
	}

	ret = task_activate((FAR struct tcb_s *)tcb);
	if (ret < 0) {
		berr("Failed : task_activate() %d\n", ret);
		ret = -get_errno();
		goto errout_with_task;
	}

	return ret;

errout_with_task:
	sched_removeblocked((struct tcb_s *)tcb);
	sched_releasetcb(&tcb->cmn, TCB_FLAG_TTYPE_TASK);
	return ret;

errout_with_stack:
	kumm_free(stack);

errout_with_tcb:
	kmm_free(tcb);
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int test_task(int cmd, unsigned long arg)
{
	int ret = -EINVAL;
	switch (cmd) {
	case TESTIOC_TASK_REPARENT:
		ret = test_task_reparent(arg);
		break;
	case TESTIOC_TASK_INIT_TEST:
		ret = test_task_init((main_t)arg);
		break;
	}
	return ret;
}
