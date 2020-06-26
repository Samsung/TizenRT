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
 * apps/examples/timer/timer_main.c
 *
 *   Copyright (C) 2015 Gregory Nutt. All rights reserved.
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

#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>

#include <tinyara/timer.h>
#include <tinyara/clock.h>
#include <tinyara/irq.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifndef TIMER_DEVNAME
#  define TIMER_DEVNAME  "/dev/timer%d"
#endif

#ifndef EXAMPLE_TIMER_INTERVAL
#  define EXAMPLE_TIMER_INTERVAL USEC_PER_SEC
#endif

#ifndef EXAMPLE_TIMER_NSAMPLES
#  define EXAMPLE_TIMER_NSAMPLES 10
#endif

#define TIMER_THEAD_SIZE                2048
#define TIMER_THEAD_PRIORITY            255

#ifdef CONFIG_EXAMPLES_TIMER_FRT_MEASUREMENT
#define ALLOWABLE_PERCENTAGE (0.1)
#endif

struct timer_args {
	int fd;
	int count;
	int intval;
#ifdef CONFIG_EXAMPLES_TIMER_FRT_MEASUREMENT
	int devno;
#endif
};
/****************************************************************************
 * Private Functions
 ****************************************************************************/

static pthread_addr_t timer_thread(pthread_addr_t arg)
{
	struct timer_args *pargs = (struct timer_args *)arg;
	struct timer_notify_s notify;
	int count = pargs->count;
	int intval = pargs->intval;
	int fd = pargs->fd;
#ifdef CONFIG_EXAMPLES_TIMER_FRT_MEASUREMENT
	int frt_fd;
	char path[_POSIX_PATH_MAX];
	int frt_dev;
	struct timer_status_s before;
	struct timer_status_s after;
	uint32_t time_diff;
	uint32_t expected_time;
#endif

	/*
	 * Register a callback for notifications using the configured signal.
	 * NOTE: If no callback is attached, the timer stop at the first interrupt.
	 */
	fprintf(stdout, "Attach timer handler\n");

	notify.arg   = NULL;
	notify.pid   = (pid_t)getpid();

	if (ioctl(fd, TCIOC_NOTIFICATION,
			(unsigned long)((uintptr_t)&notify)) < 0) {
		fprintf(stderr, "ERROR: Failed to set the timer handler: %d\n", errno);
		goto error;
	}

	/* Set the timer interval */
	fprintf(stdout, "Set timer interval to %lu, repeat %d\n",
			(unsigned long)intval, count);

	if (ioctl(fd, TCIOC_SETTIMEOUT, intval) < 0) {
		fprintf(stderr, "ERROR: Failed to set the timer interval: %d\n", errno);
		goto error;
	}

	/* Start the timer */
	fprintf(stdout, "Start the timer\n");
	if (ioctl(fd, TCIOC_START, 0) < 0) {
		fprintf(stderr, "ERROR: Failed to start the timer: %d\n", errno);
		goto error;
	}

#ifdef CONFIG_EXAMPLES_TIMER_FRT_MEASUREMENT
	/* Open the FRTimer device */
	if (pargs->devno == 0) {
		frt_dev = 1;
	} else {
		frt_dev = 0;
	}
	snprintf(path, _POSIX_PATH_MAX, TIMER_DEVNAME, frt_dev);
	fprintf(stdout, "FRT Open %s\n", path);

	expected_time = count * intval;

	frt_fd = open(path, O_RDONLY);
	if (frt_fd < 0) {
		fprintf(stderr, "ERROR: Failed to open Free Run Timer: %d\n", errno);
		goto error;
	}
	if (ioctl(frt_fd, TCIOC_SETMODE, MODE_FREERUN) < 0) {
		fprintf(stderr, "ERROR: Failed to set Free Run Timer: %d\n", errno);
		goto error;
	}
	if (ioctl(frt_fd, TCIOC_START, TRUE) < 0) {
		fprintf(stderr, "ERROR: Failed to start Free Run Timer: %d\n", errno);
		goto error;
	}

	if (ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&before) < 0) {
		fprintf(stderr, "ERROR: Failed to get Free Run Timer status: %d\n", errno);
		goto error;
	}

#endif
	while (count--) {
		fin_wait();
	}
#ifdef CONFIG_EXAMPLES_TIMER_FRT_MEASUREMENT
	if (ioctl(frt_fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&after) < 0) {
		fprintf(stderr, "ERROR: Failed to get Free Run Timer status: %d\n", errno);
	}
#endif
	/* In high resolution timers, timer has to be stopped immediately once
	the sigwait handling is completed. If not timer interrupt would keep
	posting the events which would results in stack overflows. */
	if (ioctl(fd, TCIOC_STOP, 0) < 0) {
		fprintf(stderr, "ERROR: Failed to stop the timer: %d\n", errno);
	}

#ifdef CONFIG_EXAMPLES_TIMER_FRT_MEASUREMENT
	if (ioctl(frt_fd, TCIOC_STOP, 0) < 0) {
		fprintf(stderr, "ERROR: Failed to stop the Free Run Timer: %d\n", errno);
	}
