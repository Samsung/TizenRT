/******************************************************************************
 *
 * Copyright(c) 2020 - 2021 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
/******************************************************************************
 * history *
 * 2020.11.20 | Andrew Su | Modification after first review.
 * 1, Delete global variable p_inic_ipc_host_send.
 * 2, Change ipc_msg to variable from pointer, and delete wlan_idx and skb in
 * ipc_dev_tx_buf.
 * 3, Modify the inic_enqueue_xmitbuf.
 * 4, Delete irq_th_lock in ipc_dev_priv.
 * 5, Add reponse in inic_ipc_dev_tx_handler for tx pending in host.
******************************************************************************/
#define __INIC_IPC_DEV_TX_C__

/* -------------------------------- Includes -------------------------------- */
/* external head files */

/* internal head files */
#include "inic_ipc_dev_trx.h"
#include "inic_ipc_msg_queue.h"
#include "wifi_conf.h"
#include "wifi_performance_monitor.h"
#define CONFIG_ENABLE_CACHE

/* -------------------------------- Defines --------------------------------- */

/* -------------------------------- Macros ---------------------------------- */

/* ------------------------------- Data Types ------------------------------- */
/* xmit buffer to store the data from IPC host to queue. */
struct ipc_dev_tx_buf {
	_list list;
	inic_ipc_ex_msg_t ipc_msg; /* to store ipc message from host */
};

/* ipc device tx structure */
struct ipc_dev_priv {
	_sema xmit_sema; /* tx sema */
	_sema rx_pending_sema; /* rx pending sema */
	_sema rx_done_sema; /* sema to notice rx done */
	_queue xmit_queue; /* xmit queue */
	u32 tx_bytes; /* xmit bytes */
	u32 tx_pkts; /* xmit number of packets */
	u8 rx_pending_flag; /* host rx pending flag */
};

struct skb_data {
	/* starting address must be aligned by 32 bytes for km4 cache. */
	struct list_head list __attribute__((aligned(32)));
	unsigned char buf[MAX_SKB_BUF_SIZE];
	/* to protect ref when to invalid cache, its address must be
	 * aligned by 32 bytes. */
	atomic_t ref __attribute__((aligned(32)));
};

struct skb_buf {
	struct list_head list;
	struct sk_buff skb;
	u8 rsvd[10];
};

/* -------------------------- Function declaration -------------------------- */
extern void kfree_skb(struct sk_buff *skb);

/* ---------------------------- Global Variables ---------------------------- */

/* --------------------------- Private Variables ---------------------------- */
static struct ipc_dev_priv g_ipc_dev_priv;

/* ---------------------------- Private Functions --------------------------- */
/**
 * @brief  to enqueue the pxmitbuf into the queue.
 * @param  pxmitbuf[inout]: the xmit buffer enqueued into the queue.
 * @param  queue[inout]: the xmit queue.
 * @return if is OK, return _SUCCESS, failed return _FAIL.
 */
static sint inic_enqueue_xmitbuf(struct ipc_dev_tx_buf *pxmitbuf, _queue *queue)
{
	_irqL irqL;

	rtw_enter_critical(&queue->lock, &irqL);
	rtw_list_insert_tail(&pxmitbuf->list, get_list_head(queue));
	rtw_exit_critical(&queue->lock, &irqL);

	return _SUCCESS;
}

/**
 * @brief  to get the xmit buffer from the xmit queue.
 * @param  queue[inout]: the xmit queue.
 * @return return xmit buffer. if is NULL, the queue is empty.
 */
static struct ipc_dev_tx_buf *inic_dequeue_xmitbuf(_queue *queue)
{
	_irqL irqL;
	struct ipc_dev_tx_buf *pxmitbuf;
	_list *plist, *phead;

	rtw_enter_critical(&queue->lock, &irqL);

	if (rtw_queue_empty(queue) == _TRUE) {
		pxmitbuf = NULL;
	} else {
		phead = get_list_head(queue);
		plist = get_next(phead);
		pxmitbuf = LIST_CONTAINOR(plist, struct ipc_dev_tx_buf, list);
		rtw_list_delete(&pxmitbuf->list);
	}

	rtw_exit_critical(&queue->lock, &irqL);

	return pxmitbuf;
}

/**
 * @brief  to handle the ipc message from the ipc host. They incude tx data and
 *	command.
 * @param  pxmitbuf[inout]: the xmit buffer from xmit queue.
 * @return none.
 */
