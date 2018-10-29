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
#include <signal.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>

#include <tinyara/timer.h>
#include <tinyara/clock.h>

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

#ifndef EXAMPLE_TIMER_SIGNO
#  define EXAMPLE_TIMER_SIGNO    17
#endif

#define TIMER_THEAD_SIZE                512
#define TIMER_THEAD_PRIORITY            100

/****************************************************************************
 * Private Data
 ****************************************************************************/
static sem_t g_sem;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void timer_sighandler(int signo, FAR siginfo_t *siginfo,
							 FAR void *context)
{
	/*
	 * Does nothing in this example except for increment a count of signals
	 * received.
	 *
	 * NOTE: The use of signal handler is not recommended if you are concerned
	 * about the signal latency.  Instead, a dedicated, high-priority thread
	 * that waits on sigwaitinfo() is recommended.  High priority is required
	 * if you want a deterministic wake-up time when the signal occurs.
	 */
	sem_post(&g_sem);
}

static pthread_addr_t timer_thread(pthread_addr_t arg)
{
	int nbr = 0;
	int i = (int)arg;

	while (i--) {
		sem_wait(&g_sem);
		fprintf(stdout, "time limits(%d)\n", ++nbr);
	}

	pthread_exit(NULL);

	return NULL;
}

static pthread_t create_timer_thread(void *arg)
{
	pthread_t tid;
	pthread_attr_t attr;
	struct sched_param sparam;
	int ret = OK;

	ret = sem_init(&g_sem, 0, 0);
	if (ret != 0) {
		fprintf(stderr, "failed to set semaphore init(%d)\n", ret);
		return -ret;
	}

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

	pthread_setname_np(tid, "time handler thread");
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
		"               Millisecond unit. (DEFAULT 1000msec)\n" \
		"     -h        display this help and exit\n"

/****************************************************************************
 * Public Functions
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int timer_main(int argc, char *argv[])
#endif
{
	struct timer_notify_s notify;
	struct sigaction act;
	int tid = 0;
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
			intval = strtol(optarg, NULL, 10) * MSEC_PER_SEC;
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

	if (optind >= argc) {
		fprintf(stderr, "timer: invalid option -- \'%s\'\n", argv[optind]);
		fprintf(stdout, USAGE);
		return EXIT_SUCCESS;
	}

	/* Open the timer device */
	sprintf(path, TIMER_DEVNAME, dev);
	fprintf(stdout, "Open %s\n", path);

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "ERROR: Failed to open %s: %d\n", path, errno);
		return EXIT_FAILURE;
	}

	if (repeat <= 0) {
		repeat = EXAMPLE_TIMER_NSAMPLES;
	}

	/* Create Time Handler Thread */
	tid = create_timer_thread((void *)repeat);
	if (tid < 0) {
		close(fd);
		return EXIT_FAILURE;
	}

	if (intval <= 0) {
		intval = EXAMPLE_TIMER_INTERVAL;
	}

	/* Set the timer interval */
	fprintf(stdout, "Set timer interval to %lu\n",
			(unsigned long)intval);

	if (ioctl(fd, TCIOC_SETTIMEOUT, intval) < 0) {
		fprintf(stderr, "ERROR: Failed to set the timer interval: %d\n", errno);
		close(fd);
		return EXIT_FAILURE;
	}

	/*
	 * Attach a signal handler to catch the notifications.  NOTE that using
	 * signal handler is very slow.  A much more efficient thing to do is to
	 * create a separate pthread that waits on sigwaitinfo() for timer events.
	 * Much less overhead in that case.
	 */
	act.sa_sigaction = timer_sighandler;
	act.sa_flags     = SA_SIGINFO;

	(void)sigfillset(&act.sa_mask);
	(void)sigdelset(&act.sa_mask, EXAMPLE_TIMER_SIGNO);

	if (sigaction(EXAMPLE_TIMER_SIGNO, &act, NULL) != OK) {
		fprintf(stderr, "ERROR: Fsigaction failed: %d\n", errno);
		close(fd);
		return EXIT_FAILURE;
	}

	/*
	 * Register a callback for notifications using the configured signal.
	 * NOTE: If no callback is attached, the timer stop at the first interrupt.
	 */
	fprintf(stdout, "Attach timer handler\n");

	notify.arg   = NULL;
	notify.pid   = getpid();
	notify.signo = EXAMPLE_TIMER_SIGNO;

	if (ioctl(fd, TCIOC_NOTIFICATION,
			(unsigned long)((uintptr_t)&notify)) < 0) {
		fprintf(stderr, "ERROR: Failed to set the timer handler: %d\n", errno);
		close(fd);
		return EXIT_FAILURE;
	}

	/* Start the timer */
	fprintf(stdout, "Start the timer\n");
	if (ioctl(fd, TCIOC_START, 0) < 0) {
		fprintf(stderr, "ERROR: Failed to start the timer: %d\n", errno);
		close(fd);
		return EXIT_FAILURE;
	}

	/* Wait a bit showing timer thread */
	pthread_join(tid, NULL);
	sem_destroy(&g_sem);

	/* Stop the timer */
	fprintf(stdout, "Stop the timer\n");

	if (ioctl(fd, TCIOC_STOP, 0) < 0) {
		fprintf(stderr, "ERROR: Failed to stop the timer: %d\n", errno);
		close(fd);
		return EXIT_FAILURE;
	}

	/* Detach the signal handler */
	act.sa_handler = SIG_DFL;
	sigaction(EXAMPLE_TIMER_SIGNO, &act, NULL);

	/* Close the timer driver */
	fprintf(stdout, "Finished\n");
	close(fd);

	return EXIT_SUCCESS;
}