#endif
	fprintf(stdout, "Stop the timer\n");
#ifdef CONFIG_EXAMPLES_TIMER_FRT_MEASUREMENT
	fprintf(stdout, ">> Summary\n");

	time_diff = after.timeleft - before.timeleft;
	fprintf(stdout, "\tExpected %d, Estimated %d, Ratio %.9f\n", expected_time, time_diff, ((float)time_diff / (float)expected_time) - 1.0);
	if (((float)expected_time * (1.0 + ALLOWABLE_PERCENTAGE / 100.0)) > (float)time_diff) {
		fprintf(stdout, "\tPASS : Estimated Elapsed time is within the expected range.\n");
	} else {
		fprintf(stdout, "\tFAIL : There are many missing timer interrupts.\n");
	}
#endif
error:
	pthread_exit(NULL);
#ifdef CONFIG_EXAMPLES_TIMER_FRT_MEASUREMENT
	close(frt_fd);
#endif
	return NULL;
}

static pthread_t create_timer_thread(void *arg)
{
	pthread_t tid;
	pthread_attr_t attr;
	struct sched_param sparam;
	int ret = OK;

	ret = pthread_attr_init(&attr);
	if (ret != 0) {
		fprintf(stderr, "failed to set pthread init(%d)\n", ret);
		return -ret;
	}

	ret = pthread_attr_setschedpolicy(&attr, SCHED_RR);
	if (ret != 0) {
		fprintf(stderr, "failed to set policy(%d)\n", ret);
		return -ret;
	}

	ret = pthread_attr_setstacksize(&attr, TIMER_THEAD_SIZE);
	if (ret != 0) {
		fprintf(stderr, "failed to set stack size(%d)\n", ret);
		return -ret;
	}

	sparam.sched_priority = TIMER_THEAD_PRIORITY;
	ret = pthread_attr_setschedparam(&attr, &sparam);
	if (ret != 0) {
		fprintf(stderr, "failed to set sched param(%d)\n", ret);
		return -ret;
	}

	ret = pthread_create(&tid, &attr, timer_thread, arg);
	if (ret != 0) {
		fprintf(stderr, "failed to create pthread(%d)\n", ret);
		return -ret;
	}

	pthread_setname_np(tid, "time sigwait thread");
	pthread_detach(tid);

	return tid;
}

#define USAGE \
		"Usage: timer\n" \
		"  or:  timer [-f] TIMER NUMBER [OPTION]...\n" \
		"  or:  timer [OPTION]...\n" \
		"Run the timer corresponding to the number.\n" \
		"\n" \
		"Arguments\n" \
		"  -f [NUMBER]  \'/dev/timer<>\'\n" \
		"               DEFAULT 0\n" \
		"  -n [REPEAT]  Number of samples to repeat test.\n" \
		"               DEFAULT 10\n" \
		"  -t [INTR]    Timer interval\n" \
		"               Microsecond unit. (DEFAULT 1000000usec)\n" \
		"     -h        display this help and exit\n"

/****************************************************************************
 * Public Functions
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int timer_example_main(int argc, char *argv[])
#endif
{
	pthread_t tid;
	struct timer_args args;
	int fd;
	int opt = 0;

	char path[_POSIX_PATH_MAX];
	int dev = 0;
	int intval = -1;
	int repeat = -1;

	optind = 0;

	while ((opt = getopt(argc, argv, "t:n:f:h")) != -1) {
		switch (opt) {
		case 't':
			intval = strtol(optarg, NULL, 10);
			break;
		case 'n':
			repeat = strtol(optarg, NULL, 10);
			break;
		case 'f':
			dev = strtol(optarg, NULL, 10);
			break;
		case 'h':
		default:
			fprintf(stdout, USAGE);
			return opt == 'h' ? EXIT_SUCCESS : EXIT_FAILURE;
		}
	}

	if (optind > argc) {
		fprintf(stderr, "timer: invalid option -- \'%s\'\n", argv[optind]);
		fprintf(stdout, USAGE);
		return EXIT_SUCCESS;
	}

	/* Open the timer device */
	snprintf(path, _POSIX_PATH_MAX, TIMER_DEVNAME, dev);
	fprintf(stdout, "Open %s\n", path);

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "ERROR: Failed to open %s: %d\n", path, errno);
		return EXIT_FAILURE;
	}

	if (repeat <= 0) {
		repeat = EXAMPLE_TIMER_NSAMPLES;
	}

	if (intval <= 0) {
		intval = EXAMPLE_TIMER_INTERVAL;
	}

	args.count = repeat;
	args.fd = fd;
	args.intval = intval;
#ifdef CONFIG_EXAMPLES_TIMER_FRT_MEASUREMENT
	args.devno = dev;
#endif

	/* Create Time Handler Thread */
	tid = create_timer_thread((void *)&args);
	if (tid < 0) {
		close(fd);
		return EXIT_FAILURE;
	}

	/* Wait a bit showing timer thread */
	pthread_join(tid, NULL);

	/* Close the timer driver */
	fprintf(stdout, "Finished\n");
	close(fd);

	return EXIT_SUCCESS;
}
