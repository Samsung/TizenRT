/****************************************************************************
 *
 * Copyright (c) 2016 Samsung Electronics Co., Ltd. All rights reserved
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
