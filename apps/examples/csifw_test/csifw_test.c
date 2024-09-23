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

#include <stdio.h>

/****************************************************************************
 * csifw_test
 ****************************************************************************/

#include <fcntl.h>
static int gFd; /* File descriptor*/

#define PRNT_LN1 printf("______________________________________________________________________________________________________\n")
#define PRNT_LN2 printf("______________________________________________________________________________________________________\n\n")
#define PRNT_LN3 printf("\n\n______________________________________________________________________________________________________\n")


static unsigned int gCounter;

static void print_buf(const unsigned char* buf, int len)
{
	printf("\n RAW DATA %d\n\n", len);
	unsigned long long *buff_tmp = (u64 *)buf;
	int buff_len = (len/8) + 1;
	for (int i = 0; i < buff_len; i++) 
		printf("[%02d]0x%016llx\n", i, buff_tmp[i]);
}

static void demo_upd_raw_data_listener(CSIFW_RES res, int csi_buff_len, unsigned char *csi_raw_buff, void* ptr)
{
	if(res == CSIFW_OK) {
		gCounter++;
		printf("\n%d. Raw Data received in APP [%d]\n", gCounter, csi_buff_len);
		printf("\n[APP]LOGGING DATA\n");
		print_buf(csi_raw_buff, csi_buff_len);
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

#define LOG_PARSED_DATA 1
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

int csifw_test_main(void)
{
	printf("\n[APP]: CSI Manager DEFAULT FLOW \n");
	PRNT_LN1;
	PRNT_LN2;

	PRNT_LN3;
	printf("[APP]: csi_manager_init\n");
	csi_manager_init();
	PRNT_LN3;

	printf("[APP]: csi_service_init\n");
	PRNT_LN2;
	CSIFW_RES res = csi_service_init(demo_upd_raw_data_listener, demo_upd_parsed_data_listener, 40000, NULL); 
	if (res != CSIFW_OK) {
		PRNT_LN2;
		if (res == CSIFW_ERROR_WIFI_NOT_CONNECTED) {
			printf("\n[APP]: CSIFW-> {WIFI NOT CONNECTED}\n");
		} else {
			printf("\n[APP]: CSI Manager INIT FAIL\n");
			return 0;
		}
	}
	PRNT_LN2;
	printf("\n[APP]: CSI Manager INIT SUCCESS\n");
	printf("[APP]: SLEEP--> 5 seconds\n");
	PRNT_LN2;
	sleep(5);

	PRNT_LN3;
	csi_action_param_t config = {0};
	config.group_num = CSI_GROUP_NUM_1;
	config.mode = CSI_MODE_NORMAL;
	config.accuracy = CSI_ACCU_1BYTE;
	config.ch_opt = CSI_CH_NON_LEGACY;
	config.trig_period = 200;
	config.data_rate = 0x80; //Kernel/os/board/rtl8720e/src/component/wifi/driver/include/rtw_wifi_constants.h L465  MGN_6M

	printf("[APP]: csi_service_set_config\n");
	PRNT_LN2;
	if (csi_service_set_config(config) == CSIFW_ERROR) {
		PRNT_LN2;
		printf("[APP]: CSI Manager CONFIG SET FAIL\n");
		return 0;
	}
	printf("[APP]: CSI Manager CONFIG SET SUCCESS\n");
	PRNT_LN2;

	PRNT_LN3;
	printf("[APP]: csi_service_start\n");
	PRNT_LN2;
	sleep(2);
	if (csi_service_start() == CSIFW_ERROR) {
		PRNT_LN2;
		printf("[APP]: CSI Manager START FAIL\n");
		return 0;
	}
	printf("[APP]: CSI Manager START SUCCESS\n");
	printf("[APP]: SLEEP--> 15 seconds\n");
	PRNT_LN2;
	sleep(15);
	
	PRNT_LN3;
	printf("[APP]: csi_service_stop\n");
	PRNT_LN2;
	sleep(2);
	CSIFW_REASON reason = CSIFW_NORMAL;
	
	if (csi_service_stop(reason)== CSIFW_ERROR) {
		printf("[APP]: CSI Manager STOP FAIL\n");
		PRNT_LN2;
		return 0;
		}
	printf("[APP]: CSI Manager STOP SUCCESS\n");
	printf("[APP]: SLEEP--> 10 seconds\n");
	PRNT_LN2;
	sleep(10);

	PRNT_LN3;
	printf("[APP]: csi_service_start\n");
	PRNT_LN2;
	sleep(2);
	if (csi_service_start() == CSIFW_ERROR) {
		printf("[APP]: CSI Manager START FAIL\n");
		PRNT_LN2;
		return 0;
	}
	printf("[APP]: SLEEP--> 20 seconds\n");
	PRNT_LN2;
	sleep(20);

	PRNT_LN3;
	printf("[APP]: csi_service_stop\n");
	PRNT_LN2;
	sleep(2);
	if (csi_service_stop(reason) == CSIFW_ERROR) {
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
	sleep(2);
	if (csi_service_deinit() == CSIFW_ERROR) {
		printf("[APP]: CSI Manager DEINIT FAIL\n");
		PRNT_LN2;
		return 0;
	}
	printf("[APP]: SLEEP--> 20 seconds\n");
	PRNT_LN2;
	sleep(20);
	PRNT_LN2;
	printf("[APP]: DEFAULT FLOW ENDSSS\n");
	printf("[APP]: DEFAULT FLOW ENDSSS\n");
	
	return 0;
}

void csi_manager_csifw_test_init(void)
{
	csifw_test_main();
}

