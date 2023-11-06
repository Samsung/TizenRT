/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *aifw_
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#include <semaphore.h>
#include <signal.h>
#include <memory>
#include <errno.h>
#include "aifw/aifw_timer.h"
#include "aifw/aifw_log.h"

#define AIFW_TIMER_SIGNAL 17

static void aifw_timer_cb(int signo, siginfo_t *info, void *ucontext);
static void *aifw_timerthread_cb(void *parameter);

aifw_timer_result aifw_timer_create(aifw_timer *timer, void *timer_function, void *function_args, unsigned int interval)
{
	if (!timer) {
		AIFW_LOGE("Pointer to aifw_timer structure is NULL");
		return AIFW_TIMER_INVALID_ARGS;
	}
	if (!timer_function) {
		AIFW_LOGE("Pointer to timer function is NULL");
		return AIFW_TIMER_INVALID_ARGS;
	}
	if (interval <= 0 || interval > 0x7FFFFFFF) {
		AIFW_LOGE("Value of timer interval is out of bounds");
		return AIFW_TIMER_INVALID_ARGS;
	}
	/* Fill values in timer structure */
	timer->function = (aifw_timer_callback)timer_function;
	timer->function_args = function_args;
	timer->interval = interval;
	timer->enable = false;
	return AIFW_TIMER_SUCCESS;
}

aifw_timer_result aifw_timer_start(aifw_timer *timer)
{
	if (!timer) {
		AIFW_LOGE("Pointer to aifw_timer structure is NULL");
		return AIFW_TIMER_INVALID_ARGS;
	}
	AIFW_LOGV("Start Timer");
	int result = pthread_create(&(timer->timerThread), NULL, aifw_timerthread_cb, (void *)timer);
	if (result != 0) {
		AIFW_LOGE("ERROR Failed to start aifw_timerthread_cb");
		return AIFW_TIMER_FAIL;
	}
	AIFW_LOGV("Started aifw_timerthread_cb");
	return AIFW_TIMER_SUCCESS;
}

aifw_timer_result aifw_timer_change_interval(aifw_timer *timer, unsigned int interval)
{
	if (!timer) {
		AIFW_LOGE("Pointer to aifw_timer structure is NULL");
		return AIFW_TIMER_INVALID_ARGS;
	}
	if (interval <= 0) {
		AIFW_LOGE("Invalid argument interval: %d", interval);
		return AIFW_TIMER_INVALID_ARGS;
	}
	timer->interval = interval;
	if (!timer->enable) {
		AIFW_LOGV("Timer not started/enabled yet");
		return aifw_timer_start(timer);
	}
	struct itimerspec its;
	uint32_t interval_secs = interval / 1000;
	uint64_t interval_nsecs = (interval % 1000) * 1000000;
	AIFW_LOGV("setInterval: %d %d %lld", interval, interval_secs, interval_nsecs);
	its.it_value.tv_sec = interval_secs;
	its.it_value.tv_nsec = interval_nsecs;
	its.it_interval.tv_sec = interval_secs;
	its.it_interval.tv_nsec = interval_nsecs;
	int status = timer_settime(timer->id, 0, &its, NULL);
	if (status != OK) {
		AIFW_LOGE("setInterval: timer_settime failed, errno: %d", errno);
		return AIFW_TIMER_FAIL;
	}
	return AIFW_TIMER_SUCCESS;
}


aifw_timer_result aifw_timer_stop(aifw_timer *timer)
{
	if (!timer) {
		AIFW_LOGE("Pointer to aifw_timer structure is NULL");
		return AIFW_TIMER_INVALID_ARGS;
	}
	if (!timer->enable) {
		AIFW_LOGE("Timer is not enabled/started yet.");
		return AIFW_TIMER_SUCCESS;
	}
	AIFW_LOGV("Stop Timer");
	if (sem_post(&(timer->semaphore)) != 0) {
		AIFW_LOGE("Timer stop failed, error: %d", errno);
		return AIFW_TIMER_FAIL;
	}
	timer->enable = false;
	AIFW_LOGV("Stop Timer posted");	
	return AIFW_TIMER_SUCCESS;
}

aifw_timer_result aifw_timer_destroy(aifw_timer *timer)
{
	if (!timer) {
		AIFW_LOGE("Pointer to aifw_timer structure is NULL");
		return AIFW_TIMER_INVALID_ARGS;
	}
	memset(timer, 0, sizeof(aifw_timer));
	return AIFW_TIMER_SUCCESS;
}

