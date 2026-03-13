/**
  ******************************************************************************
  * @file    inic_ipc.h
  * @author
  * @version
  * @date
  * @brief   Routines to access hardware
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef __WHC_IPC_H__
#define __WHC_IPC_H__

/* -------------------------------- Includes -------------------------------- */
#ifndef CONFIG_FULLMAC
#include "rtw_wifi_common.h"
#include "platform_autoconf.h"
#endif
#include "whc_def.h"
#ifndef CONFIG_FULLMAC
#include "ameba_soc.h"
#include "os_wrapper.h"
#include "wifi_api.h"
#include "wifi_intf_drv_to_app_internal.h"
#ifdef CONFIG_WIFI_TUNNEL
#include "wifi_api_wtn.h"
#endif
#endif
/* -------------------------------- Defines --------------------------------- */
/*msg q task*/
#define CONFIG_INIC_IPC_MSG_Q_PRI	(6)
#if defined(CONFIG_WHC_HOST)
#define WIFI_STACK_SIZE_INIC_MSG_Q	(608 + 128 + CONTEXT_SAVE_SIZE) /* max 600 in smart */
#elif defined(CONFIG_WHC_DEV)
#define WIFI_STACK_SIZE_INIC_MSG_Q	(688 + 128 + CONTEXT_SAVE_SIZE) /* max 688 in smart */
#endif

/*host api task*/
#define CONFIG_INIC_IPC_HOST_API_PRIO 3
#define CONFIG_INIC_IPC_HOST_EVT_API_PRIO 3
#define WIFI_STACK_SIZE_INIC_IPC_HST_API (400 + 128 + CONTEXT_SAVE_SIZE)	// for psp overflow when update group key: jira: https://jira.realtek.com/browse/RSWLANQC-1027
#define WIFI_STACK_SIZE_INIC_IPC_HST_EVT_API (3096 + 128 + CONTEXT_SAVE_SIZE)
#define CONFIG_INIC_IPC_HOST_EVT_Q_DEPTH 10

#define whc_dev_init                                     whc_ipc_dev_init
#define whc_dev_wifi_event_indicate                      whc_ipc_dev_wifi_event_indicate
#define whc_dev_scan_user_callback_indicate              whc_ipc_dev_scan_user_callback_indicate
#define whc_dev_acs_info_indicate                        whc_ipc_dev_acs_info_indicate
#define whc_dev_scan_each_report_user_callback_indicate  whc_ipc_dev_scan_each_report_user_callback_indicate
#define whc_dev_promisc_callback_indicate                whc_ipc_dev_promisc_callback_indicate
#define whc_dev_ip_in_table_indicate                     whc_ipc_dev_ip_in_table_indicate
#define whc_dev_ap_ch_switch_callback_indicate           whc_ipc_dev_ap_ch_switch_callback_indicate
#define whc_dev_get_lwip_info                            whc_ipc_dev_get_lwip_info
#define whc_dev_set_netif_info                           whc_ipc_dev_set_netif_info
#define whc_dev_cfg80211_indicate_scan_report            whc_ipc_dev_cfg80211_indicate_scan_report
#define whc_dev_update_regd_event_indicate               whc_ipc_dev_update_regd_event_indicate
#define whc_dev_api_init                                 whc_ipc_dev_api_init

#ifndef CONFIG_FULLMAC
#define whc_host_init                                    whc_ipc_host_init
#define whc_host_api_message_send                        whc_ipc_host_api_message_send
#define whc_host_init_skb                                whc_ipc_host_init_skb
#endif

#ifdef CONFIG_NAN
#define whc_dev_cfg80211_indicate_nan_match              whc_ipc_dev_cfg80211_indicate_nan_match
#define whc_dev_cfg80211_nan_func_free                   whc_ipc_dev_cfg80211_nan_func_free
#define whc_dev_cfg80211_nan_cfgvendor_event_report      whc_ipc_dev_cfg80211_nan_cfgvendor_event_report
#define whc_dev_cfg80211_cfgvendor_send_cmd_reply        whc_ipc_dev_cfg80211_cfgvendor_send_cmd_reply
#endif

#ifdef CONFIG_WIFI_TUNNEL
#define whc_dev_wtn_rnat_ap_init                         whc_ipc_dev_wtn_rnat_ap_init
#define whc_dev_wtn_ota_callback_indicate                whc_ipc_dev_wtn_ota_callback_indicate
#ifdef CONFIG_WTN_SOCKET_APP
#define whc_dev_wtn_socket_send                          whc_ipc_dev_wtn_socket_send
#define whc_dev_wtn_socket_init                          whc_ipc_dev_wtn_socket_init
#endif
#endif
/* -------------------------------- Macros ---------------------------------- */
#define FLAG_WLAN_IF_NOT_RUNNING		0xFFFFFFFF

#define HOST_MSG_PARAM_NUM		(9)
#define HOST_MSG_DUMY_NUM		(64 - (HOST_MSG_PARAM_NUM + 2) * 4)
#define DEV_MSG_PARAM_NUM		(8)
#define DEV_MSG_DUMY_NUM		(64 - (DEV_MSG_PARAM_NUM + 2) * 4)

/* ------------------------------- Data Types ------------------------------- */
enum IPC_WIFI_CTRL_TYPE {
	IPC_WIFI_MSG_READ_DONE = 0,
	IPC_WIFI_MSG_MEMORY_NOT_ENOUGH = 1,
	IPC_WIFI_MSG_RECV_DONE = 2,
	IPC_WIFI_CMD_XIMT_PKTS = 3,
	IPC_WIFI_EVT_RECV_PKTS = 4,
	IPC_WIFI_EVT_TX_DONE = 5
};

