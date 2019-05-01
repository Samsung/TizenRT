/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <tinyara/pthread.h>
#include <tinyara/config.h>
#include "tc_internal.h"
#include <sys/types.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <string.h>

#define WAIT_TIME_1                   1
#define WAIT_TIME_2                   2
#define WAIT_TIME_4                   4
#define WAIT_TIME_8                   8
#define WAIT_TIME_0                   0
#define BUF_LEN 32
#define INIT_MINUSONE -1
#define SIG_ERROR 0
#define LOOP_SIZE CONFIG_ITC_KERN_PTHREAD_LOOP_SIZE

static pthread_t g_pthread;
static bool g_flag;
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_key_t g_key;
static int  g_exec_index;
static bool g_bpthreadcallback = false;

/****************************************************************************
*Helper Functions
**************************************************************************/
/**
* @fn                   :thread_setgetname
* @brief                :utility function for creating thread
* @return               :void*
*/
static void *thread_setgetname(void *param)
{
	while (1) {
		sleep(WAIT_TIME_1);
		if (g_exec_index == LOOP_SIZE) {
			break;
		}
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
	if (pthread_equal(g_pthread, pthread_self()) == false) {
		g_flag = false;
		return NULL;
	}
	g_flag = true;
	return NULL;
}

/**
* @fn                   :itc_pthread_key_test
* @brief                :utility function for itc_pthread_key_create_set_getspecific_p_multitime
* @return               :void*
*/
static void *itc_pthread_key_test(void *param)
{
	int *get_spec;
	int ret_chk;
	int exec_index;
	g_flag = false;

	for (exec_index = 1; exec_index <= LOOP_SIZE; exec_index++) {
		ret_chk = pthread_key_create(&g_key, NULL);
		if (ret_chk != OK) {
			printf("pthread_key_create FAIL\n");
			return NULL;
		}

		ret_chk = pthread_setspecific(g_key, &exec_index);
		if (ret_chk != OK) {
			printf("pthread_setspecific FAIL\n");
			return NULL;
		}

		get_spec = pthread_getspecific(g_key);
		if (*get_spec != exec_index) {
			printf("pthread_getspecific FAIL\n");
			return NULL;
		}

		ret_chk = pthread_key_delete(g_key);
		if (ret_chk != OK) {
			printf("pthread_key_delete FAIL! ret: %d\n", ret_chk);
			return NULL;
		}
	}

	g_flag = true;
	return NULL;
}

/**
* @fn                   :task_thread
* @brief                :utility function for itc_pthread_pthread_kill_n_rekill
* @return               :void*
*/
static void *task_thread(void *param)
{
	sigset_t mask;
	siginfo_t info;
	struct timespec timeout;
	int ret = INIT_MINUSONE;

	timeout.tv_sec  = WAIT_TIME_4;
	timeout.tv_nsec = WAIT_TIME_0;
	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1);
	ret = pthread_sigmask(SIG_BLOCK, &mask, NULL);
	if (ret != 0) {
		printf("pthread_sigmask Called failed  \n");
		return NULL;
	}
	ret = sigtimedwait(&mask, &info, &timeout);
	if (ret < SIG_ERROR) {
		printf("Signal SIGSUR1:  Not Received inside  task_thread in =%d sec  \n", timeout.tv_sec);
	} else {
		printf("Signal SIGSUR1:  Received inside  task_thread\n");
		g_bpthreadcallback = true;
	}

	return NULL;
}

/****************************************************************************
  * Private Functions
 ****************************************************************************/

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
	pthread_t thread;
	g_bpthreadcallback = false;

	ret_chk = pthread_create(&thread, NULL, task_thread, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);
	sleep(WAIT_TIME_1);

	ret_chk = pthread_kill(thread, SIGUSR1);
	sleep(WAIT_TIME_2);
	TC_ASSERT_EQ_CLEANUP("pthread_kill", ret_chk, OK, pthread_detach(thread));
	TC_ASSERT_EQ_CLEANUP("pthread_kill", g_bpthreadcallback, true, pthread_detach(thread));

	ret_chk = pthread_join(thread, NULL);
	TC_ASSERT_EQ("pthread_join", ret_chk, OK);

	ret_chk = pthread_kill(thread, SIGUSR1);
	TC_ASSERT_EQ("pthread_kill", ret_chk, ESRCH);

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
	TC_ASSERT_EQ("pthread_cond_destroy", ret_chk, OK);

	ret_chk = pthread_condattr_destroy(&attr);
	TC_ASSERT_EQ("pthread_condattr_destroy", ret_chk, OK);

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
	int exec_index;
	pthread_t name_th;
	char thread_name[BUF_LEN];
	char *thread_str = "NameThread";
	char get_name[BUF_LEN];
	g_exec_index = 0;

	ret_chk = pthread_create(&name_th, NULL, thread_setgetname, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);

	for (exec_index = 0; exec_index < LOOP_SIZE; exec_index++) {
		snprintf(thread_name, BUF_LEN, "%s_%d", thread_str, exec_index);
		ret_chk = pthread_setname_np(name_th, thread_name);
		TC_ASSERT_EQ_CLEANUP("pthread_setname_np", ret_chk, OK, pthread_detach(name_th));

		ret_chk = pthread_getname_np(name_th, get_name);
		TC_ASSERT_EQ_CLEANUP("pthread_getname_np", ret_chk, OK, pthread_detach(name_th));
		TC_ASSERT_EQ_CLEANUP("pthread_getname_np", strcmp(get_name, thread_name), 0, pthread_detach(name_th));
		g_exec_index++;
	}

	ret_chk = pthread_join(name_th, NULL);
	TC_ASSERT_EQ("pthread_join", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_pthread_equal_pthread_self_p
* @brief                :Compares two thread which is equal or not
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
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);
	sleep(WAIT_TIME_1);
	TC_ASSERT_EQ_CLEANUP("pthread_create", g_flag, true, pthread_detach(g_pthread));

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
	int exec_index;

	for (exec_index = 0; exec_index < LOOP_SIZE; exec_index++) {
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
* @fn                   :itc_pthread_key_create_set_get_delete_specific_p_multitime
* @brief                :thread-specific data management
* @Scenario             :pthread_key_create, then set and get key_specific, delete key value multiple time
* API's covered         :pthread_setspecific, pthread_getspecific, pthread_key_create, pthread_key_delete
* Preconditions         :pthread_key_create
* Postconditions        :none
*/
static void itc_pthread_key_create_set_get_delete_specific_p_multitime(void)
{
	int ret_chk;
	g_flag = false;

	ret_chk = pthread_create(&g_pthread, NULL, itc_pthread_key_test, NULL);
	TC_ASSERT_EQ("pthread_create", ret_chk, OK);

	ret_chk = pthread_join(g_pthread, NULL);
	TC_ASSERT_EQ_CLEANUP("pthread_join", ret_chk, OK, pthread_detach(g_pthread));
	TC_ASSERT_EQ("pthread_join", g_flag, true);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: itc_pthread
 ****************************************************************************/

int itc_pthread_main(void)
{
	itc_pthread_pthread_kill_n_rekill();
	itc_pthread_pthread_cond_init_destroy_n();
	itc_pthread_setgetname_np_p_reset_name();
	itc_pthread_equal_pthread_self_p();
	itc_pthread_mutex_init_destroy_p_multitime();
	itc_pthread_key_create_set_get_delete_specific_p_multitime();
	return 0;
}
