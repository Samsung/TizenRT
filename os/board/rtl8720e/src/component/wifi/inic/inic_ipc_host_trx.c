/******************************************************************************
 *
 * Copyright(c) 2020 - 2021 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
/******************************************************************************
 * history *
 * 2020.11.20 | Andrew Su | Modification after first review.
 * 1, Change p_inic_ipc_dev_send to local variable from global, new name is
 * p_ipc_dev_msg.
 * 2, Delete irq_th_lock and xmit_skb in host_priv.
 * 3, Modify the inic_enqueue_recvbuf.
******************************************************************************/
#define __INIC_IPC_HOST_TRX_C__

/* -------------------------------- Includes -------------------------------- */
/* external head files */

/* internal head files */
#include "inic_ipc_host_trx.h"
#include "inic_ipc_msg_queue.h"
#include <tinyara/netmgr/netdev_mgr.h>
#include <netdev_mgr_internal.h>
#define CONFIG_ENABLE_CACHE

#define pdFALSE 0
#define pdTRUE 1

/* -------------------------------- Defines --------------------------------- */
#ifndef GET_NETIF_FROM_NETDEV
#define GET_NETIF_FROM_NETDEV(dev) (struct netif *)(((struct netdev_ops *)(dev)->ops)->nic)
#endif
/* -------------------------------- Macros ---------------------------------- */

/* ------------------------------- Data Types ------------------------------- */
/* recv buffer to store the data from IPC to queue. */
struct host_recv_buf {
	_list list;
	int idx_wlan; /* index for wlan */
	struct pbuf *p_buf; /* rx data for ethernet buffer*/
};

/* recv structure */
struct host_priv {
	_sema recv_sema; /* sema to wait allloc skb from device */
	_sema xmit_sema; /* sema to wait allloc skb from device */
	_sema alloc_skb_sema; /* sema to wait allloc skb from device */
	_sema host_send_sema; /* sema to protect inic ipc host send */
	_queue recv_queue; /* recv queue */
	u32 rx_bytes; /* recv bytes */
	u32 rx_pkts; /* recv number of packets */
	u32 tx_bytes; /* xmit bytes */
	u32 tx_pkts; /* xmit number of packets */
	u8 rx_pending_flag; /* host rx pending flag */
	u8 rsvd[16]; /* keep total size 64B alignment */
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
	u8 rsvd[16];
};

/* -------------------------- Function declaration -------------------------- */

/* ---------------------------- Global Variables ---------------------------- */
extern struct netif xnetif[NET_IF_NUM];

/* --------------------------- Private Variables ---------------------------- */
struct host_priv g_inic_host_priv __attribute__((aligned(64)));
struct sk_buff *allocated_tx_skb = NULL;
struct task_struct inic_ipc_host_rx_task;

/* ---------------------------- Private Functions --------------------------- */
/**
 * @brief  to enqueue the precvbuf into the queue.
 * @param  precvbuf[inout]: the recv buffer enqueued into the queue.
 * @param  queue[inout]: the recv queue.
 * @return if is OK, return _SUCCESS, failed return _FAIL.
 */
static sint inic_enqueue_recvbuf(struct host_recv_buf *precvbuf, _queue *queue)
{
	_irqL irqL;

	rtw_enter_critical(&queue->lock, &irqL);
	rtw_list_insert_tail(&precvbuf->list, get_list_head(queue));
	rtw_exit_critical(&queue->lock, &irqL);

	return _SUCCESS;
}

/**
 * @brief  to get the recv buffer from the recv queue.
 * @param  queue[inout]: the recv queue.
 * @return return recv buffer. if is NULL, the queue is empty.
 */
static struct host_recv_buf *inic_dequeue_recvbuf(_queue *queue)
{
	_irqL irqL;
	struct host_recv_buf *precvbuf;
	_list *plist, *phead;

	rtw_enter_critical(&queue->lock, &irqL);

	if (rtw_queue_empty(queue) == _TRUE) {
		precvbuf = NULL;
	} else {
		phead = get_list_head(queue);
		plist = get_next(phead);
		precvbuf = LIST_CONTAINOR(plist, struct host_recv_buf, list);
		rtw_list_delete(&precvbuf->list);
	}

