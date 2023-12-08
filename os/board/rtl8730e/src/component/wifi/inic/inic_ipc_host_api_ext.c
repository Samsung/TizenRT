
#include "main.h"
#if CONFIG_LWIP_LAYER
#include <lwip_netconf.h>
#ifndef CONFIG_PLATFORM_TIZENRT_OS
#include <dhcp/dhcps.h>
#endif
#endif

#include <wifi_ind.h>
#include <osdep_service.h>
#include <rtw_timer.h>

#ifdef CONFIG_AS_INIC_AP
#include "inic_ipc_api.h"
#endif
#include "wpa_lite_intf.h"
#include "inic_ipc_host_trx.h"

extern rtw_join_status_t rtw_join_status;

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
	char is_wps_trigger;
};
#endif
/******************************************************
 *               Variables Declarations
 ******************************************************/

#if CONFIG_LWIP_LAYER
extern struct netif xnetif[NET_IF_NUM];
#endif

#if CONFIG_AUTO_RECONNECT
p_wlan_autoreconnect_hdl_t p_wlan_autoreconnect_hdl = NULL;
#endif

ap_channel_switch_callback_t p_ap_channel_switch_callback = NULL;

#if ATCMD_VER == ATVER_2
extern unsigned char dhcp_mode_sta;
#endif

/******************************************************
 *               Variables Declarations
 ******************************************************/
void *param_indicator;
struct task_struct wifi_autoreconnect_task = {0};

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
int wifi_is_connected_to_ap(void)
{
	int ret = 0;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_IS_CONNECTED_TO_AP, NULL, 0);
	return ret;
}

//----------------------------------------------------------------------------//
int wifi_set_channel(unsigned char wlan_idx, u8 channel)
{
	int ret = 0;
	u32 param_buf[2];
	param_buf[0] = (u32)wlan_idx;
	param_buf[1] = (u32)channel;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_CHANNEL, param_buf, 2);
	return ret;
}

int wifi_get_channel(unsigned char wlan_idx, u8 *channel)
{
	int ret = 0;
	u32 param_buf[2];
	u8 *channel_temp = (u8 *)rtw_zmalloc(sizeof(int));

	if (channel_temp == NULL) {
		return -1;
	}

	param_buf[0] = (u32)wlan_idx;
	param_buf[1] = (u32)channel_temp;
	DCache_CleanInvalidate((u32)channel_temp, sizeof(u8));

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_GET_CHANNEL, param_buf, 2);
	DCache_Invalidate((u32)channel_temp, sizeof(int));
	*channel = *channel_temp;
	rtw_mfree((u8 *)channel_temp, 0);

	return ret;
}

//----------------------------------------------------------------------------//

int wifi_get_disconn_reason_code(unsigned short *reason_code)
{
	int ret = 0;
	u32 param_buf[1];
	unsigned short *reason_code_temp = (unsigned short *)rtw_zmalloc(sizeof(unsigned short));

	if (reason_code_temp == NULL) {
		return -1;
	}

	DCache_CleanInvalidate((u32)reason_code_temp, sizeof(unsigned short));
	param_buf[0] = (u32)reason_code_temp;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_GET_DISCONN_REASCON, param_buf, 1);
	DCache_Invalidate((u32)reason_code_temp, sizeof(unsigned short));
	*reason_code = *reason_code_temp;
	rtw_mfree((u8 *)reason_code_temp, 0);
	return ret;
}

