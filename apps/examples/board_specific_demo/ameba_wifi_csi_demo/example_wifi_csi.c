/******************************************************************************
 *
 * Copyright(c) 2007 - 2015 Realtek Corporation. All rights reserved.
 *
 *
 ******************************************************************************/
 /******************************************************************************

 * Copyright 2022 Samsung Electronics All Rights Reserved.
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

  ******************************************************************************/


#include <stdio.h>
#include <tinyara/config.h>
#include <tinyara/wifi/rtk/wifi_structures.h>
#include <tinyara/wifi/rtk/wifi_constants.h>
#include <sched.h>
#include <semaphore.h>

#define IP_ADDR_INVALID "0.0.0.0"
extern int wifi_csi_config(rtw_csi_action_parm_t *act_param);
extern int wifi_csi_report(uint32_t buf_len, uint8_t *csi_buf, uint32_t *len);
extern int wifi_is_running(unsigned char wlan_idx);
static sem_t wc_ready_sema;

static unsigned int csi_data_len = 2048;

/* wifi csi report callback */
void example_wifi_csi_report_cb(char *buf, int buf_len, int flags, void *userdata)
{
	sem_post(&wc_ready_sema);
	csi_data_len = flags;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int wificsi_main(int argc, char *argv[])
#endif
{
	printf("WIFI CSI\n");
	rtw_csi_action_parm_t act_param = {0};
	unsigned int len;
	unsigned char *csi_buf = NULL;
	unsigned long long *buff_tmp = NULL; /* to printf csi data*/
	unsigned int csi_seq_num;
	unsigned int timestamp;
	act_param.group_num = 0;
	act_param.mode = 2;  /* currently only supported */
	act_param.accuracy = 0;
	act_param.trig_period = 200;  /* ms */
	act_param.data_rate = 0xC;  /* ofdm 6 mpbs*/
//	act_param.mac_addr = {0x00, 0xe0, 0x4c, 0x81, 0x92, 0xbb};
	char ipv4_address[4];
	char ipv4_buf[16];
	while (1) {
		if (netlib_get_ipv4addr("wlan0", ipv4_address) == 0) {
			snprintf(ipv4_buf, 16, "%d.%d.%d.%d", ipv4_address[0], ipv4_address[1], ipv4_address[2], ipv4_address[3]);
		}
		if (wifi_is_running(WLAN0_IDX) && ((wifi_get_join_status()) && (strncmp(ipv4_buf, IP_ADDR_INVALID, sizeof(ipv4_buf))))) {
			sleep(2); /* 2s */
			break;
		}
		sleep(2); /* 2s */
	}
	/**
	 * should use semaphore to wait wifi event happen
	 * the following example shows that we wait for wifi csi ready
	*/
	sem_init(&wc_ready_sema, 0, 0);

	/* register wifi event callback function */
	wifi_reg_event_handler(15, example_wifi_csi_report_cb, NULL);

	/* csi cfg and csi en */
	act_param.act = 1;  /* csi cfg */
	act_param.enable = 0;
	wifi_csi_config(&act_param);

	act_param.act = 0;  /* csi en */
	act_param.enable = 1;
	wifi_csi_config(&act_param);
	csi_buf = (unsigned char *)malloc(csi_data_len);
	if (csi_buf) {
		while (1) {
			/* example: when wifi csi rx done, call csi report handle function. */
			if (sem_wait(&wc_ready_sema) == -1) {
				act_param.act = 0;  /* csi dis */
				act_param.enable = 0;
				wifi_csi_config(&act_param);
				break;
			}
			wifi_csi_report(csi_data_len, csi_buf, &len);
			/*do something for handing csi info*/
			timestamp = (int)(csi_buf[18] << 24) | (int)(csi_buf[17] << 16) | (int)(csi_buf[16] << 8) | (int)csi_buf[15];
			csi_seq_num = (int)(csi_buf[37] << 24) | (int)(csi_buf[36] << 16) | (int)(csi_buf[35] << 8) | (int)csi_buf[34];
			printf("\n[CH INFO] csi_sequence = %d,timestamp = %d us, csi data: \n", csi_seq_num, timestamp);
			buff_tmp = (u64 *)csi_buf;
			for (int i = 0; i < 19; i++) {
				printf("[%02d]0x%016llx\n", i, buff_tmp[i]);
			}
				printf("\n CSI raw data done!\n");

		}
		free(csi_buf);
	}


	/* unregister wifi event callback function */
	wifi_unreg_event_handler(15, example_wifi_csi_report_cb);

	sem_destroy(&wc_ready_sema);

	return 0;

}