static void inic_xmit_tasklet_handler(struct ipc_dev_tx_buf *p_xmit_buf)
{
	inic_ipc_ex_msg_t *p_ipc_msg = NULL;
	struct sk_buff *skb = NULL;

	p_ipc_msg = &(p_xmit_buf->ipc_msg);

	switch (p_ipc_msg->event_num) {
	case IPC_WIFI_CMD_XIMT_PKTS:
		skb = (struct sk_buff *)(p_ipc_msg->msg_addr);
#ifdef CONFIG_ENABLE_CACHE
		DCache_Invalidate(((u32)skb - sizeof(struct list_head)), sizeof(struct skb_buf));
		DCache_Invalidate(((u32)skb->head - sizeof(struct list_head)), sizeof(struct skb_data));

#endif /* CONFIG_ENABLE_CACHE */

		g_ipc_dev_priv.tx_bytes += skb->len;
		g_ipc_dev_priv.tx_pkts++;
		WIFI_MONITOR_TIMER_START(wlan_send_skb_time);
		wifi_send_skb(p_ipc_msg->msg_len, skb);
		WIFI_MONITOR_TIMER_END(wlan_send_skb_time, skb->len);
		break;
	default:
		DBG_8195A("Device Unknown Event(%d)!\n", \
				  p_ipc_msg->event_num);
		break;
	}
}

/**
 * @brief  rx task to handle the rx data, get the data from the rx queue and
 * 	send to upper layer.
 * @param  none.
 * @return none.
 */
static void inic_xmit_tasklet(void)
{
	struct ipc_dev_tx_buf *p_xmit_buf = NULL;
	_queue *p_xmit_queue = NULL;

	p_xmit_queue = &g_ipc_dev_priv.xmit_queue;
	do {
		rtw_down_sema(&g_ipc_dev_priv.xmit_sema);

		/* get the data from tx queue. */
		p_xmit_buf = inic_dequeue_xmitbuf(p_xmit_queue);
		while (p_xmit_buf) {
			WIFI_MONITOR_TIMER_START(xmit_handler_time);
			inic_xmit_tasklet_handler(p_xmit_buf);
			WIFI_MONITOR_TIMER_END(xmit_handler_time, 502);

			/* release the memory for this packet. */
			rtw_mfree((u8 *)p_xmit_buf, sizeof(struct ipc_dev_tx_buf));
			p_xmit_buf = inic_dequeue_xmitbuf(p_xmit_queue);
		}
	} while (1);

	vTaskDelete(NULL);
}


/* ---------------------------- Public Functions ---------------------------- */

/**
 * @brief  to initialize the parameters of xmit.
 * @param  none.
 * @return none.
 */
void inic_ipc_dev_init_priv(void)
{
	/* initialize the sema of tx. */
	rtw_init_sema(&g_ipc_dev_priv.xmit_sema, 0);

	/* initialize the sema of rx pending. */
	rtw_init_sema(&g_ipc_dev_priv.rx_pending_sema, 0);

	rtw_init_sema(&(g_ipc_dev_priv.rx_done_sema), 0);

	/* initialize the tx queue. */
	rtw_init_queue(&(g_ipc_dev_priv.xmit_queue));

	g_ipc_dev_priv.tx_bytes = 0;
	g_ipc_dev_priv.tx_pkts = 0;
	g_ipc_dev_priv.rx_pending_flag = 0;

	/* Initialize the tX task */
	if (pdTRUE != xTaskCreate((TaskFunction_t)inic_xmit_tasklet, \
							  (const char *const)"inic_ipc_dev_tx_tasklet", 1024, NULL, \
							  tskIDLE_PRIORITY + CONFIG_INIC_IPC_DEV_XMIT_PRI, NULL)) {
		DBG_8195A("Create inic_ipc_dev_tx_tasklet Err!!\n");
	}
}

/**
 * @brief  to put tx skb buffer into tx queue.
 * @param  p_ipc_msg[inout]: ipc message from host that will be haddled.
 * @return none.
 */