	rtw_exit_critical(&queue->lock, &irqL);

	return precvbuf;
}

/**
 * @brief  rx task to handle the rx data, get the data from the rx queue and send
 *	to upper layer.
 * @param  none.
 * @return none.
 */
static void inic_ipc_host_rx_tasklet(void)
{
	struct host_recv_buf *precvbuf = NULL;
	_queue *recv_queue = NULL;
	struct pbuf *p_buf = NULL;
	int index = 0;
	err_enum_t error = ERR_OK;
	inic_ipc_ex_msg_t ipc_msg = {0};

	recv_queue = &g_inic_host_priv.recv_queue;
	do {
		rtw_down_sema(&g_inic_host_priv.recv_sema);

		while ((precvbuf = inic_dequeue_recvbuf(recv_queue))) {
			p_buf = precvbuf->p_buf;
			index = precvbuf->idx_wlan;

			g_inic_host_priv.rx_bytes += p_buf->len;
			g_inic_host_priv.rx_pkts++;

			struct netdev *dev_tmp = NULL;
			dev_tmp = rtk_get_netdev(index);
			struct netif *netif = GET_NETIF_FROM_NETDEV(dev_tmp);
			if (netif->input(p_buf, netif) != ERR_OK) {
				LWIP_DEBUGF(NETIF_DEBUG, ("input processing error\n"));
				LINK_STATS_INC(link.err);
				pbuf_free(p_buf);
			} else {
				LINK_STATS_INC(link.recv);
			}

			/* release the memory for this packet. */
			rtw_mfree((u8 *)precvbuf, sizeof(struct host_recv_buf));
		}

		if (g_inic_host_priv.rx_pending_flag) {
			g_inic_host_priv.rx_pending_flag = 0;
			ipc_msg.event_num = IPC_WIFI_MSG_RECV_DONE;
			inic_ipc_ipc_send_msg(&ipc_msg);
		}
	} while (1);
	rtw_delete_task(&inic_ipc_host_rx_task);
}

/**
 * @brief  to apply for the skb buffer to send the data from device.
 * @param  total_len[in]: the length to require for sending from device.
 * @return address of skb from device.
 */
static struct sk_buff *inic_ipc_host_alloc_skb(unsigned int total_len, int idx)
{
	struct sk_buff *skb = NULL;
	inic_ipc_ex_msg_t ipc_msg = {0};

	ipc_msg.event_num = IPC_WIFI_MSG_ALLOC_SKB;
	ipc_msg.msg_addr = idx;
	ipc_msg.msg_len = total_len;
	inic_ipc_ipc_send_msg(&ipc_msg);
	rtw_down_sema(&(g_inic_host_priv.alloc_skb_sema));

	skb = allocated_tx_skb;
	if (skb == NULL || (((u32)skb) == FLAG_WLAN_IF_NOT_RUNNING)) {
		allocated_tx_skb = NULL;
		return skb;
	}

#ifdef CONFIG_ENABLE_CACHE
	DCache_Invalidate(((u32)skb - sizeof(struct list_head)), sizeof(struct skb_buf));
	DCache_Invalidate(((u32)skb->head - sizeof(struct list_head)), sizeof(struct skb_data));
#endif /* CONFIG_ENABLE_CACHE */

	allocated_tx_skb = NULL;

	return skb;
}

/**
 * @brief  to send skb to device for port idx.
 * @param  idx[in]: which port of wifi to tx.
 * @param  skb[inout]: skb to send.
 * @return -1 failed, 0 seccessful.
 */
static int inic_ipc_host_send_skb(int idx, struct sk_buff *skb)
{
	inic_ipc_ex_msg_t ipc_msg = {0};

	if (idx == -1) {
		DBG_8195A("%s=>wlan index is wrong!\n\r", __func__);
		return -1;
	}

	ipc_msg.event_num = IPC_WIFI_CMD_XIMT_PKTS;
	ipc_msg.msg_addr = (u32)skb;
	ipc_msg.msg_len = idx;
	inic_ipc_ipc_send_msg(&ipc_msg);

	/* wait for device tx done */
	rtw_down_sema(&g_inic_host_priv.xmit_sema);
	return 0;
}