struct whc_ipc_dev_req_msg {
	u32	enevt_id;
	u32	param_buf[DEV_MSG_PARAM_NUM];
	int	ret;
	u8	dummy[DEV_MSG_DUMY_NUM]; /* add for 64B size alignment */
};

struct whc_ipc_host_req_msg {
	u32	api_id;
	u32	param_buf[HOST_MSG_PARAM_NUM];
	int	ret;
	u8 dummy[HOST_MSG_DUMY_NUM];//add for 64B size alignment
};

struct whc_ipc_ex_msg {
	u32	event_num;
	u32	msg_addr;
	u32 msg_queue_status;
	u32	wlan_idx;
	u32 rsvd[12]; /* keep total size 64B aligned */
};

struct ipc_msg_node {
	struct list_head		list;
	u32	msg_addr;
	u8	is_used				: 1;
	u8	wlan_idx			: 2;
	u8	event_num			: 5;
};

#ifndef CONFIG_FULLMAC
struct whc_ipc_host_unblk_api_q_priv {
	struct list_head	host_unblk_api_queue;
	rtos_sema_t			host_unblk_api_q_sema;
	rtos_mutex_t		host_unblk_api_q_mutex;
	u8					host_unblk_api_node_cnt;
};

struct whc_ipc_host_unblk_api_node {
	struct list_head	list;
	u32					event_id;
	u32					param_buf[HOST_MSG_PARAM_NUM];
};

struct ipc_host_unblk_api_func_t {
	u32 host_evt_id;
	void (*func)(struct whc_ipc_host_unblk_api_node *p_unblk_api_node);
	u8 free_idx;	/* indicate whether there are buffers that need to free, 0xFF means no need free */
};
#endif

/* -------------------------- Function declaration -------------------------- */
#ifndef CONFIG_FULLMAC
/*for ipc host*/
void whc_ipc_host_init(void);
void whc_ipc_host_trx_int_hdl(void *Data, u32 IrqStatus, u32 ChanNum);
void whc_ipc_host_heap_statistics(u8 start);

/*for ipc host api*/
void whc_ipc_host_api_init(void);
void whc_ipc_host_deinit_skb(void);
void whc_ipc_host_api_int_hdl(void *Data, u32 IrqStatus, u32 ChanNum);
int whc_ipc_host_api_message_send(u32 id, u32 *param_buf, u32 buf_len);
#endif

/*for ipc device*/
void whc_ipc_dev_init(void);
void whc_ipc_dev_trx_int_hdl(void *Data, u32 IrqStatus, u32 ChanNum);

/*for ipc dev api*/
void whc_ipc_dev_api_init(void);
void whc_ipc_dev_api_int_hdl(void *Data, u32 IrqStatus, u32 ChanNum);
void whc_ipc_dev_wifi_event_indicate(u32 event_cmd, u8 *evt_info, s32 evt_len);
void whc_ipc_dev_scan_user_callback_indicate(unsigned int ap_num, void *user_data);
void whc_ipc_dev_acs_info_indicate(struct rtw_acs_mntr_rpt *acs_mntr_rpt);
void whc_ipc_dev_scan_each_report_user_callback_indicate(struct rtw_scan_result *scanned_ap_info, void *user_data);
u8 whc_ipc_dev_promisc_callback_indicate(struct rtw_rx_pkt_info *pkt_info);
void whc_ipc_dev_ap_ch_switch_callback_indicate(unsigned char channel, s8 ret);
void whc_ipc_dev_update_regd_event_indicate(struct rtw_country_code_table *table);
int whc_ipc_dev_set_netif_info(int idx_wlan, unsigned char *dev_addr);
int whc_dev_get_lwip_info(u32 type, unsigned char *input, int index);
u64 whc_ipc_host_api_get_wifi_tsf(unsigned char port_id);
s32 whc_ipc_host_api_get_wifi_latched_tsf_i2s(struct rtw_speaker_read_latch_req *req, struct rtw_speaker_read_latch_rpt *rpt);
int whc_ipc_host_api_get_txbuf_pkt_num(void);
void whc_ipc_dev_cfg80211_indicate_scan_report(u32 channel, u32 frame_is_bcn, s32 rssi, u8 *mac_addr, u8 *IEs, u32 ie_len);
void whc_ipc_host_init_skb(void);
#ifdef CONFIG_NAN
void whc_ipc_dev_cfg80211_indicate_nan_match(u8 type, u8 inst_id, u8 peer_inst_id, u8 *addr, u32 info_len, u8 *info, u64 cookie);
void whc_ipc_dev_cfg80211_nan_func_free(u64 data);
void whc_ipc_dev_cfg80211_nan_cfgvendor_event_report(u16 event_id, void *event, int event_len);
void whc_ipc_dev_cfg80211_cfgvendor_send_cmd_reply(void *data, int len);
#endif
#ifdef CONFIG_P2P
void whc_ipc_dev_cfg80211_indicate_channel_ready(void *scan_userdata);
#endif
int whc_ipc_dev_ip_in_table_indicate(u8 gate, u8 ip);
#ifdef CONFIG_WIFI_TUNNEL
void whc_ipc_dev_wtn_rnat_ap_init(u8 enable);
int whc_ipc_dev_wtn_ota_callback_indicate(u8 *buf, u16 len);
#ifdef CONFIG_WTN_SOCKET_APP
int whc_ipc_dev_wtn_socket_send(u8 *buf, u32 len);
void whc_ipc_dev_wtn_socket_init(u8 enable, u8 rnat_ap_start);
#endif
#endif
#endif /* __INIC_IPC_H__ */
