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

/// @file libc_pthread.c

/// @brief Test Case Example for Libc Pthread API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <sys/types.h>
#include "tc_internal.h"

#define ST_SIZE         1024
#define SCHED_PRIOR     100
#define THREAD_CNT      2
#define SEC_5           5
#define PTHREAD_NUM     2
#define LOOPCOUNT       5
#define LOOP_COUNT      150000
#define VAL_9999        9999

pthread_t g_pthread_usr;
pthread_t g_pthread_barrier1, g_pthread_barrier2;
pthread_t g_rgpthread[PTHREAD_NUM];

pthread_barrier_t g_pthreadbarrier;
pthread_cond_t g_pthread_cond;
pthread_mutex_t g_pthread_mutex = PTHREAD_MUTEX_INITIALIZER;
int g_icondition_met = 0;
int g_counter = 0;
volatile int g_mutex_counter = 0;
pthread_mutex_t g_mutex;

/**
* @fn                   :mutex_testing
* @description          :helper function for tc_libc_pthread_pthread_mutexattr
* @return               :void*
*/
void *mutex_testing(void *param)
{
	int index;
	for (index = 0; index < LOOPCOUNT; index++) {
		pthread_mutex_lock(&g_mutex);
		g_mutex_counter++;
		usleep(1);
		pthread_mutex_unlock(&g_mutex);
	}
	pthread_exit("exit from the mutex working method");
}

/**
* @fn                   :barrier_thread_1
* @description          :helper function for tc_libc_pthread_pthread_barrierattr_init_destroy_set_get_pshared
* @return               :void*
*/
static void *barrier_thread_1(void *arg)
{
	int iloopindex = LOOP_COUNT;	/* An arbitrary value used in while loop counter. */

	while (iloopindex > 0) {
		/* This is busy loop, just for eating cpu cycle
		   to time to another thread for execution. */
		iloopindex--;
	}

	/* The synchronization point */
	pthread_barrier_wait(&g_pthreadbarrier);
	pthread_exit(NULL);
	return NULL;
}

/**
* @fn                   :barrier_thread_2
* @description          :helper function for tc_libc_pthread_pthread_barrierattr_init_destroy_set_get_pshared
* @return               :void*
*/
static void *barrier_thread_2(void *arg)
{
	int iloopindex = LOOP_COUNT;	/* An arbitrary value used in while loop counter. */
	while (iloopindex > 0) {
		/* This is busy loop, just for eating cpu cycle
		   to time to another thread for execution. */
		iloopindex--;
	}

	/* The synchronization point */
	pthread_barrier_wait(&g_pthreadbarrier);
	pthread_exit(NULL);
	return NULL;
}

/**
* @fn                   :pthread_condattr_thread_func
* @description          :helper function for tc_libc_pthread_pthread_condattr
* @return               :void*
*/
static void *pthread_condattr_thread_func(void *arg)
{
	pthread_mutex_lock(&g_pthread_mutex);
	while (!g_icondition_met) {
		g_counter = g_counter + 1;
		/* Thread blocked here. */
		pthread_cond_wait(&g_pthread_cond, &g_pthread_mutex);
	}
	pthread_mutex_unlock(&g_pthread_mutex);
	g_counter = g_counter - 1;
	pthread_exit(NULL);
	return NULL;
}

/**
* @fn                   :avsprintfFunc
* @description          :Function for tc_libc_stdio_avsprintf
* @return               :int
*/
static void *work_usrthread(void *arg)
{
	int iloopindex = LOOP_COUNT;	/* An arbitrary value used in while loop counter. */
	while (iloopindex > 0) {	/* This is a dummy calculation loop for thread */
		iloopindex--;
	}
	pthread_exit(NULL);
	return NULL;
}

