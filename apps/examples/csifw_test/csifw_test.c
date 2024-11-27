/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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

#include "csifw_test.h"
#include "csifw/csifw_api.h"
#include <stdlib.h>
#include <stdio.h>

/****************************************************************************
 * csifw_test
 ****************************************************************************/

#include <fcntl.h>
static int gFd; /* File descriptor*/

#define PRNT_LN1 printf("______________________________________________________________________________________________________\n")
#define PRNT_LN2 printf("______________________________________________________________________________________________________\n\n")
#define PRNT_LN3 printf("\n\n______________________________________________________________________________________________________\n")

#define LONG_RUN_TEST 0
#define PRINT_RAW_DATA 0
#define LOG_PARSED_DATA 0

typedef enum  CSI_DATA_TYPE {
    CSI_DATA_UNKNOWN = -1,
    RAW_CSI_DATA =0,
    PARSED_CSI_DATA =1,
    RAW_PARSED_CSI_DATA =2,
}CSI_DATA_TYPE;

static CSI_DATA_TYPE g_csi_data_type;
static CSI_CONFIG_TYPE g_csi_config_type;
static int g_interval;
static unsigned int gCounter;
static unsigned int g_app_id[3] = {1,2,3};
static unsigned int g_app_number = 0;

static void print_buf(const unsigned char* buf, int len)
{
	printf("\n RAW DATA %d\n\n", len);
	unsigned long long *buff_tmp = (u64 *)buf;
	int buff_len = (len / 8) + 1;
	for (int i = 0; i < buff_len; i++) 
		printf("[%02d]0x%016llx\n", i, buff_tmp[i]);
}

static void demo_upd_raw_data_listener(CSIFW_RES res, int csi_buff_len, unsigned char *csi_raw_buff, void* ptr)
{
	if(res == CSIFW_OK) {
		gCounter++;
		unsigned int app_id = *(unsigned int*)ptr;
		printf("\n%d. Raw Data received in APP[%d] [%d]\n", gCounter, app_id, csi_buff_len);
		#if PRINT_RAW_DATA
		printf("\n[APP]LOGGING DATA\n");
		print_buf(csi_raw_buff, csi_buff_len);
		#endif
		return;
	}
	if(res == CSIFW_ERROR_WIFI_DIS_CONNECTED) {
		printf("<URGENT CALLBACK N/W CHANGE>[APP] wifi disconnected\n");
		printf("<URGENT CALLBACK N/W CHANGE>[APP] so service will PAUSE\n");
		return;
	}
	if (res == CSIFW_ERROR) {
		printf("[APP] Some error\n");
		return;
	}
}

static void demo_upd_parsed_data_listener(CSIFW_RES res, int csi_data_len, float *csi_parsed_buff, void* ptr)
{
	// print parsed data buffer
	printf("\n%d. Parsed Data received in APP [%d]\n", gCounter, csi_data_len);
	
	#if LOG_PARSED_DATA
	printf("[APP] Displaying parsed data, csi_data_len: %d\n\n", csi_data_len);
	for(int i = 0; i < csi_data_len; i += 2) {
		printf("sub_carrier:%d[%f,%f], ", i/2, csi_parsed_buff[i], csi_parsed_buff[i+1]);
		}
		printf("\n");
	#endif
	printf("\n\n");
}

