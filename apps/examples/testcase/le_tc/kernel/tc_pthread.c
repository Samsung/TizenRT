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

/// @file pthread.c
/// @brief Test Case Example for Pthread API

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include "../../../../../os/kernel/group/group.h"
#include "../../../../../os/kernel/pthread/pthread.h"
#include "tc_internal.h"

#define SEC_1                   1
#define SEC_2                   2
#define SEC_3                   3
#define SEC_5                   5
#define SEC_8                   8
#define VAL_TWO                 2
#define VAL_THREE               3
#define PTHREAD_CNT             4
#define VAL_TEN                 10
#define SCHED_PRIORITY          100
#define RETURN_PTHREAD_JOIN     ((void *)100)
#define INTHREAD                0
#define INMAIN                  1
#define SIGQUIT                 3

struct mallinfo mem;

pthread_t g_thread1;
pthread_t g_thread2;
pthread_t thread[PTHREAD_CNT];

pthread_mutex_t g_mutex;
pthread_cond_t g_cond;
pthread_once_t g_once = PTHREAD_ONCE_INIT;
pthread_key_t g_tlskey;
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
static int isemaphore;

int g_cond_sig_chk = 0;
bool g_chk_err = false;
bool g_sig_chk = false;
pthread_cond_t g_cond_signal;
pthread_mutex_t g_mutex_cond_signal = PTHREAD_MUTEX_INITIALIZER;

static bool g_sig_handle = false;

pthread_t self_pid;
volatile uint8_t check_prio;

static void *setgetname_thread(void *param)
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
* @fn                   :do_nothing_thread
* @brief                :utility function
* @return               :void*
*/
static void *do_nothing_thread(void *param)
{
	pthread_exit(0);
	return NULL;
}

/**
* @fn                   :setschedprio_test_thread
* @brief                :utility function for tc_pthread_setschedprio
* @return               :void*
*/

