//----------------------------------------------------------------------------//
//#include <flash/stm32_flash.h>
#include "main.h"
#if CONFIG_LWIP_LAYER
#include <lwip_netconf.h>
//#include <dhcp/dhcps.h>
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

#if defined(CONFIG_AS_INIC_AP)
#include "inic_ipc_api.h"
#include "freertos/wrapper.h"
#endif
/******************************************************
 *                    Constants
 ******************************************************/
#define RTW_JOIN_TIMEOUT 20000 //INIC_IPC_API_TODO
/******************************************************
 *               Variables Declarations
 ******************************************************/
#if CONFIG_LWIP_LAYER
extern struct netif xnetif[NET_IF_NUM];
#endif

/******************************************************
 *               Variables Definitions
 ******************************************************/
static internal_join_block_param_t *join_block_param = NULL;

rtw_result_t (*scan_user_callback_ptr)(unsigned int, void *) = NULL;
rtw_result_t (*scan_each_report_user_callback_ptr)(rtw_scan_result_t *, void *) = NULL;

extern rtw_mode_t wifi_mode;
rtw_join_status_t rtw_join_status;
rtw_joinstatus_callback_t p_wifi_joinstatus_user_callback = NULL;
rtw_joinstatus_callback_t p_wifi_joinstatus_internal_callback = NULL;

wifi_do_fast_connect_ptr p_wifi_do_fast_connect = NULL;
write_fast_connect_info_ptr p_store_fast_connect_info = NULL;
/* Give default value if not defined */
/******************************************************
 *               Function Definitions
 ******************************************************/

#if CONFIG_WLAN
//----------------------------------------------------------------------------//
#if defined(CONFIG_PLATFORM_TIZENRT_OS)
#include "rtk_wifi_utils.h"
unsigned char ap_bssid[ETH_ALEN];
static rtk_network_link_callback_t g_link_up = NULL;
static rtk_network_link_callback_t g_link_down = NULL;


typedef void (*rtk_network_link_callback_t)(rtk_reason_t *reason);
int8_t WiFiRegisterLinkCallback(rtk_network_link_callback_t link_up, rtk_network_link_callback_t link_down)
{
	if (!g_link_up) {
		g_link_up = link_up;
	}
	if (!g_link_down) {
		g_link_down = link_down;
	}

	return RTK_STATUS_SUCCESS;
}
extern void linkup_handler(rtk_reason_t *reason);
extern void linkdown_handler(rtk_reason_t *reason);
#endif

static void _wifi_join_status_indicate(rtw_join_status_t join_status)
{
	/* step 1: internal process for wifi_connect*/
	if (join_status == RTW_JOINSTATUS_SUCCESS) {
#if CONFIG_LWIP_LAYER
		LwIP_netif_set_link_up(0);
#endif

		/* if not use fast dhcp, store fast connect info to flash when connect successfully*/
#if defined(CONFIG_FAST_DHCP) && CONFIG_FAST_DHCP == 0
		if (p_store_fast_connect_info) {
			p_store_fast_connect_info(0, 0);
		}
#endif
		/* if Synchronized connect, up sema when connect success*/
		if (join_block_param && join_block_param->block) {
			rtw_up_sema(&join_block_param->join_sema);
		}
	}

	if (join_status == RTW_JOINSTATUS_FAIL) {
		/* if blocking connection, up sema when connect fail*/
		if (join_block_param && join_block_param->block) {
			rtw_up_sema(&join_block_param->join_sema);
		}
	}

	if (join_status == RTW_JOINSTATUS_DISCONNECT) {
#if CONFIG_LWIP_LAYER
		LwIP_DHCP_stop(0);
		LwIP_netif_set_link_down(0);
#endif
#if defined(CONFIG_PLATFORM_TIZENRT_OS)
		rtk_reason_t reason;
		memset(&reason, 0, sizeof(rtk_reason_t));

		if (g_link_down) {
			nvdbg("RTK_API %s send link_down\n",__func__);
			g_link_down(&reason);
		}
#endif
	}

	rtw_join_status = join_status;

	/* step 2: execute user callback to process join_status*/
	if (p_wifi_joinstatus_user_callback) {
		p_wifi_joinstatus_user_callback(join_status);
	}
}

