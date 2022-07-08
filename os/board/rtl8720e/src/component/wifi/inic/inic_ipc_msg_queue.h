/******************************************************************************
 *
 * Copyright(c) 2020 - 2021 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
#ifndef __INIC_IPC_MSG_QUEUE_H__
#define __INIC_IPC_MSG_QUEUE_H__
/* -------------------------------- Includes -------------------------------- */
/* external head files */
//#include "freertos_service.h"
#include "osdep_service.h"
#include "platform_stdlib.h"
#include "platform_opts.h"
#include "skbuff.h"

/* internal head files */
#include "inic_ipc.h"

/* -------------------------------- Defines --------------------------------- */
#define CONFIG_INIC_IPC_MSG_Q_PRI	(7)

/* -------------------------------- Macros ---------------------------------- */

/* ------------------------------- Data Types ------------------------------- */

/* ---------------------------- Global Variables ---------------------------- */

/* -------------------------- Function declaration -------------------------- */
extern int rtw_in_interrupt(void);
void inic_ipc_msg_q_init(void (*task_hdl)(inic_ipc_ex_msg_t *));
void inic_ipc_msgQ_wlan_task_deinit(void);
sint inic_ipc_msg_enqueue(inic_ipc_ex_msg_t *p_ipc_msg);
void inic_ipc_msg_q_deinit(void);
u8 inic_ipc_msg_get_queue_status(void);
void inic_ipc_ipc_send_msg(inic_ipc_ex_msg_t *p_ipc_msg);

#endif /* __INIC_IPC_MSG_QUEUE_H__ */
