/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "csifw_test.h"
#include <tinyara/config.h>
#include "csimanager/csifw_api.h"
#include <tinyara/wifi_csi/wifi_csi_struct.h>

extern csifw_service_handle g_CSIServiceInstance_1;
extern csifw_service_handle g_CSIServiceInstance_2;
extern csifw_service_handle g_CSIServiceInstance_3;
extern csifw_service_handle g_CSIServiceInstance_4;

static pthread_mutex_t handle_mutex = PTHREAD_MUTEX_INITIALIZER;

int test_case_multi_handle_concurrent(int interval, csi_config_type_t config_type)
{
	printf("CSIFW_TEST_APP: Running multi-handle sequence with 4 concurrent handles. Config type: %d, interval: %d ms\n", config_type, interval);
	initialize_csi_service_1(interval, config_type);
	initialize_csi_service_2(interval, config_type);
	initialize_csi_service_3(interval, config_type);
	initialize_csi_service_4(interval, config_type);
	start_csi_service(g_CSIServiceInstance_1);
	start_csi_service(g_CSIServiceInstance_2);
	start_csi_service(g_CSIServiceInstance_3);
	start_csi_service(g_CSIServiceInstance_4);
	sleep(10);
	stop_csi_service(g_CSIServiceInstance_1);
	stop_csi_service(g_CSIServiceInstance_2);
	stop_csi_service(g_CSIServiceInstance_3);
	stop_csi_service(g_CSIServiceInstance_4);
	deinitialize_csi_service(&g_CSIServiceInstance_1);
	deinitialize_csi_service(&g_CSIServiceInstance_2);
	deinitialize_csi_service(&g_CSIServiceInstance_3);
	deinitialize_csi_service(&g_CSIServiceInstance_4);
	return 0;
}

typedef struct
{
	int interval;
	csi_config_type_t config_type;
} thread_parameters;

void* thread_execute_sequence_1(void* arg)
{
	thread_parameters* params = (thread_parameters*)arg;
	pthread_mutex_lock(&handle_mutex);
    if (initialize_csi_service_1(params->interval, params->config_type) != 0) {
		pthread_mutex_unlock(&handle_mutex);
        return NULL;
    }
	
	if (start_csi_service(g_CSIServiceInstance_1) != 0) {
		printf("CSIFW_TEST_APP: Failed to start CSIService_1\n");
		pthread_mutex_unlock(&handle_mutex);
		return NULL;
	}
	pthread_mutex_unlock(&handle_mutex);
	
	sleep(120);
	
	pthread_mutex_lock(&handle_mutex);
	if (stop_csi_service(g_CSIServiceInstance_1) != 0) {
		printf("CSIFW_TEST_APP: Failed to stop CSIService_1\n");
		pthread_mutex_unlock(&handle_mutex);
		return NULL;
	}
	
	if (deinitialize_csi_service(&g_CSIServiceInstance_1) != 0)	{
		printf("CSIFW_TEST_APP: Failed to deinitialize CSIService_1\n");
		pthread_mutex_unlock(&handle_mutex);
		return NULL;
	}
	pthread_mutex_unlock(&handle_mutex);
	return NULL;
}

void* thread_execute_sequence_2(void* arg)
{
	thread_parameters* params = (thread_parameters*)arg;
	pthread_mutex_lock(&handle_mutex);
    if (initialize_csi_service_2(params->interval, params->config_type) != 0) {
        pthread_mutex_unlock(&handle_mutex);
        return NULL;
    }
	
	if (start_csi_service(g_CSIServiceInstance_2) != 0) {
		printf("CSIFW_TEST_APP: Failed to start CSIService_2\n");
		pthread_mutex_unlock(&handle_mutex);
		return NULL;
	}
	pthread_mutex_unlock(&handle_mutex);
	
	sleep(120);
	
	pthread_mutex_lock(&handle_mutex);
	if (stop_csi_service(g_CSIServiceInstance_2) != 0) {
		printf("CSIFW_TEST_APP: Failed to stop CSIService_2\n");
		pthread_mutex_unlock(&handle_mutex);
		return NULL;
	}
	
	if (deinitialize_csi_service(&g_CSIServiceInstance_2) != 0)	{
		printf("CSIFW_TEST_APP: Failed to deinitialize CSIService_2\n");
		pthread_mutex_unlock(&handle_mutex);
		return NULL;
	}
	pthread_mutex_unlock(&handle_mutex);
	return NULL;
}

int test_case_multi_threaded_execution(int interval, csi_config_type_t config_type)
{
	printf("CSIFW_TEST_APP: Running multithreaded test scenario with 2 concurrent threads. Config type: %d, interval: %d ms\n", config_type, interval);
	pthread_t thread1, thread2;

	thread_parameters params1;
	thread_parameters params2;

	params1.interval = interval;
	params1.config_type = config_type;
	params2.interval = interval;
	params2.config_type = config_type;

	if (pthread_create(&thread1, NULL, thread_execute_sequence_1, &params1) != 0) {
		printf("CSIFW_TEST_APP: Failed to create thread 1\n");
		return -1; 
	}

	if (pthread_create(&thread2, NULL, thread_execute_sequence_2, &params2) != 0) {
		printf("CSIFW_TEST_APP: Failed to create thread 2\n");
		pthread_join(thread1, NULL); 
		return -1;
	}

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("CSIFW_TEST_APP: Multithreaded test completed successfully\n");
	return 0;
}
