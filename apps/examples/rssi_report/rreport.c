/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include "rreport_config.h"

/****************************************************************************
 * Private Data
 ****************************************************************************/


/****************************************************************************
* Public Data
****************************************************************************/
#define USAGE \
	"rreport ssid passwd url"

typedef struct {
	char *url;
} rr_app_msg;

/*
 * External Function
 */
extern char *rr_generate_message(int *data, int size);
extern int rr_send_result(char *str, char *url);
extern int rr_sync_time(void);
extern int rr_connect_ap(char *ssid, char *passwd);

/*
 * Private Function
 */

void monitor_rssi(rr_app_msg *app_msg)
{
	int rssi[RR_NUM_OF_RSSI] = {0,};
	//for (int j = 0; j < 3; j++) {
	while (1) {
		for (int i = 0; i < RR_NUM_OF_RSSI; i++) {
			rssi[i] = rr_get_rssi();
			usleep(RR_INTERVAL * 1000);
		}

		char *msg = rr_generate_message(rssi, RR_NUM_OF_RSSI);
		if (!msg) {
			RR_ERR;
			continue;
		}

		rr_send_result(msg, app_msg->url);

		free(msg);
	}

	return;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int rreport_main(int argc, char *argv[])
#endif
{
	if (argc != 4) {
		printf("invalid arguments\n");
		return -1;
	}
	rr_app_msg msg = {argv[3]};

	if (rr_connect_ap(argv[1], argv[2]) < 0) {
		printf("fail to connect AP\n");
		return -1;
	}

	if (rr_sync_time() < 0) {
		printf("fail to sync time\n");
		return -1;
	}

	monitor_rssi(&msg);

	printf("report is finish\n");
	return 0;
}
