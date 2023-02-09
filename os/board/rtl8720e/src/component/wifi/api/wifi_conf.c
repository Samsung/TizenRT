//----------------------------------------------------------------------------//
//#include <flash/stm32_flash.h>
#if !defined(CONFIG_MBED_ENABLED) && !defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
#include "main.h"
#if defined(CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
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
#include "atcmd_wifi.h"
#endif
#if defined(CONFIG_PLATFORM_8721D) || defined(CONFIG_PLATFORM_AMEBAD2) || defined(CONFIG_PLATFORM_8735B) || defined(CONFIG_PLATFORM_AMEBALITE) || defined(CONFIG_PLATFORM_AMEBADPLUS)
#include "platform_opts_bt.h"
#endif

#if defined(CONFIG_AS_INIC_NP)
#include "inic_ipc_api.h"
#endif

void stop_bss_network(void);
u8 rtw_set_chplan(u8 chplan);

/******************************************************
 *                    Constants
 ******************************************************/
#define RTW_JOIN_TIMEOUT 20000

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *               Variables Declarations
 ******************************************************/

#if !defined(CONFIG_MBED_ENABLED)
#if defined(CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
extern struct netif xnetif[NET_IF_NUM];
#endif
#endif

extern Rltk_wlan_t	rltk_wlan_info[NET_IF_NUM];

/******************************************************
 *               Variables Definitions
 ******************************************************/
static internal_join_block_param_t *join_block_param = NULL;

rtw_join_status_t rtw_join_status;
rtw_joinstatus_callback_t p_wifi_joinstatus_user_callback = NULL;
rtw_joinstatus_callback_t p_wifi_joinstatus_internal_callback = NULL;

wifi_do_fast_connect_ptr p_wifi_do_fast_connect = NULL;
write_fast_connect_info_ptr p_store_fast_connect_info = NULL;

/* The flag to check if wifi init is completed */
static int _wifi_is_on = 0;
extern void *param_indicator;

/******************************************************
 *               Variables Definitions
 ******************************************************/
/* Give default value if not defined */
/******************************************************
 *               Function Definitions
 ******************************************************/

