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
#ifndef __DRIVERS_WIFI_CSI_RTK8730E
#define __DRIVERS_WIFI_CSI_RTK8730E

#include <tinyara/wifi_csi/wifi_csi.h>
#include "wifi_intf_drv_to_app_basic.h"
#include <semaphore.h>

struct wifi_csi_dev_s {
	/* common parts */
	struct wifi_csi_lowerhalf_s dev; /* rtk wifi_csi lower half (this device) */

	sem_t devsem;			/* Protection for both pendq & dev */

	/* rtk_wifi_csi specific data */
	rtw_csi_action_parm_t act_param;
	unsigned char rtk_csi_buf[MAX_CSI_BUFF_LEN];
};

#endif				/* __DRIVERS_WIFI_CSI_RTK8730E */

