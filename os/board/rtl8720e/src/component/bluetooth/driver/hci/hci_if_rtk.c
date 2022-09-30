/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include "osif.h"
#include "hci_if_rtk.h"
#include "hci_process.h"
#include "hci_transport.h"
#include "hci_uart.h"
#include "hci_platform.h"
#include "hci_dbg.h"

#define H4_HDR_LEN          (1)
#define RESERVED_LEN        (H4_HDR_LEN + 0)

#define SECURE_CONTEXT_SIZE (128)

#ifdef CONFIG_AYNSC_HCI_INTF
#define HCI_IF_TASK_SIZE    (2*1024)
#define HCI_IF_TASK_PRIO    (5)
#define HCI_IF_MSG_OPEN     0x01
#define HCI_IF_MSG_READY    0x02
#define HCI_IF_MSG_FAIL     0x03
#define HCI_IF_MSG_CLOSE    0x04
#define HCI_IF_MSG_WRITE    0x05

struct tx_item_t {
	struct tx_item_t *next;
	uint8_t *buf;
	uint32_t len;
};

struct tx_queue_t {
	struct tx_item_t *head;
	struct tx_item_t *tail;
};
#endif

static struct {
	uint8_t status;
	HCI_IF_CALLBACK cb;
#ifdef CONFIG_AYNSC_HCI_INTF
	struct tx_queue_t tx_queue;
	void *tx_lock;
	void *task_hdl;
	void *msg_q;
#endif
} hci_if_rtk = {
	.status = 0,
	.cb = 0,
#ifdef CONFIG_AYNSC_HCI_INTF
	.tx_queue = {0, 0},
	.tx_lock = 0,
	.task_hdl = 0,
	.msg_q = 0,
#endif
};
void *hci_deinit_sem = NULL;

static uint8_t *rtk_stack_get_buf(uint8_t type, uint16_t len, uint32_t timeout)
{
	(void)timeout;
	uint8_t *buf = NULL;
	uint16_t actual_len = RESERVED_LEN + len;

	switch (type) {
	case H4_ACL:
#if HCI_ISO_DATA_PACKET
	case H4_ISO:
#endif
		actual_len += HCI_H4_RX_ACL_PKT_BUF_OFFSET;
		buf = (uint8_t *)osif_mem_aligned_alloc(RAM_TYPE_DATA_ON, actual_len, 4);
		memset(buf, 0, actual_len);
		return (buf + RESERVED_LEN + HCI_H4_RX_ACL_PKT_BUF_OFFSET);
		break;
	case H4_EVT:
	case H4_SCO:
		buf = (uint8_t *)osif_mem_aligned_alloc(RAM_TYPE_DATA_ON, actual_len, 4);
		memset(buf, 0, actual_len);
		return (buf + RESERVED_LEN);
		break;
	default:
		break;
	}

	return NULL;
}

static uint8_t rtk_stack_recv(uint8_t type, uint8_t *buf, uint16_t len)
{
	uint8_t *hci_buf = buf - RESERVED_LEN;
	uint32_t actual_len = len + H4_HDR_LEN;

	if (H4_ACL == type
#if HCI_ISO_DATA_PACKET
			|| H4_ISO == type
#endif
	)
	{
		hci_buf -= HCI_H4_RX_ACL_PKT_BUF_OFFSET;
		actual_len += HCI_H4_RX_ACL_PKT_BUF_OFFSET;
	}

	hci_buf[RESERVED_LEN - H4_HDR_LEN] = type;

	hci_if_rtk.cb(HCI_IF_EVT_DATA_IND, true, &hci_buf[RESERVED_LEN - H4_HDR_LEN], actual_len);

	return HCI_SUCCESS;
}

