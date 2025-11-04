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

#include "csimanager/csifw_api.h"
#include "csifw_test.h"
#include <debug.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

csifw_service_handle handle_1 = 0;
csifw_service_handle handle_2 = 0;
csifw_service_handle handle_3 = 0;
csifw_service_handle handle_4 = 0;

void raw_data_cb_1(CSIFW_RES res, int csi_buff_len, unsigned char *csi_raw_buff, void *ptr) 
{
    printf("Send success from raw_data_listener1 %d\n", csi_buff_len);
}

void raw_data_cb_2(CSIFW_RES res, int csi_buff_len, unsigned char *csi_raw_buff, void *ptr) 
{
    printf("Send success from raw_data_listener2 %d\n", csi_buff_len);
}

void raw_data_cb_3(CSIFW_RES res, int csi_buff_len, unsigned char *csi_raw_buff, void *ptr) 
{
    printf("Send success from raw_data_listener3 %d\n", csi_buff_len);
}

void raw_data_cb_4(CSIFW_RES res, int csi_buff_len, unsigned char *csi_raw_buff, void *ptr) 
{
    printf("Send success from raw_data_listener4 %d\n", csi_buff_len);
}

static int c_init_1(int interval, csi_config_type_t config_type)
{
	service_callbacks_t p_svc_in_1 = {
		.raw_data_cb = raw_data_cb_1,
		.parsed_data_cb = NULL,
		.user_data = NULL
	};
	
    CSIFW_RES res = csifw_registerService(&handle_1, &p_svc_in_1, config_type, interval);
    if (res != CSIFW_OK) {
        printf("CSIFW_TEST_APP: csifw_registerService1 FAIL\n");
        return -1;
    }
    sleep(1);
    printf("CSIFW_TEST_APP: csifw_registerService1 SUCCESS\n");
	return 0;
}

static int c_init_2(int interval, csi_config_type_t config_type)
{
    	service_callbacks_t p_svc_in_2 = {
		.raw_data_cb = raw_data_cb_2,
		.parsed_data_cb = NULL,
		.user_data = NULL
	};
	
    CSIFW_RES res = csifw_registerService(&handle_2, &p_svc_in_2, config_type, interval);
    if (res != CSIFW_OK) {
        printf("CSIFW_TEST_APP: csifw_registerService2 FAIL\n");
        return -1;
    }
    sleep(1);
    printf("CSIFW_TEST_APP: csifw_registerService2 SUCCESS\n");
	return 0;
}

static int c_init_3(int interval, csi_config_type_t config_type)
{
    	service_callbacks_t p_svc_in_3 = {
		.raw_data_cb = raw_data_cb_3,
		.parsed_data_cb = NULL,
		.user_data = NULL
	};
	
    CSIFW_RES res = csifw_registerService(&handle_3, &p_svc_in_3, config_type, interval);
    if (res != CSIFW_OK) {
        printf("CSIFW_TEST_APP: csifw_registerService3 FAIL\n");
        return -1;
    }
    sleep(1);
    printf("CSIFW_TEST_APP: csifw_registerService3 SUCCESS\n");
	return 0;
}

static int c_init_4(int interval, csi_config_type_t config_type)
{
   	service_callbacks_t p_svc_in_4 = {
		.raw_data_cb = raw_data_cb_4,
		.parsed_data_cb = NULL,
		.user_data = NULL
	};
	
    CSIFW_RES res = csifw_registerService(&handle_4, &p_svc_in_4, config_type, interval);
    if (res != CSIFW_OK) {
        printf("CSIFW_TEST_APP: csifw_registerService4 FAIL\n");
        return -1;
    }
    sleep(1);
    printf("CSIFW_TEST_APP: csifw_registerService4 SUCCESS\n");
	return 0;
}

static int c_start_common(csifw_service_handle h)
{
	if (csifw_start(h) != CSIFW_OK) {
		printf("CSIFW_TEST_APP: csifw_start FAIL \n");
		return -1;
	}
	sleep(1);
	printf("CSIFW_TEST_APP: csifw_start SUCCESS\n");
	return 0;
}

static int c_start_1(void)
{
	return c_start_common(handle_1);
}

static int c_start_2(void)
{
	return c_start_common(handle_2);
}

static int c_start_3(void)
{
	return c_start_common(handle_3);
}

static int c_start_4(void)
{
	return c_start_common(handle_4);
}

static int c_stop_common(csifw_service_handle h)
{
	if (csifw_stop(h) != CSIFW_OK) {
		printf("CSIFW_TEST_APP: csifw_stop FAIL \n");
		return -1;
	}
	sleep(1);
	printf("CSIFW_TEST_APP: csifw_stop SUCCESS\n");
	return 0;
}

static int c_stop_1(void)
{
	return c_stop_common(handle_1);
}

static int c_stop_2(void)
{
	return c_stop_common(handle_2);
}

static int c_stop_3(void)
{
	return c_stop_common(handle_3);
}

static int c_stop_4(void)
{
	return c_stop_common(handle_4);
}

