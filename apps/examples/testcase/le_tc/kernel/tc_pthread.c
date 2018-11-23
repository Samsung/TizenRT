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

/// @file tc_pthread.c

/// @brief Test Case Example for Pthread API

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sched.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include "../../../../../os/kernel/group/group.h"
#include "tc_internal.h"

#define SEC_1                   1
#define SEC_2                   2
#define SEC_3                   3
#define SEC_5                   5
#define SEC_8                   8
#define VAL_ONE                 1
#define VAL_TWO                 2
#define VAL_THREE               3
#define PTHREAD_CNT             4
#define VAL_TEN                 10
#define SCHED_PRIORITY          100
#define RETURN_PTHREAD_JOIN     ((void *)100)
#define INTHREAD                0
#define INMAIN                  1
#define SIGQUIT                 3
#define NOSIG                   333

struct mallinfo mem;

pthread_t g_thread1;
pthread_t g_thread2;
pthread_t thread[PTHREAD_CNT];

pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_cond;
static pthread_once_t g_once;
static bool g_bpthreadcallback = false;

static int g_cnt;
bool g_maskquitrecv;
bool g_maskusrrecv;

pthread_barrier_t g_pthread_barrier;
int g_barrier_count_in = 0;
int g_barrier_count_out = 0;
int g_barrier_count_spare = 0;

static pthread_mutex_t g_mutex_timedwait;
static pthread_cond_t cond;
static int g_mutex_cnt = 0;

int g_cond_sig_val = 0;

static bool g_sig_handle = false;

pthread_t self_pid;
volatile uint8_t check_prio;

static void *infinite_loop_thread(void *param)
{
	while (1) {
		sleep(1);
	}

	return NULL;
}

/**
* @fn                   :sigmask_handler
* @brief                :utility function used in tc_pthread_pthread_sigmask
* @return               :int
*/
static void sigmask_handler(int signo)
{
	if (signo != SIGQUIT) {
		printf("tc_pthread_pthread_sigmask FAIL sigquit_handler error\n");
		return;
	}
	g_sig_handle = true;
}

/**
* @fn                   :self_test_thread
* @brief                :utility function for tc_pthread_self
* @return               :void*
*/
static void *self_test_thread(void *param)
{
	self_pid = pthread_self();

	pthread_exit(0);
	return NULL;
}

/**
* @fn                   :setschedprio_test_thread
* @brief                :utility function for tc_pthread_setschedprio
* @return               :void*
*/

static void *setschedprio_test_thread(void *param)
{
	struct sched_param param_info;
	int ret_chk;

	/* sleep to guarantee running of pthread_setschedprio() */
	sleep(1);

	/* get current priority */
	ret_chk = sched_getparam(0, &param_info);
	if (ret_chk == ERROR) {
		check_prio = 0;	// Fail to get current priority
	} else {
		/* give getting value to global variable to compare it in another function */
		check_prio = param_info.sched_priority;
	}

	return NULL;
}

/**
* @fn                   :task_barrier
* @brief                :utility function for tc_pthread_pthread_barrier_init_destroy_wait
* @return               :void*
*/
static void *task_barrier(void *param)
{
	int istatus = ERROR;
	pthread_setname_np(0, "task_barrier");
	g_barrier_count_in++;
	istatus = pthread_barrier_wait(&g_pthread_barrier);
	if (istatus == OK || istatus == PTHREAD_BARRIER_SERIAL_THREAD) {
		g_barrier_count_out++;
	} else {
		g_barrier_count_spare++;
	}
	pthread_exit(NULL);
}

/**
* @fn                   :pthread_exit_thread
* @brief                :utility function for tc_pthread_pthread_create_exit_join
* @return               :void*
*/
static void *pthread_exit_thread(void *param)
{
	pthread_exit(RETURN_PTHREAD_JOIN);
	return NULL;
}

/**
* @fn                   :sighand
* @brief                :utility function for thread_kill_func_callback
* @return               :void
*/
static void sighand(int signo)
{
	g_bpthreadcallback = true;
}

/**
* @fn                   :thread_kill_func_callback
* @brief                :utility function for tc_pthread_pthread_kill
* @return               :void*
*/
static void *thread_kill_func_callback(void *param)
{
	sigset_t set;
	struct sigaction act;
	int status;
	(void)sigemptyset(&set);
	(void)sigaddset(&set, SIGUSR1);
	pthread_setname_np(0, "thread_kill_func_callback");
	status = sigprocmask(SIG_UNBLOCK, &set, NULL);
	if (status != OK) {
		tckndbg("sigprocmask failed, status=%d\n", status);
	}
	tcknvdbg("Registering signal handler\n");
	act.sa_sigaction = (void *)sighand;
	act.sa_flags = SA_SIGINFO;
	(void)sigfillset(&act.sa_mask);
	(void)sigdelset(&act.sa_mask, SIGUSR1);

	status = sigaction(SIGUSR1, &act, NULL);
	if (status != OK) {
		tckndbg("sigaction failed, status=%d\n", status);
	}
	sleep(SEC_5);
	act.sa_handler = SIG_DFL;
	(void)sigaction(SIGUSR1, &act, NULL);

	tcknvdbg("done\n");

	pthread_exit((pthread_addr_t)1);
	return NULL;
}

/**
* @fn                   :pthread_wait_callback
* @brief                :utility function for tc_pthread_pthread_cond_broadcast
* @return               :void*
*/
static void *pthread_wait_callback(void *param)
{
	int iret = ERROR;
	pthread_setname_np(0, "pthread_wait_callback");
	pthread_mutex_lock(&g_mutex);
	g_barrier_count_in++;
	iret = pthread_cond_wait(&g_cond, &g_mutex);
	if (iret != OK) {
		printf("pthread_cond_wait FAIL\n");
		g_barrier_count_out--;
	}
	g_barrier_count_out++;
	pthread_mutex_unlock(&g_mutex);
	pthread_exit(NULL);
	return NULL;
}

/**
* @fn                   :threadfunc_sched
* @brief                :utility function for tc_pthread_pthread_set_get_schedparam
* @return               :void*
*/
static void *threadfunc_sched(void *param)
{
	pthread_setname_np(0, "threadfunc_sched");
	sleep(SEC_8);
	pthread_exit((pthread_addr_t)1);
	return NULL;
}