int wifi_connect(rtw_network_info_t *connect_param, unsigned char block)
{
	rtw_result_t result = RTW_SUCCESS;
	internal_join_block_param_t *block_param = NULL;
	u32 param_buf[1] = {0};

#if defined(CONFIG_PLATFORM_TIZENRT_OS)
		rtk_reason_t reason;
#endif

	if (connect_param == NULL) {
		RTW_API_INFO("\nwifi connect param not set!");
		return RTW_ERROR;
	}

	/* step1: check if there's ongoing connect*/
	if ((rtw_join_status > RTW_JOINSTATUS_UNKNOWN) && (rtw_join_status < RTW_JOINSTATUS_SUCCESS)) {
		RTW_API_INFO("\nthere is ongoing wifi connect!");
		return RTW_BUSY;
	}

	p_wifi_joinstatus_user_callback = connect_param->joinstatus_user_callback;
	p_wifi_joinstatus_internal_callback = _wifi_join_status_indicate;

	/*clear for last connect status */
	rtw_join_status = RTW_JOINSTATUS_STARTING;
	_wifi_join_status_indicate(RTW_JOINSTATUS_STARTING);

	/* step2: malloc and set synchronous connection related variables*/
	if (block) {
		block_param = (internal_join_block_param_t *)rtw_zmalloc(sizeof(internal_join_block_param_t));
		if (!block_param) {
			result = (rtw_result_t) RTW_NOMEM;
			rtw_join_status = RTW_JOINSTATUS_FAIL;
			goto error;
		}
		block_param->block = block;
		rtw_init_sema(&block_param->join_sema, 0);
		if (!block_param->join_sema) {
			result = (rtw_result_t) RTW_NOMEM;
			rtw_join_status = RTW_JOINSTATUS_FAIL;
			goto error;
		}

	}

	/* step3: set connect cmd to driver*/
	if (connect_param->password_len) {
		DCache_Clean((u32)connect_param->password, connect_param->password_len);
	}
	DCache_Clean((u32)connect_param, sizeof(rtw_network_info_t));
	param_buf[0] = (u32)connect_param;
	result = inic_ipc_api_host_message_send(IPC_API_WIFI_CONNECT, param_buf, 1);

	if (result != RTW_SUCCESS) {
		rtw_join_status = RTW_JOINSTATUS_FAIL;
		goto error;
	}

	/* step4: wait connect finished for synchronous connection*/
	if (block) {
		join_block_param = block_param;

#ifdef CONFIG_ENABLE_EAP
		// for eap connection, timeout should be longer (default value in wpa_supplicant: 60s)
		if (wifi_get_eap_phase()) {
			block_param->join_timeout = 60000;
		} else
#endif
			block_param->join_timeout = RTW_JOIN_TIMEOUT;

		if (rtw_down_timeout_sema(&block_param->join_sema, block_param->join_timeout) == RTW_FALSE) {
			RTW_API_INFO("RTW API: Join bss timeout\r\n");
			rtw_join_status = RTW_JOINSTATUS_FAIL;
			result = RTW_TIMEOUT;
			goto error;
		} else {
			if (wifi_is_connected_to_ap() != RTW_SUCCESS) {
				result = RTW_ERROR;
				rtw_join_status = RTW_JOINSTATUS_FAIL;
				goto error;
			}

#if defined(CONFIG_PLATFORM_TIZENRT_OS)
			memset(&reason, 0, sizeof(rtk_reason_t));
			reason.reason_code = RTK_STATUS_SUCCESS;

			if (g_link_up) {
				if (reason.reason_code) {
					nvdbg("reason.reason_code=%d\n", reason.reason_code);
				}
				printf("RTK_API %s() send link_up\n", __func__);
				g_link_up(&reason);
			}
#endif
		}
	}

error:
	if (block_param) {
		if (block_param->join_sema) {
			rtw_free_sema(&block_param->join_sema);
		}
		rtw_free((u8 *)block_param);
		join_block_param = NULL;
	}

	if (rtw_join_status == RTW_JOINSTATUS_FAIL) {
		_wifi_join_status_indicate(RTW_JOINSTATUS_FAIL);
	}

	return result;
}
//----------------------------------------------------------------------------//


int wifi_disconnect(void)
{
	int ret = 0;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_DISCONNECT, NULL, 0);
#if defined(CONFIG_PLATFORM_TIZENRT_OS)
	rtk_reason_t dummy_reason;
	memset(&dummy_reason, 0, sizeof(rtk_reason_t));
	if (g_link_down) {
		nvdbg("RTK_API rtk_handle_disconnect send link_down\n");
		g_link_down(&dummy_reason); //dummy_reason was not processed in _wt_sta_disconnected callback in TizenRT
	}
#endif
	return ret;
}

//----------------------------------------------------------------------------//
int wifi_is_connected_to_ap(void)
{
	int ret = 0;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_IS_CONNECTED_TO_AP, NULL, 0);
	return ret;
}

