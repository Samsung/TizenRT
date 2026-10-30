/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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
 * apps/testing/ostest/posixtimer.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <assert.h>
#include <errno.h>
#include <sched.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include <tinyara/signal.h>

#include "ostest.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

#define MY_TIMER_SIGNAL SIGRTMIN
#define SIGVALUE_INT    42

/****************************************************************************
 * Private Data
 ****************************************************************************/

static sem_t sem;
static int g_nsigreceived = 0;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void timer_expiration(int signo, siginfo_t *info, void *ucontext)
{
	sigset_t oldset;
	sigset_t allsigs;
	int status;

	printf("timer_expiration: Received signal %d\n", signo);

	g_nsigreceived++;

	/* Check signo */

	if (signo != MY_TIMER_SIGNAL) {
		printf("timer_expiration: ERROR expected signo=%d\n",
		       MY_TIMER_SIGNAL);
		ASSERT(false);
	}

	/* Check siginfo */

	if (info->si_value.sival_int != SIGVALUE_INT) {
		printf("timer_expiration: ERROR sival_int=%d expected %d\n",
		        info->si_value.sival_int, SIGVALUE_INT);
		ASSERT(false);
	} else {
		printf("timer_expiration: sival_int=%d\n", info->si_value.sival_int);
	}

	if (info->si_signo != MY_TIMER_SIGNAL) {
		printf("timer_expiration: ERROR expected si_signo=%d, got=%d\n",
		         MY_TIMER_SIGNAL, info->si_signo);
		ASSERT(false);
	}

	if (info->si_code == SI_TIMER) {
		printf("timer_expiration: si_code=%d (SI_TIMER)\n", info->si_code);
	} else {
		printf("timer_expiration: ERROR si_code=%d, expected SI_TIMER=%d\n",
		       info->si_code, SI_TIMER);
		ASSERT(false);
	}

	/* Check ucontext_t */

	printf("timer_expiration: ucontext=%p\n", ucontext);

	/* Check sigprocmask */

	sigfillset(&allsigs);
	status = sigprocmask(SIG_SETMASK, NULL, &oldset);
	if (status != OK) {
		printf("timer_expiration: ERROR sigprocmask failed, status=%d\n",
		        status);
		ASSERT(false);
	}

	if (!sigset_isequal(&oldset, &allsigs)) {
		printf("timer_expiration: ERROR sigprocmask=" SIGSET_FMT
		       " expected=" SIGSET_FMT "\n",
		       SIGSET_ELEM(&oldset), SIGSET_ELEM(&allsigs));
		ASSERT(false);
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void timer_test(void)
{
	sigset_t           set;
	struct sigaction   act;
	struct sigaction   oact;
	struct sigevent    notify;
	struct itimerspec  timer;
	timer_t            timerid;
	int                status;
	int                i;

	printf("timer_test: Initializing semaphore to 0\n");
	sem_init(&sem, 0, 0);

	/* Start waiter thread  */

	printf("timer_test: Unmasking signal %d\n", MY_TIMER_SIGNAL);

	sigemptyset(&set);
	sigaddset(&set, MY_TIMER_SIGNAL);
	status = sigprocmask(SIG_UNBLOCK, &set, NULL);
	if (status != OK) {
		printf("timer_test: ERROR sigprocmask failed, status=%d\n",
		        status);
		ASSERT(false);
	}

	printf("timer_test: Registering signal handler\n");
	act.sa_sigaction = timer_expiration;
	act.sa_flags  = SA_SIGINFO;

	sigfillset(&act.sa_mask);
	sigdelset(&act.sa_mask, MY_TIMER_SIGNAL);

	status = sigaction(MY_TIMER_SIGNAL, &act, &oact);
	if (status != OK) {
		printf("timer_test: ERROR sigaction failed, status=%d\n", status);
		ASSERT(false);
	}

#ifndef SDCC
	printf("timer_test: oact.sigaction=%p oact.sa_flags=%x "
	       "oact.sa_mask=" SIGSET_FMT "\n",
	       oact.sa_sigaction, oact.sa_flags, SIGSET_ELEM(&oact.sa_mask));
#endif

	/* Create the POSIX timer */

	printf("timer_test: Creating timer\n");

	notify.sigev_notify            = SIGEV_SIGNAL;
	notify.sigev_signo             = MY_TIMER_SIGNAL;
	notify.sigev_value.sival_int   = SIGVALUE_INT;
#ifdef CONFIG_SIG_EVTHREAD
	notify.sigev_notify_function   = NULL;
	notify.sigev_notify_attributes = NULL;
#endif

	status = timer_create(CLOCK_REALTIME, &notify, &timerid);
	if (status != OK) {
		printf("timer_test: ERROR timer_create failed, errno=%d\n", errno);
		ASSERT(false);
		goto errorout;
	}

	/* Start the POSIX timer */

	printf("timer_test: Starting timer\n");

	timer.it_value.tv_sec     = 2;
	timer.it_value.tv_nsec    = 0;
	timer.it_interval.tv_sec  = 2;
	timer.it_interval.tv_nsec = 0;

	status = timer_settime(timerid, 0, &timer, NULL);
	if (status != OK) {
		printf("timer_test: ERROR timer_settime failed, errno=%d\n", errno);
		ASSERT(false);
		goto errorout;
	}

	/* Take the semaphore */

	for (i = 0; i < 5; i++) {
		printf("timer_test: Waiting on semaphore\n");
		FFLUSH();
		status = sem_wait(&sem);
		if (status != 0) {
			int error = errno;
			if (error == EINTR) {
				printf("timer_test: sem_wait() successfully interrupted "
				       "by signal\n");
			} else {
				printf("timer_test: ERROR sem_wait failed, errno=%d\n", error);
				ASSERT(false);
			}
		} else {
			printf("timer_test: ERROR awakened with no error!\n");
			ASSERT(false);
		}

		printf("timer_test: g_nsigreceived=%d\n", g_nsigreceived);
	}

errorout:
	sem_destroy(&sem);

	/* Then delete the timer */

	printf("timer_test: Deleting timer\n");
	status = timer_delete(timerid);
	if (status != OK) {
		printf("timer_test: ERROR timer_create failed, errno=%d\n", errno);
		ASSERT(false);
	}

	/* Detach the signal handler */

	act.sa_handler = SIG_DFL;
	status = sigaction(MY_TIMER_SIGNAL, &act, &oact);

	printf("timer_test: done\n");
	FFLUSH();
}
