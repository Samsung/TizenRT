/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * apss/examples/rtc/rtc_main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <tinyara/rtc.h>

#define TEST_TIME 300	/* 300 secs */
#define TEST_STACKSIZE 4096
#define RTC_DEVPATH "/dev/rtc0"

bool rtctest_started;

#ifdef CONFIG_RTC_ALARM
#define RTC_ALARM_SIGNO     1
#define RTC_ALARM_ID        0
#define ABS_TIME_MODE       0
#define REL_TIME_MODE       1
#define CANCEL_MODE         2
#define INVALID             3

static struct rtc_time g_alarm_time;
static int g_command_mode = INVALID;
#endif

static void rtc_test_stop(void)
{
	/* Has the rtc test already started? */
	if (rtctest_started) {
		/* Stop the test. */
		printf("Stopping, not stopped yet.\n");
		rtctest_started = FALSE;
	} else {
		printf("There is no active rtc test.\n");
	}
}

static void *rtc_test(void *arg)
{
	int ret;
	int fd;

	/* populate prev_time to epoch (01-01-1970) in order reset RTC as it provides number of seconds passed starting from epoch */
	struct rtc_time prev_time;
	struct rtc_time next_time;
	struct tm prev_tm;
	struct tm next_tm;
	time_t prev;
	time_t next;
	int total_cnt = 0;

	printf("=== RTC Test ===\n");

	/* This sample runs forever with printing the log repeatedly on every TEST_TIME secs until "rtc stop".
	 * You can check and compare the time which is from Serial Log Tool like Teraterm and the printed log from line 140.
	 */

	memset(&next_time, 0, sizeof(struct rtc_time));
	memset(&prev_tm, 0, sizeof(struct tm));
	memset(&next_tm, 0, sizeof(struct tm));

	fd = open(RTC_DEVPATH, O_RDWR);
	if (fd < 0) {
		printf("ERROR : Fail to open rtc.\n");
		return NULL;
	}
	/* Read initial RTC time */
	ret = ioctl(fd, RTC_RD_TIME, (unsigned long)&prev_time);
	if (ret < 0) {
		printf("ERROR : Fail to get from RTC.\n");
		close(fd);
		return NULL;
	}

	sched_lock();
	while (rtctest_started) {
		sleep(TEST_TIME);
		total_cnt++;

		ret = ioctl(fd, RTC_RD_TIME, (unsigned long)&next_time);
		if (ret < 0) {
			printf("ERROR : Fail to get from RTC.\n");
			close(fd);
			sched_unlock();
			return NULL;
		} else {
			memcpy(&next_tm, (struct tm *)&next_time, sizeof(struct tm));
			memcpy(&prev_tm, (struct tm *)&prev_time, sizeof(struct tm));

			prev = mktime(&prev_tm);
			next = mktime(&next_tm);

			printf("[Total %dm passed] Time Difference : %ds\n", total_cnt * (TEST_TIME / 60), next - prev);

			/* Update the current time into 'prev_time' for next comparison. */
			memcpy(&prev_time, &next_time, sizeof(struct rtc_time));

			prev_time = next_time;

		}
	}

	close(fd);
	sched_unlock();
	printf("RTC Test Stopped well\n");
	return NULL;
}

/****************************************************************************
 * Name: show_usage
 ****************************************************************************/

static void show_usage(void)
{
	printf("\nUsage:  rtc\n");
	printf("\tor: rtc stop\n");
	printf("\t\tStart, or Stop rtc test\n");
#ifdef CONFIG_RTC_ALARM
	printf("\tor rtc alarm [-r seconds] [-a hh:mm:ss] [-c]\n");
	printf("\t\tset, or cancel rtc alarm\n");
	printf("Where:\n");
	printf("\t-a hh:mm:ss \n");
	printf("\t\tset alarm by absolute time in hh:mm:ss format.\n");
	printf("\t-r <seconds> \n");
	printf("\t\tset alarm by relative time. The number of seconds until the alarm expires.\n");
	printf("\t-c\n\t\tCancel previously set alarm\n");
	printf("\t\t");
#endif
}

#ifdef CONFIG_RTC_ALARM
/****************************************************************************
 * Name: alarm_handler
 ****************************************************************************/