int wifi_get_scan_records(unsigned int *AP_num, char *scan_buf)
{
	int ret = 0;
	u32 param_buf[2];

	unsigned int *AP_num_temp = (unsigned int *)rtw_zmalloc(sizeof(unsigned int));
	if (AP_num_temp == NULL) {
		return -1;
	}
	*AP_num_temp = *AP_num;

	char *scan_buf_temp = (char *)rtw_zmalloc((*AP_num) * sizeof(rtw_scan_result_t));
	if (scan_buf_temp == NULL) {
		rtw_free(AP_num_temp);
		return -1;
	}

	param_buf[0] = (u32)AP_num_temp;
	param_buf[1] = (u32)scan_buf_temp;
	DCache_CleanInvalidate((u32)AP_num_temp, sizeof(unsigned int));
	DCache_CleanInvalidate((u32)scan_buf_temp, (*AP_num)*sizeof(rtw_scan_result_t));

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_GET_SCANNED_AP_INFO, param_buf, 2);
	DCache_Invalidate((u32)AP_num_temp, sizeof(unsigned int));
	DCache_Invalidate((u32)scan_buf_temp, (*AP_num)*sizeof(rtw_scan_result_t));
	*AP_num = *AP_num_temp;
	rtw_memcpy(scan_buf, scan_buf_temp, ((*AP_num)*sizeof(rtw_scan_result_t)));

	rtw_mfree((u8 *)AP_num_temp, 0);
	rtw_mfree((u8 *)scan_buf_temp, 0);
	return ret;
}

int wifi_scan_abort(void)
{
	int ret = 0;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SCAN_ABORT, NULL, 0);

	return ret;
}
//----------------------------------------------------------------------------//

int wifi_get_mac_address(int idx, rtw_mac_t *mac, u8 efuse)
{
	int ret = 0;
	u32 param_buf[3];

	rtw_mac_t *mac_temp = (rtw_mac_t *)rtw_zmalloc(sizeof(rtw_mac_t));
	if (mac_temp == NULL) {
		return -1;
	}

	param_buf[0] = idx;
	param_buf[1] = (u32)mac_temp;
	param_buf[2] = efuse;
	DCache_CleanInvalidate((u32)mac_temp, sizeof(rtw_mac_t));
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_GET_MAC_ADDR, param_buf, 3);

	DCache_Invalidate((u32)mac_temp, sizeof(rtw_mac_t));
	rtw_memcpy(mac, mac_temp, sizeof(rtw_mac_t));
	rtw_mfree((u8 *)mac_temp, 0);
	return ret;
}

//----------------------------------------------------------------------------//
int wifi_btcoex_set_ble_scan_duty(u8 duty)
{
	int ret = 0;
	u32 param_buf[1];

	param_buf[0] = (u32)duty;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_COEX_BLE_SET_SCAN_DUTY, param_buf, 1);
	return ret;
}

u8 wifi_driver_is_mp(void)
{
	int ret = 0;

	ret = (u8)inic_ipc_api_host_message_send(IPC_API_WIFI_DRIVE_IS_MP, NULL, 0);
	return ret;
}

//----------------------------------------------------------------------------//
int wifi_get_associated_client_list(rtw_client_list_t *client_list_buffer)
{
	int ret = 0;
	u32 param_buf[1];

	rtw_client_list_t *client_list_buffer_temp = (rtw_client_list_t *)rtw_zmalloc(sizeof(rtw_client_list_t));
	if (client_list_buffer_temp == NULL) {
		return -1;
	}

	param_buf[0] = (u32)client_list_buffer_temp;
	DCache_CleanInvalidate((u32)client_list_buffer_temp, sizeof(rtw_client_list_t));

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_GET_ASSOCIATED_CLIENT_LIST, param_buf, 1);
	DCache_Invalidate((u32)client_list_buffer_temp, sizeof(rtw_client_list_t));
	rtw_memcpy(client_list_buffer, client_list_buffer_temp, sizeof(rtw_client_list_t));
	rtw_mfree((u8 *)client_list_buffer_temp, 0);
	return ret;
}
//----------------------------------------------------------------------------//
int wifi_get_setting(unsigned char wlan_idx, rtw_wifi_setting_t *psetting)
{
	int ret = 0;
	u32 param_buf[2];

	rtw_wifi_setting_t *setting_temp = (rtw_wifi_setting_t *)rtw_zmalloc(sizeof(rtw_wifi_setting_t));
	if (setting_temp == NULL) {
		return -1;
	}

	param_buf[0] = (u32)wlan_idx;
	param_buf[1] = (u32)setting_temp;
	DCache_CleanInvalidate((u32)setting_temp, sizeof(rtw_wifi_setting_t));

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_GET_SETTING, param_buf, 2);
	DCache_Invalidate((u32)setting_temp, sizeof(rtw_wifi_setting_t));
	rtw_memcpy(psetting, setting_temp, sizeof(rtw_wifi_setting_t));
	rtw_mfree((u8 *)setting_temp, 0);

	return ret;
}

