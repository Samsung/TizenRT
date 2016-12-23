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

/// @file sched.c
/// @brief Test Case Example for Sched API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "../../../../../os/kernel/sched/sched.h"
#include "tc_internal.h"

#define USECVAL         1000000
#define SCHED_PRIORITY  13
#define SLEEPVAL        10000
#define SEC_2           2
#define LOOPCOUNT       2
#define ARRLEN          2
#define VAL_3           3
#define VAL_5           5

pthread_t thread1, thread2;

pid_t g_task_pid;
bool g_callback = false;
bool g_pthread_callback = true;

/**
* @fn                   :sched_foreach_callback
* @description          :Function for tc_sched_sched_foreach
* @return               :void
*/
static void sched_foreach_callback(struct tcb_s *tcb, void *arg)
{
	/* it enumerate every task tcb, that means tcb created for current process will also be enumerated */
	if (tcb->pid == g_task_pid) {
		g_callback = true;
	}
}

/**
* @fn                   :function_wait
* @description          :Function for tc_sched_wait
* @return               :int
*/
static int function_wait(int argc, char *argv[])
{
	usleep(USECVAL);
	task_delete(0);
	return 0;
}

#if defined(CONFIG_SCHED_WAITPID) && defined(CONFIG_SCHED_HAVE_PARENT)
/**
* @fn                   :function_waitlong
* @description          :Function for tc_sched_wait
* @return               :int
*/
static int function_waitlong(int argc, char *argv[])
{
	sleep(SEC_2);
	task_delete(0);
	return 0;
}

/**
* @fn                   :function_waitid
* @description          :Function for tc_sched_waitid
* @return               :int
*/
static int function_waitid(int argc, char *argv[])
{
	usleep(SLEEPVAL);
	task_delete(0);
	return 0;
}
#endif

/**
* @fn                   :threadfunc_callback
* @description          :Function for tc_sched_sched_yield
* @return               :void*
*/
static void *threadfunc_callback(void *param)
{
	g_pthread_callback = true;
	sleep(VAL_3);
	sched_yield();
	pthread_exit((pthread_addr_t)1);
	/* yield to another thread, g_pthread_callback will remain true in main process */
	g_pthread_callback = false;
	return NULL;
}

