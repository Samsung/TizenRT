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

#include <debug.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "csifw_test.h"
#include <tinyara/config.h>
#include "csimanager/csifw_api.h"
#include <tinyara/wifi_csi/wifi_csi_struct.h>

csifw_service_handle g_CSIServiceInstance_1 = 0;
csifw_service_handle g_CSIServiceInstance_2 = 0;
csifw_service_handle g_CSIServiceInstance_3 = 0;
csifw_service_handle g_CSIServiceInstance_4 = 0;

void raw_data_cb_1(CSIFW_RES res, int csi_buff_len, unsigned char *csi_raw_buff, void *ptr) 
{
    printf("CSIFW_TEST_APP: Raw data callback 1 executed successfully. Buffer length: %d bytes\n", csi_buff_len);
}

void raw_data_cb_2(CSIFW_RES res, int csi_buff_len, unsigned char *csi_raw_buff, void *ptr) 
{
    printf("CSIFW_TEST_APP: Raw data callback 2 executed successfully. Buffer length: %d bytes\n", csi_buff_len);
}

void raw_data_cb_3(CSIFW_RES res, int csi_buff_len, unsigned char *csi_raw_buff, void *ptr) 
{
    printf("CSIFW_TEST_APP: Raw data callback 3 executed successfully. Buffer length: %d bytes\n", csi_buff_len);
}

void raw_data_cb_4(CSIFW_RES res, int csi_buff_len, unsigned char *csi_raw_buff, void *ptr) 
{
    printf("CSIFW_TEST_APP: Raw data callback 4 executed successfully. Buffer length: %d bytes\n", csi_buff_len);
}

int initialize_csi_service_1(int interval, csi_config_type_t config_type)
{
	service_callbacks_t sv_callbacks_1 = {
		.raw_data_cb = raw_data_cb_1,
		.parsed_data_cb = NULL,
		.user_data = NULL
	};
	
    CSIFW_RES result = csifw_registerService(&g_CSIServiceInstance_1, &sv_callbacks_1, config_type, interval);
    if (result != CSIFW_OK) {
        printf("CSIFW_TEST_APP: Failed to register csi service 1. Error code: %d\n", result);
        return -1;
    }
    printf("CSIFW_TEST_APP: Successfully registered csi service 1 with config type: %d, interval: %d ms\n", config_type, interval);
	return 0;
}

int initialize_csi_service_2(int interval, csi_config_type_t config_type)
{
    service_callbacks_t sv_callbacks_2 = {
		.raw_data_cb = raw_data_cb_2,
		.parsed_data_cb = NULL,
		.user_data = NULL
	};
	
    CSIFW_RES result = csifw_registerService(&g_CSIServiceInstance_2, &sv_callbacks_2, config_type, interval);
    if (result != CSIFW_OK) {
        printf("CSIFW_TEST_APP: Failed to register csi service 2. Error code: %d\n", result);
        return -1;
    }
    printf("CSIFW_TEST_APP: Successfully registered csi service 2 with config type: %d, interval: %d ms\n", config_type, interval);
	return 0;
}

int initialize_csi_service_3(int interval, csi_config_type_t config_type)
{
    service_callbacks_t sv_callbacks_3 = {
		.raw_data_cb = raw_data_cb_3,
		.parsed_data_cb = NULL,
		.user_data = NULL
	};
	
    CSIFW_RES result = csifw_registerService(&g_CSIServiceInstance_3, &sv_callbacks_3, config_type, interval);
    if (result != CSIFW_OK) {
        printf("CSIFW_TEST_APP: Failed to register csi service 3. Error code: %d\n", result);
        return -1;
    }
    printf("CSIFW_TEST_APP: Successfully registered csi service 3 with config type: %d, interval: %d ms\n", config_type, interval);
	return 0;
}

int initialize_csi_service_4(int interval, csi_config_type_t config_type)
{
    service_callbacks_t sv_callbacks_4 = {
		.raw_data_cb = raw_data_cb_4,
		.parsed_data_cb = NULL,
		.user_data = NULL
	};
	
    CSIFW_RES result = csifw_registerService(&g_CSIServiceInstance_4, &sv_callbacks_4, config_type, interval);
    if (result != CSIFW_OK) {
        printf("CSIFW_TEST_APP: Failed to register csi service 4. Error code: %d\n", result);
        return -1;
    }
    printf("CSIFW_TEST_APP: Successfully registered csi service 4 with config type: %d, interval: %d ms\n", config_type, interval);
	return 0;
}