/* ---------------------------- Public Functions ---------------------------- */
/**
 * @brief  to initialize the parameters of recv.
 * @param  none
 * @return none.
 */
void inic_ipc_host_init_priv(void)
{
	rtw_memset(&g_inic_host_priv, 0, sizeof(struct host_priv));

	/* initialize semaphores. */
	rtw_init_sema(&(g_inic_host_priv.recv_sema), 0);
	rtw_init_sema(&(g_inic_host_priv.alloc_skb_sema), 0);
	rtw_init_sema(&(g_inic_host_priv.host_send_sema), 0);
	rtw_up_sema(&(g_inic_host_priv.host_send_sema));
	rtw_init_sema(&(g_inic_host_priv.xmit_sema), 0);

	/* initialize the Rx queue. */
	rtw_init_queue(&(g_inic_host_priv.recv_queue));

	g_inic_host_priv.rx_bytes = 0;
	g_inic_host_priv.rx_pkts = 0;

	g_inic_host_priv.tx_bytes = 0;
	g_inic_host_priv.tx_pkts = 0;
	g_inic_host_priv.rx_pending_flag = 0;

	/* Initialize the RX task */

// if (pdTRUE != xTaskCreate((TaskFunction_t)inic_ipc_host_rx_tasklet, \
// 						  (const char *const)"inic_host_rx_tasklet", 1024, NULL, \
// 						  4, NULL)) {
// 	DBG_8195A("Create TaskCommon_8721D Err!!\n");
// }
	if (rtw_create_task(&inic_ipc_host_rx_task, (const char *const)"inic_host_rx_tasklet", 1024, 4, (void *)inic_ipc_host_rx_tasklet, NULL) != _SUCCESS){
			DBG_8195A("Create inic_ipc_host_rx_task Err!!\n");
		}
}

/**
 * @brief  to put the Rx data from rx buffer into Rx queue.
 * @param  idx_wlan[in]: which port of wifi to rx.
 * @param  skb[inout]: data from the ipc bus, its structure is sk_buff.
 * @return none.
 */
void inic_ipc_host_rx_handler(int idx_wlan, struct sk_buff *skb)
{
	_queue *recv_queue = NULL;
	struct host_recv_buf *precvbuf = NULL;
	struct pbuf *p_buf = NULL, *temp_buf = NULL;
	inic_ipc_ex_msg_t ipc_msg = {0};

	/* get the rx queue. */
	recv_queue = &(g_inic_host_priv.recv_queue);

#ifdef CONFIG_ENABLE_CACHE
	DCache_Invalidate(((u32)skb - sizeof(struct list_head)), sizeof(struct skb_buf));
	DCache_Invalidate(((u32)skb->head - sizeof(struct list_head)), sizeof(struct skb_data));
#endif /* CONFIG_ENABLE_CACHE */

	/* allocate pbuf to store ethernet data from IPC. */
	p_buf = pbuf_alloc(PBUF_RAW, skb->len, PBUF_POOL);
	if (p_buf == NULL) {
		//DBG_8195A("Alloc pbuf rx buf Err, alloc_sz %d!!\n\r", skb->len);
		g_inic_host_priv.rx_pending_flag = 1;
		ipc_msg.event_num = IPC_WIFI_MSG_RECV_PENDING;
		inic_ipc_ipc_send_msg(&ipc_msg);
		/* wakeup recv task */
		rtw_up_sema(&g_inic_host_priv.recv_sema);
		return;
	}

	/* cpoy data from skb(ipc data) to pbuf(ether net data) */
	temp_buf = p_buf;
	while (temp_buf) {
		/* If tot_len > PBUF_POOL_BUFSIZE_ALIGNED, the skb will be
		 * divided into several pbufs. Therefore, there is a while to
		 * use to assigne data to pbufs.
		 */
		rtw_memcpy(temp_buf->payload, skb->data, temp_buf->len);
		skb_pull(skb, temp_buf->len);
		temp_buf = temp_buf->next;
	}
#ifdef CONFIG_ENABLE_CACHE
	DCache_CleanInvalidate(((u32)skb->head - sizeof(struct list_head)), sizeof(struct skb_data));
	DCache_CleanInvalidate(((u32)skb - sizeof(struct list_head)), sizeof(struct skb_buf));
#endif /* CONFIG_ENABLE_CACHE */

	/* allocate host_recv_buf and associate to the p_buf */
	precvbuf = (struct host_recv_buf *)rtw_zmalloc(sizeof(struct host_recv_buf));
	precvbuf->p_buf = p_buf;
	precvbuf->idx_wlan = idx_wlan;

	/* enqueue recv buffer  */
	inic_enqueue_recvbuf(precvbuf, recv_queue);
	ipc_msg.event_num = IPC_WIFI_MSG_RECV_DONE;
	inic_ipc_ipc_send_msg(&ipc_msg);
	/* wakeup recv task */
	rtw_up_sema(&g_inic_host_priv.recv_sema);
}

