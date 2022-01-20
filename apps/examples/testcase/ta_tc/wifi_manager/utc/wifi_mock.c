/****************************************************************************
 *
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
 ****************************************************************************/
#include <tinyara/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <net/if.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/net/if/wifi.h>
#include "wifi_mock.h"

#define WU_INTF_NAME "wlan0"
#define TAG "[WT]"

static inline int _send_msg(lwnl_msg *msg)
{
	int fd = socket(AF_LWNL, SOCK_RAW, LWNL_ROUTE);
	if (fd < 0) {
		printf("send lwnl msg open error\n");
		return -1;
	}

	int res = write(fd, msg, sizeof(*msg));
	close(fd);
	if (res < 0) {
		printf("send lwnl msg write error\n");
		return -2;
	}
	return 0;
}

int utc_control_mock(int cmd, int key, int value, int wait)
{
	vwifi_ioctl_msg_s vmsg = {key, value, wait};
	trwifi_msg_s dmsg = {cmd, (void *)&vmsg};

	trwifi_result_e res = TRWIFI_SUCCESS;
	lwnl_msg msg = {WU_INTF_NAME, {LWNL_REQ_WIFI_IOCTL},
					sizeof(trwifi_msg_s), (void *)&dmsg, (void *)&res};
	if (_send_msg(&msg) < 0) {
		return -1;
	}
	return res;
}
