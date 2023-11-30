//----------------------------------------------------------------------------//
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

#if defined(CONFIG_AS_INIC_AP)
#include "inic_ipc_api.h"
#endif
#include <wifi_intf_ram_to_rom.h>
#include "wpa_lite_intf.h"

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
internal_join_block_param_t *join_block_param = NULL;

rtw_result_t (*scan_user_callback_ptr)(unsigned int, void *) = NULL;
rtw_result_t (*scan_each_report_user_callback_ptr)(rtw_scan_result_t *, void *) = NULL;

void (*promisc_user_callback_ptr)(void *) = NULL;

extern void *param_indicator;
rtw_join_status_t rtw_join_status;
rtw_joinstatus_callback_t p_wifi_joinstatus_user_callback = NULL;
rtw_joinstatus_callback_t p_wifi_joinstatus_internal_callback = NULL;

wifi_do_fast_connect_ptr p_wifi_do_fast_connect = NULL;
write_fast_connect_info_ptr p_store_fast_connect_info = NULL;

extern void wifi_set_user_config(void);

/* Give default value if not defined */
/******************************************************
 *               Function Definitions
 ******************************************************/

#if CONFIG_WLAN
#if defined(CONFIG_PLATFORM_TIZENRT_OS)
#include "rtk_wifi_utils.h"
unsigned char ap_bssid[ETH_ALEN];
rtk_network_link_callback_t g_link_up = NULL;
rtk_network_link_callback_t g_link_down = NULL;


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
static void *_my_calloc(size_t nelements, size_t elementSize)
{
	size_t size;
	void *ptr = NULL;

	size = nelements * elementSize;
	ptr = rtw_zmalloc(size);

	return ptr;
}

static void _my_free(void *pbuf)
{
	rtw_free(pbuf);
}

static int _my_random(void *p_rng, unsigned char *output, size_t output_len)
{
	/* To avoid gcc warnings */
	(void) p_rng;

	rtw_get_random_bytes(output, output_len);
	return 0;
}
int wifi_set_platform_rom_func(void *(*calloc_func)(size_t, size_t),
							   void (*free_func)(void *),
							   int (*rand_func)(void *, unsigned char *, size_t))
{
	/* Realtek added to initialize HW crypto function pointers
	* mbedtls RAM codes use function pointers in platform memory implementation
	* Not use malloc/free in ssl ram map for mbedtls RAM codes
	*/
	p_wifi_rom_func_map = (struct _wifi_rom_func_map *)&wifi_rom_func_map;
	p_wifi_rom_func_map->zmalloc = calloc_func;
	p_wifi_rom_func_map->mfree = free_func;
	p_wifi_rom_func_map->random = rand_func;

	return (0);
}

//----------------------------------------------------------------------------//
int wifi_connect(rtw_network_info_t *connect_param, unsigned char block)
{
	rtw_result_t result = RTW_SUCCESS;
	internal_join_block_param_t *block_param = NULL;
	u32 param_buf[1] = {0};

#if defined(CONFIG_PLATFORM_TIZENRT_OS)
		rtk_reason_t reason;
#endif

	/* check if SoftAP is running */
	if ((wifi_user_config.concurrent_enabled == _FALSE) && wifi_is_running(SOFTAP_WLAN_INDEX)) {
		RTW_API_INFO("Concurrent mode is disabled. To use it, please set concurrent_enabled to _TRUE in wifi_set_user_config() !!\n");
		return RTW_ERROR;
	}

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
	p_wifi_joinstatus_internal_callback = wifi_join_status_indicate;

	/*clear for last connect status */
	rtw_join_status = RTW_JOINSTATUS_STARTING;
	wifi_join_status_indicate(RTW_JOINSTATUS_STARTING);

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

		if (rtw_down_timeout_sema(&block_param->join_sema, block_param->join_timeout) == _FAIL) {
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
		wifi_join_status_indicate(RTW_JOINSTATUS_FAIL);
	}

	return result;
}

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
int wifi_is_running(unsigned char wlan_idx)
{
	int ret;
	u32 param_buf[1];
	param_buf[0] = wlan_idx;

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_IS_RUNNING, param_buf, 1);
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
	static u32 wifi_boot = 0;
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
	wifi_set_platform_rom_func(_my_calloc, _my_free, _my_random);
	param_buf[0] = mode;

	inic_ipc_host_init_skb();
	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_ON, param_buf, 1);

	if (wifi_boot == 0) {
		wifi_boot = 1;
		init_timer_wrapper();
		init_timer_pool();
		rtw_psk_wpa_init(STA_WLAN_INDEX);

		if (p_wifi_do_fast_connect && (mode == RTW_MODE_STA)) {
			p_wifi_do_fast_connect();
		}
	}

	if (ret == RTW_SUCCESS) { //wifi on success
#if !defined(CONFIG_PLATFORM_TIZENRT_OS)
#if CONFIG_LWIP_LAYER
		if (mode == RTW_MODE_STA) {
			LwIP_netif_set_up(0);
		}
#endif
#endif
	}

	return ret;
}

int wifi_off(void)
{
	int ret = 0;

	//inic_ipc_host_deinit_skb();/*should be called after np deinit*/
	return ret;
}

int wifi_set_mode(rtw_mode_t mode)
{
	(void) mode;

	return 0;
}

