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
#include <unistd.h>
#include "tc_internal.h"

#define USECVAL         1000000
#define TEST_STRING     "test"
#define ONEXIT_VAL      123
#define SEC_2           2
#define SEC_3           3

static int g_icounter = 0;
static bool g_callback = false;
static bool g_exit_val = false;
#ifndef CONFIG_BUILD_PROTECTED
static volatile int vfork_val;
static volatile int task_cnt;
pid_t existed_pid[CONFIG_MAX_TASKS];
static volatile int vfork_cnt;
static volatile pid_t ppid;
static volatile int existed_pid_idx;

/**
* @fn                   :task_cnt_func
* @brief                :handler of sched_foreach for counting the alive tasks and saving the pids
* @return               :void
*/
static void task_cnt_func(struct tcb_s *tcb, void *arg)
{
	existed_pid[task_cnt] = tcb->pid;
	task_cnt++;
}

/**
* @fn                   :vfork_task function
* @brief                :utility function for tc_task_vfork
* @return               :void
*/
static int vfork_task(int argc, char *argv[])
{
	pid_t pid;
	bool result_chk = true;
	int repeat_criteria;
	int repeat_iter;

	/* initialize the task_cnt and existed_pid arr */
	task_cnt = vfork_val = 0;
	for (existed_pid_idx = 0; existed_pid_idx < CONFIG_MAX_TASKS; existed_pid_idx++) {
		existed_pid[existed_pid_idx] = -1;
	}

	ppid = getpid();

	sched_foreach(task_cnt_func, NULL);
	existed_pid_idx = task_cnt;

	for (vfork_cnt = 0; vfork_cnt < (CONFIG_MAX_TASKS - task_cnt + 1); vfork_cnt++) {
		pid = vfork();
		if (pid == 0) {
			vfork_val++;
			/* check that created pid is repeated or not */
			if (existed_pid[existed_pid_idx] != -1) {
				result_chk = false;
				exit(0);
			} else {
				existed_pid[existed_pid_idx++] = getpid();
			}
		} else if (pid < 0) {
			if (vfork_val >= (CONFIG_MAX_TASKS - task_cnt) && errno == EPERM) {
				/* the num of tasks is full, and the errno is set to EPERM */
				break;
			}
			result_chk = false;
		}
	}

	while (vfork_val != CONFIG_MAX_TASKS - task_cnt) {
		usleep(10);
	}

	if (getpid() != ppid) {
		exit(0);
	}

	/* check all pids in existed_pid arr whether repeated or not */
	for (repeat_criteria = 0; repeat_criteria < CONFIG_MAX_TASKS; repeat_criteria++) {
		for (repeat_iter = repeat_criteria + 1; repeat_iter < CONFIG_MAX_TASKS; repeat_iter++) {
			if (existed_pid[repeat_criteria] == existed_pid[repeat_iter]) {
				result_chk = false;
				break;
			}
		}
		if (result_chk == false) {
			break;
		}
	}

	if (result_chk == true) {
		printf("tc_task_vfork PASS\n");
		total_pass++;
	} else {
		printf("tc_task_vfork FAIL\n");
		total_fail++;
	}

	return 0;
}
#endif
/**
* @fn                   :exit_task function
* @brief                :utility function for tc_task_exit
* @return               :void
*/
static int exit_task(int argc, char *argv[])
{
	g_exit_val = true;
	exit(0);
	g_exit_val = false;
	return false;
}

/**
* @fn                   :Fn_exit
* @brief                :utility function for tc_task_atexit
* @return               :void
*/
static void fn_exit(void)
{
	usleep(USECVAL);
}

/**
* @fn                   :temp_task
* @brief                :utility function for tc_task_getpid
* @return               :int
*/
static int temp_task(int argc, char *argv[])
{
	g_callback = true;
	sleep(SEC_2);
	task_delete(getpid());
	return 0;
}

/**
* @fn                   :restart_task
* @brief                :utility function for tc_task_task_restart
* @return               :int
*/
static int restart_task(int argc, char *argv[])
{
	g_icounter++;
	sleep(SEC_3);
	task_delete(getpid());
	return 0;
}

/**
* @fn                   :delete_task
* @brief                :utility function for tc_task_task_delete
* @return               :int
*/
static int delete_task(int argc, char *argv[])
{
	int ret_chk;
	g_callback = true;
	ret_chk = task_delete(getpid());
	if (ret_chk != OK) {
		printf("tc_task_task_delete FAIL : ret_chk is %d\n", ret_chk);
	}
	g_callback = false;
	return 0;
}

/**
* @fn                   :fn_onExit
* @brief                :utility function for tc_task_on_exit
* @return               :void
*/
static void fn_onexit(int status, void *arg)
{
	char *ptr = (char *)arg;

	if (strcmp(ptr, TEST_STRING) != 0 || status != OK) {
		printf("%s FAIL, Error No: %d\n", __func__, errno);
		total_fail++;
	}
	g_callback = true;
}

/**
* @fn                   :onexit_task
* @brief                :utility function for tc_task_on_exit
* @return               :int
*/
static int onexit_task(int argc, char *argv[])
{
	usleep(USECVAL);
	int ret_chk = 0;
	char *ptr = TEST_STRING;
	ret_chk = on_exit(fn_onexit, ptr);
	if (ret_chk != OK) {
		printf("%s FAIL, Error No: %d\n", __func__, errno);
		total_fail++;
		return ERROR;
	}
	task_delete(getpid());
	return ONEXIT_VAL;
}

