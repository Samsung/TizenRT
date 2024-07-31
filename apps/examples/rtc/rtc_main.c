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

bool rtctest_started;

#ifdef CONFIG_RTC_ALARM
static struct rtc_time g_alarm_time[CONFIG_RTC_NALARMS];
static bool g_alarm_received[CONFIG_RTC_NALARMS];
#endif

static void rtc_test_stop(void)
{
	/* Has the rtc accuracy test already started? */
	if (rtctest_started) {
		/* Stop the test. */
		printf("Stopping, not stopped yet.\n");
		rtctest_started = FALSE;
	} else {
		printf("There is no active rtc accuracy test.\n");
	}
}

static void *rtc_test(void *arg)
{
	int ret;
	int fd;

	/* populate prev_time to epoch (01-01-1970) in order reset RTC as it provides number of seconds passed starting from epoch */
	struct rtc_time prev_time = RTC_TIME_INITIALIZER(1970, 1, 1, 0, 0, 0);
	struct rtc_time next_time;
	struct tm prev_tm;
	struct tm next_tm;
	time_t prev;
	time_t next;
	int total_cnt = 0;

	printf("=== RTC Accuracy Test ===\n");

	/* This sample runs forever with printing the log repeatedly on every TEST_TIME secs until "rtc stop".
	 * You can check and compare the time which is from Serial Log Tool like Teraterm and the printed log from line 140.
	 */

	memset(&next_time, 0, sizeof(struct rtc_time));
	memset(&prev_tm, 0, sizeof(struct tm));
	memset(&next_tm, 0, sizeof(struct tm));

	fd = open("/dev/rtc0", O_RDWR);
	if (fd < 0) {
		printf("ERROR : Fail to open rtc.\n");
		return NULL;
	}

	/* Initialize the RTC time as 0 */
	ret = ioctl(fd, RTC_SET_TIME, (unsigned long)&prev_time);
	if (ret < 0) {
		printf("ERROR : Fail to set RTC init time.\n");
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
	printf("Rtc Accuracy Test Stopped well\n");
	return NULL;
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

	fd = open(CONFIG_EXAMPLES_RTC_ALARM_DEVPATH, O_WRONLY);
	if (fd < 0) {
		fprintf(stderr, "ERROR: Failed to open %s: %d\n", CONFIG_EXAMPLES_RTC_ALARM_DEVPATH, errno);
		return;
	}
	almndx = info->si_value.sival_int;
	if (almndx >= 0 && almndx < CONFIG_RTC_NALARMS) {
		ret = ioctl(fd, RTC_RD_TIME, (unsigned long)&alarm_recieve_time);
		if (ret < 0) {
			fprintf(stderr, "ERROR: RTC_RD_TIME ioctl failed: %d\n", errno);
			close(fd);
			return;
		}
		close(fd);
		secs = (alarm_recieve_time.tm_min - g_alarm_time[almndx].tm_min) * 60 + (alarm_recieve_time.tm_sec - g_alarm_time[almndx].tm_sec);
		printf("Alarm %d received after %d seconds\n", almndx, secs);
		g_alarm_received[almndx] = false;
	}
}

/****************************************************************************
 * Name: show_usage
 ****************************************************************************/

static void show_usage(FAR const char *progname)
{
	fprintf(stderr, "\nUsage:  rtc\n");
	fprintf(stderr, "\tor: rtc stop\n");
	fprintf(stderr, "\t\tStart, or Stop rtc accuracy test\n");
	fprintf(stderr, "\tor %s alarm [-r seconds] [-a minutes seconds] [-c]\n", progname);
	fprintf(stderr, "\t\tset, or cancel rtc alarm\n");
	fprintf(stderr, "Where:\n");
	fprintf(stderr, "\t-a <minutes> <seconds> \n");
	fprintf(stderr, "\t\tset alarm by absolute time in minutes and seconds (default alarm id: 0)\n");
	fprintf(stderr, "\t-r <seconds> \n");
	fprintf(stderr, "\t\tset alarm by relative time. The number of seconds until the alarm expires (default alarm id: 0).\n");
	fprintf(stderr, "\t-c\n\t\tCancel previously set alarm\n");
	fprintf(stderr, "\t\t");
}

int rtc_alarm_main(int argc, char *argv[])
{
	int seconds = 0;
	int mins = 0;
	bool badarg = false;
	bool cancelmode = false;
	bool relset = false;
	int alarmid = 0;
	int fd;
	int ret;
	FAR struct rtc_time time;
	FAR struct sigaction act;
	FAR struct rtc_setalarm_s setalarm;
	FAR	struct rtc_setrelative_s setrel;
	sigset_t set;

	if (!strncmp(argv[2], "-a", strlen("-a") + 1) || !strncmp(argv[2], "-r", strlen("-r") + 1) || !strncmp(argv[2], "-c", strlen("-c") + 1)) {
		switch (argv[2][1]) {
			case 'a':
				/* -a: Select alarm id, if alarm is set, then it is
				 * done by absolute time (RTC_SET_ALARM ioctl call).
				 */
				break;
			case 'r':
				/* -r: Select alarm id, if alarm is set,then it is
				 *  done by relative time (RTC_SET_RELATIVE ioctl call).
				 */
				relset = true;
				break;
			case 'c':
				cancelmode = true;
				break;
			default:
				/* fall through */
				badarg = true;
		}
	}

	if (optind >= argc) {
		badarg = true;
	}
	if (badarg) {
		show_usage(argv[0]);
		return EXIT_FAILURE;
	}

	if (!cancelmode) {
		/* Get the number of seconds until the alarm expiration */
		if(relset) {
			seconds = strtoul(argv[3], NULL, 10);
			if (seconds < 1) {
				fprintf(stderr, "ERROR: Invalid number of seconds: %lu\n", seconds);
				show_usage(argv[0]);
				return EXIT_FAILURE;
			}
		}
		else {
			mins = strtoul(argv[3], NULL, 10);
			seconds = strtoul(argv[4], NULL, 10);
		}
	}

	/* Open the RTC driver */

	printf("Opening %s\n", CONFIG_EXAMPLES_RTC_ALARM_DEVPATH);

	fd = open(CONFIG_EXAMPLES_RTC_ALARM_DEVPATH, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "ERROR: Failed to open %s: %d\n", CONFIG_EXAMPLES_RTC_ALARM_DEVPATH, errno);
		return EXIT_FAILURE;
	}

	if (cancelmode) {
		ret = ioctl(fd, RTC_CANCEL_ALARM, (unsigned long)alarmid);
		if (ret < 0) {
			fprintf(stderr, "ERROR: RTC_CANCEL_ALARM ioctl failed: %d\n", errno);
			close(fd);
			return EXIT_FAILURE;
		}

		printf("Alarm %d has been canceled\n", alarmid);
	}
	else { /*!cancelmode */

		sigemptyset(&set);
		sigaddset(&set, CONFIG_EXAMPLES_RTC_ALARM_SIGNO);
		ret = sigprocmask(SIG_UNBLOCK, &set, NULL);
		if (ret != OK) {
			fprintf(stderr, "ERROR: sigprocmask failed: %d\n", errno);
			return EXIT_FAILURE;
		}

		act.sa_sigaction = alarm_handler;
		act.sa_flags = SA_SIGINFO;
		sigfillset(&act.sa_mask);
		sigdelset(&act.sa_mask, CONFIG_EXAMPLES_RTC_ALARM_SIGNO);
		ret = sigaction(CONFIG_EXAMPLES_RTC_ALARM_SIGNO, &act, NULL);
		if (ret < 0) {
			fprintf(stderr, "ERROR: sigaction failed: %d\n", errno);
		}

		if (relset) {
			/* Set the alarm */

			setrel.id = alarmid;
			setrel.pid = 0;
			setrel.reltime = (time_t)seconds;
			setrel.signo = CONFIG_EXAMPLES_RTC_ALARM_SIGNO;
			setrel.sigvalue.sival_int = alarmid;
			ret = ioctl(fd, RTC_SET_RELATIVE, (unsigned long)((uintptr_t)&setrel));
			if (ret < 0) {
				fprintf(stderr, "ERROR: RTC_SET_RELATIVE ioctl failed: %d\n", errno);
				close(fd);
				return EXIT_FAILURE;
			}
			ret = ioctl(fd, RTC_RD_TIME, (unsigned long)&g_alarm_time[setalarm.id]);
			if (ret < 0) {
				fprintf(stderr, "ERROR: RTC_RD_TIME ioctl failed: %d\n", errno);
				close(fd);
				return EXIT_FAILURE;
			}
			printf("Alarm %d set in %lu seconds\n", alarmid, seconds);
			sleep(seconds + 1);
		}
		else {

			/* Set the alarm */

			setalarm.id = alarmid;
			setalarm.pid = 0;

			ret = ioctl(fd, RTC_RD_TIME, (unsigned long)&time);
			if (ret < 0) {
				fprintf(stderr, "ERROR: RTC_RD_TIME ioctl failed: %d\n", errno);
				close(fd);
				return EXIT_FAILURE;
			}
			if(mins * 60 + seconds < time.tm_min * 60 + time.tm_sec) {
				printf( "ERROR: Please Enter proper time which is more than current time\n");
				close(fd);
				return EXIT_FAILURE;
			}
			setalarm.time.tm_sec = seconds;
			setalarm.time.tm_min = mins;
			setalarm.signo = CONFIG_EXAMPLES_RTC_ALARM_SIGNO;
			setalarm.sigvalue.sival_int = alarmid;

			ret = ioctl(fd, RTC_SET_ALARM, (unsigned long)((uintptr_t)&setalarm));
			if (ret < 0) {
				fprintf(stderr, "ERROR: RTC_SET_ALARM ioctl failed: %d\n", errno);
				close(fd);
				return EXIT_FAILURE;
			}
			ret = ioctl(fd, RTC_RD_TIME, (unsigned long)&g_alarm_time[setalarm.id]);
			if (ret < 0) {
				fprintf(stderr, "ERROR: RTC_RD_TIME ioctl failed: %d\n", errno);
				close(fd);
				return EXIT_FAILURE;
			}
			printf("Alarm %d set in %lu seconds\n", alarmid, (mins * 60 + seconds) - (time.tm_min * 60 + time.tm_sec));
			sleep((mins * 60 + seconds) - (time.tm_min * 60 + time.tm_sec) + 1);
		}
	}

	close(fd);
	return EXIT_SUCCESS;
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
	pthread_t rtctest;
	pthread_attr_t rtctest_attr;

	if (argc > 1) {
		if (!strncmp(argv[1], "stop", strlen("stop") + 1)) {
			/* stop the rtc accuracy test */
			rtc_test_stop();
			return OK;
#ifdef CONFIG_RTC_ALARM
		} else if(!strncmp(argv[1], "alarm", strlen("alarm") + 1)) {
			rtc_alarm_main(argc, argv);
			return OK;
#endif
		} else {
			show_usage(argv[0]);
			return ERROR;
		}
	}

	/* Has the rtc daemon already started? */

	if (!rtctest_started) {
		int ret;

		/* No.. start it now */
		/* Then start the rtc accuracy test */

		rtctest_started = TRUE;

		pthread_attr_init(&rtctest_attr);
		rtctest_attr.stacksize = TEST_STACKSIZE;
		rtctest_attr.priority = SCHED_PRIORITY_MAX;
		rtctest_attr.inheritsched = PTHREAD_EXPLICIT_SCHED;

		ret = pthread_create(&rtctest, &rtctest_attr, rtc_test, NULL);
		if (ret != OK) {
			printf("ERROR: Failed to start the rtc accuracy test pthread: %d\n", errno);
			rtctest_started = FALSE;
			return ERROR;
		}
		pthread_setname_np(rtctest, "RTC");

		ret = pthread_detach(rtctest);
		if (ret != OK) {
			printf("ERROR: Failed to detach the rtc accuracy test pthread: %d\n", errno);
			pthread_cancel(rtctest);
			rtctest_started = FALSE;
			return ERROR;
		}
	} else {
		printf("rtc accuracy test already started\n");
	}
#else
	rtctest_started = TRUE;
	rtc_test(NULL);
#endif

	return OK;
}
