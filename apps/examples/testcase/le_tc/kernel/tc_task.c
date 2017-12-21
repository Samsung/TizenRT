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

/// @file task.c
/// @brief Test Case Example for Task API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "tc_internal.h"

#define TEST_STRING     "test"
#define ONEXIT_VAL      123
#define SEC_1           1
#define SEC_2           2
#define USEC_10         10
#define PR_INVALID      -1
#define PID_INVALID     -1

static int g_callback;
#ifndef CONFIG_BUILD_PROTECTED
static volatile int task_cnt;
static volatile pid_t ppid;
#endif

/**
* @fn                   :create_task
* @brief                :utility function for tc_task_create
* @return               :int
*/
static int create_task(int argc, char *argv[])
{
	if (strcmp(argv[1], TEST_STRING) == 0) {
		g_callback = OK;
	}

	return OK;
}

/**
* @fn                   :delete_task
* @brief                :utility function for tc_task_task_delete
* @return               :int
*/
static int delete_task(int argc, char *argv[])
{
	g_callback = OK;
	task_delete(getpid());

	/* this sleep is for entering callcellation point */

	sleep(SEC_1);
	g_callback = ERROR;
	return OK;
}

/**
* @fn                   :restart_task
* @brief                :utility function for tc_task_task_restart
* @return               :int
*/
static int restart_task(int argc, char *argv[])
{
	g_callback++;
	return OK;
}

/**
* @fn                   :exit_task function
* @brief                :utility function for tc_task_exit
* @return               :void
*/
static int exit_task(int argc, char *argv[])
{
	g_callback = OK;
	exit(OK);
	g_callback = ERROR;
	return OK;
}

/**
* @fn                   :fn_atexit
* @brief                :utility function for tc_task_atexit
* @return               :void
*/
static void fn_atexit(void)
{
	g_callback = OK;
}

/**
* @fn                   :atexit_task
* @brief                :utility function for tc_task_atexit
* @return               :int
*/
static int atexit_task(int argc, char *argv[])
{
	atexit(fn_atexit);
	return OK;
}

/**
* @fn                   :fn_onExit
* @brief                :utility function for tc_task_on_exit
* @return               :void
*/
static void fn_onexit(int status, void *arg)
{
	if (status != ONEXIT_VAL || strcmp((char *)arg, TEST_STRING) != 0) {
		return;
	}

	g_callback = OK;
}

/**
* @fn                   :onexit_task
* @brief                :utility function for tc_task_on_exit
* @return               :int
*/
static int onexit_task(int argc, char *argv[])
{
	if (on_exit(fn_onexit, TEST_STRING) != OK) {
		return ERROR;
	}

	exit(ONEXIT_VAL);
	return OK;
}

/**
* @fn                   :getpid_task
* @brief                :utility function for tc_task_getpid
* @return               :int
*/
static int getpid_task(int argc, char *argv[])
{
	g_callback = (int)getpid();
	return OK;
}

#ifndef CONFIG_BUILD_PROTECTED
/**
* @fn                   :task_cnt_func
* @brief                :handler of sched_foreach for counting the alive tasks and saving the pids
* @return               :void
*/
static void task_cnt_func(struct tcb_s *tcb, void *arg)
{
	task_cnt++;
}

static int vfork_temp_task(int argc, char *argv[])
{
	int rc = 0;

	waitpid(ppid, &rc, 0);
	return OK;
}

/**
* @fn                   :vfork_task function
* @brief                :utility function for tc_task_vfork
* @return               :void
*/
static int vfork_task(int argc, char *argv[])
{
	pid_t pid;
	int vfork_cnt;

	task_cnt = 0;

	ppid = getpid();

	sched_foreach(task_cnt_func, NULL);

	pid = vfork();
	if (pid == 0) {
		exit(OK);
	} else if (pid < 0) {
		g_callback = ERROR;
		return ERROR;
	}

	/* intentionally creates more than CONFIG_MAX_TASKS */
	for (vfork_cnt = 0; vfork_cnt < CONFIG_MAX_TASKS - task_cnt; vfork_cnt++) {
		task_create("tc_vfork_temp", SCHED_PRIORITY_MAX - 1, 1024, vfork_temp_task, (char * const *)NULL);
	}

	pid = vfork();
	if (pid == 0) {
		g_callback = ERROR;
		exit(OK);
	} else if (pid < 0) {
		g_callback = OK;
	}

	return OK;
}
#endif

