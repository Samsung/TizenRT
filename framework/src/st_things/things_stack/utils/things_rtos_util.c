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

#define _POSIX_C_SOURCE 200809L

#include <pthread.h>
#include <sched.h>

#include "things_rtos_util.h"
#include "logging/things_logger.h"

#define TAG "[things_rtos_util]"

#define THINGS_STACK_PTHREAD_STACK_DEFAULT		2048
#define THINGS_STACK_PTHREAD_DEFAULT_PRIORITY	100
#define THINGS_STACK_PTHREAD_DEFAULT_POLICY		SCHED_RR
#define THINGS_STACK_PTHREAD_EXPLICIT_SCHED		SCHED_FIFO

void pthread_init_rtos(pthread_attr_t *thread_attr)
{
	thread_attr->stacksize = THINGS_STACK_PTHREAD_STACK_DEFAULT;
	thread_attr->priority = THINGS_STACK_PTHREAD_DEFAULT_PRIORITY;
	thread_attr->policy = THINGS_STACK_PTHREAD_DEFAULT_POLICY;
	thread_attr->inheritsched = THINGS_STACK_PTHREAD_EXPLICIT_SCHED;
}

int pthread_create_rtos(FAR pthread_t *thread, FAR const pthread_attr_t *attr, pthread_startroutine_t start_routine, pthread_addr_t arg, long stack_size, char *thread_name)
{
	int ret;

	pthread_attr_t thread_attr;
	memset(&thread_attr, 0, sizeof(thread_attr));
	pthread_init_rtos(&thread_attr);

	ret = pthread_attr_setstacksize(&thread_attr, stack_size);
	if (ret == 0) {
		ret = pthread_create(thread, &thread_attr, start_routine, (void *)arg);
	} else {
		THINGS_LOG_D(THINGS_ERROR, TAG, "malloc fail for stack ERROR : %d", ret);
		ret = pthread_create(thread, NULL, start_routine, (void *)arg);
	}

	if (ret == 0) {
		pthread_setname_np(*thread, thread_name);
		pthread_detach(*thread);
	} else {
		if (thread_name == NULL) {
			THINGS_LOG_D(THINGS_ERROR, TAG, "pthread_create ERROR : %d", ret);
		} else {
			THINGS_LOG_D(THINGS_ERROR, TAG, "pthread_create thread_name : %s , ERROR : %d", thread_name, ret);
		}
	}
	
	return ret;
}
