/*
 * Copyright (c) 2025 Realtek Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "osif.h"
#include "hci_if_rtk.h"
#include "hci_transport.h"
#include "hci_uart.h"
#include "hci_controller.h"
#include "hci_platform.h"
#include "bt_debug.h"
#include "dlist.h"

#define HCI_IF_HDR_LEN      (1)

#define HCI_IF_TASK_SIZE    (2*1024)
#define HCI_IF_TASK_PRIO    (5)

#define HCI_IF_TX_QUEUE_NUM (32)

#define HCI_IF_TASK_CLOSED 0
#define HCI_IF_TASK_RUNNING 1
#define HCI_IF_TASK_CLOSING 2

#define IS_ALIGNED(ptr, align) (((uintptr_t)(ptr)) % (align) == 0)

struct tx_packet_t {
	uint8_t *buf;
	uint32_t len;
};

static struct {
	HCI_IF_CALLBACK cb;
	uint8_t state;
	void *tx_queue;
	void *task_hdl;
} hci_if_rtk;

static void rtk_stack_recv(struct hci_rx_packet_t *pkt)
{
	uint8_t offset = HCI_IF_HDR_LEN;

	if (!pkt) {
		return;
	}

	if (!hci_if_rtk.cb) {
		goto fail;
	}

	if (pkt->type == HCI_ACL || pkt->type == HCI_ISO) {
		offset += HCI_IF_RX_ACL_PKT_BUF_OFFSET;
	} else if (pkt->type == HCI_SCO) {
		offset += HCI_IF_RX_SCO_PKT_BUF_OFFSET;
	}

	if (!IS_ALIGNED(pkt->buf, 4)) {
		BT_LOGE("rx buf not aligned\r\n");
		goto fail;
	}
	if ((pkt->len + offset) > hci_rx_buf_size(pkt->type)) {
		BT_LOGE("rx buf will overflow\r\n");
		goto fail;
	}

	memmove(pkt->buf + offset, pkt->buf, pkt->len);
	memset(pkt->buf, 0, offset);
	pkt->buf[0] = pkt->type;

	/* If indicate OK, stack will call hci_if_confirm when process of the packet is completed. */
	if (!hci_if_rtk.cb(HCI_IF_EVT_DATA_IND, true, pkt->buf, pkt->len + offset)) {
		goto fail;
	}
	return;

fail:
	hci_rx_pkt_free(pkt);
}

static struct hci_transport_cb rtk_stack_cb = {
	.recv = rtk_stack_recv,
};

static void _hci_if_send(uint8_t *buf, uint32_t len)
{
	uint16_t offset = HCI_IF_HDR_LEN;
	if (HCI_ACL == buf[0] || HCI_ISO == buf[0]) {
		offset += HCI_IF_TX_ACL_PKT_BUF_OFFSET;
	}

	hci_transport_send(buf[0], buf + offset, len - offset, true);
	if (hci_if_rtk.cb) {
		hci_if_rtk.cb(HCI_IF_EVT_DATA_XMIT, true, buf, len);
	}
}

static bool _tx_queue_send(uint8_t *buf, uint32_t len)
{
	bool ret = false;
	struct tx_packet_t pkt = {0};

	/* buf == NULL is exit flag */
	if (hci_if_rtk.state != HCI_IF_TASK_RUNNING && buf) {
		return false;
	}

	pkt.buf = buf;
	pkt.len = len;

	ret = osif_msg_send(hci_if_rtk.tx_queue, &pkt, BT_TIMEOUT_NONE);
	if (!ret) {
		if (hci_if_rtk.cb) {
			hci_if_rtk.cb(HCI_IF_EVT_DATA_XMIT, false, buf, len);
		}
		BT_LOGE("pkt send fail!\r\n");
	}

	return ret;
}

static void hci_if_task(void *context)
{
	(void)context;
	struct tx_packet_t pkt = {0};

	while (osif_msg_recv(hci_if_rtk.tx_queue, &pkt, BT_TIMEOUT_FOREVER)) {
		if (!pkt.buf) {
			break;
		}

		_hci_if_send(pkt.buf, pkt.len);
	}

	hci_if_rtk.state = HCI_IF_TASK_CLOSED;
	osif_task_delete(NULL);
}

bool hci_if_open(HCI_IF_CALLBACK callback)
{
	if (hci_controller_is_opened()) {
		BT_LOGD("Hci Driver Already Open!\r\n");
		goto end;
	}

	if (!hci_controller_open()) {
		return false;
	}

	if (!hci_is_mp_mode()) {
		/* HCI Transport Bridge to RTK Stack */
		hci_transport_register(&rtk_stack_cb);
	}

	memset(&hci_if_rtk, 0, sizeof(hci_if_rtk));
	hci_if_rtk.cb = callback;

	if (false == osif_msg_queue_create(&hci_if_rtk.tx_queue, HCI_IF_TX_QUEUE_NUM, sizeof(struct tx_packet_t))) {
		goto failed;
	}

	if (false == osif_task_create(&hci_if_rtk.task_hdl, "hci_if_task", hci_if_task, 0, HCI_IF_TASK_SIZE, HCI_IF_TASK_PRIO)) {
		osif_msg_queue_delete(hci_if_rtk.tx_queue);
		goto failed;
	}

	hci_if_rtk.state = HCI_IF_TASK_RUNNING;
	goto end;

failed:
	hci_controller_close();
	hci_controller_free();
	return false;

end:
	/* Upperstack will call hci_if_write immediately after this OPEN cb.
	 * Therefore, please make sure hci_if_write is ready here. */
	if (hci_if_rtk.cb) {
		hci_if_rtk.cb(HCI_IF_EVT_OPENED, true, NULL, 0);
	}
	BT_LOGA("Start upper stack\r\n");
	return true;
}

bool hci_if_close(void)
{
	if (!hci_controller_is_opened()) {
		return true;
	}

	hci_if_rtk.state = HCI_IF_TASK_CLOSING;

	_tx_queue_send(NULL, 0);

	while (hci_if_rtk.state != HCI_IF_TASK_CLOSED) {
		osif_delay(5);
	}

	hci_controller_close();

	osif_msg_queue_delete(hci_if_rtk.tx_queue);

	if (hci_if_rtk.cb) {
		hci_if_rtk.cb(HCI_IF_EVT_CLOSED, true, NULL, 0);
	}

	return true;
}

void hci_if_del_task(void)
{
	hci_if_close();
}

void hci_if_deinit(void)
{
	memset(&hci_if_rtk, 0, sizeof(hci_if_rtk));
	hci_controller_free();
}

/* Stack tx use, indicate to stack when tx done */
bool hci_if_write(uint8_t *buf, uint32_t len)
{
	return _tx_queue_send(buf, len);
}

bool hci_if_confirm(uint8_t *buf)
{
	return hci_rx_buf_find_free(buf);
}