#if !defined(CONFIG_BUILD_PROTECTED)
static void *setschedprio_test_thread(void *param)
{
	volatile struct tcb_s *set_tcb;

	/*if this thread's priority is changed, we can terminate the loop */
	while (1) {
		set_tcb = sched_gettcb((pid_t)pthread_self());
		if (set_tcb != NULL && set_tcb->sched_priority == 101) {
			break;
		}
		sleep(1);
	}

	check_prio = set_tcb->sched_priority;
	pthread_exit(0);
	return NULL;
}
#endif
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
* @fn                   :task_exit
* @brief                :utility function for tc_pthread_pthread_create_exit_join
* @return               :void*
*/
static void *task_exit(void *param)
{
	isemaphore = INMAIN;
	pthread_setname_np(0, "task_exit");
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
	act.sa_sigaction = SIG_DFL;
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

/**
* @fn                   :func_set_get_callback
* @brief                :utility function for tc_pthread_pthread_key_create_set_getspecific
* @return               :void*
*/
static void *func_set_get_callback(void *param)
{
	int *pret_chk;
	int ret_chk = 0;
	int check_val = VAL_TEN;
	g_bpthreadcallback = false;
	pthread_setname_np(0, "func_set_get_callback");

	ret_chk = pthread_setspecific(g_tlskey, &check_val);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_key_create_set_getspecific: TC FAIL\n Error No: %d\n", errno);
		return NULL;
	}

	pret_chk = pthread_getspecific(g_tlskey);
	if (*pret_chk != VAL_TEN) {
		printf("tc_pthread_pthread_key_create_set_getspecific: TC FAIL\n Error No: %d\n", errno);
		return NULL;
	}
	sleep(SEC_3);
	g_bpthreadcallback = true;
	pthread_exit((pthread_addr_t)1);
	return NULL;
}

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
* @fn                   :findjoininfo_callback
* @brief                :utilify function for tc_pthread_pthread_findjoininfo_destroyjoin
* @return               :void*
*/
#if !defined(CONFIG_BUILD_PROTECTED)
static void *findjoininfo_callback(void *param)
{
	int ret_chk = 0;
	FAR struct task_group_s *group;
	FAR struct join_s *st_pjoininfo;

	g_bpthreadcallback = true;

	pid_t pid = getpid();
	group = task_getgroup(pid);

	st_pjoininfo = pthread_findjoininfo(group, pid);
	if (st_pjoininfo == NULL) {
		printf("pthread_findjoininfo: Fail \n");
		g_bpthreadcallback = false;
		goto err;
	}
	ret_chk = pthread_equal((pid_t)st_pjoininfo->thread, pid);
	if (ret_chk != 1) {
		printf("tc_pthread_pthread_findjoininfo_destroyjoin pthread_equal fail\n");
		g_bpthreadcallback = false;
		goto err;
	}

	pthread_destroyjoin(group, st_pjoininfo);

	st_pjoininfo = pthread_findjoininfo(group, pid);
	if (st_pjoininfo != NULL) {
		printf("pthread_findjoininfo: st_pjoininfo Fail \n");
		g_bpthreadcallback = false;
		goto err;
	}
err:
	pthread_exit(NULL);
	return NULL;
}
#endif
/**
* @fn                   :run_once
* @brief                :utilify function for tc_pthread_pthread_once
* @return               :void*
*/
static void run_once(void)
{
	static int run_idx = 0;
	g_bpthreadcallback = true;
	if (run_idx != 0) {
		printf("tc_pthread_pthread_once FAIL\n");
		g_bpthreadcallback = false;
		RETURN_ERR;
	}
	run_idx++;
	RETURN_ERR;
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
	int ret;

	pthread_setname_np(0, "thread_waiter");
	/* Take the mutex */
	g_bpthreadcallback = true;
	ret = pthread_mutex_lock(&g_mutex_timedwait);
	if (ret != OK) {
		printf("thread_waiter: ERROR pthread_mutex_lock failed, ret=%d\n", ret);
	}

	ret = clock_gettime(CLOCK_REALTIME, &ts);
	if (ret != OK) {
		printf("thread_waiter: ERROR clock_gettime failed\n");
	}
	ts.tv_sec += SEC_5;

	/* The wait -- no-one is ever going to awaken us */

	ret = pthread_cond_timedwait(&cond, &g_mutex_timedwait, &ts);
	if (ret != OK) {
		if (ret != ETIMEDOUT) {
			g_bpthreadcallback = false;
			printf("thread_waiter: ERROR pthread_cond_timedwait failed, ret=%d\n", ret);
		}
	} else {
		g_bpthreadcallback = false;
		printf("thread_waiter: ERROR pthread_cond_timedwait returned without timeout, ret=%d\n", ret);
	}

	/* Release the mutex */
	ret = pthread_mutex_unlock(&g_mutex_timedwait);
	if (ret != OK) {
		g_bpthreadcallback = false;
		printf("thread_waiter: ERROR pthread_mutex_unlock failed, ret=%d\n", ret);
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
		total_fail++;
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
		total_fail++;
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

	while (!g_chk_err) {
		/* Usually worker threads will loop on these operations */
		ret_chk = pthread_mutex_lock(&g_mutex_cond_signal);
		if (ret_chk != OK) {
			printf("pthread_mutex_lock FAIL \n");
			g_chk_err = true;
			goto err;
		}

		while (!g_cond_sig_chk) {
			g_sig_chk = true;
			/* printf("Thread blocked\n"); */
			ret_chk = pthread_cond_wait(&g_cond_signal, &g_mutex_cond_signal);
			if (g_sig_chk == true) {
				printf(" g_sig_chk should be false \n");
				g_chk_err = true;
				goto err;
			}
			if (ret_chk != OK) {
				printf("pthread_cond_wait FAIL \n");
				g_chk_err = true;
				goto err;
			}
		}
		/* printf("Thread awake, finish work!\n"); */

		g_cond_sig_chk = 0;

		ret_chk = pthread_mutex_unlock(&g_mutex_cond_signal);
		if (ret_chk != OK) {
			printf("pthread_mutex_unlock FAIL \n");
			g_chk_err = true;
			goto err;
		}
	}
err:
	pthread_exit(NULL);
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
	int ithread = PTHREAD_CNT;
	int iret = ERROR;
	int ithreadid;
	g_barrier_count_in = 0;
	g_barrier_count_out = 0;
	g_barrier_count_spare = 0;

	pthread_t rgthreads[ithread];
	iret = pthread_barrier_init(&g_pthread_barrier, 0, ithread);
	if (iret != OK) {
		printf("[%s] pthread_barrier_init FAIL \n", __func__);
		total_fail++;
		RETURN_ERR;
	}

	for (ithreadid = 0; ithreadid < ithread; ithreadid++)
		iret = pthread_create(&rgthreads[ithreadid], NULL, task_barrier, (void *)ithreadid);

	if (iret != OK) {
		printf("[%s] pthread_create FAIL \n", __func__);
	}
	for (ithreadid = 0; ithreadid < ithread; ithreadid++) {
		iret = pthread_join(rgthreads[ithreadid], 0);
		if (iret != OK) {
			printf("[%s] pthread_join FAIL \n", __func__);
		}
	}

	iret = pthread_barrier_destroy(&g_pthread_barrier);
	if (iret != OK) {
		printf("[%s] pthread_barrier_init FAIL \n", __func__);
		total_fail++;
		RETURN_ERR;
	}

	if (g_barrier_count_spare != OK || g_barrier_count_in != ithread || g_barrier_count_out != ithread) {
		printf("[%s] pthread_barrier_init FAIL \n", __func__);
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_pthread_pthread_barrier_init_destroy_wait PASS \n");
	total_pass++;
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

	pthread_t pthread;
	void *p_value = 0;
	isemaphore = ERROR;
	int iret = ERROR;

	iret = pthread_create(&pthread, NULL, task_exit, NULL);
	if (iret != OK) {
		printf("[%s] pthread_create FAIL \n", __func__);
		total_fail++;
		RETURN_ERR;
	}

	/* To make sure thread is created before we join it */
	while (isemaphore == INTHREAD) {
		sleep(SEC_1);
	}

	iret = pthread_join(pthread, &p_value);
	if (iret != OK) {
		printf("[%s] pthread_join FAIL \n", __func__);
		total_fail++;
		RETURN_ERR;
	}

	if (p_value != RETURN_PTHREAD_JOIN) {
		printf("[%s] pthread_join FAIL : did not return pthread_exit() value\n", __func__);
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_pthread_pthread_create_exit_join PASS \n");
	total_pass++;

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
	int status = OK;
	g_bpthreadcallback = false;

	status = pthread_create(&g_thread1, NULL, thread_kill_func_callback, NULL);
	if (status != OK) {
		tckndbg("ERROR pthread_create FAIL\n");
		pthread_detach(g_thread1);
	}
	sleep(SEC_2);

	status = pthread_kill(g_thread1, SIGUSR1);
	sleep(SEC_1);
	if (status != OK || !g_bpthreadcallback) {
		pthread_detach(g_thread1);
		printf("tc_pthread_pthread_kill FAIL\n");
		total_fail++;
	} else {
		pthread_join(g_thread1, NULL);
	}
	printf("tc_pthread_pthread_kill PASS\n");
	total_pass++;
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
	pthread_t rgthread[PTHREAD_CNT];
	int iret = 0;
	int ipthread_id = 0;
	g_barrier_count_in = 0;
	g_barrier_count_out = 0;
	g_cnt = 0;

	iret = pthread_mutex_init(&g_mutex, NULL);
	if (iret != OK) {
		printf("[%s]pthread_mutex_init FAIL,error : %d\n", __func__, iret);
		total_fail++;
		RETURN_ERR;
	}

	iret = pthread_cond_init(&g_cond, NULL);
	if (iret != OK) {
		printf("[%s]pthread_cond_init FAIL,error : %d\n", __func__, iret);
		pthread_mutex_destroy(&g_mutex);
		total_fail++;
		RETURN_ERR;
	}

	for (ipthread_id = 0; ipthread_id < PTHREAD_CNT; ipthread_id++) {
		iret = pthread_create(&rgthread[ipthread_id], NULL, pthread_wait_callback, NULL);
		if (iret != OK) {
			printf("[%s] pthread_create :  %d FAIL\n", __func__, ipthread_id);
			int iindex;
			for (iindex = 0; iindex < ipthread_id; iindex++) {
				pthread_cancel(rgthread[iindex]);
				pthread_join(rgthread[iindex], NULL);
			}
			pthread_cond_destroy(&g_cond);
			pthread_mutex_destroy(&g_mutex);
			total_fail++;
			RETURN_ERR;
		}
	}

	/* To check all thread block conditional lock */
	while (g_barrier_count_in < PTHREAD_CNT) {
		usleep(SEC_1);
	}

	/* To make sure all waiter are currently block on pthread_cond_wait */
	iret = pthread_mutex_lock(&g_mutex);
	if (iret != OK) {
		printf("[%s]pthread_mutex_lock FAIL,error : %d\n", __func__, iret);
		total_fail++;
		pthread_cond_broadcast(&g_cond);
		for (ipthread_id = 0; ipthread_id < PTHREAD_CNT; ++ipthread_id) {
			pthread_cancel(rgthread[ipthread_id]);
			pthread_join(rgthread[ipthread_id], NULL);
		}
		pthread_cond_destroy(&g_cond);
		pthread_mutex_destroy(&g_mutex);
		RETURN_ERR;
	}

	iret = pthread_mutex_unlock(&g_mutex);
	if (iret != OK) {
		pthread_cond_broadcast(&g_cond);
		printf("[%s]pthread_mutex_unlock FAIL,error : %d\n", __func__, iret);
		total_fail++;
		for (ipthread_id = 0; ipthread_id < PTHREAD_CNT; ++ipthread_id) {
			pthread_cancel(rgthread[ipthread_id]);
			pthread_join(rgthread[ipthread_id], NULL);
		}
		pthread_cond_destroy(&g_cond);
		pthread_mutex_destroy(&g_mutex);
		RETURN_ERR;
	}

	iret = pthread_cond_broadcast(&g_cond);
	if (iret != OK) {
		printf("[%s] pthread_cond_broadcast FAIL, error no: %d\n", iret);
		total_fail++;
		for (ipthread_id = 0; ipthread_id < PTHREAD_CNT; ++ipthread_id) {
			pthread_cancel(rgthread[ipthread_id]);
			pthread_join(rgthread[ipthread_id], NULL);
		}
		pthread_cond_destroy(&g_cond);
		pthread_mutex_destroy(&g_mutex);
		RETURN_ERR;
	}

	sleep(SEC_1);

	if (g_barrier_count_out < PTHREAD_CNT) {
		printf("[%s]pthread_cond_broadcast FAIL, unable to release lock for all thread \n", __func__);
		for (ipthread_id = 0; ipthread_id < PTHREAD_CNT; ++ipthread_id) {
			pthread_cancel(rgthread[ipthread_id]);
			pthread_join(rgthread[ipthread_id], NULL);
		}
		pthread_cond_destroy(&g_cond);
		pthread_mutex_destroy(&g_mutex);
		RETURN_ERR;
	}

	/* Wait till terminate all thread */
	for (ipthread_id = 0; ipthread_id < PTHREAD_CNT; ipthread_id++) {
		iret = pthread_join(rgthread[ipthread_id], NULL);
		if (iret != OK) {
			printf("[%s]pthread_join FAIL,error : %d\n", __func__, iret);
		}
	}

	iret = pthread_cond_destroy(&g_cond);
	if (iret != OK) {
		printf("[%s]pthread_cond_destroy FAIL,error : %d\n", __func__, iret);
		pthread_mutex_destroy(&g_mutex);
		total_fail++;
		RETURN_ERR;
	}

	iret = pthread_mutex_destroy(&g_mutex);
	if (iret != OK) {
		printf("[%s]pthread_mutex_destroy FAIL,error : %d\n", __func__, iret);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_pthread_pthread_cond_broadcast PASS\n");
	total_pass++;
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
	int ret_chk = ERROR;
	pthread_condattr_t attr;
	pthread_cond_t cond_nullparam;
	pthread_cond_t cond_attrparam;

	ret_chk = pthread_condattr_init(&attr);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_cond_init_destroy: TC FAIL\n");
		total_fail++;
		RETURN_ERR;
	}
	/* parameters of pthread_cond_init are of opaque data type hence parameter check not possible */
	ret_chk = pthread_cond_init(&cond_nullparam, NULL);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_cond_init_destroy: TC FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	ret_chk = pthread_cond_init(&cond_attrparam, &attr);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_cond_init_destroy: TC FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	ret_chk = pthread_cond_destroy(&cond_nullparam);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_cond_init_destroy: TC FAIL\n");
		total_fail++;
		RETURN_ERR;
	}
	ret_chk = pthread_cond_destroy(&cond_attrparam);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_cond_init_destroy: TC FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	ret_chk = pthread_condattr_destroy(&attr);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_cond_init_destroy: TC FAIL\n");
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_pthread_pthread_cond_init_destroy PASS\n");
	total_pass++;
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
	int ret_chk = 0;
	struct sched_param st_setparam;
	struct sched_param st_getparam;
	int getpolicy;
	int setpolicy = SCHED_FIFO;

	memset(&st_setparam, 0, sizeof(st_setparam));

	st_setparam.sched_priority = SCHED_PRIORITY;

	ret_chk = pthread_create(&g_thread1, NULL, threadfunc_sched, NULL);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_set_get_schedparam: pthread_create FAIL, Error No: %d\n", errno);
		pthread_detach(g_thread1);
		total_fail++;
		RETURN_ERR;
	}

	sleep(SEC_2);

	ret_chk = pthread_setschedparam(g_thread1, setpolicy, &st_setparam);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_set_get_schedparam: pthread_setschedparam FAIL, Error No: %d\n", errno);
		pthread_detach(g_thread1);
		total_fail++;
		RETURN_ERR;
	}

	sleep(SEC_1);

	ret_chk = pthread_getschedparam(g_thread1, &getpolicy, &st_getparam);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_set_get_schedparam: pthread_setschedparam FAIL, Error No: %d\n", errno);
		pthread_detach(g_thread1);
		total_fail++;
		RETURN_ERR;
	}
	if (getpolicy != setpolicy || st_getparam.sched_priority != st_setparam.sched_priority) {
		printf("tc_pthread_pthread_set_get_schedparam: FAIL, Values Mismatch\n");
		pthread_detach(g_thread1);
		total_fail++;
		RETURN_ERR;
	}

	ret_chk = pthread_join(g_thread1, NULL);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_set_get_schedparam: pthread_join FAIL, Error No: %d\n", errno);
		pthread_detach(g_thread1);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_pthread_pthread_set_get_schedparam PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_pthread_pthread_set_getspecific
