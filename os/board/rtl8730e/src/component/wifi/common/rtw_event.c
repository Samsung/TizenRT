/**
  ******************************************************************************
  * @file    rtw_event.c
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
#include "rtw_inic_common.h"
#if !defined (CONFIG_FULLMAC) && !(defined(ZEPHYR_WIFI) && defined(CONFIG_WHC_HOST))
#include "wifi_api.h"
#include "platform_stdlib.h"
#if !defined(CONFIG_WHC_DEV)|| defined(CONFIG_WPA_LOCATION_DEV) || defined(CONFIG_WHC_WPA_SUPPLICANT_OFFLOAD)
#ifndef CONFIG_PLATFORM_TIZENRT_OS
#include "atcmd_service.h"
#include <wifi_auto_reconnect.h>
#else
#include "rtk_wifi_utils.h"
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
#include "wpa_lite_intf.h"
#include "../wpa_supplicant/wps_protocol_handler.h"
#include "../wpa_supplicant/eap_protocol_handler.h"
#endif
#ifndef CONFIG_PLATFORM_TIZENRT_OS
#if defined (CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
#include <lwip_netconf.h>
#include <dhcp/dhcps.h>
#endif
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
#endif
#include "../../soc/common/diagnose/ameba_diagnose.h"
/**********************************************************************************************
 *                                          Globals
 *********************************************************************************************/
/* 1. single core or host 2.WPAoD. */
#if (!(defined CONFIG_WHC_DEV) || defined(CONFIG_WHC_WPA_SUPPLICANT_OFFLOAD))
#ifndef CONFIG_WPA_STD

extern int (*p_store_fast_connect_info)(unsigned int data1, unsigned int data2);
extern u8 rtw_join_status;
extern int join_fail_reason;
extern struct internal_block_param *join_block_param;
extern void (*p_wifi_join_info_free)(u8 iface_type);
extern void eap_disconnected_hdl(void);
#if !defined(CONFIG_MP_SHRINK) && defined (CONFIG_WHC_HOST)
extern u8 wifi_cast_get_initialized(void);
extern void wifi_cast_wifi_join_status_ev_hdl(u8 *evt_info);
#endif
#if defined(CONFIG_WHC_HOST) && !defined(CONFIG_PLATFORM_ZEPHYR) && !defined(CONFIG_MP_SHRINK) && defined(CONFIG_RMESH_EN)
extern void wtn_zrpp_get_ap_info_evt_hdl(u8 *evt_info);
#endif
/**********************************************************************************************
 *                                          Internal events
 *********************************************************************************************/

extern rtk_network_link_callback_t g_link_up;
extern rtk_network_link_callback_t g_link_down;

