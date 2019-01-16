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

#include <sys/types.h>
#include <pthread.h>
#include <time.h>

#include "things_wifi_scan.h"
#include "things_network.h"
#include "things_rtos_util.h"
#include "logging/things_logger.h"

#define TAG "[things_wifi_scan]"

#define SCAN_AP_INTERVAL 60

typedef void *(*pthread_func_type)(void *);

static void *auto_scanning_loop(void);
static pthread_t g_wifi_scan_thread;
static bool b_scan_wifi = false; //False indicates wifi  scanning is disabled.
static pthread_mutex_t g_wifi_scan_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_wifi_scan_cond = PTHREAD_COND_INITIALIZER;

static void *__attribute__((optimize("O0"))) auto_scanning_loop(void)
{
	struct timespec abs_time;
	pthread_mutex_lock(&g_wifi_scan_mutex);
	while (b_scan_wifi && !things_is_connected_ap()) {
		if (!things_wifi_scan_ap()) {
			break;
		}

		clock_gettime(CLOCK_REALTIME , &abs_time);
		abs_time.tv_sec += SCAN_AP_INTERVAL;
		pthread_cond_timedwait(&g_wifi_scan_cond, &g_wifi_scan_mutex, &abs_time);
	}
	b_scan_wifi = false; //Wifi scan ends.
	pthread_mutex_unlock(&g_wifi_scan_mutex);
	return NULL;
}

bool things_start_wifi_scan(void)
{
	bool result = false;
	pthread_mutex_lock(&g_wifi_scan_mutex);
	if (b_scan_wifi) {
		THINGS_LOG_D(TAG, "WiFi scan is in progress.");
		result = true; //Wifi scan which is in progress is not considered as an error.
	} else {
		if (pthread_create_rtos(&g_wifi_scan_thread, NULL, (pthread_func_type)auto_scanning_loop, NULL, THINGS_STACK_AP_SCAN_THREAD) == 0) {
			b_scan_wifi = true;
			result = true;
		} else {
			THINGS_LOG_E(TAG, "Failed to create thread for WiFi scan.");
		}
	}
	pthread_mutex_unlock(&g_wifi_scan_mutex);
	return result;
}

void things_stop_wifi_scan(void)
{
	pthread_mutex_lock(&g_wifi_scan_mutex);
	if (!b_scan_wifi) {
		pthread_mutex_unlock(&g_wifi_scan_mutex);
		return;
	}

	b_scan_wifi = false;
	pthread_cond_signal(&g_wifi_scan_cond);
	pthread_mutex_unlock(&g_wifi_scan_mutex);
	pthread_join(g_wifi_scan_thread, NULL);
}