//----------------------------------------------------------------------------//
int wifi_is_running(unsigned char wlan_idx)
{
	int ret;
	u32 param_buf[1];
	param_buf[0] = wlan_idx;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_IS_RUNNING, param_buf, 1);
	return ret;
}

//----------------------------------------------------------------------------//
int wifi_set_channel(int channel)
{
	int ret = 0;
	u32 param_buf[1];
	param_buf[0] = channel;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_CHANNEL, param_buf, 1);
	return ret;
}

int wifi_get_channel(int *channel)
{
	int ret = 0;
	u32 param_buf[1];
	int *channel_temp = (int *)rtw_malloc(sizeof(int));

	if (channel_temp == NULL) {
		return -1;
	}

	param_buf[0] = (u32)channel_temp;
	DCache_CleanInvalidate((u32)channel_temp, sizeof(int));

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_GET_CHANNEL, param_buf, 1);
	DCache_Invalidate((u32)channel_temp, sizeof(int));
	*channel = *channel_temp;
	rtw_mfree((u8 *)channel_temp, 0);

	return ret;
}

void wifi_set_user_config(void)
{
	u32 param_buf[1];
	struct wifi_user_conf *p_wifi_user_config = (struct wifi_user_conf *)rtw_zmalloc(sizeof(struct wifi_user_conf));
	if (!p_wifi_user_config) {
		DBG_ERR("wifi set user config fail\n");
		return;
	}

	//below items for user config
	/* adaptivity */
	p_wifi_user_config->rtw_adaptivity_en = DISABLE;
	p_wifi_user_config->rtw_adaptivity_mode = 0; //0 : RTW_ADAPTIVITY_MODE_NORMAL,1: RTW_ADAPTIVITY_MODE_CARRIER_SENSE
	/* trp */
	p_wifi_user_config->rtw_tx_pwr_lmt_enable = 2;	// 0: disable, 1: enable, 2: Depend on efuse(flash)
	p_wifi_user_config->rtw_tx_pwr_by_rate	= 2;	// 0: disable, 1: enable, 2: Depend on efuse(flash)
	p_wifi_user_config->rtw_trp_tis_cert_en = RTW_TRP_TIS_DISABLE;

	p_wifi_user_config->rtw_powersave_en = 1;

#ifdef CONFIG_SAE_SUPPORT
	// set to 'ENABLE' when using WPA3
	p_wifi_user_config->rtw_cmd_tsk_spt_wap3 = ENABLE;
#endif

	p_wifi_user_config->g_user_ap_sta_num = NUM_STA;//NUM_STA (2 + AP_STA_NUM)

	/* power save */
	p_wifi_user_config->lps_dtim = 0;
	p_wifi_user_config->lps_enter_threshold = 0; // LPS_THRESH_PKT_COUNT

	p_wifi_user_config->rtw_power_mgnt = PS_MODE_MIN;
#if defined(CONFIG_LPS_PG)
	p_wifi_user_config->rtw_lps_level = LPS_PG;
#else
	p_wifi_user_config->rtw_lps_level = LPS_NORMAL;
#endif
#ifdef CONFIG_PLATFORM_8195BHP
	p_wifi_user_config->rtw_lps_level = LPS_NORMAL;
	p_wifi_user_config->rtw_power_mgnt = PS_MODE_MAX;//PS_MODE_MIN
#endif
	p_wifi_user_config->rtw_ps_timeout = 20;// default: 20
	p_wifi_user_config->smart_ps = 2;

	/* AP */
	p_wifi_user_config->bForwardingDisabled = 0;

	p_wifi_user_config->bAcceptAddbaReq = (u8)_TRUE; // 0:Reject AP's Add BA req, 1:Accept AP's Add BA req.
	p_wifi_user_config->bIssueAddbaReq = (u8)_TRUE;

	p_wifi_user_config->bCheckDestAddress = (u8)_TRUE;

	p_wifi_user_config->ap_compatibilty_enabled = 0x0B;

#ifdef CONFIG_LAYER2_ROAMING
	p_wifi_user_config->max_roaming_times = 2;
#endif

#ifdef CONFIG_ENABLE_AP_POLLING_CLIENT_ALIVE
	p_wifi_user_config->ap_polling_sta = 1;
#else
	p_wifi_user_config->ap_polling_sta = 0;
#endif

	p_wifi_user_config->channel_plan = 0;

	DCache_Clean((u32)p_wifi_user_config, sizeof(struct wifi_user_conf));
	param_buf[0] = (u32)p_wifi_user_config;
	inic_ipc_api_host_message_send(IPC_API_WIFI_SET_USR_CFG, param_buf, 1);
	rtw_mfree((u8 *)p_wifi_user_config, 0);
}