/**
* @fn                   :tc_libc_pthread_pthread_attr_set_get_stacksize
* @brief                :The tc_libc_pthread_pthread_attr_set_get_stacksize() test case function set the stack size for thread by using
*                        API pthread_attr_setstacksize and retrieve the thread stack size by API pthread_attr_getstacksize
* @Scenario             :A thread is created with specific thread attributes sets thread stack size then create the
*                        thread using this attribute, after it we check whether thread attribute is applied or not.
* @API'scovered         :pthread_attr_setstacksize, pthread_attr_getstacksize.
* @Preconditions        :pthread_attr_t object should be initialize first and pthread_create API should be called using pthread_attr_t object
* @Postconditions       :NA
* @return               :void
*/
static void tc_libc_pthread_pthread_attr_set_get_stacksize(void)
{
	pthread_attr_t attr;
	int status;
	long nstacksize = ST_SIZE;
	long nretstacksize;

	status = pthread_attr_init(&attr);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_set_get_stacksize FAIL : pthread_attr_init failed\n");
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_attr_setstacksize(&attr, nstacksize);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_set_get_stacksize FAIL : pthread_attr_setstacksize failed\n");
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_create(&g_pthread_usr, &attr, work_usrthread, NULL);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_set_get_stacksize FAIL : pthread_create failed\n");
		pthread_detach(g_pthread_usr);
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_attr_getstacksize(&attr, &nretstacksize);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_set_get_stacksize FAIL : pthread_attr_getstacksize failed\n");
		pthread_detach(g_pthread_usr);
		total_fail++;
		RETURN_ERR;
	}

	if (nstacksize != nretstacksize) {
		printf("tc_libc_pthread_pthread_attr_set_get_stacksize FAIL :pthread_attr_stack was set as %d\n", nstacksize);
		pthread_detach(g_pthread_usr);
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_join(g_pthread_usr, NULL);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_set_get_stacksize FAIL : pthread_join failed\n");
		pthread_detach(g_pthread_usr);
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_attr_destroy(&attr);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_set_get_stacksize FAIL : pthread_attr_destroy failed\n");
		pthread_detach(g_pthread_usr);
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_libc_pthread_pthread_attr_set_get_stacksize PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_libc_pthread_pthread_attr_set_get_schedparam
* @brief                :The tc_libc_pthread_pthread_attr_set_get_schedparam() test case function set the thread priority for thread by using
*                        API pthread_attr_setschedparam and retrieve thread priority by API pthread_attr_setschedparam
* @Scenario             :A thread is created with specific thread attributes sets thread priority then create the
*                        thread using this attribute, after it we check whether thread attribute is applied or not.
* @API'scovered         :pthread_attr_setschedparam, pthread_attr_setschedparam.
* @Preconditions        :pthread_attr_t object should be initialize first and pthread_create API
*                        should be called using pthread_attr_t object
* @Postconditions       :NA
* @return               :void
*/
static void tc_libc_pthread_pthread_attr_set_get_schedparam(void)
{
	pthread_attr_t attr;
	struct sched_param param;
	struct sched_param rparam;
	int status;
	status = pthread_attr_init(&attr);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_set_get_schedparam FAIL : pthread_attr_init failed\n");
		total_fail++;
		RETURN_ERR;
	}

	param.sched_priority = SCHED_PRIOR;

	status = pthread_attr_setschedparam(&attr, &param);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_set_get_schedparam FAIL : pthread_attr_setschedparam failed\n");
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_create(&g_pthread_usr, &attr, work_usrthread, NULL);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_set_get_schedparam FAIL : pthread_create failed\n");
		pthread_detach(g_pthread_usr);
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_attr_getschedparam(&attr, &rparam);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_set_get_schedparam FAIL : pthread_attr_getschedparam failed\n");
		pthread_detach(g_pthread_usr);
		total_fail++;
		RETURN_ERR;
	}

	if (rparam.sched_priority != param.sched_priority) {
		printf("tc_libc_pthread_pthread_attr_set_get_schedparam FAIL : pthread_attr_schedparam(priority) was set as %d\n", rparam.sched_priority);
		pthread_detach(g_pthread_usr);
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_join(g_pthread_usr, NULL);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_set_get_schedparam FAIL : pthread_join failed\n");
		pthread_detach(g_pthread_usr);
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_attr_destroy(&attr);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_set_get_schedparam FAIL : pthread_attr_destroy failed\n");
		pthread_detach(g_pthread_usr);
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_libc_pthread_pthread_attr_set_get_schedparam PASS\n");
	total_pass++;
	pthread_detach(g_pthread_usr);
}

