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
#include <tinyara/rtc.h>

#define TEST_TIME 300	/* 300 secs */
#define TEST_STACKSIZE 4096

bool rtctest_started;

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
	struct rtc_time prev_time;
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

	memset(&prev_time, 0, sizeof(struct rtc_time));
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
		} else {
			printf("\nUsage: rtc\n");
			printf("   or: rtc stop\n");
			printf("Start, or Stop rtc accuracy daemon\n");
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