#if defined(CONFIG_PLATFORM_TIZENRT_OS)
static void wifi_ap_sta_assoc_hdl( char* buf, int buf_len, int flags, void* userdata)
{
	/* To avoid gcc warnings */
	( void ) buf;
	( void ) buf_len;
	( void ) flags;
	( void ) userdata;
	//USER TODO
	rtk_reason_t reason;
	memset(&reason, 0, sizeof(rtk_reason_t));
	if (strlen(buf) >= 17) {			  // bssid is a 17 character string
		memcpy(&(reason.bssid), buf, 17); // Exclude null-termination
	}

	if (g_link_up) {
		nvdbg("RTK_API rtk_link_event_handler send link_up\n");
		g_link_up(&reason);
	}
}
static void wifi_ap_sta_disassoc_hdl( char* buf, int buf_len, int flags, void* userdata)
{
	/* To avoid gcc warnings */
	( void ) buf;
	( void ) buf_len;
	( void ) flags;
	( void ) userdata;
	//USER TODO
	rtk_reason_t reason;
	memset(&reason, 0, sizeof(rtk_reason_t));
	if (strlen(buf) >= 17) { // bssid is a 17 character string
		memcpy(&(reason.bssid), buf, 17);
	}
	if (g_link_down) {
		nvdbg("RTK_API rtk_handle_disconnect send link_down\n");
		g_link_down(&reason);
	}
}
#endif

int wifi_start_ap(rtw_softap_info_t *softAP_config)
{
	int ret = 0;
	u32 param_buf[1];
	struct psk_info PSK_INFO = {0};
	/* check if STA is running */
	if ((wifi_user_config.concurrent_enabled == _FALSE) &&
		(rtw_join_status > RTW_JOINSTATUS_UNKNOWN) && (rtw_join_status <= RTW_JOINSTATUS_SUCCESS)) {
		RTW_API_INFO("Concurrent mode is disabled. To use it, please set concurrent_enabled to _TRUE in wifi_set_user_config() !!\n");
		ret = RTW_ERROR;
		goto exit;
	}

	DCache_Clean((u32)softAP_config->password, softAP_config->password_len);
	DCache_Clean((u32)softAP_config, sizeof(rtw_softap_info_t));
	param_buf[0] = (u32)softAP_config;

#if defined(CONFIG_PLATFORM_TIZENRT_OS)
	wifi_reg_event_handler(WIFI_EVENT_STA_ASSOC, wifi_ap_sta_assoc_hdl, NULL);
	wifi_reg_event_handler(WIFI_EVENT_STA_DISASSOC, wifi_ap_sta_disassoc_hdl, NULL);
#endif

	rtw_psk_wpa_init(SOFTAP_WLAN_INDEX);

	if (softAP_config->password && softAP_config->password_len) {
		PSK_INFO.index = SOFTAP_WLAN_INDEX;
		PSK_INFO.security_type = softAP_config->security_type;
		rtw_memcpy(PSK_INFO.psk_essid, softAP_config->ssid.val, softAP_config->ssid.len);
		rtw_memcpy(PSK_INFO.psk_passphrase, softAP_config->password, softAP_config->password_len);
		rtw_psk_set_psk_info(&PSK_INFO);
	}

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_START_AP, param_buf, 1);

	if (ret == RTW_SUCCESS) {
#if !defined(CONFIG_PLATFORM_TIZENRT_OS)
#if CONFIG_LWIP_LAYER
		LwIP_netif_set_up(SOFTAP_WLAN_INDEX);
		LwIP_netif_set_link_up(SOFTAP_WLAN_INDEX);
#endif
#endif
	}

exit:
	return ret;
}

int wifi_stop_ap(void)
{
	int ret = 0;

	if (wifi_is_running(SOFTAP_WLAN_INDEX) == 0) {
		RTW_API_INFO("\n\rWIFI is not running");
		return 0;
	}

#if !defined(CONFIG_PLATFORM_TIZENRT_OS)
#if CONFIG_LWIP_LAYER
	dhcps_deinit();
	LwIP_netif_set_down(1);
	LwIP_netif_set_link_down(1);
#endif
#endif

	ret = inic_ipc_api_host_message_send(IPC_API_WIFI_STOP_AP, NULL, 0);
	rtw_psk_wpa_deinit(SOFTAP_WLAN_INDEX);

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
#if defined(CONFIG_PLATFORM_TIZENRT_OS)
		/* for TizenRT, ensure the entire ssid array is cleaned when ssid is empty */
		if (strlen(scan_param->ssid) == 0) {
			DCache_Clean((u32)scan_param->ssid, (TRWIFI_PASSPHRASE_LEN + 1));
		}
		else {
			DCache_Clean((u32)scan_param->ssid, strlen(scan_param->ssid));
		}
#else
		DCache_Clean((u32)scan_param->ssid, strlen(scan_param->ssid));
#endif
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

void wifi_promisc_enable(u32 enable, promisc_para_t *para)
{
	u32 buf[3] = {0};
	buf[0] = enable;
	buf[1] = (u32)para->filter_mode;
	if (para->callback) {
		promisc_user_callback_ptr = para->callback;
		buf[2] = ENABLE;
	}
	inic_ipc_api_host_message_send(IPC_API_WIFI_PROMISC_INIT, buf, 3);
}

#endif	//#if CONFIG_WLAN