/**
* @fn                   :tc_libc_pthread_pthread_attr_init_destroy_set_get_inheritsched
* @brief                :The tc_libc_pthread_pthread_attr_init_destroy_set_get_inheritsched() test case function set the inherit-scheduler for thread by using
*                        API pthread_attr_setinheritsched and retrieve thread priority by API pthread_attr_getinheritsched
* @Scenario             :The pthread_attr_setinheritsched() function sets the inherit-scheduler attribute
*                        of the thread attributes object referred to by attr to the value specified in inheritsched.
*                        The inherit-scheduler attribute determines whether a thread created using the thread
*                        attributes object attr will inherit its scheduling attributes from the calling thread
*                        or whether it will take them from attr.
* @API'scovered         :pthread_attr_init, pthread_attr_setinheritsched, pthread_attr_getinheritsched, pthread_attr_destroy.
* @Preconditions        :pthread_attr_t object should be initialize first and pthread_create API
*                        should be called using pthread_attr_t object
* @Postconditions       :NA
* @return               :void
*/
static void tc_libc_pthread_pthread_attr_init_destroy_set_get_inheritsched(void)
{
	pthread_attr_t attr;
	int inherit_sched_setval = PTHREAD_EXPLICIT_SCHED;
	int inherit_sched_getval;
	int status;
	status = pthread_attr_init(&attr);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_init_destroy_set_get_inheritsched FAIL : pthread_attr_init failed\n");
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_attr_setinheritsched(&attr, inherit_sched_setval);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_init_destroy_set_get_inheritsched FAIL : pthread_attr_setinheritsched failed\n");
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_create(&g_pthread_usr, &attr, work_usrthread, NULL);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_init_destroy_set_get_inheritsched FAIL : pthread_create failed\n");
		pthread_detach(g_pthread_usr);
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_attr_getinheritsched(&attr, &inherit_sched_getval);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_init_destroy_set_get_inheritsched FAIL : pthread_attr_getinheritsched failed\n");
		pthread_detach(g_pthread_usr);
		total_fail++;
		RETURN_ERR;
	}

	if (inherit_sched_setval != inherit_sched_getval) {
		printf("tc_libc_pthread_pthread_attr_init_destroy_set_get_inheritsched FAIL : Inherit scheduler was set as %d\n", inherit_sched_setval);
		pthread_detach(g_pthread_usr);
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_join(g_pthread_usr, NULL);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_init_destroy_set_get_inheritsched FAIL : pthread_attr_join failed\n");
		pthread_detach(g_pthread_usr);
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_attr_destroy(&attr);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_init_destroy_set_get_inheritsched FAIL : pthread_attr_destroy failed\n");
		pthread_detach(g_pthread_usr);
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_libc_pthread_pthread_attr_init_destroy_set_get_inheritsched PASS\n");
	total_pass++;
	pthread_cancel(g_pthread_usr);
}

/**
* @fn                   :tc_libc_pthread_pthread_attr_schedpolicy
* @brief                :The tc_libc_pthread_pthread_attr_schedpolicy() test case function sets the scheduling policy for thread by using
*                        API pthread_attr_setschedpolicy and retrieve scheduling policy by API pthread_attr_getschedpolicy
* @Scenario             :The pthread_attr_setschedpolicy() function sets the scheduling policy
*                        attribute of the thread attributes object referred to by attr to the
*                        value specified in policy. This attribute determines the scheduling
*                        policy of a thread created using the thread attributes object attr.
* @API'scovered         :pthread_attr_init, pthread_attr_setschedpolicy, pthread_attr_getschedpolicy, pthread_attr_destroy.
* @Preconditions        :pthread_attr_t object should be initialize first and pthread_create API
*                        should be called using pthread_attr_t object
* @Postconditions       :NA
* @return               :void
*/
static void tc_libc_pthread_pthread_attr_schedpolicy(void)
{
	pthread_attr_t attr;
	int sched_policy_setval = SCHED_RR;
	int sched_policy_getval;
	int status;
	status = pthread_attr_init(&attr);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_schedpolicy FAIL : pthread_attr_init failed\n");
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_attr_setschedpolicy(&attr, sched_policy_setval);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_schedpolicy FAIL : pthread_attr_setschedpolicy failed\n");
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_create(&g_pthread_usr, &attr, work_usrthread, NULL);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_schedpolicy FAIL : pthread_create failed\n");
		pthread_detach(g_pthread_usr);
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_attr_getschedpolicy(&attr, &sched_policy_getval);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_schedpolicy FAIL : pthread_attr_getschedpolicy failed\n");
		pthread_detach(g_pthread_usr);
		total_fail++;
		RETURN_ERR;
	}

	if (sched_policy_setval != sched_policy_getval) {
		printf("tc_libc_pthread_pthread_attr_schedpolicy FAIL :Scheduler Policy  was set as %d\n", sched_policy_setval);
		pthread_detach(g_pthread_usr);
		total_fail++;
		RETURN_ERR;
	}
	status = pthread_join(g_pthread_usr, NULL);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_schedpolicy FAIL : pthread_join failed\n");
		pthread_detach(g_pthread_usr);
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_attr_destroy(&attr);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_attr_schedpolicy FAIL : pthread_attr_destroy failed\n");
		pthread_detach(g_pthread_usr);
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_libc_pthread_pthread_attr_schedpolicy PASS\n");
	total_pass++;
	pthread_detach(g_pthread_usr);
}