int wifi_get_disconn_reason_code(unsigned short *reason_code)
{
	int ret = 0;
	u32 param_buf[1];
	unsigned short *reason_code_temp = (unsigned short *)rtw_malloc(sizeof(unsigned short));

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

rtw_join_status_t wifi_get_join_status(void)
{
	return rtw_join_status;
}

int wifi_on(rtw_mode_t mode)
{
	int ret = 1;
	u32 param_buf[1];

#if defined(CONFIG_PLATFORM_TIZENRT_OS)
	ret = WiFiRegisterLinkCallback(&linkup_handler, &linkdown_handler);
	if (ret != RTK_STATUS_SUCCESS) {
		printf("[RTK] Link callback handles: register failed !\n");
		return -1;
	} else {
		printf("[RTK] Link callback handles: registered\n");
	}
#endif

	wifi_set_user_config();

	param_buf[0] = mode;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_ON, param_buf, 1);
	init_timer_wrapper();

	if (p_wifi_do_fast_connect && (mode == RTW_MODE_STA)) {
		p_wifi_do_fast_connect();
	}

	if (ret == RTW_SUCCESS) { //wifi on success
#if CONFIG_LWIP_LAYER
		LwIP_netif_set_up(0);
		if (mode == RTW_MODE_AP) {
			LwIP_netif_set_link_up(0);
		} else if (mode == RTW_MODE_STA_AP) {
			LwIP_netif_set_up(1);
			LwIP_netif_set_link_up(1);
		}
#endif
	}

	return ret;
}

int wifi_off(void)
{
	int ret = 0;

	if ((wifi_is_running(WLAN0_IDX) == 0) &&
		(wifi_is_running(WLAN1_IDX) == 0)) {
		RTW_API_INFO("\n\rWIFI is not running");
		return 0;
	}

#if CONFIG_LWIP_LAYER
#if !defined(CONFIG_PLATFORM_TIZENRT_OS)
	dhcps_deinit();
	LwIP_DHCP(0, DHCP_STOP);
#endif
	LwIP_netif_set_down(0);
	LwIP_netif_set_down(1);
#endif

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_OFF, NULL, 0);
	deinit_timer_wrapper();

	return ret;
}

int wifi_set_mode(rtw_mode_t mode)
{
	int ret = 0;
	u32 param_buf[1];
#ifdef CONFIG_WLAN_SWITCH_MODE
	rtw_mode_t curr_mode, next_mode;
#if defined(CONFIG_AUTO_RECONNECT) && CONFIG_AUTO_RECONNECT
	u8 autoreconnect_mode;
#endif
#endif
	device_mutex_lock(RT_DEV_LOCK_WLAN);//INIC_IPC_API_TODO

	if ((wifi_is_running(WLAN0_IDX) == 0) &&
		(wifi_is_running(WLAN1_IDX) == 0)) {
		RTW_API_INFO("\n\r[%s] WIFI is not running", __FUNCTION__);
		device_mutex_unlock(RT_DEV_LOCK_WLAN);
		return -1;
	}

#ifdef CONFIG_WLAN_SWITCH_MODE
#if defined(CONFIG_AUTO_RECONNECT) && CONFIG_AUTO_RECONNECT
	wifi_get_autoreconnect(&autoreconnect_mode);
	if (autoreconnect_mode != RTW_AUTORECONNECT_DISABLE) {
		wifi_config_autoreconnect(RTW_AUTORECONNECT_DISABLE, 0, 0);

		// if set to AP mode, delay until the autoconnect task is finished
		if ((mode == RTW_MODE_AP) || (mode == RTW_MODE_STA_AP)) {
			while (param_indicator != NULL) {
				rtw_msleep_os(2);
			}
		}
	}
#endif
#endif

	param_buf[0] = mode;
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SET_MODE, param_buf, 1);

	if (ret == RTW_SUCCESS) {
		if ((wifi_mode == RTW_MODE_STA) && (mode == RTW_MODE_AP)) {
#if CONFIG_LWIP_LAYER
			LwIP_netif_set_link_up(0);
#endif
			wifi_mode = mode;
		} else if ((wifi_mode == RTW_MODE_AP) && (mode == RTW_MODE_STA)) {
#if CONFIG_LWIP_LAYER
			LwIP_netif_set_link_down(0);
#endif
			wifi_mode = mode;
		} else if ((wifi_mode == RTW_MODE_AP) && (mode == RTW_MODE_PROMISC)) {
#if CONFIG_LWIP_LAYER
			LwIP_netif_set_link_down(0);
#endif
			wifi_mode = mode;
		}
#ifdef CONFIG_CONCURRENT_MODE
#ifdef CONFIG_WLAN_SWITCH_MODE
		else if ((wifi_mode == RTW_MODE_STA_AP) && (mode == RTW_MODE_STA)) {
#if CONFIG_LWIP_LAYER
			dhcps_deinit();
			LwIP_netif_set_down(1);
			LwIP_netif_set_link_down(1);
#endif
			wifi_mode = RTW_MODE_STA;
		} else if ((wifi_mode == RTW_MODE_STA) && (mode == RTW_MODE_STA_AP)) {
#if CONFIG_LWIP_LAYER
			dhcps_init(&xnetif[1]);
			LwIP_netif_set_up(1);
			LwIP_netif_set_link_up(1);
#endif
			wifi_mode = mode;
		}
#endif
#endif

#ifdef CONFIG_WLAN_SWITCH_MODE
#if defined(CONFIG_AUTO_RECONNECT) && CONFIG_AUTO_RECONNECT
		/* enable auto reconnect */
		if (autoreconnect_mode != RTW_AUTORECONNECT_DISABLE) {
			wifi_config_autoreconnect(autoreconnect_mode, AUTO_RECONNECT_COUNT, AUTO_RECONNECT_INTERVAL);
		}
#endif
#endif
		device_mutex_unlock(RT_DEV_LOCK_WLAN);
		return ret;
	} else {
#ifdef CONFIG_WLAN_SWITCH_MODE
#if defined(CONFIG_AUTO_RECONNECT) && CONFIG_AUTO_RECONNECT
		/* enable auto reconnect */
		if (autoreconnect_mode != RTW_AUTORECONNECT_DISABLE) {
			wifi_config_autoreconnect(autoreconnect_mode, AUTO_RECONNECT_COUNT, AUTO_RECONNECT_INTERVAL);
		}
#endif
#endif
		device_mutex_unlock(RT_DEV_LOCK_WLAN);
		return -1;
	}
}

