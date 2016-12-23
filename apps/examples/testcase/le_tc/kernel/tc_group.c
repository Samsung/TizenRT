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

/// @file group.c

/// @brief Test Case Example for Group API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include "../../../../../os/kernel/group/group.h"
#include "tc_internal.h"

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
	if (!st_tcb) {
		printf("tc_group_group_add_find_remove_child FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	group = st_tcb->group;
	if (!group) {
		printf("tc_group_group_add_find_remove_child FAIL");
		total_fail++;
		RETURN_ERR;
	}

	child = group_allocchild();
	if (child == NULL) {
		printf("tc_group_group_add_find_remove_child FAIL\n");
		total_fail++;
		RETURN_ERR;
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
		printf("tc_group_group_add_find_remove_child FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	child_returned = group_removechild(group, child_pid);
	if (child != child_returned) {
		printf("tc_group_group_add_find_remove_child FAIL\n");
		total_fail++;
		RETURN_ERR;
	}
	child_returned = group_findchild(group, child_pid);
	if (child_returned != NULL) {
		printf("tc_group_group_add_find_remove_child FAIL\n");
		group_removechild(group, child_pid);
		group_freechild(child);
		total_fail++;
		RETURN_ERR;
	}
	group_freechild(child);

	printf("tc_group_group_add_find_remove_child PASS\n");
	total_pass++;
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
	if (!st_tcb) {
		printf("tc_group_group_alloc_free_child FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	group = st_tcb->group;
	if (!group) {
		printf("tc_group_group_alloc_free_child FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	child = group_allocchild();
	if (child == NULL) {
		printf("tc_group_group_alloc_free_child FAIL\n");
		total_fail++;
		RETURN_ERR;
	}
	if (child->flink != NULL) {
		printf("tc_group_group_alloc_free_child FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	child->flink = &child_dummy;
	group_freechild(child);
	if (child->flink == &child_dummy) {
		printf("tc_group_group_alloc_free_child FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_group_group_alloc_free_child PASS \n");
	total_pass++;
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
	if (!st_tcb) {
		printf("tc_group_group_exit_child FAIL");
		total_fail++;
		RETURN_ERR;
	}

	group = st_tcb->group;
	if (!group) {
		printf("tc_group_group_exit_child FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	child_pid = task_create("group", SCHED_PRIORITY_DEFAULT, CONFIG_USERMAIN_STACKSIZE, group_exitchild_func, (char *const *)NULL);

	child = group_findchild(group, child_pid);
	if (child == NULL) {
		printf("tc_group_group_exit_child FAIL\n");
		total_fail++;
		RETURN_ERR;
	}
	sleep(3);

	child_returned = group_exitchild(group);
	if (child != child_returned) {
		printf("tc_group_group_exit_child FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	child_returned = group_removechild(group, child_pid);
	if (child != child_returned) {
		printf(" tc_group_group_exit_child FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	group_freechild(child);

	printf("tc_group_group_exit_child PASS\n");
	total_pass++;
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
	if (!st_tcb) {
		printf("tc_group_group_removechildren FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	group = st_tcb->group;
	if (!group) {
		printf("tc_group_group_removechildren FAIL");
		total_fail++;
		RETURN_ERR;
	}

	child = group_allocchild();
	if (child == NULL) {
		printf("tc_group_group_removechildren FAIL\n");
		total_fail++;
		RETURN_ERR;
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
		printf("tc_group_group_removechildren FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	group_removechildren(group);
	if (group->tg_children != NULL) {
		printf("tc_group_group_removechildren FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_group_group_removechildren PASS\n");
	total_pass++;
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
