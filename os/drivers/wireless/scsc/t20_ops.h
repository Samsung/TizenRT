/*****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#ifndef __T20_OPS_H__
#define __T20_OPS_H__

#include "dev.h"
#include "wpa_driver_copy.h"

#define SDEV_FROM_WIPHY(wps) ((struct slsi_dev *)(wps)->drv_priv)

#define WLAN_CIPHER_SUITE_PMK 0x00904C00

#define SLSI_WPS_REQUEST_TYPE_POS               15
#define SLSI_WPS_REQUEST_TYPE_ENROLEE_INFO_ONLY 0x00
#define SLSI_WPS_OUI_PATTERN                    0x04F25000
#define SLSI_P2P_OUI_PATTERN                    0x099a6f50
#define SLSI_VENDOR_OUI_AND_TYPE_LEN            4

struct slsi_t20_drv {
	void *global;
	void *ctx;
	struct slsi_dev *sdev;
	char ifname[IFNAMSIZ + 1];
	struct wpa_driver_capa capa;
};

struct slsi_dev *slsi_new(void);
int slsi_t20_80211_register(struct slsi_dev *sdev);
void slsi_t20_80211_unregister(struct slsi_dev *sdev);
void *slsi_t20_init(void *ctx, const char *ifname, void *global_priv);
int slsi_t20_set_param(void *priv, const char *param);
int slsi_t20_get_capa(void *priv, struct wpa_driver_capa *capa);
#endif /*__T20_OPS_H__*/