int wifi_start_ap(rtw_softap_info_t *softAP_config)
{
	int ret = 0;
	u32 param_buf[1];

	DCache_Clean((u32)softAP_config->password, softAP_config->password_len);
	DCache_Clean((u32)softAP_config, sizeof(rtw_softap_info_t));
	param_buf[0] = (u32)softAP_config;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_START_AP, param_buf, 1);

	if (ret == RTW_SUCCESS) {
#if defined(CONFIG_ENABLE_WPS_AP) && CONFIG_ENABLE_WPS_AP
		const char *ifname = WLAN0_NAME;
		wpas_wps_init(ifname);
#endif
#if CONFIG_LWIP_LAYER
		if (wifi_mode == RTW_MODE_STA_AP) {
			LwIP_netif_set_link_up(1);
		} else {
			LwIP_netif_set_link_up(0);
		}
#endif
	}

	return ret;
}


int wifi_scan_networks(rtw_scan_param_t *scan_param, unsigned char block)
{
	assert_param(scan_param);
	int ret = 0;
	u32 param_buf[3];

	/* lock 2s to forbid suspend under scan */
	rtw_wakelock_timeout(2 * 1000);
	scan_user_callback_ptr = scan_param->scan_user_callback;
	scan_each_report_user_callback_ptr = scan_param->scan_report_each_mode_user_callback;

	if (scan_param->ssid) {
		DCache_Clean((u32)scan_param->ssid, strlen(scan_param->ssid));
	}
	if (scan_param->channel_list) {
		DCache_Clean((u32)scan_param->channel_list, scan_param->channel_list_num);
	}
	DCache_Clean((u32)scan_param, sizeof(rtw_scan_param_t));
	param_buf[0] = (u32)scan_param;
	param_buf[1] = block;
	if (scan_param->ssid) {
		param_buf[2] = strlen(scan_param->ssid);
	} else {
		param_buf[2] = 0;
	}

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_SCAN_NETWROKS, param_buf, 3);
	return ret;
}

int wifi_get_scan_records(unsigned int *AP_num, char *scan_buf)
{
	int ret = 0;
	u32 param_buf[2];

	unsigned int *AP_num_temp = (unsigned int *)rtw_malloc(sizeof(unsigned int));
	if (AP_num_temp == NULL) {
		return -1;
	}
	*AP_num_temp = *AP_num;

	char *scan_buf_temp = (char *)rtw_malloc((*AP_num) * sizeof(rtw_scan_result_t));
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

#endif	//#if CONFIG_WLAN
