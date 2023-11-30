/******************************************************************************
 *
 * Copyright(c) 2020 - 2021 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
/******************************************************************************
 * history *
******************************************************************************/
#define __INIC_IPC_MSG_QUEUE_C__

/* -------------------------------- Includes -------------------------------- */
/* external head files */

/* internal head files */
#include "inic_ipc_msg_queue.h"
#ifdef CONFIG_AS_INIC_NP
#include "wifi_intf_drv_to_upper.h"

extern struct wifi_user_conf wifi_user_config;
#endif
/* -------------------------------- Defines --------------------------------- */
#define IPC_MSG_QUEUE_DEPTH (20)
#define IPC_MSG_QUEUE_WARNING_DEPTH (4)

/* -------------------------------- Macros ---------------------------------- */

/* ------------------------------- Data Types ------------------------------- */
/* node to store the message to the queue. */
struct ipc_msg_node {
	_list list;
	inic_ipc_ex_msg_t ipc_msg; /* to store ipc message */
	u8 is_used; /* sign whether to be used */
};

/* message queue priv */
struct ipc_msg_q_priv {
	_queue msg_queue; /* msg queue */
	_sema msg_q_sema;
	_sema msg_send_sema;
	void (*task_hdl)(inic_ipc_ex_msg_t *); /* the haddle function of task */
	u8 b_queue_working; /* flag to notice the queue is working */
	struct ipc_msg_node ipc_msg_pool[IPC_MSG_QUEUE_DEPTH]; /* static pool for queue node */
	u8 queue_free; /* the free size of queue */
	u8 rsvd[40]; /* keep total size 64B alignment */
};

/* -------------------------- Function declaration -------------------------- */

/* ---------------------------- Global Variables ---------------------------- */

/* --------------------------- Private Variables ---------------------------- */

static struct ipc_msg_q_priv g_ipc_msg_q_priv __attribute__((aligned(64)));
#ifdef IPC_DIR_MSG_TX
inic_ipc_ex_msg_t g_inic_ipc_ex_msg __attribute__((aligned(64)));
#else
static inic_ipc_ex_msg_t g_inic_ipc_ex_msg = {0};
#endif
#if defined(configNUM_CORES) && (configNUM_CORES > 1)
static spinlock_t ipc_msg_queue_lock = {0};
#endif
#ifdef CONFIG_PLATFORM_TIZENRT_OS
struct task_struct ipc_msgQ_wlan_task;
#endif
/* ---------------------------- Private Functions --------------------------- */
/**
 * @brief  put the ipc message to queue.
 * @param  p_node[in]: the pointer for the ipc message node that need to be
 * 	pushed into the queue.
 * @param  p_queue[in]: the queue used to store the p_node.
 * @return status, always _SUCCESS.
 */
static sint enqueue_ipc_msg_node(struct ipc_msg_node *p_node, _queue *p_queue)
{
#if defined(configNUM_CORES) && (configNUM_CORES > 1)
	/* this function is called in ISR, no need to mask interrupt since gic already do it*/
	spin_lock(&ipc_msg_queue_lock);
#else
	_irqL irqL;
	rtw_enter_critical(&(p_queue->lock), &irqL);
#endif
	/* put the ipc message to the tail of the queue */
	rtw_list_insert_tail(&(p_node->list), get_list_head(p_queue));
#if defined(configNUM_CORES) && (configNUM_CORES > 1)
	spin_unlock(&ipc_msg_queue_lock);
#else
	rtw_exit_critical(&(p_queue->lock), &irqL);
#endif
	return _SUCCESS;
}

/**
 * @brief  get the ipc message from queue.
 * @param  p_ipc_msg[in]: the queue used to store the p_node.
 * @return the ipc_msg_node got from message queue.
 */
static struct ipc_msg_node *dequeue_ipc_msg_node(_queue *p_queue)
{
	struct ipc_msg_node *p_node;
	_list *plist, *phead;

#if defined(configNUM_CORES) && (configNUM_CORES > 1)
	u32 isr_status = portDISABLE_INTERRUPTS();
	spin_lock(&ipc_msg_queue_lock);
#else
	_irqL irqL;
	rtw_enter_critical(&(p_queue->lock), &irqL);
#endif

	if (rtw_queue_empty(p_queue) == _TRUE) {
		p_node = NULL;
	} else {
		phead = get_list_head(p_queue);
		plist = get_next(phead);
		p_node = LIST_CONTAINOR(plist, struct ipc_msg_node, list);
		rtw_list_delete(&(p_node->list));
	}

#if defined(configNUM_CORES) && (configNUM_CORES > 1)
	spin_unlock(&ipc_msg_queue_lock);
	portRESTORE_INTERRUPTS(isr_status);
#else
	rtw_exit_critical(&(p_queue->lock), &irqL);
#endif

	return p_node;
}

/**
 * @brief  task to operation the queue when the queue is not empty.
 * @param  none
 * @return none
 */