void inic_ipc_dev_tx_handler(inic_ipc_ex_msg_t *p_ipc_msg)
{
	_queue *p_xmit_queue = NULL;
	struct ipc_dev_tx_buf *p_xmit_buf = NULL;

	if (!wifi_is_running((unsigned char)p_ipc_msg->msg_len)) {
		/*free skb and return*/
		struct sk_buff *skb = (struct sk_buff *)p_ipc_msg->msg_addr;
		skb->busy = 0;
		struct skb_buf *skb_buf = container_of(skb, struct skb_buf, skb);
		DCache_Clean((u32)skb_buf, sizeof(struct skb_buf));
		return;
	}

	/* get the tx queue. */
	p_xmit_queue = &(g_ipc_dev_priv.xmit_queue);

	/* allocate xmit buf and associate to the skb */
	p_xmit_buf = (struct ipc_dev_tx_buf *)rtw_zmalloc(sizeof(struct ipc_dev_tx_buf));
	if (p_xmit_buf == NULL) {
		DBG_8195A("Alloc xmit buffer Err!!\n\r");
		//todo: may need send ipc to inform xmit fail other than ximt done
		goto END;
	}

	/* To store the ipc message from host to queue's node. */
	p_xmit_buf->ipc_msg.event_num = p_ipc_msg->event_num;
	p_xmit_buf->ipc_msg.msg_addr = p_ipc_msg->msg_addr;
	p_xmit_buf->ipc_msg.msg_len = p_ipc_msg->msg_len;

	/* enqueue xmit buffer. */
	inic_enqueue_xmitbuf(p_xmit_buf, p_xmit_queue);

END:
	/* wakeup xmit task */
	rtw_up_sema(&g_ipc_dev_priv.xmit_sema);
}

/**
 * @brief  to allocate the skb to store tx data from ipc host.
 * @param  len[in]: length to allocate buffer size.
 * @param  len[idx]: index of wlan interface which want to transmit.
 * @return none.
 */
void inic_ipc_dev_alloc_tx_skb(int len, int idx)
{
	struct sk_buff *skb = NULL;
	inic_ipc_ex_msg_t ipc_msg = {0};

	if (!wifi_is_running((unsigned char)idx)) {
		/* set skb to WLAN_IF_NOT_RUNNING to indicate corresponding wlan interface is not running*/
		ipc_msg.msg_addr = FLAG_WLAN_IF_NOT_RUNNING;
		goto send_response;
	}

	skb = rltk_wlan_alloc_skb(len);
	if (skb == NULL) {
		//DBG_8195A("Alloc tx buffer Err, alloc_sz %d!!\n\r", len);
	}
#ifdef CONFIG_ENABLE_CACHE
	else {
		DCache_CleanInvalidate(((u32)skb->head - sizeof(struct list_head)), sizeof(struct skb_data));
		DCache_CleanInvalidate(((u32)skb - sizeof(struct list_head)), sizeof(struct skb_buf));
	}
#endif /* CONFIG_ENABLE_CACHE */
	ipc_msg.msg_addr = (u32)skb;

send_response:
	/* response to request skb from host. */
	ipc_msg.event_num = IPC_WIFI_MSG_ALLOC_SKB;
	ipc_msg.msg_len = sizeof(struct sk_buff);
	inic_ipc_ipc_send_msg(&ipc_msg);

	return;
}

/**
 * @brief  receiving function to send the received data to host.
 * @param  idx_wlan[in]: which port of wifi to set.
 * @return none.
 */
void inic_ipc_dev_recv(int idx)
{
	struct sk_buff *skb = NULL;
	inic_ipc_ex_msg_t ipc_msg = {0};

	skb = rltk_wlan_get_recv_skb(idx);

	ipc_msg.event_num = IPC_WIFI_EVT_RECV_PKTS;
	ipc_msg.msg_addr = (u32)skb;
	ipc_msg.msg_len = idx;
#ifdef CONFIG_ENABLE_CACHE
	DCache_CleanInvalidate(((u32)skb->head - sizeof(struct list_head)), sizeof(struct skb_data));
	DCache_CleanInvalidate(((u32)skb - sizeof(struct list_head)), sizeof(struct skb_buf));
#endif /* CONFIG_ENABLE_CACHE */
	inic_ipc_ipc_send_msg(&ipc_msg);
}

/**
 * @brief  to notice dev rx done from host.
 * @param  none.
 * @return none.
 */
void inic_ipc_dev_rx_done(inic_ipc_ex_msg_t *p_ipc_msg)
{
	struct sk_buff *skb = (struct sk_buff *)p_ipc_msg->msg_addr;
	kfree_skb(skb);
}

/**
 * @brief  to notice dev rx pending from host.
 * @param  none.
 * @return none.
 */
void inic_ipc_dev_rx_pending(void)
{
	g_ipc_dev_priv.rx_pending_flag = 1;
	rtw_up_sema(&(g_ipc_dev_priv.rx_done_sema));
}