#if CONFIG_WLAN
//----------------------------------------------------------------------------//
static void _wifi_join_status_indicate(rtw_join_status_t join_status)
{
	/* step 1: internal process for different status*/
	if (join_status == RTW_JOINSTATUS_SUCCESS) {
#if defined(CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
		LwIP_netif_set_link_up(0);
#endif

		/* if not use fast dhcp, store fast connect info to flash when connect successfully*/
#if !(defined(CONFIG_FAST_DHCP) && CONFIG_FAST_DHCP)
		if (p_store_fast_connect_info) {
			p_store_fast_connect_info(0, 0);
		}
#endif

		/* if Synchronous connection, up sema when connect success*/
		if (join_block_param && join_block_param->block) {
			rtw_up_sema(&join_block_param->join_sema);
		}
	}

	if (join_status == RTW_JOINSTATUS_FAIL) {
		/* if synchronous connection, up sema when connect fail*/
		if (join_block_param && join_block_param->block) {
			rtw_up_sema(&join_block_param->join_sema);
		}
	}

	if (join_status == RTW_JOINSTATUS_DISCONNECT) {
#if defined(CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
		LwIP_DHCP_stop(0);
#if LWIP_AUTOIP
		/*delete auto ip process for conflict with dhcp*/
		//LwIP_AUTOIP_STOP(0);
#endif
		LwIP_netif_set_link_down(0);
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

	if (connect_param == NULL) {
		RTW_API_INFO("\nwifi connect param not set!");
		return RTW_ERROR;
	}

	/* step1: check if there's ongoing connect*/
	if ((rtw_join_status > RTW_JOINSTATUS_UNKNOWN) && (rtw_join_status < RTW_JOINSTATUS_SUCCESS)) {
		RTW_API_INFO("\nthere is ongoing wifi connect!");
		return RTW_BUSY;
	}

	if (is_promisc_enabled()) {
		RTW_API_INFO("\npromisc is ongoing, can not do wifi connect now!");
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
	result = rtw_wx_connect_local(connect_param);
	if (result != RTW_SUCCESS) {
		rtw_join_status = RTW_JOINSTATUS_FAIL;
		goto error;
	}

	/* step4: wait connect finished for synchronous connection*/
	if (block) {
		join_block_param = block_param;

		// for eap connection, timeout should be longer (default value in wpa_supplicant: 60s)
		if (wifi_get_eap_phase()) {
			block_param->join_timeout = 60000;
		} else {
			block_param->join_timeout = RTW_JOIN_TIMEOUT;
		}

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

	//set MAC address last byte to 1 since driver will filter the mac with all 0x00 or 0xff
	//add extra 2 zero byte for check of #@ in wext_set_bssid()
	const __u8 null_bssid[ETH_ALEN + 2] = {0, 0, 0, 0, 0, 1, 0, 0};

	if (rtw_wx_set_wap(STA_WLAN_INDEX, (__u8 *)null_bssid) < 0) {
		RTW_API_INFO("\n\rWEXT: Failed to set bogus BSSID to disconnect");
		ret = -1;
	}
	return ret;
}

//----------------------------------------------------------------------------//
int wifi_is_running(unsigned char wlan_idx)
{
	return rtw_is_netdev_enable(wlan_idx) && _wifi_is_on;
}

//----------------------------------------------------------------------------//

void wifi_set_user_config(void)
{
#ifndef CONFIG_AS_INIC_NP
	// adaptivity
	wifi_user_config.rtw_adaptivity_en = DISABLE;
	/*
	 * 0 : RTW_ADAPTIVITY_MODE_NORMAL
	 * 1: RTW_ADAPTIVITY_MODE_CARRIER_SENSE
	 */
	wifi_user_config.rtw_adaptivity_mode = 0;
	wifi_user_config.rtw_adaptivity_th_l2h_ini = 0;
	//trp
	wifi_user_config.rtw_tx_pwr_lmt_enable = 2;	// 0: disable, 1: enable, 2: Depend on efuse(flash)
	wifi_user_config.rtw_tx_pwr_by_rate	= 2;	// 0: disable, 1: enable, 2: Depend on efuse(flash)
	wifi_user_config.rtw_trp_tis_cert_en = RTW_TRP_TIS_DISABLE;

#ifdef CONFIG_SAE_SUPPORT
	// set to 'ENABLE' when using WPA3
	wifi_user_config.rtw_cmd_tsk_spt_wap3 = ENABLE;
#endif
	wifi_user_config.wifi_wpa_mode = WPA_AUTO_MODE;

	wifi_user_config.g_user_ap_sta_num = NUM_STA;//NUM_STA (2 + AP_STA_NUM)

	/* power save */
	wifi_user_config.lps_dtim = 0;
	wifi_user_config.lps_enter_threshold = 0; // LPS_THRESH_PKT_COUNT

	wifi_user_config.rtw_power_mgnt = PS_MODE_MIN;
#if defined(CONFIG_LPS_PG)
	wifi_user_config.rtw_lps_level = LPS_PG;
#else
	wifi_user_config.rtw_lps_level = LPS_NORMAL;
#endif
	wifi_user_config.smart_ps = 2;

	/* AP */
	wifi_user_config.bForwardingDisabled = 0;

	wifi_user_config.bAcceptAddbaReq = (u8)_TRUE; // 0:Reject AP's Add BA req, 1:Accept AP's Add BA req.
	wifi_user_config.bIssueAddbaReq = (u8)_TRUE;
#ifdef CONFIG_80211AC_VHT
	wifi_user_config.ampdu_factor = 0;
#endif

	wifi_user_config.bCheckDestAddress = (u8)_TRUE;

	wifi_user_config.ap_compatibilty_enabled = 0x0B;

#ifdef CONFIG_LAYER2_ROAMING
	wifi_user_config.max_roaming_times = 2;
#endif

#ifdef CONFIG_ENABLE_AP_POLLING_CLIENT_ALIVE
	wifi_user_config.ap_polling_sta = 1;
#else
	wifi_user_config.ap_polling_sta = 0;
#endif

	wifi_user_config.channel_plan = 0;
	wifi_user_config.country_code = RTW_COUNTRY_MAX;

	wifi_user_config.auto_reconnect_count = 8;
	wifi_user_config.auto_reconnect_interval = 5; /* in sec*/
#endif
}

rtw_join_status_t wifi_get_join_status(void)
{
	return rtw_join_status;
}

void wifi_set_join_status(rtw_join_status_t status)
{
	rtw_join_status = status;
}

static int _wifi_on_boot(void)
{
	int ret = 1;
	static int event_init = 0;
	struct net_device *pnetdev;

	if (event_init == 0) {
		init_event_callback_list();
		event_init = 1;
	}

	// set wifi mib
	wifi_set_user_config();
	RTW_API_INFO("\n\rInitializing WIFI ...");

	wifi_if1_init();

	pnetdev = rtw_get_netdev(STA_WLAN_INDEX);
	ret = netdev_open(pnetdev);
	if (ret == 0) {
		_wifi_is_on = 1;

		//Used by FAST RECONNECTION
		if (p_wifi_do_fast_connect) {
			p_wifi_do_fast_connect();
		}
	}
	if (ret < 0) {
		RTW_API_INFO("\n\rERROR: Start WIFI Failed!");
		wifi_if1_deinit();
		return ret;
	}

	if (wifi_user_config.channel_plan) {
		rtw_set_chplan(wifi_user_config.channel_plan);
	}

#if defined(CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
	LwIP_netif_set_up(STA_WLAN_INDEX);
#endif

	return ret;
}

static int _wifi_on_ap(void)
{
	int ret = 1;
	struct net_device *pnetdev;

	if (wifi_is_running(SOFTAP_WLAN_INDEX)) {
		RTW_API_INFO("\n\rWIFI is already running\n");
		return 1;
	}

	RTW_API_INFO("\n\rInitializing WIFI AP Interface...\n");
	ret = wifi_if2_init();
	if (ret < 0) {
		return ret;
	}

	pnetdev = rtw_get_netdev(SOFTAP_WLAN_INDEX);
	ret = netdev_if2_open(pnetdev);
	if (ret < 0) {
		RTW_API_INFO("\n\rERROR: Start WIFI AP Interface Failed!\n");
		wifi_if2_deinit();
		return ret;
	}

#if defined(CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
	LwIP_netif_set_up(SOFTAP_WLAN_INDEX);
#endif

	return ret;
}

int wifi_on(rtw_mode_t mode)
{
	(void) mode;
	static u32 wifi_boot = 0;
	int ret = RTW_SUCCESS;

	/* boot on STA */
	if (wifi_boot == 0) {
		wifi_boot = 1;
		ret = _wifi_on_boot();
		return ret;
	}

	return ret;
}

int wifi_off(void)
{
	return RTW_SUCCESS;
}

int wifi_set_mode(rtw_mode_t mode)
{
	(void) mode;
	return 0;
}

int wifi_start_ap(rtw_softap_info_t *softAP_config)
{
	unsigned char wlan_idx = SOFTAP_WLAN_INDEX;
	int ret = 0;
	u8 *ext_key = NULL;
	u16 ext_key_len = 0;
	u16 alg;
	u32 key_idx = 0;

	_wifi_on_ap();

	if (softAP_config->ssid.len > 32) {
		RTW_API_INFO("Error: SSID should be 0-32 characters\r\n");
		ret = RTW_BADARG;
		goto exit;
	}

	if (softAP_config->password == NULL) {
		if (softAP_config->security_type != RTW_SECURITY_OPEN) {
			ret = RTW_INVALID_KEY;
			goto exit;
		}
	}
	if (softAP_config->security_type != RTW_SECURITY_OPEN) {
		if (softAP_config->password_len <= RTW_MAX_PSK_LEN &&
			softAP_config->password_len >= RTW_MIN_PSK_LEN) {
			if (softAP_config->password_len == RTW_MAX_PSK_LEN) { //password_len=64 means pre-shared key, pre-shared key should be 64 hex characters
				unsigned char i, j;
				for (i = 0; i < 64; i++) {
					j = softAP_config->password[i];
					if (!((j >= '0' && j <= '9') || (j >= 'A' && j <= 'F') || (j >= 'a' && j <= 'f'))) {
						RTW_API_INFO("Error: password should be 64 hex characters or 8-63 ASCII characters\n\r");
						ret = RTW_INVALID_KEY;
						goto exit;
					}
				}
			}
		}
#ifdef CONFIG_FPGA
		else if ((softAP_config->password_len == 5) && (softAP_config->security_type == RTW_SECURITY_WEP_PSK)) {
		}
#endif
		else {
			printf("Error: password should be 64 hex characters or 8-63 ASCII characters\n\r");
			ret = RTW_INVALID_KEY;
			goto exit;
		}
	}

	if (is_promisc_enabled()) {
		wifi_set_promisc(0, NULL, 0);
	}

	ret = rtw_wx_set_mode(wlan_idx, RTW_MODE_MASTER);
	if (ret < 0) {
		goto exit;
	}
	ret = wifi_set_freq(wlan_idx, softAP_config->channel);	//Set channel before starting ap
	if (ret < 0) {
		goto exit;
	}

	switch (softAP_config->security_type) {
	case RTW_SECURITY_OPEN:
		goto open;
#if defined(CONFIG_FPGA) && CONFIG_FPGA
	case RTW_SECURITY_WEP_PSK:
		alg = RTW_ENCODE_ALG_WEP;
		key_idx = 1;
		ext_key = (u8 *)softAP_config->password;
		ext_key_len = softAP_config->password_len;
		break;
	case RTW_SECURITY_WPA2_TKIP_PSK:
		alg = RTW_ENCODE_ALG_TKIP;
		break;
#endif
	case RTW_SECURITY_WPA2_AES_PSK:
		alg = RTW_ENCODE_ALG_CCMP;
		break;
#ifdef CONFIG_IEEE80211W
	case RTW_SECURITY_WPA2_AES_CMAC:
		alg = RTW_ENCODE_ALG_AES_CMAC;
		break;
#endif
	default:
		ret = -1;
		RTW_API_INFO("\n\rWIFICONF: security type is not supported");
		goto exit;
	}

	ret = rtw_wx_set_auth(wlan_idx, RTW_AUTH_ALG_OPEN_SYSTEM);
	if (ret == 0) {
		ret = rtw_wx_set_enc_ext(wlan_idx, alg, NULL, 0, key_idx, 0, 0, ext_key, ext_key_len);
	}
	if (ret == 0 && ext_key == NULL) {
		ret = rtw_wx_set_passphrase(wlan_idx, (u8 *)softAP_config->password, softAP_config->password_len);
	}

	if (ret < 0) {
		goto exit;
	}

open:
	ret = rtw_wx_set_ap_essid(softAP_config, 1);
	if (ret < 0) {
		goto exit;
	}

#if defined(CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
	LwIP_netif_set_link_up(wlan_idx);
#endif

exit:

	return ret;
}


int wifi_stop_ap(void)
{
	if (wifi_is_running(SOFTAP_WLAN_INDEX) == 0) {
		RTW_API_INFO("\n\rWIFI AP Interface is not running");
		return RTW_SUCCESS;
	}

	stop_bss_network();

#if defined(CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
	dhcps_deinit();
	LwIP_netif_set_link_down(1);
	LwIP_netif_set_down(1);
#endif

	RTW_API_INFO("\n\rDeinitializing WIFI ...");
	wifi_if2_deinit();

	return RTW_SUCCESS;
}


int wifi_scan_networks(rtw_scan_param_t *scan_param, unsigned char block)
{
	int ret = RTW_SUCCESS;

	if (scan_param == NULL) {
		RTW_API_INFO("SCAN param not set!\n");
		return RTW_ERROR;
	}

	if (block && scan_param->scan_user_callback) {
		RTW_API_INFO("not support user callback for block scan!\n");
		return RTW_ERROR;
	}

	if (scan_param->options & RTW_SCAN_REPORT_EACH) {
		if (scan_param->scan_report_each_mode_user_callback == NULL) {
			RTW_API_INFO("please register scan_report_each_mode_user_callback for RTW_SCAN_REPORT_EACH mode!\n");
			return RTW_ERROR;
		}
	}
	/* lock 2s to forbid suspend under scan */
	rtw_wakelock_timeout(2 * 1000);

	ret = rtw_wx_set_scan(scan_param, block);

	return ret;
}

#endif	//#if CONFIG_WLAN