/**
* @fn                  :tc_libc_pthread_pthread_barrierattr_init_destroy_set_get_pshared
* @brief               :The pthread_barrierattr_setpshared() function shall set the process-shared attribute in
*                       an initialized attributes object referenced by attr.The pthread_barrierattr_getpshared() function
*                       shall obtain the value of the process-shared attribute from the attributes object referenced by attr.
* @Scenario            :The tc_libc_pthread_pthread_barrierattr_init_destroy_set_get_pshared() test case function initializes the thread barrier attribute object (attr)
*                       using pthread_barrierattr_init() function. Then set one of its attributes like pthread_barrierattr_setpshared.
*                       Then create barrier and use in threads after completing threads displaying barrier attributes which was set initially..
* API'scovered         :pthread_barrierattr_init,  pthread_barrierattr_getpshared,  pthread_barrierattr_setpshared,  pthread_barrierattr_destroy.
* Preconditions        :NA
* @Postconditions      :NA
* @return              :void
*/
static void tc_libc_pthread_pthread_barrierattr_init_destroy_set_get_pshared(void)
{
	pthread_barrierattr_t attr;
	int status = 0;
	int thread_count = THREAD_CNT;
	int pshared = PTHREAD_PROCESS_PRIVATE;
	int pshared_ret;

	status = pthread_barrierattr_init(&attr);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_barrierattr_init_destroy_set_get_pshared FAIL : pthread_barrierattr_init failed\n");
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_barrierattr_setpshared(&attr, pshared);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_barrierattr_init_destroy_set_get_pshared FAIL : pthread_barrierattr_setpshared failed\n");
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_barrier_init(&g_pthreadbarrier, &attr, thread_count);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_barrierattr_init_destroy_set_get_pshared FAIL : pthread_barrier_init failed\n");
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_create(&g_pthread_barrier1, NULL, barrier_thread_1, NULL);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_barrierattr_init_destroy_set_get_pshared FAIL : pthread_create failed\n");
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_barrierattr_getpshared(&attr, &pshared_ret);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_barrierattr_init_destroy_set_get_pshared FAIL : pthread_barrierattr_getpshared failed\n");
		pthread_detach(g_pthread_barrier1);
		total_fail++;
		RETURN_ERR;
	}

	if (pshared_ret != pshared) {
		printf("tc_libc_pthread_pthread_barrierattr_init_destroy_set_get_pshared FAIL\n");
		pthread_detach(g_pthread_barrier1);
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_create(&g_pthread_barrier2, NULL, barrier_thread_2, NULL);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_barrierattr_init_destroy_set_get_pshared FAIL : pthread_create failed\n");
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_barrierattr_destroy(&attr);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_barrierattr_init_destroy_set_get_pshared FAIL : pthread_barrierattr_destroy failed\n");
		pthread_detach(g_pthread_barrier2);
		total_fail++;
		RETURN_ERR;
	}

	status = pthread_barrier_destroy(&g_pthreadbarrier);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_barrierattr_init_destroy_set_get_pshared FAIL : pthread_barrier_destroy failed\n");
		pthread_detach(g_pthread_barrier2);
		total_fail++;
		RETURN_ERR;
	}
	pthread_join(g_pthread_barrier1, 0);
	pthread_join(g_pthread_barrier2, 0);
	printf("tc_libc_pthread_pthread_barrierattr_init_destroy_set_get_pshared PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_libc_pthread_pthread_condattr()
* @brief                :The tc_libc_pthread_pthread_condattr() test case function initializes the conditional attribute object(attr)
*                        and condition object first, then create three threads which wait on this condition object.
* @Scenario             :In this test case we create a five threads and a global variable is incremented before
*                        conditional attribute object,after executing threads global variable count is checked it
*                        should be five,we decrement this global variable after conditional wait statement.
*                        so we check its count should be zero after wait condition is signalled.  we destroy conditional
*                        attribute and condition objects by pthread_condattr_destroy API.
* API'scovered          :pthread_condattr_destroy,pthread_condattr_init,pthread_cond_init,pthread_cond_broadcast.
* Preconditions         :NA
* @Postconditions       :NA
* @return               :void
*/
static void tc_libc_pthread_pthread_condattr(void)
{
	int status = 0;
	g_counter = 0;
	g_icondition_met = 0;
	int pthread_num;
	pthread_condattr_t attr;

	/* Create a default condition attribute */
	status = pthread_condattr_init(&attr);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_condattr FAIL : pthread_condattr_init failed\n");
		total_fail++;
		RETURN_ERR;
	}

	/* Create the condition using the condition attributes object */
	status = pthread_cond_init(&g_pthread_cond, &attr);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_condattr FAIL : pthread_cond_init failed\n");
		total_fail++;
		RETURN_ERR;
	}
	for (pthread_num = 0; pthread_num < PTHREAD_NUM; pthread_num++) {
		pthread_create(&g_rgpthread[pthread_num], NULL, pthread_condattr_thread_func, NULL);
	}
	sleep(SEC_5);
	if (g_counter != PTHREAD_NUM) {
		printf("FAIL:g_counter!=threadcount\n");
		total_fail++;
		for (pthread_num = 0; pthread_num < PTHREAD_NUM; pthread_num++) {
			pthread_detach(g_rgpthread[pthread_num]);
		}
		printf("tc_libc_pthread_pthread_condattr FAIL : pthread_create failed\n");
		RETURN_ERR;
	}

	pthread_mutex_lock(&g_pthread_mutex);
	g_icondition_met = 1;
	pthread_cond_broadcast(&g_pthread_cond);
	pthread_mutex_unlock(&g_pthread_mutex);

	for (pthread_num = 0; pthread_num < PTHREAD_NUM; pthread_num++) {
		status = pthread_join(g_rgpthread[pthread_num], NULL);
		if (status != OK) {
			printf("tc_libc_pthread_pthread_condattr FAIL : pthread_join failed\n");
			pthread_detach(g_rgpthread[pthread_num]);
			total_fail++;
			RETURN_ERR;
		}
	}
	if (g_counter != 0) {
		printf("FAIL:g_counter!=0\n");
		total_fail++;
		RETURN_ERR;
	}

	/* Destroy cond attribute */
	status = pthread_condattr_destroy(&attr);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_condattr FAIL : pthread_condattr_destroy failed\n");
		total_fail++;
		RETURN_ERR;
	}

	/* Destroy condition */
	status = pthread_cond_destroy(&g_pthread_cond);
	if (status != OK) {
		printf("tc_libc_pthread_pthread_condattr FAIL : pthread_cond_destroy failed\n");
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_libc_pthread_pthread_condattr PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_libc_pthread_pthread_mutexattr()
* @brief                :The tc_libc_pthread_pthread_mutexattr() test case function initialize mutex attribute object and set two propery
*                        of this attribute object. Then initialize mutex object using this attribute object, finally destroy both the objects.
* @Scenario             :In this test case we create mutext attribute object and set mutex is as PTHREAD_MUTEX_ERRORCHECK  and
*                        set mutex process shared attribute as PTHREAD_PROCESS_PRIVATE using pthread_mutexattr_settype and
*                        pthread_mutexattr_setpshared API,later by using pthread_mutexattr_gettype and pthread_mutexattr_getpshared
*                        API we check the values which was set initially.
* API'scovered          :pthread_mutexattr_destroy,pthread_mutexattr_getpshared,pthread_mutexattr_gettype,pthread_mutexattr_init,pthread_mutexattr_setpshared,pthread_mutexattr_settype
* Preconditions         :NA
* @Postconditions       :NA
* @return               :void
*/
static void tc_libc_pthread_pthread_mutexattr(void)
{
	int iret = 0;
	int ithreadid = 0;
	int mutex_settype;
	int mutex_gettype = -1;
	int mutex_pshared_set;
	int mutex_pshared_get = -1;
	int ithreadcount = 3;
	void *ithread;
	mutex_pshared_set = PTHREAD_MUTEX_DEFAULT;
	mutex_settype = PTHREAD_PROCESS_SHARED;
	pthread_t threads[ithreadcount];
	pthread_mutexattr_t mutexAttr;
	iret = pthread_mutexattr_init(&mutexAttr);

	if (iret != OK) {
		printf("tc_libc_pthread_pthread_mutexattr FAIL : pthread_mutexattr_init failed\n");
		total_fail++;
		RETURN_ERR;
	}

	iret = pthread_mutexattr_setpshared(&mutexAttr, mutex_pshared_set);
	if (iret != OK) {
		printf("tc_libc_pthread_pthread_mutexattr FAIL : pthread_mutexattr_setpshared failed\n");
		total_fail++;
		RETURN_ERR;
	}

	iret = pthread_mutexattr_settype(&mutexAttr, mutex_settype);
	if (iret != OK) {
		printf("tc_libc_pthread_pthread_mutexattr FAIL : pthread_mutexattr_settype failed\n");
		total_fail++;
		RETURN_ERR;
	}

	iret = pthread_mutex_init(&g_mutex, &mutexAttr);
	if (iret != OK) {
		printf("tc_libc_pthread_pthread_mutexattr FAIL : pthread_mutex_init failed\n");
		total_fail++;
		RETURN_ERR;
	}

	for (ithreadid = 0; ithreadid < ithreadcount; ++ithreadid) {
		iret = pthread_create(&threads[ithreadid], NULL, mutex_testing, (void *)ithreadid);
		if (iret != 0) {
			printf("[%s]pthread_create fail \n", __func__);
		}
	}

	/* apply join on thread */
	for (ithreadid = 0; ithreadid < ithreadcount; ++ithreadid) {
		iret = pthread_join(threads[ithreadid], &ithread);
		if (iret != OK) {
			printf("[%s]pthread_join failed \n", __func__);
		}
	}

	iret = pthread_mutexattr_gettype(&mutexAttr, &mutex_gettype);
	if (iret != OK || mutex_gettype != mutex_settype) {
		printf("tc_libc_pthread_pthread_mutexattr FAIL : pthread_mutexattr_gettype failed\n");
		pthread_mutexattr_destroy(&mutexAttr);
		pthread_mutex_destroy(&g_mutex);
		total_fail++;
		RETURN_ERR;
	}

	iret = pthread_mutexattr_getpshared(&mutexAttr, &mutex_pshared_get);

	if (iret != OK || mutex_pshared_get != mutex_pshared_set) {
		printf("[%s]pthread_mutexattr_getpshared failed \n", __func__);
		pthread_mutexattr_destroy(&mutexAttr);
		pthread_mutex_destroy(&g_mutex);
		total_fail++;
		RETURN_ERR;
	}

	iret = pthread_mutexattr_destroy(&mutexAttr);
	if (iret != OK) {
		printf("[%s]pthread_mutexattr_destroy failed \n", __func__);
		pthread_mutex_destroy(&g_mutex);
		total_fail++;
		RETURN_ERR;
	}
	/* Destroying mutex object */
	iret = pthread_mutex_destroy(&g_mutex);
	if (iret != OK) {
		printf("tc_libc_pthread_pthread_mutexattr FAIL : pthread_mutexattr_destroy failed\n");
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_libc_pthread_pthread_mutexattr PASS\n");
	total_pass++;
}

/****************************************************************************
 * Name: libc_pthread
 ****************************************************************************/

int libc_pthread_main(void)
{
	tc_libc_pthread_pthread_attr_set_get_stacksize();
	tc_libc_pthread_pthread_attr_set_get_schedparam();
	tc_libc_pthread_pthread_attr_init_destroy_set_get_inheritsched();
	tc_libc_pthread_pthread_attr_schedpolicy();
	tc_libc_pthread_pthread_barrierattr_init_destroy_set_get_pshared();
	tc_libc_pthread_pthread_condattr();
	tc_libc_pthread_pthread_mutexattr();

	return 0;
}