static void alarm_handler(int signo, FAR siginfo_t *info, FAR void *ucontext)
{
	int ret;
	int fd;
	int secs;
	int almndx;
	FAR struct rtc_time alarm_recieve_time;

	fd = open(RTC_DEVPATH, O_WRONLY);
	if (fd < 0) {
		printf("ERROR: Failed to open %s: %d\n", RTC_DEVPATH, get_errno());
		return;
	}
	almndx = info->si_value.sival_int;
	if (almndx >= 0 && almndx < CONFIG_RTC_NALARMS) {
		ret = ioctl(fd, RTC_RD_TIME, (unsigned long)&alarm_recieve_time);
		if (ret < 0) {
			printf("ERROR: RTC_RD_TIME ioctl failed: %d\n", get_errno());
			close(fd);
			return;
		}
		if (g_command_mode == REL_TIME_MODE) {
			secs = (alarm_recieve_time.tm_hour - g_alarm_time.tm_hour) * 3600 + (alarm_recieve_time.tm_min - g_alarm_time.tm_min) * 60 + (alarm_recieve_time.tm_sec - g_alarm_time.tm_sec);
			printf("Alarm received after %d seconds\n", secs);
		} else {
			printf("Alarm recieved at %d:%d:%d\n",alarm_recieve_time.tm_hour, alarm_recieve_time.tm_min, alarm_recieve_time.tm_sec);
		}
	}
	close(fd);
}

/****************************************************************************
 * Name: set_absolute_alarm
 ****************************************************************************/

int set_absoulute_alarm(int fd, int hrs, int mins, int seconds)
{
	int ret;
	int cur_time;
	int alarm_time;
	FAR struct rtc_setalarm_s setalarm;

	setalarm.id = RTC_ALARM_ID;
	setalarm.pid = 0;

	ret = ioctl(fd, RTC_RD_TIME, (unsigned long)&g_alarm_time);
	if (ret < 0) {
		printf("ERROR: RTC_RD_TIME ioctl failed: %d\n", get_errno());
		close(fd);
		return ERROR;
	}
	alarm_time = hrs * 3600 + mins * 60 + seconds;
	cur_time = g_alarm_time.tm_hour * 3600 + g_alarm_time.tm_min * 60 + g_alarm_time.tm_sec;
	if (alarm_time < cur_time) {
		printf( "ERROR: Please enter proper time which is more than current time (%d:%d:%d)\n", g_alarm_time.tm_hour, g_alarm_time.tm_min, g_alarm_time.tm_sec);
		close(fd);
		return ERROR;
	}
	
	setalarm.time = g_alarm_time;
	setalarm.time.tm_sec = seconds;     
	setalarm.time.tm_min = mins;
	setalarm.time.tm_hour = hrs;
	setalarm.signo = RTC_ALARM_SIGNO;
	setalarm.sigvalue.sival_int = RTC_ALARM_ID;

	ret = ioctl(fd, RTC_SET_ALARM, (unsigned long)((uintptr_t)&setalarm));
	if (ret < 0) {
		printf("ERROR: RTC_SET_ALARM ioctl failed: %d\n", get_errno());
		close(fd);
		return ERROR;
	}
	printf("Alarm is set to expire at %d:%d:%d\n", hrs, mins, seconds);
	sleep((alarm_time) - (cur_time) + 1);
	return OK;
}

/****************************************************************************
 * Name: set_relative_alarm
 ****************************************************************************/

int set_relative_alarm(int fd, int seconds)
{
	int ret;
	FAR struct rtc_setrelative_s setrel;

	setrel.id = RTC_ALARM_ID;
	setrel.pid = 0;
	setrel.reltime = (time_t)seconds;
	setrel.signo = RTC_ALARM_SIGNO;
	setrel.sigvalue.sival_int = RTC_ALARM_ID;
	ret = ioctl(fd, RTC_SET_RELATIVE, (unsigned long)((uintptr_t)&setrel));
	if (ret < 0) {
		printf("ERROR: RTC_SET_RELATIVE ioctl failed: %d\n", get_errno());
		close(fd);
		return ERROR;
	}
	ret = ioctl(fd, RTC_RD_TIME, (unsigned long)&g_alarm_time);
	if (ret < 0) {
		printf("ERROR: RTC_RD_TIME ioctl failed: %d\n", get_errno());
		close(fd);
		return ERROR;
	}
	printf("Alarm is set to expire in %lu seconds\n", seconds);
	sleep(seconds + 1);
	return OK;
}

/****************************************************************************
 * Name: cancel_alarm
 ****************************************************************************/

int cancel_alarm(int fd)
{
	int ret;
	ret = ioctl(fd, RTC_CANCEL_ALARM, (unsigned long)RTC_ALARM_ID);
	if (ret < 0) {
		printf("ERROR: RTC_CANCEL_ALARM ioctl failed: %d\n", get_errno());
		close(fd);
		return ERROR;
	}

	printf("Alarm has been canceled\n");
	return OK;
}

