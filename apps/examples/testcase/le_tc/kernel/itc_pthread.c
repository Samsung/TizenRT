/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

/// @file itc_pthread.c
/// @brief ITC Test Case Example for Pthread API

/****************************************************************************
 * Included and defines
 ****************************************************************************/

#include <stdio.h>
#include <errno.h>
#include "../../../../../os/kernel/group/group.h"
#include "../../../../../os/kernel/pthread/pthread.h"
#include "tc_internal.h"

#define SEC_1                   1
#define SEC_2                   2
#define SEC_8                   8
#define RETURN_PTHREAD_JOIN     ((void *)100)
#define BUF_LEN 32
#define LOOP_CNT 10

/****************************************************************************
 * Helper Functions
 **************************************************************************/
static pthread_t g_pthread;
static bool g_flag;
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_key_t g_key;

/**
* @fn                   :thread_setgetname
* @brief                :utility function for creating thread
* @return               :void*
*/
static void *thread_setgetname(void *param)
{
	while (1) {
		sleep(SEC_1);
	}
	return NULL;
}

/**
* @fn                   :thread_equal_check
* @brief                :utility function for itc_pthread_pthread_equal_p
* @return               :void*
*/
static void *thread_equal_check(void *param)
{
	sleep(SEC_1);
	if (pthread_equal(g_pthread, pthread_self()) == false) {
		g_flag = false;
		return NULL;
	}
	g_flag = true;
	return NULL;
}

/**
* @fn                   :thread_set_get_specific
* @brief                :utility function for itc_pthread_key_create_set_getspecific_p_multitime
* @return               :void*
*/
static void *thread_set_get_specific(void *param)
{
	int *get_spec;
	int ret_chk;
	int index;
	g_flag = false;

	for (index = 1; index <= LOOP_CNT; index++) {
		ret_chk = pthread_setspecific(g_key, &index);
		if (ret_chk != OK) {
			printf("itc_pthread_key_create_set_getspecific_p_multitime: TC FAIL\n");
			return NULL;
		}

		get_spec = pthread_getspecific(g_key);
		if (*get_spec != index) {
			printf("itc_pthread_key_create_set_getspecific_p_multitime: TC FAIL\n");
			return NULL;
		}
	}
	sleep(SEC_1);
	g_flag = true;
	return NULL;
}

/**
* @fn                   :task_exit
* @brief                :utility function for itc_pthread_pthread_kill_n_rekill
* @return               :void*
*/
static void *task_exit(void *param)
{
	pthread_setname_np(0, "task_exit");
	pthread_exit(RETURN_PTHREAD_JOIN);
	return NULL;
}

/**
* @fn                   :threadfunc_sched
* @brief                :utility function for itc_pthread_pthread_set_get_schedparam_n
* @return               :void*
*/
static void *threadfunc_sched(void *param)
{
	pthread_setname_np(0, "threadfunc_sched");
	sleep(SEC_8);
	pthread_exit((pthread_addr_t)1);
	return NULL;
}

/****************************************************************************
  * Private Functions
 ****************************************************************************/