static void inic_ipc_msg_q_task(void)
{
	struct ipc_msg_node *p_node = NULL;
	_queue *p_queue = NULL;

	p_queue = &g_ipc_msg_q_priv.msg_queue;
	do {
		//xSemaphoreTake(g_ipc_msg_q_priv.msg_q_sema, 0xFFFFFFFF);
		rtw_down_sema(&g_ipc_msg_q_priv.msg_q_sema);
		/* get the data from tx queue. */
		while (1) {
			p_node = dequeue_ipc_msg_node(p_queue);
			if (p_node == NULL) {
				break;
			}
			/* haddle the message */
			if (g_ipc_msg_q_priv.task_hdl) {
				g_ipc_msg_q_priv.task_hdl(&(p_node->ipc_msg));
			}
			/* release the memory for this ipc message. */
#if defined(configNUM_CORES) && (configNUM_CORES > 1)
			u32 isr_status = portDISABLE_INTERRUPTS();
			spin_lock(&ipc_msg_queue_lock);
#else
			rtw_enter_critical(NULL, NULL);
#endif
			p_node->is_used = 0;
			g_ipc_msg_q_priv.queue_free++;
#if defined(configNUM_CORES) && (configNUM_CORES > 1)
			spin_unlock(&ipc_msg_queue_lock);
			portRESTORE_INTERRUPTS(isr_status);
#else
			rtw_exit_critical(NULL, NULL);
#endif
		}
	} while (g_ipc_msg_q_priv.b_queue_working);
	rtw_delete_task(&ipc_msgQ_wlan_task);
}

/* ---------------------------- Public Functions ---------------------------- */
/**
 * @brief  to initialize the message queue.
 * @param  task_func[in]: the pointer to the task function to operate this
 * 	queue.
 * @return none
 */
void inic_ipc_msg_q_init(void (*task_hdl)(inic_ipc_ex_msg_t *))
{
	int i = 0;

	rtw_memset(&g_ipc_msg_q_priv, 0, sizeof(struct ipc_msg_q_priv));
	rtw_memset(&g_inic_ipc_ex_msg, 0, sizeof(inic_ipc_ex_msg_t));

	/* initialize queue. */
	rtw_init_queue(&(g_ipc_msg_q_priv.msg_queue));

	/* assign the haddle function for the task */
	g_ipc_msg_q_priv.task_hdl = task_hdl;

	/* initialize the sema to wakeup the message queue task */
	rtw_init_sema(&g_ipc_msg_q_priv.msg_q_sema, 0);
	rtw_init_sema(&g_ipc_msg_q_priv.msg_send_sema, 0);
	rtw_up_sema(&g_ipc_msg_q_priv.msg_send_sema);

	for (i = 0; i < IPC_MSG_QUEUE_DEPTH; i++) {
		g_ipc_msg_q_priv.ipc_msg_pool[i].is_used = 0;
	}
	g_ipc_msg_q_priv.queue_free = IPC_MSG_QUEUE_DEPTH;

	/* Initialize the queue task */
	if (rtw_create_task(&ipc_msgQ_wlan_task, (const char *const)"inic_msg_q_task", 1024, (0 + CONFIG_INIC_IPC_MSG_Q_PRI), (void*)inic_ipc_msg_q_task, NULL) != 1) {
		DBG_8195A("Create inic_ipc_msg_q_task Err!!\n");
	}

	/* sign the queue is working */
	g_ipc_msg_q_priv.b_queue_working = 1;
}

/**
 * @brief  put the ipc message to queue.
 * @param  p_node[in]: the pointer for the ipc message that need to be
 * 	pushed into the queue.
 * @return status, always _SUCCESS.
 */
sint inic_ipc_msg_enqueue(inic_ipc_ex_msg_t *p_ipc_msg)
{
	struct ipc_msg_node *p_node = NULL;
	_queue *p_queue = &(g_ipc_msg_q_priv.msg_queue);
	sint ret = FAIL;
	int i = 0;

#if defined(configNUM_CORES) && (configNUM_CORES > 1)
	/* this function is called in ISR, no need to mask interrupt since gic already do it*/
	spin_lock(&ipc_msg_queue_lock);
#else
	_irqL irqL;
	rtw_enter_critical(&(p_queue->lock), &irqL);
#endif
	/* allocate memory for message node */
	for (i = 0; i < IPC_MSG_QUEUE_DEPTH; i++) {
		if (g_ipc_msg_q_priv.ipc_msg_pool[i].is_used == 0) {
			p_node = &(g_ipc_msg_q_priv.ipc_msg_pool[i]);
			/* a node is used, the free node will decrease */
			p_node->is_used = 1;
			g_ipc_msg_q_priv.queue_free--;
			break;
		}
	}
#if defined(configNUM_CORES) && (configNUM_CORES > 1)
	spin_unlock(&ipc_msg_queue_lock);
#else
	rtw_exit_critical(&(p_queue->lock), &irqL);
#endif

	if (p_node == NULL) {
		DBG_8195A("NO buffer for new nodes, waiting!\n\r");
		goto func_out;
	}

	/* To store the ipc message to queue's node. */
	p_node->ipc_msg.event_num = p_ipc_msg->event_num;
	p_node->ipc_msg.msg_addr = p_ipc_msg->msg_addr;
	p_node->ipc_msg.msg_queue_status = p_ipc_msg->msg_queue_status;
	p_node->ipc_msg.wlan_idx = p_ipc_msg->wlan_idx;

	/* put the ipc message to the queue */
	ret = enqueue_ipc_msg_node(p_node, p_queue);

	/* the free number of nodes is smaller than the warning depth. */
	if (g_ipc_msg_q_priv.queue_free <= IPC_MSG_QUEUE_WARNING_DEPTH) {
		/* ask peer to wait */
		ret = FAIL;
	}

func_out:
	/* wakeup task */
	rtw_up_sema_from_isr(&g_ipc_msg_q_priv.msg_q_sema);

	return ret;
}

