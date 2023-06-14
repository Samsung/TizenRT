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
 * 2020.11.20 | Andrew Su | Modification after first review.
 * 1, Add IPC_WIFI_MSG_XMIT_DONE in IPC_WIFI_CTRL_TYPE to notice the tx is
 * done in device port.
******************************************************************************/

#ifndef __INIC_IPC_H__
#define __INIC_IPC_H__

/* -------------------------------- Includes -------------------------------- */
#include "platform_opts.h"

#include "ameba_soc.h"
#include "osdep_service.h"
#include "rtw_wifi_constants.h"
#include "rtw_skbuff.h"

#include "inic_ipc_cfg.h"

/* -------------------------------- Defines --------------------------------- */
#define _rtl_printf			DBG_8195A
#define rtw_memcpy			_memcpy
#define FLAG_WLAN_IF_NOT_RUNNING		0xFFFFFFFF
/* -------------------------------- Macros ---------------------------------- */

/* ------------------------------- Data Types ------------------------------- */
typedef struct inic_ipc_ex_msg {
	u32	event_num;
	u32	msg_addr;
	u32 msg_queue_status;
	u32	wlan_idx;
	u32 rsvd[12]; /* keep total size 64B aligned */
} inic_ipc_ex_msg_t;

enum IPC_WIFI_CTRL_TYPE {
	IPC_WIFI_MSG_READ_DONE = 0,
	IPC_WIFI_MSG_MEMORY_NOT_ENOUGH,
	IPC_WIFI_MSG_RECV_DONE,
	IPC_WIFI_CMD_XIMT_PKTS,
	IPC_WIFI_EVT_RECV_PKTS
};

/* -------------------------- Function declaration -------------------------- */
/*for ipc host*/
void inic_ipc_init_host(VOID);
void inic_ipc_host_event_int_hdl(VOID *Data, u32 IrqStatus, u32 ChanNum);

/*for ipc device*/
void inic_ipc_init(VOID);
void inic_ipc_dev_event_int_hdl(VOID *Data, u32 IrqStatus, u32 ChanNum);

#endif /* __INIC_IPC_H__ */