void wifi_event_join_status_internal_hdl(u8 *evt_info)
{
#if (!defined(CONFIG_MP_SHRINK) && !defined(CONFIG_WHC_DEV)) || defined(CONFIG_WHC_WPA_SUPPLICANT_OFFLOAD)
#ifndef CONFIG_PLATFORM_TIZENRT_OS
// TizenRT currently doesn't support saving deauth info as it requires a flash region
	struct deauth_info  *deauth_data, *deauth_data_pre;
	u8 zero_mac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS

	struct rtw_event_join_status_info *join_status_info = (struct rtw_event_join_status_info *)evt_info;
	u8 join_status = join_status_info->status;
	struct rtw_event_join_fail *join_fail;
#if defined(CONFIG_WHC_INTF_IPC)
	struct rtw_event_disconnect *disconnect;
	struct diag_evt_wifi_disconn diag_disconn;
	struct diag_evt_wifi_join_fail diag_join_fail;
#endif
	rtw_join_status = join_status;

	/* step 1: internal process for different status*/
	if (join_status == RTW_JOINSTATUS_SUCCESS) {
#ifndef CONFIG_PLATFORM_TIZENRT_OS
		at_printf_indicate("wifi connected\r\n");
#else
		dbg_noarg("wifi connected\r\n");
		if (g_link_up) {
			RTK_LOGD(TAG_WLAN_INIC, "%s() send link_up\n", __func__);
			rtk_reason_t reason = {0};
			reason.if_id = RTK_WIFI_STATION_IF;
			reason.reason_code = join_status;
			g_link_up(&reason);
		}
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
#ifndef CONFIG_PLATFORM_TIZENRT_OS
#if defined(CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
		LwIP_netif_set_link_up(NETIF_WLAN_STA_INDEX);
#endif
#endif

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
		if (join_block_param) {
			rtos_sema_give(join_block_param->sema);
		}
	}

	if (join_status == RTW_JOINSTATUS_FAIL) {
		join_fail = &join_status_info->priv.fail;
		/* if synchronous connection, up sema when connect fail*/
		if (join_block_param) {
			join_fail_reason = join_fail->fail_reason;
			rtos_sema_give(join_block_param->sema);
		}
#if defined(CONFIG_WHC_INTF_IPC)
		diag_join_fail.reason = -join_fail->fail_reason;
		diag_join_fail.reason_code = join_fail->reason_or_status_code;
		rtk_diag_event_add(RTK_EVENT_LEVEL_INFO, DIAG_EVT_WIFI_JOIN_FAIL, (u8 *)&diag_join_fail, sizeof(struct diag_evt_wifi_join_fail));
#endif
#ifndef CONFIG_PLATFORM_TIZENRT_OS
		at_printf_indicate("wifi connect failed\r\n");
#else
		dbg_noarg("wifi connect failed\r\n");
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
	}

	if (join_status == RTW_JOINSTATUS_DISCONNECT) {
#if defined(CONFIG_WHC_INTF_IPC)
		disconnect = &join_status_info->priv.disconnect;
		diag_disconn.reason = disconnect->disconn_reason;
		rtk_diag_event_add(RTK_EVENT_LEVEL_INFO, DIAG_EVT_WIFI_DISCONN, (u8 *)&diag_disconn, sizeof(struct diag_evt_wifi_disconn));
#endif
#ifndef CONFIG_PLATFORM_TIZENRT_OS
		at_printf_indicate("wifi disconnected\r\n");
#if defined(CONFIG_LWIP_LAYER) && CONFIG_LWIP_LAYER
		LwIP_DHCP_stop(NETIF_WLAN_STA_INDEX);
		LwIP_netif_set_link_down(NETIF_WLAN_STA_INDEX);
#endif

#if (!defined(CONFIG_WHC_DEV) && !(defined(ZEPHYR_WIFI) && defined(CONFIG_WHC_HOST))) || defined(CONFIG_WHC_WPA_SUPPLICANT_OFFLOAD)
// TizenRT currently doesn't support saving deauth info as it requires a flash region
		deauth_data_pre = (struct deauth_info *)rtos_mem_zmalloc(sizeof(struct deauth_info));
		rtw_psk_deauth_info_flash((u8 *)deauth_data_pre, sizeof(struct deauth_info), FLASH_READ);
		if (memcmp(deauth_data_pre->bssid, zero_mac, 6) != 0) {
			deauth_data = (struct deauth_info *)rtos_mem_zmalloc(sizeof(struct deauth_info));
			rtw_psk_deauth_info_flash((u8 *)deauth_data, sizeof(struct deauth_info), FLASH_WRITE);
			rtos_mem_free((u8 *)deauth_data);
		}
		rtos_mem_free((u8 *)deauth_data_pre);
#endif
#else
		dbg_noarg("wifi disconnected\r\n");
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS

		eap_disconnected_hdl();
	}

	if ((join_status == RTW_JOINSTATUS_DISCONNECT) || (join_status == RTW_JOINSTATUS_FAIL)) {
		/*wpa lite disconnect hdl*/
		rtw_psk_disconnect_hdl(join_status_info->bssid, IFACE_PORT0);
#ifdef CONFIG_PLATFORM_TIZENRT_OS
		if (g_link_down) {
			RTK_LOGD(TAG_WLAN_INIC, "%s() send link_down\n", __func__);
			rtk_reason_t reason = {0};
			reason.if_id = RTK_WIFI_STATION_IF;
			reason.reason_code = join_status;
			g_link_down(&reason);
		}
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
	}

#if !defined(CONFIG_WHC_DEV) && CONFIG_AUTO_RECONNECT
	rtw_reconn_join_status_hdl(evt_info);
#endif

#if defined(CONFIG_WHC_HOST) && !defined(CONFIG_PLATFORM_ZEPHYR)
	if (wifi_cast_get_initialized()) {
		wifi_cast_wifi_join_status_ev_hdl(evt_info);
	}
#endif
#else
	UNUSED(evt_info);
#endif
}