/**
 * @brief  to assigne the skb from device and wake up the alloc_skb_sema.
 * @param  p_ipc_msg[inout]: ipc message from ipc device response. The
 *	skb address is stored in msg_addr.
 * @return none.
 */
void inic_ipc_host_tx_alloc_skb_rsp(inic_ipc_ex_msg_t *p_ipc_msg)
{
	allocated_tx_skb = (struct sk_buff *)p_ipc_msg->msg_addr;
	rtw_up_sema(&(g_inic_host_priv.alloc_skb_sema));
}

/**
 * @brief  to notice the tx is done in device port. It will wakeup
 * 	inic_ipc_host_send_skb
 * @param  none.
 * @return none.
 */
void inic_ipc_host_tx_done(void)
{
	rtw_up_sema(&g_inic_host_priv.xmit_sema);
}

/**
 * @brief  to put the Rx data from rx buffer into Rx queue.
 * @param  idx[in]: which port of wifi to tx.
 * @param  sg_list[in]: pbuf list to send.
 * @param  sg_len[in]: the length of sg_list.
 * @param  total_len[in]: the length of data to send.
 * @return result.
 */
int inic_ipc_host_send(int idx, struct eth_drv_sg *sg_list, int sg_len,
					   int total_len)
{
	struct sk_buff *skb = NULL;
	struct eth_drv_sg *psg_list;
	int ret = 0, i = 0;

	rtw_down_sema(&g_inic_host_priv.host_send_sema);
	/* allocate the skb buffer to send from device */
	skb = inic_ipc_host_alloc_skb(total_len, idx);

	if (skb == NULL) {
		//DBG_ERR("rltk_wlan_alloc_skb() for data len=%d failed!", total_len);
		rtw_up_sema(&g_inic_host_priv.host_send_sema);
		return ERR_BUF;
	}

	if (((u32)skb) == FLAG_WLAN_IF_NOT_RUNNING) {
		/* since corresponding wlan interface is not running ,return ERR_IF*/
		rtw_up_sema(&g_inic_host_priv.host_send_sema);
		return ERR_IF;
	}

	psg_list = sg_list;
	for (i = 0; i < sg_len; i++) {
		psg_list = &sg_list[i];
		rtw_memcpy(skb->tail, (void *)(psg_list->buf), psg_list->len);
		skb_put(skb, psg_list->len);
	}

#ifdef CONFIG_ENABLE_CACHE
	DCache_CleanInvalidate(((u32)skb->head - sizeof(struct list_head)), sizeof(struct skb_data));
	DCache_CleanInvalidate(((u32)skb - sizeof(struct list_head)), sizeof(struct skb_buf));
#endif /* CONFIG_ENABLE_CACHE */

	inic_ipc_host_send_skb(idx, skb);
	rtw_up_sema(&g_inic_host_priv.host_send_sema);

	return ret;
}
