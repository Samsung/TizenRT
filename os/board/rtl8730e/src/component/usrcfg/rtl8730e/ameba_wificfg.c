/*
* Copyright (c) 2024 Realtek Semiconductor Corp.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifdef CONFIG_FULLMAC
#include <rtw_cfg80211_fullmac.h>
#else
#include "platform_autoconf.h"
#include <wifi_conf.h>
#endif

struct wifi_user_conf wifi_user_config __attribute__((aligned(64)));

_WEAK void wifi_set_user_config(void)
{
	memset(&wifi_user_config, 0, sizeof(struct wifi_user_conf));

	/* below items for user config, for details, see wifi_user_conf in wifi_intf_drv_to_app_basic.h */
#ifdef CONFIG_ENABLE_HOMELYNK
	wifi_user_config.concurrent_enabled = (u8)_TRUE; /*Softap's mac address will equal chip's mac address + 1 if this value set as _TRUE*/
#else
	wifi_user_config.concurrent_enabled = (u8)_FALSE;
#endif //#ifdef CONFIG_ENABLE_HOMELYNK
	wifi_user_config.softap_addr_offset_idx = 1;
	wifi_user_config.auto_reconnect_count = 10;
	wifi_user_config.auto_reconnect_interval = 5;
	wifi_user_config.no_beacon_disconnect_time = 9; /* unit 2s, default 18s */
#ifdef CONFIG_HIGH_TP_TEST /*enable high tp in make menuconfig*/
	wifi_user_config.skb_num_np = 22;
	wifi_user_config.skb_num_ap = 8;
#else
#ifdef CONFIG_FULLMAC
	wifi_user_config.skb_num_ap = 10;
#else
	wifi_user_config.skb_num_ap = 8;	/*adjust to 8 for ping 10k*/
#endif
	wifi_user_config.skb_num_np = 22;  /*4 for rx_ring_buffer + 16 for rx_ampdu + 2 for mgnt trx*/
#endif
	wifi_user_config.rx_ampdu_num = 16;

#ifdef CONFIG_SINGLE_CORE_WIFI
	wifi_user_config.skb_num_ap = 0;
#endif
	wifi_user_config.skb_buf_size = 0;
	wifi_user_config.wifi_wpa_mode_force = WPA_AUTO_MODE;

	/*Regulatory related*/
	wifi_user_config.country_code[0] = 0;
	wifi_user_config.country_code[1] = 0;
	wifi_user_config.rtw_tx_pwr_lmt_enable = 0;
	wifi_user_config.rtw_tx_pwr_by_rate	= 2;
	wifi_user_config.rtw_802_11d_en = 0;
	wifi_user_config.rtw_trp_tis_cert_en = RTW_TRP_TIS_DISABLE;
	wifi_user_config.rtw_edcca_mode = RTW_EDCCA_NORM;
	wifi_user_config.tdma_dig_enable = 0; /*add for customer ctrl tdma dig on/off*/
	wifi_user_config.antdiv_mode = RTW_ANTDIV_DISABLE;
	wifi_user_config.probe_hidden_ap_on_passive_ch = 0; /* 0: No probe request will be sent on passive channel */


	/* IPS(Inactive Power Save), power save when wifi unconnected */
	wifi_user_config.ips_enable = 1;
	wifi_user_config.ips_level = IPS_WIFI_OFF;

	/* LPS(Leisure Power Save), power save when wifi connected, has 2 mode - legacy power save and uapsd power save*/
	wifi_user_config.lps_enable = 1;
	wifi_user_config.lps_mode = PS_MODE_LEGACY;
	wifi_user_config.legacy_ps_listen_interval = 0;

	/*wmm ps, when wifi_user_config.lps_enable = 1 and wifi_user_config.lps_mode = PS_MODE_UAPSD_WMM */
	wifi_user_config.uapsd_max_sp_len = 0;
	wifi_user_config.uapsd_ac_enable = 0;

	/* Softap related */
	wifi_user_config.ap_sta_num = 12;	/*should not exceed AP_STA_NUM */
	wifi_user_config.ap_polling_sta = 0;

	/* MISC */
	wifi_user_config.en_mcc = 0; /*not support if CONFIG_MCC_MODE is undefined*/
	wifi_user_config.max_roaming_times = 2;
	wifi_user_config.ampdu_rx_enable = 1; /* not support if CONFIG_80211AC_VHT is undefined*/
	wifi_user_config.ampdu_tx_enable = 1; /* not support if CONFIG_80211AC_VHT is undefined*/
	wifi_user_config.bCheckDestAddress = 1;
	wifi_user_config.ap_compatibilty_enabled = 0x0B;
	wifi_user_config.set_channel_api_do_rfk = 1;
	wifi_user_config.rf_calibration_disable = 0;
	wifi_user_config.tx_shortcut_enable = 1;
	wifi_user_config.rx_shortcut_enable = 1;
	wifi_user_config.keepalive_interval = 20;

#ifdef CONFIG_FULLMAC
	/* Linux wifi supports cfg80211 ops. */
	wifi_user_config.cfg80211 = 1;
#endif

	/* WPS */
	wifi_user_config.wps_retry_count = 4;
	wifi_user_config.wps_retry_interval = 5000;

	/* wifi speaker */
	wifi_user_config.wifi_speaker_feature = 0;

	/*Automatic channel selection*/
	wifi_user_config.acs_en = 0;

#ifdef CONFIG_PLATFORM_TIZENRT_OS
	/* SoftAP silent table to reduce probe response when receiving probe request continuously */
	wifi_user_config.softap_keep_silent_table_enable = 1;
	wifi_user_config.softap_keep_silent_table_interval = 500; /* ms. Once interval period is reached, remove from silent table, so can send probe response to same STA again. */
#endif //CONFIG_PLATFORM_TIZENRT_OS
}

