/**
 ****************************************************************************************
 *
 * @file param_config.c
 *
 *
 * Copyright (C) Beken Corp 2011-2016
 *
 ****************************************************************************************
 */
#include <common/bk_include.h>
#include "bk_wifi_types.h"
#include "bk_phy.h"
#include "bk_wifi.h"
#include "bk_wifi_private.h"
#include "../wpa_supplicant-2.10/hostapd/hostapd_cfg.h"
#include "wifi_config.h"
#include "os/mem.h"


general_param_t *g_wlan_general_param = NULL;
ap_param_t *g_ap_param_ptr = NULL;
sta_param_t *g_sta_param_ptr = NULL;
wifi_mac_config_t g_wifi_mac_config = {
	WIFI_CFG_DFT_RAW_TX_AC,
	WIFI_CFG_DFT_RAW_TX_AC_TIMEOUT_MS,
	WIFI_CFG_DFT_TX_MSDU_MAX_CNT,
	WIFI_CFG_DFT_MIN_RSV_MEM,
	false,
	WIFI_CFG_DFT_MEDIA_QUALITY,
	WIFI_CFG_MGMT_RTY_NUM,
	WIFI_CFG_DATA_RTY_NUM,
	false,
	false,
};

//TODO config it via menuconfig
//Same as NX_VIRT_DEV_MAX which is defined in rwnx_config.h
#define CFG_MAX_DEV 2

static bool cfg_is_all_zero_mac(uint8_t *mac)
{
	return ((mac[0] == 0) && (mac[1] == 0) && (mac[2] == 0)
		&& (mac[3] == 0) && (mac[4] == 0) && (mac[5] == 0));
}

uint32_t cfg_ap_is_open_system(void)
{
	uint32_t no_passcode_flag = 0;

	if ((NULL != g_ap_param_ptr)
		&& (!cfg_is_all_zero_mac((u8 *)&g_ap_param_ptr->bssid))
		&& (BK_SECURITY_TYPE_NONE == g_ap_param_ptr->cipher_suite))
		no_passcode_flag = 1;

	return no_passcode_flag;
}

uint32_t cfg_param_init(void)
{
	if (NULL == g_wlan_general_param) {
		g_wlan_general_param = (general_param_t *)os_zalloc(sizeof(general_param_t));
		BK_ASSERT(g_wlan_general_param); /* ASSERT VERIFIED */
	}

	if (NULL == g_ap_param_ptr) {
		g_ap_param_ptr = (ap_param_t *)os_zalloc(sizeof(ap_param_t));
		BK_ASSERT(g_ap_param_ptr); /* ASSERT VERIFIED */
	}

#if COFIG_AP_STATYPE_LIMIT
	if (wifi_bk_feature_ap_statype_limit_enable())
		g_ap_param_ptr->max_statype_num[0] = CFG_SUPPORTED_MAX_STA_NUM;
#endif

	if (NULL == g_sta_param_ptr) {
		g_sta_param_ptr = (sta_param_t *)os_zalloc(sizeof(sta_param_t));
		BK_ASSERT(g_sta_param_ptr); /* ASSERT VERIFIED */
#if CONFIG_STA_VSIE
	if (wifi_bk_feature_sta_vsie_enable()) {
		for (int i = 0; i < NUM_WIFI_VENDOR_ELEM_FRAMES; i++) {
			g_sta_param_ptr->vsies[i] = (struct bk_vise *)os_zalloc(sizeof(struct bk_vise));
			BK_ASSERT(g_sta_param_ptr->vsies[i]);
		}
	}

#endif
	}

	return 0;
}

