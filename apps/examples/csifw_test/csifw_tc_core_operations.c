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

static void get_config_info_service_1(void)
{
	csi_config_type_t config_type;
	if (csifw_get_config(g_CSIServiceInstance_1, &config_type) != CSIFW_OK) {
		printf("CSIFW_TEST_APP: Failed to get CSI configuration for CSIService_1: %p\n", g_CSIServiceInstance_1);
		return;
	}

	const char *type_str;
	switch (config_type) {
	case HT_CSI_DATA:
		type_str = "HT_CSI_DATA";
		break;
	case MIN_CSI_CONFIG_TYPE:
		type_str = "MIN_CSI_CONFIG_TYPE";
		break;
	case NON_HT_CSI_DATA:
		type_str = "NON_HT_CSI_DATA";
		break;
	case HT_CSI_DATA_ACC1:
		type_str = "HT_CSI_DATA_ACC1";
		break;
	case NON_HT_CSI_DATA_ACC1:
		type_str = "NON_HT_CSI_DATA_ACC1";
		break;
	case MAX_CSI_CONFIG_TYPE:
		type_str = "MAX_CSI_CONFIG_TYPE";
		break;
	default:
		type_str = "UNKNOWN";
		break;
	}

	printf("CSIFW_TEST_APP: Current CSI Configuration Type for CSIService_1: %s (%d)\n", type_str, config_type);
	sleep(1);
}

int test_case_initialize_csi_service_only(int interval, csi_config_type_t config_type)
{
	printf("CSIFW_TEST_APP: Running CSI Initialization only with config type: %d, interval: %d ms\n", config_type, interval);
	return initialize_csi_service_1(interval, config_type);
}

int test_case_start_csi_data_collection_only(void)
{
	printf("CSIFW_TEST_APP: Running CSI Start only for CSIService_1\n");
	return start_csi_service(g_CSIServiceInstance_1);
}

int test_case_stop_csi_data_collection_only(void)
{
	printf("CSIFW_TEST_APP: Running CSI Stop only for CSIService_1\n");
	return stop_csi_service(g_CSIServiceInstance_1);
}

int test_case_deinitialize_csi_service_only(void)
{
	printf("CSIFW_TEST_APP: Running CSI DeInitialization only for CSIService_1\n");
	return deinitialize_csi_service(&g_CSIServiceInstance_1);
}

int test_case_get_configuration_information(void)
{
	unsigned int interval;
	csifw_mac_info mac_info;

	printf("CSIFW_TEST_APP: Getting current CSI configuration, interval, and AP MAC address for CSIService_1\n");
	get_config_info_service_1();
	
	csifw_get_interval(g_CSIServiceInstance_1, &interval);
	printf("CSIFW_TEST_APP: Current CSI Collection Interval for CSIService_1: %d ms\n", interval);
	
	csifw_get_ap_mac_addr(g_CSIServiceInstance_1, &mac_info);
	printf("CSIFW_TEST_APP: AP MAC Address for CSIService_1: %02x:%02x:%02x:%02x:%02x:%02x\n", mac_info.mac_addr[0], mac_info.mac_addr[1], mac_info.mac_addr[2], mac_info.mac_addr[3], mac_info.mac_addr[4], mac_info.mac_addr[5]);
	return 0;
}

int test_case_set_collection_interval(int user_interval)
{
	printf("CSIFW_TEST_APP: Setting CSI collection interval to: %d ms\n", user_interval);
	int result = 0;
	if (csifw_set_interval(g_CSIServiceInstance_1, user_interval) == CSIFW_OK) {
		printf("CSIFW_TEST_APP: Successfully updated CSI collection interval for CSIService_1 to: %d ms\n", user_interval);
	} else {
		printf("CSIFW_TEST_APP: Setting CSI collection interval to %d ms failed\n", user_interval);
		result = -1;
	}
	return result;
}

int test_case_execute_primary_handle_basic_sequence(int interval, csi_config_type_t config_type)
{
	printf("CSIFW_TEST_APP: Running basic sequence (Init->Start->Stop->Deinit) for CSIService_1 with config type: %d, interval: %d ms\n", 
		config_type, interval);
	
	if (initialize_csi_service_1(interval, config_type) != 0) {
		printf("CSIFW_TEST_APP: Failed to initialize CSIService_1\n");
		return -1;
	}
	
	if (start_csi_service(g_CSIServiceInstance_1) != 0) {
		printf("CSIFW_TEST_APP: Failed to start CSIService_1\n");
		return -1;
	}
	
	sleep(120);

	if (stop_csi_service(g_CSIServiceInstance_1) != 0) {
		printf("CSIFW_TEST_APP: Failed to stop CSIService_1\n");
		return -1;
	}
	
	if (deinitialize_csi_service(&g_CSIServiceInstance_1) != 0) {
		printf("CSIFW_TEST_APP: Failed to deinitialize CSIService_1\n");
		return -1;
	}
	return 0;
}

int test_case_execute_secondary_handle_basic_sequence(int interval, csi_config_type_t config_type)
{
	printf("CSIFW_TEST_APP: Running basic sequence (Init->Start->Stop->Deinit) for CSIService_2 with config type: %d, interval: %d ms\n", 
		config_type, interval);

	if (initialize_csi_service_2(interval, config_type) != 0) {
		printf("CSIFW_TEST_APP: Failed to initialize CSIService_2\n");
		return -1;
	}
	
	if (start_csi_service(g_CSIServiceInstance_2) != 0) {
		printf("CSIFW_TEST_APP: Failed to start CSIService_2\n");
		return -1;
	}

	sleep(120);
	
	if (stop_csi_service(g_CSIServiceInstance_2) != 0) {
		printf("CSIFW_TEST_APP: Failed to stop CSIService_2\n");
		return -1;
	}
	
	if (deinitialize_csi_service(&g_CSIServiceInstance_2) != 0) {
		printf("CSIFW_TEST_APP: Failed to deinitialize CSIService_2\n");
		return -1;
	}
	return 0;
}

int test_case_basic_sequence_with_parameters(int user_interval, int user_config, int run_time)
{
	printf("CSIFW_TEST_APP: Running basic sequence for %d seconds with config type: %d and interval: %d ms\n", 
		run_time, user_config, user_interval);
	
    if (initialize_csi_service_1(user_interval, user_config) != 0) {
        printf("CSIFW_TEST_APP: Failed to initialize service\n");
        return -1;
    }
    if (start_csi_service(g_CSIServiceInstance_1) != 0) {
        printf("CSIFW_TEST_APP: Failed to start service\n");
        deinitialize_csi_service(&g_CSIServiceInstance_1);
        return -1;
    }
    sleep(run_time);
    if (stop_csi_service(g_CSIServiceInstance_1) != 0) {
        printf("CSIFW_TEST_APP: Failed to stop service\n");
    }
    if (deinitialize_csi_service(&g_CSIServiceInstance_1) != 0) {
        printf("CSIFW_TEST_APP: Failed to deinitialize service\n");
        return -1;
    }
	return 0;
}