int wifi_set_ips_internal(u8 enable)
{
	int ret = 0;
	u32 param_buf[1];

	param_buf[0] = (u32)enable;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_IPS_EN, param_buf, 1);
	return ret;
}

int wifi_set_lps_enable(u8 enable)
{
	int ret = 0;
	u32 param_buf[1];

	param_buf[0] = (u32)enable;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_LPS_EN, param_buf, 1);
	return ret;
}

//----------------------------------------------------------------------------//

int wifi_set_mfp_support(unsigned char value)
{
	int ret = 0;
	u32 param_buf[1];

	param_buf[0] = (u32)value;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_MFP_SUPPORT, param_buf, 1);
	return ret;
}

int wifi_set_group_id(unsigned char value)
{
	rtw_sae_set_user_group_id(value);

	return RTW_SUCCESS;
}

int wifi_set_pmk_cache_enable(unsigned char value)
{
	int ret = 0;
	u32 param_buf[1];

	param_buf[0] = (u32)value;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_PMK_CACHE_EN, param_buf, 1);
	return ret;
}

//----------------------------------------------------------------------------//
int wifi_get_sw_statistic(unsigned char idx, rtw_sw_statistics_t *statistic)
{
	u32 param_buf[2];
	int ret = 0;

	rtw_sw_statistics_t *statistic_temp = (rtw_sw_statistics_t *)rtw_zmalloc(sizeof(rtw_sw_statistics_t));
	if (statistic_temp == NULL) {
		return 0;
	}
	param_buf[0] = (u32)idx;
	param_buf[1] = (u32)statistic_temp;
	DCache_CleanInvalidate((u32)statistic_temp, sizeof(rtw_sw_statistics_t));

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_GET_SW_STATISTIC, param_buf, 2);

	DCache_Invalidate((u32)statistic_temp, sizeof(rtw_sw_statistics_t));
	rtw_memcpy(statistic, statistic_temp, sizeof(rtw_sw_statistics_t));
	rtw_mfree((u8 *)statistic_temp, 0);
	return ret;
}

int wifi_fetch_phy_statistic(rtw_phy_statistics_t *phy_statistic)
{
	u32 param_buf[1];
	int ret = 0;

	rtw_phy_statistics_t *phy_statistic_temp = (rtw_phy_statistics_t *)rtw_zmalloc(sizeof(rtw_phy_statistics_t));
	if (phy_statistic_temp == NULL) {
		return -1;
	}

	param_buf[0] = (u32)phy_statistic_temp;
	DCache_CleanInvalidate((u32)phy_statistic_temp, sizeof(rtw_phy_statistics_t));

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_GET_PHY_STATISTIC, param_buf, 1);

	DCache_Invalidate((u32)phy_statistic_temp, sizeof(rtw_phy_statistics_t));
	rtw_memcpy(phy_statistic, phy_statistic_temp, sizeof(rtw_phy_statistics_t));
	rtw_mfree((u8 *)phy_statistic_temp, 0);
	return ret;
}

int wifi_get_network_mode(void)
{
	return inic_ipc_api_host_message_send(IPC_API_WIFI_GET_NETWORK_MODE, NULL, 0);
}

int wifi_set_network_mode(enum wlan_mode mode)
{
	int ret = 0;
	u32 param_buf[1];

	param_buf[0] = (u32)mode;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_NETWORK_MODE, param_buf, 1);
	return ret;
}

