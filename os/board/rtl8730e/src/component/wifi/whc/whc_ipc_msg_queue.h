/**
  ******************************************************************************
  * @file    inic_ipc_msg_queue.h
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

#ifndef __WHC_IPC_MSG_QUEUE_H__
#define __WHC_IPC_MSG_QUEUE_H__
/* -------------------------------- Includes -------------------------------- */
/* external head files */
#include "os_wrapper.h"
#include "rtw_queue.h"

/* internal head files */
#include "whc_ipc.h"

/* message queue priv */
struct ipc_msg_q_priv {
	struct __queue msg_queue; /* msg queue */
	rtos_sema_t msg_q_sema;
	rtos_sema_t msg_send_sema;
	u8 b_queue_working; /* flag to notice the queue is working */
	struct ipc_msg_node *ipc_msg_pool; /* dynamic pool for queue node */
	u8 ipc_msg_node_max;
};

/* -------------------------- Function declaration -------------------------- */
void whc_msg_q_init(void);
sint whc_msg_enqueue(struct whc_ipc_ex_msg *p_ipc_msg);
void whc_msg_q_deinit(void);
u8 whc_msg_get_queue_status(void);
void whc_ipc_send_msg(u32 event_num, u32 msg_addr, u32 msg_queue_status, u32 wlan_idx);

#endif /* __INIC_IPC_MSG_QUEUE_H__ */