* @brief                :thread-specific data management
* @Scenario             :The pthread_getspecific() function shall return the value currently bound
*                        to the specified key on behalf of the calling thread.
*                        The pthread_setspecific() function shall associate a thread-specific value
*                        with a key obtained via a previous call to pthread_key_create().
* API's covered         :pthread_setspecific, pthread_getspecific, pthread_key_create
* Preconditions         :pthread_key_create
* Postconditions        :none
* @return               :void
*/
static void tc_pthread_pthread_key_create_set_getspecific(void)
{
	int ret_chk = 0;
	g_bpthreadcallback = false;
	g_tlskey = 0;

	/* Cannot create keys more than PTHREAD_KEYS_MAX, Not able to delete key */
	ret_chk = pthread_key_create(&g_tlskey, NULL);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_key_create_set_getspecific FAIL : pthread_key_create FAIL\n Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	sleep(SEC_2);

	ret_chk = pthread_create(&g_thread1, NULL, func_set_get_callback, NULL);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_key_create_set_getspecific FAIL : pthread_create FAIL\n Error No: %d\n", errno);
		pthread_detach(g_thread1);
		total_fail++;
		RETURN_ERR;
	}

	ret_chk = pthread_join(g_thread1, NULL);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_key_create_set_getspecific FAIL : pthread_create FAIL\n Error No: %d\n", errno);
		pthread_detach(g_thread1);
		total_fail++;
		RETURN_ERR;
	}

	if (g_bpthreadcallback != true) {
		printf("tc_pthread_pthread_key_create_set_getspecific: g_bpthreadcallback FAIL\n ");
		pthread_detach(g_thread1);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_pthread_pthread_key_create_set_getspecific PASS\n");
	total_pass++;
}

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
	int ret_chk = 0;
	g_bpthreadcallback = false;

	ret_chk = pthread_create(&g_thread1, NULL, cancel_state_func, NULL);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_cancel_setcancelstate PTHREAD CREATE_EXIT FAIL, Error No: %d\n", errno);
		pthread_detach(g_thread1);
		total_fail++;
		RETURN_ERR;
	}

	sleep(SEC_1);
	/* this cancel request goes to pending if cancel is disabled */
	ret_chk = pthread_cancel(g_thread1);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_cancel_setcancelstate pthread_cancel FAIL, Error No: %d\n", errno);
		pthread_detach(g_thread1);
		total_fail++;
		RETURN_ERR;
	}

	sleep(SEC_3);

	if (g_bpthreadcallback == false) {
		printf("tc_pthread_pthread_cancel_setcancelstate FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	ret_chk = pthread_detach(g_thread1);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_cancel_setcancelstate pthread_detach FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_pthread_pthread_cancel_setcancelstate PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_pthread_pthread_take_give_semaphore
* @brief                :Support managed access to the private data sets.
* @Scenario             :Support managed access to the private data sets.
* API's covered         :pthread_takesemaphore, pthread_givesemaphore
* Preconditions         :none
* Postconditions        :none
* @return               :void
 */
#if !defined(CONFIG_BUILD_PROTECTED)
static void tc_pthread_pthread_take_give_semaphore(void)
{
	int ret_chk = 0;
	int get_value;
	sem_t sem;
	sem_init(&sem, 0, VAL_THREE);

	ret_chk = pthread_takesemaphore(&sem);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_takesemaphore: Error No %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	sem_getvalue(&sem, &get_value);
	if (get_value != VAL_TWO) {
		printf("tc_pthread_pthread_takesemaphore: Value Mismatch Error No %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	ret_chk = pthread_givesemaphore(&sem);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_givesemaphore: Error No %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	sem_getvalue(&sem, &get_value);
	if (get_value != VAL_THREE) {
		printf("tc_pthread_pthread_takesemaphore: Value Mismatch Error No %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_pthread_pthread_take_give_semaphore PASS\n");
	total_pass++;
}
#endif
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
	pthread_t waiter;
	pthread_attr_t attr;
	struct sched_param sparam;
	void *result;
	int prio_max;
	int ret;

	/* Initialize the mutex */
	g_bpthreadcallback = false;
	ret = pthread_mutex_init(&g_mutex_timedwait, NULL);
	if (ret != OK) {
		printf("tc_pthread_pthread_timed_wait FAIL: ERROR pthread_mutex_init failed\n");
		total_fail++;
		RETURN_ERR;
	}

	/* Initialize the condition variable */

	ret = pthread_cond_init(&cond, NULL);
	if (ret != OK) {
		printf("tc_pthread_pthread_timed_wait FAIL: ERROR pthread_condinit failed\n");
		total_fail++;
		RETURN_ERR;
	}

	/* Start the waiter thread at higher priority */

	ret = pthread_attr_init(&attr);
	if (ret != OK) {
		printf("tc_pthread_pthread_timed_wait FAIL: pthread_attr_init failed\n");
		total_fail++;
		RETURN_ERR;
	}

	prio_max = sched_get_priority_max(SCHED_FIFO);
	ret = sched_getparam(getpid(), &sparam);
	if (ret != OK) {
		sparam.sched_priority = PTHREAD_DEFAULT_PRIORITY;
	}

	sparam.sched_priority = (prio_max + sparam.sched_priority) / 2;
	ret = pthread_attr_setschedparam(&attr, &sparam);
	if (ret != OK) {
		printf("tc_pthread_pthread_timed_wait FAIL: pthread_attr_setschedparam failed, ret=%d\n", ret);
		total_fail++;
		RETURN_ERR;
	}

	ret = pthread_create(&waiter, &attr, thread_waiter, NULL);
	if (ret != OK) {
		printf("tc_pthread_pthread_timed_wait FAIL: pthread_create failed\n");
		total_fail++;
		RETURN_ERR;
	}

	if (g_bpthreadcallback == false) {
		printf("tc_pthread_pthread_timed_wait FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	ret = pthread_join(waiter, &result);
	if (ret != OK) {
		printf("tc_pthread_pthread_timed_wait FAIL: ERROR pthread_join failed\n");
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_pthread_pthread_timed_wait PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_pthread_pthread_findjoininfo_destroyjoin
* @brief                :Find a join structure in a local data set.
*                        pthread_destroyjoin is called from pthread_completejoin if the join info was
*                        detached or from pthread_join when the last waiting thread has received
*                        the thread exit info.
* @scenario             :Find a join structure in a local data set.
*                        pthread_destroyjoin is called from pthread_completejoin if the join info was
*                        detached or from pthread_join when the last waiting thread has received
*                        the thread exit info.
* API's covered         :pthread_findjoininfo, pthread_destroyjoin
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
#if !defined(CONFIG_BUILD_PROTECTED)
static void tc_pthread_pthread_findjoininfo_destroyjoin(void)
{
	int ret_chk = ERROR;
	g_bpthreadcallback = false;

	ret_chk = pthread_create(&g_thread1, NULL, findjoininfo_callback, NULL);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_findjoininfo_destroyjoin fail\n");
		pthread_detach(g_thread1);
		total_fail++;
		RETURN_ERR;
	}

	pthread_join(g_thread1, NULL);

	if (g_bpthreadcallback == false) {
		printf("tc_pthread_pthread_findjoininfo_destroyjoin g_bpthreadcallback fail\n");
		pthread_detach(g_thread1);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_pthread_pthread_findjoininfo_destroyjoin PASS \n");
	total_pass++;
}
#endif
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
	int ret_chk = ERROR;
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

	pthread_mutex_init(&g_mutex, NULL);
	ret_chk = pthread_mutex_lock(&g_mutex);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_mutex_lock_unlock_trylock PTHREAD_MUTEX_DEFAULT FAIL  Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	ret_chk = pthread_mutex_trylock(&g_mutex);
	if (ret_chk != EBUSY) {
		pthread_mutex_unlock(&g_mutex);
		printf("tc_pthread_pthread_mutex_lock_unlock_trylock PTHREAD_MUTEX_DEFAULT FAIL  Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	ret_chk = pthread_mutex_unlock(&g_mutex);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_mutex_lock_unlock_trylock PTHREAD_MUTEX_DEFAULT FAIL  Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	ret_chk = pthread_mutex_trylock(&g_mutex);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_mutex_lock_unlock_trylock PTHREAD_MUTEX_DEFAULT FAIL  Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	ret_chk = pthread_mutex_unlock(&g_mutex);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_mutex_lock_unlock_trylock PTHREAD_MUTEX_DEFAULT FAIL  Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	pthread_mutex_destroy(&g_mutex);

	sleep(SEC_2);

	/* initalize mutex with PTHREAD_MUTEX_RECURSIVE attribute */
	pthread_mutex_init(&g_mutex, &attr);

	ret_chk = pthread_mutex_lock(&g_mutex);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_mutex_lock_unlock_trylock PTHREAD_MUTEX_RECURSIVE FAIL  Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	ret_chk = pthread_mutex_lock(&g_mutex);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_mutex_lock_unlock_trylock PTHREAD_MUTEX_RECURSIVE FAIL  Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	ret_chk = pthread_mutex_unlock(&g_mutex);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_mutex_lock_unlock_trylock PTHREAD_MUTEX_RECURSIVE FAIL  Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	ret_chk = pthread_mutex_unlock(&g_mutex);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_mutex_lock_unlock_trylock PTHREAD_MUTEX_RECURSIVE FAIL  Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	/* mutex_lock mutex_unlock check through multi threads */
	g_mutex_cnt = 0;
	ret_chk = pthread_mutex_init(&g_mutex, NULL);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_mutex_lock_unlock_trylock FAIL: mutex init failed\n");
		total_fail++;
		RETURN_ERR;
	}

	ret_chk = pthread_create(&g_thread1, NULL, &thread_mutex, NULL);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_mutex_lock_unlock_trylock FAIL: pthread_create failed\n");
		total_fail++;
		RETURN_ERR;
	}
	ret_chk = pthread_create(&g_thread2, NULL, &thread_mutex, NULL);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_mutex_lock_unlock_trylock FAIL: pthread_create failed\n");
		total_fail++;
		RETURN_ERR;
	}

	pthread_join(g_thread1, NULL);
	pthread_join(g_thread2, NULL);

	pthread_mutex_destroy(&g_mutex);

	printf("tc_pthread_pthread_mutex_lock_unlock_trylock PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_pthread_pthread_mutex_init_destroy
* @brief                :shall initialize and destroy the mutex referenced by mutex with attributes specified by attr.
* @Scenario             :If successful, the pthread_mutex_destroy() and pthread_mutex_init() functions shall return zero\
*                        otherwise, an error number shall be returned to indicate the error.
* API's covered         :pthread_mutex_init, pthread_mutex_destroy
* Preconditions         :pthread_mutexattr_init
* Postconditions         :pthread_mutexattr_destroy
* @return               :void
*/

static void tc_pthread_pthread_mutex_init_destroy(void)
{
	int ret_chk = ERROR;
	pthread_mutex_t mMutex;
	pthread_mutexattr_t attr;
	int nType = PTHREAD_MUTEX_ERRORCHECK;

	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, nType);

	ret_chk = pthread_mutex_init(&mMutex, &attr);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_mutex_init FAIL  Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	ret_chk = pthread_mutex_lock(&mMutex);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_mutex_init FAIL  Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	pthread_mutex_unlock(&mMutex);

	ret_chk = pthread_mutex_destroy(&mMutex);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_mutex_destroy FAIL  Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	pthread_mutexattr_destroy(&attr);

	/* if Mutex is destoryed, then sem of mutex will be set to 1. */
	if (mMutex.sem.semcount != 1) {
		printf("tc_pthread_pthread_mutex_destroy trylock FAIL Error No: %d %d\n", ret_chk, errno);
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_pthread_pthread_mutex_init_destroy PASS\n");
	total_pass++;
	pthread_mutex_destroy(&mMutex);
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
	int ret_chk = ERROR;
	ret_chk = pthread_once(&g_once, &run_once);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_once FAIL : pthread_create FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	sleep(SEC_1);
	ret_chk = pthread_once(&g_once, &run_once);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_once FAIL : pthread_create FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	sleep(SEC_1);
	if (g_bpthreadcallback != true) {
		printf("tc_pthread_pthread_once FAIL : pthread_create FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_pthread_pthread_once PASS\n");
	total_pass++;
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
	if (ret_chk != OK) {
		printf("tc_pthread_yield FAIL : pthread_create FAIL!\n");
		pthread_detach(g_thread1);
	}
	sleep(SEC_1);
	if (g_bpthreadcallback == false) {
		printf("tc_pthread_pthread_yield FAIL\n");
		pthread_detach(g_thread1);
		total_fail++;
		RETURN_ERR;
	}
	ret_chk = pthread_create(&g_thread2, NULL, thread_yield_callback, NULL);
	if (ret_chk != OK) {
		printf("tc_pthread_yield FAIL : pthread_create FAIL!\n");
		pthread_detach(g_thread2);
		total_fail++;
		RETURN_ERR;
	}
	if (g_bpthreadcallback == false) {
		printf("tc_pthread_pthread_yield FAIL\n");
		pthread_detach(g_thread2);
		total_fail++;
		RETURN_ERR;
	}
	/* wait for threads to exit */
	sleep(SEC_5);

	ret_chk = pthread_join(g_thread1, NULL);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_yield FAIL, pthread_join FAIL: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	ret_chk = pthread_join(g_thread2, NULL);
	if (ret_chk != OK) {
		printf("tc_pthread_pthread_yield FAIL, pthread_join FAIL: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_pthread_pthread_yield PASS\n");
	total_pass++;
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
	pthread_t pthread_1;
	pthread_t pthread_2;
	pthread_cond_init(&g_cond_signal, NULL);
	g_sig_chk = false;
	g_chk_err = false;
	int iret = ERROR;
	int indexloop = 0;

	iret = pthread_create(&pthread_1, NULL, thread_cond_signal, NULL);
	if (iret != OK) {
		printf("[%s] tc_pthread_pthread_cond_signal_wait FAIL \n", __func__);
		total_fail++;
		RETURN_ERR;
	}
	iret = pthread_create(&pthread_2, NULL, thread_cond_signal, NULL);
	if (iret != OK) {
		printf("[%s] tc_pthread_pthread_cond_signal_wait FAIL \n", __func__);
		total_fail++;
		RETURN_ERR;
	}
	sleep(SEC_3);

	for (indexloop = 0; indexloop < VAL_THREE; ++indexloop) {
		/* printf("Wake up a worker, work to do...\n"); */
		if (g_sig_chk == false) {
			printf("[%s] tc_pthread_pthread_cond_signal_wait FAIL \n", __func__);
			total_fail++;
			RETURN_ERR;
		}
		iret = pthread_mutex_lock(&g_mutex_cond_signal);
		if (iret != OK) {
			printf("[%s] tc_pthread_pthread_cond_signal_wait FAIL \n", __func__);
			total_fail++;
			RETURN_ERR;
		}

		if (g_cond_sig_chk) {
			tckndbg("Work already present, likely threads are busy\n");
		}
		g_cond_sig_chk = 1;
		g_sig_chk = false;
		iret = pthread_cond_signal(&g_cond_signal);
		if (iret != OK) {
			pthread_mutex_unlock(&g_mutex_cond_signal);
			printf("[%s] tc_pthread_pthread_cond_signal_wait FAIL \n", __func__);
			total_fail++;
			RETURN_ERR;
		}

		iret = pthread_mutex_unlock(&g_mutex_cond_signal);
		if (iret != OK) {
			printf("[%s] tc_pthread_pthread_cond_signal_wait FAIL \n", __func__);
			total_fail++;
			RETURN_ERR;
		}
		sleep(SEC_3);
	}

	if (g_chk_err == true) {
		printf("[%s] tc_pthread_pthread_cond_signal_wait FAIL, g_chk_err is true \n", __func__);
		total_fail++;
		RETURN_ERR;
	}
	g_chk_err = true;
	pthread_cancel(pthread_1);
	pthread_cancel(pthread_2);
	pthread_join(pthread_1, NULL);
	pthread_join(pthread_2, NULL);
	printf("tc_pthread_pthread_cond_signal_wait PASS\n");
	total_pass++;
}

static void tc_pthread_pthread_detach(void)
{
	pthread_t new_th;
	int ret;

	/* Create the thread */
	if (pthread_create(&new_th, NULL, do_nothing_thread, NULL) != 0) {
		printf("tc_pthread_pthread_detach FAIL : pthread_create\n");
		total_fail++;
		RETURN_ERR;
	}

	/* Wait 'till the thread returns.
	 * The thread could have ended by the time we try to join, so
	 * don't worry about it, just so long as other errors don't
	 * occur. The point is to make sure the thread has ended execution. */
	if (pthread_join(new_th, NULL) == EDEADLK) {
		printf("tc_pthread_pthread_detach FAIL : pthread_join\n");
		total_fail++;
		RETURN_ERR;
	}

	/* Detach the non-existant thread. */
	ret = pthread_detach(new_th);

	/* Check return value of pthread_detach() */
	if (ret == OK) {
		printf("tc_pthread_pthread_detach FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	total_pass++;
	printf("tc_pthread_pthread_detach PASS\n");
}

static void tc_pthread_pthread_sigmask(void)
{
	int ret_chk = ERROR;
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

	if (sigaction(SIGQUIT, &st_act, &st_oact)) {
		printf("tc_pthread_pthread_sigmask FAIL : sigaction error\n");
		total_fail++;
		RETURN_ERR;
	}
	sigemptyset(&st_newmask);
	sigaddset(&st_newmask, SIGQUIT);

	ret_chk = pthread_sigmask(SIG_BLOCK, &st_newmask, &st_oldmask);
	if (ret_chk < 0) {
		printf("tc_pthread_pthread_sigmask FAIL SIG_BLOCK error\n");
		total_fail++;
		RETURN_ERR;
	}

	nanosleep(&st_timespec, NULL);

	kill(pid, SIGQUIT);
	/* to call the handler function for verifying the sigpromask */
	ret_chk = pthread_sigmask(SIG_SETMASK, &st_oldmask, NULL);
	if (ret_chk < 0) {
		printf("tc_pthread_pthread_sigmask FAIL SIG_SETMASK error\n");
		total_fail++;
		RETURN_ERR;
	}

	st_timespec.tv_sec = 1;

	nanosleep(&st_timespec, NULL);

	ret_chk = pthread_sigmask(SIG_UNBLOCK, &st_oldmask, NULL);
	if (ret_chk < 0) {
		printf("tc_pthread_pthread_sigmask FAIL SIG_UNBLOCK error\n");
		total_fail++;
		RETURN_ERR;
	}

	ret_chk = sigpending(&st_pendmask);
	if (ret_chk < 0) {
		printf("tc_pthread_pthread_sigmask FAIL sigpending error\n");
		total_fail++;
		RETURN_ERR;
	}

	nanosleep(&st_timespec, NULL);

	if (!g_sig_handle) {
		printf("tc_pthread_pthread_sigmask FAIL\n");
		total_fail++;
		RETURN_ERR;
	}

	if (sigaction(SIGQUIT, &st_oact, NULL)) {
		printf("tc_pthread_pthread_sigmask FAIL sigaction error\n");
		total_fail++;
		RETURN_ERR;
	}
	printf("tc_pthread_pthread_sigmask PASS\n");
	total_pass++;
}

static void tc_pthread_pthread_self(void)
{
	pthread_t tcb_pid;
	if (pthread_create(&tcb_pid, NULL, self_test_thread, NULL) != 0) {
		printf("tc_pthread_pthread_self FAIL : pthread_create\n");
		total_fail++;
		RETURN_ERR;
	}

	if (self_pid != tcb_pid) {
		printf("tc_pthread_pthread_self FAIL : pids are not matched %d %d\n", tcb_pid, self_pid);
		total_fail++;
		RETURN_ERR;
	} else {
		total_pass++;
		printf("tc_pthread_pthread_self PASS\n");
	}

	pthread_join(tcb_pid, NULL);
}

static void tc_pthread_pthread_equal(void)
{
	pthread_t first_th;
	pthread_t second_th;
	bool check_same;

	if (pthread_create(&first_th, NULL, do_nothing_thread, NULL) != 0) {
		printf("tc_pthread_pthread_equal FAIL : pthread_create\n");
		total_fail++;
		RETURN_ERR;
	}

	if (pthread_create(&second_th, NULL, do_nothing_thread, NULL) != 0) {
		printf("tc_pthread_pthread_equal FAIL : pthread_create\n");
		total_fail++;
		RETURN_ERR;
	}

	pthread_join(first_th, NULL);
	pthread_join(second_th, NULL);

	check_same = pthread_equal(first_th, second_th);
	if (check_same != FALSE) {
		total_fail++;
		printf("tc_pthread_pthread_equal FAIL\n");
		RETURN_ERR;
	} else {
		total_pass++;
		printf("tc_pthread_pthread_equal PASS\n");
	}
}

#if !defined(CONFIG_BUILD_PROTECTED)
static void tc_pthread_pthread_setschedprio(void)
{
	int ret;
	pthread_t set_th;
	uint8_t set_prio = 101;

	if (pthread_create(&set_th, NULL, setschedprio_test_thread, NULL) != 0) {
		printf("tc_pthread_pthread_setschedprio FAIL : pthread_create\n");
		total_fail++;
		RETURN_ERR;
	}
	/* change set_th PID's priority to set_prio */
	ret = pthread_setschedprio(set_th, set_prio);
	if (ret != OK) {
		printf("tc_pthread_pthread_setschedprio FAIL : setschedprio, returned value = %d\n", ret);
		total_fail++;
		RETURN_ERR;
	}

	ret = pthread_join(set_th, NULL);
	if (ret != OK) {
		printf("tc_pthread_pthread_setschedprio FAIL : pthread_join, returned value = %d\n", ret);
		total_fail++;
		RETURN_ERR;
	}
	if (check_prio != set_prio) {
		printf("tc_pthread_pthread_setschedprio FAIL : not matched - SETPID = %d, GETPID = %d\n", set_prio, check_prio);
		total_fail++;
		RETURN_ERR;
	} else {
		total_pass++;
		printf("tc_pthread_pthread_setschedprio PASS\n");
	}
}
#endif

static void tc_pthread_pthread_setgetname_np(void)
{
	int ret;
	pthread_t name_th;
	char *thread_name = "NameThread";
	char get_name[32];

	ret = pthread_create(&name_th, NULL, setgetname_thread, NULL);
	if (ret != OK) {
		printf("tc_pthread_pthread_setgetname_np FAIL : pthread_create\n");
		total_fail++;
		RETURN_ERR;
	}

	ret = pthread_setname_np(name_th, "NameThread");
	if (ret != OK) {
		printf("tc_pthread_pthread_setgetname_np FAIL : pthread_setname_np\n");
		total_fail++;
		RETURN_ERR;
	}

	ret = pthread_getname_np(name_th, get_name);
	if (ret != OK) {
		printf("tc_pthread_pthread_setgetname_np FAIL : pthread_getname_np, errno : %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	if (strcmp(get_name, thread_name) != 0) {
		printf("tc_pthread_pthread_setgetname_np FAIL : not matched between set and get name\n");
		total_fail++;
		RETURN_ERR;
	}

	ret = pthread_cancel(name_th);
	if (ret != OK) {
		printf("tc_pthread_pthread_setgetname_np FAIL : pthread_cancel, returned value = %d\n", ret);
		total_fail++;
		RETURN_ERR;
	}
	ret = pthread_join(name_th, NULL);
	if (ret != OK) {
		printf("tc_pthread_pthread_setgetname_np FAIL : pthread_join, returned value = %d\n", ret);
		total_fail++;
		RETURN_ERR;
	}
	total_pass++;
	printf("tc_pthread_pthread_setgetname_np PASS\n");
}

/****************************************************************************
 * Name: pthread
 ****************************************************************************/

int pthread_main(void)
{
	tc_pthread_pthread_barrier_init_destroy_wait();
	tc_pthread_pthread_create_exit_join();
	tc_pthread_pthread_kill();
	tc_pthread_pthread_cond_broadcast();
	tc_pthread_pthread_cond_init_destroy();
	tc_pthread_pthread_set_get_schedparam();
	tc_pthread_pthread_key_create_set_getspecific();
	tc_pthread_pthread_cancel_setcancelstate();
#if !defined(CONFIG_BUILD_PROTECTED)
	tc_pthread_pthread_take_give_semaphore();
	tc_pthread_pthread_findjoininfo_destroyjoin();
	tc_pthread_pthread_setschedprio();
#endif
	tc_pthread_pthread_timed_wait();
	tc_pthread_pthread_mutex_lock_unlock_trylock();
	tc_pthread_pthread_mutex_init_destroy();
	tc_pthread_pthread_once();
	tc_pthread_pthread_yield();
	tc_pthread_pthread_cond_signal_wait();
	tc_pthread_pthread_detach();
	tc_pthread_pthread_sigmask();
	tc_pthread_pthread_self();
	tc_pthread_pthread_equal();
	tc_pthread_pthread_setgetname_np();

	return 0;
}
