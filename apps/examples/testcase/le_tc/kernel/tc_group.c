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
#include <tinyara/kernel_test_drv.h>
#include "tc_internal.h"


/**
 * @fn                           :tc_group_group_add_find_remove_child
 * @brief                        :Add child status structure in given TCB, release a child status structure
 * API's covered                 :group_addchild, group_findchild, group_removechild
 * Preconditions                 :Callback (sched_self) should return valid tcb_s*
 * @return                       :void
 */

static void tc_group_group_add_find_remove_child(void)
{
	int fd;
	int ret_chk;

	fd = tc_get_drvfd();
	ret_chk = ioctl(fd, TESTIOC_GROUP_ADD_FINED_REMOVE_TEST, 0);
	TC_ASSERT_EQ("group_add_find_remove_chil", ret_chk, OK);

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
	int fd;
	int ret_chk;

	fd = tc_get_drvfd();
	ret_chk = ioctl(fd, TESTIOC_GROUP_ALLOC_FREE_TEST, 0);
	TC_ASSERT_EQ("group_alloc_free_child", ret_chk, OK);

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
	int fd;
	int ret_chk;

	fd = tc_get_drvfd();
	ret_chk = ioctl(fd, TESTIOC_GROUP_EXIT_CHILD_TEST, 0);
	TC_ASSERT_EQ("group_exit_child", ret_chk, OK);

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
	int fd;
	int ret_chk;

	fd = tc_get_drvfd();
	ret_chk = ioctl(fd, TESTIOC_GROUP_REMOVECHILDREN_TEST, 0);
	TC_ASSERT_EQ("group_removechildren", ret_chk, OK);

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