#ifdef CONFIG_AYNSC_HCI_INTF
static void hci_if_task(void *context)
{
	(void)context;
	uint8_t task_run = 1, msg;
	struct tx_queue_t *tx_queue = &hci_if_rtk.tx_queue;

#if defined(CONFIG_BUILD_NONSECURE)
	osif_create_secure_context(SECURE_CONTEXT_SIZE);
#endif

	while (task_run) {
		if (osif_msg_recv(hci_if_rtk.msg_q, &msg, 0xFFFFFFFF) == true) {
			switch (msg) {
			case HCI_IF_MSG_OPEN: {
				/* BT Board Init */
				if (HCI_FAIL == hci_platform_init()) {
					task_run = 0;
					break;
				}
				/* HCI Transport */
				if (HCI_FAIL == hci_transport_open()) {
					task_run = 0;
					break;
				}
				/* HCI Transport Bridge to StandAlone */
				hci_transport_set_get_buf(hci_sa_recv_get_buf);
				hci_transport_set_recv(hci_sa_recv);

				/* HCI UART Bridge to Transport */
				hci_uart_set_rx_ind(hci_transport_recv_ind);

				if (HCI_FAIL == hci_process()) {
					task_run = 0;
					break;
				}
				/* HCI Transport Bridge to RTK Stack
				 * (Stop and Start rx_ind for this Moment)
				 */
				hci_uart_set_rx_ind(NULL);
				hci_transport_set_get_buf(rtk_stack_get_buf);
				hci_transport_set_recv(rtk_stack_recv);
				hci_uart_set_rx_ind(hci_transport_recv_ind);

				hci_if_rtk.status = 1;
				if (wifi_driver_is_mp()) {
					hci_if_rtk.cb(HCI_IF_EVT_OPENED, false, NULL, 0);	//If in MP mode, do not start upper stack
					HCI_PRINT("Not start upper stack for MP test\r\n");
				} else {
					hci_if_rtk.cb(HCI_IF_EVT_OPENED, true, NULL, 0);	//If in normal mode, start upper stack
					HCI_PRINT("Start upper stack\r\n");
				}
				break;
			}
			case HCI_IF_MSG_CLOSE: {
				task_run = 0;
				break;
			}
			case HCI_IF_MSG_WRITE: {
				while (tx_queue->head) {
					osif_mutex_take(hci_if_rtk.tx_lock, 0xffffffff);
					struct tx_item_t *tx_item = tx_queue->head;
					tx_queue->head = tx_queue->head->next;
					if (tx_item == tx_queue->tail) {
						tx_queue->tail = 0;
					}
					tx_item->next = 0;
					osif_mutex_give(hci_if_rtk.tx_lock);

					uint16_t offset = 0;
					if (H4_ACL == tx_item->buf[0]
#if HCI_ISO_DATA_PACKET
						|| H4_ISO == tx_item->buf[0]
#endif
						) {
						offset = HCI_H4_TX_ACL_PKT_BUF_OFFSET + H4_HDR_LEN;
					}
					else {
						offset = H4_HDR_LEN;
					}

					hci_transport_send(tx_item->buf[0], tx_item->buf + offset, tx_item->len - offset, 1);
					hci_if_rtk.cb(HCI_IF_EVT_DATA_XMIT, true, tx_item->buf, tx_item->len);
					osif_mem_free(tx_item);
				}
			}
			default:
				break;
			}
		} else {
			HCI_ERR("hci_if_rtk.msg_q recv fail!");
		}
	}

	osif_sem_give(hci_deinit_sem);

	/* Platform Deinit First */
	hci_platform_deinit();

	/* HCI Transport Close */
	hci_transport_close();

	hci_if_rtk.cb(HCI_IF_EVT_CLOSED, true, NULL, 0);
	hci_if_rtk.status = 0;

	osif_task_delete(NULL);
}
#endif

bool hci_if_open(HCI_IF_CALLBACK callback)
{
	hci_if_rtk.cb = callback;

	if (hci_if_rtk.status) {
		HCI_DBG("Hci Driver Already Open!");
		hci_if_rtk.cb(HCI_IF_EVT_OPENED, true, NULL, 0);	//If in normal mode, start upper stack
		HCI_PRINT("Start upper stack\r\n");

		return true;
	}

#ifdef CONFIG_HCI_FLOWCONTROL
	rtk_hci_ps_uart_rx_queue_fill_register(hci_rx_queue_fill);
#endif

#ifdef CONFIG_AYNSC_HCI_INTF
	osif_mutex_create(&hci_if_rtk.tx_lock);
	osif_msg_queue_create(&hci_if_rtk.msg_q, 64, sizeof(uint8_t));
	osif_task_create(&hci_if_rtk.task_hdl, "hci_if_task", hci_if_task,
					 0, HCI_IF_TASK_SIZE, HCI_IF_TASK_PRIO);

	uint8_t msg = HCI_IF_MSG_OPEN;
	osif_msg_send(hci_if_rtk.msg_q, &msg, 0);
#else
	/* BT Board Init */
	if (HCI_FAIL == hci_platform_init()) {
		return false;
	}

	/* HCI Transport */
	if (HCI_FAIL == hci_transport_open()) {
		return false;
	}

	/* HCI Transport Bridge to StandAlone */
	hci_transport_set_get_buf(hci_sa_recv_get_buf);
	hci_transport_set_recv(hci_sa_recv);

	/* HCI UART Bridge to Transport */
	hci_uart_set_rx_ind(hci_transport_recv_ind);

	if (HCI_FAIL == hci_process()) {
		return false;
	}

	/* HCI Transport Bridge to RTK Stack
	 * (Stop and Start rx_ind for this Moment)
	 */
	hci_uart_set_rx_ind(NULL);
	hci_transport_set_get_buf(rtk_stack_get_buf);
	hci_transport_set_recv(rtk_stack_recv);
	hci_uart_set_rx_ind(hci_transport_recv_ind);

	hci_if_rtk.status = 1;
	hci_if_rtk.cb(HCI_IF_EVT_OPENED, true, NULL, 0);	//If in normal mode, start upper stack
	HCI_PRINT("Start upper stack\r\n");
#endif
	return true;
}

