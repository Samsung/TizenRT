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

#ifndef __INIC_IPC_MSG_QUEUE_H__
#define __INIC_IPC_MSG_QUEUE_H__
/* -------------------------------- Includes -------------------------------- */
/* external head files */
#include "os_wrapper.h"
#include "rtw_queue.h"

/* internal head files */
#include "inic_ipc.h"

#define CONFIG_INIC_IPC_MSG_Q_PRI	(6)

#if defined(CONFIG_AS_INIC_AP)
#define WIFI_STACK_SIZE_INIC_MSG_Q	(600 + 128 + CONTEXT_SAVE_SIZE) /* max 600 in smart */
#elif defined(CONFIG_AS_INIC_NP)
#define WIFI_STACK_SIZE_INIC_MSG_Q	(688 + 128 + CONTEXT_SAVE_SIZE) /* max 688 in smart */
#endif

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
void inic_msg_q_init(void);
sint inic_msg_enqueue(struct inic_ipc_ex_msg *p_ipc_msg);
void inic_msg_q_deinit(void);
u8 inic_msg_get_queue_status(void);
void inic_ipc_send_msg(u32 event_num, u32 msg_addr, u32 msg_queue_status, u32 wlan_idx);

#endif /* __INIC_IPC_MSG_QUEUE_H__ */