/**
* @fn                   :atexit_test
* @brief                :utility function for tc_task_atexit
* @return               :int
*/
static int atexit_test(int argc, char *argv[])
{
	int ret_chk;
	ret_chk = atexit(fn_exit);
	if (ret_chk != OK) {
		printf("tc_task_atexit FAIL, Error No: %d\n", errno);
		total_fail++;
		return ERROR;
	}
	printf("tc_task_atexit PASS\n");
	total_pass++;
	task_delete(getpid());
	return OK;
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

	pid = task_create("tc_atexit", SCHED_PRIORITY_DEFAULT, 2048, atexit_test, (char *const *)NULL);
	if (pid < 0) {
		printf("tc_task_atexit FAIL : task_create\n");
		total_fail++;
		RETURN_ERR;
	}
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
	int id;
	pid_t pid;
	id = task_create("tc_getpid", SCHED_PRIORITY_DEFAULT, 2048, temp_task, (char *const *)NULL);
	if (id == 0) {
		pid = getpid();
		if (pid < 0) {
			printf("tc_task_getpid FAIL, Error No: %d\n", errno);
			total_fail++;
			RETURN_ERR;
		}
	}
	printf("tc_task_getpid PASS\n");
	total_pass++;
}

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
	g_callback = false;
	pid = task_create("tc_task_create", SCHED_PRIORITY_DEFAULT, 2048, temp_task, (char *const *)NULL);
	sleep(SEC_2);
	if (pid < 0 || g_callback == false) {
		printf("tc_task_task_create FAIL %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	if (pid == ENOMEM) {
		printf("Not enough memory to create task \n");
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_task_task_create PASS\n");
	total_pass++;
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
	task_create("tc_task_del", SCHED_PRIORITY_DEFAULT, 2048, delete_task, (char *const *)NULL);
	sleep(1);
	if (g_callback == false) {
		printf("tc_task_task_delete FAIL\n");
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_task_task_delete PASS\n");
	total_pass++;
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
	int ret_chk;
	pid_t pid = -1;
	g_icounter = 0;
	pid = task_create("tc_task_re", SCHED_PRIORITY_DEFAULT, 2048, restart_task, (char *const *)NULL);
	sleep(SEC_2);
	ret_chk = task_restart(pid);
	if (ret_chk != OK) {
		printf("tc_task_task_restart FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	sleep(SEC_2);
	/* g_icounter shall be increment when do start and restart operation */
	if (g_icounter != 2) {
		printf("tc_task_task_restart FAIL, unexpected value of function counter %d \n", g_icounter);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_task_task_restart PASS\n");
	total_pass++;
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
	int ret_chk;
	const char *setname = "a Test program";
	char getname[CONFIG_TASK_NAME_SIZE + 1];
	char oldname[CONFIG_TASK_NAME_SIZE + 1];

	/* save taskname */

	ret_chk = prctl(PR_GET_NAME, (unsigned long)oldname, 0, 0, 0);
	if (ret_chk != OK) {
		printf("tc_task_prctl FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	/* set taskname */

	ret_chk = prctl(PR_SET_NAME, (unsigned long)setname, 0, 0, 0);
	if (ret_chk != OK) {
		printf("tc_task_prctl FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	/* get taskname */

	ret_chk = prctl(PR_GET_NAME, (unsigned long)getname, 0, 0, 0);
	if (ret_chk != OK) {
		printf("tc_task_prctl FAIL, Error No: %d\n", errno);
		prctl(PR_SET_NAME, (unsigned long)oldname, 0, 0, 0);
		total_fail++;
		RETURN_ERR;
	}

	/* compare getname and setname */

	if (strncmp(getname, setname, CONFIG_TASK_NAME_SIZE) != 0) {
		printf("tc_task_prctl FAIL, The new process name doesn't match the expected FAIL, Error No: %d\n", errno);
		prctl(PR_SET_NAME, (unsigned long)oldname, 0, 0, 0);
		total_fail++;
		RETURN_ERR;
	}

	prctl(PR_SET_NAME, (unsigned long)oldname, 0, 0, 0);
	printf("tc_task_prctl PASS\n");
	total_pass++;
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
	unsigned int remain;
	g_callback = false;

	pid = task_create("tc_on_exit", SCHED_PRIORITY_DEFAULT, 2048, onexit_task, (char *const *)NULL);
	if (pid == ENOMEM || pid < 0) {
		printf("tc_task_on_exit task_create FAIL, Error No: %d %d\n", errno, pid);
		total_fail++;
		RETURN_ERR;
	}

	remain = sleep(SEC_3);
	while (remain > 0) {
		remain = sleep(remain);
	}

	if (!g_callback) {
		printf("tc_task_on_exit FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_task_on_exit PASS\n");
	total_pass++;
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
	task_create("tc_exit", SCHED_PRIORITY_MAX - 1, 1024, exit_task, (char *const *)NULL);

	if (g_exit_val == true) {
		printf("tc_task_exit PASS\n");
		total_pass++;
	} else {
		printf("tc_task_exit FAIL\n");
		total_fail++;
	}
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
	task_create("tc_vfork", SCHED_PRIORITY_MAX - 1, 1024, vfork_task, (char *const *)NULL);
}
#endif
/****************************************************************************
 * Name: task
 ****************************************************************************/
int task_main(void)
{
	tc_task_task_restart();
	tc_task_atexit();
	tc_task_getpid();
	tc_task_task_create();
	tc_task_task_delete();
	tc_task_prctl();
	tc_task_on_exit();
	tc_task_exit();
#if defined(CONFIG_ARCH_HAVE_VFORK) && defined(CONFIG_SCHED_WAITPID) && !defined(CONFIG_BUILD_PROTECTED)
	tc_task_vfork();
#endif

	return 0;
}
