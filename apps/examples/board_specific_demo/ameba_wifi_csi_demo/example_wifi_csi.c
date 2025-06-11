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
#include <example_wifi_csi.h>

#define IP_ADDR_INVALID "0.0.0.0"
extern int wifi_csi_config(rtw_csi_action_parm_t *act_param);
extern int wifi_csi_report(uint32_t buf_len, uint8_t *csi_buf, uint32_t *len);
extern int wifi_is_running(unsigned char wlan_idx);
extern int wifi_get_setting(unsigned char wlan_idx, struct _rtw_wifi_setting_t *psetting);
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
#ifdef CONFIG_ARCH_CHIP_RTL8730E
	printf("WIFI CSI\n");
	rtw_csi_action_parm_t act_param = {0};
	unsigned int len;
	unsigned char *csi_buf = NULL;
	unsigned long long *buff_tmp = NULL; /* to printf csi data*/
	unsigned int csi_seq_num;
	unsigned int timestamp;
	struct _rtw_wifi_setting_t wlan_setting = {0};
	unsigned char device_0[6] = {0};  /* get mac_addr of AP via wifi_get_setting */
	/* To test multi-STA CSI, uncomment and set the mac addr of the other devices */
	// unsigned char device_1[6] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};  /* need modify to mac address of associated AP when sta mode */
	// unsigned char device_2[6] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};  /* need modify to mac address of associated AP when sta mode */
	act_param.group_num = 0;
	act_param.mode = 2;  /* 0: rx normal 2:rx respon */
	act_param.accuracy = 0;
	act_param.trig_period = 200;  /* ms */
	act_param.data_rate = 0xc;  /* ofdm 6 mpbs*/
	act_param.ch_opt = 0; //0: legacy 1: non-legacy
	act_param.trig_frame_mgnt = 0;   /* no need for rx resp mode, default 0*/
	act_param.trig_frame_ctrl = 0;   /* no need for rx resp mode, default 0*/
	act_param.trig_frame_data = 0;   /* no need for rx resp mode, default 0*/
	/* If rx normal mode (mode = 0) is used, set the packet type that will trigger CSI */
	// act_param.trig_frame_data = CSI_TRIG_QOS_DATA 

	char ipv4_address[4];
	char ipv4_buf[16];
	while (1) {
		if (netlib_get_ipv4addr("wlan0", ipv4_address) == 0) {
			snprintf(ipv4_buf, 16, "%d.%d.%d.%d", ipv4_address[0], ipv4_address[1], ipv4_address[2], ipv4_address[3]);
		}
		if (wifi_is_running(WLAN0_IDX) && ((wifi_get_join_status()) && (strncmp(ipv4_buf, IP_ADDR_INVALID, sizeof(ipv4_buf))))) {
			/* get mac_addr of connected ap */
			wifi_get_setting(WLAN0_IDX, &wlan_setting);
			memcpy(device_0, wlan_setting.bssid, 6);
			sleep(2); /* 2s */
			break;
		}
		sleep(2); /* 2s */
	}
	memcpy(act_param.mac_addr, device_0, 6);
	/**
	 * should use semaphore to wait wifi event happen
	 * the following example shows that we wait for wifi csi ready
	*/
	
	sem_init(&wc_ready_sema, 0, 0);

	/* register wifi event callback function */
	wifi_reg_event_handler(WIFI_CSI_DONE, example_wifi_csi_report_cb, NULL);

	/* csi cfg and csi en */
	act_param.act = 1;  /* csi cfg */
	act_param.enable = 0;
	wifi_csi_config(&act_param);

	/* Enable CSI for device_0 */
	act_param.trig_flag = 0;
	act_param.act = 0;  /* csi en */
	act_param.enable = 1;
	wifi_csi_config(&act_param);

	/* Uncomment the following to test Multi-STA CSI */
	/* Enable CSI for device_1 */
	// act_param.trig_flag = 1;
	// memcpy(act_param.mac_addr, device_1, 6);
	// wifi_csi_config(&act_param);
	
	/* Uncomment the following to test Multi-STA CSI */
	/* Enable CSI for device_2 */
	// act_param.trig_flag = 2;
	// memcpy(act_param.mac_addr, device_2, 6);
	// wifi_csi_config(&act_param);

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
	wifi_unreg_event_handler(WIFI_CSI_DONE, example_wifi_csi_report_cb);

	sem_destroy(&wc_ready_sema);

	return 0;
#else
	printf("\n wificsi example only supported on RTL8730E!\n");
	return 0;
#endif //#ifdef CONFIG_ARCH_CHIP_RTL8730E

}
