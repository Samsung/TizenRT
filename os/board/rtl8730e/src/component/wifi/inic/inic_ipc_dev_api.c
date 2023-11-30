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

#include "wifi_intf_drv_to_upper.h"
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
extern int wext_private_command(unsigned char wlan_idx, char *cmd, int show_msg, char *user_buf);
#endif
extern int wifi_set_ips_internal(u8 enable);
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

			if (wifi_user_config.cfg80211) {
				/* KVR Roaming from cfg80211. */
				rtw_joinbss_by_roaming(pWifi);
			}

			if (pWifi->password_len) {
				DCache_Invalidate((u32)pWifi->password, pWifi->password_len);
			}

			ret = rtw_joinbss_start_api(pWifi);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_ADD_KEY: {
			struct rtw_crypt_info *crypt = (struct rtw_crypt_info *)p_ipc_msg->param_buf[0];
			DCache_Invalidate((u32)(crypt), sizeof(struct rtw_crypt_info));
			wifi_wpa_add_key(crypt);
			p_ipc_msg->ret = 0;
			break;
		}

		case IPC_API_WPA_PMKSA_OPS: {
			struct rtw_pmksa_ops_t *pmksa_ops = (struct rtw_pmksa_ops_t *)p_ipc_msg->param_buf[0];
			DCache_Invalidate((u32)(pmksa_ops), sizeof(struct rtw_pmksa_ops_t));
			wifi_wpa_pmksa_ops(pmksa_ops);
			p_ipc_msg->ret = 0;
			break;
		}
		case IPC_API_WIFI_GET_CHPLAN: {
			u8 *chplan = (u8 *)p_ipc_msg->param_buf[0];
			DCache_Invalidate((u32)chplan, sizeof(int));
			ret = wifi_get_chplan(chplan);
			DCache_Clean((u32)chplan, sizeof(int));
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
			unsigned char wlan_idx = (unsigned char)p_ipc_msg->param_buf[0];
			u8 channel = (u8)p_ipc_msg->param_buf[1];
			ret = wifi_set_channel(wlan_idx, channel);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_GET_CHANNEL: {
			unsigned char wlan_idx = (unsigned char)p_ipc_msg->param_buf[0];
			u8 *channel = (u8 *)p_ipc_msg->param_buf[1];
			DCache_Invalidate((u32)channel, sizeof(int));
			ret = wifi_get_channel(wlan_idx, channel);
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
		case IPC_API_WIFI_INIT_AP: {
			ret = _wifi_on_ap();
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_DEINIT_AP: {
			ret = _wifi_off_ap();
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_START_AP: {
			rtw_softap_info_t *softAP_config = (rtw_softap_info_t *)p_ipc_msg->param_buf[0];
			DCache_Invalidate((u32)softAP_config->password, softAP_config->password_len);
			DCache_Invalidate((u32)softAP_config, sizeof(rtw_softap_info_t));

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
		case IPC_API_WIFI_SET_MAC_ADDR: {
			int idx = (int)p_ipc_msg->param_buf[0];
			unsigned char *mac = (unsigned char *)p_ipc_msg->param_buf[1];
			u8 efuse = (u8)p_ipc_msg->param_buf[2];
			DCache_Invalidate((u32)mac, sizeof(rtw_mac_t));
			ret = wifi_set_mac_address(idx, mac, efuse);
			DCache_Clean((u32)mac, sizeof(rtw_mac_t));
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_GET_MAC_ADDR: {
			int idx = (int)p_ipc_msg->param_buf[0];
			rtw_mac_t *mac = (rtw_mac_t *)p_ipc_msg->param_buf[1];
			u8 efuse = (u8)p_ipc_msg->param_buf[2];
			DCache_Invalidate((u32)mac, sizeof(rtw_mac_t));
			ret = wifi_get_mac_address(idx, mac, efuse);
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
			u8 role = (u8)p_ipc_msg->param_buf[1];
			u8 process = (u8)p_ipc_msg->param_buf[2];
			wifi_btcoex_set_pta(gntbt_id, role, process);
			break;
		}
		case IPC_API_WIFI_COEX_BT_RFK: {

			struct bt_rfk_param *rfk_param = (struct bt_rfk_param *)p_ipc_msg->param_buf[0];
			DCache_Invalidate((u32)rfk_param, sizeof(struct bt_rfk_param));

			ret = wifi_btcoex_bt_rfk(rfk_param);
			DCache_Clean((u32)rfk_param, sizeof(struct bt_rfk_param));
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_COEX_ZB_RFK: {
			wifi_zigbee_coex_zb_rfk();
			break;
		}
		case IPC_API_WIFI_SET_WPA_MODE: {
			rtw_wpa_mode wpa_mode = p_ipc_msg->param_buf[0];
			ret = wifi_set_wpa_mode(wpa_mode);
			p_ipc_msg->ret = ret;
			break;
		}

		case IPC_API_WPA_4WAY_FAIL: {
			wifi_wpa_sta_4way_fail_notify();
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
			rtw_client_list_t *client_list_buffer = (rtw_client_list_t *)p_ipc_msg->param_buf[0];
			DCache_Invalidate((u32)client_list_buffer, sizeof(rtw_client_list_t));

			ret = wifi_get_associated_client_list(client_list_buffer);
			DCache_Clean((u32)client_list_buffer, sizeof(rtw_client_list_t));
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
			ret = wifi_set_ips_internal(ips_en);
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
		case IPC_API_WIFI_SAE_STATUS: {
			unsigned char wlan_idx = (unsigned char)p_ipc_msg->param_buf[0];
			u16 value = (u16)p_ipc_msg->param_buf[1];
			u8 *mac_addr = (u8 *)p_ipc_msg->param_buf[2];
			if (mac_addr) {
				DCache_Invalidate((u32)mac_addr, 6);
			}
			ret = wifi_sae_status_indicate(wlan_idx, value, mac_addr);
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
			ret = wifi_fetch_phy_statistic(statistic);
			DCache_Clean((u32)statistic, sizeof(rtw_phy_statistics_t));
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_NETWORK_MODE: {
			enum wlan_mode mode = (enum wlan_mode)p_ipc_msg->param_buf[0];
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
			ret = wifi_if_send_eapol(wlan_idx, buf, buf_len, flags);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_CONFIG_AUTORECONNECT: {
#if CONFIG_AUTO_RECONNECT
			u8 mode = (u8)p_ipc_msg->param_buf[0];
			ret = wifi_config_autoreconnect(mode);
			p_ipc_msg->ret = ret;
#else
			p_ipc_msg->ret = -1;
#endif
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
			ret = wifi_send_mgnt(raw_data_desc);
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
			rtw_csa_parm_t *csa_param = (rtw_csa_parm_t *)p_ipc_msg->param_buf[0];
			DCache_Invalidate((u32)csa_param, sizeof(rtw_csa_parm_t));
			ret = wifi_ap_switch_chl_and_inform(csa_param);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_NO_BEACON_TIMEOUT: {
			unsigned char timeout = (unsigned char)p_ipc_msg->param_buf[0];
			wifi_set_no_beacon_timeout(timeout);
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
			if (wifi_user_config.cfg80211) {
				pmu_release_wakelock(PMU_OS);
			}
			p_ipc_msg->ret = 0;
			break;
		}
#ifdef CONFIG_NAN
		case IPC_API_NAN_START: {
			u8 master_pref = (u8)p_ipc_msg->param_buf[0];
			u8 band_support = p_ipc_msg->param_buf[1];
			nan_intfs_init();
			p_ipc_msg->ret = rtw_start_nan_api(master_pref, band_support);
			break;
		}
		case IPC_API_NAN_STOP: {
			rtw_stop_nan_api();
			nan_intfs_deinit();
			p_ipc_msg->ret = 0;
			break;
		}
		case IPC_API_NAN_ADD_FUNC: {
			void *func = (void *)p_ipc_msg->param_buf[0];
			void *nan_func_pointer = (void *)p_ipc_msg->param_buf[1];
			u32 len = p_ipc_msg->param_buf[2];
			DCache_Invalidate((u32)func, len);
			p_ipc_msg->ret = rtw_add_nan_func(func, nan_func_pointer);
			break;
		}
		case IPC_API_NAN_DEL_FUNC: {
			u64 cookie = 0;
			cookie = (u64)(p_ipc_msg->param_buf[1]);
			cookie = cookie << 32;
			cookie |= p_ipc_msg->param_buf[0];
			p_ipc_msg->ret = rtw_del_nan_func(cookie);
			break;
		}
		case IPC_API_NAN_CFGVENFOR: {
			u16 vendor_cmd = p_ipc_msg->param_buf[0];
			void *data = (void *)p_ipc_msg->param_buf[1];
			u32 len = p_ipc_msg->param_buf[2];
			DCache_Invalidate((u32)data, len);
			rtw_cfgvendor_cmd_process(vendor_cmd, data, len);
			p_ipc_msg->ret = 0;
			break;
		}

#endif

#ifdef CONFIG_MP_INCLUDED
		case IPC_API_WIFI_MP_CMD: {
			char *cmd = (char *)p_ipc_msg->param_buf[0];
			int cmd_len = (int)p_ipc_msg->param_buf[1];
			int show_msg = (int)p_ipc_msg->param_buf[2];
			char *user_buf = (char *)p_ipc_msg->param_buf[3];
			if (cmd) {
				DCache_Invalidate((u32)cmd, cmd_len);
			}
			if (user_buf) {
				DCache_Invalidate((u32)user_buf, INIC_MP_MSG_BUF_SIZE);
			}
			ret = wext_private_command(STA_WLAN_INDEX, cmd, show_msg, user_buf);
			DCache_Clean((u32)user_buf, INIC_MP_MSG_BUF_SIZE);
			p_ipc_msg->ret = ret;
			break;
		}
		case IPC_API_WIFI_SET_BT_SEL: {
			u8 bt_ant = p_ipc_msg->param_buf[0];
			wifi_btcoex_set_bt_ant(bt_ant);
			break;
		}
#endif
		/* jira: https://jira.realtek.com/browse/RSWLANQC-1036 */
		case IPC_API_WIFI_MSG_TO: {
			DBG_8195A("MSG TO, No need\n\r");
			break;
		}
		case IPC_API_WIFI_PROMISC_INIT: {
			promisc_para_t promisc_para;
			u32 status  = p_ipc_msg->param_buf[0];
			memset(&promisc_para, 0, sizeof(promisc_para_t));
			promisc_para.filter_mode = p_ipc_msg->param_buf[1];
			promisc_para.callback = (void *)p_ipc_msg->param_buf[2]; //true or flase
			wifi_promisc_enable(status, &promisc_para);
			break;
		}
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
			DelayUs(5);
		} else if (g_dev_ipc_api_request_info.param_buf[0] == WIFI_EVENT_CSI_DONE) {
			/* high sounding rate csi requires less processing time, so used us delay instead of ms */
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

void inic_ipc_autoreconnect_indicate(rtw_security_t security_type, u8 *ssid, int ssid_len, u8 *password, int password_len, int key_id, char is_wps_trigger)
{
	u32 param_buf[7];
	param_buf[0] = security_type;
	param_buf[1] = (u32)ssid;
	param_buf[2] = ssid_len;
	param_buf[3] = (u32)password;
	param_buf[4] = password_len;
	param_buf[5] = key_id;
	param_buf[6] = is_wps_trigger;
	DCache_Clean((u32)ssid, ssid_len);
	DCache_Clean((u32)password, password_len);
	inic_ipc_api_dev_message_send(IPC_WIFI_EVT_AUTO_RECONNECT, param_buf, 7);
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

void inic_ipc_promisc_callback_indicate(void *pkt_info)
{
	u32 param_buf[3];
	struct rx_pkt_info *ppktinfo = (struct rx_pkt_info *)pkt_info;

	/*make inic_ipc_api_dev_task priority higer to process API event during callback*/
	u32 origin_prio = rtw_get_priority_task(api_dev_task);
	rtw_set_priority_task(api_dev_task, 6);

	DCache_Clean((u32)ppktinfo->buf, (u32)ppktinfo->len);
	DCache_CleanInvalidate((u32)pkt_info, sizeof(struct rx_pkt_info));
	param_buf[0] = (u32)pkt_info;
	inic_ipc_api_dev_message_send(IPC_WIFI_EVT_PROMISC_CALLBACK, param_buf, 1);
	rtw_set_priority_task(api_dev_task, origin_prio);
}

int inic_ip_in_table_indicate(u8 gate, u8 ip)
{
	u32 param_buf[2];
	int ret;
	param_buf[0] = gate;
	param_buf[1] = ip;

	ret = inic_ipc_api_dev_message_send(IPC_WIFI_EVT_IP_TABLE_CHK, param_buf, 2);
	return ret;
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

void inic_ipc_cfg80211_indicate_scan_report(u32 channel, u32 frame_is_bcn, s32 rssi, u8 *mac_addr, u8 *IEs, u32 ie_len)
{
	u32 param_buf[6];
	param_buf[0] = channel;
	param_buf[1] = frame_is_bcn;
	param_buf[2] = (u32)rssi;
	param_buf[3] = (u32)mac_addr;
	param_buf[4] = (u32)IEs;
	param_buf[5] = ie_len;

	if (mac_addr) {
		DCache_Clean((u32)mac_addr, ETH_ALEN);
	}
	if (IEs && (ie_len > 0)) {
		DCache_Clean((u32)IEs, ie_len);
	}
	inic_ipc_api_dev_message_send(IPC_WIFI_EVT_CFG80211_SCAN_REPORT, param_buf, 6);
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

#ifdef CONFIG_NAN
void inic_ipc_cfg80211_indicate_nan_match(u8 type, u8 inst_id, u8 peer_inst_id, u8 *addr, u32 info_len, u8 *info, u64 cookie)
{
	if (!wifi_user_config.cfg80211) {
		return;
	}

	u32 param_buf[8];
	param_buf[0] = type;
	param_buf[1] = inst_id;
	param_buf[2] = peer_inst_id;
	param_buf[3] = (u32)addr;
	param_buf[4] = info_len;
	param_buf[5] = (u32)info;
	param_buf[6] = cookie & 0xFFFFFFFF;
	param_buf[7] = cookie >> 32 & 0xFFFFFFFF;
	if (addr) {
		DCache_Clean((u32)addr, ETH_ALEN);
	}
	if (info && (info_len > 0)) {
		DCache_Clean((u32)info, info_len);
	}

	inic_ipc_api_dev_message_send(IPC_WIFI_EVT_CFG80211_NAN_REPORT_MATCH_EVENT, param_buf, 8);
}

void inic_ipc_cfg80211_nan_func_free(void  *data)
{
	if (!wifi_user_config.cfg80211) {
		return;
	}

	u32 param_buf[1];
	param_buf[0] = (u32)data;

	inic_ipc_api_dev_message_send(IPC_WIFI_EVT_CFG80211_NAN_DEL_FUNC, param_buf, 1);
}

void inic_ipc_cfg80211_nan_cfgvendor_event_report(u16 event_id, void *event, int event_len)
{
	if (!wifi_user_config.cfg80211) {
		return;
	}

	u32 param_buf[3];
	param_buf[0] = (u32)event_id;
	param_buf[1] = (u32)event;
	param_buf[2] = (u32)event_len;
	if (event && (event_len > 0)) {
		DCache_Clean((u32)event, event_len);
	}

	inic_ipc_api_dev_message_send(IPC_WIFI_EVT_CFG80211_NAN_CFGVENDOR_EVENT, param_buf, 3);
}

void inic_ipc_cfg80211_cfgvendor_send_cmd_reply(void *data, int len)
{
	if (!wifi_user_config.cfg80211) {
		return;
	}

	u32 param_buf[2];
	param_buf[0] = (u32)data;
	param_buf[1] = (u32)len;
	if (data && (len > 0)) {
		DCache_Clean((u32)data, len);
	}

	inic_ipc_api_dev_message_send(IPC_WIFI_EVT_CFG80211_NAN_CFGVENDOR_CMD_REPLY, param_buf, 2);
}

#endif

/* ---------------------------- Global Variables ---------------------------- */
#if defined(CONFIG_PLATFORM_AMEBALITE) || defined(CONFIG_PLATFORM_AMEBAD2) || defined(CONFIG_PLATFORM_AMEBADPLUS) || defined(CONFIG_PLATFORM_RTL8720F)
IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE ipc_api_dev_table = {
	.USER_MSG_TYPE = IPC_USER_POINT,
	.Rxfunc = inic_ipc_api_dev_int_hdl,
	.RxIrqData = (VOID *) NULL,
	.Txfunc = IPC_TXHandler,
	.TxIrqData = (VOID *) NULL,
	.IPC_Direction = IPC_DIR_MSG_RX,
	.IPC_Channel = IPC_H2D_WIFI_API_TRAN
};
#endif
