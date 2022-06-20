//----------------------------------------------------------------------------//
//#include <flash/stm32_flash.h>
#if !defined(CONFIG_MBED_ENABLED) && !defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
#include "main.h"
#if CONFIG_LWIP_LAYER
#include <lwip_netconf.h>
#include <dhcp/dhcps.h>
#endif
#endif
#include <platform_stdlib.h>
#include <wifi_conf.h>
#include <wifi_ind.h>
#include <osdep_service.h>
#include <device_lock.h>

#if (defined(CONFIG_EXAMPLE_UART_ATCMD) && CONFIG_EXAMPLE_UART_ATCMD) || (defined(CONFIG_EXAMPLE_SPI_ATCMD) && CONFIG_EXAMPLE_SPI_ATCMD)
#include "at_cmd/atcmd_wifi.h"
#endif
#if defined(CONFIG_PLATFORM_8721D) || defined(CONFIG_PLATFORM_8710C) || defined(CONFIG_PLATFORM_AMEBAD2) || defined(CONFIG_PLATFORM_8735B) || defined(CONFIG_PLATFORM_AMEBALITE)
#include "platform_opts_bt.h"
#endif
#if defined(CONFIG_ENABLE_WPS_AP) && CONFIG_ENABLE_WPS_AP
#include <wifi_wps_config.h>
#endif

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/
#if CONFIG_AUTO_RECONNECT
struct wifi_autoreconnect_param {
	rtw_security_t security_type;
	char *ssid;
	int ssid_len;
	char *password;
	int password_len;
	int key_id;
};
#endif
/******************************************************
 *               Variables Declarations
 ******************************************************/

#if !defined(CONFIG_MBED_ENABLED)
#if CONFIG_LWIP_LAYER
extern struct netif xnetif[NET_IF_NUM];
#endif
#endif
#if CONFIG_AUTO_RECONNECT
extern p_wlan_autoreconnect_hdl_t p_wlan_autoreconnect_hdl;
#endif

#if defined(ATCMD_VER) && defined(ATCMD_VER) && ATCMD_VER == ATVER_2
extern unsigned char dhcp_mode_sta;
#endif

/******************************************************
 *               Variables Declarations
 ******************************************************/
#if defined(CONFIG_MBED_ENABLED)
rtw_mode_t wifi_mode = RTW_MODE_STA;
#endif
extern rtw_mode_t wifi_mode;

void *param_indicator;
struct task_struct wifi_autoreconnect_task;

/******************************************************
 *               Variables Definitions
 ******************************************************/

/*NETMASK*/
#ifndef NETMASK_ADDR0
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0
#endif

/*Gateway Address*/
#ifndef GW_ADDR0
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   1
#define GW_ADDR3   1
#endif

/******************************************************
 *               Function Definitions
 ******************************************************/

#if CONFIG_WLAN

//----------------------------------------------------------------------------//
int wifi_get_mac_address(rtw_mac_t *mac)
{
	return rtw_ex_read_mac(WLAN0_IDX, mac);
}

//----------------------------------------------------------------------------//
//  20210422
//  wifi_btcoex_set_bt_on(), wifi_btcoex_set_bt_off() have been replaced.
//  Remove defination and reference.
//
/*void wifi_btcoex_set_bt_on(void)
{
	//rltk_wlan_btcoex_set_bt_state(1);
}*/

/*void wifi_btcoex_set_bt_off(void)
{
	//rltk_wlan_btcoex_set_bt_state(0);
}*/

int wifi_btcoex_set_ble_scan_duty(u8 duty)
{
	return rltk_coex_set_ble_scan_duty(duty);
}

//----------------------------------------------------------------------------//
int wifi_get_associated_client_list(void *client_list_buffer, uint16_t buffer_length)
{
	/* To avoid gcc warnings */
	(void) buffer_length;

	unsigned char wlan_idx = WLAN0_IDX;
	int ret = 0;

	if (wifi_mode == RTW_MODE_STA_AP) {
		wlan_idx = WLAN1_IDX;
	}

	ret = rtw_ex_get_client_list(wlan_idx, (rtw_maclist_t *)client_list_buffer);

	return ret;
}

