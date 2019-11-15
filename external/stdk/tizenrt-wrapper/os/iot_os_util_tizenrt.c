/* ***************************************************************************
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

#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <mqueue.h>
#include <event_groups.h>
#ifdef CONFIG_ARCH_BOARD_ESP32_FAMILY
#include <esp32_queue_api.h>
#else
#include <queue_api.h>
#endif
#include <semaphore.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <tinyara/config.h>

#include "iot_error.h"
#include "iot_os_util.h"

#define pdFALSE 0
#define pdTRUE 1

#define portMAX_DELAY (0xffffffff)

const unsigned int iot_os_max_delay = portMAX_DELAY;
const unsigned int iot_os_true = pdTRUE;
const unsigned int iot_os_false = pdFALSE;

#define VALIDATE_MSEC2TICK(ms) (((ms) == iot_os_max_delay) ? iot_os_max_delay : MSEC2TICK(ms))

typedef struct tizenrt_timer {
	clock_t ticks_to_wait;
	clock_t time_out;
} tizenrt_timer;

/* Thread */
int iot_os_thread_create(void *thread_function, const char *name, int stack_size,
		void *data, int priority, iot_os_thread *thread_handle)
{
	int status;
	pthread_attr_t attr;
	pthread_t *pid_h = (pthread_t *)malloc(sizeof(pthread_t));

	status = pthread_attr_init(&attr);
	if (status != 0) {
		return IOT_OS_FALSE;
	}

	status = pthread_attr_setstacksize(&attr, stack_size);
	if (status != 0) {
		return IOT_OS_FALSE;
	}

	status = pthread_create(pid_h, &attr, thread_function, (pthread_addr_t)data);
	if (status != 0) {
		return IOT_OS_FALSE;
	}

	pthread_setname_np(*pid_h, name);

	if (thread_handle) {
		*thread_handle = (iot_os_thread)pid_h;
	}
	return IOT_OS_TRUE;
}

void iot_os_thread_delete(iot_os_thread thread_handle)
{
	if (thread_handle) {
		pthread_t *pid_h = (pthread_t *)thread_handle;
		pthread_cancel(*pid_h);
		free(pid_h);
	}
}

void iot_os_thread_yield()
{
	sched_yield();
}

/* Queue */
iot_os_queue *iot_os_queue_create(int queue_length, int item_size)
{
	return queue_create_wrapper(queue_length, item_size);
}

int iot_os_queue_reset(iot_os_queue *queue_handle)
{
	return queue_reset_wrapper((void *)queue_handle);
}

void iot_os_queue_delete(iot_os_queue *queue_handle)
{
	queue_delete_wrapper((void *)queue_handle);
}

int iot_os_queue_send(iot_os_queue *queue_handle, void *data, unsigned int wait_time_ms)
{
	return queue_send_wrapper(queue_handle, data, VALIDATE_MSEC2TICK(wait_time_ms));
}

int iot_os_queue_receive(iot_os_queue *queue_handle, void *data, unsigned int wait_time_ms)
{
	return queue_recv_wrapper((void *)queue_handle, data, VALIDATE_MSEC2TICK(wait_time_ms));
}

/* Event Group */
iot_os_eventgroup* iot_os_eventgroup_create(void)
{
	return event_group_create();
}

void iot_os_eventgroup_delete(iot_os_eventgroup *eventgroup_handle)
{
	event_group_delete(eventgroup_handle);
}

unsigned int iot_os_eventgroup_wait_bits(iot_os_eventgroup *eventgroup_handle,
		const unsigned int bits_to_wait_for, const int clear_on_exit,
		const int wait_for_all_bits, const unsigned int wait_time_ms)
{
	return event_group_wait_bits(eventgroup_handle, bits_to_wait_for, clear_on_exit, wait_for_all_bits, VALIDATE_MSEC2TICK(wait_time_ms));
}

unsigned int iot_os_eventgroup_set_bits(iot_os_eventgroup *eventgroup_handle,
		const unsigned int bits_to_set)
{
	return event_group_set_bits(eventgroup_handle, bits_to_set);
}

unsigned int iot_os_eventgroup_clear_bits(iot_os_eventgroup *eventgroup_handle,
		const unsigned int bits_to_clear)
{
	return event_group_clear_bits(eventgroup_handle, bits_to_clear);
}