void rtw_ap_sta_assoc_hdl(u8 *evt_info)
{
	struct rtw_event_ap_sta_assoc *sta_assoc_info = (struct rtw_event_ap_sta_assoc *)evt_info;
	u8 *mac_addr = NULL;

	/* softap add sta */
	mac_addr = sta_assoc_info->sta_mac; // GetAddr2Ptr
#ifndef CONFIG_PLATFORM_TIZENRT_OS
	at_printf_indicate("client connected:\""MAC_FMT"\"\r\n", MAC_ARG(mac_addr));
#else
	dbg_noarg("client connected:\""MAC_FMT"\"\r\n", MAC_ARG(mac_addr));
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
}

void rtw_ap_sta_disassoc_hdl(u8 *evt_info)
{
	struct rtw_event_ap_sta_disassoc *sta_disassoc_info = (struct rtw_event_ap_sta_disassoc *)evt_info;
	u8 *mac_addr = NULL;

	/* softap dis sta */
	mac_addr = sta_disassoc_info->sta_mac;
#ifndef CONFIG_PLATFORM_TIZENRT_OS
	at_printf_indicate("client disconnected:\""MAC_FMT"\"\r\n", MAC_ARG(mac_addr));
#else
	dbg_noarg("client disconnected:\""MAC_FMT"\"\r\n", MAC_ARG(mac_addr));
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
	rtw_psk_disconnect_hdl(mac_addr, IFACE_PORT1);
}

void rtw_join_status_hdl(u8 *evt_info)
{
	wifi_event_join_status_internal_hdl(evt_info);
}

void rtw_eapol_start_hdl(u8 *evt_info)
{
	(void)evt_info;
#ifdef CONFIG_ENABLE_EAP
	if (get_eap_phase()) {
		eap_eapol_start_hdl(evt_info);
	}
#endif
}

void rtw_eapol_recvd_hdl(u8 *evt_info)
{
	struct rtw_event_report_frame *evt_rpt_frm = (struct rtw_event_report_frame *)evt_info;
	(void)evt_rpt_frm;
#ifdef CONFIG_ENABLE_EAP
	if (get_eap_phase()) {
		eap_eapol_recvd_hdl(evt_rpt_frm->frame, evt_rpt_frm->frame_len);
	}
#endif

#if defined(CONFIG_ENABLE_WPS) && CONFIG_ENABLE_WPS
	if (get_wps_phase()) {
		wpas_wsc_eapol_recvd_hdl(evt_rpt_frm->frame, evt_rpt_frm->frame_len);
	}
#endif
}

void rtw_dhcp_status_hdl(u8 *evt_info)
{
	(void)evt_info;
#if !defined(CONFIG_WHC_DEV) && CONFIG_AUTO_RECONNECT
	rtw_reconn_dhcp_status_hdl(evt_info);
#endif
}

void rtw_p2p_channel_switch_ready(u8 *evt_info)
{
	(void)evt_info;
#ifdef CONFIG_P2P
	//TODO
#endif
}

/**
 * @brief internal event handle
 */
