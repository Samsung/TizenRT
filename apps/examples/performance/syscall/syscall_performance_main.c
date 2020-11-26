/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

/// @file syscall_performance_main.c

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>
#include <time.h>
#include <tinyara/time.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>

#define NUM_LOOPS	1000000
#define SEC_10	10
#define TEST_MSGLEN	31
#define TEST_TIMEDSEND_NMSGS	3
#define SIGEV_SIGNAL	1		/* Notify via signal */

static int sig_no = SIGRTMIN;

/*
 * measure_performance : variadic macro for time measurements for function calls
 *
 * Args-
 * fun : function
 * sc_no : System call number
 * args : variable number of arguments to be passed to function
 *
 */
#define measure_performance(fun, sc_no, args...) \
{ \
	int i = 0; \
	int pass = 0; \
	struct timespec stime; \
	struct timespec etime; \
	struct timespec res_time; \
	clock_gettime(CLOCK_REALTIME, &stime); \
	for (i = 0; i < NUM_LOOPS; i++) { \
		fun(args); \
		pass++; \
	} \
	clock_gettime(CLOCK_REALTIME, &etime); \
	if (etime.tv_nsec - stime.tv_nsec < 0) { \
		res_time.tv_sec = etime.tv_sec - stime.tv_sec - 1; \
		res_time.tv_nsec = etime.tv_nsec - stime.tv_nsec + 1000000000; \
	} else { \
		res_time.tv_sec = etime.tv_sec - stime.tv_sec; \
		res_time.tv_nsec = etime.tv_nsec - stime.tv_nsec; \
	} \
	printf("%s - sys_call%s - [pass = %d] - timediff -> (%lld.%09ld secs)\n", #fun, #sc_no, pass, (long long)res_time.tv_sec, res_time.tv_nsec); \
}


/*
 * @fn                   :syscall_perf_clock_getres
 * @description          :Measuring performance for clock_getres
 * @return               :void
 */
static void syscall_perf_clock_getres(void)
{
	struct timespec st_res;

	/* It only work for realtime. */

	measure_performance(clock_getres, 2, CLOCK_REALTIME, &st_res);
}

/*
 * @fn                   :syscall_perf_setenv
 * @description          :Measuring performance for setenv
 * @return               :void
 */
static void syscall_perf_setenv(void)
{
	const char *psz_name = "abc";
	const char *psz_value = "xyz";

	measure_performance(setenv, 3, psz_name, psz_value, 1);
}

/*
 * @fn                   :syscall_perf_unsetenv
 * @description          :Measuring performance for unsetenv
 * @return               :void
 */
static void syscall_perf_unsetenv(void)
{
	const char *pusz_name = "arv";

	measure_performance(unsetenv, 1, pusz_name);
}

/*
 * @fn                   :syscall_perf_clearenv
 * @description          :Measuring performance for clearenv
 * @return               :void
 */
static void syscall_perf_clearenv(void)
{
	measure_performance(clearenv, 0);
}

/*
 * @fn                   :syscall_perf_mq_open
 * @description          :Measuring performance for mq_open
 * @return               :void
 */
static void syscall_perf_mq_open(void)
{
	struct mq_attr attr;

	/* Fill in attributes for message queue */

	attr.mq_maxmsg = TEST_TIMEDSEND_NMSGS;
	attr.mq_msgsize = TEST_MSGLEN;
	attr.mq_flags = 0;

	measure_performance(mq_open, 6, "t_mqueue", O_WRONLY, 0666, &attr);
}

/*
 * @fn                   :syscall_perf_mq_timedsend
 * @description          :Measuring performance for mq_timedsend
 * @return               :void
 */
static void syscall_perf_mq_timedsend(void)
{
	char msg_buffer[TEST_MSGLEN];
	static mqd_t g_timedsend_mqfd;

	measure_performance(mq_timedsend, 5, g_timedsend_mqfd, msg_buffer, TEST_MSGLEN, 42, NULL);
}

/*
 * @fn                   :syscall_perf_timer_settime
 * @description          :Measuring performance for timer_settime
 * @return               :void
 */
static void syscall_perf_timer_settime(void)
{
	int ret_chk;
	timer_t timer_id;
	struct sigevent st_sigevent;
	clockid_t clockid = CLOCK_REALTIME;

	/* Set and enable alarm */

	st_sigevent.sigev_notify = SIGEV_SIGNAL;
	st_sigevent.sigev_signo = sig_no;
	st_sigevent.sigev_value.sival_ptr = &timer_id;

	ret_chk = timer_create(clockid, &st_sigevent, &timer_id);

	measure_performance(timer_settime, 4, timer_id, 0, NULL, NULL);
}

/****************************************************************************
 * Name: Syscall Performance
 ****************************************************************************/
int syscall_performance_main(void)
{
	/* System Call 0 */
	sched_lock();
	syscall_perf_clearenv();
	sched_unlock();

	/* System Call 1 */
	sched_lock();
	syscall_perf_unsetenv();
	sched_unlock();

	/* System Call 2 */
	sched_lock();
	syscall_perf_clock_getres();
	sched_unlock();

	/* System Call 3 */
	sched_lock();
	syscall_perf_setenv();
	sched_unlock();

	/* System Call 4 */
	sched_lock();
	syscall_perf_timer_settime();
	sched_unlock();

	/* System Call 5 */
	sched_lock();
	syscall_perf_mq_timedsend();
	sched_unlock();

	/* System Call 6 */
	sched_lock();
	syscall_perf_mq_open();
	sched_unlock();

	return 0;
}