/* Mutex */
static void *recursive_mutex_create_wrapper(void)
{
	pthread_mutexattr_t mattr;
	int status = 0;

	pthread_mutex_t *mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (mutex == NULL) {
		return NULL;
	}
	pthread_mutexattr_init(&mattr);
	status = pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
	if (status != 0) {
		return NULL;
	}
	status = pthread_mutex_init(mutex, &mattr);
	if (status) {
		return NULL;
	}
	return (void *)mutex;
}


int iot_os_mutex_init(iot_os_mutex *mutex)
{
    if (!mutex) {
        return IOT_ERROR_INVALID_ARGS;
    }

    mutex->sem = recursive_mutex_create_wrapper();
    if (!mutex->sem) {
        return IOT_ERROR_MEM_ALLOC;
    }
    return IOT_ERROR_NONE;
}

int iot_os_mutex_lock(iot_os_mutex *mutex)
{
    int ret;

    if (!mutex) {
        return IOT_ERROR_INVALID_ARGS;
    }

    ret = pthread_mutex_lock((pthread_mutex_t *)mutex->sem);
    if (!ret) {
        ret = IOT_ERROR_NONE;
    } else {
        ret = IOT_ERROR_BAD_REQ;
    }

    return ret;
}

int iot_os_mutex_unlock(iot_os_mutex *mutex)
{
    int ret;

    if (!mutex) {
        return IOT_ERROR_INVALID_ARGS;
    }

    ret = pthread_mutex_unlock((pthread_mutex_t *)mutex->sem);
    if (!ret) {
        ret = IOT_ERROR_NONE;
    } else {
        ret = IOT_ERROR_BAD_REQ;
    }

    return ret;
}

int iot_os_mutex_delete(iot_os_mutex *mutex)
{
    int ret;

	if (!mutex) {
		return IOT_ERROR_INVALID_ARGS;
	}

	ret = pthread_mutex_destroy((pthread_mutex_t *)mutex->sem);
	if (!ret) {
	    ret = IOT_ERROR_NONE;
	} else {
	    ret = IOT_ERROR_BAD_REQ;
	}
	return ret;
}

/* Delay */
void iot_os_delay(unsigned int delay_ms)
{
	usleep(delay_ms * 1000);
}

static int check_for_timeout(clock_t *const ptime_out, clock_t *const pticks_to_wait)
{
	int ret;

	/* Minor optimisation.  The tick count cannot change in this block. */
	const clock_t const_tick_count = clock_systimer();
	const clock_t elapsed_time = const_tick_count - *ptime_out;

	if (*pticks_to_wait == portMAX_DELAY) {
		/* If INCLUDE_vTaskSuspend is set to 1 and the block time
		specified is the maximum block time then the task should block
		indefinitely, and therefore never time out. */
		ret = pdFALSE;
	} else if (elapsed_time < *pticks_to_wait ) {
		/* Not a genuine timeout. Adjust parameters for time remaining. */
		*pticks_to_wait -= elapsed_time;
		*ptime_out = clock_systimer();
		ret = pdFALSE;
	} else {
		*pticks_to_wait = 0;
		ret = pdTRUE;
	}

	return ret;
}

void iot_os_timer_count_ms(iot_os_timer timer, unsigned int timeout_ms)
{
	((tizenrt_timer *)timer)->ticks_to_wait = VALIDATE_MSEC2TICK(timeout_ms); /* convert milliseconds to ticks */
	((tizenrt_timer *)timer)->time_out = clock_systimer(); /* Record the time at which this function was entered. */
}

unsigned int iot_os_timer_left_ms(iot_os_timer timer)
{
	tizenrt_timer *os_timer = (tizenrt_timer *)timer;

	if (os_timer->ticks_to_wait == portMAX_DELAY) {
		return portMAX_DELAY;
	}

	check_for_timeout(&os_timer->time_out, &os_timer->ticks_to_wait); /* updates ticks_to_wait to the number left */
	return (os_timer->ticks_to_wait <= 0) ? 0 : TICK2MSEC(os_timer->ticks_to_wait);
}

char iot_os_timer_isexpired(iot_os_timer timer)
{
	tizenrt_timer *os_timer = (tizenrt_timer *)timer;

	return check_for_timeout(&os_timer->time_out, &os_timer->ticks_to_wait) == pdTRUE;
}

void iot_os_timer_init(iot_os_timer *timer)
{
	*timer = malloc(sizeof(tizenrt_timer));
	if (*timer == NULL) {
		return;
	}
	memset(*timer, '\0', sizeof(tizenrt_timer));

	return;
}

void iot_os_timer_destroy(iot_os_timer *timer)
{
	if (timer == NULL || *timer == NULL) {
		return;
	}
	free(*timer);
	*timer = NULL;
}
