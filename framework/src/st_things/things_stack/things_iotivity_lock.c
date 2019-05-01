/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#include "things_iotivity_lock.h"
#include "logging/things_logger.h"

#define TAG "[things_iotivity_api_lock]"

static pthread_mutex_t g_iotivity_lock;
static pthread_mutexattr_t g_mutex_attr;

void init_iotivity_api_lock(void)
{
	int err = pthread_mutexattr_init(&g_mutex_attr);
	if (err) {
		THINGS_LOG_E(TAG, "pthread_mutexattr_init failed with error code (%d).", err);
		return;
	}

	err = pthread_mutexattr_settype(&g_mutex_attr, PTHREAD_MUTEX_RECURSIVE);
	if (err) {
		THINGS_LOG_E(TAG, "pthread_mutexattr_settype failed with error code (%d).", err);
		return;
	}

	err = pthread_mutex_init(&g_iotivity_lock, &g_mutex_attr);
	if (err) {
		THINGS_LOG_E(TAG, "pthread_mutex_init failed with error code (%d).", err);
	}
}

void iotivity_api_lock(void)
{
	int err = pthread_mutex_lock(&g_iotivity_lock);
	if (err) {
		THINGS_LOG_E(TAG, "Failed to acquire lock on iotivity APIs. Error code: (%d).", err);
	}
}

void iotivity_api_unlock(void)
{
	int err = pthread_mutex_unlock(&g_iotivity_lock);
	if (err) {
		THINGS_LOG_E(TAG, "Failed to release lock on iotivity APIs. Error code: (%d).", err);
	}
}

void deinit_iotivity_api_lock(void)
{
	int err = pthread_mutexattr_destroy(&g_mutex_attr);
	if (err) {
		THINGS_LOG_E(TAG, "pthread_mutexattr_destroy failed with error code (%d).", err);
		return;
	}

	err = pthread_mutex_destroy(&g_iotivity_lock);
	if (err) {
		THINGS_LOG_E(TAG, "pthread_mutex_destroy failed with error code (%d).", err);
	}
}