bool hci_if_close(void)
{
	if (!hci_if_rtk.status) {
		return true;
	}

#ifdef CONFIG_AYNSC_HCI_INTF
	osif_sem_create(&hci_deinit_sem, 0, 1);
	osif_mutex_delete(hci_if_rtk.tx_lock);
	hci_if_rtk.tx_lock = NULL;
	uint8_t msg = HCI_IF_MSG_CLOSE;
	osif_msg_send(hci_if_rtk.msg_q, &msg, 0);

	osif_sem_take(hci_deinit_sem, 0xFFFFFFFF);
	osif_sem_delete(hci_deinit_sem);
	hci_deinit_sem = NULL;

	osif_msg_queue_delete(hci_if_rtk.msg_q);
	hci_if_rtk.msg_q = NULL;
#else

	if (HCI_FAIL == hci_platform_deinit() || /* Platform Deinit First */
		HCI_FAIL == hci_transport_close()) { /* HCI Transport Close */
		return false;
	}

	hci_if_rtk.cb(HCI_IF_EVT_CLOSED, true, NULL, 0);
	hci_if_rtk.status = 0;
#endif
	return true;
}

void hci_if_del_task(void)
{
	hci_if_close();
}

void hci_if_deinit(void)
{
	while (hci_if_rtk.status)
		osif_delay(1);

	memset(&hci_if_rtk, 0, sizeof(hci_if_rtk));

	if (HCI_FAIL == hci_uart_free() ||       /* UART Free */
		HCI_FAIL == hci_transport_free()) {  /* HCI Transport Free */
		HCI_ERR("hci_if_deinit fail!");
	}
}

bool hci_if_write(uint8_t *buf, uint32_t len)
{
#ifdef CONFIG_AYNSC_HCI_INTF
	uint8_t msg = HCI_IF_MSG_WRITE;
	struct tx_queue_t *tx_queue = &hci_if_rtk.tx_queue;
	struct tx_item_t *tx_item = NULL;

	tx_item = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct tx_item_t));
	if (!tx_item) {
		hci_if_rtk.cb(HCI_IF_EVT_DATA_XMIT, false, buf, len);
		return false;
	}

	tx_item->buf   = buf;
	tx_item->len   = len;
	tx_item->next  = 0;

	osif_mutex_take(hci_if_rtk.tx_lock, 0xffffffff);
	if (0 == tx_queue->head) {
		tx_queue->head = tx_item;
	}
	if (tx_queue->tail) {
		tx_queue->tail->next = tx_item;
	}
	tx_queue->tail = tx_item;
	osif_mutex_give(hci_if_rtk.tx_lock);

	osif_msg_send(hci_if_rtk.msg_q, &msg, 0);
#else
	uint16_t offset = 0;

	if (H4_ACL == buf[0]
#if HCI_ISO_DATA_PACKET
		|| H4_ISO == buf[0]
#endif
	) {
		offset = HCI_H4_TX_ACL_PKT_BUF_OFFSET + H4_HDR_LEN;
	}
	else {
		offset = H4_HDR_LEN;
	}

	hci_transport_send(buf[0], buf + offset, len - offset, 1);

	hci_if_rtk.cb(HCI_IF_EVT_DATA_XMIT, true, buf, len);
#endif
	return true;
}

bool hci_if_confirm(uint8_t *buf)
{
	osif_mem_aligned_free(buf - (RESERVED_LEN - H4_HDR_LEN));
	return true;
}