int start_csi_service(csifw_service_handle handle)
{
	if (csifw_start(handle) != CSIFW_OK) {
		printf("CSIFW_TEST_APP: Failed to start CSI data collection for CSI service: %p\n", handle);
		return -1;
	}
	printf("CSIFW_TEST_APP: Successfully started CSI data collection for CSI service: %p\n", handle);
	return 0;
}

int stop_csi_service(csifw_service_handle handle)
{
	if (csifw_stop(handle) != CSIFW_OK) {
		printf("CSIFW_TEST_APP: Failed to stop CSI data collection for CSI service: %p\n", handle);
		return -1;
	}
	printf("CSIFW_TEST_APP: Successfully stopped CSI data collection for CSI service: %p\n", handle);
	return 0;
}

int deinitialize_csi_service(csifw_service_handle *handle)
{
	if (csifw_unregisterService(*handle) != CSIFW_OK) {
		printf("CSIFW_TEST_APP: Failed to unregister service service: %p\n", *handle);
		return -1;
	}
	*handle = 0;
	printf("CSIFW_TEST_APP: Successfully unregistered service service\n");
	return 0;
}

void display_detailed_usage_examples(void)
{
	printf("CSIFW_TEST_APP: CSI_TESTER APP usage: csifw_tc {option} [config_type] [interval] [run_time]\n"
			"\nTest Case Options:\n"
			" 1: Initialize CSI service only (handle 1)\n"
			"    Usage: csifw_tc 1 [config_type] [interval]\n"
			"    Example: csifw_tc 1 0 64\n"
			"             This initializes handle 1 with config_type=0 (HT_CSI_DATA) and interval=64ms\n"
			"    Note: This only initializes the service. You must manually start, stop, and deinitialize.\n"
			"\n"
			" 2: Start CSI data collection only (handle 1)\n"
			"    Usage: csifw_tc 2\n"
			"    Example: csifw_tc 2\n"
			"    Note: Handle 1 must be initialized before starting data collection.\n"
			"\n"
			" 3: Stop CSI data collection only (handle 1)\n"
			"    Usage: csifw_tc 3\n"
			"    Example: csifw_tc 3\n"
			"    Note: Data collection must be started before it can be stopped.\n"
			"\n"
			" 4: Deinitialize CSI service only (handle 1)\n"
			"    Usage: csifw_tc 4\n"
			"    Example: csifw_tc 4\n"
			"    Note: Handle 1 must be initialized and data collection stopped before deinitialization.\n"
			"\n"
			" 5: Get current CSI configuration, interval, and AP MAC address\n"
			"    Usage: csifw_tc 5\n"
			"    Example: csifw_tc 5\n"
			"    Note: Handle 1 must be initialized to get configuration information.\n"
			"\n"
			" 6: Set CSI collection interval [user_interval]\n"
			"    Usage: csifw_tc 6 [user_interval]\n"
			"    Example: csifw_tc 6 100\n"
			"             This sets the collection interval to 100ms\n"
			"    Note: Handle 1 must be initialized before setting interval.\n"
			"\n"
			" 7: Run basic sequence: Init->Start->Stop->Deinit (handle 1)\n"
			"    Usage: csifw_tc 7 [config_type] [interval]\n"
			"    Example: csifw_tc 7 2 50\n"
			"             This runs the basic sequence with config_type=2 (NON_HT_CSI_DATA) and interval=50ms\n"
			"\n"
			" 8: Run basic sequence for CSI service 2: Init->Start->Stop->Deinit\n"
			"    Usage: csifw_tc 8 [config_type] [interval]\n"
			"    Example: csifw_tc 8 1 75\n"
			"             This runs the basic sequence on handle 2 with config_type=1 (NON_HT_CSI_DATA) and interval=75ms\n"
			"\n"
			" 9: Run extended sequence with restart and multi-handle operations\n"
			"    Usage: csifw_tc 9 [config_type] [interval]\n"
			"    Example: csifw_tc 9 3 60\n"
			"             This runs an extended sequence with config_type=3 (HT_CSI_DATA_ACC1) and interval=60ms\n"
			"\n"
			"10: Run complex sequence with interleaved handle operations\n"
			"    Usage: csifw_tc 10 [config_type] [interval]\n"
			"    Example: csifw_tc 10 2 90\n"
			"             This runs a complex sequence with config_type=2 (HT_CSI_DATA_ACC1) and interval=90ms\n"
			"\n"
			"11: Run complex sequence with timed delays between operations\n"
			"    Usage: csifw_tc 11 [config_type] [interval]\n"
			"    Example: csifw_tc 11 1 55\n"
			"             This runs a complex sequence with delays using config_type=1 and interval=55ms\n"
			"\n"
			"12: Run multi-handle sequence with 4 concurrent handles\n"
			"    Usage: csifw_tc 12 [config_type] [interval]\n"
			"    Example: csifw_tc 12 2 70\n"
			"             This runs a multi-handle sequence with config_type=2 and interval=70ms\n"
			"\n"
			"13: Run multi-threaded scenario with 2 concurrent threads\n"
			"    Usage: csifw_tc 13 [config_type] [interval]\n"
			"    Example: csifw_tc 13 3 65\n"
			"             This runs a multi-threaded scenario with config_type=3 and interval=65ms\n"
			"\n"
			"14: Run basic sequence with user-defined parameters [config_type] [interval] [run_time]\n"
			"    Usage: csifw_tc 14 [config_type] [interval] [run_time]\n"
			"    Example: csifw_tc 14 0 64 100\n"
			"             This runs test case 16 with config_type=0, interval=64ms, run_time=100sec\n"
			"\n"
			"\nParameters:\n"
			" config_type: CSI configuration type (0-3)\n"
			"              0: HT_CSI_DATA\n"
			"              1: NON_HT_CSI_DATA\n"
			"              2: HT_CSI_DATA_ACC1\n"
			"              3: NON_HT_CSI_DATA_ACC1\n"
			" interval: Data collection interval in milliseconds (min 30ms)\n"
			" run_time: Application run time in seconds\n"
			" user_interval: New interval value for option 6\n"
			"\nExample: csifw_tc 14 0 64 100\n"
			"         This runs test case 14 with config_type=0, interval=64ms, run_time=100sec\n"
		);
}

