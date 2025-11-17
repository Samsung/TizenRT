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

#ifndef __INIC_IPC_HOST_TRX_H__
#define __INIC_IPC_HOST_TRX_H__
/* -------------------------------- Includes -------------------------------- */
/* external head files */
#include "platform_autoconf.h"
#include "os_wrapper.h"
#include "rtw_skbuff.h"
#ifdef CONFIG_PLATFORM_TIZENRT_OS
#include "ethernetif_tizenrt.h"
#else
#include "ethernetif.h"
#endif //CONFIG_PLATFORM_TIZENRT_OS
#include "lwip_netconf.h"
#include "rtw_queue.h"
#include "rtw_atomic.h"

/* internal head files */
#include "inic_ipc.h"

/* -------------------------------- Defines --------------------------------- */
#define SKB_DATA_ALIGN(X)	(((X) + (4 - 1)) & ~(4 - 1))
/* 308 from DP asm 2024/4/11 */
#define WIFI_STACK_SIZE_INIC_TRX_HST	(512 + CONTEXT_SAVE_SIZE)

#define INIC_SKIP_RX_TASK
/* -------------------------------- Macros ---------------------------------- */
/* ------------------------------- Data Types ------------------------------- */

/* ---------------------------- Global Variables ---------------------------- */

/* -------------------------- Function declaration -------------------------- */
void inic_host_init_skb(void);
void inic_host_init_priv(void);
void inic_host_rx_handler(int idx_wlan, struct sk_buff *skb);
int inic_host_send(int idx, struct eth_drv_sg *sg_list, int sg_len, int total_len, struct skb_raw_para *raw_para, u8 is_special_pkt);
void inic_host_trx_event_hdl(u8 event_num, u32 msg_addr, u8 wlan_idx);


#endif /* __INIC_IPC_HOST_TRX_H__ */