#if CONFIG_NPTHREAD_KEYS > 0
/**
* @fn                   :pthread_key_test
* @brief                :utility function for tc_pthread_pthread_key_create_delete_set_getspecific
* @return               :void*
*/
static void *pthread_key_test(void *param)
{
	int key_index;
	pthread_key_t key_val[PTHREAD_KEYS_MAX + 1];
	int ret_chk;
	int *get_val;

	g_bpthreadcallback = false;
	pthread_setname_np(0, "pthread_key_test");

	/* delete all of pthread key and init key_val with invalid value before testing */
	for (key_index = 0; key_index <= PTHREAD_KEYS_MAX; key_index++) {
		(void)pthread_key_delete(key_index);
		key_val[key_index] = PTHREAD_KEYS_MAX;
	}

	/* valid creation within PTHREAD_KEYS_MAX */
	for (key_index = 0; key_index < PTHREAD_KEYS_MAX; key_index++) {
		ret_chk = pthread_key_create(&key_val[key_index], NULL);
		if (ret_chk != OK) {
			printf("pthread_key_test positive create FAIL! index: %d, ret: %d\n", key_index, ret_chk);
			goto test_out;
		}
	}

	/* invalid creation out of range of key max */
	ret_chk = pthread_key_create(&key_val[PTHREAD_KEYS_MAX], NULL);
	if (ret_chk != EAGAIN) {
		printf("pthread_key_test negative create FAIL! index: %d, ret: %d\n", PTHREAD_KEYS_MAX, ret_chk);
		goto test_out;
	}

	/* set value on each key */
	for (key_index = 0; key_index < PTHREAD_KEYS_MAX; key_index++) {
		ret_chk = pthread_setspecific(key_val[key_index], &key_index);
		if (ret_chk != OK) {
			printf("pthread_key_test setspecific FAIL ret: %d\n", ret_chk);
			goto test_out;
		}
	}

	/* get value */
	for (key_index = 0; key_index < PTHREAD_KEYS_MAX; key_index++) {
		get_val = pthread_getspecific(key_val[key_index]);
		if (*(int *)get_val != key_index) {
			printf("pthread_key_test getspecific FAIL ret val: %d, expected: %d\n", *(int *)get_val, key_index);
			goto test_out;
		}
	}

	/* valid delete key */
	ret_chk = pthread_key_delete(key_val[PTHREAD_KEYS_MAX - 1]);
	if (ret_chk != OK) {
		printf("pthread_key_test delete FAIL! ret: %d\n", ret_chk);
		goto test_out;
	}

	/* invalid delete key */
	ret_chk = pthread_key_delete(PTHREAD_KEYS_MAX + 1);
	if (ret_chk == OK) {
		printf("pthread_key_test delete FAIL! ret: %d\n", ret_chk);
		goto test_out;
	}

	/* valid creation after delete */
	ret_chk = pthread_key_create(&key_val[PTHREAD_KEYS_MAX - 1], NULL);
	if (ret_chk != OK) {
		printf("pthread_key_test create after delete FAIL! ret: %d\n", ret_chk);
		goto test_out;
	}

	g_bpthreadcallback = true;

test_out:
	for (key_index = 0; key_index < PTHREAD_KEYS_MAX; key_index++) {
		(void)pthread_key_delete(key_val[key_index]);
	}
	return NULL;
}
#endif

/**
* @fn                   :cancel_state_func
* @brief                :utilify function for tc_pthread_pthread_cancel_setcancelstate
* @return               :void*
*/
static void *cancel_state_func(void *param)
{
	int ret_chk = 0;
	int prev_val = 0;

	pthread_setname_np(0, "cancel_state_func");
	ret_chk = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_cancel_setcancelstate FAIL, Error No: %d\n", errno);
		return NULL;
	}

	sleep(SEC_3);

	g_bpthreadcallback = true;
	ret_chk = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &prev_val);
	if (ret_chk != OK || prev_val != PTHREAD_CANCEL_DISABLE) {
		printf("tc_pthread_pthread_setcancelstate FAIL, Error No: %d %d %d \n", errno, prev_val, PTHREAD_CANCEL_DISABLE);
		return NULL;
	}
	sleep(SEC_2);
	g_bpthreadcallback = false;
	pthread_exit(NULL);
	return NULL;
}

/**
* @fn                   :run_once
* @brief                :utilify function for tc_pthread_pthread_once
* @return               :void
*/
static void run_once(void)
{
	g_bpthreadcallback = !g_bpthreadcallback;
}

/**
* @fn                   :thread_yieldCallBack
* @brief                :utilify function for tc_pthread_pthread_yield
* @return               :void*
*/
static void *thread_yield_callback(void *arg)
{
	g_bpthreadcallback = true;
	sleep(SEC_3);
	pthread_yield();
	/* yield to another thread, g_bpthreadcallback will remain true in main process */
	g_bpthreadcallback = false;
	pthread_exit((pthread_addr_t)1);
	return NULL;
}

/**
* @fn                   :thread_waiter
* @brief                :utilify function for tc_pthread_pthread_timed_wait
* @return               :void*
*/
static void *thread_waiter(void *parameter)
{
	struct timespec ts;
	int ret_chk;

	pthread_setname_np(0, "thread_waiter");
	/* Take the mutex */
	g_bpthreadcallback = true;
	ret_chk = pthread_mutex_lock(&g_mutex_timedwait);
	if (ret_chk != OK) {
		printf("thread_waiter: ERROR pthread_mutex_lock failed, ret_chk=%d\n", ret_chk);
	}

	ret_chk = clock_gettime(CLOCK_REALTIME, &ts);
	if (ret_chk != OK) {
		printf("thread_waiter: ERROR clock_gettime failed\n");
	}
	ts.tv_sec += SEC_5;

	/* The wait -- no-one is ever going to awaken us */

	ret_chk = pthread_cond_timedwait(&cond, &g_mutex_timedwait, &ts);
	if (ret_chk != OK) {
		if (ret_chk != ETIMEDOUT) {
			g_bpthreadcallback = false;
			printf("thread_waiter: ERROR pthread_cond_timedwait failed, ret_chk=%d\n", ret_chk);
		}
	} else {
		g_bpthreadcallback = false;
		printf("thread_waiter: ERROR pthread_cond_timedwait returned without timeout, ret_chk=%d\n", ret_chk);
	}

	/* Release the mutex */
	ret_chk = pthread_mutex_unlock(&g_mutex_timedwait);
	if (ret_chk != OK) {
		g_bpthreadcallback = false;
		printf("thread_waiter: ERROR pthread_mutex_unlock failed, ret_chk=%d\n", ret_chk);
	}

	pthread_exit((pthread_addr_t)0x12345678);
	return NULL;
}

/**
* @fn                   :thread_mutex
* @brief                :utilify function for tc_pthread_pthread_timed_wait
* @return               :void*
*/
void *thread_mutex(void *arg)
{
	int ret_chk;
	g_bpthreadcallback = true;
	ret_chk = pthread_mutex_lock(&g_mutex);
	if (ret_chk != OK) {
		g_bpthreadcallback = false;
		printf("tc_pthread_pthread_mutex_lock_unlock_trylock  FAIL  Error No: %d\n", errno);
		return NULL;
	}

	int iindex = 0;
	g_mutex_cnt += 1;

	tckndbg("\n Thread %d started\n", g_mutex_cnt);

	for (iindex = 0; iindex < 0x10000000; iindex++);

	tckndbg("\n Thread %d finished\n", g_mutex_cnt);

	ret_chk = pthread_mutex_unlock(&g_mutex);
	if (ret_chk != OK) {
		g_bpthreadcallback = false;
		printf("tc_pthread_pthread_mutex_lock_unlock_trylock  FAIL  Error No: %d\n", errno);
		goto err;
	}

err:
	pthread_exit(NULL);
	return NULL;
}