int csifw_test_main(int argc, char **args)
{
    return csifw_tc_app_init(argc, args);
}

int csifw_tc_app_init(int argc, char **args)
{
	if (argc == 1) {
		display_detailed_usage_examples();
		return -1;
	}

	if (argc >= 6) {
		printf("CSIFW_TEST_APP: Invalid number of arguments\n");
		return -1;
	}
	
	int test_case = atoi(args[1]);
	int interval = 30;
	csi_config_type_t config_type = HT_CSI_DATA;
	int usr_conf = 0;
	int usr_intrvl = 0;
	int run_time = 0;

	/* Convert arguments once to avoid redundant atoi() calls */
	if (argc >= 3) {
		usr_conf = atoi(args[2]);
		if (test_case == 6) {
			usr_intrvl = usr_conf;
			usr_conf = 0;
		}
	}
	
	if (argc >= 4) {
		usr_intrvl = atoi(args[3]);
		config_type = usr_conf;
		interval = usr_intrvl;
	}

	if (argc >= 5) {
		run_time = atoi(args[4]);
	}

	switch (test_case) {
	case 1:
		return test_case_initialize_csi_service_only(interval, config_type);

	case 2:
		return test_case_start_csi_data_collection_only();

	case 3:
		return test_case_stop_csi_data_collection_only();

	case 4:
		return test_case_deinitialize_csi_service_only();

	case 5:
		return test_case_get_configuration_information();

	case 6:
		return test_case_set_collection_interval(usr_intrvl);

	case 7:
		return test_case_execute_primary_handle_basic_sequence(interval, config_type);

	case 8:
		return test_case_execute_secondary_handle_basic_sequence(interval, config_type);

	case 9:
		return test_case_extended_sequence_with_restart(interval, config_type);

	case 10:
		return test_case_interleaved_handle_operations(interval, config_type);

	case 11:
		return test_case_timed_delays_sequence(interval, config_type);

	case 12:
		return test_case_multi_handle_concurrent(interval, config_type);

	case 13:
		return test_case_multi_threaded_execution(interval, config_type);

	case 14:
		return test_case_basic_sequence_with_parameters(usr_intrvl, usr_conf, run_time);

	default:
		printf("CSIFW_TEST_APP: Invalid test case specified: %d\n", test_case);
		return -1;
	}
}
