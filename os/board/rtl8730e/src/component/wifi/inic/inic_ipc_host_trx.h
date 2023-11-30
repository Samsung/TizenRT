/******************************************************************************
 *
 * Copyright(c) 2020 - 2021 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
#ifndef __INIC_IPC_HOST_TRX_H__
#define __INIC_IPC_HOST_TRX_H__
/* -------------------------------- Includes -------------------------------- */
/* external head files */
#include "osdep_service.h"
#include "platform_opts.h"
#include "rtw_skbuff.h"
// #include "ethernetif.h"
#include "lwip_netconf.h"

/* internal head files */
#include "inic_ipc.h"

/* -------------------------------- Defines --------------------------------- */
#define SKB_DATA_ALIGN(X)	(((X) + (4 - 1)) & ~(4 - 1))

/* -------------------------------- Macros ---------------------------------- */
/* ------------------------------- Data Types ------------------------------- */

/* ---------------------------- Global Variables ---------------------------- */

/* -------------------------- Function declaration -------------------------- */
void inic_ipc_host_init_skb(void);
void inic_ipc_host_init_priv(void);
void inic_ipc_host_rx_handler(int idx_wlan, struct sk_buff *skb);
void inic_ipc_host_tx_alloc_skb_rsp(inic_ipc_ex_msg_t *p_ipc_msg);
int inic_ipc_host_send(int idx, struct eth_drv_sg *sg_list, int sg_len, int total_len, struct skb_raw_para *raw_para);

#endif /* __INIC_IPC_HOST_TRX_H__ */