const struct rtw_event_hdl_func_t event_internal_hdl[] = {
	{RTW_EVENT_AP_STA_ASSOC,		rtw_ap_sta_assoc_hdl},
	{RTW_EVENT_AP_STA_DISASSOC,		rtw_ap_sta_disassoc_hdl},
	{RTW_EVENT_JOIN_STATUS,			rtw_join_status_hdl},
#if defined(CONFIG_ENABLE_WPS) && CONFIG_ENABLE_WPS
	{RTW_EVENT_WPA_STA_WPS_START,	wpas_wsc_sta_wps_start_hdl},
	{RTW_EVENT_WPA_WPS_FINISH,		wpas_wsc_wps_finish_hdl},
#endif
	{RTW_EVENT_WPA_EAPOL_START,		rtw_eapol_start_hdl},
	{RTW_EVENT_WPA_EAPOL_RECVD,		rtw_eapol_recvd_hdl},
#if !defined(CONFIG_WHC_DEV) && CONFIG_AUTO_RECONNECT
	{RTW_EVENT_DHCP_STATUS,			rtw_dhcp_status_hdl},
#endif
	{RTW_EVENT_RX_MGNT,				rtw_sae_sta_rx_auth},
	{RTW_EVENT_RX_MGNT_AP,			rtw_sae_ap_rx_auth},
	{RTW_EVENT_EXTERNAL_AUTH_REQ,	rtw_sae_sta_start},
	{RTW_EVENT_WPA_STA_4WAY_START,	rtw_psk_sta_start_4way},
	{RTW_EVENT_WPA_AP_4WAY_START,	rtw_psk_ap_start_4way},
	{RTW_EVENT_WPA_STA_4WAY_RECV,	rtw_psk_sta_recv_eapol},
	{RTW_EVENT_WPA_AP_4WAY_RECV,	rtw_psk_ap_recv_eapol},
	{RTW_EVENT_WPA_SET_PSK_INFO,	rtw_psk_set_psk_info_evt_hdl},
	{RTW_EVENT_OWE_START_CALC,		rtw_owe_start_calc},
	{RTW_EVENT_OWE_PEER_KEY_RECV,	rtw_owe_peer_key_recv},
#if defined(CONFIG_IEEE80211V) || defined(CONFIG_IEEE80211K) || defined(CONFIG_IEEE80211R)
	{RTW_EVENT_KVR_CAP_UPDATE,		rtw_roam_kvr_cap_update},
#if defined(CONFIG_IEEE80211V) || defined(CONFIG_IEEE80211K)
	{RTW_EVENT_NB_RESP_RECV,		rtw_roam_nb_rpt_elem_parsing},
#endif
#ifdef CONFIG_IEEE80211V
	{RTW_EVENT_BTM_REQ_RECV,		rtw_wnm_btm_req_process},
	{RTW_EVENT_BTM_DEBUG_CMD,		rtw_wnm_dbg_cmd},
#endif
#ifdef CONFIG_IEEE80211R
	{RTW_EVENT_FT_AUTH_START,		rtw_ft_auth_start},
	{RTW_EVENT_FT_RX_MGNT,			rtw_ft_rx_mgnt},
#endif
#endif
#ifdef CONFIG_P2P
	{RTW_EVENT_WPA_P2P_CHANNEL_RDY,	rtw_p2p_channel_switch_ready},
#endif
#ifndef CONFIG_PLATFORM_TIZENRT_OS
// TizenRT currently doesn't support saving deauth info as it requires a flash region
	{RTW_EVENT_DEAUTH_INFO_FLASH,	rtw_psk_deauth_info_flash_event_hdl},
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
#if defined(CONFIG_WHC_HOST) && !defined(CONFIG_PLATFORM_ZEPHYR) && !defined(CONFIG_MP_SHRINK) && defined(CONFIG_RMESH_EN)
	{RTW_EVENT_WTN_ZRPP_GET_AP_INFO, wtn_zrpp_get_ap_info_evt_hdl},
#endif
};

void wifi_event_handle_internal(u32 event_cmd, u8 *evt_info)
{
#ifndef CONFIG_MP_SHRINK
#if !(defined(ZEPHYR_WIFI) && defined(CONFIG_WHC_HOST))
	/*internal & common events all need handle*/
#else
	/*only common events need handle*/
	if (event_cmd > RTW_EVENT_INTERNAL_BASE) {
		return;
	}
#endif

	for (u32 i = 0; i < sizeof(event_internal_hdl) / sizeof(struct rtw_event_hdl_func_t); i++) {
		if (event_internal_hdl[i].evt_id == event_cmd) {
			if (event_internal_hdl[i].handler == NULL) {
				continue;
			}
			event_internal_hdl[i].handler(evt_info);
		}
	}
#else
	UNUSED(event_cmd);
	UNUSED(evt_info);
#endif
}
#endif

void wifi_event_init(void)
{
	// Nothing TODO, global BSS variable default is 0
}
#endif

/* 1. single core or host 2.not ipc dev. */
#if !(defined CONFIG_WHC_DEV) || !defined(CONFIG_WHC_INTF_IPC)
extern struct rtw_event_hdl_func_t event_external_hdl[];
extern u16 array_len_of_event_external_hdl;
/**********************************************************************************************
 *                                          External events
 *********************************************************************************************/