int wifi_set_wps_phase(unsigned char is_trigger_wps)
{
	int ret = 0;
	u32 param_buf[1];
	param_buf[0] = is_trigger_wps;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_WPS_PHASE, param_buf, 1);
	return ret;
}

int wifi_set_gen_ie(unsigned char wlan_idx, char *buf, __u16 buf_len, __u16 flags)
{
	int ret = 0;
	u32 param_buf[4];

	DCache_Clean((u32)buf, (u32)buf_len);
	param_buf[0] = (u32)wlan_idx;
	param_buf[1] = (u32)buf;
	param_buf[2] = (u32)buf_len;
	param_buf[3] = (u32)flags;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_GEN_IE, param_buf, 4);
	return ret;
}

int wifi_set_eap_phase(unsigned char is_trigger_eap)
{
#ifdef CONFIG_EAP
	int ret = 0;
	u32 param_buf[1];
	param_buf[0] = is_trigger_eap;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_EAP_PHASE, param_buf, 1);
	return ret;
#else
	return -1;
#endif
}

unsigned char wifi_get_eap_phase(void)
{
#ifdef CONFIG_EAP
	unsigned char eap_phase = 0;

	eap_phase = (u8)inic_ipc_api_host_message_send(IPC_API_WIFI_GET_EAP_PHASE, NULL, 0);
	return eap_phase;
#else
	return 0;
#endif
}

int wifi_set_eap_method(unsigned char eap_method)
{
#ifdef CONFIG_EAP
	int ret = 0;
	u32 param_buf[1];
	param_buf[0] = eap_method;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_EAP_METHOD, param_buf, 1);
	return ret;
#else
	return -1;
#endif
}

int wifi_if_send_eapol(unsigned char wlan_idx, char *buf, __u16 buf_len, __u16 flags)
{
	int ret = 0;
	u32 param_buf[4];

	DCache_Clean((u32)buf, (u32)buf_len);
	param_buf[0] = (u32)wlan_idx;
	param_buf[1] = (u32)buf;
	param_buf[2] = buf_len;
	param_buf[3] = flags;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SEND_EAPOL, param_buf, 4);
	return ret;
}

static void rtw_autoreconnect_thread(void *param)
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

#ifdef CONFIG_SAE_SUPPORT
#ifdef CONFIG_WPS
	connect_param.is_wps_trigger = reconnect_info->is_wps_trigger;
#endif
#endif
	ret = wifi_connect(&connect_param, 1);
#if CONFIG_LWIP_LAYER
	if (ret == RTW_SUCCESS) {
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
	}
#endif //#if CONFIG_LWIP_LAYER
	param_indicator = NULL;
	rtw_delete_task(&wifi_autoreconnect_task);
#endif

}

#if CONFIG_AUTO_RECONNECT
void rtw_autoreconnect_hdl(rtw_security_t security_type,
						   char *ssid, int ssid_len,
						   char *password, int password_len,
						   int key_id, char is_wps_trigger)
{
	static struct wifi_autoreconnect_param param;
	param_indicator = &param;
	param.security_type = security_type;
	param.ssid = ssid;
	param.ssid_len = ssid_len;
	param.password = password;
	param.password_len = password_len;
	param.key_id = key_id;
	param.is_wps_trigger = is_wps_trigger;

	if (wifi_autoreconnect_task.task != 0) {
#if CONFIG_LWIP_LAYER
		netifapi_dhcp_stop(&xnetif[0]);
#endif
		u32 start_tick = rtw_get_current_time();
		while (1) {
			rtw_msleep_os(2);
			u32 passing_tick = rtw_get_current_time() - start_tick;
			if (rtw_systime_to_sec(passing_tick) >= 2) {
				RTW_API_INFO("\r\n Create wifi_autoreconnect_task timeout \r\n");
				return;
			}

			if (wifi_autoreconnect_task.task == 0) {
				break;
			}
		}
	}

	rtw_create_task(&wifi_autoreconnect_task, (const char *)"wifi_autoreconnect", 512, 1, rtw_autoreconnect_thread, &param);
}
#endif

