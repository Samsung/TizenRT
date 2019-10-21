/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#ifndef __INCLUDE_RTKDRV_H__
#define __INCLUDE_RTKDRV_H__

#include <tinyara/config.h>
#include <tinyara/lwnl/lwnl80211.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <debug.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define RTKDRV_TAG "[RTKDRV]"

#define RTKDRV_ERR                                      \
	do {                                                \
		vddbg(RTKDRV_TAG "[ERR] %s: %d line err(%s)\n", \
			  __FILE__, __LINE__, strerror(errno));     \
	} while (0)

#define RTKDRV_ENTER                                     \
	do {                                                 \
		vddbg(RTKDRV_TAG "%s:%d\n", __FILE__, __LINE__); \
	} while (0)

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
lwnl80211_result_e rtkdrv_init(struct lwnl80211_lowerhalf_s *dev);
lwnl80211_result_e rtkdrv_deinit(void);
lwnl80211_result_e rtkdrv_scan_ap(lwnl80211_ap_config_s *config);
lwnl80211_result_e rtkdrv_connect_ap(lwnl80211_ap_config_s *ap_connect_config, void *arg);
lwnl80211_result_e rtkdrv_disconnect_ap(void *arg);
lwnl80211_result_e rtkdrv_get_info(lwnl80211_info *wifi_info);
lwnl80211_result_e rtkdrv_start_softap(lwnl80211_softap_config_s *softap_config);
lwnl80211_result_e rtkdrv_start_sta(void);
lwnl80211_result_e rtkdrv_stop_softap(void);
lwnl80211_result_e rtkdrv_set_autoconnect(uint8_t check);

/* Registrations */
struct lwnl80211_lowerhalf_s *rtk_drv_initialize(void);

#endif /*  __INCLUDE_RTKDRV_H__ */