/**
* @fn                   :tc_sched_sched_setget_scheduler_param
* @brief                :set and get scheduler policies for the named process
* @scenario             :set and get scheduler policies for the named process, sched_getscheduler should return scheduler set
* API's covered         :sched_setscheduler, sched_getscheduler
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_sched_sched_setget_scheduler_param(void)
{
	int ret_val = ERROR;
	struct sched_param st_setparam;
	struct sched_param st_getparam;
	int loop_cnt = LOOPCOUNT;
	int arr_idx = 0;
	int sched_arr[ARRLEN] = { SCHED_RR, SCHED_FIFO };

	while (arr_idx < loop_cnt) {
		st_setparam.sched_priority = SCHED_PRIORITY;
		ret_val = sched_setparam(getpid(), &st_setparam);
		if (ret_val != OK) {
			printf("tc_sched_sched_setget_scheduler_param SETPARAM FAIL, Error No: %d\n", errno);
			total_fail++;
			RETURN_ERR;
		}

		ret_val = sched_setscheduler(getpid(), sched_arr[arr_idx], &st_setparam);
		if (ret_val == ERROR) {
			printf("tc_sched_sched_setget_scheduler_param SETSCHEDULER FAIL1 Error No: %d\n", errno);
			total_fail++;
			RETURN_ERR;
		}

		/* ret_val should be SCHED set */
		ret_val = sched_getscheduler(getpid());
		if (ret_val != sched_arr[arr_idx]) {
			printf("tc_sched_sched_setget_scheduler_param FAIL, Error No: %d\n", errno);
			total_fail++;
			RETURN_ERR;
		}

		ret_val = sched_getparam(getpid(), &st_getparam);
		if (ret_val != OK) {
			printf("tc_sched_sched_setget_scheduler_param FAIL, Error No: %d\n", errno);
			total_fail++;
			RETURN_ERR;
		}

		if (st_setparam.sched_priority != st_getparam.sched_priority) {
			printf("tc_sched_sched_setget_scheduler_param FAIL, Error No: %d\n", errno);
			total_fail++;
			RETURN_ERR;
		}
		arr_idx++;
	}
	printf("tc_sched_sched_setget_scheduler_param PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_sched_sched_rr_get_interval
* @brief                :get  the  SCHED_RR  interval for the named process
* @scenario             :get the SCHED_RR interval for the named process
* API's covered         :sched_rr_get_interval
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_sched_sched_rr_get_interval(void)
{
	int ret_val;
	struct timespec st_timespec1;
	struct timespec st_timespec2;
	st_timespec1.tv_sec = 0;
	st_timespec1.tv_nsec = -1;

	st_timespec2.tv_sec = 0;
	st_timespec2.tv_nsec = -1;
	/* Values are filled in st_timespec structure to differentiate them with values overwritten by rr_interval */
	ret_val = sched_rr_get_interval(0, &st_timespec1);
	if (ret_val == ERROR || st_timespec1.tv_nsec < 0 || st_timespec1.tv_nsec >= 1000000000) {
		printf("tc_sched_sched_rr_get_interval API FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	ret_val = sched_rr_get_interval(getpid(), &st_timespec2);
	if (ret_val == ERROR || st_timespec2.tv_nsec < 0 || st_timespec2.tv_nsec >= 1000000000) {
		printf("tc_sched_sched_rr_get_interval API FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	/* after sched_rr_get_interval() call, st_timespec structure should be overwritten with rr_interval values */
	if ((st_timespec1.tv_sec - st_timespec2.tv_sec) != 0 || (st_timespec1.tv_nsec - st_timespec2.tv_nsec) != 0) {
		printf("tc_sched_sched_rr_get_interval get interval FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_sched_sched_rr_get_interval PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_sched_sched_yield
* @brief                :sched_yield() causes the calling thread to relinquish the CPU.
* @scenario             :sched_yield() causes the calling thread to relinquish the CPU.  The thread is moved
*                        to the end of the queue for its static priority and a new thread gets to run.
* API's covered         :sched_yield
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_sched_sched_yield(void)
{
	int ret_val = 0;

	ret_val = pthread_create(&thread1, NULL, threadfunc_callback, NULL);
	if (ret_val != OK) {
		printf("tc_sched_sched_yield FAIL : pthread_create FAIL!\n");
	}
	if (g_pthread_callback == false) {
		printf("tc_sched_sched_yield FAIL\n");
		total_fail++;
		RETURN_ERR;
	}
	ret_val = pthread_create(&thread2, NULL, threadfunc_callback, NULL);
	if (ret_val != OK) {
		printf("tc_sched_sched_yield FAIL : pthread_create FAIL!\n");
		total_fail++;
		RETURN_ERR;
	}
	if (g_pthread_callback == false) {
		printf("tc_sched_sched_yield FAIL\n");
		total_fail++;
		RETURN_ERR;
	}
	/* wait for threads to exit */
	pthread_join(thread1, 0);
	pthread_join(thread2, 0);
	printf("tc_sched_sched_yield PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_sched_wait
* @brief                :Suspends execution of the current process until one of its children terminates
* @scenario             :wait for state changes in a child of the calling process, and obtain information about
*                        the child whose state has changed. A state change is considered to be: the child
*                        terminated; the child was stopped by a signal; or the child was resumed by a signal
* API's covered         :wait
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
#ifdef CONFIG_SCHED_WAITPID
#ifdef CONFIG_SCHED_HAVE_PARENT
static void tc_sched_wait(void)
{
	int ret_val = 0;
	pid_t child1_pid;
	pid_t child2_pid;
	int status;

	/* creating new process */
	child1_pid = task_create("sched1", SCHED_PRIORITY_DEFAULT, CONFIG_USERMAIN_STACKSIZE, function_wait, (char *const *)NULL);
	child2_pid = task_create("sched2", SCHED_PRIORITY_DEFAULT, CONFIG_USERMAIN_STACKSIZE, function_waitlong, (char *const *)NULL);
	if (child1_pid >= 0 && child2_pid >= 0) {
		/* child which exits first is handled by wait, here child1_pid exits earlier. */
		usleep(SLEEPVAL);
		ret_val = wait(&status);	/* wait for child to exit, and store child's exit status */
		if (ret_val == ERROR) {
			printf("tc_sched_wait FAIL, wait() didn't work Error No: %d\n", errno);
			total_fail++;
			RETURN_ERR;
		}
		if (!(child1_pid == (pid_t)ret_val || child2_pid == (pid_t)ret_val)) {
			printf("tc_sched_wait FAIL, child_pid mismatched to retVal of wait() Error No: %d\n", errno);
			printf("retval is %d\n", ret_val);
			total_fail++;
			RETURN_ERR;
		}
	} else {
		printf("tc_sched_wait FAIL: Child PID < 0. Could not be created, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	/* wait for second child to exit */
	sleep(SEC_2);
	printf("tc_sched_wait PASS \n");
	total_pass++;
}

/**
* @fn                   :tc_sched_waitid
* @brief                :Suspends execution of the current process until one of its children changes state
* @scenario             :provides more precise control over which child state changes to wait for.
* API's covered         :waitid
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/

static void tc_sched_waitid(void)
{
	int ret_val = 0;
	pid_t child_pid;
	siginfo_t info;
	child_pid = task_create("tc_waitid", SCHED_PRIORITY_DEFAULT, CONFIG_USERMAIN_STACKSIZE, function_waitid, (char *const *)NULL);

	if (child_pid >= 0) {
		ret_val = waitid(P_PID, child_pid, &info, WEXITED);
		if (ret_val == ERROR) {
			printf("tc_sched_waitid FAIL: waitid fails, Error No: %d\n", errno);
			total_fail++;
			RETURN_ERR;
		}
		if (info.si_pid != child_pid) {
			printf("tc_sched_waitid FAIL: pid mismatch, Error No: %d\n", errno);
			total_fail++;
			RETURN_ERR;
		}
	} else {
		printf("tc_sched_waitid FAIL: child could not be created, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_sched_waitid PASS\n");
	total_pass++;
}
#endif

/**
* @fn                   :tc_sched_waitpid
* @brief                :Suspends the calling process until a specified process terminates
* @scenario             :The waitpid() system call suspends execution of the calling process until a child
*                        specified by pid argument has changed state. By default, waitpid() waits only for
*                        terminated children
* API's covered         :waitpid
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/

static void tc_sched_waitpid(void)
{
	int ret_val = ERROR;
	pid_t child_pid;
	int *status = (int *)malloc(sizeof(int));
	child_pid = task_create("tc_waitpid", SCHED_PRIORITY_DEFAULT, CONFIG_USERMAIN_STACKSIZE, function_wait, (char *const *)NULL);

	if (child_pid >= 0) {
		ret_val = waitpid(child_pid, status, 0);
		if (ret_val == ERROR) {
			printf("tc_sched_waitpid FAIL: waitpid fails, Error No: %d\n", errno);
			free(status);
			total_fail++;
			RETURN_ERR;
		}
		if (ret_val != child_pid) {
			printf("tc_sched_waitpid FAIL, child_pid mismatched to retVal of waitpid() Error No: %d\n", errno);
			free(status);
			total_fail++;
			RETURN_ERR;
		}
	} else {
		printf("tc_sched_waitpid FAIL: Child PID Could not be created, Error No: %d\n", errno);
		free(status);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_sched_waitpid PASS\n");
	total_pass++;
	free(status);
}
#endif

/**
* @fn                   :tc_sched_sched_gettcb
* @brief                :Given a tsk id, this function will return pointer to corresponding TCB or null if no such task id
* @scenario             :return pointer to corresponding TCB or null for given task id
* API's covered         :gettcb
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_sched_sched_gettcb(void)
{
	struct tcb_s *st_tcb;
	pid_t child_pid;
	child_pid = task_create("tc_gettcb", SCHED_PRIORITY_DEFAULT, CONFIG_USERMAIN_STACKSIZE, function_wait, (char *const *)NULL);
	st_tcb = sched_gettcb(child_pid);
	if (st_tcb == NULL) {
		printf("tc_sched_sched_gettcb API FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	if (st_tcb->pid != child_pid) {
		printf("tc_sched_sched_gettcb FAIL,child_pid mismatch with st_tcb->pid Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_sched_sched_gettcb PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_sched_sched_lock_unlock
* @brief                :sched_lock disables context switching by disabling addition of new tasks,
*                        to the task list and increment lock count, sched_unlock decrements preemption lock count
* @scenario             :sched_lock increments lock count, sched_unlock decrements preemption lock count
* API's covered         :sched_lock, sched_unlock
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/

static void tc_sched_sched_lock_unlock(void)
{
	int ret_val = ERROR;
	int cntlock;
	struct tcb_s *st_tcb = NULL;

	st_tcb = sched_self();
	if (st_tcb == NULL) {
		printf("tc_sched_sched_lock_unlock LOCK FAIL\n");
		total_fail++;
		RETURN_ERR;
	}
	cntlock = st_tcb->lockcount;
	ret_val = sched_lock();
	if (ret_val == ERROR) {
		printf("tc_sched_sched_lock_unlock LOCK FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	/* after sched_lock, lock count gets incremented */
	ret_val = cntlock;
	cntlock = st_tcb->lockcount;
	if (ret_val != cntlock - 1) {
		printf("tc_sched_sched_lock_unlock Lock FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	ret_val = sched_lock();
	if (ret_val == ERROR) {
		printf("tc_sched_sched_lock_unlock Lock FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	/* after sched_lock, lock count gets incremented */
	ret_val = cntlock;
	cntlock = st_tcb->lockcount;
	if (ret_val != cntlock - 1) {
		printf("tc_sched_sched_lock_unlock Lock FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	ret_val = sched_unlock();
	if (ret_val == ERROR) {
		printf("tc_sched_sched_lock_unlock UnLock FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	/* after sched_unlock, lock count gets decremented */
	ret_val = cntlock;
	cntlock = st_tcb->lockcount;
	if (ret_val != cntlock + 1) {
		printf("tc_sched_sched_lock_unlock UnLock FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	ret_val = sched_unlock();
	if (ret_val == ERROR) {
		printf("tc_sched_sched_lock_unlock UnLock FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	/* after sched_unlock, lock count gets decremented */
	ret_val = cntlock;
	cntlock = st_tcb->lockcount;
	if (ret_val != cntlock + 1) {
		printf("tc_sched_sched_lock_unlock UnLock FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_sched_sched_lock_unlock PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_sched_sched_self
* @brief                :Return current thread tcb
* @scenario             :Return current thread tcb structure, verified by getting sched_gettcb(getpid)
* API's covered         :sched_self
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_sched_sched_self(void)
{
	struct tcb_s *st_tcbself;
	struct tcb_s *st_tcbpid;
	/* get process id */

	st_tcbpid = sched_self();
	if (st_tcbpid == NULL) {
		printf("tc_sched_sched_self sched_gettcb FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	/* should return tcb for current process */
	st_tcbself = sched_self();
	if (st_tcbself == NULL) {
		printf("tc_sched_sched_self API FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	if (st_tcbself->pid != st_tcbpid->pid) {
		printf("tc_sched_sched_self FAIL, pid's not matched Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_sched_sched_self PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_sched_sched_verifytcb
* @brief                :Returns true if tcb refers to active task, false if it is state tcb handle
* @scenario             :sched_verifytcb returns true if tcb refers to active task, false if it is state tcb handle
* API's covered         :sched_verifytcb,sched_gettcb
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_sched_sched_verifytcb(void)
{
	bool ret_chk = false;
	struct tcb_s *st_tcb;
	st_tcb = sched_self();
	ret_chk = sched_verifytcb(st_tcb);
	if (ret_chk == false) {
		printf("tc_sched_sched_verifytcb FAIL \n");
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_sched_sched_verifytcb PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_sched_sched_foreach
* @brief                :Enumerate over each task and provide the TCB of each task to a user callback functions.
* @scenario             :provides TCB to user callback function "sched_foreach_callback"
* API's covered         :sched_foreach
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_sched_sched_foreach(void)
{
	g_callback = false;
	struct tcb_s *st_tcb;
	st_tcb = sched_self();
	g_task_pid = st_tcb->pid;
	/* provides TCB to user callback function "sched_foreach_callback" */
	sched_foreach(sched_foreach_callback, NULL);
	if (g_callback != true) {
		printf("tc_sched_sched_foreach FAIL \n");
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_sched_sched_foreach PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_sched_sched_lockcount
* @brief                :sched_lockcount returns the lock count
* @scenario             :after sched_lock and sched_unlock, check the lockcount
* API's covered         :sched_lockcount
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/

static void tc_sched_sched_lockcount(void)
{
	int ret_val = ERROR;
	int prev_cnt;
	int cur_cnt;
	struct tcb_s *st_tcb = NULL;

	st_tcb = sched_self();
	if (st_tcb == NULL) {
		printf("tc_sched_sched_lockcount LOCK FAIL\n");
		total_fail++;
		RETURN_ERR;
	}
	prev_cnt = sched_lockcount();
	ret_val = sched_lock();
	if (ret_val == ERROR) {
		printf("tc_sched_sched_lockcount FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	/* after sched_lock, lock count gets incremented */
	cur_cnt = sched_lockcount();
	if (prev_cnt != cur_cnt-1) {
		printf("tc_sched_sched_lockcount FAIL: not matched\n");
		total_fail++;
		RETURN_ERR;
	}

	prev_cnt = cur_cnt;

	ret_val = sched_unlock();
	if (ret_val == ERROR) {
		printf("tc_sched_sched_lockcount FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	/* after sched_unlock, lock count gets decremented */
	cur_cnt = sched_lockcount();
	if (prev_cnt != cur_cnt+1) {
		printf("tc_sched_sched_lockcount FAIL: not matched\n");
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_sched_sched_lockcount PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_sched_sched_getstreams
* @brief                :return a pointer to the streams list for this thread
* @scenario             :check the streams list for current thread
* API's covered         :sched_getstreams
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/

static void tc_sched_sched_getstreams(void)
{
	struct streamlist *stream;

	stream = sched_getstreams();
	if (stream == NULL) {
		printf("tc_sched_sched_getstreams FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_sched_sched_getstreams PASS\n");
	total_pass++;
}

/****************************************************************************
 * Name: sched
 ****************************************************************************/
int sched_main(void)
{
#ifdef CONFIG_SCHED_WAITPID
#ifdef CONFIG_SCHED_HAVE_PARENT
	tc_sched_wait();
	tc_sched_waitid();
#endif
	tc_sched_waitpid();
#endif
	tc_sched_sched_setget_scheduler_param();
	tc_sched_sched_rr_get_interval();
	tc_sched_sched_yield();
	tc_sched_sched_gettcb();
	tc_sched_sched_lock_unlock();
	tc_sched_sched_self();
	tc_sched_sched_verifytcb();
	tc_sched_sched_foreach();
	tc_sched_sched_lockcount();
	tc_sched_sched_getstreams();

	return 0;
}