int wifi_config_autoreconnect(__u8 mode)
{
	int ret = -1;
	u32 param_buf[1];
#if CONFIG_AUTO_RECONNECT
	if (mode == RTW_AUTORECONNECT_DISABLE) {
		p_wlan_autoreconnect_hdl = NULL;
	} else {
		p_wlan_autoreconnect_hdl = rtw_autoreconnect_hdl;
	}

	param_buf[0] = mode;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_CONFIG_AUTORECONNECT, param_buf, 1);
#endif
	return ret;

}

int wifi_get_autoreconnect(__u8 *mode)
{
	int ret = 0;
	u32 param_buf[1];
#if CONFIG_AUTO_RECONNECT
	__u8 *mode_temp = rtw_zmalloc(sizeof(__u8));
	if (mode_temp == NULL) {
		return -1;
	}
	DCache_CleanInvalidate((u32)mode_temp, sizeof(__u8));
	param_buf[0] = (u32)mode_temp;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_GET_AUTORECONNECT, param_buf, 1);
	DCache_Invalidate((u32)mode_temp, sizeof(__u8));
	*mode = *mode_temp;
	rtw_mfree((u8 *)mode_temp, 0);
	return ret;
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
 * rtw_custom_ie_t buf[2] = {{test_1, BEACON},
 *		 {test_2, PROBE_RSP}};
 * u8 buf_test2[] = {221, 2, 1, 3} ;
 * rtw_custom_ie_t buf_update = {buf_test2, PROBE_RSP};
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
 * delete all ie for specific wlan interface
 * static void cmd_del_ie(int argc, char **argv)
 * {
 *	 wifi_del_custom_ie(SOFTAP_WLAN_INDEX);
 * }
 */
int wifi_add_custom_ie(void *cus_ie, int ie_num)
{
	int ret = 0;
	u32 param_buf[3];
	u8 ie_len = 0;
	int cnt = 0;

	p_rtw_custom_ie_t pcus_ie = cus_ie;
	for (cnt = 0; cnt < ie_num; cnt++) {
		rtw_custom_ie_t ie_t = *(pcus_ie + cnt);
		ie_len = ie_t.ie[1];
		DCache_Clean((u32)ie_t.ie, (u32)(ie_len + 2));
	}
	DCache_Clean((u32)cus_ie, ie_num * sizeof(rtw_custom_ie_t));
	param_buf[0] = 0;//type 0 means add
	param_buf[1] = (u32)cus_ie;
	param_buf[2] = ie_num;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_CUS_IE, param_buf, 3);
	return ret;
}

int wifi_update_custom_ie(void *cus_ie, int ie_index)
{
	int ret = 0;
	u32 param_buf[3];
	u8 ie_len = 0;


	rtw_custom_ie_t ie_t = *(p_rtw_custom_ie_t)(cus_ie);
	ie_len = *(u8 *)(ie_t.ie + 1);
	DCache_Clean((u32)ie_t.ie, (u32)ie_len);

	DCache_Clean((u32)cus_ie, sizeof(rtw_custom_ie_t));
	param_buf[0] = 1;//type 1 means update
	param_buf[1] = (u32)cus_ie;
	param_buf[2] = ie_index;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_CUS_IE, param_buf, 3);
	return ret;
}

int wifi_del_custom_ie(unsigned char wlan_idx)
{
	u32 param_buf[2];

	param_buf[0] = 2;//type 2 means delete
	param_buf[1] = (u32)wlan_idx;
	return inic_ipc_api_host_message_send(IPC_API_WIFI_CUS_IE, param_buf, 2);
}
//----------------------------------------------------------------------------//
void wifi_set_indicate_mgnt(int enable)
{
	u32 param_buf[1];
	param_buf[0] = (u32)enable;
	inic_ipc_api_host_message_send(IPC_API_WIFI_SET_IND_MGNT, param_buf, 1);
}