/**
* @fn                   :thread_cond_signal
* @brief                :utilify function for tc_pthread_pthread_cond_signal
* @return               :void*
*/
void *thread_cond_signal(void *parm)
{
	int ret_chk;

	/* Wait mutex */
	ret_chk = pthread_mutex_lock(&g_mutex);
	if (ret_chk != OK) {
		g_cond_sig_val = ERROR;
		goto err;
	}

	g_cond_sig_val = VAL_TWO;

	/* Wait pthread_cond_signal */
	ret_chk = pthread_cond_wait(&g_cond, &g_mutex);
	if (ret_chk != OK) {
		g_cond_sig_val = ERROR;
		goto err;
	}

	g_cond_sig_val = VAL_THREE;

	ret_chk = pthread_mutex_unlock(&g_mutex);
	if (ret_chk != OK) {
		g_cond_sig_val = ERROR;
		goto err;
	}
err:
	return NULL;
}

/**
* @fn                   :thread_mutex_lock_exit_without_unlock
* @brief                :utility function
* @return               :void*
*/
static void *thread_mutex_lock_exit_without_unlock(void *param)
{
	pthread_mutex_lock(&g_mutex);
	return NULL;
}

/**
* @fn                   :tc_pthread_pthread_barrier_init_destroy_wait
* @brief                :The pthread_barrier_init() function allocates any resources required to use
*                        the barrier referenced by 'barrier' and initialized the barrier with the
*                        attributes referenced by attr.
*                        barrier_wait synchronizse participating threads at the barrier referenced by 'barrier'.
* @Scenario             :The pthread_barrier_init() function allocates any resources required to use
*                        the barrier referenced by 'barrier' and initialized the barrier with the
*                        attributes referenced by attr.
*                        barrier_wait synchronizse participating threads at the barrier referenced by 'barrier'.
*                        destroy reference of created attributes of pthread barrier.
* API's covered         :pthread_barrier_init, pthread_barrier_wait , pthread_barrier_destroy
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_pthread_pthread_barrier_init_destroy_wait(void)
{
	int ret_chk;
	int ithread = PTHREAD_CNT;
	int ithreadid;
	g_barrier_count_in = 0;
	g_barrier_count_out = 0;
	g_barrier_count_spare = 0;

	pthread_t rgthreads[ithread];
	ret_chk = pthread_barrier_init(&g_pthread_barrier, 0, ithread);
	TC_ASSERT_EQ("pthread_barrier_init", ret_chk, OK);

	for (ithreadid = 0; ithreadid < ithread; ithreadid++) {
		ret_chk = pthread_create(&rgthreads[ithreadid], NULL, task_barrier, (void *)ithreadid);
	}

	if (ret_chk != OK) {
		printf("[%s] pthread_create FAIL \n", __func__);
	}
	for (ithreadid = 0; ithreadid < ithread; ithreadid++) {
		ret_chk = pthread_join(rgthreads[ithreadid], 0);
		if (ret_chk != OK) {
			printf("[%s] pthread_join FAIL \n", __func__);
		}
	}

	ret_chk = pthread_barrier_destroy(&g_pthread_barrier);
	TC_ASSERT_EQ("pthread_barrier_destroy", ret_chk, OK);

	TC_ASSERT_EQ("pthread_barrier_init_destroy_wait", g_barrier_count_spare, OK);
	TC_ASSERT_EQ("pthread_barrier_init_destroy_wait", g_barrier_count_in, ithread);
	TC_ASSERT_EQ("pthread_barrier_init_destroy_wait", g_barrier_count_out, ithread);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_pthread_pthread_create_exit_join
* @brief                :creates a new thread with a specified attributes and \
*                        terminates execution of a thread started with pthread_create.
* @Scenario             :creates a new thread with a specified attributes and \
*                        terminates execution of a thread started with pthread_create.
* API's covered         :pthread_create, pthread_exit, pthread_join
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_pthread_pthread_create_exit_join(void)
{
	int ret_chk;
	pthread_t pthread;
	void *p_value = 0;

	ret_chk = pthread_create(&pthread, NULL, pthread_exit_thread, NULL);
	TC_ASSERT_EQ("pthread create", ret_chk, OK);

	/* To make sure thread is created before we join it */
	sleep(SEC_1);

	ret_chk = pthread_join(pthread, &p_value);
	TC_ASSERT_EQ("pthread_join", ret_chk, OK);
	TC_ASSERT_EQ("pthread_join", p_value, RETURN_PTHREAD_JOIN);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_pthread_pthread_tryjoin_np
