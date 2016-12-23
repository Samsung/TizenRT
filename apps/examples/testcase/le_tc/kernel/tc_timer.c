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

/// @file timer.c
/// @brief Test Case Example for Timer API
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include "../../os/kernel/timer/timer.h"
#include "tc_internal.h"

#define USECINT 10000000

int sig_no = SIGRTMIN;
extern volatile sq_queue_t g_freetimers;
extern volatile sq_queue_t g_alloctimers;

/**
* @fn                   :tc_timer_timer_create_delete
* @brief                :Create and delete a POSIX per-process timer
* @scenario             :Create and delete a POSIX per-process timer
* API's covered         :timer_create, timer_delete
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_timer_timer_create_delete(void)
{
	int ret_chk = 0;
	timer_t timer_id;
	timer_t gtimer_id;
	clockid_t clockid = CLOCK_REALTIME;
	struct sigevent st_sigevent;
	struct posix_timer_s *st_ret_prt;
	struct itimerspec st_timer_spec_val;
	/* Set and enable alarm */
	st_sigevent.sigev_notify = SIGEV_SIGNAL;
	st_sigevent.sigev_signo = sig_no;
	st_sigevent.sigev_value.sival_ptr = &timer_id;

	ret_chk = timer_create(clockid, &st_sigevent, &timer_id);
	if (ret_chk == ERROR) {
		printf("tc_timer_timer_create_delete FAIL : timer_cretae FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	if (timer_id == NULL) {		/* check fail condition of timer_id */
		printf("tc_timer_timer_create_delete FAIL : timer_create FAIL, Timer ID not available\n");
		total_fail++;
		RETURN_ERR;
	}
	st_ret_prt = (struct posix_timer_s *)timer_id;

	if (st_ret_prt->pt_value.sival_ptr != st_sigevent.sigev_value.sival_ptr || st_ret_prt->pt_signo != st_sigevent.sigev_signo || st_ret_prt->pt_crefs != 1 || st_ret_prt->pt_owner != getpid() || st_ret_prt->pt_delay != 0) {
		ret_chk = timer_delete(timer_id);
		printf("tc_timer_timer_create_delete FAIL : timer_create FAIL, posix_timer_s for sigevent not available\n");
		total_fail++;
		RETURN_ERR;
	}

	st_timer_spec_val.it_value.tv_sec = 1;
	st_timer_spec_val.it_value.tv_nsec = 0;

	st_timer_spec_val.it_interval.tv_sec = 1;
	st_timer_spec_val.it_interval.tv_nsec = 0;

	ret_chk = timer_settime(timer_id, 0, &st_timer_spec_val, NULL);
	if (ret_chk == ERROR) {
		printf("tc_timer_timer_create_delete FAIL : timer_settime FAIL, Error No: %d\n", errno);
		timer_delete(timer_id);
		total_fail++;
		RETURN_ERR;
	}

	ret_chk = timer_delete(timer_id);
	if (ret_chk == ERROR) {
		printf("tc_timer_timer_create_delete FAIL : timer_delete FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	ret_chk = timer_create(CLOCK_REALTIME, NULL, &gtimer_id);
	if (ret_chk == ERROR) {
		printf("tc_timer_timer_create_delete FAIL : timer_create FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	st_ret_prt = (struct posix_timer_s *)gtimer_id;

	if (st_ret_prt->pt_value.sival_ptr != st_ret_prt || st_ret_prt->pt_signo != SIGALRM || st_ret_prt->pt_crefs != 1 || st_ret_prt->pt_owner != getpid() || st_ret_prt->pt_delay != 0) {
		timer_delete(gtimer_id);
		printf("tc_timer_timer_create_delete FAIL : timer_create FAIL, posix_timer_s for NULL not available\n");
		total_fail++;
		RETURN_ERR;
	}

	ret_chk = timer_delete(gtimer_id);
	if (ret_chk == ERROR) {
		printf("tc_timer_timer_create_delete FAIL : timer_delete FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	printf("tc_timer_timer_create_delete PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_timer_timer_set_get_time
* @brief                :arm/disarm and fetch state of POSIX per-process timer
* @scenario             :arm/disarm and fetch state of POSIX per-process timer
* API's covered         :timer_create, timer_settime, timer_gettime
* Preconditions         :Creation of timer_id(timer_create)
* Postconditions        :none
* @return               :void
*/
static void tc_timer_timer_set_get_time(void)
{
	int ret_chk = ERROR;
	clockid_t clockid = CLOCK_REALTIME;
	struct sigevent st_sigevent;
	struct itimerspec st_timer_spec_set;
	struct itimerspec st_timer_spec_get;
	timer_t timer_id;

	/* Set and enable alarm */
	st_sigevent.sigev_notify = SIGEV_SIGNAL;
	st_sigevent.sigev_signo = sig_no;
	st_sigevent.sigev_value.sival_ptr = &timer_id;
	ret_chk = timer_create(clockid, &st_sigevent, &timer_id);
	if (ret_chk == ERROR || timer_id == NULL) {
		printf("tc_timer_timer_set_get_time TIMER CREATE FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	st_timer_spec_set.it_interval.tv_sec = 1;
	st_timer_spec_set.it_interval.tv_nsec = 0;	/* interval; */
	st_timer_spec_set.it_value.tv_sec = 1;
	st_timer_spec_set.it_value.tv_nsec = 0;	/* expire; */

	ret_chk = timer_settime(timer_id, 0, &st_timer_spec_set, NULL);	/* Flag =1 :TIMER_ABSTIME */
	if (ret_chk != OK) {
		timer_delete(timer_id);
		printf("tc_timer_timer_set_get_time TIMER SETTIME FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}

	usleep(USECINT);

	ret_chk = timer_gettime(timer_id, &st_timer_spec_get);
	if ((ret_chk == ERROR) || (st_timer_spec_get.it_interval.tv_nsec < st_timer_spec_set.it_interval.tv_nsec) || (st_timer_spec_get.it_interval.tv_sec < st_timer_spec_set.it_interval.tv_sec) || (st_timer_spec_get.it_value.tv_sec < st_timer_spec_set.it_value.tv_sec) || (st_timer_spec_get.it_value.tv_nsec < st_timer_spec_set.it_value.tv_nsec)) {
		timer_delete(timer_id);
		printf("tc_timer_timer_set_get_time TIMER GETTIME FAIL, Error No: %d\n", errno);
		total_fail++;
		RETURN_ERR;
	}
	timer_delete(timer_id);
	printf("tc_timer_timer_set_get_time PASS\n");
	total_pass++;
}

/**
* @fn                   :tc_timer_timer_initialize
* @brief                :Boot up configuration of the POSIX timer facility.
* @brief                :Boot up configuration of the POSIX timer facility.
* API's covered         :timer_initialize
* Preconditions         :Creation of timer_id(timer_create)
* Postconditions        :none
* @return               :void
*/
static void tc_timer_timer_initialize(void)
{
	timer_t timer_id;
	clockid_t clockid = CLOCK_REALTIME;
	struct sigevent st_sigevent;
	FAR struct posix_timer_s *timer;
	FAR struct posix_timer_s *next;

	int initalloc_cnt = 0;
	int initfree_cnt = 0;
	int createalloc_cnt = 0;
	int createfree_cnt = 0;
	int finalalloc_cnt = 0;
	int finalfree_cnt = 0;

	/* Set and enable alarm */
	st_sigevent.sigev_notify = SIGEV_SIGNAL;
	st_sigevent.sigev_signo = sig_no;
	st_sigevent.sigev_value.sival_ptr = &timer_id;

	/* check the count for g_alloctimers and g_freetimers after timer_initialize */
	timer_initialize();

	for (timer = (FAR struct posix_timer_s *)g_alloctimers.head; timer; timer = next) {
		next = timer->flink;
		initalloc_cnt++;
	}

	for (timer = (FAR struct posix_timer_s *)g_freetimers.head; timer; timer = next) {
		next = timer->flink;
		initfree_cnt++;
	}

	/* check the count for g_alloctimers and g_freetimers after create now they change */
	timer_create(clockid, &st_sigevent, &timer_id);

	for (timer = (FAR struct posix_timer_s *)g_alloctimers.head; timer; timer = next) {
		next = timer->flink;
		createalloc_cnt++;
	}

	for (timer = (FAR struct posix_timer_s *)g_freetimers.head; timer; timer = next) {
		next = timer->flink;
		createfree_cnt++;
	}

	/* check the count for g_alloctimers and g_freetimers after timer_initialize now they change to original value */
	timer_initialize();

	for (timer = (FAR struct posix_timer_s *)g_alloctimers.head; timer; timer = next) {
		next = timer->flink;
		finalalloc_cnt++;
	}

	for (timer = (FAR struct posix_timer_s *)g_freetimers.head; timer; timer = next) {
		next = timer->flink;
		finalfree_cnt++;
	}

	if (initalloc_cnt != finalalloc_cnt || initfree_cnt != finalfree_cnt || createalloc_cnt == finalalloc_cnt || createfree_cnt == finalfree_cnt) {
		timer_delete(timer_id);
		printf("tc_timer_timer_initialize FAIL, Timer ID not available\n");
		total_fail++;
		RETURN_ERR;
	}
	timer_delete(timer_id);
	printf("tc_timer_timer_initialize PASS\n");
	total_pass++;
}

/****************************************************************************
 * Name: timer
 ****************************************************************************/

int timer_main(void)
{
	tc_timer_timer_create_delete();
	tc_timer_timer_set_get_time();
	tc_timer_timer_initialize();

	return 0;
}
