/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

/******************************************************************************
 * history *

******************************************************************************/
#include "inic_ipc_api.h"
#include "wifi_conf.h"
#include "wifi_conf_inter.h"

#include "wlan_intf.h"
#include "inic_ipc_cfg.h"

/* -------------------------------- Defines --------------------------------- */
#define CONFIG_INIC_IPC_DEV_API_PRIO 3
#ifdef CONFIG_PHYDM_CMD/*halbb debug cmd need bigger stack size*/
#define INIC_IPC_API_STACK 1024
#else
#define INIC_IPC_API_STACK 512
#endif
typedef void (*inic_ipc_promisc_callback_t)(\
		unsigned char *buf, unsigned int len, void *user_data);

/* -------------------------------- Macros ---------------------------------- */

/* ---------------------------- Global Variables ---------------------------- */
_sema g_dev_inic_api_task_wake_sema = NULL;
_sema g_dev_inic_api_message_send_sema = NULL;

xTaskHandle api_dev_task = NULL;

//SRAM_BD_DATA_SECTION
inic_ipc_dev_request_message g_dev_ipc_api_request_info __attribute__((aligned(64)));
#ifdef IPC_DIR_MSG_TX
IPC_MSG_STRUCT g_dev_ipc_api_msg __attribute__((aligned(64)));
#endif

#if CONFIG_AUTO_RECONNECT
extern p_wlan_autoreconnect_hdl_t p_wlan_autoreconnect_hdl;
#endif
extern u8 inic_ipc_ip_addr[4];
#ifdef CONFIG_MP_INCLUDED
extern int wext_private_command(unsigned char wlan_idx, char *cmd, int show_msg);
#endif
/* ---------------------------- Private Functions --------------------------- */


/* ---------------------------- Public Functions ---------------------------- */
/**
 * @brief  haddle the message of IPC.
 * @param  none.
 * @return none.
 */
