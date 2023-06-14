/******************************************************************************
 *
 * Copyright(c) 2020 - 2021 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
#ifndef __INIC_IPC_DEV_TX_H__
#define __INIC_IPC_DEV_TX_H__
/* -------------------------------- Includes -------------------------------- */
/* external head files */
#include "osdep_service.h"
#include "platform_opts.h"
#include "rtw_skbuff.h"
#include "wifi_intf_drv_to_lwip.h"

/* internal head files */
#include "inic_ipc.h"

/* -------------------------------- Defines --------------------------------- */
#define CONFIG_INIC_IPC_DEV_XMIT_PRI 5
/* -------------------------------- Macros ---------------------------------- */

/* ------------------------------- Data Types ------------------------------- */

/* ---------------------------- Global Variables ---------------------------- */

/* -------------------------- Function declaration -------------------------- */
void inic_ipc_dev_init_priv(void);
void inic_ipc_dev_tx_handler(inic_ipc_ex_msg_t *ipc_msg);
void inic_ipc_dev_recv(int idx);
void inic_ipc_dev_rx_done(inic_ipc_ex_msg_t *p_ipc_msg);

#endif /* __INIC_IPC_DEV_TX_H__ */
