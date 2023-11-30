//----------------------------------------------------------------------------//
#include "wifi_ind.h"
#include "wifi_conf.h"
#include "platform_stdlib.h"
#if defined(CONFIG_AS_INIC_NP)
#include "inic_ipc_api.h"
#else
#include "wpa_lite_intf.h"
#endif

#ifndef CONFIG_PLATFORM_TIZENRT_OS
#if defined (CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
#include <lwip_netconf.h>
#include <dhcp/dhcps.h>
#endif
#else
#include "rtk_wifi_utils.h"
#endif

/******************************************************
 *                    Globals
 ******************************************************/
extern rtw_joinstatus_callback_t p_wifi_joinstatus_internal_callback;

static event_list_elem_t     event_callback_list[WIFI_EVENT_MAX][WIFI_EVENT_MAX_ROW];

extern write_fast_connect_info_ptr p_store_fast_connect_info;
extern rtw_joinstatus_callback_t p_wifi_joinstatus_user_callback;
extern rtw_join_status_t rtw_join_status;
extern internal_join_block_param_t *join_block_param;

#ifdef CONFIG_PLATFORM_TIZENRT_OS
extern rtk_network_link_callback_t g_link_up;
extern rtk_network_link_callback_t g_link_down;
#endif

//----------------------------------------------------------------------------//
rtw_result_t rtw_indicate_event_handle(int event_cmd, char *buf, int buf_len, int flags)
{
	rtw_event_handler_t handle = NULL;
	int i;

	if (event_cmd >= WIFI_EVENT_MAX) {
		return (rtw_result_t)RTW_BADARG;
	}

	for (i = 0; i < WIFI_EVENT_MAX_ROW; i++) {
		handle = event_callback_list[event_cmd][i].handler;
		if (handle == NULL) {
			continue;
		}
		handle(buf, buf_len, flags, event_callback_list[event_cmd][i].handler_user_data);
	}

	return RTW_SUCCESS;
}

void wifi_indication(rtw_event_indicate_t event, char *buf, int buf_len, int flags)
{
	//
	// If upper layer application triggers additional operations on wireless event,
	// 		please strictly check current stack size usage (by using uxTaskGetStackHighWaterMark() )
	//		, and tries not to share the same stack with wlan driver if remaining stack space is
	//		not available for the following operations.
	//		ex: using semaphore to notice another thread.

#if defined(CONFIG_AS_INIC_NP)
	inic_ipc_wifi_event_indicate(event, buf, buf_len, flags);
#endif

	if (event == WIFI_EVENT_JOIN_STATUS) {
		if (p_wifi_joinstatus_internal_callback) {
			p_wifi_joinstatus_internal_callback((rtw_join_status_t)flags);
		}
	} else {
		rtw_indicate_event_handle(event, buf, buf_len, flags);
	}
}

void wifi_reg_event_handler(unsigned int event_cmds, rtw_event_handler_t handler_func, void *handler_user_data)
{
	int i = 0, j = 0;
	if (event_cmds < WIFI_EVENT_MAX) {
		for (i = 0; i < WIFI_EVENT_MAX_ROW; i++) {
			if (event_callback_list[event_cmds][i].handler == NULL) {
				for (j = 0; j < WIFI_EVENT_MAX_ROW; j++) {
					if (event_callback_list[event_cmds][j].handler == handler_func) {
						return;
					}
				}
				event_callback_list[event_cmds][i].handler = handler_func;
				event_callback_list[event_cmds][i].handler_user_data = handler_user_data;
				return;
			}
		}
	}
}

void wifi_unreg_event_handler(unsigned int event_cmds, rtw_event_handler_t handler_func)
{
	int i;
	if (event_cmds < WIFI_EVENT_MAX) {
		for (i = 0; i < WIFI_EVENT_MAX_ROW; i++) {
			if (event_callback_list[event_cmds][i].handler == handler_func) {
				event_callback_list[event_cmds][i].handler = NULL;
				event_callback_list[event_cmds][i].handler_user_data = NULL;
				return;
			}
		}
	}
}

void init_event_callback_list(void)
{
	memset(event_callback_list, 0, sizeof(event_callback_list));
}

void wifi_join_status_indicate(rtw_join_status_t join_status)
{
#ifndef CONFIG_AS_INIC_NP
	struct deauth_info  *deauth_data, *deauth_data_pre;
	u8 zero_mac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif

	/* step 1: internal process for different status*/
	if (join_status == RTW_JOINSTATUS_SUCCESS) {
#ifndef CONFIG_PLATFORM_TIZENRT_OS
#if defined(CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
		LwIP_netif_set_link_up(0);
#endif
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
#ifndef CONFIG_PLATFORM_TIZENRT_OS
#if defined(CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
		LwIP_DHCP_stop(0);
#if LWIP_AUTOIP
		/*delete auto ip process for conflict with dhcp*/
		//LwIP_AUTOIP_STOP(0);
#endif
		LwIP_netif_set_link_down(0);
#endif
#endif
#if defined(CONFIG_PLATFORM_TIZENRT_OS)
		rtk_reason_t reason;
		memset(&reason, 0, sizeof(rtk_reason_t));

		if (g_link_down) {
			nvdbg("RTK_API %s send link_down\n",__func__);
			g_link_down(&reason);
		}
#endif
#ifndef CONFIG_AS_INIC_NP
		deauth_data_pre = (struct deauth_info *)rtw_zmalloc(sizeof(struct deauth_info));
		rtw_psk_deauth_info_flash((char *)deauth_data_pre, sizeof(struct deauth_info), FLASH_READ, NULL);
		if (!rtw_memcmp(deauth_data_pre->bssid, zero_mac, 6)) {
			deauth_data = (struct deauth_info *)rtw_zmalloc(sizeof(struct deauth_info));
			rtw_psk_deauth_info_flash((char *)deauth_data, sizeof(struct deauth_info), FLASH_WRITE, NULL);
			rtw_mfree((u8 *)deauth_data, 0);
		}
		rtw_mfree((u8 *)deauth_data_pre, 0);
#endif
	}

	rtw_join_status = join_status;

	/* step 2: execute user callback to process join_status*/
	if (p_wifi_joinstatus_user_callback) {
		p_wifi_joinstatus_user_callback(join_status);
	}
}