/**
* @fn                   :itc_pthread_pthread_barrier_destroy_n_redestory
* @brief                :The pthread_barrier_init() function allocates any resources required to use
*                        the barrier referenced by 'barrier' and initialized the barrier with the
*                        attributes referenced by attr.
*                        barrier_wait synchronizse participating threads at the barrier referenced by 'barrier'.
* @Scenario             :pthread_barrier_destroy re-destroying the object as already being destroyed
* API's covered         :pthread_barrier_init, pthread_barrier_wait , pthread_barrier_destroy
* Preconditions         :none
* Postconditions        :none
*/
static void itc_pthread_pthread_barrier_destroy_n_redestory(void)
{
	int ret;
	pthread_barrier_t pthread_barrier;
	int cnt = 1;
	
	ret = pthread_barrier_init(&pthread_barrier, 0, cnt);
	TC_ASSERT_EQ("pthread_barrier_init", ret, OK);

	ret = pthread_barrier_destroy(&pthread_barrier);
	TC_ASSERT_EQ("pthread_barrier_destroy", ret, OK);

	ret = pthread_barrier_destroy(&pthread_barrier);
	TC_ASSERT_EQ("pthread_barrier_destroy", ret, EINVAL);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_pthread_pthread_kill_n_rekill
* @brief                :send a signal to a thread
* @Scenario             :The pthread_kill() call again when it's already being called for same thread
* API's covered         :pthread_kill, pthread_join
* Preconditions         :none
* Postconditions        :none
*/
static void itc_pthread_pthread_kill_n_rekill(void)
{
	int ret_chk;
	bool bpthreadcallback = false;
	pthread_t thread;

	ret_chk = pthread_create(&thread, NULL, task_exit, NULL);
	if (ret_chk != OK) {
		tckndbg("ERROR pthread_create FAIL\n");
		pthread_detach(thread);
	}
	sleep(SEC_2);

	ret_chk = pthread_kill(thread, SIGUSR1);
	sleep(SEC_1);
	TC_ASSERT_EQ_CLEANUP("pthread_kill", ret_chk, OK, pthread_detach(thread));
	TC_ASSERT_EQ_CLEANUP("pthread_kill", bpthreadcallback, true, pthread_detach(thread));

	ret_chk = pthread_kill(thread, SIGUSR1);
	TC_ASSERT_EQ("pthread_kill", ret_chk, ESRCH);

	pthread_join(thread, NULL);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_pthread_pthread_cond_init_destroy_n
* @brief                :pthread_cond_init initialises the condition variable referenced by cond with attributes referenced by attr
*                        pthread_cond_destroy destroy the given condition variable specified by cond
* @Scenario             :validating with valid and in-valid params in init and destroy
* API's covered         :pthread_cond_init, pthread_cond_destroy
* Preconditions         :none
* Postconditions        :none
*/
static void itc_pthread_pthread_cond_init_destroy_n(void)
{
	int ret_chk;
	pthread_condattr_t attr;
	pthread_cond_t *cond_nullparam = NULL;
	pthread_cond_t cond_attrparam;

	ret_chk = pthread_condattr_init(&attr);
	TC_ASSERT_EQ("pthread_condattr_init", ret_chk, OK);

	ret_chk = pthread_cond_init(cond_nullparam, NULL);
	TC_ASSERT_EQ("pthread_cond_init", ret_chk, EINVAL);

	ret_chk = pthread_cond_init(&cond_attrparam, &attr);
	TC_ASSERT_EQ("pthread_cond_init", ret_chk, OK);

	ret_chk = pthread_cond_destroy(&cond_attrparam);
	TC_ASSERT_EQ("pthread_cond_destroy", ret_chk, EINVAL);

	ret_chk = pthread_condattr_destroy(&attr);
	TC_ASSERT_EQ("pthread_condattr_destroy", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_pthread_pthread_cond_destroy_n_redestroy
* @brief                :pthread_cond_init initialises the condition variable referenced by cond with attributes referenced by attr
*                        pthread_cond_destroy destroy the given condition variable specified by cond
* @Scenario             :pthread_cond_destroy re-destroying the object as already being destroyed
* API's covered         :pthread_cond_init, pthread_cond_destroy
* Preconditions         :none
* Postconditions        :none
*/
static void itc_pthread_pthread_cond_destroy_n_redestroy(void)
{
	int ret_chk;
	pthread_condattr_t attr;
	pthread_cond_t cond_attrparam;

	ret_chk = pthread_condattr_init(&attr);
	TC_ASSERT_EQ("pthread_condattr_init", ret_chk, OK);

	ret_chk = pthread_cond_init(&cond_attrparam, &attr);
	TC_ASSERT_EQ("pthread_cond_init", ret_chk, OK);

	ret_chk = pthread_cond_destroy(&cond_attrparam);
	TC_ASSERT_EQ("pthread_cond_destroy", ret_chk, OK);

	ret_chk = pthread_cond_destroy(&cond_attrparam);
	TC_ASSERT_EQ("pthread_cond_destroy", ret_chk, EINVAL);

	ret_chk = pthread_condattr_destroy(&attr);
	TC_ASSERT_EQ("pthread_condattr_destroy", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_pthread_pthread_cond_broadcast_n_after_destroy
* @brief                :broadcast or signal a condition
* @Scenario             :pthread_cond_broadcast when object as already being destroyed.
* API's covered         :pthread_cond_broadcast ,pthread_cond_init , pthread_cond_destroy , pthread_cond_wait
* Preconditions         :none
* Postconditions        :none
*/
static void itc_pthread_pthread_cond_broadcast_n_after_destroy(void)
{
	int ret_chk;
	pthread_cond_t cond_attrparam;
	pthread_condattr_t attr;

	ret_chk = pthread_cond_init(&cond_attrparam, &attr);
	TC_ASSERT_EQ("pthread_cond_init", ret_chk, OK);

	ret_chk = pthread_cond_destroy(&cond_attrparam);
	TC_ASSERT_EQ("pthread_cond_destroy", ret_chk, EINVAL);

	ret_chk = pthread_cond_broadcast(&cond_attrparam);
	TC_ASSERT_EQ("pthread_cond_broadcast", ret_chk, EINVAL);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_pthread_getname_np_p_defaultname
* @brief                :The pthread_getname_np function gets the name of pthread
* @Scenario             :create pthread and get its default name
* API's covered         :pthread_setname_np, pthread_getname_np
* Preconditions         :none
* Postconditions        :none
*/
static void itc_pthread_getname_np_p_defaultname(void)
{
	int ret_chk;
	pthread_t name_th;
	char get_name[BUF_LEN];
	char *default_name = "<pthread>";

	ret_chk = pthread_create(&name_th, NULL, thread_setgetname, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);

	ret_chk = pthread_getname_np(name_th, get_name);
	TC_ASSERT_EQ("pthread_getname_np", ret_chk, OK);
	TC_ASSERT_EQ("pthread_getname_np", strcmp(get_name, default_name), 0);

	ret_chk = pthread_cancel(name_th);
	TC_ASSERT_EQ("pthread_cancel", ret_chk, OK);

	ret_chk = pthread_join(name_th, NULL);
	TC_ASSERT_EQ("pthread_join", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_pthread_setgetname_np_p_reset_name
* @brief                :The pthread_getname_np function gets the name of pthread
* @Scenario             :create pthread and get its default name
* API's covered         :pthread_setname_np, pthread_getname_np
* Preconditions         :none
* Postconditions        :none
*/
static void itc_pthread_setgetname_np_p_reset_name(void)
{
	int ret_chk;
	int index;
	int len;
	pthread_t name_th;
	char thread_name[BUF_LEN];
	char *thread_str = "NameThread";
	char get_name[BUF_LEN];

	len = strlen(thread_name);
	ret_chk = pthread_create(&name_th, NULL, thread_setgetname, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);

	for (index = 0; index < LOOP_CNT; index++) {
		snprintf(thread_name, BUF_LEN, "%s_%d", thread_str, index);
		ret_chk = pthread_setname_np(name_th, thread_name);
		TC_ASSERT_EQ("pthread_setname_np", ret_chk, OK);

		ret_chk = pthread_getname_np(name_th, get_name);
		TC_ASSERT_EQ("pthread_getname_np", ret_chk, OK);
		TC_ASSERT_EQ("pthread_getname_np", strcmp(get_name, thread_name), 0);
	}
	ret_chk = pthread_cancel(name_th);
	TC_ASSERT_EQ("pthread_cancel", ret_chk, OK);

	ret_chk = pthread_join(name_th, NULL);
	TC_ASSERT_EQ("pthread_join", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_pthread_equal_pthread_self_p
* @brief                :ompares two thread which is equal or not
* @Scenario             :create 2 pthread and compares two thread
* API's covered         :pthread_equal, pthread_self
* Preconditions         :none
* Postconditions        :none
*/
static void itc_pthread_equal_pthread_self_p(void)
{
	int ret_chk;

	g_flag = false;
	ret_chk = pthread_create(&g_pthread, NULL, thread_equal_check, NULL);
	sleep(SEC_1);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);
	TC_ASSERT_EQ("pthread_create", g_flag, true);

	ret_chk = pthread_join(g_pthread, NULL);
	TC_ASSERT_EQ("pthread_join", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_pthread_mutex_init_destroy_p_multitime
* @brief                :initialise and destroy mutex
* @Scenario             :initialise mutex and then destory mutex multitime
* API's covered         :pthread_mutex_init, pthread_mutex_destroy
* Preconditions         :none
* Postconditions        :none
*/
static void itc_pthread_mutex_init_destroy_p_multitime(void)
{
	pthread_mutexattr_t attr;
	int ret_chk;
	int index;

	for (index = 0; index < LOOP_CNT; index++) {
		ret_chk = pthread_mutexattr_init(&attr);
		TC_ASSERT_EQ("pthread_mutexattr_init", ret_chk, OK);

		ret_chk = pthread_mutex_init(&g_mutex, &attr);
		TC_ASSERT_EQ("pthread_mutex_init", ret_chk, OK);

		ret_chk = pthread_mutex_lock(&g_mutex);
		TC_ASSERT_EQ_CLEANUP("pthread_mutex_lock", ret_chk, OK, pthread_mutex_destroy(&g_mutex));

		ret_chk = pthread_mutex_destroy(&g_mutex);
		TC_ASSERT_EQ("pthread_mutex_destroy", ret_chk, EBUSY);

		ret_chk = pthread_mutex_unlock(&g_mutex);
		TC_ASSERT_EQ_CLEANUP("pthread_mutex_unlock", ret_chk, OK, pthread_mutex_destroy(&g_mutex));

		ret_chk = pthread_mutex_destroy(&g_mutex);
		TC_ASSERT_EQ("pthread_mutex_destroy", ret_chk, OK);
	}

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_pthread_mutex_lock_n_after_mutex_destroy
* @brief                :Locks the mutex
* @Scenario             :initialise mutex, destory mutex and then try to lock mutex
* API's covered         :pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock
* Preconditions         :none
* Postconditions        :none
*/
static void itc_pthread_mutex_lock_n_after_mutex_destroy(void)
{
	pthread_mutexattr_t attr;
	int ret_chk;

	ret_chk = pthread_mutexattr_init(&attr);
	TC_ASSERT_EQ("pthread_mutexattr_init", ret_chk, OK);

	ret_chk = pthread_mutex_init(&g_mutex, &attr);
	TC_ASSERT_EQ("pthread_mutex_init", ret_chk, OK);

	ret_chk = pthread_mutex_destroy(&g_mutex);
	TC_ASSERT_EQ("pthread_mutex_destroy", ret_chk, OK);

	ret_chk = pthread_mutex_lock(&g_mutex);
	TC_ASSERT_NEQ("pthread_mutex_lock", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_pthread_mutex_destroy_n_redestroy
* @brief                :destroy mutex
* @Scenario             :initialise mutex, destory mutex and then resestroy mutex
* API's covered         :pthread_mutex_init, pthread_mutex_destroy
* Preconditions         :none
* Postconditions        :none
*/
static void itc_pthread_mutex_destroy_n_redestroy(void)
{
	pthread_mutexattr_t attr;
	int ret_chk;

	ret_chk = pthread_mutexattr_init(&attr);
	TC_ASSERT_EQ("pthread_mutexattr_init", ret_chk, OK);

	ret_chk = pthread_mutex_init(&g_mutex, &attr);
	TC_ASSERT_EQ("pthread_mutex_init", ret_chk, OK);

	ret_chk = pthread_mutex_destroy(&g_mutex);
	TC_ASSERT_EQ("pthread_mutex_destroy", ret_chk, OK);

	ret_chk = pthread_mutex_destroy(&g_mutex);
	TC_ASSERT_NEQ("pthread_mutex_destroy", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_pthread_key_create_set_getspecific_p_multitime
* @brief                :thread-specific data management
* @Scenario             :pthread_key_create, then set and get key_specific value multiple time
* API's covered         :pthread_setspecific, pthread_getspecific, pthread_key_create
* Preconditions         :pthread_key_create
* Postconditions        :none
*/
static void itc_pthread_key_create_set_getspecific_p_multitime(void)
{
	int ret_chk;
	g_flag = false;
	g_key = 0;

	ret_chk = pthread_key_create(&g_key, NULL);
	TC_ASSERT_EQ("pthread_key_create", ret_chk, OK);

	ret_chk = pthread_create(&g_pthread, NULL, thread_set_get_specific, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);

	sleep(SEC_1);

	ret_chk = pthread_join(g_pthread, NULL);
	TC_ASSERT_EQ("pthread_join", ret_chk, OK);
	TC_ASSERT_EQ("pthread_join", g_flag, true);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: itc_pthread
 ****************************************************************************/

int itc_pthread_main(void)
{
	itc_pthread_pthread_barrier_destroy_n_redestory();
	itc_pthread_pthread_kill_n_rekill();
	itc_pthread_pthread_cond_init_destroy_n();
	itc_pthread_pthread_cond_destroy_n_redestroy();
	itc_pthread_pthread_cond_broadcast_n_after_destroy();
	itc_pthread_getname_np_p_defaultname();
	itc_pthread_setgetname_np_p_reset_name();
	itc_pthread_equal_pthread_self_p();
	itc_pthread_mutex_init_destroy_p_multitime();
	itc_pthread_mutex_lock_n_after_mutex_destroy();
	itc_pthread_mutex_destroy_n_redestroy();
	itc_pthread_key_create_set_getspecific_p_multitime();
	return 0;
}