void wifi_event_handle_external(u32 event_cmd, u8 *evt_info)
{
	if (!array_len_of_event_external_hdl) {
		return;
	}

	for (u32 i = 0; i < array_len_of_event_external_hdl; i++) {
		if (event_external_hdl[i].evt_id == event_cmd) {
			if (event_external_hdl[i].handler == NULL) {
				continue;
			}
			event_external_hdl[i].handler(evt_info);
		}
	}
}
#endif

/**********************************************************************************************
 *                                          Common events
 *********************************************************************************************/
int wifi_event_handle(u32 event_cmd, u8 *evt_info)
{
	u32 prepend_len;
	(void)evt_info;
	(void)prepend_len;

	if (event_cmd & (BIT(31))) {
		/* WPAoH: due to whc_dev_wifi_event_indicate*/
		prepend_len = 2 * sizeof(u32);
	} else {
		/* 1.host or single 2.ipc dev 3.WPAoD */
		prepend_len = 0;
	}
	event_cmd &= (~(BIT(31)));
	if ((event_cmd >= RTW_EVENT_MAX && event_cmd <= RTW_EVENT_INTERNAL_BASE) || event_cmd > RTW_EVENT_INTERNAL_MAX) {
		RTK_LOGS(TAG_WLAN_INIC, RTK_LOG_ERROR, "invalid evt: %d \n", event_cmd);
		return -RTK_ERR_BADARG;
	}

	/* 1. single core or host 2.WPAoD. */
#if (!(defined CONFIG_WHC_DEV) || defined(CONFIG_WHC_WPA_SUPPLICANT_OFFLOAD))
#ifndef CONFIG_WPA_STD
	wifi_event_handle_internal(event_cmd, evt_info + prepend_len); //prepend_len = 0
#endif
#endif
	/* 1. single core or host 2.not ipc dev. */
#if !(defined CONFIG_WHC_DEV) || !defined(CONFIG_WHC_INTF_IPC)
	wifi_event_handle_external(event_cmd, evt_info + prepend_len);
#endif
	return RTK_SUCCESS;
}

void wifi_indication(u32 event, u8 *evt_info, s32 evt_len)
{
	(void)evt_len;

	/* 1. ipc dev 2. WPAoH */
#if defined(CONFIG_WHC_DEV) && !defined(CONFIG_WHC_WPA_SUPPLICANT_OFFLOAD)
	extern void whc_dev_wifi_event_indicate(u32 event_cmd, u8 * evt_info, s32 evt_len);
	whc_dev_wifi_event_indicate(event, evt_info, evt_len);
#endif

#if defined(CONFIG_WPA_STD)
	extern void whc_dev_wpas_wifi_event_indicate(u32 event_cmd, u8 * evt_info, s32 evt_len);
	whc_dev_wpas_wifi_event_indicate(event, evt_info, evt_len);
#endif

	wifi_event_handle(event, evt_info);
}

void wifi_indication_ext(u32 event, u8 *info_buf, s32 info_len, u8 *frame_buf, s32 frame_len)
{
	u32 *evt_buf = NULL, *dst_buf = NULL;
	s32 data_len = info_len + frame_len;
	u32 evt_buf_len, prepend_len = 0;

	/* WPAoH: due to whc_dev_wifi_event_indicate*/
#if defined(CONFIG_WHC_DEV) && !defined(CONFIG_WHC_WPA_SUPPLICANT_OFFLOAD) && !defined(CONFIG_WHC_INTF_IPC)
	prepend_len = 2 * sizeof(u32);   /* Prepend event and data_len if non IPC intf */
#endif

	evt_buf_len = data_len + prepend_len;
	evt_buf = (u32 *) rtos_mem_zmalloc(evt_buf_len);
	dst_buf = evt_buf;
	if (prepend_len) {
		evt_buf[0] = event;
		evt_buf[1] = data_len;
		dst_buf += 2;
		/* Using bit31 to indicate that memory has malloced in case of non IPC intf */
		event |= ((u32)1 << 31);
	}

	memcpy((u8 *)dst_buf, info_buf, info_len);
	memcpy(((u8 *)dst_buf + info_len), frame_buf, frame_len);

	wifi_indication(event, (u8 *)evt_buf, evt_buf_len);
	rtos_mem_free(evt_buf);
}

