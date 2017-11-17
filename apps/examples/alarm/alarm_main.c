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
/****************************************************************************
 * examples/alarm/alarm_main.c
 *
 *   Copyright (C) 2016 Gregory Nutt. All rights reserved.
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

#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <semaphore.h>
#include <sched.h>

#include <tinyara/rtc.h>

enum alarm_mode_e {
	ALARM_MODE_NULL,
	ALARM_MODE_SETTIME,
	ALARM_MODE_READ,
	ALARM_MODE_CANCLE,
};
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define USAGE(prog, n) \
	fprintf(stdout, "USAGE:\n" \
	"   %s [-a <alarmid>] [-s <seconds>] [-cr]\n" \
	"Where:\n" \
	"   -a <alarmid>\n" \
	"      <alarmid> selects the alarm: 0..%d (default: 0)\n" \
	"   -c   Cancel previously set alarm\n" \
	"   -r   Read previously set alarm\n" \
	"   -s <seconds>\n" \
	"      <seconds> The number of seconds until the alarm expires.\n", prog, n)

#define LOGINFO(fmt, ...) \
	fprintf(stdout, "INFO: "fmt, ##__VA_ARGS__)

#define LOGERROR(fmt, ...) \
	fprintf(stderr, "ERROR: "fmt, ##__VA_ARGS__)

/****************************************************************************
 * Private Data
 ****************************************************************************/

static bool daemon_started;

static pid_t daemon_pid;

static bool received[CONFIG_RTC_NALARMS];

static sem_t wait;
/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: alarm_handler
 ****************************************************************************/

static void alarm_handler(int signo, FAR siginfo_t *info, FAR void *ucontext)
{
	int idx = info->si_value.sival_int;

	if (idx >= 0 && idx < CONFIG_RTC_NALARMS) {
		received[idx] = true;
	}

	sem_post_from_isr(&wait);
}

/****************************************************************************
 * Name: alarm_daemon
 ****************************************************************************/

static int alarm_daemon(int argc, FAR char *argv[])
{
	struct sigaction act;
	sigset_t set;
	int ret;
	int i;

	/* Indicate that we are running */

	daemon_started = true;

	printf("alarm_daemon: Running\n");

	/* Make sure that the alarm signal is unmasked */

	(void) sigemptyset(&set);
	(void) sigaddset(&set, CONFIG_EXAMPLES_ALARM_SIGNO);

	ret = sigprocmask(SIG_UNBLOCK, &set, NULL);
	if (ret != OK) {
		LOGERROR("sigprocmask failed: %d\n", errno);
		goto errout;
	}

	/* Register alarm signal handler */

	act.sa_sigaction = alarm_handler;
	act.sa_flags = SA_SIGINFO;

	(void) sigfillset(&act.sa_mask);
	(void) sigdelset(&act.sa_mask, CONFIG_EXAMPLES_ALARM_SIGNO);

	ret = sigaction(CONFIG_EXAMPLES_ALARM_SIGNO, &act, NULL);
	if (ret < 0) {
		LOGERROR("sigaction failed: %d\n", errno);
		goto errout;
	}

	/* Now loop forever, waiting for alarm signals */

	for (;;) {

		sem_wait(&wait);
		/* Check if any alarms fired.
		 *
		 * NOTE that there are race conditions here... if we missing an alarm,
		 * we will just report it a half second late.
		 */

		for (i = 0; i < CONFIG_RTC_NALARMS; i++) {
			if (received[i]) {
				LOGINFO("alarm_demon: alarm %d received\n", i);
				received[i] = false;
			}
		}

		/* Now wait a little while and poll again.  If a signal is received
		 * this should cuase us to awken earlier.
		 */
	}

errout:
	daemon_started = false;

	printf("alarm_daemon: Terminating\n");

	return EXIT_FAILURE;
}

/****************************************************************************
 * Name: start_daemon
 ****************************************************************************/

