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

#ifndef __INIC_IPC_API_H__
#define __INIC_IPC_API_H__

/* -------------------------------- Includes -------------------------------- */
#include "platform_opts.h"

#include "ameba_soc.h"

#include "osdep_service.h"
#include "rtw_wifi_constants.h"
#include "wifi_conf.h"
#include "rtw_skbuff.h"

/* -------------------------------- Defines --------------------------------- */
#define HOST_MSG_PARAM_NUM 9
#define HOST_MSG_DUMY_NUM (64 - (HOST_MSG_PARAM_NUM + 2) * 4)
#define DEV_MSG_PARAM_NUM 7
#define DEV_MSG_DUMY_NUM (64 - (DEV_MSG_PARAM_NUM + 2) * 4)

#define INIC_MP_MSG_BUF_SIZE 4096
/* -------------------------------- Macros ---------------------------------- */


/* ------------------------------- Data Types ------------------------------- */
enum IPC_WIFI_C2H_EVENT_TYPE {
	IPC_WIFI_API_PROCESS_DONE = 0,
	IPC_WIFI_EVT_SCAN_USER_CALLBACK,
	IPC_WIFI_EVT_SCAN_EACH_REPORT_USER_CALLBACK,
	IPC_WIFI_EVT_AUTO_RECONNECT,
	IPC_WIFI_EVT_EAP_AUTO_RECONNECT,
	IPC_WIFI_EVT_AP_CH_SWITCH,
	IPC_WIFI_EVT_HDL,
	IPC_WIFI_EVT_PROMISC_CALLBACK,
	IPC_WIFI_EVT_GET_LWIP_INFO,
	IPC_WIFI_EVT_SET_NETIF_INFO,
	IPC_WIFI_EVT_CFG80211_SCAN_REPORT,
	IPC_WIFI_EVT_IP_TABLE_CHK,

	/*NAN related*/
	IPC_WIFI_EVT_CFG80211_NAN_REPORT_MATCH_EVENT,
	IPC_WIFI_EVT_CFG80211_NAN_DEL_FUNC,
	IPC_WIFI_EVT_CFG80211_NAN_CFGVENDOR_EVENT,
	IPC_WIFI_EVT_CFG80211_NAN_CFGVENDOR_CMD_REPLY,
	IPC_BUFFERED_PRINTF_NP
};

enum IPC_WIFI_H2C_EVENT_TYPE {
	IPC_WIFI_EVT_PROCESS_DONE = 0,
	//basic
	IPC_API_WIFI_CONNECT,
	IPC_API_WIFI_DISCONNECT,
	IPC_API_WIFI_IS_CONNECTED_TO_AP,
	IPC_API_WIFI_IS_RUNNING,
	IPC_API_WIFI_SET_CHANNEL,
	IPC_API_WIFI_GET_CHANNEL,
	IPC_API_WIFI_SET_USR_CFG,
	IPC_API_WIFI_GET_DISCONN_REASCON,
	IPC_API_WIFI_ON,
	IPC_API_WIFI_OFF,
	IPC_API_WIFI_SET_MODE,
	IPC_API_WIFI_INIT_AP,
	IPC_API_WIFI_DEINIT_AP,
	IPC_API_WIFI_START_AP,
	IPC_API_WIFI_STOP_AP,
	IPC_API_WIFI_SCAN_NETWROKS,
	IPC_API_WIFI_GET_SCANNED_AP_INFO,
	IPC_API_WIFI_SCAN_ABORT,
	//ext
	IPC_API_WIFI_SET_MAC_ADDR,
	IPC_API_WIFI_GET_MAC_ADDR,
	IPC_API_WIFI_COEX_BLE_SET_SCAN_DUTY,
	IPC_API_WIFI_DRIVE_IS_MP,
	IPC_API_WIFI_GET_ASSOCIATED_CLIENT_LIST,
	IPC_API_WIFI_GET_SETTING,
	IPC_API_WIFI_SET_IPS_EN,
	IPC_API_WIFI_SET_LPS_EN,
	IPC_API_WIFI_SET_MFP_SUPPORT,
	IPC_API_WIFI_SET_GROUP_ID,
	IPC_API_WIFI_SET_PMK_CACHE_EN,
	IPC_API_WIFI_SAE_STATUS,
	IPC_API_WIFI_GET_SW_STATISTIC,
	IPC_API_WIFI_GET_PHY_STATISTIC,
	IPC_API_WIFI_SET_NETWORK_MODE,
	IPC_API_WIFI_SET_WPS_PHASE,
	IPC_API_WIFI_SET_GEN_IE,
	IPC_API_WIFI_SET_EAP_PHASE,
	IPC_API_WIFI_GET_EAP_PHASE,
	IPC_API_WIFI_SET_EAP_METHOD,
	IPC_API_WIFI_SEND_EAPOL,
	IPC_API_WIFI_CONFIG_AUTORECONNECT,
	IPC_API_WIFI_GET_AUTORECONNECT,
	IPC_API_WIFI_CUS_IE,
	IPC_API_WIFI_SET_IND_MGNT,
	IPC_API_WIFI_SEND_MGNT,
	IPC_API_WIFI_SET_TXRATE_BY_TOS,
	IPC_API_WIFI_SET_EDCA_PARAM,
	IPC_API_WIFI_SET_TX_CCA,
	IPC_API_WIFI_SET_CTS2SEFL_DUR_AND_SEND,
	IPC_API_WIFI_MAC_FILTER,
	IPC_API_WIFI_GET_ANTENNA_INFO,
	IPC_API_WIFI_GET_BAND_TYPE,
	IPC_API_WIFI_GET_AUTO_CHANNEL,
	IPC_API_WIFI_DEL_STA,
	IPC_API_WIFI_AP_CH_SWITCH,
	IPC_API_WIFI_SET_NO_BEACON_TIMEOUT,
	IPC_API_WIFI_CONFIG_CSI,
	IPC_API_WIFI_GET_CSI_REPORT,
	//inter
	IPC_API_WIFI_COEX_SET_PTA,
	IPC_API_WIFI_SET_WPA_MODE,
	IPC_API_WIFI_SET_PMF_MODE,
	IPC_API_WIFI_COEX_BT_RFK,
	IPC_API_WIFI_COEX_ZB_RFK,
	IPC_API_WIFI_SET_BT_SEL,
	IPC_API_WIFI_ADD_KEY,
	IPC_API_WIFI_GET_CHPLAN,
	IPC_API_WPA_4WAY_FAIL,
	IPC_API_WPA_PMKSA_OPS,