int wifi_send_mgnt(raw_data_desc_t *raw_data_desc)
{
	int ret = 0;
	u32 param_buf[1];

	DCache_Clean((u32)raw_data_desc, sizeof(raw_data_desc_t));
	DCache_Clean((u32)raw_data_desc->buf, (u32)raw_data_desc->buf_len);
	param_buf[0] = (u32)raw_data_desc;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SEND_MGNT, param_buf, 1);
	return ret;
}

int wifi_set_tx_rate_by_ToS(unsigned char enable, unsigned char ToS_precedence, unsigned char tx_rate)
{
	int ret = 0;
	u32 param_buf[3];

	param_buf[0] = (u32)enable;
	param_buf[1] = (u32)ToS_precedence;
	param_buf[2] = (u32)tx_rate;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_TXRATE_BY_TOS, param_buf, 3);
	return ret;
}

int wifi_set_EDCA_param(unsigned int AC_param)
{
	int ret = 0;
	u32 param_buf[1];

	param_buf[0] = AC_param;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_EDCA_PARAM, param_buf, 1);
	return ret;
}

int wifi_set_TX_CCA(unsigned char enable)
{
	int ret = 0;
	u32 param_buf[1];

	param_buf[0] = enable;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_TX_CCA, param_buf, 1);
	return ret;
}

int wifi_set_cts2self_duration_and_send(unsigned char wlan_idx, unsigned short duration)
{
	int ret = 0;
	u32 param_buf[2];

	param_buf[0] = (u32)wlan_idx;
	param_buf[1] = (u32)duration;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_CTS2SEFL_DUR_AND_SEND, param_buf, 2);
	return ret;

}
int wifi_init_mac_filter(void)
{
	u32 param_buf[1];

	param_buf[0] = 0;//type 0 means init
	return inic_ipc_api_host_message_send(IPC_API_WIFI_MAC_FILTER, param_buf, 1);
}

int wifi_add_mac_filter(unsigned char *hwaddr)
{
	int ret = 0;
	u32 param_buf[2];

	DCache_Clean((u32)hwaddr, ETH_ALEN);
	param_buf[0] = 1;//type 1 means add
	param_buf[1] = (u32)hwaddr;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_MAC_FILTER, param_buf, 2);
	return ret;
}

int wifi_del_mac_filter(unsigned char *hwaddr)
{
	int ret = 0;
	u32 param_buf[2];

	DCache_Clean((u32)hwaddr, ETH_ALEN);
	param_buf[0] = 2;//type 2 means delete
	param_buf[1] = (u32)hwaddr;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_MAC_FILTER, param_buf, 2);
	return ret;
}

int wifi_get_antenna_info(unsigned char *antenna)
{
#ifdef CONFIG_ANTENNA_DIVERSITY
	int ret = 0;
	u32 param_buf[1];

	DCache_Clean((u32)antenna, sizeof(unsigned char));
	param_buf[0] = (u32)antenna;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_GET_ANTENNA_INFO, param_buf, 1);
	DCache_Invalidate((u32)antenna, sizeof(unsigned char));

	return ret;
#else
	UNUSED(antenna);
	return -1;
#endif
}

/*
 * @brief get WIFI band type
 *@retval  the support band type.
 * 	WL_BAND_2_4G: only support 2.4G
 *	WL_BAND_5G: only support 5G
 *      WL_BAND_2_4G_5G_BOTH: support both 2.4G and 5G
 */
WL_BAND_TYPE wifi_get_band_type(void)
{
	u8 ret;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_GET_BAND_TYPE, NULL, 0);

	if (ret == 0) {
		return WL_BAND_2_4G;
	} else if (ret == 1) {
		return WL_BAND_5G;
	} else {
		return WL_BAND_2_4G_5G_BOTH;
	}
}

