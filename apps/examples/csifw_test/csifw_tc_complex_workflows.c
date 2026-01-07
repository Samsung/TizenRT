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
#include "csifw_test.h"
#include <tinyara/config.h>
#include "csimanager/csifw_api.h"
#include <tinyara/wifi_csi/wifi_csi_struct.h>

extern csifw_service_handle g_CSIServiceInstance_1;
extern csifw_service_handle g_CSIServiceInstance_2;

int test_case_interleaved_handle_operations(int interval, csi_config_type_t config_type)
{
	printf("CSIFW_TEST_APP: Running complex sequence with interleaved handle operations. Config type: %d, interval: %d ms\n", config_type, interval);
	initialize_csi_service_1(interval, config_type);
	initialize_csi_service_2(interval, config_type);
	start_csi_service(g_CSIServiceInstance_2);
	start_csi_service(g_CSIServiceInstance_1);
	sleep(10);
	stop_csi_service(g_CSIServiceInstance_1);
	deinitialize_csi_service(&g_CSIServiceInstance_1);
	stop_csi_service(g_CSIServiceInstance_2);
	start_csi_service(g_CSIServiceInstance_2);
	sleep(10);
	stop_csi_service(g_CSIServiceInstance_2);
	deinitialize_csi_service(&g_CSIServiceInstance_2);
	initialize_csi_service_1(interval, config_type);
	return 0;
}

int test_case_timed_delays_sequence(int interval, csi_config_type_t config_type)
{
	printf("CSIFW_TEST_APP: Running complex sequence with timed delays between operations. Config type: %d, interval: %d ms\n", config_type, interval);
	initialize_csi_service_1(interval, config_type);
	initialize_csi_service_2(interval, config_type);
	start_csi_service(g_CSIServiceInstance_2);
	sleep(10);
	start_csi_service(g_CSIServiceInstance_1);
	sleep(10);
	stop_csi_service(g_CSIServiceInstance_1);
	deinitialize_csi_service(&g_CSIServiceInstance_1);
	stop_csi_service(g_CSIServiceInstance_2);
	sleep(3);
	start_csi_service(g_CSIServiceInstance_2);
	sleep(10);
	stop_csi_service(g_CSIServiceInstance_2);
	deinitialize_csi_service(&g_CSIServiceInstance_2);
	sleep(3);
	initialize_csi_service_1(interval, config_type);
	sleep(3);
	deinitialize_csi_service(&g_CSIServiceInstance_1);
	return 0;
}

int test_case_extended_sequence_with_restart(int interval, csi_config_type_t config_type)
{
	printf("CSIFW_TEST_APP: Running extended sequence with restart and multi-handle operations. Config type: %d, interval: %d ms\n", config_type, interval);
	initialize_csi_service_1(interval, config_type);
	start_csi_service(g_CSIServiceInstance_1);
	sleep(10);
	stop_csi_service(g_CSIServiceInstance_1);
	sleep(3);
	start_csi_service(g_CSIServiceInstance_1);
	sleep(10);
	stop_csi_service(g_CSIServiceInstance_1);
	deinitialize_csi_service(&g_CSIServiceInstance_1);

	initialize_csi_service_1(interval, config_type);
	initialize_csi_service_2(interval, config_type);
	start_csi_service(g_CSIServiceInstance_1);
	start_csi_service(g_CSIServiceInstance_2);
	sleep(10);
	stop_csi_service(g_CSIServiceInstance_2);
	deinitialize_csi_service(&g_CSIServiceInstance_2);
	return 0;
}