* @brief                :test the pthread_tryjoin_np function
* @Scenario             :1. creates a new thread with infinite loop
*                        2. call pthread_tryjoin_np
*                        3. cancel thread
*                        4. call pthread_tryjoin_np again
* API's covered         :pthread_create, pthread_cancel, pthread_tryjoin_np
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_pthread_pthread_tryjoin_np(void)
{
	int ret_chk;
	pthread_t pid;
	void *pexit_value = 0;

	ret_chk = pthread_create(&pid, NULL, infinite_loop_thread, NULL);
	TC_ASSERT_EQ("pthread create", ret_chk, OK);

	/* To make sure thread is running */
	sleep(SEC_1);

	ret_chk = pthread_tryjoin_np(pid, NULL);
	TC_ASSERT_EQ("pthread_tryjoin_np", ret_chk, EBUSY);

	ret_chk = pthread_cancel(pid);
	TC_ASSERT_EQ("pthread_cancel", ret_chk, OK);

	/* To make sure thread is terminated */
	sleep(SEC_1);

	ret_chk = pthread_tryjoin_np(pid, &pexit_value);
	TC_ASSERT_EQ("pthread_tryjoin_np", ret_chk, ESRCH);

	ret_chk = pthread_create(&pid, NULL, pthread_exit_thread, NULL);
	TC_ASSERT_EQ("pthread create", ret_chk, OK);

	/* To make sure thread is started and terminated */
	sleep(SEC_1);

	ret_chk = pthread_tryjoin_np(pid, &pexit_value);
	TC_ASSERT_EQ("pthread_tryjoin_np", ret_chk, OK);
	TC_ASSERT_EQ("pthread_tryjoin_np", pexit_value, RETURN_PTHREAD_JOIN);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_pthread_pthread_kill
* @brief                :send a signal to a thread
* @Scenario             :The pthread_kill() function sends the signal sig to thread, a thread
*                        in the same process as the caller.  The signal is asynchronously
*                        directed to thread.
* API's covered         :pthread_kill, pthread_join
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_pthread_pthread_kill(void)
{
	int ret_chk;
	g_bpthreadcallback = false;

	ret_chk = pthread_create(&g_thread1, NULL, thread_kill_func_callback, NULL);
	if (ret_chk != OK) {
		tckndbg("ERROR pthread_create FAIL\n");
		pthread_detach(g_thread1);
	}
	sleep(SEC_2);

	ret_chk = pthread_kill(g_thread1, SIGUSR1);
	sleep(SEC_1);
	TC_ASSERT_EQ_CLEANUP("pthread_kill", ret_chk, OK, pthread_detach(g_thread1));
	TC_ASSERT_EQ_CLEANUP("pthread_kill", g_bpthreadcallback, true, pthread_detach(g_thread1));
	pthread_join(g_thread1, NULL);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_pthread_pthread_cond_broadcast
* @brief                :broadcast or signal a condition
* @Scenario             :function shall unblock all threads currently blocked on the specified condition variable cond.
* API's covered         :pthread_cond_broadcast ,pthread_cond_init , pthread_cond_destroy , pthread_cond_wait
* Preconditions         :pthread_cond_init
* Postconditions        :pthread_cond_destroy
* @return               :void
*/
static void tc_pthread_pthread_cond_broadcast(void)
{
	int ret_chk;
	int ipthread_id = 0;
	pthread_t rgthread[PTHREAD_CNT];
	g_barrier_count_in = 0;
	g_barrier_count_out = 0;
	g_cnt = 0;

	ret_chk = pthread_mutex_init(&g_mutex, NULL);
	TC_ASSERT_EQ("pthread_mutex_init", ret_chk, OK);

	ret_chk = pthread_cond_init(&g_cond, NULL);
	TC_ASSERT_EQ_CLEANUP("pthread_cond_init", ret_chk, OK, pthread_mutex_destroy(&g_mutex));

	for (ipthread_id = 0; ipthread_id < PTHREAD_CNT; ipthread_id++) {
		ret_chk = pthread_create(&rgthread[ipthread_id], NULL, pthread_wait_callback, NULL);
		TC_ASSERT_EQ_CLEANUP("pthread_create", ret_chk, OK, goto cleanup_cond);
	}

	/* To check all thread block conditional lock */
	while (g_barrier_count_in < PTHREAD_CNT) {
		usleep(SEC_1);
	}

	/* To make sure all waiter are currently block on pthread_cond_wait */
	ret_chk = pthread_mutex_lock(&g_mutex);
	TC_ASSERT_EQ_CLEANUP("pthread_mutex_lock", ret_chk, OK, goto cleanup_mutex);

	ret_chk = pthread_mutex_unlock(&g_mutex);
	TC_ASSERT_EQ_CLEANUP("pthread_mutex_unlock", ret_chk, OK, goto cleanup_mutex);

	ret_chk = pthread_cond_broadcast(&g_cond);
	TC_ASSERT_EQ_CLEANUP("pthread_cond_broadcast", ret_chk, OK, goto cleanup_cond);

	sleep(SEC_1);
	TC_ASSERT_GEQ_CLEANUP("pthread_cond_broadcast", g_barrier_count_out, PTHREAD_CNT, goto cleanup_cond);

	/* Wait till terminate all thread */
	for (ipthread_id = 0; ipthread_id < PTHREAD_CNT; ipthread_id++) {
		ret_chk = pthread_join(rgthread[ipthread_id], NULL);
		TC_ASSERT_EQ_CLEANUP("pthread_join", ret_chk, OK, goto cleanup_cond);
	}

	ret_chk = pthread_cond_destroy(&g_cond);
	TC_ASSERT_EQ_CLEANUP("pthread_cond_destroy", ret_chk, OK, pthread_mutex_destroy(&g_mutex));

	ret_chk = pthread_mutex_destroy(&g_mutex);
	TC_ASSERT_EQ("pthread_mutex_destroy", ret_chk, OK);

	TC_SUCCESS_RESULT();
cleanup_mutex:
	pthread_cond_broadcast(&g_cond);
cleanup_cond:
	for (ipthread_id = 0; ipthread_id < PTHREAD_CNT; ++ipthread_id) {
		pthread_cancel(rgthread[ipthread_id]);
		pthread_join(rgthread[ipthread_id], NULL);
	}
	pthread_cond_destroy(&g_cond);
	pthread_mutex_destroy(&g_mutex);
}

/**
* @fn                   :tc_pthread_pthread_cond_init_destroy
* @brief                :pthread_cond_init initialises the condition variable referenced by cond with attributes referenced by attr
*                        pthread_cond_destroy destroy the given condition variable specified by cond
* @Scenario             :pthread_cond_init initialises the condition variable referenced by cond with attributes referenced by attr
*                        pthread_cond_destroy destroy the given condition variable specified by cond
* API's covered         :pthread_cond_init, pthread_cond_destroy
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_pthread_pthread_cond_init_destroy(void)
{
	int ret_chk;
	pthread_condattr_t attr;
	pthread_cond_t cond_nullparam;
	pthread_cond_t cond_attrparam;

	ret_chk = pthread_condattr_init(&attr);
	TC_ASSERT_EQ("pthread_condattr_init", ret_chk, OK);

	/* parameters of pthread_cond_init are of opaque data type hence parameter check not possible */
	ret_chk = pthread_cond_init(&cond_nullparam, NULL);
	TC_ASSERT_EQ("pthread_cond_init", ret_chk, OK);

	ret_chk = pthread_cond_init(&cond_attrparam, &attr);
	TC_ASSERT_EQ("pthread_cond_init", ret_chk, OK);

	ret_chk = pthread_cond_destroy(&cond_nullparam);
	TC_ASSERT_EQ("pthread_cond_destroy", ret_chk, OK);

	ret_chk = pthread_cond_destroy(&cond_attrparam);
	TC_ASSERT_EQ("pthread_cond_destroy", ret_chk, OK);

	ret_chk = pthread_condattr_destroy(&attr);
	TC_ASSERT_EQ("pthread_condattr_destroy", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_pthread_pthread_set_get_schedparam
* @brief                :set/get scheduling policy and parameters of a thread
* @Scenario             :The pthread_setschedparam() function sets the scheduling policy and
*                        parameters of the thread , Policy specifies the new scheduling policy for thread.
*                        The pthread_getschedparam() function returns the scheduling policy and parameters
*                        of the thread thread, in the buffers pointed to by policy and param, respectively.
* API's covered          :pthread_setschedparam, pthread_getschedparam
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_pthread_pthread_set_get_schedparam(void)
{
	int ret_chk;
	struct sched_param st_setparam;
	struct sched_param st_getparam;
	int getpolicy;
	int setpolicy = SCHED_FIFO;

	memset(&st_setparam, 0, sizeof(st_setparam));

	st_setparam.sched_priority = SCHED_PRIORITY;

	ret_chk = pthread_create(&g_thread1, NULL, threadfunc_sched, NULL);
	TC_ASSERT_EQ_CLEANUP("pthread_create", ret_chk, OK, pthread_detach(g_thread1));
	sleep(SEC_2);

	ret_chk = pthread_setschedparam(g_thread1, setpolicy, &st_setparam);
	TC_ASSERT_EQ_CLEANUP("pthread_setschedparam", ret_chk, OK, pthread_detach(g_thread1));

	sleep(SEC_1);

	ret_chk = pthread_getschedparam(g_thread1, &getpolicy, &st_getparam);
	TC_ASSERT_EQ_CLEANUP("pthread_getschedparam", ret_chk, OK, pthread_detach(g_thread1));
	TC_ASSERT_EQ_CLEANUP("pthread_getschedparam", getpolicy, setpolicy, pthread_detach(g_thread1));
	TC_ASSERT_EQ_CLEANUP("pthread_getschedparam", st_getparam.sched_priority, st_setparam.sched_priority, pthread_detach(g_thread1));

	ret_chk = pthread_join(g_thread1, NULL);
	TC_ASSERT_EQ_CLEANUP("pthread_join", ret_chk, OK, pthread_detach(g_thread1));

	TC_SUCCESS_RESULT();
}

#if CONFIG_NPTHREAD_KEYS > 0
/**
* @fn                   :tc_pthread_pthread_key_create_delete_set_getspecific
* @brief                :thread-specific data management
* @Scenario             :The pthread_getspecific() function shall return the value currently bound
*                        to the specified key on behalf of the calling thread.
*                        The pthread_setspecific() function shall associate a thread-specific value
*                        with a key obtained via a previous call to pthread_key_create().
* API's covered         :pthread_setspecific, pthread_getspecific, pthread_key_create, pthread_key_delete
* Preconditions         :pthread_key_create
* Postconditions        :none
* @return               :void
*/
static void tc_pthread_pthread_key_create_delete_set_getspecific(void)
{
	int ret_chk;
	g_bpthreadcallback = false;

	ret_chk = pthread_create(&g_thread1, NULL, pthread_key_test, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);

	ret_chk = pthread_join(g_thread1, NULL);
	TC_ASSERT_EQ_CLEANUP("pthread_join", ret_chk, OK, pthread_detach(g_thread1));

	TC_ASSERT_EQ("pthread_join", g_bpthreadcallback, true);

	TC_SUCCESS_RESULT();
}
#endif

/**
* @fn                   :tc_pthread_pthread_cancel_setcancelstate
* @brief                :set  cancelability state and type and sends a cancellation request to a thread
* @Scenario             :The pthread_setcancelstate() sets the cancelability state of the calling
*                        thread to the value given in state. The pthread_cancel() function sends a cancellation request to the thread thread.
*                        Whether and when the target thread reacts to the cancellation request depends
*                        on two attributes that are under the control of that thread:
*                        its cancelability state and type
* API's covered         :pthread_setcancelstate, pthread_cancel
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_pthread_pthread_cancel_setcancelstate(void)
{
	int ret_chk;
	g_bpthreadcallback = false;

	ret_chk = pthread_create(&g_thread1, NULL, cancel_state_func, NULL);
	TC_ASSERT_EQ_CLEANUP("pthread_create", ret_chk, OK, pthread_detach(g_thread1));

	sleep(SEC_1);
	/* this cancel request goes to pending if cancel is disabled */
	ret_chk = pthread_cancel(g_thread1);
	TC_ASSERT_EQ_CLEANUP("pthread_cancel", ret_chk, OK, pthread_detach(g_thread1));

	sleep(SEC_3);
	TC_ASSERT_EQ("pthread_cancel", g_bpthreadcallback, true);

	ret_chk = pthread_detach(g_thread1);
	TC_ASSERT_EQ("pthread_detach", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_pthread_pthread_timed_wait
* @brief                :function shall block on a condition variable.
* @Scenario             :function is called with mutex locked by the calling thread or undefined behavior results.
* API's covered         :pthread_create, pthread_join, pthread_mutex_init, pthread_mutex_destroy, pthread_cond_init, pthread_cond_destroy, pthread_cond_timedwait
* Preconditions         :none
* Postconditions        :none
* @return               :void
 */
static void tc_pthread_pthread_timed_wait(void)
{
	int ret_chk;
	pthread_t waiter;
	pthread_attr_t attr;
	struct sched_param sparam;
	void *result;
	int prio_max;

	/* Initialize the mutex */
	g_bpthreadcallback = false;
	ret_chk = pthread_mutex_init(&g_mutex_timedwait, NULL);
	TC_ASSERT_EQ("pthread_mutex_init", ret_chk, OK);

	/* Initialize the condition variable */

	ret_chk = pthread_cond_init(&cond, NULL);
	TC_ASSERT_EQ("pthread_cond_init", ret_chk, OK);

	/* Start the waiter thread at higher priority */

	ret_chk = pthread_attr_init(&attr);
	TC_ASSERT_EQ("pthread_attr_init", ret_chk, OK);

	prio_max = sched_get_priority_max(SCHED_FIFO);
	ret_chk = sched_getparam(getpid(), &sparam);
	if (ret_chk != OK) {
		sparam.sched_priority = PTHREAD_DEFAULT_PRIORITY;
	}

	sparam.sched_priority = (prio_max + sparam.sched_priority) / 2;
	ret_chk = pthread_attr_setschedparam(&attr, &sparam);
	TC_ASSERT_EQ("pthread_attr_setschedparam", ret_chk, OK);

	ret_chk = pthread_create(&waiter, &attr, thread_waiter, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);

	TC_ASSERT_EQ("pthread_create", g_bpthreadcallback, true);

	ret_chk = pthread_join(waiter, &result);
	TC_ASSERT_EQ("pthread_join", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_pthread_pthread_mutex_lock_unlock_trylock
* @brief                :The mutex object referenced by mutex is locked/unlocked by calling
*                        pthread_mutex_lock/pthread_mutex_unlock
* @Scenario             :The mutex object referenced by mutex is locked/unlocked by calling
*                        pthread_mutex_lock/pthread_mutex_unlock
* API's covered         :pthread_mutex_lock, pthread_mutex_unlock
* Preconditions         :pthread_mutex_init
* Postconditions        :pthread_mutex_destroy
* @return               :void
*/
static void tc_pthread_pthread_mutex_lock_unlock_trylock(void)
{
	int ret_chk;
	pthread_mutexattr_t attr;
	
	ret_chk = pthread_mutexattr_init(&attr);
	TC_ASSERT_EQ("pthread_mutexattr_init", ret_chk, OK);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

	pthread_mutex_init(&g_mutex, NULL);
	ret_chk = pthread_mutex_lock(&g_mutex);
	TC_ASSERT_EQ("pthread_mutex_lock", ret_chk, OK);

	ret_chk = pthread_mutex_trylock(&g_mutex);
	TC_ASSERT_EQ_CLEANUP("pthread_mutex_trylock", ret_chk, EBUSY, pthread_mutex_unlock(&g_mutex));

	ret_chk = pthread_mutex_unlock(&g_mutex);
	TC_ASSERT_EQ("pthread_mutex_unlock", ret_chk, OK);

	ret_chk = pthread_mutex_trylock(&g_mutex);
	TC_ASSERT_EQ("pthread_mutex_trylock", ret_chk, OK);

	ret_chk = pthread_mutex_unlock(&g_mutex);
	TC_ASSERT_EQ("pthread_mutex_unlock", ret_chk, OK);

	pthread_mutex_destroy(&g_mutex);

	sleep(SEC_2);

#ifdef CONFIG_PTHREAD_MUTEX_TYPES
	/* initalize mutex with PTHREAD_MUTEX_RECURSIVE attribute */
	pthread_mutex_init(&g_mutex, &attr);

	ret_chk = pthread_mutex_lock(&g_mutex);
	TC_ASSERT_EQ("pthread_mutex_lock", ret_chk, OK);

	ret_chk = pthread_mutex_lock(&g_mutex);
	TC_ASSERT_EQ("pthread_mutex_lock", ret_chk, OK);

	ret_chk = pthread_mutex_unlock(&g_mutex);
	TC_ASSERT_EQ("pthread_mutex_unlock", ret_chk, OK);

	ret_chk = pthread_mutex_unlock(&g_mutex);
	TC_ASSERT_EQ("pthread_mutex_unlock", ret_chk, OK);
#endif

	/* mutex_lock mutex_unlock check through multi threads */
	g_mutex_cnt = 0;
	ret_chk = pthread_mutex_init(&g_mutex, NULL);
	TC_ASSERT_EQ("pthread_mutex_init", ret_chk, OK);

	ret_chk = pthread_create(&g_thread1, NULL, &thread_mutex, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);

	ret_chk = pthread_create(&g_thread2, NULL, &thread_mutex, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);

	pthread_join(g_thread1, NULL);
	pthread_join(g_thread2, NULL);

	TC_ASSERT_EQ("pthread_mutex_lock_unlock", g_bpthreadcallback, true);

	pthread_mutex_destroy(&g_mutex);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_pthread_pthread_mutex_init
* @brief                :this tc test pthread_mutex_init
* @Scenario             :If mutex is NULL, pthread_mutex_init return EINVAL.
*                        Otherwise, it return OK and set mutex as default value.
* API's covered         :pthread_mutex_init
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/

static void tc_pthread_pthread_mutex_init(void)
{
	pthread_mutexattr_t attr;
	pthread_mutex_t mutex;
	int ret_chk;

	ret_chk = pthread_mutexattr_init(&attr);
	TC_ASSERT_EQ("pthread_mutexattr_init", ret_chk, OK);

	ret_chk = pthread_mutex_init(NULL, &attr);
	TC_ASSERT_EQ("pthread_mutex_init", ret_chk, EINVAL);

	ret_chk = pthread_mutex_init(&mutex, &attr);
	TC_ASSERT_EQ("pthread_mutex_init", ret_chk, OK);
	TC_ASSERT_EQ("pthread_mutex_init", mutex.pid, -1);
#ifndef CONFIG_PTHREAD_MUTEX_UNSAFE
	TC_ASSERT_EQ("pthread_mutex_init", mutex.flink, NULL);
	TC_ASSERT_EQ("pthread_mutex_init", mutex.flags, (attr.robust == PTHREAD_MUTEX_ROBUST ? _PTHREAD_MFLAGS_ROBUST : 0));
#endif
#ifdef CONFIG_PTHREAD_MUTEX_TYPES
	TC_ASSERT_EQ("pthread_mutex_init", mutex.type, attr.type);
	TC_ASSERT_EQ("pthread_mutex_init", mutex.nlocks, 0);
#endif

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_pthread_pthread_mutex_destroy
* @brief                :this tc test pthread_mutex_destroy
* @Scenario             :1. If mutex is NULL, pthread_mutex_destroy makes assertion or returns EINVAL
*                        2. If an alive thread holds mutex, pthread_mutex_destroy returns EBUSY
*                        3. But the thread is no longer exist, pthread_mutex_destory returns OK and destroys mutex
*                        4. pthread_mutex_destroy returns OK even mutex is already destroyed
* API's covered         :pthread_mutex_destroy
* Preconditions         :none
* Postconditions        :g_mutex will be destroyed
* @return               :void
*/

static void tc_pthread_pthread_mutex_destroy(void)
{
	pthread_mutexattr_t attr;
	pthread_t pid;
	int ret_chk;

	ret_chk = pthread_mutexattr_init(&attr);
	TC_ASSERT_EQ("pthread_mutexattr_init", ret_chk, OK);

	ret_chk = pthread_mutex_init(&g_mutex, &attr);
	TC_ASSERT_EQ("pthread_mutex_init", ret_chk, OK);

#ifndef CONFIG_DEBUG
	/* test NULL case. ASSERT will rise when debug is enabled */

	ret_chk = pthread_mutex_destroy(NULL);
	TC_ASSERT_EQ("pthread_mutex_destroy", ret_chk, EINVAL);
#endif

	/* try pthread_mutex_destroy while mutex is used */

	ret_chk = pthread_mutex_lock(&g_mutex);
	TC_ASSERT_EQ("pthread_mutex_lock", ret_chk, OK);

	ret_chk = pthread_mutex_destroy(&g_mutex);
	TC_ASSERT_EQ("pthread_mutex_destroy", ret_chk, EBUSY);

	ret_chk = pthread_mutex_unlock(&g_mutex);
	TC_ASSERT_EQ("pthread_mutex_unlock", ret_chk, OK);

	/* try ptherad_mutex_destroy when dead thread hold mutex */

	ret_chk = pthread_create(&pid, NULL, thread_mutex_lock_exit_without_unlock, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);

	ret_chk = pthread_join(pid, NULL);
	TC_ASSERT_EQ("pthread_join", ret_chk, OK);

	ret_chk = pthread_mutex_destroy(&g_mutex);
	TC_ASSERT_EQ("pthread_mutex_destroy", ret_chk, OK);
	TC_ASSERT_EQ("pthread_mutex_destroy", g_mutex.pid, -1);

	/* test revisit case */

	ret_chk = pthread_mutex_destroy(&g_mutex);
	TC_ASSERT_EQ("pthread_mutex_destroy", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_pthread_pthread_once
* @brief                :pthread_once() function performs one time initialization based on a specific once_control variable.\
*                        The init_routine is called only one time when multiple calls to pthread_once() use the same once_control.
* @Scenario             :The first call to pthread_once() by any thread in a process, with a given once_control, will call the init_routine() with no arguments.\
*                        Subsequent calls of pthread_once() with the same once_control will not call the init_routine().
* API's covered         :pthread_once
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_pthread_pthread_once(void)
{
	int ret_chk;

	/* Initialize g_once */

	g_once = PTHREAD_ONCE_INIT;

	/* Test NULL case */

	g_bpthreadcallback = false;
	ret_chk = pthread_once(NULL, &run_once);
	TC_ASSERT_EQ("pthread_once", ret_chk, EINVAL);
	TC_ASSERT_EQ("pthread_once", g_bpthreadcallback, false);

	g_bpthreadcallback = false;
	ret_chk = pthread_once(&g_once, NULL);
	TC_ASSERT_EQ("pthread_once", ret_chk, EINVAL);
	TC_ASSERT_EQ("pthread_once", g_bpthreadcallback, false);

	/* g_bpthreadcallback will be toggled only once */

	g_bpthreadcallback = false;
	ret_chk = pthread_once(&g_once, &run_once);
	TC_ASSERT_EQ("pthread_once", ret_chk, OK);
	TC_ASSERT_EQ("pthread_once", g_bpthreadcallback, true);

	g_bpthreadcallback = false;
	ret_chk = pthread_once(&g_once, &run_once);
	TC_ASSERT_EQ("pthread_once", ret_chk, OK);
	TC_ASSERT_EQ("pthread_once", g_bpthreadcallback, false);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_pthread_pthread_yield
* @brief                :The pthread_yield subroutine forces the calling thread to relinquish use of its processor, and to wait in the run queue before it is scheduled again
* @Scenario             :pthread_yield() causes the calling thread to relinquish the CPU.\
*                        The thread is placed at the end of the run queue for its static priority\
*                        and another thread is scheduled to run.
* API's covered         :pthread_yield
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/

static void tc_pthread_pthread_yield(void)
{
	int ret_chk = 0;
	g_bpthreadcallback = false;

	ret_chk = pthread_create(&g_thread1, NULL, thread_yield_callback, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);

	sleep(SEC_1);

	TC_ASSERT_EQ_CLEANUP("pthread_yield", g_bpthreadcallback, true, pthread_detach(g_thread1));

	ret_chk = pthread_create(&g_thread2, NULL, thread_yield_callback, NULL);
	TC_ASSERT_EQ_CLEANUP("pthread_create", ret_chk, OK, pthread_detach(g_thread2));
	TC_ASSERT_EQ_CLEANUP("pthread_create", g_bpthreadcallback, true, pthread_detach(g_thread2));

	/* wait for threads to exit */
	sleep(SEC_5);

	ret_chk = pthread_join(g_thread1, NULL);
	TC_ASSERT_EQ("pthread_join", ret_chk, OK);

	ret_chk = pthread_join(g_thread2, NULL);
	TC_ASSERT_EQ("pthread_join", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_pthread_pthread_cond_signal_wait
* @brief                :The pthread_cond_signal function wakes up at least one thread that is currently waiting on the \
*                        condition variable specified by cond. If no threads are currently blocked on the condition variable, this call has no effect.
* @Scenario             :pthread_cond_signal() Signal Condition to One Waiting Thread.
* API's covered         :pthread_cond_signal, pthread_cond_wait
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_pthread_pthread_cond_signal_wait(void)
{
	pthread_t pthread_waiter;
	int ret_chk = ERROR;
	g_cond_sig_val = VAL_ONE;

	ret_chk = pthread_mutex_init(&g_mutex, NULL);
	TC_ASSERT_EQ("pthread_mutex_init", ret_chk, OK);

	ret_chk = pthread_cond_init(&g_cond, NULL);
	TC_ASSERT_EQ("pthread_cond_init", ret_chk, OK);

	ret_chk = pthread_mutex_lock(&g_mutex);
	TC_ASSERT_EQ("pthread_mutex_lock", ret_chk, OK);

	ret_chk = pthread_create(&pthread_waiter, NULL, thread_cond_signal, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);

	/* if g_cond_sig_val is not matched with VAL_ONE, then TC fails. but we will not use g_mutex anymore, so destroy it */
	TC_ASSERT_EQ_CLEANUP("pthread_mutex_lock", g_cond_sig_val, VAL_ONE, pthread_mutex_destroy(&g_mutex));

	ret_chk = pthread_mutex_unlock(&g_mutex);
	TC_ASSERT_EQ("pthread_mutex_unlock", ret_chk, OK);

	sleep(SEC_1);

	TC_ASSERT_EQ("pthread_cond_signal_wait", g_cond_sig_val, VAL_TWO);

	ret_chk = pthread_cond_signal(&g_cond);
	TC_ASSERT_EQ("pthread_cond_signal", ret_chk, OK);

	sleep(SEC_1);

	TC_ASSERT_EQ("pthread_cond_signal", g_cond_sig_val, VAL_THREE);

	pthread_cancel(pthread_waiter);
	pthread_join(pthread_waiter, NULL);

	TC_SUCCESS_RESULT();
}

static void tc_pthread_pthread_detach(void)
{
	int ret_chk;
	pthread_t new_th;

	/* Create the thread */
	ret_chk = pthread_create(&new_th, NULL, pthread_exit_thread, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);

	/* Detach the non-existant thread. */
	ret_chk = pthread_detach(new_th);
	TC_ASSERT_EQ("pthread_detach", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

static void tc_pthread_pthread_sigmask(void)
{
	int ret_chk;
	pid_t pid = getpid();

	sigset_t st_newmask;
	sigset_t st_oldmask;
	sigset_t st_pendmask;
	struct timespec st_timespec;
	struct sigaction st_act;
	struct sigaction st_oact;

	g_sig_handle = false;

	st_timespec.tv_sec = SEC_5;
	st_timespec.tv_nsec = 0;

	memset(&st_act, 0, sizeof(st_act));
	st_act.sa_handler = sigmask_handler;

	ret_chk = sigaction(SIGQUIT, &st_act, &st_oact);
	TC_ASSERT_EQ("signaction", ret_chk, OK);

	sigemptyset(&st_newmask);
	sigaddset(&st_newmask, SIGQUIT);

	ret_chk = pthread_sigmask(NOSIG, &st_newmask, &st_oldmask);
	TC_ASSERT_EQ("pthread_sigmask", ret_chk, EINVAL);

	ret_chk = pthread_sigmask(SIG_BLOCK, &st_newmask, &st_oldmask);
	TC_ASSERT_GEQ("pthread_sigmask", ret_chk, 0);

	nanosleep(&st_timespec, NULL);

	kill(pid, SIGQUIT);
	/* to call the handler function for verifying the sigpromask */
	ret_chk = pthread_sigmask(SIG_SETMASK, &st_oldmask, NULL);
	TC_ASSERT_GEQ("pthread_sigmask", ret_chk, 0);

	st_timespec.tv_sec = 1;

	nanosleep(&st_timespec, NULL);

	ret_chk = pthread_sigmask(SIG_UNBLOCK, &st_oldmask, NULL);
	TC_ASSERT_GEQ("pthread_sigmask", ret_chk, 0);

	ret_chk = sigpending(&st_pendmask);
	TC_ASSERT_GEQ("sigpending", ret_chk, 0);

	nanosleep(&st_timespec, NULL);
	TC_ASSERT_EQ("pthread_sigmask", g_sig_handle, true);

	ret_chk = sigaction(SIGQUIT, &st_oact, NULL);
	TC_ASSERT_EQ("signaction", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

static void tc_pthread_pthread_self(void)
{
	int ret_chk;
	pthread_t tcb_pid;

	ret_chk = pthread_create(&tcb_pid, NULL, self_test_thread, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);

	pthread_join(tcb_pid, NULL);
	TC_ASSERT_EQ("pthread_self", self_pid, tcb_pid);

	TC_SUCCESS_RESULT();
}

static void tc_pthread_pthread_equal(void)
{
	int ret_chk;
	pthread_t first_th;
	pthread_t second_th;
	bool check_same;

	ret_chk = pthread_create(&first_th, NULL, pthread_exit_thread, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);

	ret_chk = pthread_create(&second_th, NULL, pthread_exit_thread, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);

	pthread_join(first_th, NULL);
	pthread_join(second_th, NULL);

	check_same = pthread_equal(first_th, second_th);
	TC_ASSERT_EQ("pthread_equal", check_same, false);

	TC_SUCCESS_RESULT();
}

static void tc_pthread_pthread_setschedprio(void)
{
	int ret_chk;
	pthread_t set_th;
	uint8_t set_prio = 101;

	ret_chk = pthread_create(&set_th, NULL, setschedprio_test_thread, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);

	/* change set_th PID's priority to set_prio */
	ret_chk = pthread_setschedprio(set_th, set_prio);
	TC_ASSERT_EQ_CLEANUP("pthread_setschedprio", ret_chk, OK, pthread_detach(set_th));

	ret_chk = pthread_join(set_th, NULL);
	TC_ASSERT_EQ_CLEANUP("pthread_join", ret_chk, OK, pthread_detach(set_th));
	TC_ASSERT_EQ("pthread_setschedprio", check_prio, set_prio);

	TC_SUCCESS_RESULT();
}

static void tc_pthread_pthread_setgetname_np(void)
{
	int ret_chk;
	pthread_t name_th;
	char *thread_name = "NameThread";
	char get_name[32];

	ret_chk = pthread_create(&name_th, NULL, infinite_loop_thread, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);

	ret_chk = pthread_setname_np(name_th, "NameThread");
	TC_ASSERT_EQ("pthread_setname_np", ret_chk, OK);

	ret_chk = pthread_getname_np(name_th, get_name);
	TC_ASSERT_EQ("pthread_getname_np", ret_chk, OK);
	TC_ASSERT_EQ("pthread_getname_np", strcmp(get_name, thread_name), 0);

	ret_chk = pthread_cancel(name_th);
	TC_ASSERT_EQ("pthread_cancel", ret_chk, OK);

	ret_chk = pthread_join(name_th, NULL);
	TC_ASSERT_EQ("pthread_join", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_pthread_pthread_setcanceltype
* @brief                :This tc tests pthread_setcanceltype()
* @Scenario             :The function shall atomically both set the calling thread's cancelability type to the indicated type
*                        and return the previous cancelability type at the location referenced by oldtype
*                        If successful pthread_setcanceltype() function shall return zero;
*                        otherwise, an error number shall be returned to indicate the error.
* @API'scovered         :pthread_setcanceltype
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
#ifdef CONFIG_CANCELLATION_POINTS
static void tc_pthread_pthread_setcanceltype(void)
{
	int type;
	int oldtype;
	int ret_chk;

	type = PTHREAD_CANCEL_ASYNCHRONOUS;
	ret_chk = pthread_setcanceltype(type, &oldtype);
	TC_ASSERT_EQ("pthread_setcanceltype", ret_chk, OK);

	type = PTHREAD_CANCEL_DEFERRED;
	ret_chk = pthread_setcanceltype(type, &oldtype);
	TC_ASSERT_EQ("pthread_setcanceltype", ret_chk, ENOSYS);
	TC_ASSERT_EQ("pthread_setcanceltype", oldtype, PTHREAD_CANCEL_ASYNCHRONOUS);

	TC_SUCCESS_RESULT();
}
#endif

/**
* @fn                   :tc_libc_pthread_pthread_testcancel
* @brief                :This tc tests pthread_testcancel()
* @Scenario             :The function shall create a cancellation point in the calling thread
*                        It has no effect if cancelability is disabled.
* @API'scovered         :pthread_testcancel
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
#ifdef CONFIG_CANCELLATION_POINTS
static void tc_pthread_pthread_testcancel(void)
{
	pthread_testcancel();

	TC_SUCCESS_RESULT();
}
#endif

/****************************************************************************
 * Name: pthread
 ****************************************************************************/

int pthread_main(void)
{
	tc_pthread_pthread_barrier_init_destroy_wait();
	tc_pthread_pthread_create_exit_join();
	tc_pthread_pthread_tryjoin_np();
	tc_pthread_pthread_kill();
	tc_pthread_pthread_cond_broadcast();
	tc_pthread_pthread_cond_init_destroy();
	tc_pthread_pthread_set_get_schedparam();
#if CONFIG_NPTHREAD_KEYS > 0
	tc_pthread_pthread_key_create_delete_set_getspecific();
#endif
	tc_pthread_pthread_cancel_setcancelstate();
	tc_pthread_pthread_setschedprio();
	tc_pthread_pthread_timed_wait();
	tc_pthread_pthread_mutex_init();
	tc_pthread_pthread_mutex_destroy();
	tc_pthread_pthread_mutex_lock_unlock_trylock();
	tc_pthread_pthread_once();
	tc_pthread_pthread_yield();
	tc_pthread_pthread_cond_signal_wait();
	tc_pthread_pthread_detach();
	tc_pthread_pthread_sigmask();
	tc_pthread_pthread_self();
	tc_pthread_pthread_equal();
	tc_pthread_pthread_setgetname_np();
#ifdef CONFIG_CANCELLATION_POINTS
	tc_pthread_pthread_setcanceltype();
	tc_pthread_pthread_testcancel();
#endif

	return 0;
}