int rtc_alarm_main(int argc, char *argv[])
{
	int seconds = 0;
	int mins = 0;
	int hrs = 0;
	int fd;
	int ret;
	int opt;
	FAR struct sigaction act;
	sigset_t set;
	g_command_mode = INVALID;

	while ((opt = getopt(argc, argv, ":a:r:c")) != ERROR) {
		switch (opt) {
		case 'a':
			/* -a: alarm is set by absolute time (RTC_SET_ALARM ioctl call).
			*/
			g_command_mode = ABS_TIME_MODE;
			break;
		case 'r':
			/* -r: alarm is set by relative time (RTC_SET_RELATIVE ioctl call).
			*/
			g_command_mode = REL_TIME_MODE;
			break;
		case 'c':
			g_command_mode = CANCEL_MODE;
			break;
		default:
			printf("<unknown parameter '-%c'>\n\n", opt);
			/* fall through */

		case '?':
		case ':':
			g_command_mode = INVALID;
		}
	}
	/* Open the RTC driver */

	printf("Opening %s\n", RTC_DEVPATH);

	fd = open(RTC_DEVPATH, O_RDWR);
	if (fd < 0) {
		printf("ERROR: Failed to open %s: %d\n", RTC_DEVPATH, get_errno());
		return ERROR;
	}

	sigemptyset(&set);
	sigaddset(&set, RTC_ALARM_SIGNO);
	ret = sigprocmask(SIG_UNBLOCK, &set, NULL);
	if (ret != OK) {
		printf("ERROR: sigprocmask failed: %d\n", get_errno());
		close(fd);
		return ERROR;
	}

	act.sa_sigaction = alarm_handler;
	act.sa_flags = SA_SIGINFO;
	sigfillset(&act.sa_mask);
	sigdelset(&act.sa_mask, RTC_ALARM_SIGNO);
	ret = sigaction(RTC_ALARM_SIGNO, &act, NULL);
	if (ret < 0) {
		printf("ERROR: sigaction failed: %d\n", get_errno());
		close(fd);
		return ERROR;
	}

	switch (g_command_mode) {
	case ABS_TIME_MODE:
		sscanf(argv[2], "%d:%d:%d", &hrs, &mins, &seconds);
		ret = set_absoulute_alarm(fd, hrs, mins, seconds);
		break;
	case REL_TIME_MODE:
		seconds = strtoul(argv[2], NULL, 10);
		ret = set_relative_alarm(fd, seconds);
		break;
	case CANCEL_MODE:
		ret = cancel_alarm(fd);
		break;
	case INVALID:
	default:
		show_usage();
		ret = ERROR;
	}

	close(fd);
	return ret;
}
#endif

/****************************************************************************
 * rtc_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int rtc_main(int argc, char *argv[])
#endif
{
#ifndef CONFIG_DISABLE_PTHREAD
	int ret;
	pthread_t rtctest;
	pthread_attr_t rtctest_attr;

	if (argc > 1) {
		if (!strncmp(argv[1], "stop", strlen("stop") + 1)) {
			/* stop the rtc test */
			rtc_test_stop();
			return OK;
#ifdef CONFIG_RTC_ALARM
		} else if (!strncmp(argv[1], "alarm", strlen("alarm") + 1)) {
			ret = rtc_alarm_main(argc - 1, ++argv);
			return ret;
#endif
		} else {
			show_usage();
			return ERROR;
		}
	}

	/* Has the rtc daemon already started? */

	if (!rtctest_started) {
		int ret;

		/* No.. start it now */
		/* Then start the rtc test */

		rtctest_started = TRUE;

		pthread_attr_init(&rtctest_attr);
		rtctest_attr.stacksize = TEST_STACKSIZE;
		rtctest_attr.priority = SCHED_PRIORITY_MAX;
		rtctest_attr.inheritsched = PTHREAD_EXPLICIT_SCHED;

		ret = pthread_create(&rtctest, &rtctest_attr, rtc_test, NULL);
		if (ret != OK) {
			printf("ERROR: Failed to start the rtc test pthread: %d\n", get_errno());
			rtctest_started = FALSE;
			return ERROR;
		}
		pthread_setname_np(rtctest, "RTC");

		ret = pthread_detach(rtctest);
		if (ret != OK) {
			printf("ERROR: Failed to detach the rtc test pthread: %d\n", get_errno());
			pthread_cancel(rtctest);
			rtctest_started = FALSE;
			return ERROR;
		}
	} else {
		printf("rtc test already started\n");
	}
#else
	rtctest_started = TRUE;
	rtc_test(NULL);
#endif

	return OK;
}