/**
* @fn                   :tc_task_task_create
* @brief                :Creates a task with the arguments provided like taskname,Priority,stacksize and function name
* @Scenario             :Creates a task with the arguments provided like taskname,Priority,stacksize and function name
* API's covered         :task_create
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_task_task_create(void)
{
	int pid;
	const char *task_param[2] = { TEST_STRING, NULL };
	g_callback = ERROR;

	/* Inavlid priority value check */

	pid = task_create("tc_task_create", SCHED_PRIORITY_MIN - 1, 1024, create_task, (char * const *)task_param);
	TC_ASSERT_EQ("task_create", pid, ERROR);
	TC_ASSERT_EQ("task_create", errno, EINVAL);

	/* Regular functionality check */

	pid = task_create("tc_task_create", SCHED_PRIORITY_MAX - 1, 1024, create_task, (char * const *)task_param);
	TC_ASSERT_GT("task_create", pid, 0);
	TC_ASSERT_EQ("task_create", g_callback, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_task_task_delete
* @brief                :Delete a task
* @Scenario             :Delete a task
* API's covered         :task_create, task_delete
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_task_task_delete(void)
{
	int pid;
	int ret_chk;
	g_callback = ERROR;

	pid = task_create("tc_task_del", SCHED_PRIORITY_MAX - 1, 1024, delete_task, (char * const *)NULL);
	TC_ASSERT_GT("task_create", pid, 0);

	ret_chk = task_delete(pid);
	TC_ASSERT_LT("task_delete", ret_chk, 0);
	TC_ASSERT_EQ("task_delete", g_callback, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_task_task_restart
* @brief                :Restart a task
* @Scenario             :Restart a task
* API's covered         :task_restart
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_task_task_restart(void)
{
	int pid;
	int ret_chk;

	/* Check for NULL pid parameter  */

	pid = task_create("tc_task_re", SCHED_PRIORITY_MAX - 1, 1024, restart_task, (char * const *)NULL);
	TC_ASSERT_GT("task_create", pid, 0);

	g_callback = 0;
	ret_chk = task_restart(0);
	TC_ASSERT_EQ("task_restart", ret_chk, ERROR);
	TC_ASSERT_EQ("task_restart", errno, ENOSYS);
	TC_ASSERT_EQ("task_restart", g_callback, 0);
	g_callback = 0;

	/* Check for reinitialization of task using task_restart */

	pid = task_create("tc_task_re", SCHED_PRIORITY_MAX - 1, 1024, restart_task, (char * const *)NULL);
	TC_ASSERT_GT("task_create", pid, 0);

	ret_chk = task_restart(pid);

	sleep(SEC_1);

	TC_ASSERT_EQ("task_restart", ret_chk, 0);

	/* g_icounter shall be increment when do start and restart operation */

	TC_ASSERT_EQ("task_restart", g_callback, 1);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_task_exit
* @brief                :exit function will terminate the task
* @Scenario             :make a task which calls exit(). after calling exit,
*                       that task cannot do anything such as printf
* API's covered         :on_exit
* Preconditions         :task_create
* Postconditions        :none
* @return               :void
*/
static void tc_task_exit(void)
{
	int pid;
	g_callback = ERROR;
	pid = task_create("tc_exit", SCHED_PRIORITY_MAX - 1, 1024, exit_task, (char * const *)NULL);
	TC_ASSERT_GT("task_create", pid, 0);
	TC_ASSERT_EQ("task_exit", g_callback, OK);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_task_atexit
* @brief                :Register a function to be called at normal process termination
* @Scenario             :Register a function to be called at normal process termination
* API's covered         :atexit
* Preconditions         :none
* Postconditions        :none
* @return               :return SUCCESS on success
*/
static void tc_task_atexit(void)
{
	int pid;
	g_callback = ERROR;
	pid = task_create("tc_atexit", SCHED_PRIORITY_MAX - 1, 1024, atexit_task, (char * const *)NULL);
	TC_ASSERT_GT("task_create", pid, 0);
	TC_ASSERT_EQ("task_atexit", g_callback, OK);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_task_on_exit
* @brief                :on_exit() function registers the given function to be called\
*                        at normal process termination
* @Scenario             :The function is passed the status argument\
*                        given to the last call to exit and the arg argument from on_exit().
* API's covered         :on_exit
* Preconditions         :task_create
* Postconditions        :none
* @return               :void
*/
static void tc_task_on_exit(void)
{
	int pid;
	g_callback = ERROR;

	pid = task_create("tc_on_exit", SCHED_PRIORITY_MAX - 1, 1024, onexit_task, (char * const *)NULL);
	TC_ASSERT_GT("task_create", pid, 0);
	TC_ASSERT_EQ("on_exit", g_callback, OK);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_task_prctl
* @brief                :functions will set and get the process name and then compare the fetched\
*                        name with the set name child task, constructed from a regular executable file.
* @Scenario             :functions will set and get the process name and then compare the fetched\
*                        name with the set name child task, constructed from a regular executable file.
* API's covered         :prctl
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_task_prctl(void)
{
	const char *setname = "a Test program";
	char getname[CONFIG_TASK_NAME_SIZE + 1];
	char oldname[CONFIG_TASK_NAME_SIZE + 1];
	int ret_chk;

	/* unrecognized option case */

	ret_chk = prctl(PR_INVALID, oldname, 0, 0, 0);
	TC_ASSERT_EQ("prctl", ret_chk, ERROR);
	TC_ASSERT_EQ("prctl", get_errno(), EINVAL);

	/* no such process case */

	ret_chk = prctl(PR_GET_NAME, oldname, PID_INVALID, 0, 0);
	TC_ASSERT_EQ("prctl", ret_chk, ERROR);
	TC_ASSERT_EQ("prctl", get_errno(), ESRCH);

	/* no name case */

	ret_chk = prctl(PR_GET_NAME, NULL, 0, 0, 0);
	TC_ASSERT_EQ("prctl", ret_chk, ERROR);
	TC_ASSERT_EQ("prctl", get_errno(), EFAULT);

	/* save taskname */

	ret_chk = prctl(PR_GET_NAME, oldname, 0, 0, 0);
	TC_ASSERT_EQ("prctl", ret_chk, OK);

	/* set taskname */

	ret_chk = prctl(PR_SET_NAME, setname, 0, 0, 0);
	TC_ASSERT_EQ("prctl", ret_chk, OK);

	/* get taskname */

	ret_chk = prctl(PR_GET_NAME, getname, 0, 0, 0);
	TC_ASSERT_EQ_ERROR_CLEANUP("prctl", ret_chk, OK, get_errno(), prctl(PR_SET_NAME, oldname, 0, 0, 0));

	/* compare getname and setname */

	TC_ASSERT_EQ_ERROR_CLEANUP("prctl", strncmp(getname, setname, CONFIG_TASK_NAME_SIZE), 0, get_errno(), prctl(PR_SET_NAME, oldname, 0, 0, 0));

	prctl(PR_SET_NAME, oldname, 0, 0, 0);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_task_getpid
* @brief                :Returns the process ID of the calling process
* @Scenario             :Returns the process ID of the calling process
* API's covered         :getpid
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_task_getpid(void)
{
	int pid;
	g_callback = ERROR;
	pid = task_create("tc_getpid", SCHED_PRIORITY_MAX - 1, 1024, getpid_task, (char * const *)NULL);
	TC_ASSERT_GT("task_create", pid, 0);
	TC_ASSERT_EQ("getpid", pid, g_callback);
	TC_SUCCESS_RESULT();
}

#ifndef CONFIG_BUILD_PROTECTED
/**
* @fn                   :tc_task_vfork
* @brief                :make children through vfork
* @Scenario             :make children above CONFIG_MAX_TASKS through vfork, and check the return value
*                       and check all pids are different of not
* API's covered         :vfork
* Preconditions         :N/A
* Postconditions        :none
* @return               :void
*/
static void tc_task_vfork(void)
{
	int pid;
	g_callback = OK;
	pid = task_create("tc_vfork", SCHED_PRIORITY_MAX - 1, 1024, vfork_task, (char * const *)NULL);
	TC_ASSERT_GT("task_create", pid, 0);
	TC_ASSERT_EQ("vfork", g_callback, OK);

	TC_SUCCESS_RESULT();
}
#endif
/****************************************************************************
 * Name: task
 ****************************************************************************/
int task_main(void)
{
	tc_task_atexit();
	tc_task_exit();
	tc_task_getpid();
	tc_task_on_exit();
	tc_task_prctl();
	tc_task_task_create();
	tc_task_task_delete();
	tc_task_task_restart();
#if defined(CONFIG_ARCH_HAVE_VFORK) && defined(CONFIG_SCHED_WAITPID) && !defined(CONFIG_BUILD_PROTECTED)
	tc_task_vfork();
#endif
	return 0;
}