	//promisc
	IPC_API_WIFI_PROMISC_INIT,
	//nan
	IPC_API_NAN_START,
	IPC_API_NAN_STOP,
	IPC_API_NAN_ADD_FUNC,
	IPC_API_NAN_DEL_FUNC,
	IPC_API_NAN_CFGVENFOR,
	//misc
	IPC_API_WIFI_IP_UPDATE,
	IPC_API_WIFI_IWPRIV_INFO, //dbg cmd
	IPC_API_WIFI_MP_CMD, //mp cmd
	IPC_API_WIFI_GET_NETWORK_MODE,

	IPC_API_WIFI_MSG_TO,
	IPC_API_BUFFERED_PRINTF_SET_NP_EN
};

enum IPC_LWIP_INFO_TYPE {
	IPC_WLAN_GET_IP,
	IPC_WLAN_GET_GW,
	IPC_WLAN_GET_GWMSK,
	IPC_WLAN_GET_HW_ADDR,
	IPC_WLAN_IS_VALID_IP
};

typedef struct inic_ipc_host_request_message {
	u32	API_ID;
	u32	param_buf[HOST_MSG_PARAM_NUM];
	int	ret;
	u8 dummy[HOST_MSG_DUMY_NUM];//add for 64B size alignment
} inic_ipc_host_request_message;

typedef struct inic_ipc_dev_request_message {
	u32	EVENT_ID;
	u32	param_buf[DEV_MSG_PARAM_NUM];
	int ret;
	u8 dummy[DEV_MSG_DUMY_NUM]; /* add for 64B size alignment */
} inic_ipc_dev_request_message;


/* -------------------------- Function declaration -------------------------- */
/*for ipc host api*/
void inic_ipc_api_init_host(VOID);
void inic_ipc_host_deinit_skb(void);
void inic_ipc_api_host_int_hdl(VOID *Data, u32 IrqStatus, u32 ChanNum);
int inic_ipc_api_host_message_send(u32 id, u32 *param_buf, u32 buf_len);

/*for ipc dev api*/
void inic_ipc_api_init_dev(VOID);
void inic_ipc_api_dev_int_hdl(VOID *Data, u32 IrqStatus, u32 ChanNum);
void inic_ipc_wifi_event_indicate(int event_cmd, char *buf, int buf_len, int flags);
void inic_ipc_autoreconnect_indicate(rtw_security_t security_type, u8 *ssid, int ssid_len, u8 *password, int password_len, int key_id, char is_wps_trigger);
void inic_ipc_eap_autoreconnect_indicate(u8 saved_eap_method);
void inic_ipc_scan_user_callback_indicate(unsigned int ap_num, void *user_data);
void inic_ipc_scan_each_report_user_callback_indicate(rtw_scan_result_t *scanned_ap_info, void *user_data);
void inic_ipc_promisc_callback_indicate(void *pkt_info);
void inic_ipc_ap_ch_switch_callback_indicate(unsigned char channel, rtw_channel_switch_res_t ret);
int inic_ipc_dev_set_netif_info(int idx_wlan, unsigned char *dev_addr);
int inic_ipc_get_lwip_info(u32 type, unsigned char *input, int index);
u64 inic_ipc_host_get_wifi_tsf(unsigned char port_id);
int inic_ipc_host_get_txbuf_pkt_num(void);
void inic_ipc_cfg80211_indicate_scan_report(u32 channel, u32 frame_is_bcn, s32 rssi, u8 *mac_addr, u8 *IEs, u32 ie_len);
extern void inic_ipc_host_init_skb(VOID);
#ifdef CONFIG_NAN
void inic_ipc_cfg80211_indicate_nan_match(u8 type, u8 inst_id, u8 peer_inst_id, u8 *addr, u32 info_len, u8 *info, u64 cookie);
void inic_ipc_cfg80211_nan_func_free(void  *data);
void inic_ipc_cfg80211_nan_cfgvendor_event_report(u16 event_id, void *event, int event_len);
void inic_ipc_cfg80211_cfgvendor_send_cmd_reply(void *data, int len);
#endif
int inic_ip_in_table_indicate(u8 gate, u8 ip);

#endif /* __INIC_IPC_H__ */
