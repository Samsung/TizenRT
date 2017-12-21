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

/// @file itc_sched.c
/// @brief Test Case Example for Sched API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include "tc_internal.h"

#define SEC_1 1
#define ARRLEN 2
#define THREAD_CNT 3

static bool g_pthread_callback;

/**
* @fn                   :thread_verifytcb_cb
* @description          :Function for itc_sched_verifytcb_p
* @return               :void*
*/
static void *thread_verifytcb_cb(void *param)
{
	bool ret_chk = false;
	struct tcb_s *st_tcb;
	st_tcb = sched_self();
	if (st_tcb == NULL) {
		g_pthread_callback = false;
		return NULL;
	}
	ret_chk = sched_verifytcb(st_tcb);
	if (ret_chk == false) {
		g_pthread_callback = false;
		return NULL;
	}

	g_pthread_callback = true;
	return NULL;
}

/**
* @fn                   :thread_schedself_cb
* @description          :Function for itc_sched_self_p
* @return               :void*
*/
static void *thread_schedself_cb(void *param)
{
	struct tcb_s *st_tcbself;
	struct tcb_s *st_tcbpid;
	st_tcbpid = sched_self();
	if (st_tcbpid == NULL) {
		g_pthread_callback = false;
		return NULL;
	}

	/* should return tcb for current process */
	st_tcbself = sched_self();
	if (st_tcbself == NULL || st_tcbself->pid != st_tcbpid->pid) {
		g_pthread_callback = false;
		return NULL;
	}

	g_pthread_callback = true;
	return NULL;
}

/**
* @fn                   :itc_sched_setget_scheduler_param_p_all_priority
* @brief                :set and get scheduler policies for the named process
* @scenario             :set and get scheduler policies for the named process, sched_getscheduler should return scheduler set
* API's covered         :sched_setscheduler, sched_getscheduler
* Preconditions         :none
* Postconditions        :none
*/
static void itc_sched_setget_scheduler_param_p_all_priority(void)
{
	int ret_chk = ERROR;
	struct sched_param st_setparam;
	struct sched_param st_getparam;
	int arr_idx = 0;
	int index;
	int sched_arr[ARRLEN] = { SCHED_RR, SCHED_FIFO };

	while (arr_idx < 2) {
		for (index = SCHED_PRIORITY_MIN; index <= SCHED_PRIORITY_MAX; index++) {
			st_setparam.sched_priority = index;
			ret_chk = sched_setparam(getpid(), &st_setparam);
			TC_ASSERT_EQ("sched_setparam", ret_chk, OK);

			ret_chk = sched_setscheduler(getpid(), sched_arr[arr_idx], &st_setparam);
			TC_ASSERT_NEQ("sched_setscheduler", ret_chk, ERROR);

			/* ret_chk should be SCHED set */
			ret_chk = sched_getscheduler(getpid());
			TC_ASSERT_EQ("sched_getscheduler", ret_chk, sched_arr[arr_idx]);

			ret_chk = sched_getparam(getpid(), &st_getparam);
			TC_ASSERT_EQ("sched_getparam", ret_chk, OK);
			TC_ASSERT_EQ("sched_getparam", st_setparam.sched_priority, st_getparam.sched_priority);
		}
		arr_idx++;
	}
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_sched_self_p
* @brief                :Return current thread tcb
* @scenario             :Return current thread tcb structure, verified by getting sched_gettcb(getpid)
* API's covered         :sched_self
* Preconditions         :none
* Postconditions        :none
*/
static void itc_sched_self_p(void)
{
	pthread_t thread_id[THREAD_CNT];
	int ret_chk;
	int index;

	for (index = 0; index < THREAD_CNT; index++) {
		g_pthread_callback = false;
		ret_chk = pthread_create(&thread_id[index], NULL, thread_schedself_cb, NULL);
		sleep(SEC_1);
		TC_ASSERT_EQ("pthread_create", ret_chk, OK);
		TC_ASSERT_EQ("sched_self", g_pthread_callback, true);
	}

	for (index = 0; index < THREAD_CNT; index++) {
		pthread_join(thread_id[index], 0);
	}

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :itc_sched_verifytcb_p
* @brief                :Returns true if tcb refers to active task, false if it is state tcb handle
* @scenario             :sched_verifytcb returns true if tcb refers to active task, false if it is state tcb handle
* API's covered         :sched_verifytcb
* Preconditions         :none
* Postconditions        :none
*/
static void itc_sched_verifytcb_p(void)
{
	pthread_t thread_id[THREAD_CNT];
	int ret_chk;
	int index;

	for (index = 0; index < THREAD_CNT; index++) {
		g_pthread_callback = false;
		ret_chk = pthread_create(&thread_id[index], NULL, thread_verifytcb_cb, NULL);
		sleep(SEC_1);
		TC_ASSERT_EQ("pthread_create", ret_chk, OK);
		TC_ASSERT_EQ("sched_verifytcb", g_pthread_callback, true);
	}

	for (index = 0; index < THREAD_CNT; index++) {
		pthread_join(thread_id[index], 0);
	}

	TC_SUCCESS_RESULT();
}

int itc_sched_main(void)
{
	itc_sched_setget_scheduler_param_p_all_priority();
	itc_sched_self_p();
	itc_sched_verifytcb_p();

	return 0;
}
