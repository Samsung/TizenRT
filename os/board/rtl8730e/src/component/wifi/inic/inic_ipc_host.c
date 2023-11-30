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
 * 1, Change p_inic_ipc_dev_send to local variable from global, it will be
 *    more safe and not need protect.
 * 2, Delete irq_th_lock, not need protect in interrupt.
 * 3, Add case in interrupt to wakeup the block to ensure tx done.
 * 2020.12.09 | Andrew Su | Move the command to inic_cmd.c
******************************************************************************/
#include "inic_ipc_host_trx.h"
#include "inic_ipc_msg_queue.h"
#define CONFIG_ENABLE_CACHE

/* -------------------------------- Defines --------------------------------- */

/* -------------------------------- Macros ---------------------------------- */

/* ---------------------------- Global Variables ---------------------------- */

/* --------------------------- Private Variables ---------------------------- */

/* ---------------------------- Private Functions --------------------------- */
/**
 * @brief  haddle the message of IPC.
 * @param  none.
 * @return none.
 */
static void inic_ipc_host_task_hdl(inic_ipc_ex_msg_t *p_ipc_msg)
{
	if (p_ipc_msg == NULL) {
		DBG_8195A("Device IPC message is NULL, invalid!\n\r");
		return;
	}

	switch (p_ipc_msg->event_num) {
	/* receive the data from device */
	case IPC_WIFI_EVT_RECV_PKTS:
		inic_ipc_host_rx_handler(p_ipc_msg->wlan_idx,
								 (struct sk_buff *)(p_ipc_msg->msg_addr));
		break;
	/* other contrl operations */
	default:
		DBG_8195A("Host Unknown event(%d)!\n\r", \
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
void inic_ipc_host_event_int_hdl(VOID *Data, u32 IrqStatus, u32 ChanNum)
{
	(void) Data;
	(void) IrqStatus;
	(void) ChanNum;

	inic_ipc_ex_msg_t *p_ipc_msg = NULL;
	sint ret = FAIL;

#ifdef IPC_DIR_MSG_RX
	PIPC_MSG_STRUCT p_ipc_recv_msg = ipc_get_message(IPC_DIR_MSG_RX, \
									 IPC_D2H_WIFI_TRX_TRAN);
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
		inic_ipc_host_task_hdl(p_ipc_msg);
		ret = _SUCCESS;
	}

	if (ret == _SUCCESS) {
		p_ipc_msg->msg_queue_status = 0;
	} else {
		p_ipc_msg->msg_queue_status = IPC_WIFI_MSG_MEMORY_NOT_ENOUGH;
	}
	/* enqueuing message is seccussful, send acknowledgement to another port*/
	p_ipc_msg->event_num = IPC_WIFI_MSG_READ_DONE;
#ifdef CONFIG_ENABLE_CACHE
	DCache_Clean((u32)p_ipc_msg, sizeof(inic_ipc_ex_msg_t));
#endif /* CONFIG_ENABLE_CACHE */
}

/**
 * @brief  to initialize the ipc host for the inic.
 * @param  none.
 * @return none.
 */
void inic_ipc_init_host(VOID)
{
	/* initialize the message queue, and assign the task haddle function */
	inic_ipc_msg_q_init(inic_ipc_host_task_hdl);

	/* Initialize the parameters of ipc host. */
	inic_ipc_host_init_priv();
}
/* ---------------------------- Global Variables ---------------------------- */
#if defined(CONFIG_PLATFORM_AMEBALITE) || defined(CONFIG_PLATFORM_AMEBAD2) || defined(CONFIG_PLATFORM_AMEBADPLUS) || defined(CONFIG_PLATFORM_RTL8720F)
IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE ipc_host_event_table = {
	.USER_MSG_TYPE = IPC_USER_POINT,
	.Rxfunc = inic_ipc_host_event_int_hdl,
	.RxIrqData = (VOID *) NULL,
	.Txfunc = IPC_TXHandler,
	.TxIrqData = (VOID *) NULL,
	.IPC_Direction = IPC_DIR_MSG_RX,
	.IPC_Channel = IPC_D2H_WIFI_TRX_TRAN
};
#endif