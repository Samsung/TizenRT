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
#include <sys/ioctl.h>
#include <tinyara/os_api_test_drv.h>
#ifndef CONFIG_BUILD_PROTECTED
#include "../../os/kernel/timer/timer.h"
#endif
#include "tc_internal.h"

#define USECINT 10000000

static int sig_no = SIGRTMIN;

#ifndef CONFIG_BUILD_PROTECTED
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
	clockid_t clockid;
#ifdef CONFIG_CLOCK_MONOTONIC
	clockid = CLOCK_MONOTONIC;
#endif
	struct sigevent st_sigevent;
	struct posix_timer_s *st_ret_prt;
	struct itimerspec st_timer_spec_val;

	/* Set and enable alarm */

	st_sigevent.sigev_notify = SIGEV_SIGNAL;
	st_sigevent.sigev_signo = sig_no;
	st_sigevent.sigev_value.sival_ptr = &timer_id;

	/* Check for clock ID != CLOCK_REALTIME */

#ifdef CONFIG_CLOCK_MONOTONIC
	ret_chk = timer_create(clockid, &st_sigevent, &timer_id);
	TC_ASSERT_EQ("timer_create", ret_chk, ERROR);
	TC_ASSERT_EQ("timer_create", errno, EINVAL);
#endif

	clockid = CLOCK_REALTIME;

	/* Check for NULL timer ID */

	ret_chk = timer_create(clockid, &st_sigevent, NULL);
	TC_ASSERT_EQ("timer_create", ret_chk, ERROR);
	TC_ASSERT_EQ("timer_create", errno, EINVAL);

	/* Check for NULL timer ID */

	ret_chk = timer_delete(NULL);
	TC_ASSERT_EQ("timer_delete", ret_chk, ERROR);
	TC_ASSERT_EQ("timer_delete", errno, EINVAL);

	ret_chk = timer_create(clockid, &st_sigevent, &timer_id);
	TC_ASSERT_NEQ("timer_create", ret_chk, ERROR);
	TC_ASSERT_NEQ("timer_create", timer_id, NULL);

	st_ret_prt = (struct posix_timer_s *)timer_id;
	TC_ASSERT_EQ_CLEANUP("timer_create", st_ret_prt->pt_value.sival_ptr, st_sigevent.sigev_value.sival_ptr, timer_delete(timer_id));
	TC_ASSERT_EQ_CLEANUP("timer_create", st_ret_prt->pt_signo, st_sigevent.sigev_signo, timer_delete(timer_id));
	TC_ASSERT_EQ_CLEANUP("timer_create", st_ret_prt->pt_crefs, 1, timer_delete(timer_id));
	TC_ASSERT_EQ_CLEANUP("timer_create", st_ret_prt->pt_owner, getpid(), timer_delete(timer_id));
	TC_ASSERT_EQ_CLEANUP("timer_create", st_ret_prt->pt_delay, 0, timer_delete(timer_id));

	st_timer_spec_val.it_value.tv_sec = 1;
	st_timer_spec_val.it_value.tv_nsec = 0;

	st_timer_spec_val.it_interval.tv_sec = 1;
	st_timer_spec_val.it_interval.tv_nsec = 0;

	ret_chk = timer_settime(timer_id, 0, &st_timer_spec_val, NULL);
	TC_ASSERT_EQ_ERROR_CLEANUP("timer_settime", ret_chk, OK, errno, timer_delete(timer_id));

	ret_chk = timer_delete(timer_id);
	TC_ASSERT_NEQ("timer_delete", ret_chk, ERROR);

	ret_chk = timer_create(CLOCK_REALTIME, NULL, &gtimer_id);
	TC_ASSERT_NEQ("timer_create", ret_chk, ERROR);

	st_ret_prt = (struct posix_timer_s *)gtimer_id;

	TC_ASSERT_EQ_CLEANUP("timer_create", st_ret_prt->pt_value.sival_ptr, st_ret_prt, timer_delete(gtimer_id));
	TC_ASSERT_EQ_CLEANUP("timer_create", st_ret_prt->pt_crefs, 1, timer_delete(gtimer_id));
	TC_ASSERT_EQ_CLEANUP("timer_create", st_ret_prt->pt_owner, getpid(), timer_delete(gtimer_id));
	TC_ASSERT_EQ_CLEANUP("timer_create", st_ret_prt->pt_delay, 0, timer_delete(gtimer_id));

	ret_chk = timer_delete(gtimer_id);
	TC_ASSERT_NEQ("timer_delete", ret_chk, ERROR);

	TC_SUCCESS_RESULT();
}
#endif	/* CONFIG_BUILD_PROTECTED */

