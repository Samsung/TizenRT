/**
  ******************************************************************************
  * @file    wifi_ind.c
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */


#include "wifi_ind.h"
#include "wifi_conf.h"
#include "platform_stdlib.h"
#include "basic_types.h"
#include "inic_ipc.h"
#if !defined(CONFIG_AS_INIC_NP)
#include "wpa_lite_intf.h"
#if CONFIG_AUTO_RECONNECT
#include <wifi_auto_reconnect.h>
#endif //CONFIG_AUTO_RECONNECT
#endif

#ifndef CONFIG_PLATFORM_TIZENRT_OS
#if defined (CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
#include <lwip_netconf.h>
#include <dhcp/dhcps.h>
#endif
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS

#ifdef CONFIG_ENABLE_EAP
extern void eap_disconnected_hdl(void);
#endif

/******************************************************
 *                    Globals
 ******************************************************/
extern rtw_joinstatus_callback_t p_wifi_joinstatus_internal_callback;

static event_list_elem_t     event_callback_list[WIFI_EVENT_MAX][WIFI_EVENT_MAX_ROW];

extern write_fast_connect_info_ptr p_store_fast_connect_info;
extern enum rtw_join_status_type rtw_join_status;
extern enum _rtw_result_t join_fail_reason;
extern struct internal_join_block_param *join_block_param;
extern wifi_jioninfo_free_ptr p_wifi_join_info_free;

#ifdef CONFIG_PLATFORM_TIZENRT_OS
extern enum rtw_join_status_type prev_join_status;
#endif //CONFIG_PLATFORM_TIZENRT_OS
//----------------------------------------------------------------------------//
enum _rtw_result_t rtw_indicate_event_handle(int event_cmd, char *buf, int buf_len, int flags)
{
	rtw_event_handler_t handle = NULL;
	int i;

	if (event_cmd >= WIFI_EVENT_MAX) {
		return (enum _rtw_result_t)RTW_BADARG;
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

void wifi_indication(enum rtw_event_indicate event, char *buf, int buf_len, int flags)
{
	//
	// If upper layer application triggers additional operations on wireless event,
	// 		please strictly check current stack size usage (by using uxTaskGetStackHighWaterMark() )
	//		, and tries not to share the same stack with wlan driver if remaining stack space is
	//		not available for the following operations.
	//		ex: using semaphore to notice another thread.

#if defined(CONFIG_AS_INIC_NP) || defined(CONFIG_SDIO_BRIDGE)
	inic_wifi_event_indicate(event, buf, buf_len, flags);
#endif

#ifndef CONFIG_AS_INIC_NP
	if (event == WIFI_EVENT_JOIN_STATUS) {
		wifi_event_join_status_internal_hdl(buf, flags);
	}
	rtw_indicate_event_handle(event, buf, buf_len, flags);
#endif
}

void wifi_reg_event_handler(unsigned int event_cmds, rtw_event_handler_t handler_func, void *handler_user_data)
{
	int i = 0, j = 0;
	if (event_cmds < WIFI_EVENT_MAX) {
		for (i = 0; i < WIFI_EVENT_MAX_ROW; i++) {
			if (event_callback_list[event_cmds][i].handler == NULL) { //there exists an empty position for new handler
				for (j = 0; j < WIFI_EVENT_MAX_ROW; j++) {
					if (event_callback_list[event_cmds][j].handler == handler_func) { //the new handler already exists in the table
						return;
					}
				}
				event_callback_list[event_cmds][i].handler = handler_func;
				event_callback_list[event_cmds][i].handler_user_data = handler_user_data;
				return;
			}
		}
		//there is no empty position for new handler
		RTK_LOGS(TAG_WLAN_INIC, "WifiEvtReg fail: %d %d \n", event_cmds, WIFI_EVENT_MAX_ROW);
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

void wifi_event_join_status_internal_hdl(char *buf, int flags)
{
#if !defined(CONFIG_MP_SHRINK) && !defined(CONFIG_AS_INIC_NP)
	struct deauth_info  *deauth_data, *deauth_data_pre;
	u8 zero_mac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	enum rtw_join_status_type join_status = (enum rtw_join_status_type)flags;
	struct rtw_event_join_fail_info_t *fail_info = (struct rtw_event_join_fail_info_t *)buf;

	/* step 1: internal process for different status*/
	if (join_status == RTW_JOINSTATUS_SUCCESS) {
#ifndef CONFIG_PLATFORM_TIZENRT_OS
#if defined(CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
		LwIP_netif_set_link_up(0);
#endif
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS

		/* if not use fast dhcp, store fast connect info to flash when connect successfully*/
#if !(defined(CONFIG_FAST_DHCP) && CONFIG_FAST_DHCP)
		if (p_store_fast_connect_info) {
			p_store_fast_connect_info(0, 0);
		}
		if (p_wifi_join_info_free) {
			/* free key here after join success */
			p_wifi_join_info_free(IFACE_PORT0);
		}
#endif

		/* if Synchronous connection, up sema when connect success*/
		if (join_block_param && join_block_param->block) {
			rtos_sema_give(join_block_param->join_sema);
		}
	}

	if (join_status == RTW_JOINSTATUS_FAIL) {
		/* if synchronous connection, up sema when connect fail*/
		if (join_block_param && join_block_param->block) {
			join_fail_reason = fail_info->fail_reason;
			rtos_sema_give(join_block_param->join_sema);
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
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
#ifndef CONFIG_AS_INIC_NP
		deauth_data_pre = (struct deauth_info *)rtos_mem_zmalloc(sizeof(struct deauth_info));
		rtw_psk_deauth_info_flash((char *)deauth_data_pre, sizeof(struct deauth_info), FLASH_READ, NULL);
		if (memcmp(deauth_data_pre->bssid, zero_mac, 6) != 0) {
			deauth_data = (struct deauth_info *)rtos_mem_zmalloc(sizeof(struct deauth_info));
			rtw_psk_deauth_info_flash((char *)deauth_data, sizeof(struct deauth_info), FLASH_WRITE, NULL);
			rtos_mem_free((u8 *)deauth_data);
		}
		rtos_mem_free((u8 *)deauth_data_pre);
#endif

#ifdef CONFIG_ENABLE_EAP
		eap_disconnected_hdl();
#endif
	}
#ifdef CONFIG_PLATFORM_TIZENRT_OS
	prev_join_status = rtw_join_status;
#endif //CONFIG_PLATFORM_TIZENRT_OS
	rtw_join_status = join_status;

	if ((join_status == RTW_JOINSTATUS_DISCONNECT) || (join_status == RTW_JOINSTATUS_FAIL)) {
		/*wpa lite disconnect hdl*/
		u8 port = IFACE_PORT0;
		rtw_psk_disconnect_hdl(buf, 0, flags, &port);
	}

#if CONFIG_AUTO_RECONNECT
	rtw_reconn_join_status_hdl(buf, flags);
#endif

#else
	UNUSED(flags);
	UNUSED(buf);
#endif
}