//----------------------------------------------------------------------------//
int wifi_get_setting(unsigned char wlan_idx, rtw_wifi_setting_t *psetting)
{
	int ret = 0;
	int mode = 0;
	unsigned short security = 0;
	unsigned int auth_type = 0;

	if (wlan_idx > WLAN1_IDX) {
		return RTW_ERROR;
	}

	memset(psetting, 0, sizeof(rtw_wifi_setting_t));
	if (rtw_wx_get_mode(wlan_idx, &mode) < 0) {
		ret = -1;
	}

	switch (mode) {
	case RTW_MODE_MASTER:
		psetting->mode = RTW_MODE_AP;
		break;
	case RTW_MODE_INFRA:
	default:
		psetting->mode = RTW_MODE_STA;
		break;
		//default:
		//RTW_API_INFO("\r\n%s(): Unknown mode %d\n", __func__, mode);
		//break;
	}

	if (rtw_wx_get_wap(wlan_idx, psetting->bssid) < 0) {
		ret = -1;
	}
	if (rtw_wx_get_essid(wlan_idx, psetting->ssid) < 0) {
		ret = -1;
	}
	if (rtw_wx_get_freq(wlan_idx, &psetting->channel) < 0) {
		ret = -1;
	}
	if (rtw_wx_get_enc_ext(wlan_idx, &security, &psetting->key_idx, psetting->password) < 0) {
		ret = -1;
	}

	switch (security) {
	case RTW_ENCODE_ALG_NONE:
		psetting->security_type = RTW_SECURITY_OPEN;
		break;
	case RTW_ENCODE_ALG_WEP:
		psetting->security_type = RTW_SECURITY_WEP_PSK;
		break;
	case RTW_ENCODE_ALG_TKIP:
		if (rtw_wx_get_auth_type(wlan_idx, &auth_type) < 0) {
			ret = -1;
		}
		if (auth_type == WPA_SECURITY) {
			psetting->security_type = RTW_SECURITY_WPA_TKIP_PSK;
		} else if (auth_type == WPA2_SECURITY) {
			psetting->security_type = RTW_SECURITY_WPA2_TKIP_PSK;
		}
		break;
	case RTW_ENCODE_ALG_CCMP:
		if (rtw_wx_get_auth_type(wlan_idx, &auth_type) < 0) {
			ret = -1;
		}
		if (auth_type == WPA_SECURITY) {
			psetting->security_type = RTW_SECURITY_WPA_AES_PSK;
		} else if (auth_type == WPA2_SECURITY) {
			psetting->security_type = RTW_SECURITY_WPA2_AES_PSK;
		} else if (auth_type == WPA3_SECURITY) {
			psetting->security_type = RTW_SECURITY_WPA3_AES_PSK;
		}
		break;
	default:
		break;
	}

	if (security == RTW_ENCODE_ALG_TKIP || security == RTW_ENCODE_ALG_CCMP)
		if (rtw_wx_get_passphrase(wlan_idx, psetting->password) < 0) {
			ret = -1;
		}

	return ret;

}

int wifi_fetch_phy_statistic(rtw_phy_statistics_t *phy_statistic)
{
	return rtw_wx_get_phy_statistic(phy_statistic);
}

int wifi_get_ccmp_key(unsigned char *uncst_key, unsigned char *group_key)
{
	return rltk_wlan_get_ccmp_key(uncst_key, group_key);
}
//----------------------------------------------------------------------------//
int wifi_set_powersave_mode(u8 ips_mode, u8 lps_mode)
{
	int ret = RTW_SUCCESS;

	if ((ips_mode >= IPS_MODE_NUM) && (lps_mode >= LPS_MODE_NUM)) {
		printf("invalid powersave mode\n");
		return RTW_ERROR;
	}

	if (ips_mode < IPS_MODE_NUM) {
		ret = rtw_pm_set(RTW_PM_OPTION_IPS_MODE, &ips_mode);
	}
	if (lps_mode < LPS_MODE_NUM) {
		ret = rtw_pm_set(RTW_PM_OPTION_LPS_MODE, &lps_mode);
	}
	return ret;
}