#ifndef CONFIG_DISABLE_POSIX_TIMERS
/**
* @fn                   :tc_timer_timer_getoverrun
* @brief                :queue a signal to the process for a given timer at any point in time
* @scenario             :If the timer_getoverrun() function succeeds, it will return the timer expiration overrun count
* @scenario              EINVAL if The timerid argument does not correspond to an ID returned by timer_create() but not yet deleted by timer_delete().
* API's covered         :timer_create, timer_getoverrun
* Preconditions         :Creation of timer_id(timer_create)
* Postconditions        :none
* @return               :void
*/
static void tc_timer_timer_getoverrun(void)
{
	int ret_chk = 0;
	timer_t timer_id;
	clockid_t clockid = CLOCK_REALTIME;
	struct sigevent st_sigevent;

	/* Set and enable alarm */

	st_sigevent.sigev_notify = SIGEV_SIGNAL;
	st_sigevent.sigev_signo = sig_no;
	st_sigevent.sigev_value.sival_ptr = &timer_id;

	/* Check for NULL clock ID != CLOCK_REALTIME */

	ret_chk = timer_create(clockid, &st_sigevent, &timer_id);
	TC_ASSERT_NEQ("timer_create", ret_chk, ERROR);
	TC_ASSERT_NEQ("timer_create", timer_id, NULL);

	ret_chk = timer_getoverrun(timer_id);
	TC_ASSERT_EQ_CLEANUP("timer_getoverrun", ret_chk, ERROR, timer_delete(timer_id));
	TC_ASSERT_EQ_CLEANUP("timer_getoverrun", errno, ENOSYS, timer_delete(timer_id));

	ret_chk = timer_delete(timer_id);
	TC_ASSERT_NEQ("timer_delete", ret_chk, ERROR);

	TC_SUCCESS_RESULT();
}
#endif

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
	TC_ASSERT_NEQ("timer_create", ret_chk, ERROR);
	TC_ASSERT_NEQ("timer_create", timer_id, NULL);

	st_timer_spec_set.it_interval.tv_sec = 1;
	st_timer_spec_set.it_interval.tv_nsec = 0;	/* interval; */
	st_timer_spec_set.it_value.tv_sec = 1;
	st_timer_spec_set.it_value.tv_nsec = 0;	/* expire; */

	/* Null timer ID check for timer_settime */

	ret_chk = timer_settime(NULL, 0, &st_timer_spec_set, NULL);
	TC_ASSERT_EQ_CLEANUP("timer_settime", ret_chk, ERROR, timer_delete(timer_id));
	TC_ASSERT_EQ_CLEANUP("timer_settime", errno, EINVAL, timer_delete(timer_id));

	/* Null timer value check for timer_settime */

	ret_chk = timer_settime(timer_id, 0, NULL, NULL);
	TC_ASSERT_EQ_CLEANUP("timer_settime", ret_chk, ERROR, timer_delete(timer_id));
	TC_ASSERT_EQ_CLEANUP("timer_settime", errno, EINVAL, timer_delete(timer_id));

	/* Null it_value parameter check for timer_settime */

	st_timer_spec_set.it_value.tv_sec = 0;
	ret_chk = timer_settime(timer_id, 0, &st_timer_spec_set, NULL);
	TC_ASSERT_EQ_CLEANUP("timer_settime", ret_chk, OK, timer_delete(timer_id));

	/* Null it_value parameter check for timer_settime */

	st_timer_spec_set.it_value.tv_sec = -1;
	ret_chk = timer_settime(timer_id, 0, &st_timer_spec_set, NULL);
	TC_ASSERT_EQ_CLEANUP("timer_settime", ret_chk, OK, timer_delete(timer_id));
	st_timer_spec_set.it_value.tv_sec = 1;

	/* Check if TIMER_ABSTIME (Flag = 1) is selected */

	ret_chk = timer_settime(timer_id, 1, &st_timer_spec_set, NULL);
	TC_ASSERT_EQ_CLEANUP("timer_settime", ret_chk, OK, timer_delete(timer_id));

	ret_chk = timer_settime(timer_id, 0, &st_timer_spec_set, NULL);	/* Flag =1 :TIMER_ABSTIME */
	TC_ASSERT_EQ_ERROR_CLEANUP("timer_settime", ret_chk, OK, errno, timer_delete(timer_id));

	usleep(USECINT);

	/* Null timer ID check for timer_gettime */

	ret_chk = timer_gettime(NULL, &st_timer_spec_get);
	TC_ASSERT_EQ("timer_gettime", ret_chk, ERROR);
	TC_ASSERT_EQ("timer_gettime", errno, EINVAL);

	ret_chk = timer_gettime(timer_id, &st_timer_spec_get);
	TC_ASSERT_EQ_ERROR_CLEANUP("timer_gettime", ret_chk, OK, errno, timer_delete(timer_id));
	TC_ASSERT_GEQ_CLEANUP("timer_gettime", st_timer_spec_get.it_interval.tv_nsec, st_timer_spec_set.it_interval.tv_nsec, timer_delete(timer_id));
	TC_ASSERT_GEQ_CLEANUP("timer_gettime", st_timer_spec_get.it_interval.tv_sec, st_timer_spec_set.it_interval.tv_sec, timer_delete(timer_id));
	TC_ASSERT_GEQ_CLEANUP("timer_gettime", st_timer_spec_get.it_value.tv_sec, st_timer_spec_set.it_value.tv_sec, timer_delete(timer_id));
	TC_ASSERT_GEQ_CLEANUP("timer_gettime", st_timer_spec_get.it_value.tv_nsec, st_timer_spec_set.it_value.tv_nsec, timer_delete(timer_id));

	timer_delete(timer_id);
	TC_SUCCESS_RESULT();
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
	int fd;
	int ret_chk;
	fd = tc_get_drvfd();

	ret_chk = ioctl(fd, TESTIOC_TIMER_INITIALIZE_TEST, 0);
	TC_ASSERT_EQ("timer_initialize", ret_chk, OK);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: timer
 ****************************************************************************/