void inic_ipc_api_dev_task(void)
{
	int ret = -1;
	inic_ipc_host_request_message *p_ipc_msg = NULL;

	do {
		rtw_down_sema(&g_dev_inic_api_task_wake_sema);
#ifdef IPC_DIR_MSG_RX
		PIPC_MSG_STRUCT p_ipc_recv_msg = ipc_get_message(IPC_DIR_MSG_RX, \
										 IPC_H2D_WIFI_API_TRAN);
		p_ipc_msg = (inic_ipc_host_request_message *)p_ipc_recv_msg->msg;
#else
		p_ipc_msg = (inic_ipc_host_request_message *)ipc_get_message(IPC_INT_CHAN_WIFI_API_TRAN);
#endif /* IPC_DIR_MSG_RX */

		DCache_Invalidate((u32)p_ipc_msg, sizeof(inic_ipc_host_request_message));
		if (p_ipc_msg == NULL) {
			DBG_8195A("Device IPC message is NULL, invalid!\n\r");
			continue;
		}

		switch (p_ipc_msg->API_ID) {
		case IPC_API_WIFI_CONNECT: {
			rtw_network_info_t *pWifi = (rtw_network_info_t *)p_ipc_msg->param_buf[0];
			DCache_Invalidate((u32)pWifi, sizeof(rtw_network_info_t));
			if (pWifi->password_len) {
				DCache_Invalidate((u32)pWifi->password, pWifi->password_len);
			}
			ret = rtw_wx_connect_local(pWifi);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_DISCONNECT: {
			ret = wifi_disconnect();
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_IS_CONNECTED_TO_AP: {
			ret = wifi_is_connected_to_ap();
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_IS_RUNNING: {
			unsigned char wlan_idx = (unsigned char)p_ipc_msg->param_buf[0];
			ret = wifi_is_running(wlan_idx);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_CHANNEL: {
			int channel = (int)p_ipc_msg->param_buf[0];
			/* Set INIC STA Channel. */
			ret = wifi_set_freq(STA_WLAN_INDEX, channel);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_GET_CHANNEL: {
			int *channel = (int *)p_ipc_msg->param_buf[0];
			DCache_Invalidate((u32)channel, sizeof(int));
			/* Get INIC STA Channel. */
			ret = wifi_get_freq(STA_WLAN_INDEX, (u8 *)channel);
			DCache_Clean((u32)channel, sizeof(int));
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_USR_CFG: {
			struct wifi_user_conf *usr_conf = (struct wifi_user_conf *)p_ipc_msg->param_buf[0];
			DCache_Invalidate((u32)usr_conf, sizeof(struct wifi_user_conf));
			rtw_memcpy(&wifi_user_config, usr_conf, sizeof(struct wifi_user_conf));
			p_ipc_msg->ret = 0;
			break;
		}
		case IPC_API_WIFI_GET_DISCONN_REASCON: {
			unsigned short *reason_code = (unsigned short *)p_ipc_msg->param_buf[0];
			DCache_Invalidate((u32)reason_code, sizeof(unsigned short));
			ret = wifi_get_disconn_reason_code(reason_code);
			DCache_Clean((u32)reason_code, sizeof(unsigned short));
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_ON: {
			rtw_mode_t mode = (rtw_mode_t)p_ipc_msg->param_buf[0];
			ret = wifi_on(mode);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_OFF: {
			ret = wifi_off();
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_MODE: {
			rtw_mode_t mode = (rtw_mode_t)p_ipc_msg->param_buf[0];
			ret = wifi_set_mode(mode);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_START_AP: {
			rtw_softap_info_t *softAP_config = (rtw_softap_info_t *)p_ipc_msg->param_buf[0];
			DCache_Invalidate((u32)softAP_config->password, softAP_config->password_len);

			ret = wifi_start_ap(softAP_config);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_STOP_AP: {
			ret = wifi_stop_ap();
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SCAN_NETWROKS: {
			rtw_scan_param_t *scan_param = (rtw_scan_param_t *)p_ipc_msg->param_buf[0];
			DCache_Invalidate((u32)scan_param, sizeof(rtw_scan_param_t));
			u8 block = (u8)p_ipc_msg->param_buf[1];
			u8 ssid_len = (u8)p_ipc_msg->param_buf[2];
			if (scan_param->ssid) {
				DCache_Invalidate((u32)scan_param->ssid, ssid_len);
			}
			if (scan_param->channel_list) {
				DCache_Invalidate((u32)scan_param->channel_list, scan_param->channel_list_num);
			}
			ret = wifi_scan_networks(scan_param, block);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SCAN_ABORT: {
			ret = wifi_scan_abort();
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_GET_SCANNED_AP_INFO: {
			unsigned int *AP_num = (unsigned int *)p_ipc_msg->param_buf[0];
			char *scan_buf = (char *)p_ipc_msg->param_buf[1];
			DCache_Invalidate((u32)AP_num, sizeof(unsigned int));
			DCache_Invalidate((u32)scan_buf, (*AP_num)*sizeof(rtw_scan_result_t));
			ret = wifi_get_scan_records(AP_num, scan_buf);
			DCache_Clean((u32)AP_num, sizeof(unsigned int));
			DCache_Clean((u32)scan_buf, (*AP_num)*sizeof(rtw_scan_result_t));
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_PSK_INFO_SET: {
			struct psk_info *PSK_INFO = (struct psk_info *)p_ipc_msg->param_buf[0];
			DCache_Invalidate((u32)PSK_INFO, sizeof(struct psk_info));
			DCache_Invalidate((u32)PSK_INFO->psk_essid, (32 + 4));
			DCache_Invalidate((u32)PSK_INFO->psk_passphrase, (64 + 1));
			DCache_Invalidate((u32)PSK_INFO->wpa_global_PSK, 32);
			wifi_psk_info_set(PSK_INFO);
			break;
		}
		case IPC_API_WIFI_PSK_INFO_GET: {
			struct psk_info *PSK_INFO = (struct psk_info *)p_ipc_msg->param_buf[0];
			DCache_Invalidate((u32)PSK_INFO, sizeof(struct psk_info));

			wifi_psk_info_get(PSK_INFO);
			DCache_Clean((u32)PSK_INFO, sizeof(struct psk_info));
			DCache_Clean((u32)PSK_INFO->psk_essid, (32 + 4));
			DCache_Clean((u32)PSK_INFO->psk_passphrase, (64 + 1));
			DCache_Clean((u32)PSK_INFO->wpa_global_PSK, (20 * 2));
			break;
		}
		case IPC_API_WIFI_GET_MAC_ADDR: {
			rtw_mac_t *mac = (rtw_mac_t *)p_ipc_msg->param_buf[0];
			DCache_Invalidate((u32)mac, sizeof(rtw_mac_t));
			ret = wifi_get_mac_address(mac);
			DCache_Clean((u32)mac, sizeof(rtw_mac_t));
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_COEX_BLE_SET_SCAN_DUTY: {
			u8 duty = (u8)p_ipc_msg->param_buf[0];
			ret = wifi_btcoex_set_ble_scan_duty(duty);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_COEX_SET_PTA: {
			pta_type_t gntbt_id = p_ipc_msg->param_buf[0];
			wifi_btcoex_set_pta(gntbt_id);
			break;
		}
		case IPC_API_WIFI_COEX_BT_RFK: {
			struct bt_rfk_param rfk_param;
			rfk_param.type = p_ipc_msg->param_buf[0];
			rfk_param.rfk_data1 = p_ipc_msg->param_buf[1];
			rfk_param.rfk_data2 = p_ipc_msg->param_buf[2];
			rfk_param.rfk_data3 = p_ipc_msg->param_buf[3];
			rfk_param.rfk_data4 = p_ipc_msg->param_buf[4];

			ret = wifi_btcoex_bt_rfk(&rfk_param);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_WPA_MODE: {
			rtw_wpa_mode wpa_mode = p_ipc_msg->param_buf[0];
			ret = wifi_set_wpa_mode(wpa_mode);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_PMF_MODE: {
			u8 pmf_mode = p_ipc_msg->param_buf[0];
			ret = wifi_set_pmf_mode(pmf_mode);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_DRIVE_IS_MP: {
			ret = wifi_driver_is_mp();
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_GET_ASSOCIATED_CLIENT_LIST: {
			void *client_list_buffer = (rtw_maclist_t *)p_ipc_msg->param_buf[0];
			unsigned short buffer_len = (unsigned short)p_ipc_msg->param_buf[1];
			DCache_Invalidate((u32)client_list_buffer, buffer_len);

			ret = wifi_get_associated_client_list(client_list_buffer, buffer_len);
			DCache_Clean((u32)client_list_buffer, buffer_len);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_GET_SETTING: {
			unsigned char wlan_idx = (unsigned char)p_ipc_msg->param_buf[0];
			rtw_wifi_setting_t *ap_info = (rtw_wifi_setting_t *)p_ipc_msg->param_buf[1];
			DCache_Invalidate((u32)ap_info, sizeof(rtw_wifi_setting_t));

			ret = wifi_get_setting(wlan_idx, ap_info);
			DCache_Clean((u32)ap_info, sizeof(rtw_wifi_setting_t));
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_IPS_EN: {
			u8 ips_en = (u8)p_ipc_msg->param_buf[0];
			ret = wifi_set_ips_enable(ips_en);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_LPS_EN: {
			u8 lps_en = (u8)p_ipc_msg->param_buf[0];
			ret = wifi_set_lps_enable(lps_en);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_MFP_SUPPORT: {
			unsigned char value = (unsigned char)p_ipc_msg->param_buf[0];
			ret = wifi_set_mfp_support(value);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_GROUP_ID: {
			unsigned char value = (unsigned char)p_ipc_msg->param_buf[0];
			ret = wifi_set_group_id(value);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_PMK_CACHE_EN: {
			unsigned char value = (unsigned char)p_ipc_msg->param_buf[0];
			ret = wifi_set_pmk_cache_enable(value);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_GET_SW_STATISTIC: {
			unsigned char wlan_idx = (unsigned char)p_ipc_msg->param_buf[0];
			rtw_sw_statistics_t *statistic = (rtw_sw_statistics_t *)p_ipc_msg->param_buf[1];
			ret = wifi_get_sw_statistic(wlan_idx, statistic);
			DCache_Clean((u32)statistic, sizeof(rtw_sw_statistics_t));
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_GET_PHY_STATISTIC: {
			rtw_phy_statistics_t *statistic = (rtw_phy_statistics_t *)p_ipc_msg->param_buf[0];
			unsigned char wlan_idx = p_ipc_msg->param_buf[1];
			ret = wifi_fetch_phy_statistic(wlan_idx, statistic);
			DCache_Clean((u32)statistic, sizeof(rtw_phy_statistics_t));
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_NETWORK_MODE: {
			rtw_network_mode_t mode = (rtw_network_mode_t)p_ipc_msg->param_buf[0];
			ret = wifi_set_network_mode(mode);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_GET_NETWORK_MODE: {
			ret = wifi_get_network_mode();
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_WPS_PHASE: {
			unsigned char is_trigger_wps = (unsigned char)p_ipc_msg->param_buf[0];
			ret = wifi_set_wps_phase(is_trigger_wps);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_GEN_IE: {
			unsigned char wlan_idx = (unsigned char)p_ipc_msg->param_buf[0];
			char *buf = (char *)p_ipc_msg->param_buf[1];
			u16 buf_len = (u16)p_ipc_msg->param_buf[2];
			u16 flags = (u16)p_ipc_msg->param_buf[3];
			DCache_Invalidate((u32)buf, buf_len);
			ret = wifi_set_gen_ie(wlan_idx, buf, buf_len, flags);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_EAP_PHASE: {
			unsigned char is_trigger_eap = (unsigned char)p_ipc_msg->param_buf[0];
			ret = wifi_set_eap_phase(is_trigger_eap);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_GET_EAP_PHASE: {
			ret = wifi_get_eap_phase();
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_EAP_METHOD: {
			unsigned char eap_method = (unsigned char)p_ipc_msg->param_buf[0];
			ret = wifi_set_eap_method(eap_method);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SEND_EAPOL: {
			unsigned char wlan_idx = (unsigned char)p_ipc_msg->param_buf[0];
			char *buf = (char *)p_ipc_msg->param_buf[1];
			u16 buf_len = (u16)p_ipc_msg->param_buf[2];
			u16 flags = (u16)p_ipc_msg->param_buf[3];
			DCache_Invalidate((u32)buf, buf_len);
			ret = wifi_send_eapol(wlan_idx, buf, buf_len, flags);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_CONFIG_AUTORECONNECT: {
			u8 mode = (u8)p_ipc_msg->param_buf[0];
			ret = wifi_config_autoreconnect(mode);
			if (mode != RTW_AUTORECONNECT_DISABLE) {
				p_wlan_autoreconnect_hdl = (p_wlan_autoreconnect_hdl_t)0xFFFFFFFF;
			}
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_GET_AUTORECONNECT: {
			u8 *mode = (u8 *)p_ipc_msg->param_buf[0];
			ret = wifi_get_autoreconnect(mode);
			DCache_Clean((u32)mode, sizeof(u8));
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_CUS_IE: {
			u8 type = (u8)p_ipc_msg->param_buf[0];
			if (type == 2) {
				unsigned char wlan_idx = p_ipc_msg->param_buf[1];
				ret = wifi_del_custom_ie(wlan_idx);
			} else if (type == 1) {
				rtw_custom_ie_t *cus_ie = (rtw_custom_ie_t *)p_ipc_msg->param_buf[1];
				int ie_index = (int)p_ipc_msg->param_buf[2];
				DCache_Invalidate((u32)cus_ie, sizeof(rtw_custom_ie_t));
				DCache_Invalidate((u32)(cus_ie->ie), (cus_ie->ie[1] + 2));
				ret = wifi_update_custom_ie(cus_ie, ie_index);
			} else if (type == 0) {
				int cnt = 0;
				u8 ie_len = 0;
				void *cus_ie = (void *)p_ipc_msg->param_buf[1];
				int ie_num = (int)p_ipc_msg->param_buf[2];
				DCache_Invalidate((u32)cus_ie, ie_num * sizeof(rtw_custom_ie_t));
				p_rtw_custom_ie_t pcus_ie = cus_ie;
				for (cnt = 0; cnt < ie_num; cnt++) {
					rtw_custom_ie_t ie_t = *(pcus_ie + cnt);
					ie_len = ie_t.ie[1];
					DCache_Invalidate((u32)ie_t.ie, (u32)(ie_len + 2));
				}
				ret = wifi_add_custom_ie(cus_ie, ie_num);
			}
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_IND_MGNT: {
			int enable = (int)p_ipc_msg->param_buf[0];
			wifi_set_indicate_mgnt(enable);
			break;
		}
		case IPC_API_WIFI_SEND_MGNT: {
			raw_data_desc_t *raw_data_desc = (raw_data_desc_t *)p_ipc_msg->param_buf[0];
			DCache_Invalidate((u32)raw_data_desc, sizeof(raw_data_desc_t));
			DCache_Invalidate((u32)raw_data_desc->buf, raw_data_desc->buf_len);
			ret = wifi_send_raw_frame(raw_data_desc);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_TXRATE_BY_TOS: {
			unsigned char enable = (unsigned char)p_ipc_msg->param_buf[0];
			unsigned char ToS_precedence = (unsigned char)p_ipc_msg->param_buf[1];
			unsigned char tx_rate = (unsigned char)p_ipc_msg->param_buf[2];
			ret = wifi_set_tx_rate_by_ToS(enable, ToS_precedence, tx_rate);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_EDCA_PARAM: {
			unsigned int ac_param = p_ipc_msg->param_buf[0];
			ret = wifi_set_EDCA_param(ac_param);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_TX_CCA: {
			unsigned char enable = (unsigned char)p_ipc_msg->param_buf[0];
			ret = wifi_set_TX_CCA(enable);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_CTS2SEFL_DUR_AND_SEND: {
			unsigned char wlan_idx = (unsigned char)p_ipc_msg->param_buf[0];
			unsigned short duration = (unsigned short)p_ipc_msg->param_buf[1];
			ret = wifi_set_cts2self_duration_and_send(wlan_idx, duration);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_MAC_FILTER: {
			u32 type = p_ipc_msg->param_buf[0];
			if (type == 0) {
				ret = wifi_init_mac_filter();
			} else if (type == 1) {
				unsigned char *hwaddr = (unsigned char *)p_ipc_msg->param_buf[0];
				DCache_Invalidate((u32)hwaddr, ETH_ALEN);
				ret = wifi_add_mac_filter(hwaddr);
			} else if (type == 2) {
				unsigned char *hwaddr = (unsigned char *)p_ipc_msg->param_buf[0];
				DCache_Invalidate((u32)hwaddr, ETH_ALEN);
				ret = wifi_del_mac_filter(hwaddr);
			}
			p_ipc_msg->ret = ret;
			break;
		}
#ifdef CONFIG_ANTENNA_DIVERSITY
		case IPC_API_WIFI_GET_ANTENNA_INFO: {
			unsigned char *antenna = (unsigned char *)p_ipc_msg->param_buf[0];
			DCache_Invalidate((u32)antenna, sizeof(unsigned char));
			ret = wifi_get_antenna_info(antenna);
			DCache_Clean((u32)antenna, sizeof(unsigned char));
			p_ipc_msg->ret = ret;
			break;
		}
#endif
		case IPC_API_WIFI_GET_BAND_TYPE : {
			ret = wifi_get_band_type();
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_PROMISC_FILTER_RETRANSMIT_PKT: {
			u8 enable = (u8)p_ipc_msg->param_buf[0];
			u8 filter_interval_ms = (u8)p_ipc_msg->param_buf[1];
			ret = promisc_filter_retransmit_pkt(enable, filter_interval_ms);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_PROMISC_SET: {
			rtw_rcr_level_t enabled = (rtw_rcr_level_t)p_ipc_msg->param_buf[0];
			inic_ipc_promisc_callback_t callback = (inic_ipc_promisc_callback_t)p_ipc_msg->param_buf[1];
			unsigned char len_used = (unsigned char)p_ipc_msg->param_buf[2];
			ret = promisc_set(enabled, callback, len_used);

			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_PROMISC_IS_ENABLED: {
			ret = is_promisc_enabled();
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_PROMISC_GET_FIXED_CHANNEL: {
			void *fixed_bssid = (void *)p_ipc_msg->param_buf[0];
			u8 *ssid = (u8 *)p_ipc_msg->param_buf[1];
			int *ssid_length = (int *)p_ipc_msg->param_buf[2];
			DCache_Invalidate((u32)fixed_bssid, ETH_ALEN);
			DCache_Invalidate((u32)ssid, 33);
			ret = promisc_get_fixed_channel(fixed_bssid, ssid, ssid_length);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_PROMISC_ISSUE_PROBERSP: {
			unsigned char *da = (unsigned char *)p_ipc_msg->param_buf[0];
			DCache_Invalidate((u32)da, ETH_ALEN);
			promisc_issue_probersp(da);
			break;
		}
		case IPC_API_WIFI_GET_AUTO_CHANNEL: {
			unsigned char wlan_idx = (unsigned char)p_ipc_msg->param_buf[0];
			unsigned char *channel_set = (unsigned char *)p_ipc_msg->param_buf[1];
			unsigned char channel_num = (unsigned char)p_ipc_msg->param_buf[2];
			DCache_Invalidate((u32)channel_set, (u32)channel_num);
			ret = wifi_get_auto_chl(wlan_idx, channel_set, channel_num);
			DCache_Clean((u32)channel_set, (u32)channel_num);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_DEL_STA: {
			unsigned char wlan_idx = (unsigned char)p_ipc_msg->param_buf[0];
			unsigned char *hwaddr = (unsigned char *)p_ipc_msg->param_buf[1];
			DCache_Invalidate((u32)hwaddr, ETH_ALEN);
			ret = wifi_del_station(wlan_idx, hwaddr);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_AP_CH_SWITCH: {
			unsigned char new_ch = (unsigned char)p_ipc_msg->param_buf[0];
			unsigned char chl_switch_cnt = (unsigned char)p_ipc_msg->param_buf[1];
			ap_channel_switch_callback_t callback = (ap_channel_switch_callback_t)p_ipc_msg->param_buf[2];
			ret = wifi_ap_switch_chl_and_inform(new_ch, chl_switch_cnt, callback);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_NO_BEACON_TIMEOUT: {
			unsigned char timeout = (unsigned char)p_ipc_msg->param_buf[0];
			wifi_set_no_beacon_timeout(timeout);
			break;
		}
		case IPC_API_PROMISC_SET_MGNTFRAME: {
			u8 enable = (u8)p_ipc_msg->param_buf[0];
			ret = promisc_set_mgntframe(enable);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_PROMISC_FILTER_BY_AP_AND_PHONE_MAC: {
			u8 enable = (u8)p_ipc_msg->param_buf[0];
			u8 *ap_mac = (u8 *)p_ipc_msg->param_buf[1];
			u8 *phone_mac = (u8 *)p_ipc_msg->param_buf[2];
			if (ap_mac) {
				DCache_Invalidate((u32)ap_mac, ETH_ALEN);
			}
			if (phone_mac) {
				DCache_Invalidate((u32)phone_mac, ETH_ALEN);
			}
			promisc_filter_by_ap_and_phone_mac(enable, ap_mac, phone_mac);
			break;
		}
		case IPC_API_PROMISC_TX_BEACON_CONTROL: {
			u32 control = p_ipc_msg->param_buf[0];
			if (control == 1) {
				promisc_stop_tx_beacn();
			} else if (control == 2) {
				promisc_resume_tx_beacn();
			}
			break;
		}
		case IPC_API_PROMISC_INIT_PACKET_FILTER: {
			wifi_init_packet_filter();
			break;
		}
		case IPC_API_PROMISC_ADD_PACKET_FILTER: {
			u8 filter_id = (u8)p_ipc_msg->param_buf[0];
			rtw_packet_filter_pattern_t *patt = (rtw_packet_filter_pattern_t *)p_ipc_msg->param_buf[1];
			rtw_packet_filter_rule_t rule = (rtw_packet_filter_rule_t)p_ipc_msg->param_buf[2];

			if (patt) {
				DCache_Invalidate((u32)patt, sizeof(rtw_packet_filter_pattern_t));

				if (patt->mask) {
					DCache_Invalidate((u32)patt->mask, (u32)patt->mask_size);
				}

				if (patt->pattern) {
					DCache_Invalidate((u32)patt->pattern, (u32)patt->mask_size);
				}

				if ((patt->mask != NULL) && (patt->pattern != NULL)) {
					ret = wifi_add_packet_filter(filter_id, patt, rule);
				}

			}

			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_PROMISC_PACKET_FILTER_CONTROL: {
			u8 filter_id = (u8)p_ipc_msg->param_buf[0];
			u8 control = (u8)p_ipc_msg->param_buf[1];
			if (control == 1) {
				ret = wifi_enable_packet_filter(filter_id);
			} else if (control == 2) {
				ret = wifi_disable_packet_filter(filter_id);
			} else if (control == 3) {
				ret = wifi_remove_packet_filter(filter_id);
			}
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_PROMISC_UPDATE_CANDI_AP_RSSI_AVG: {
			s8 rssi = (s8)p_ipc_msg->param_buf[0];
			u8 cnt = (u8)p_ipc_msg->param_buf[1];
			promisc_update_candi_ap_rssi_avg(rssi, cnt);
			break;
		}
		case IPC_API_PROMISC_GET_CHANNEL_BY_BSSID: {
			u8 *bssid = (u8 *)p_ipc_msg->param_buf[0];
			if (bssid) {
				DCache_Invalidate((u32)bssid, ETH_ALEN);
			}
			ret = promisc_get_chnl_by_bssid(bssid);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_IWPRIV_INFO: {
			char *cmd = (char *)p_ipc_msg->param_buf[0];
			int cmd_len = (int)p_ipc_msg->param_buf[1];
			int show_msg = (int)p_ipc_msg->param_buf[2];
			if (cmd) {
				DCache_Invalidate((u32)cmd, cmd_len);
			}
			ret = rtw_iwpriv_command(STA_WLAN_INDEX, cmd, show_msg);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_CONFIG_CSI: {
			rtw_csi_action_parm_t *act_param = (rtw_csi_action_parm_t *)p_ipc_msg->param_buf[0];
			DCache_Invalidate((u32)act_param, sizeof(rtw_csi_action_parm_t));
			ret = wifi_csi_config(act_param);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_GET_CSI_REPORT: {
			void *csi_buf = (u8 *)p_ipc_msg->param_buf[0];
			u32 buf_len = (u32)p_ipc_msg->param_buf[1];
			u32 *len = (u32 *)p_ipc_msg->param_buf[2];
			DCache_Invalidate((u32)csi_buf, buf_len);
			DCache_Invalidate((u32)len, sizeof(u32));

			ret = wifi_csi_report(buf_len, csi_buf, len);
			DCache_Clean((u32)csi_buf, buf_len);
			DCache_Clean((u32)len, sizeof(u32));
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_IP_UPDATE: {
			u8 *p_ip_addr = (u8 *)p_ipc_msg->param_buf[0];
			DCache_Invalidate((u32)(p_ip_addr), 4);
			rtw_memcpy(inic_ipc_ip_addr, p_ip_addr, 4);
			p_ipc_msg->ret = 0;
			break;
		}
#ifdef CONFIG_MP_INCLUDED
		case IPC_API_WIFI_MP_CMD: {
			char *cmd = (char *)p_ipc_msg->param_buf[0];
			int cmd_len = (int)p_ipc_msg->param_buf[1];
			int show_msg = (int)p_ipc_msg->param_buf[2];
			if (cmd) {
				DCache_Invalidate((u32)cmd, cmd_len);
			}
			ret = wext_private_command(STA_WLAN_INDEX, cmd, show_msg);
			p_ipc_msg->ret = ret;
			break;
		}
#endif
		default:
			DBG_8195A("Host Unknown API(%d)!\n\r", \
					  p_ipc_msg->API_ID);
			break;
		}
		/*set API_ID to 0 to notify AP that API is finished*/
		p_ipc_msg->API_ID = 0;

		DCache_Clean((u32)p_ipc_msg, sizeof(inic_ipc_host_request_message));//sizeof(inic_ipc_host_request_message)
	} while (1);
}

/**
 * @brief  to send a ipc message to device and wait resut.
 * @param  ID[in]: API_ID.
 * @param  param_buf[inout]: pointer to API parameter.
 * @return result of API.
 */
int inic_ipc_api_dev_message_send(u32 id, u32 *param_buf, u32 buf_len)
{
	int ret = 0;
	rtw_down_sema(&g_dev_inic_api_message_send_sema);

	rtw_memset(&g_dev_ipc_api_request_info, 0, sizeof(inic_ipc_dev_request_message));

	g_dev_ipc_api_request_info.EVENT_ID = id;
	if (param_buf != NULL) {
		rtw_memcpy(g_dev_ipc_api_request_info.param_buf, param_buf, buf_len * sizeof(u32));
	}
	DCache_Clean((u32)&g_dev_ipc_api_request_info, sizeof(inic_ipc_dev_request_message));

#ifdef IPC_DIR_MSG_TX
	rtw_memset(&g_dev_ipc_api_msg, 0, sizeof(IPC_MSG_STRUCT));
	g_dev_ipc_api_msg.msg_type = IPC_USER_POINT;
	g_dev_ipc_api_msg.msg = (u32)&g_dev_ipc_api_request_info;
	g_dev_ipc_api_msg.msg_len = sizeof(inic_ipc_dev_request_message);
	DCache_Clean((u32)&g_dev_ipc_api_msg, sizeof(IPC_MSG_STRUCT));

	ipc_send_message(IPC_DIR_MSG_TX, IPC_D2H_WIFI_API_TRAN, \
					 &g_dev_ipc_api_msg);
#else
	ipc_send_message(IPC_INT_CHAN_WIFI_API_TRAN, &g_dev_ipc_api_request_info);
#endif /* IPC_DIR_MSG_TX */

	while (g_dev_ipc_api_request_info.EVENT_ID != IPC_WIFI_EVT_PROCESS_DONE) {
		if (id == IPC_WIFI_EVT_PROMISC_CALLBACK) {
			DelayUs(10);
		} else {
			rtw_mdelay_os(1);
		}
		DCache_Invalidate((u32)&g_dev_ipc_api_request_info, sizeof(inic_ipc_dev_request_message));
	}
	ret = g_dev_ipc_api_request_info.ret;
	rtw_up_sema(&g_dev_inic_api_message_send_sema);
	return ret;
}

void inic_ipc_wifi_event_indicate(int event_cmd, char *buf, int buf_len, int flags)
{
	/*not indicate Beacon event since it is too frequent*/
	/*not indicate scan event since it is handled in device*/
	u32 param_buf[4];
	param_buf[0] = event_cmd;
	param_buf[1] = (u32)buf;
	param_buf[2] = buf_len;
	param_buf[3] = flags;
	DCache_Clean((u32)buf, buf_len);
	inic_ipc_api_dev_message_send(IPC_WIFI_EVT_HDL, param_buf, 4);
}

void inic_ipc_autoreconnect_indicate(
	rtw_security_t			security_type,
	char					*ssid,
	int						ssid_len,
	char					*password,
	int						password_len,
	int						key_id)
{
	u32 param_buf[6];
	param_buf[0] = security_type;
	param_buf[1] = (u32)ssid;
	param_buf[2] = ssid_len;
	param_buf[3] = (u32)password;
	param_buf[4] = password_len;
	param_buf[5] = key_id;
	DCache_Clean((u32)ssid, ssid_len);
	DCache_Clean((u32)password, password_len);
	inic_ipc_api_dev_message_send(IPC_WIFI_EVT_AUTO_RECONNECT, param_buf, 6);
}

void inic_ipc_eap_autoreconnect_indicate(u8 saved_eap_method)
{
	u32 param_buf[1];
	param_buf[0] = saved_eap_method;
	inic_ipc_api_dev_message_send(IPC_WIFI_EVT_EAP_AUTO_RECONNECT, param_buf, 1);
}

void inic_ipc_scan_user_callback_indicate(unsigned int ap_num, void *user_data)
{
	u32 param_buf[2];
	param_buf[0] = ap_num;
	param_buf[1] = (u32)user_data;

	inic_ipc_api_dev_message_send(IPC_WIFI_EVT_SCAN_USER_CALLBACK, param_buf, 2);
}

void inic_ipc_scan_each_report_user_callback_indicate(rtw_scan_result_t *scanned_ap_info, void *user_data)
{
	u32 param_buf[2];

	if (scanned_ap_info) {
		DCache_Clean((u32)scanned_ap_info, sizeof(rtw_scan_result_t));
	}
	param_buf[0] = (u32)scanned_ap_info;
	param_buf[1] = (u32)user_data;

	inic_ipc_api_dev_message_send(IPC_WIFI_EVT_SCAN_EACH_REPORT_USER_CALLBACK, param_buf, 4);
}

void inic_ipc_promisc_callback_indicate(unsigned char *buf, unsigned int buf_len, void *userdata)
{
	u32 param_buf[3];

	/*make inic_ipc_api_dev_task priority higer to process API event during callback*/
	u32 origin_prio = rtw_get_priority_task(api_dev_task);
	rtw_set_priority_task(api_dev_task, 6);

	DCache_Clean((u32)buf, (u32)buf_len);
	DCache_Clean((u32)userdata, sizeof(ieee80211_frame_info_t));
	param_buf[0] = (u32)buf;
	param_buf[1] = buf_len;
	param_buf[2] = (u32)userdata;
	inic_ipc_api_dev_message_send(IPC_WIFI_EVT_PROMISC_CALLBACK, param_buf, 3);
	rtw_set_priority_task(api_dev_task, origin_prio);
}

void inic_ipc_ap_ch_switch_callback_indicate(unsigned char channel, rtw_channel_switch_res_t ret)
{
	u32 param_buf[2];

	param_buf[0] = (u32)channel;
	param_buf[1] = (u32)ret;
	inic_ipc_api_dev_message_send(IPC_WIFI_EVT_AP_CH_SWITCH, param_buf, 2);
}
int inic_ipc_get_lwip_info(u32 type, unsigned char *input, int index)
{
	int ret;
	u32 param_buf[3];
	param_buf[0] = type;
	param_buf[1] = (u32)input;
	param_buf[2] = index;
	if (input && (index == IPC_WLAN_IS_VALID_IP)) {
		DCache_Clean((u32)input, 4);
	}
	ret = inic_ipc_api_dev_message_send(IPC_WIFI_EVT_GET_LWIP_INFO, param_buf, 3);

	if (type == IPC_WLAN_GET_IP || type == IPC_WLAN_GET_GW || type == IPC_WLAN_GET_GWMSK) {
		DCache_Invalidate((u32)ret, 4);
	}
	if (type == IPC_WLAN_GET_HW_ADDR) {
		DCache_Invalidate((u32)ret, 6);
	}

	return ret;
}

int inic_ipc_dev_set_netif_info(int idx_wlan, unsigned char *dev_addr)
{
	int ret;
	u32 param_buf[2];
	param_buf[0] = idx_wlan;
	param_buf[1] = (u32)dev_addr;
	if (dev_addr) {
		DCache_Clean((u32)dev_addr, ETH_ALEN);
	}
	ret = inic_ipc_api_dev_message_send(IPC_WIFI_EVT_SET_NETIF_INFO, param_buf, 2);

	return ret;
}

/**
 * @brief  to haddle the ipc message interrupt, wakeup event task to process.
 * @param  Data[inout]: IPC data.
 * @param  IrqStatus[in]: interrupt status.
 * @param  ChanNum[in]: IPC channel number.
 * @return none.
 */
void inic_ipc_api_dev_int_hdl(VOID *Data, u32 IrqStatus, u32 ChanNum)
{
	/* To avoid gcc warnings */
	(void) Data;
	(void) IrqStatus;
	(void) ChanNum;

	/* wakeup task */
	rtw_up_sema_from_isr(&g_dev_inic_api_task_wake_sema);
}

/**
 * @brief  to initialize the ipc host for WIFI api.
 * @param  none.
 * @return none.
 */
void inic_ipc_api_init_dev(VOID)
{
	/* initialize the semaphores */
	rtw_init_sema(&g_dev_inic_api_task_wake_sema, 0);
	rtw_init_sema(&g_dev_inic_api_message_send_sema, 0);
	rtw_up_sema(&g_dev_inic_api_message_send_sema);

	/* Initialize the event task */
	if (pdTRUE != xTaskCreate((TaskFunction_t)inic_ipc_api_dev_task, \
							  (const char *const)"inic_ipc_api_dev_task", INIC_IPC_API_STACK, NULL, \
							  tskIDLE_PRIORITY + CONFIG_INIC_IPC_DEV_API_PRIO, &api_dev_task)) {
		DBG_8195A("Create inic_ipc_api_dev_task Err!!\n");
	}
}

/* ---------------------------- Global Variables ---------------------------- */
#if defined(CONFIG_PLATFORM_AMEBALITE) || defined(CONFIG_PLATFORM_AMEBAD2)
IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE   ipc_api_dev_table[] = {
	{IPC_USER_POINT,	inic_ipc_api_dev_int_hdl,	(VOID *) NULL, IPC_DIR_MSG_RX, IPC_H2D_WIFI_API_TRAN, IPC_RX_FULL},
};
#endif
