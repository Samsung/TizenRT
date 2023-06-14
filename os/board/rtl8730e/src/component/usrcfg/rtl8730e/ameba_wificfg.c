#include "platform_autoconf.h"
#include <wifi_conf.h>
#if defined(CONFIG_AS_INIC_AP)
#include "inic_ipc_api.h"
#endif

struct wifi_user_conf wifi_user_config __attribute__((aligned(64)));

_WEAK void wifi_set_user_config(void)
{
#ifndef CONFIG_AS_INIC_NP

#ifdef CONFIG_AS_INIC_AP
	u32 param_buf[1];
#endif

	rtw_memset(&wifi_user_config, 0, sizeof(struct wifi_user_conf));

	/* below items for user config */
	wifi_user_config.concurrent_enabled = (u8)_TRUE; /*Softap's mac address will equal chip's mac address + 1 if this value set as _TRUE*/
	wifi_user_config.auto_reconnect_count = 8;
	wifi_user_config.auto_reconnect_interval = 5; /* in sec*/
#ifdef CONFIG_HIGH_TP_TEST /*enable high tp in make menuconfig*/
	wifi_user_config.skb_num_np = 14; /*In INIC mode for all traffics except tx data, In single core mode for all traffics*/
	wifi_user_config.skb_num_ap = 5; /*In INIC mode for tx data packtes, not used in single core mode*/
#else
	wifi_user_config.skb_num_np = 10;
	wifi_user_config.skb_num_ap = 4;
#endif
#ifdef CONFIG_SINGLE_CORE_WIFI
	wifi_user_config.skb_num_ap = 0;
#endif
	wifi_user_config.wifi_wpa_mode = WPA_AUTO_MODE;

	/*Regulatory related*/
	wifi_user_config.channel_plan = 0;
	wifi_user_config.country_code = RTW_COUNTRY_MAX;
	wifi_user_config.rtw_tx_pwr_lmt_enable = 2;	/* 0: disable, 1: enable, 2: Depend on efuse(flash) */
	wifi_user_config.rtw_tx_pwr_by_rate	= 2;	/* 0: disable, 1: enable, 2: Depend on efuse(flash) */
	wifi_user_config.rtw_trp_tis_cert_en = RTW_TRP_TIS_DISABLE;
	wifi_user_config.rtw_adaptivity_en = DISABLE;
	wifi_user_config.rtw_adaptivity_mode = 0; /* 0 : RTW_ADAPTIVITY_MODE_NORMAL,1: RTW_ADAPTIVITY_MODE_CARRIER_SENSE */
	wifi_user_config.rtw_adaptivity_th_l2h_ini = 0;

	/* power save */
	wifi_user_config.lps_dtim = 0;
	wifi_user_config.lps_enter_threshold = 0; /* LPS_THRESH_PKT_COUNT */
	wifi_user_config.rtw_power_mgnt = PS_MODE_MIN;
	wifi_user_config.rtw_lps_level = LPS_NORMAL;
	wifi_user_config.smart_ps = 2;

	/* Softap related */
	wifi_user_config.g_user_ap_sta_num = NUM_STA;
	wifi_user_config.bForwardingDisabled = 0;
	wifi_user_config.ap_polling_sta = 0; /*set to 1 if CONFIG_ENABLE_AP_POLLING_CLIENT_ALIVE is defined*/

	/* MISC */
	wifi_user_config.en_mcc = (u8) DISABLE; /*not support if CONFIG_MCC_MODE is undefined*/
	wifi_user_config.max_roaming_times = 2; /*not support if CONFIG_LAYER2_ROAMING is undefined*/
	wifi_user_config.ampdu_factor = 0; /* not support if CONFIG_80211AC_VHT is undefined*/
	wifi_user_config.bAcceptAddbaReq = (u8)_TRUE; /* 0:Reject AP's Add BA req, 1:Accept AP's Add BA req.*/
	wifi_user_config.bIssueAddbaReq = (u8)_TRUE;
	wifi_user_config.bCheckDestAddress = (u8)_TRUE;
	wifi_user_config.ap_compatibilty_enabled = 0x0B;

#ifdef CONFIG_AS_INIC_AP
	DCache_Clean((u32)(&wifi_user_config), sizeof(struct wifi_user_conf));
	param_buf[0] = (u32)(&wifi_user_config);
	inic_ipc_api_host_message_send(IPC_API_WIFI_SET_USR_CFG, param_buf, 1);
#endif

#endif
}