#ifndef CONFIG_DISABLE_POSIX_TIMERS
#ifndef CONFIG_BUILD_PROTECTED
/**
 * @fn                   :tc_timer_gethandle_null
 * @brief                :Test timer_gethandle() with NULL timerid
 * @scenario             :Pass NULL to timer_gethandle() and verify it returns NULL
 * API's covered         :timer_gethandle
 * Preconditions         :none
 * Postconditions        :none
 * @return               :void
 */
static void tc_timer_gethandle_null(void)
{
	struct posix_timer_s *ret;

	ret = timer_gethandle(NULL);
	TC_ASSERT_EQ("timer_gethandle", ret, NULL);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_timer_gethandle_valid
 * @brief                :Test timer_gethandle() with valid timerid
 * @scenario             :Create valid timer, verify timer_gethandle() returns same pointer
 * API's covered         :timer_create, timer_gethandle, timer_delete
 * Preconditions         :none
 * Postconditions        :none
 * @return               :void
 */
static void tc_timer_gethandle_valid(void)
{
	int ret_chk;
	timer_t timer_id;
	clockid_t clockid = CLOCK_REALTIME;
	struct sigevent st_sigevent;
	struct posix_timer_s *ret;

	st_sigevent.sigev_notify = SIGEV_SIGNAL;
	st_sigevent.sigev_signo = SIGRTMIN;
	st_sigevent.sigev_value.sival_ptr = &timer_id;

	ret_chk = timer_create(clockid, &st_sigevent, &timer_id);
	TC_ASSERT_NEQ("timer_create", ret_chk, ERROR);
	TC_ASSERT_NEQ("timer_create", timer_id, NULL);

	ret = timer_gethandle(timer_id);
	TC_ASSERT_NEQ("timer_gethandle", ret, NULL);
	TC_ASSERT_EQ("timer_gethandle", ret, (struct posix_timer_s *)timer_id);

	timer_delete(timer_id);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_timer_gethandle_forged
 * @brief                :Test timer_gethandle() with forged handle
 * @scenario             :Pass stack variable address (forged handle), verify NULL return
 * API's covered         :timer_gethandle
 * Preconditions         :none
 * Postconditions        :none
 * @return               :void
 */
static void tc_timer_gethandle_forged(void)
{
	struct posix_timer_s *ret;
	int stack_var = 0x12345678;

	/* Pass address of stack variable as forged timer handle */
	ret = timer_gethandle((timer_t)&stack_var);
	TC_ASSERT_EQ("timer_gethandle", ret, NULL);

	/* Also test with completely random pointer */
	ret = timer_gethandle((timer_t)0x12345678);
	TC_ASSERT_EQ("timer_gethandle", ret, NULL);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_timer_gethandle_after_delete
 * @brief                :Test timer_gethandle() with deleted timer handle
 * @scenario             :Create timer, delete it, verify timer_gethandle() returns NULL
 * API's covered         :timer_create, timer_delete, timer_gethandle
 * Preconditions         :none
 * Postconditions        :none
 * @return               :void
 */
static void tc_timer_gethandle_after_delete(void)
{
	int ret_chk;
	timer_t timer_id;
	clockid_t clockid = CLOCK_REALTIME;
	struct sigevent st_sigevent;
	struct posix_timer_s *ret;

	st_sigevent.sigev_notify = SIGEV_SIGNAL;
	st_sigevent.sigev_signo = SIGRTMIN;
	st_sigevent.sigev_value.sival_ptr = &timer_id;

	ret_chk = timer_create(clockid, &st_sigevent, &timer_id);
	TC_ASSERT_NEQ("timer_create", ret_chk, ERROR);

	/* Delete the timer */
	ret_chk = timer_delete(timer_id);
	TC_ASSERT_EQ("timer_delete", ret_chk, OK);

	/* Verify timer_gethandle returns NULL for deleted timer */
	ret = timer_gethandle(timer_id);
	TC_ASSERT_EQ("timer_gethandle", ret, NULL);

	TC_SUCCESS_RESULT();
}
#endif /* CONFIG_BUILD_PROTECTED */

/**
 * @fn                   :tc_timer_delete_forged_handle
 * @brief                :Test timer_delete() with forged handle
 * @scenario             :Pass forged handle to timer_delete(), verify ERROR/EINVAL
 * API's covered         :timer_delete
 * Preconditions         :none
 * Postconditions        :none
 * @return               :void
 */
static void tc_timer_delete_forged_handle(void)
{
	int ret_chk;
	int stack_var = 0x12345678;

	ret_chk = timer_delete((timer_t)&stack_var);
	TC_ASSERT_EQ("timer_delete", ret_chk, ERROR);
	TC_ASSERT_EQ("timer_delete", errno, EINVAL);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_timer_settime_forged_handle
 * @brief                :Test timer_settime() with forged handle
 * @scenario             :Pass forged handle to timer_settime(), verify ERROR/EINVAL
 * API's covered         :timer_settime
 * Preconditions         :none
 * Postconditions        :none
 * @return               :void
 */
static void tc_timer_settime_forged_handle(void)
{
	int ret_chk;
	int stack_var = 0x12345678;
	struct itimerspec st_timer_spec;

	st_timer_spec.it_value.tv_sec = 1;
	st_timer_spec.it_value.tv_nsec = 0;
	st_timer_spec.it_interval.tv_sec = 0;
	st_timer_spec.it_interval.tv_nsec = 0;

	ret_chk = timer_settime((timer_t)&stack_var, 0, &st_timer_spec, NULL);
	TC_ASSERT_EQ("timer_settime", ret_chk, ERROR);
	TC_ASSERT_EQ("timer_settime", errno, EINVAL);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_timer_gettime_forged_handle
 * @brief                :Test timer_gettime() with forged handle
 * @scenario             :Pass forged handle to timer_gettime(), verify ERROR/EINVAL
 * API's covered         :timer_gettime
 * Preconditions         :none
 * Postconditions        :none
 * @return               :void
 */
static void tc_timer_gettime_forged_handle(void)
{
	int ret_chk;
	int stack_var = 0x12345678;
	struct itimerspec st_timer_spec;

	ret_chk = timer_gettime((timer_t)&stack_var, &st_timer_spec);
	TC_ASSERT_EQ("timer_gettime", ret_chk, ERROR);
	TC_ASSERT_EQ("timer_gettime", errno, EINVAL);

	TC_SUCCESS_RESULT();
}
#endif /* CONFIG_DISABLE_POSIX_TIMERS */

/****************************************************************************
 * Name: timer
 ****************************************************************************/

int timer_tc_main(void)
{
#ifndef CONFIG_BUILD_PROTECTED
	tc_timer_timer_create_delete();
#endif	

#ifndef CONFIG_DISABLE_POSIX_TIMERS
	tc_timer_timer_getoverrun();

#ifndef CONFIG_BUILD_PROTECTED
	/* Timer security tests for timer_gethandle() */
	tc_timer_gethandle_null();
	tc_timer_gethandle_valid();
	tc_timer_gethandle_forged();
	tc_timer_gethandle_after_delete();
#endif
	tc_timer_delete_forged_handle();
	tc_timer_settime_forged_handle();
	tc_timer_gettime_forged_handle();
#endif                     /* CONFIG_DISABLE_POSIX_TIMERS */
	tc_timer_timer_set_get_time();
	tc_timer_timer_initialize();

	return 0;
}