static int start_daemon(void)
{
	if (!daemon_started) {

		sem_init(&wait, 0, 0);

		daemon_pid = task_create("alarm_daemon",
				CONFIG_EXAMPLES_ALARM_PRIORITY,
				CONFIG_EXAMPLES_ALARM_STACKSIZE,
				alarm_daemon,
				NULL);

		if (daemon_pid < 0) {
			LOGERROR("Failed to start alarm_daemon: %d\n", errno);

			return -errno;
		}

		printf("alarm_daemon started\n");
		usleep(500 * 1000L);
	}

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * alarm_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int alarm_main(int argc, FAR char *argv[])
#endif
{
	long seconds = 0;
	enum alarm_mode_e mode = ALARM_MODE_NULL;
	int opt;
	int alarmid = 0;
	int fd;
	int ret;

	optind = 0;

	/* Parse commandline parameters. NOTE: getopt() is not thread safe nor re-entrant.
	 * To keep its state proper for the next usage, it is necessary to parse to
	 * the end of the line even if an error occurs.  If an error occurs, this
	 * logic just sets 'badarg' and continues.
	 */

	while ((opt = getopt(argc, argv, "a:s:cr")) != ERROR) {
		switch (opt) {
		case 'a':

			/* -a: Select alarm id */
			alarmid = strtol(optarg, NULL, 0);
			if (alarmid < 0 || alarmid >= CONFIG_RTC_NALARMS) {
				goto errarg;
			}
			break;
		case 's':
			mode = ALARM_MODE_SETTIME;

			/* Get the number of seconds until the alarm expiration */

			seconds = strtol(optarg, NULL, 0);
			if (seconds < 1) {
				LOGERROR("Invalid number of seconds: %lu\n", seconds);
				goto errarg;
			}
			break;

		case 'c':
			mode = ALARM_MODE_CANCLE;
			break;

		case 'r':
			mode = ALARM_MODE_READ;
			break;

		case '?':
		default:
			LOGERROR("<unknown parameter '-%s'>\n\n", argv[optind]);
			goto errarg;
		}
	}

	/* Make sure that the alarm daemon is running */

	if (start_daemon() < 0) {
		return EXIT_FAILURE;
	}

	/* Open the RTC driver */

	printf("Opening %s\n", CONFIG_EXAMPLES_ALARM_DEVPATH);

	fd = open(CONFIG_EXAMPLES_ALARM_DEVPATH, O_WRONLY);
	if (fd < 0) {
		LOGERROR("Failed to open %s: %d\n",
				CONFIG_EXAMPLES_ALARM_DEVPATH, errno);
		return EXIT_FAILURE;
	}

	switch (mode) {
	case ALARM_MODE_CANCLE: {
		ret = ioctl(fd, RTC_CANCEL_ALARM, (unsigned long) alarmid);
		if (ret < 0) {
			LOGERROR("RTC_CANCEL_ALARM ioctl failed: %d\n", errno);

			(void) close(fd);
			return EXIT_FAILURE;
		}

		printf("Alarm %d has been canceled\n", alarmid);
	}
	break;

	case ALARM_MODE_SETTIME: {
		struct rtc_setrelative_s setrel;

		/* Set the alarm */

		setrel.id = alarmid;
		setrel.signo = CONFIG_EXAMPLES_ALARM_SIGNO;
		setrel.pid = daemon_pid;
		setrel.reltime = (time_t) seconds;

		setrel.sigvalue.sival_int = alarmid;

		ret = ioctl(fd, RTC_SET_RELATIVE,
				(unsigned long) ((uintptr_t) &setrel));
		if (ret < 0) {
			LOGERROR("RTC_SET_RELATIVE ioctl failed: %d\n", errno);

			(void) close(fd);
			return EXIT_FAILURE;
		}

		printf("Alarm %d set in %lu seconds\n", alarmid, seconds);
		}
	break;

	case ALARM_MODE_READ: {
		struct rtc_rdalarm_s rd = { 0 };
		long timeleft;
		time_t now;

		rd.id = alarmid;
		time(&now);

		ret = ioctl(fd, RTC_RD_ALARM, (unsigned long) ((uintptr_t) &rd));
		if (ret < 0) {
			LOGERROR("RTC_RD_ALARM ioctl failed: %d\n", errno);

			(void) close(fd);
			return EXIT_FAILURE;
		}

		/* Some of the TizenRT RTC implementations do not support alarms
		 * longer than one month. There RTC_RD_ALARM can return partial
		 * calendar values without month and year fields.
		 * TODO: fix this in lower layers?
		 */

		if (rd.time.tm_year > 0) {

			/*
			 * Normal sane case, assuming we did not actually request an
			 * alarm expiring in year 1900.
			 */

			timeleft = mktime((struct tm *) &rd.time) - now;
		} else {
			struct tm now_tm;

			/*
			 * Periodic extend "partial" alarms by "unfolding" months,
			 * until we get alarm that is in future. Note that mktime()
			 * normalizes fields that are out of their valid values,
			 * so we don't have to handle carry to tm_year by ourselves.
			 */

			gmtime_r(&now, &now_tm);
			rd.time.tm_mon = now_tm.tm_mon;
			rd.time.tm_year = now_tm.tm_year;

			do {
				timeleft = mktime((struct tm *) &rd.time) - now;
				if (timeleft < 0) {
					rd.time.tm_mon++;
				}
			} while (timeleft < 0);
		}

		printf("Alarm %d is %s with %ld seconds to expiration\n", alarmid,
				rd.active ? "active" : "inactive", timeleft);
		}
		break;
	default:
		break;
	}

	(void) close(fd);

	return EXIT_SUCCESS;

errarg:
	USAGE(argv[0], CONFIG_RTC_NALARMS - 1);
	return EXIT_FAILURE;
}
