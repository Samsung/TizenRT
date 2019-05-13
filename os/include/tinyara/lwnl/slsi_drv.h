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
#ifndef __INCLUDE_SLSIDRV_H__
#define __INCLUDE_SLSIDRV_H__

#include <tinyara/config.h>
#include <tinyara/lwnl/lwnl80211.h>

#include <stdint.h>
#include <stdbool.h>
#include <debug.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define SLSIDRV_TAG "[SLSIDRV]"

#define SLSIDRV_ERR                                                         \
	do {                                                                    \
		vddbg(SLSIDRV_TAG"[ERR] %s: %d line err(%s)\n",                     \
				  __FILE__, __LINE__, strerror(errno));                     \
	} while (0)

#define SLSIDRV_ENTER                                                       \
	do {                                                                    \
		vddbg(SLSIDRV_TAG"%s:%d\n", __FILE__, __LINE__);                    \
	} while (0)

/****************************************************************************
 * Public Types
 ****************************************************************************/
struct slsi_drv_dev_s {
	bool initialized;
	struct lwnl80211_lowerhalf_s dev;
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/
lwnl80211_result_e slsidrv_init(struct lwnl80211_lowerhalf_s *dev);
lwnl80211_result_e slsidrv_deinit(void);
lwnl80211_result_e slsidrv_scan_ap(void *arg);
lwnl80211_result_e slsidrv_connect_ap(lwnl80211_ap_config_s *ap_connect_config, void *arg);
lwnl80211_result_e slsidrv_disconnect_ap(void *arg);
lwnl80211_result_e slsidrv_get_info(lwnl80211_info *wifi_info);
lwnl80211_result_e slsidrv_start_softap(lwnl80211_softap_config_s *softap_config);
lwnl80211_result_e slsidrv_start_sta(void);
lwnl80211_result_e slsidrv_stop_softap(void);
lwnl80211_result_e slsidrv_set_autoconnect(uint8_t check);

/* Registrations */
struct lwnl80211_lowerhalf_s *slsi_drv_initialize(void);

#endif /*  __INCLUDE_SLSIDRV_H__ */
