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
#include <platform_stdlib.h>
#include "platform_opts.h"

#include "ameba_soc.h"
#include "platform_stdlib.h"
#include "osdep_service.h"
#include "wifi_structures.h"
#include "skbuff.h"

#include "inic_ipc_cfg.h"

/* -------------------------------- Defines --------------------------------- */
#define _rtl_printf			DBG_8195A
#define rtw_memcpy			_memcpy
#define FLAG_WLAN_IF_NOT_RUNNING		0xFFFFFFFF
/* -------------------------------- Macros ---------------------------------- */
#if defined (WIFI_LOGO_CERTIFICATION) && (WIFI_LOGO_CERTIFICATION)
#define MAX_SKB_BUF_SIZE	4144
#elif defined(CONFIG_PLATFORM_AMEBAD2)
// customer requirement, send udp multicast frames around 1900 bytes without fragmentation
#define MAX_SKB_BUF_SIZE	2048
#else
#define MAX_SKB_BUF_SIZE	1658
#endif
/* ------------------------------- Data Types ------------------------------- */
typedef struct inic_ipc_ex_msg {
	u32	event_num;
	u32	msg_addr;
	u32	msg_len;
	u32 rsvd[13]; /* keep total size 64B aligned */
} inic_ipc_ex_msg_t;

enum IPC_WIFI_CTRL_TYPE {
	IPC_WIFI_MSG_READ_DONE = 0,
	IPC_WIFI_MSG_MEMORY_NOT_ENOUGH,
	IPC_WIFI_MSG_ALLOC_SKB,
	IPC_WIFI_MSG_RECV_DONE,
	IPC_WIFI_MSG_RECV_PENDING,
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
