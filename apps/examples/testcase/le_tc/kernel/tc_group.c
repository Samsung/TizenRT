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

/// @file tc_group.c

/// @brief Test Case Example for Group API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include "../../../../../os/kernel/group/group.h"
#include "tc_internal.h"

#define TASK_STACKSIZE 2048

static int group_exitchild_func(int argc, char *argv[])
{
	task_delete(0);
	return ERROR;
}

/**
 * @fn                           :tc_group_group_add_find_remove_child
 * @brief                        :Add child status structure in given TCB, release a child status structure
 * API's covered                 :group_addchild, group_findchild, group_removechild
 * Preconditions                 :Callback (sched_self) should return valid tcb_s*
 * @return                       :void
 */

static void tc_group_group_add_find_remove_child(void)
{
	struct tcb_s *st_tcb;
	struct task_group_s *group;
	struct child_status_s *child;
	struct child_status_s *child_returned;
	pid_t child_pid;

	st_tcb = sched_self();
	TC_ASSERT_NEQ("sched_self", st_tcb, NULL);

	group = st_tcb->group;
	TC_ASSERT_NEQ("sched_self", group, NULL);

	child = group_allocchild();
	TC_ASSERT_NEQ("group_allocchild", child, NULL);

	child_pid = -1;
	child->ch_flags = TCB_FLAG_TTYPE_TASK;
	child->ch_pid = child_pid;
	child->ch_status = 0;
	/* Add the entry into the TCB list of children */
	group_addchild(group, child);

	/* cross check starts */
	child_returned = group_findchild(group, child_pid);
	TC_ASSERT_EQ("group_findchild", child, child_returned);

	child_returned = group_removechild(group, child_pid);
	TC_ASSERT_EQ("group_removechild", child, child_returned);

	child_returned = group_findchild(group, child_pid);
	TC_ASSERT_EQ_CLEANUP("group_removechild",
						 child_returned,
						 NULL,
						 (group_removechild(group, child_pid), group_freechild(child)));

	group_freechild(child);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                           :tc_group_group_alloc_free_child
 * @brief                        :Alloc child status, and free
 * API's covered                 :group_allocchild, group_freechild
 * Preconditions                 :Callback (sched_self) should return valid tcb_s*
 * @return                       :void
 */
static void tc_group_group_alloc_free_child(void)
{
	struct tcb_s *st_tcb;
	struct task_group_s *group;
	struct child_status_s *child;
	struct child_status_s child_dummy;

	st_tcb = sched_self();
	TC_ASSERT_NEQ("sched_self", st_tcb, NULL);

	group = st_tcb->group;
	TC_ASSERT_NEQ("sched_self", group, NULL);

	child = group_allocchild();
	TC_ASSERT_NEQ("group_allocchild", child, NULL);
	TC_ASSERT_EQ("group_allocchild", child->flink, NULL);

	child->flink = &child_dummy;
	group_freechild(child);
	TC_ASSERT_NEQ("group_allocchild", child->flink, &child_dummy);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                           :tc_group_group_exit_child
 * @brief                        :search for any child that has exited
 * API's covered                 :group_exitchild
 * Preconditions                 :Call (task_getgroup), should return valid task_group *
 * @return                       :void
 */

static void tc_group_group_exit_child(void)
{
	struct tcb_s *st_tcb;
	struct task_group_s *group;
	struct child_status_s *child;
	struct child_status_s *child_returned;
	pid_t child_pid;

	st_tcb = sched_self();
	TC_ASSERT_NEQ("sched_self", st_tcb, NULL);

	group = st_tcb->group;
	TC_ASSERT_NEQ("sched_self", group, NULL);

	child_pid = task_create("group", SCHED_PRIORITY_DEFAULT, TASK_STACKSIZE, group_exitchild_func, (char *const *)NULL);

	child = group_findchild(group, child_pid);
	TC_ASSERT_NEQ("group_findchild", child, NULL);

	sleep(3);

	child_returned = group_exitchild(group);
	TC_ASSERT_EQ("group_exitchild", child, child_returned);

	child_returned = group_removechild(group, child_pid);
	TC_ASSERT_EQ("group_removechild", child, child_returned);

	group_freechild(child);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                           :tc_group_group_removechildren
 * @brief                        :Add child status structure in given TCB, release all child status structure
 * API's covered                 :group_removechildren
 * Preconditions                 :Callback (sched_self) should return valid tcb_s*
 * @return                       :void
 */
static void tc_group_group_removechildren(void)
{
	struct tcb_s *st_tcb;
	struct task_group_s *group;
	struct child_status_s *child;
	struct child_status_s *child_returned;
	pid_t child_pid;

	st_tcb = sched_self();
	TC_ASSERT_NEQ("sched_self", st_tcb, NULL);

	group = st_tcb->group;
	TC_ASSERT_NEQ("sched_self", group, NULL);

	child = group_allocchild();
	TC_ASSERT_NEQ("group_allocchild", child, NULL);

	child_pid = -1;
	child->ch_flags = TCB_FLAG_TTYPE_TASK;
	child->ch_pid = child_pid;
	child->ch_status = 0;
	/* Add the entry into the TCB list of children */
	group_addchild(group, child);

	/* cross check starts */
	child_returned = group_findchild(group, child_pid);
	TC_ASSERT_EQ("group_findchild", child, child_returned);

	group_removechildren(group);
	TC_ASSERT_EQ("group_removechildren", group->tg_children, NULL);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: group
 ****************************************************************************/

int group_main(void)
{
	tc_group_group_add_find_remove_child();
	tc_group_group_alloc_free_child();
	tc_group_group_exit_child();
	tc_group_group_removechildren();

	return 0;
}
