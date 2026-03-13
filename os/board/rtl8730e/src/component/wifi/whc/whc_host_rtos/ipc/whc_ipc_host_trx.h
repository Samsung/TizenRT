/**
  ******************************************************************************
  * @file    inic_ipc_host_trx.h
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

#ifndef __WHC_IPC_HOST_TRX_H__
#define __WHC_IPC_HOST_TRX_H__
/* -------------------------------- Includes -------------------------------- */
/* external head files */
#include "platform_autoconf.h"
#include "os_wrapper.h"
#include "rtw_skbuff.h"
#ifndef ZEPHYR_WIFI
#ifdef CONFIG_PLATFORM_TIZENRT_OS
#include "ethernetif_tizenrt.h" //maybe needed to lead to pbuf definition
#else
#include "ethernetif.h"
#endif //CONFIG_PLATFORM_TIZENRT_OS
// #include "lwip_netconf.h" // need to check if needed, originally used for LwIP_ethernetif_recv_inic() but technically this will not be used in host_trx.c because we need a wrapper for this in tizenrt
#endif
#include "rtw_queue.h"
#include "rtw_atomic.h"

/* internal head files */
#include "whc_ipc.h"

/* -------------------------------- Defines --------------------------------- */
#define SKB_DATA_ALIGN(X)	(((X) + (4 - 1)) & ~(4 - 1))
/* 308 from DP asm 2024/4/11 */
#define WIFI_STACK_SIZE_INIC_TRX_HST	(512 + CONTEXT_SAVE_SIZE)

// #ifndef CONFIG_AMEBAL2
// #define WHC_RX_SKB_SHARE_TO_PBUF
// #endif
/* -------------------------------- Macros ---------------------------------- */
/* ------------------------------- Data Types ------------------------------- */
/* recv buffer to store the data from IPC to queue. */
struct host_recv_buf {
	struct list_head list;
	int idx_wlan; /* index for wlan */
	struct pbuf *p_buf; /* rx data for ethernet buffer*/
};

/* recv structure */
struct host_priv {
	rtos_sema_t recv_sema; /* sema to wait allloc skb from device */
	rtos_sema_t alloc_skb_sema; /* sema to wait allloc skb from device */
	rtos_sema_t host_send_sema; /* sema to protect inic ipc host send */
	rtos_timer_t whc_stats_timer; /* sema to protect inic ipc host send */
	struct __queue recv_queue; /* recv queue */
	u32 rx_bytes; /* recv bytes */
	u32 rx_pkts; /* recv number of packets */
	u32 tx_bytes; /* xmit bytes */
	u32 tx_pkts; /* xmit number of packets */
	u32 tx_drop_pkts;
	u8 host_init_done: 1;
};

struct rtw_pbuf {
	struct pbuf_custom p;
	struct sk_buff *skb;
	u8 busy;
};

#define whc_host_send       whc_ipc_host_send
/* ---------------------------- Global Variables ---------------------------- */

/* -------------------------- Function declaration -------------------------- */
void whc_ipc_host_init_skb(void);
void whc_ipc_host_init_rtwpbuf_pool(void);
void whc_ipc_host_init_priv(void);
void whc_ipc_host_rx_handler(int idx_wlan, struct sk_buff *skb);
int whc_ipc_host_send(int idx, struct eth_drv_sg *sg_list, int sg_len, int total_len, struct skb_raw_para *raw_para, u8 is_special_pkt);
void whc_ipc_host_trx_event_hdl(u8 event_num, u32 msg_addr, u8 wlan_idx);
int whc_ipc_host_send_skb(int idx, struct sk_buff *skb);

#endif /* __INIC_IPC_HOST_TRX_H__ */