static void *aifw_timerthread_cb(void *parameter)
{
	sigset_t sigset;
	struct sigaction act;
	struct sigevent notify;
	struct itimerspec its;
	timer_t timerId;
	int status;

	if (!parameter) {
		AIFW_LOGE("aifw_timerthread_cb: invalid argument");
		return NULL;
	}

	uint32_t interval_secs = ((aifw_timer *)parameter)->interval / 1000;
	uint64_t interval_nsecs = (((aifw_timer *)parameter)->interval % 1000) * 1000000;

	AIFW_LOGV("aifw_timerthread_cb: Initializing semaphore to 0");
	sem_init(&(((aifw_timer *)parameter)->semaphore), 0, 0);

	AIFW_LOGV("Initializing exit semaphore to 0");
	sem_init(&(((aifw_timer *)parameter)->exitSemaphore), 0, 0);

	/* Start waiter thread  */
	AIFW_LOGV("aifw_timerthread_cb: Unmasking signal %d", AIFW_TIMER_SIGNAL);
	(void)sigemptyset(&sigset);
	(void)sigaddset(&sigset, AIFW_TIMER_SIGNAL);
	status = sigprocmask(SIG_UNBLOCK, &sigset, NULL);
	if (status != OK) {
		AIFW_LOGE("aifw_timerthread_cb: ERROR sigprocmask failed, status=%d", status);
		return NULL;
	}

	AIFW_LOGV("aifw_timerthread_cb: Registering signal handler");
	act.sa_sigaction = aifw_timer_cb;
	act.sa_flags = SA_SIGINFO;

	(void)sigfillset(&act.sa_mask);
	(void)sigdelset(&act.sa_mask, AIFW_TIMER_SIGNAL);
	status = sigaction(AIFW_TIMER_SIGNAL, &act, NULL);
	if (status != OK) {
		AIFW_LOGE("aifw_timerthread_cb: ERROR sigaction failed, status=%d", status);
		return NULL;
	}

	/* Create the POSIX timer */
	AIFW_LOGV("aifw_timerthread_cb: Creating timer");
	notify.sigev_notify = SIGEV_SIGNAL;
	notify.sigev_signo = AIFW_TIMER_SIGNAL;
	notify.sigev_value.sival_ptr = parameter;
	status = timer_create(CLOCK_REALTIME, &notify, &(((aifw_timer *)parameter)->id));
	timerId = ((aifw_timer *)parameter)->id;
	if (status != OK) {
		AIFW_LOGE("aifw_timerthread_cb: timer_create failed, errno=%d", errno);
		goto errorout;
	}

	/* Start the POSIX timer */
	AIFW_LOGV("aifw_timerthread_cb: Starting timer");
	AIFW_LOGV("aifw_timerthread_cb: %d %d %lld", ((aifw_timer *)parameter)->interval, interval_secs, interval_nsecs);
	its.it_value.tv_sec = interval_secs;
	its.it_value.tv_nsec = interval_nsecs;
	its.it_interval.tv_sec = interval_secs;
	its.it_interval.tv_nsec = interval_nsecs;

	status = timer_settime(((aifw_timer *)parameter)->id, 0, &its, NULL);
	if (status != OK) {
		AIFW_LOGE("aifw_timerthread_cb: timer_settime failed, errno=%d", errno);
		goto errorout;
	}
	AIFW_LOGV("aifw_timerthread_cb: exiting function");
	((aifw_timer *)parameter)->enable = true;
	/* Take the semaphore */
	while (1) {
		AIFW_LOGV("aifw_timerthread_cb: Waiting on semaphore");
		status = sem_wait(&(((aifw_timer *)parameter)->semaphore));
		if (status != 0) {
			int error = errno;
			if (error == EINTR) {
				AIFW_LOGV("aifw_timerthread_cb: sem_wait() successfully interrupted by signal");
			} else {
				AIFW_LOGE("aifw_timerthread_cb: ERROR sem_wait failed, errno=%d", error);
			}
		} else {
			AIFW_LOGI("aifw_timerthread_cb: ERROR awakened with no error!");
			break;
		}
	}
errorout:
	/* Delete the timer */
	AIFW_LOGV("aifw_timerthread_cb: Deleting timer");
	status = timer_delete(timerId);
	if (status != OK) {
		AIFW_LOGE("aifw_timerthread_cb: timer_delete failed, errno=%d", errno);
	}
	/* Detach the signal handler */
	act.sa_handler = SIG_DFL;
	status = sigaction(AIFW_TIMER_SIGNAL, &act, NULL);
	AIFW_LOGV("aifw_timerthread_cb: sem_destroy");
	sem_destroy(&(((aifw_timer *)parameter)->semaphore));
	sem_post(&(((aifw_timer *)parameter)->exitSemaphore));
	AIFW_LOGV("aifw_timerthread_cb: done");
	return NULL;
}

static void aifw_timer_cb(int signo, siginfo_t *info, void *ucontext)
{
	AIFW_LOGV("aifw_timer_cb: Received signal %d", signo);
	if (signo != AIFW_TIMER_SIGNAL) {
		AIFW_LOGE("aifw_timer_cb: ERROR expected signo: %d", AIFW_TIMER_SIGNAL);
		return;
	}
	if (info->si_signo != AIFW_TIMER_SIGNAL) {
		AIFW_LOGE("aifw_timer_cb: ERROR expected si_signo=%d, got=%d", AIFW_TIMER_SIGNAL, info->si_signo);
		return;
	}
	if (info->si_code != SI_TIMER) {
		AIFW_LOGE("aifw_timer_cb: ERROR si_code=%d, expected SI_TIMER=%d", info->si_code, SI_TIMER);
		return;
	}
	AIFW_LOGV("aifw_timer_cb: si_code=%d (SI_TIMER)", info->si_code);
	aifw_timer *timer = (aifw_timer *)info->si_value.sival_ptr;
	timer->function(timer->function_args);
}