int wifi_get_auto_chl(unsigned char wlan_idx, unsigned char *channel_set, unsigned char channel_num)
{
	int ret = 0;
	u32 param_buf[3];

	unsigned char *channel_set_temp = rtw_zmalloc(channel_num);
	if (channel_set_temp == NULL) {
		return -1;
	}
	rtw_memcpy(channel_set_temp, channel_set, (u32)channel_num);

	param_buf[0] = (u32)wlan_idx;
	param_buf[1] = (u32)channel_set_temp;
	param_buf[2] = (u32)channel_num;
	DCache_Clean((u32)channel_set_temp, (u32)channel_num);
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_GET_AUTO_CHANNEL, param_buf, 3);
	rtw_mfree((u8 *)channel_set_temp, 0);
	return ret;
}

int wifi_del_station(unsigned char wlan_idx, unsigned char *hwaddr)
{
	int ret = 0;
	u32 param_buf[2];

	DCache_Clean((u32)hwaddr, ETH_ALEN);
	param_buf[0] = (u32)wlan_idx;
	param_buf[1] = (u32)hwaddr;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_DEL_STA, param_buf, 2);
	return ret;

}

int wifi_ap_switch_chl_and_inform(rtw_csa_parm_t *csa_param)
{
	int ret = 0;
	u32 param_buf[3];
	p_ap_channel_switch_callback = csa_param->callback;

	param_buf[0] = (u32)csa_param;
	DCache_Clean((u32)csa_param, sizeof(rtw_csa_parm_t));
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_AP_CH_SWITCH, param_buf, 1);
	DCache_Invalidate((u32)csa_param, sizeof(rtw_csa_parm_t));
	return ret;
}

void wifi_set_no_beacon_timeout(unsigned char timeout_sec)
{
	u32 param_buf[1];

	param_buf[0] = (u32)timeout_sec;
	inic_ipc_api_host_message_send(IPC_API_WIFI_SET_NO_BEACON_TIMEOUT, param_buf, 1);
}

u64 wifi_get_tsf(unsigned char port_id)
{
	return inic_ipc_host_get_wifi_tsf(port_id);
}

int wifi_get_txbuf_pkt_num(void)
{
	return inic_ipc_host_get_txbuf_pkt_num();
}

//----------------------------------------------------------------------------//
int wifi_csi_config(rtw_csi_action_parm_t *act_param)
{
	int ret = 0;
	u32 param_buf[1];

	param_buf[0] = (u32)act_param;
	DCache_Clean((u32)act_param, sizeof(rtw_csi_action_parm_t));
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_CONFIG_CSI, param_buf, 1);
	DCache_Invalidate((u32)act_param, sizeof(rtw_csi_action_parm_t));
	return ret;
}

int wifi_csi_report(u32 buf_len, u8 *csi_buf, u32 *len)
{
	int ret = 0;
	u32 param_buf[3];

	void *csi_buf_temp = rtw_zmalloc(buf_len);
	if (csi_buf_temp == NULL) {
		return -1;
	}

	u32 *len_temp = (u32 *)rtw_zmalloc(sizeof(u32));
	if (len_temp == NULL) {
		rtw_mfree((u8 *)csi_buf_temp, 0);
		return -1;
	}

	param_buf[0] = (u32)csi_buf_temp;
	param_buf[1] = (u32)buf_len;
	param_buf[2] = (u32)len_temp;
	DCache_CleanInvalidate((u32)csi_buf_temp, buf_len);
	DCache_CleanInvalidate((u32)len_temp, sizeof(u32));

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_GET_CSI_REPORT, param_buf, 3);
	DCache_Invalidate((u32)csi_buf_temp, buf_len);
	rtw_memcpy(csi_buf, csi_buf_temp, buf_len);

	DCache_Invalidate((u32)len_temp, sizeof(u32));
	rtw_memcpy(len, len_temp, sizeof(u32));

	rtw_mfree((u8 *)csi_buf_temp, 0);
	rtw_mfree((u8 *)len_temp, 0);
	return ret;
}
//----------------------------------------------------------------------------//

void wifi_btcoex_set_pta(pta_type_t type, u8 role, u8 process)
{
	u32 param_buf[3];

	param_buf[0] = (u32)type;
	param_buf[1] = (u32)role;
	param_buf[2] = (u32)process;
	inic_ipc_api_host_message_send(IPC_API_WIFI_COEX_SET_PTA, param_buf, 3);
}