//----------------------------------------------------------------------------//

int wifi_set_mfp_support(unsigned char value)
{
	return rtw_wx_set_mfp_support(WLAN0_IDX, value);
}

int wifi_set_group_id(unsigned char value)
{
#ifdef CONFIG_SAE_SUPPORT
	return rtw_wx_set_group_id(WLAN0_IDX, value);
#else
	return RTW_SUCCESS;
#endif
}

int wifi_set_network_mode(rtw_network_mode_t mode)
{
	if ((mode == RTW_NETWORK_B) || (mode == RTW_NETWORK_BG) || (mode == RTW_NETWORK_BGN)) {
		return rltk_wlan_wireless_mode((unsigned char) mode);
	}

	return RTW_ERROR;
}

static void _wifi_autoreconnect_thread(void *param)
{
#if CONFIG_AUTO_RECONNECT
#if defined(configENABLE_TRUSTZONE) && (configENABLE_TRUSTZONE == 1)
	rtw_create_secure_context(configMINIMAL_SECURE_STACK_SIZE);
#endif
	int ret = RTW_ERROR;
	struct wifi_autoreconnect_param *reconnect_info = (struct wifi_autoreconnect_param *) param;
	rtw_network_info_t connect_param = {0};

	if (reconnect_info->ssid_len) {
		rtw_memcpy(connect_param.ssid.val, reconnect_info->ssid, reconnect_info->ssid_len);
		connect_param.ssid.len = reconnect_info->ssid_len;
	}
	connect_param.password = (unsigned char *)reconnect_info->password;
	connect_param.password_len = reconnect_info->password_len;
	connect_param.security_type = reconnect_info->security_type;
	connect_param.key_id = reconnect_info->key_id;

	RTW_API_INFO("\n\rauto reconnect ...\n");
	ret = wifi_connect(&connect_param, 1);

	if (ret == RTW_SUCCESS) {
#if CONFIG_LWIP_LAYER
#if ATCMD_VER == ATVER_2
		if (dhcp_mode_sta == 2) {
			struct netif *pnetif = &xnetif[0];
			u32 addr = WIFI_MAKEU32(GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
			u32 netmask = WIFI_MAKEU32(NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
			u32 gw = WIFI_MAKEU32(GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
			LwIP_SetIP(0, addr, netmask, gw);
			dhcps_init(pnetif);
		} else
#endif
		{
			LwIP_DHCP(0, DHCP_START);
#if LWIP_AUTOIP
			/*delete auto ip process for conflict with dhcp
						uint8_t *ip = LwIP_GetIP(0);
						if ((ip[0] == 0) && (ip[1] == 0) && (ip[2] == 0) && (ip[3] == 0)) {
							RTW_API_INFO("\n\nIPv4 AUTOIP ...");
							LwIP_AUTOIP(0);
						}
			*/
#endif
		}
#endif //#if CONFIG_LWIP_LAYER
	}

	param_indicator = NULL;
	rtw_delete_task(&wifi_autoreconnect_task);
#endif
}

void wifi_autoreconnect_hdl(rtw_security_t security_type,
							char *ssid, int ssid_len,
							char *password, int password_len,
							int key_id)
{
#if CONFIG_AUTO_RECONNECT
#if defined(CONFIG_MBED_ENABLED) || defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
	RTW_API_INFO("\n\r%s Not Implemented Yet!\n", __func__);
#else
	static struct wifi_autoreconnect_param param;
	param_indicator = &param;
	param.security_type = security_type;
	param.ssid = ssid;
	param.ssid_len = ssid_len;
	param.password = password;
	param.password_len = password_len;
	param.key_id = key_id;

	if (wifi_autoreconnect_task.task != NULL) {
#if CONFIG_LWIP_LAYER
		dhcp_stop(&xnetif[0]);
#endif
		u32 start_tick = rtw_get_current_time();
		while (1) {
			rtw_msleep_os(2);
			u32 passing_tick = rtw_get_current_time() - start_tick;
			if (rtw_systime_to_sec(passing_tick) >= 2) {
				RTW_API_INFO("\r\n Create wifi_autoreconnect_task timeout \r\n");
				return;
			}

			if (wifi_autoreconnect_task.task == NULL) {
				break;
			}
		}
	}


	rtw_create_task(&wifi_autoreconnect_task, (const char *)"wifi_autoreconnect", 512, tskIDLE_PRIORITY + 1, _wifi_autoreconnect_thread, &param);
#endif
#endif
}

int wifi_config_autoreconnect(__u8 mode, __u8 retry_times, __u16 timeout)
{
#if CONFIG_AUTO_RECONNECT
	if (mode == RTW_AUTORECONNECT_DISABLE) {
		p_wlan_autoreconnect_hdl = NULL;
	} else {
		p_wlan_autoreconnect_hdl = wifi_autoreconnect_hdl;
	}
	return rtw_wx_set_autoreconnect(mode, retry_times, timeout);
#else
	return -1;
#endif
}

int wifi_get_autoreconnect(__u8 *mode)
{
#if CONFIG_AUTO_RECONNECT
	return rtw_wx_get_autoreconnect(mode);
#else
	return 0;
#endif
}

//----------------------------------------------------------------------------//
/*
 * Example for custom ie
 *
 * u8 test_1[] = {221, 2, 2, 2};
 * u8 test_2[] = {221, 2, 1, 1};
 * rtw_custom_ie_t buf[2] = {{test_1, PROBE_REQ},
 *		 {test_2, PROBE_RSP | BEACON}};
 * u8 buf_test2[] = {221, 2, 1, 3} ;
 * rtw_custom_ie_t buf_update = {buf_test2, PROBE_REQ};
 *
 * add ie list
 * static void cmd_add_ie(int argc, char **argv)
 * {
 *	 wifi_add_custom_ie((void *)buf, 2);
 * }
 *
 * update current ie
 * static void cmd_update_ie(int argc, char **argv)
 * {
 *	 wifi_update_custom_ie(&buf_update, 2);
 * }
 *
 * delete all ie
 * static void cmd_del_ie(int argc, char **argv)
 * {
 *	 wifi_del_custom_ie();
 * }
 */

int wifi_add_custom_ie(void *cus_ie, int ie_num)
{
	return rtw_wx_set_custome_ie(WLAN0_NAME, cus_ie, ie_num);
}


int wifi_update_custom_ie(void *cus_ie, int ie_index)
{
	return rtw_wx_update_custome_ie(WLAN0_NAME, cus_ie, ie_index);
}

int wifi_del_custom_ie()
{
	return rtw_wx_del_custome_ie(WLAN0_NAME);
}
//----------------------------------------------------------------------------//

void wifi_set_indicate_mgnt(int enable)
{
	rltk_wlan_set_indicate_mgnt(enable);
	return;
}

int wifi_send_raw_frame(raw_data_desc_t *raw_data_desc)
{
	return rtw_wx_send_raw_frame(raw_data_desc);
}

int wifi_init_mac_filter(void)
{
	return rltk_wlan_init_mac_filter();
}

int wifi_add_mac_filter(unsigned char *hwaddr)
{
	return rltk_wlan_add_mac_filter(hwaddr);
}

int wifi_del_mac_filter(unsigned char *hwaddr)
{
	return rltk_wlan_del_mac_filter(hwaddr);
}

int wifi_get_antenna_info(unsigned char *antenna)
{
#ifdef CONFIG_ANTENNA_DIVERSITY
	int ret = 0;

	ret = rtw_ex_get_antenna_info(WLAN0_IDX, antenna);
	return ret;
#else
	UNUSED(antenna);
	return -1;
#endif
}

unsigned int wifi_get_tsf_low(unsigned char port_id)
{
	return rltk_wlan_get_tsf(port_id);
}

int wifi_set_tx_rate_by_ToS(unsigned char enable, unsigned char ToS_precedence, unsigned char tx_rate)
{
	return rltk_wlan_set_tx_rate_by_ToS(enable, ToS_precedence, tx_rate);
}

int wifi_set_EDCA_param(unsigned int AC_param)
{
	return rltk_wlan_set_EDCA_param(AC_param);
}

int wifi_set_TX_CCA(unsigned char enable)
{
	return rltk_wlan_set_TX_CCA(enable);
}

int wifi_ap_switch_chl_and_inform(unsigned char new_chl, unsigned char chl_switch_cnt, ap_channel_switch_callback_t callback)
{
	return rltk_wlan_ap_switch_chl_and_inform_sta(new_chl, chl_switch_cnt, callback);
}

int wifi_set_cts2self_duration_and_send(unsigned char wlan_idx, unsigned short duration)
{
	return rltk_wlan_set_cts2self_dur_and_send(wlan_idx, duration);
}

int wifi_get_sta_max_data_rate(OUT unsigned char *inidata_rate)
{
	return rltk_wlan_get_sta_max_data_rate(inidata_rate);
}

void wifi_set_no_beacon_timeout(unsigned char timeout_sec)
{
	rltk_wlan_set_no_beacon_timeout(timeout_sec);
}

/**
Example:
unsigned char mac[ETH_ALEN] = {0x00, 0xe0, 0x4c, 0x87, 0x12, 0x34};
wifi_change_mac_address_from_ram(idx, mac);
This method is to modify the mac and don't write to efuse.
**/
int wifi_change_mac_address_from_ram(int idx, unsigned char *mac)
{
	return rltk_change_mac_address_from_ram(idx, mac);
}

/**
Example:
unsigned char mac[] = "00e04c871234"
wifi_set_wifi_mac_address(mac);
This method is to modify the mac and write to efuse.
**/
int wifi_set_wifi_mac_address(u8 *mac)
{
	return rltk_wlan_set_wifi_mac_address(mac);
}

/**
Example:
wifi_get_wifi_mac_address(mac);
This method is to get the wifi mac from efuse.
**/
int wifi_get_wifi_mac_address(u8 *mac)
{
	return rltk_wlan_get_wifi_mac_address(mac);
}

/**
Example:
unsigned char mac[] = "00e04c871234"
wifi_set_bt_mac_address(mac);
This method is to modify the bt mac and write to efuse.
**/
int wifi_set_bt_mac_address(u8 *mac)
{
	return rltk_wlan_set_bt_mac_address(mac);
}

/**
Example:
wifi_get_bt_mac_address(mac);
This method is to get the bt mac from efuse.
**/
int wifi_get_bt_mac_address(u8 *mac)
{
	return rltk_wlan_get_bt_mac_address(mac);
}

/**
Example:
wifi_get_ap_dtim(dtim_period);
This method is to get dtim_period from ap beacon info.
**/
int wifi_get_ap_dtim(u8 *dtim_period)
{
	return rltk_wlan_get_ap_dtim(dtim_period);
}

int wifi_csi_config(rtw_csi_action_parm_t *act_param)
{
	int ret = 0;
	ret = rltk_wlan_csi_config(act_param);
	return ret;
}

int wifi_csi_report(u32 buf_len, u8 *csi_buf, u32 *len, rtw_csi_header_t *csi_header)
{
	int ret = 0;
	ret = rltk_wlan_csi_report(buf_len, csi_buf, len, csi_header);
	return ret;
}

#endif	//#if CONFIG_WLAN
