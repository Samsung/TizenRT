/******************************************************************************
 *
 * Copyright(c) 2020 - 2021 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
#ifndef __INIC_IPC_DEV_TX_H__
#define __INIC_IPC_DEV_TX_H__
/* -------------------------------- Includes -------------------------------- */
/* external head files */
#include "freertos_service.h"
#include "osdep_service.h"
#include "platform_stdlib.h"
#include "platform_opts.h"
#include "skbuff.h"
#include "lwip_intf.h"

/* internal head files */
#include "inic_ipc.h"

/* -------------------------------- Defines --------------------------------- */

/* -------------------------------- Macros ---------------------------------- */

/* ------------------------------- Data Types ------------------------------- */

/* ---------------------------- Global Variables ---------------------------- */

/* -------------------------- Function declaration -------------------------- */
void inic_ipc_dev_init_priv(void);
void inic_ipc_dev_tx_handler(inic_ipc_ex_msg_t *ipc_msg);
void inic_ipc_dev_alloc_tx_skb(int len, int idx);
void inic_ipc_dev_recv(int idx);
void inic_ipc_dev_scan_done(void);
void inic_ipc_dev_rx_done(inic_ipc_ex_msg_t *p_ipc_msg);
void inic_ipc_dev_rx_pending(void);
#endif /* __INIC_IPC_DEV_TX_H__ */