void wifi_btcoex_set_bt_ant(u8 bt_ant)
{
	u32 param_buf[1];

	param_buf[0] = (u32)bt_ant;
	inic_ipc_api_host_message_send(IPC_API_WIFI_SET_BT_SEL, param_buf, 1);
}

int wifi_set_wpa_mode(rtw_wpa_mode wpa_mode)
{
	u32 param_buf[1];
	int ret = 0;

	param_buf[0] = (u32)wpa_mode;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_WPA_MODE, param_buf, 1);
	return ret;
}

int wifi_set_pmf_mode(u8 pmf_mode)
{
	u32 param_buf[1];
	int ret = 0;

	param_buf[0] = (u32)pmf_mode;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_PMF_MODE, param_buf, 1);
	return ret;
}

int wifi_btcoex_bt_rfk(struct bt_rfk_param *rfk_param)
{
	int ret = 0;
	u32 param_buf[1];

	if (rfk_param == NULL) {
		return _FAIL;
	}

	param_buf[0] = (u32)rfk_param;

	DCache_Clean((u32)rfk_param, sizeof(struct bt_rfk_param));
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_COEX_BT_RFK, param_buf, 1);
	DCache_Invalidate((u32)rfk_param, sizeof(struct bt_rfk_param));

	return ret;
}

int wifi_zigbee_coex_zb_rfk(void)
{
	int ret = 0;
	inic_ipc_api_host_message_send(IPC_API_WIFI_COEX_ZB_RFK, NULL, 0);
	return ret;
}

void wifi_wpa_sta_4way_fail_notify(void)
{
	inic_ipc_api_host_message_send(IPC_API_WPA_4WAY_FAIL, NULL, 0);
}

void wifi_wpa_add_key(struct rtw_crypt_info *crypt)
{
	u32 param_buf[1] = {0};

	DCache_Clean((u32)crypt, sizeof(struct rtw_crypt_info));
	param_buf[0] = (u32)crypt;
	inic_ipc_api_host_message_send(IPC_API_WIFI_ADD_KEY, param_buf, 1);
}

void wifi_wpa_pmksa_ops(struct rtw_pmksa_ops_t *pmksa_ops)
{
	u32 param_buf[1] = {0};

	DCache_Clean((u32)pmksa_ops, sizeof(struct rtw_pmksa_ops_t));
	param_buf[0] = (u32)pmksa_ops;
	inic_ipc_api_host_message_send(IPC_API_WPA_PMKSA_OPS, param_buf, 1);
}

int wifi_sae_status_indicate(u8 wlan_idx, u16 status, u8 *mac_addr)
{
	u32 param_buf[3] = {0};

	param_buf[0] = (u32)wlan_idx;
	param_buf[1] = (u32)status;
	param_buf[2] = (u32)mac_addr;

	if (mac_addr) {
		DCache_Clean((u32)mac_addr, 6);
	}
	inic_ipc_api_host_message_send(IPC_API_WIFI_SAE_STATUS, param_buf, 3);
	return 0;
}

int wifi_send_raw_frame(struct raw_frame_desc_t *raw_frame_desc)
{
	int ret;
	int idx = 0;
	struct skb_raw_para raw_para;

	struct eth_drv_sg sg_list[2];
	int sg_len = 0;

	if (raw_frame_desc == NULL) {
		return -1;
	}

	raw_para.rate = raw_frame_desc->tx_rate;
	raw_para.retry_limit = raw_frame_desc->retry_limit;
	idx = raw_frame_desc->wlan_idx;

	sg_list[sg_len].buf = (unsigned int)raw_frame_desc->buf;
	sg_list[sg_len++].len = raw_frame_desc->buf_len;
	ret = inic_ipc_host_send(idx, sg_list, sg_len, raw_frame_desc->buf_len, &raw_para);

	return ret;
}

#endif	//#if CONFIG_WLAN

