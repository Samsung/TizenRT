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

#include <semaphore.h>
#include <errno.h>
#include <time.h>
#include "wifi_semaphore.h"

wifi_utils_result_e wifi_semaphore_create(wifi_semaphore *semaphore, unsigned int init_value)
{
	if (!semaphore) {
		return WIFI_UTILS_INVALID_ARGS;
	}

	if (sem_init((sem_t *)semaphore, 0, init_value)) {
		return WIFI_UTILS_FAIL;
	}

	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_semaphore_acquire(wifi_semaphore *semaphore, int time_out)
{
	int result;

	if (!semaphore || time_out < -1) {
		return WIFI_UTILS_INVALID_ARGS;
	}

	if (time_out == WIFI_UTILS_FOREVER) {
		result = sem_wait((sem_t *)semaphore);
	} else if (time_out == WIFI_UTILS_NO_WAIT) {
		result = sem_trywait((sem_t *)semaphore);
	} else {
		time_t sec;
		uint32_t nsec;
		struct timespec abstime;

		sec = time_out / 1000;
		nsec = (time_out - 1000 * sec) * 1000000;

		clock_gettime(CLOCK_REALTIME, &abstime);

		abstime.tv_sec += sec;
		abstime.tv_nsec += nsec;
		if (abstime.tv_nsec >= 1000000000) {
			abstime.tv_sec++;
			abstime.tv_nsec -= 1000000000;
		}

		result = sem_timedwait((sem_t *)semaphore, &abstime);
	}

	if (result == 0) {
		return WIFI_UTILS_SUCCESS;
	}

	if (get_errno() == ETIMEDOUT) {
		return WIFI_UTILS_TIMEOUT;
	}

	return WIFI_UTILS_FAIL;
}

wifi_utils_result_e wifi_semaphore_release(wifi_semaphore *semaphore)
{
	if (!semaphore) {
		return WIFI_UTILS_INVALID_ARGS;
	}

	if (sem_post((sem_t *)semaphore)) {
		return WIFI_UTILS_FAIL;
	}

	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_semaphore_destroy(wifi_semaphore *semaphore)
{
	if (!semaphore) {
		return WIFI_UTILS_INVALID_ARGS;
	}

	if (sem_destroy((sem_t *)semaphore)) {
		return WIFI_UTILS_FAIL;
	}

	return WIFI_UTILS_SUCCESS;
}
