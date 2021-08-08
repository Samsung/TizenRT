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
/***********************************************************************
 * examples/kernel_sample/pthread.c
 ***********************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NLOOPS 100
static pthread_t thread;

static void *empty_func(void *parameter)
{
	return NULL;
}

void pthread_test(void)
{
	int nloops = NLOOPS;
	pthread_attr_t attr;
	int status;

	/* Start pthread instance */

	(void)pthread_attr_init(&attr);

	printf("Start to create pthread %d-times repeatedly.\n", NLOOPS);

	while (nloops > 0) {
		status = pthread_create(&thread, &attr, empty_func, (pthread_addr_t)NULL);
		if (status != 0) {
			printf("Error in %d-th thread creation.\n", NLOOPS - nloops + 1);
		}
		pthread_detach(thread);
		nloops--;
		usleep(1);
	}

	printf("pthread_test: done\n");
}
