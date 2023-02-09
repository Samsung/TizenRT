/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2020 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
/******************************************************************************
 * history *
 * 2020.11.20 | Andrew Su | Modification after first review.
 * 1, Change p_inic_ipc_host_send to local variable from global, it will be
 *    more safe and not need protect.
 * 2, Delete irq_th_lock, not need protect in interrupt.
******************************************************************************/

/* -------------------------------- Includes -------------------------------- */
#include "ameba_soc.h"
#include "platform_stdlib.h"
#include "osdep_service.h"
#include "skbuff.h"

#include "inic_ipc_dev_trx.h"
#include "inic_ipc_msg_queue.h"
#define CONFIG_ENABLE_CACHE
/* ---------------------------- Private Functions --------------------------- */
/**
 * @brief  to haddle the message of IPC.
 * @param  none.
 * @return none.
 */
static void inic_ipc_dev_task_hdl(inic_ipc_ex_msg_t *p_ipc_msg)
{
	if (p_ipc_msg == NULL) {
		DBG_8195A("Host IPC message is NULL, invalid!\n\r");
		return;
	}

	switch (p_ipc_msg->event_num) {
	case IPC_WIFI_CMD_XIMT_PKTS:
		/* Above messages will take muck time and cause the
		 * pending. So they will be handdled in task not in
		 * interrupt.
		 */
		inic_ipc_dev_tx_handler(p_ipc_msg);
		break;
	case IPC_WIFI_MSG_ALLOC_SKB:
		inic_ipc_dev_alloc_tx_skb(p_ipc_msg->msg_len, p_ipc_msg->msg_addr);
		break;
	case IPC_WIFI_MSG_RECV_DONE:
		inic_ipc_dev_rx_done(p_ipc_msg);
		break;
	case IPC_WIFI_MSG_RECV_PENDING:
		inic_ipc_dev_rx_pending();
		break;
	default:
		DBG_8195A("Device Unknown Event(%d)!\n", \
				  p_ipc_msg->event_num);
		break;
	}
}

/* ---------------------------- Public Functions ---------------------------- */
/**
 * @brief  to haddle the ipc message interrupt. If the message queue is
 * 	initialized, it will enqueue the ipc message and wake up the message
 * 	task to haddle the message. If last send message cannot be done, I will
 * 	set pending for next sending message.
 * @param  Data[inout]: IPC data.
 * @param  IrqStatus[in]: interrupt status.
 * @param  ChanNum[in]: IPC channel number.
 * @return none.
 */
void inic_ipc_dev_event_int_hdl(VOID *Data, u32 IrqStatus, u32 ChanNum)
{
	UNUSED(Data);
	UNUSED(IrqStatus);
	UNUSED(ChanNum);

	inic_ipc_ex_msg_t *p_ipc_msg = NULL;
	sint ret = FAIL;

#ifdef IPC_DIR_MSG_RX
	PIPC_MSG_STRUCT p_ipc_recv_msg = ipc_get_message(IPC_DIR_MSG_RX, \
									 IPC_H2D_WIFI_TRX_TRAN);
	p_ipc_msg = (inic_ipc_ex_msg_t *)p_ipc_recv_msg->msg;
#else
	p_ipc_msg = (inic_ipc_ex_msg_t *)ipc_get_message(IPC_INT_CHAN_WIFI_TRX_TRAN);
#endif /* IPC_DIR_MSG_RX */

#ifdef CONFIG_ENABLE_CACHE
	DCache_Invalidate((u32)p_ipc_msg, sizeof(inic_ipc_ex_msg_t));
#endif /* CONFIG_ENABLE_CACHE */

	if (inic_ipc_msg_get_queue_status()) {
		/* put the ipc message to the queue */
		ret = inic_ipc_msg_enqueue(p_ipc_msg);
	} else {
		/* the message queue does't work, call haddle function
		 * directly */
		inic_ipc_dev_task_hdl(p_ipc_msg);
		ret = _SUCCESS;
	}

	if (ret == _SUCCESS) {
		p_ipc_msg->msg_len = 0;
	} else {
		p_ipc_msg->msg_len = IPC_WIFI_MSG_MEMORY_NOT_ENOUGH;
	}
	/* enqueuing message is seccussful, send acknowledgement to another port*/
	p_ipc_msg->event_num = IPC_WIFI_MSG_READ_DONE;
#ifdef CONFIG_ENABLE_CACHE
	DCache_Clean((u32)p_ipc_msg, sizeof(inic_ipc_ex_msg_t));
#endif /* CONFIG_ENABLE_CACHE */
}

/**
 * @brief  to initialize the ipc device for the inic.
 * @param  none.
 * @return none.
 */
void inic_ipc_init(void)
{

	/* initialize the message queue, and assign the task haddle function */
	inic_ipc_msg_q_init(inic_ipc_dev_task_hdl);

	/* initialize the dev priv */
	inic_ipc_dev_init_priv();
}

/* ---------------------------- Global Variables ---------------------------- */
#if defined(CONFIG_PLATFORM_AMEBALITE) || defined(CONFIG_PLATFORM_AMEBAD2)
IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE   ipc_dev_event_table[] = {
	{IPC_USER_POINT,	inic_ipc_dev_event_int_hdl, (VOID *) NULL, IPC_DIR_MSG_RX, IPC_H2D_WIFI_TRX_TRAN, IPC_RX_FULL},
};
#endif