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

#include <stdio.h>
#include <stdlib.h>

#include "logging/things_logger.h"
#include "utils/things_malloc.h"

#include "things_thread.h"

#define TAG "[things_thread]"

int things_thread_create(pthread_t *__newthread, pthread_attr_t *__attr, void *(*__start_routine)(void *), void *__arg)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");
	int ret = 0;
#ifndef __ST_THINGS_RTOS__
	int ret = -1;
	pthread_attr_t attr;
	pthread_attr_t *pAttr;

	if (__newthread == NULL || __start_routine == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Input invalied.(mandatory var: __newthread=0x%X, __start_routine=0x%X", __newthread, __start_routine);
		return ret;
	}

	pAttr = __attr;

	if (__attr == NULL) {
		if (pthread_attr_init(&attr) != 0) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "pthread_attr_init() is failed.");
			return ret;
		}

		if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "pthread_attr_setdetachstate() is failed.");
			return ret;
		}

		pAttr = &attr;
	}

	ret = pthread_create(__newthread, pAttr, __start_routine, __arg);

	if (__attr == NULL) {
		if (pthread_attr_destroy(&attr) != 0) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "pthread_attr_destroy() is failed.");
		}
	}

	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
#endif
	return ret;
}
