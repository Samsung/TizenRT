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
#include <debug.h>
#include <unistd.h>
#include <errno.h>
#include <sched.h>

#include <tinyara/kernel_test_drv.h>
#include <tinyara/sched.h>
#ifdef CONFIG_SCHED_HAVE_PARENT
#include "group/group.h"
#ifdef CONFIG_SCHED_CHILD_STATUS
#include "task/task.h"
#endif
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

#if defined(CONFIG_SCHED_HAVE_PARENT) && defined(CONFIG_SCHED_CHILD_STATUS)
#define TASK_STACKSIZE 2048
#endif

/****************************************************************************
 * Private Function
 ****************************************************************************/

#if defined(CONFIG_SCHED_HAVE_PARENT) && defined(CONFIG_SCHED_CHILD_STATUS)
static int get_group(struct tcb_s **rtcb, struct task_group_s **group)
{
	*rtcb = sched_self();
	if (*rtcb == NULL) {
		dbg("sched_self failed.");
		return ERROR;
	} else if ((*rtcb)->group == NULL) {
		dbg("group is null.");
		return ERROR;
	}
	*group = (*rtcb)->group;

	return OK;
}

static int group_exitchild_func(int argc, char *argv[])
{
	task_delete(0);
	return ERROR;
}

static int test_group_exit_child(unsigned long arg)
{
	static struct tcb_s *rtcb;
	static struct task_group_s *group;
	static struct child_status_s *child;
	static struct child_status_s *child_returned;
	static pid_t child_pid;
	if (get_group(&rtcb, &group) == ERROR) {
		return ERROR;
	}

	child_pid = kernel_thread("group", SCHED_PRIORITY_DEFAULT, TASK_STACKSIZE, group_exitchild_func, (char *const *)NULL);
	if (child_pid < 0) {
		dbg("kernel_thread failed.");
		return ERROR;
	}

	child = group_findchild(group, child_pid);
	if (child == NULL) {
		dbg("child is null.");
		return ERROR;
	}

	usleep(1);

	child_returned = group_exitchild(group);
	if (child != child_returned) {
		dbg("group_exitchild failed.");
		return ERROR;
	}

	child_returned = group_removechild(group, child_pid);
	if (child != child_returned) {
		dbg("group_removechild failed.");
		return ERROR;
	}

	group_freechild(child);
	return OK;
}

static int test_group_add_fined_remove(unsigned long arg)
{
	static struct tcb_s *rtcb;
	static struct task_group_s *group;
	static struct child_status_s *child;
	static struct child_status_s *child_returned;
	static pid_t child_pid;
	if (get_group(&rtcb, &group) == ERROR) {
		return ERROR;
	}

	child = group_allocchild();
	if (child == NULL) {
		dbg("group_allocchild failed.");
		return ERROR;
	}

	child_pid = -1;
	child->ch_flags = TCB_FLAG_TTYPE_TASK;
	child->ch_pid = child_pid;
	child->ch_status = 0;
	/* Add the entry into the TCB list of children */
	group_addchild(group, child);

	/* cross check starts */
	child_returned = group_findchild(group, child_pid);
	if (child != child_returned) {
		dbg("group_findchild failed.");
		goto errout_with_addchild;
	}

	child_returned = group_removechild(group, child_pid);
	if (child != child_returned) {
		dbg("group_removechild failed.");
		goto errout_with_addchild;
	}

	child_returned = group_findchild(group, child_pid);
	if (child_returned != NULL) {
		dbg("group_removechild failed.");
		goto errout_with_addchild;
	}
	group_freechild(child);
	return OK;

errout_with_addchild:
	group_removechild(group, child_pid);
	group_freechild(child);
	return ERROR;
}

static int test_group_alloc_free(unsigned long arg)
{
	static struct tcb_s *rtcb;
	static struct task_group_s *group;
	static struct child_status_s *child;
	struct child_status_s child_dummy;
	if (get_group(&rtcb, &group) == ERROR) {
		return ERROR;
	}

	child = group_allocchild();
	if (child == NULL) {
		dbg("group_allocchild failed.");
		return ERROR;
	}
	if (child->flink != NULL) {
		dbg("group_allocchild failed.");
		return ERROR;
	}

	child->flink = &child_dummy;
	group_freechild(child);
	if (child->flink == &child_dummy) {
		dbg("group_freechild failed.");
		group_freechild(child);
		return ERROR;
	}
	return OK;
}

static int test_group_removechildren(unsigned long arg)
{
	static struct tcb_s *rtcb;
	static struct task_group_s *group;
	static struct child_status_s *child;
	static struct child_status_s *child_returned;
	static pid_t child_pid;
	if (get_group(&rtcb, &group) == ERROR) {
		return ERROR;
	}

	child = group_allocchild();
	if (child == NULL) {
		dbg("group_allocchild failed.");
		return ERROR;
	}

	child_pid = -1;
	child->ch_flags = TCB_FLAG_TTYPE_TASK;
	child->ch_pid = child_pid;
	child->ch_status = 0;
	/* Add the entry into the TCB list of children */
	group_addchild(group, child);

	/* cross check starts */
	child_returned = group_findchild(group, child_pid);
	if (child != child_returned) {
		dbg("group_findchild failed.");
		goto errout_with_addchild;
	}

	group_removechildren(group);
	if (group->tg_children != NULL) {
		dbg("group_removechildren failed.");
		goto errout_with_addchild;
	}
	group_freechild(child);
	return OK;

errout_with_addchild:
	group_removechildren(group);
	group_freechild(child);
	return ERROR;
}
#endif
/****************************************************************************
 * Public Functions
 ****************************************************************************/

int test_group(int cmd, unsigned long arg)
{
	int ret = -EINVAL;
	switch (cmd) {
#if defined(CONFIG_SCHED_HAVE_PARENT) && defined(CONFIG_SCHED_CHILD_STATUS)
	case TESTIOC_GROUP_ADD_FINED_REMOVE_TEST:
		ret = test_group_add_fined_remove(arg);
		break;
	case TESTIOC_GROUP_ALLOC_FREE_TEST:
		ret = test_group_alloc_free(arg);
		break;
	case TESTIOC_GROUP_EXIT_CHILD_TEST:
		ret = test_group_exit_child(arg);
		break;
	case TESTIOC_GROUP_REMOVECHILDREN_TEST:
		ret = test_group_removechildren(arg);
		break;
#endif
	}
	return ret;
}