/**
 * @brief  to deinitialize the message queue.
 * @param  none.
 * @return none
 */
void inic_ipc_msg_q_deinit(void)
{
	/* sign the queue is stop */
	g_ipc_msg_q_priv.b_queue_working = 0;

	/* assign the haddle function to NULL */
	g_ipc_msg_q_priv.task_hdl = NULL;

	/* free sema to wakeup the message queue task */
	rtw_free_sema(&g_ipc_msg_q_priv.msg_q_sema);
	/* de initialize queue, todo */
}

/**
 * @brief  to get the status of queue, working or stop.
 * @param  none.
 * @return the status of queue, 1 means working, 0 means stop.
 */
u8 inic_ipc_msg_get_queue_status(void)
{
	return g_ipc_msg_q_priv.b_queue_working;
}

/**
 * @brief  to send the ipc message. It will wait when the last message is not
 * 	read.
 * @param  p_ipc_msg[inout]: the message to send.
 * @return none.
 */
void inic_ipc_ipc_send_msg(inic_ipc_ex_msg_t *p_ipc_msg)
{
	IPC_MSG_STRUCT g_inic_ipc_msg = {0};

	u32 cnt = 100000;

	/* wifi_hal_interrupt_handle(little_thread) will call rtw_enter_critical(close cpu scheduling), before call this func.
	if another thread(single_thread) hasn't up_sema, little_thread and single_thread will deadlock */
	/* LINUX_TODO: better method? */
#ifdef CONFIG_AS_INIC_NP
	if (wifi_user_config.cfg80211) {
		save_and_cli();
	} else {
		rtw_down_sema(&g_ipc_msg_q_priv.msg_send_sema);
	}
#else
	rtw_down_sema(&g_ipc_msg_q_priv.msg_send_sema);
#endif

	/* Wait for another port ack acknowledgement last message sending */
	while (g_inic_ipc_ex_msg.event_num != IPC_WIFI_MSG_READ_DONE) {
		DelayUs(2);
		DCache_Invalidate((u32)&g_inic_ipc_ex_msg, sizeof(inic_ipc_ex_msg_t));
		cnt--;
		if (cnt == 0) {
			DBG_8195A("inic ipc wait timeout\n");
			break;
		}
	}
	/* Get the warning of queue's depth not enough after recv MSG_READ_DONE,
	delay send the next message */
	if (g_inic_ipc_ex_msg.msg_queue_status == IPC_WIFI_MSG_MEMORY_NOT_ENOUGH) {
		rtw_mdelay_os(1);
	}

	/* Send the new message after last one acknowledgement */
	g_inic_ipc_ex_msg.event_num = p_ipc_msg->event_num;
	g_inic_ipc_ex_msg.msg_addr = p_ipc_msg->msg_addr;
	g_inic_ipc_ex_msg.msg_queue_status = p_ipc_msg->msg_queue_status;
	g_inic_ipc_ex_msg.wlan_idx = p_ipc_msg->wlan_idx;
	DCache_Clean((u32)&g_inic_ipc_ex_msg, sizeof(inic_ipc_ex_msg_t));

#ifdef IPC_DIR_MSG_TX
	g_inic_ipc_msg.msg_type = IPC_USER_POINT;
	g_inic_ipc_msg.msg = (u32)&g_inic_ipc_ex_msg;
	g_inic_ipc_msg.msg_len = sizeof(inic_ipc_ex_msg_t);
	ipc_send_message(IPC_DIR_MSG_TX, IPC_INT_CHAN_WIFI_TRX_TRAN, \
					 (PIPC_MSG_STRUCT)&g_inic_ipc_msg);
#else
	ipc_send_message(IPC_INT_CHAN_WIFI_TRX_TRAN, (PIPC_MSG_STRUCT)&g_inic_ipc_ex_msg);
#endif /* IPC_DIR_MSG_TX */

#ifdef CONFIG_AS_INIC_NP
	if (wifi_user_config.cfg80211) {
		restore_flags();
	} else {
		rtw_up_sema(&g_ipc_msg_q_priv.msg_send_sema);
	}
#else
	rtw_up_sema(&g_ipc_msg_q_priv.msg_send_sema);
#endif
}