static int start_test(void)
{
	printf("\n[APP]: CSI Manager DEFAULT FLOW \n");
	PRNT_LN1;
	PRNT_LN2;

	printf("[APP]: csi_service_init\n");
	PRNT_LN2;
	
	CSIFW_RES res = csi_service_init(g_csi_config_type, demo_upd_raw_data_listener, NULL, g_interval, &g_app_id[g_app_number]); 
	if (res != CSIFW_OK) {
		PRNT_LN2;
		if (res == CSIFW_ERROR_WIFI_NOT_CONNECTED) {
			printf("\n[APP]: CSIFW-> {WIFI NOT CONNECTED}\n");
		} else {
			printf("\n[APP]: CSI Manager INIT FAIL\n");
			return 0;
		}
	}
	g_app_number++;
	PRNT_LN2;
	printf("\n[APP]: CSI Manager INIT SUCCESS\n");
	printf("[APP]: SLEEP--> 5 seconds\n");
	PRNT_LN2;
	// sleep(5);

	PRNT_LN3;
	printf("[APP]: 1 csi_service_start\n");
	PRNT_LN2;
	// sleep(2);
	if (csi_service_start() == CSIFW_ERROR) {
		PRNT_LN2;
		printf("[APP]: CSI Manager START FAIL\n");
		return 0;
	}
	printf("[APP]: CSI Manager START SUCCESS\n");
	printf("[APP]: SLEEP--> 15 seconds\n");
	PRNT_LN2;
#if LONG_RUN_TEST
	while (1) {
		sleep(2);
	}
#else
	sleep(15);
	
	PRNT_LN3;
	printf("[APP]: csi_service_stop (CSIFW_WIFI_DISCONNECTED)\n");
	PRNT_LN2;
	// sleep(2);
	CSIFW_REASON reason = CSIFW_NORMAL;
	
	if (csi_service_stop(CSIFW_WIFI_DISCONNECTED)== CSIFW_ERROR) {
		printf("[APP]: CSI Manager STOP FAIL\n");
		PRNT_LN2;
		return 0;
		}
	printf("[APP]: CSI Manager STOP SUCCESS\n");
	printf("[APP]: SLEEP--> 10 seconds\n");
	PRNT_LN2;
	sleep(10);

	PRNT_LN3;
	printf("[APP]: 2 csi_service_start\n");
	PRNT_LN2;
	// sleep(2);
	if (csi_service_start() == CSIFW_ERROR) {
		printf("[APP]: CSI Manager START FAIL\n");
		PRNT_LN2;
		return 0;
	}
	printf("[APP]: SLEEP--> 20 seconds\n");
	PRNT_LN2;
	sleep(20);

	PRNT_LN3;
	printf("[APP]: csi_service_stop (CSIFW_NORMAL)\n");
	PRNT_LN2;
	// sleep(2);
	if (csi_service_stop(CSIFW_NORMAL) == CSIFW_ERROR) {
		printf("[APP]: CSI Manager STOP FAIL\n");
		PRNT_LN2;
		return 0;
	}
	printf("[APP]: SLEEP--> 10 seconds\n");
	PRNT_LN2;
	sleep(10);

	PRNT_LN3;
	printf("[APP]: csi_service_deinit\n");
	PRNT_LN2;
	// sleep(2);
	if (csi_service_deinit() == CSIFW_ERROR) {
		printf("[APP]: CSI Manager DEINIT FAIL\n");
		PRNT_LN2;
		return 0;
	}
	g_app_number--;
	printf("[APP]: SLEEP--> 20 seconds\n");
	PRNT_LN2;
	// sleep(20);
	PRNT_LN2;
	printf("[APP]: DEFAULT FLOW ENDSSS\n");
	printf("[APP]: DEFAULT FLOW ENDSSS\n");
#endif /* LONG_RUN_TEST */
	return 0;
}

void csifw_test_main(int argc, char **args)
{
	/* default values */
	g_csi_data_type = RAW_CSI_DATA;
	g_csi_config_type = HT_CSI_DATA;
	g_interval = 40;  // interval in ms
	printf("WIFI CSI %d\n", argc);

	if(argc > 1)
	{
		g_interval = atoi(args[1]);
		printf("PING_INTERVAL SET: %d us\n", g_interval);

		if(argc == 3)
		{
			g_csi_config_type = atoi(args[2]);
			printf("CSI_CONFIG_TYPE SET: %d \n", g_csi_config_type);
		}
		else
		{
			printf("CSI_CONFIG_TYPE default : %d \n", g_csi_config_type);	
		}
		if (g_csi_config_type != HT_CSI_DATA && g_csi_config_type != NON_HT_CSI_DATA) {
			printf("Invalid CSI type, try again [0: HT_CSI_DATA, 1: NON_HT_CSI_DATA]\n");
			return;
		}
	}
	else
	{
		printf("PING_INTERVAL default: %d us\n", g_interval);
		printf("CSI_CONFIG_TYPE default : %d \n", g_csi_config_type);	
	}
	start_test();
}