static int c_deinit_common(csifw_service_handle *h)
{
	if (csifw_unregisterService(*h) != CSIFW_OK) {
		printf("CSIFW_TEST_APP: csifw_unregisterService FAIL\n");
		return -1;
	}
	*h = 0; // Reset handle value
	sleep(1);
	printf("CSIFW_TEST_APP: csifw_unregisterService SUCCESS\n");
	return 0;
}

static int c_deinit_1(void)
{
	return c_deinit_common(&handle_1);
}

static int c_deinit_2(void)
{
	return c_deinit_common(&handle_2);
}

static int c_deinit_3(void)
{
	return c_deinit_common(&handle_3);
}

static int c_deinit_4(void)
{
	return c_deinit_common(&handle_4);
}

static void c_getConfig(void)
{
	csi_config_type_t config_type2;
	if (csifw_get_config(handle_1, &config_type2) != CSIFW_OK) {
		printf("CSIFW_TEST_APP: Failed to get CSI configuration\n");
		return;
	}

	const char *type_str;
	switch (config_type2) {
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

	printf("CSIFW_TEST_APP: CSI Configuration Type: %s\n", type_str);
	sleep(1);
}

static void c_getInterval(void)
{
	unsigned int intrvl;
	csifw_get_interval(handle_1, &intrvl); // Pass handle directly
	printf("CSIFW_TEST_APP: CSI INTERVAL is: %d\n", intrvl);
}

static int c_setInterval(int interval)
{
	int ret = 1;
	if (csifw_set_interval(handle_1, interval) == CSIFW_OK) {
		printf("CSIFW_TEST_APP: CSI INTERVAL updated: %d\n", interval);
	} else {
		printf("CSIFW_TEST_APP: CSI INTERVAL FAIL");
		ret = -1;
	}
	return ret;
}

static void c_getMACAddr(void)
{
	csifw_mac_info mac_inf;
	csifw_get_ap_mac_addr(handle_1, &mac_inf); // Pass handle directly
	printf("CSIFW_TEST_APP: AP MAC: %02x %02x %02x %02x %02x %02x\n", mac_inf.mac_addr[0], mac_inf.mac_addr[1], mac_inf.mac_addr[2], mac_inf.mac_addr[3], mac_inf.mac_addr[4], mac_inf.mac_addr[5]);
}

static int run_basic_sequence_1(int interval, csi_config_type_t config_type)
{
	c_init_1(interval, config_type);
	c_start_1();
	c_stop_1();
	c_deinit_1();
	sleep(1);
	return 0;
}

static int run_basic_sequence_2(int interval, csi_config_type_t config_type)
{
	c_init_2(interval, config_type);
	c_start_2();
	c_stop_2();
	c_deinit_2();
	sleep(1);
	return 0;
}

typedef struct {
	int pr_interval;
	csi_config_type_t pr_config_type;
} thread_params_t;

static void* thread_run_sequence_1_wrapper(void* arg) {
	thread_params_t* params = (thread_params_t*)arg;
	run_basic_sequence_1(params->pr_interval, params->pr_config_type);
	return NULL;
}

static void* thread_run_sequence_2_wrapper(void* arg) {
	thread_params_t* params = (thread_params_t*)arg;
	run_basic_sequence_2(params->pr_interval, params->pr_config_type);
	return NULL;
}

int csifw_test_main(int argc, char **args)
{
    return csifw_app_init(argc, args);
}

int csifw_app_init(int argc, char **args)
{
	if (argc == 1) {
		printf("CSI_TESTER APP usage: csifw_app {option}\n" 
				" 1: CSI Init only\n" 
				" 2: CSI Start only\n" 
				" 3: CSI Stop only\n" 
				" 4: CSI DeInit only\n" 
				" 5: Get Config/Interval/MAC\n" 
				" 6: Set Interval {user_interval}\n" 
				" 7: Basic sequence (Init-Start-Stop-DeInit)\n" 
				" 8: Basic sequence for handle 2\n" 
				" 9: Running extended sequence\n" 
				"10: Running sequence_1 with network disconect-connec\n" 
				"11: Running sequence_2 with network disconect-connec\n" 
				"12: Complex sequence 1\n" 
				"13: Complex sequence 2 (with sleep)\n" 
				"14: Multi-handle sequence\n"
				"15: Multi-thread scenario\n"
				"16: Basic sequence (Init-Start-Stop-DeInit) with user defined config and Interval\n");
		return -1;
	}

	if (argc >= 5) {
		printf("Invalid number of arguments\n");
		return -1;
	}
	
	int test_case = atoi(args[1]);
	int interval = 30;	// interval in ms
	csi_config_type_t config_type = HT_CSI_DATA;
	int usr_conf = 0;
	int usr_intrvl = 0;

	if (argc >= 3) {
		usr_conf = atoi(args[2]);
	}
	
	if (argc >= 4) {
		usr_intrvl = atoi(args[3]);
	}

	if (argc == 4) {
		config_type = usr_conf;
		interval = usr_intrvl;
	}

	switch (test_case) {
	case 1:
		printf("Running CSI Init only\n");
		return c_init_1(interval, config_type);

	case 2:
		printf("Running CSI Start only\n");
		return c_start_1();

	case 3:
		printf("Running CSI Stop only\n");
		return c_stop_1();

	case 4:
		printf("Running CSI DeInit only\n");
		return c_deinit_1();

	case 5:
		printf("Getting Config/Interval/MAC\n");
		c_getConfig();
		c_getInterval();
		c_getMACAddr();
		return 0;

	case 6:
		printf("Setting Interval: %d\n", usr_intrvl);
		if (c_setInterval(usr_intrvl) == 1) {
			return 0;
		} else {
			printf("Setting interval failed\n");
			return -1;
		}

	case 7:
		printf("Running basic sequence\n");
		return run_basic_sequence_1(interval, config_type);

	case 8:
		printf("Running basic sequence for handle 2\n");
		return run_basic_sequence_2(interval, config_type);

	case 9:
		printf("Running extended sequence\n");
		c_init_1(interval, config_type);
		c_start_1();
		sleep(3);
		c_stop_1();
		sleep(2);
		c_start_1();
		sleep(3);
		c_stop_1();
		c_deinit_1();

		c_init_1(interval, config_type);
		c_init_2(interval, config_type);
		c_start_1();
		c_start_2();
		sleep(3);
		c_stop_2();
		c_deinit_2();
		return 0;

	case 10:
		printf("test not available\n");
#if 0			
		c_init_1(interval, config_type);
		wifi_manager_stop();
		c_init_2(interval, config_type);
		wifi_manager_start();
		c_start_1();
		c_start_2();
#endif		
		return 0;

	case 11:
		printf("test not available\n");
#if 0		
		c_init_1(interval, config_type);
		c_start_1();
		sleep(3);
		wifi_manager_stop();
		c_stop_1();
		sleep(2);
		wifi_manager_start();
		c_start_1();
		sleep(3);
		c_stop_1();
		c_deinit_1();
#endif		
		return 0;

	case 12:
		printf("Running complex sequence 1\n");
		c_init_1(interval, config_type);
		c_init_2(interval, config_type);
		c_start_2();
		c_start_1();
		sleep(5);
		c_stop_1();
		c_deinit_1();
		c_stop_2();
		c_start_2();
		sleep(5);
		c_stop_2();
		c_deinit_2();
		c_init_1(interval, config_type);
		return 0;

	case 13:
		printf("Running complex sequence 2 (with sleep)\n");
		c_init_1(interval, config_type);
		sleep(3);
		c_init_2(interval, config_type);
		sleep(3);
		c_start_2();
		sleep(3);
		c_start_1();
		sleep(3);
		c_stop_1();
		sleep(3);
		c_deinit_1();
		sleep(3);
		c_stop_2();
		sleep(3);
		c_start_2();
		sleep(3);
		c_stop_2();
		sleep(3);
		c_deinit_2();
		sleep(3);
		c_init_1(interval, config_type);
		return 0;

	case 14:
		printf("Running multi-handle (4) sequence\n");
		c_init_1(interval, config_type);
		c_init_2(interval, config_type);
		c_init_3(interval, config_type);
		c_init_4(interval, config_type);
		c_start_1();
		c_start_2();
		c_start_3();
		c_start_4();
		sleep(3);
		c_stop_1();
		c_stop_2();
		c_stop_3();
		c_stop_4();
		c_deinit_1();
		c_deinit_2();
		c_deinit_3();
		c_deinit_4();
		return 0;

		
	case 15: {
		printf("Running multithreaded test scenario\n");
		pthread_t thread1, thread2;

		thread_params_t params1;
		thread_params_t params2;

		params1.pr_interval = interval;
		params1.pr_config_type = config_type;
		params2.pr_interval = interval;
		params2.pr_config_type = config_type;

		// Create thread for sequence 1, using the file-scoped wrapper and passing params1
		if (pthread_create(&thread1, NULL, thread_run_sequence_1_wrapper, &params1) != 0) {
			printf("Failed to create thread 1\n");
			return -1; 
		}

		// Create thread for sequence 2, using the file-scoped wrapper and passing params2
		if (pthread_create(&thread2, NULL, thread_run_sequence_2_wrapper, &params2) != 0) {
			printf("Failed to create thread 2\n");
			// Attempt to gracefully handle the failure by joining the first thread.
			pthread_join(thread1, NULL); 
			return -1;
		}

		// Wait for both threads to complete their execution.
		pthread_join(thread1, NULL);
		pthread_join(thread2, NULL);

		printf("Multithreaded test completed\n");
		return 0;	
	}

	case 16:
		printf("Running basic sequence with config: %d and interval: %d\n", usr_conf, usr_intrvl);
		return run_basic_sequence_1(usr_intrvl, usr_conf);

	default:
		printf("Invalid test case: %d\n", test_case);
		return -1;
	}
}
