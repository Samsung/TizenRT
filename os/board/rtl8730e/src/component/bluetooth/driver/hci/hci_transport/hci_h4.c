/*
 * Copyright (c) 2025 Realtek Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "osif.h"
#include "hci_controller.h"
#include "hci_transport.h"
#include "hci_platform.h"
#include "bt_debug.h"
#include "rtk_coex.h"
#include <string.h>

#define CONFIG_HCI_RX_PRIO        5
#define CONFIG_HCI_RX_STACK_SIZE  (2*1024)
/* RX ACL/SCO/ISO data */
#if defined(CONFIG_BT_DUAL_MODE) && CONFIG_BT_DUAL_MODE
#define CONFIG_HCI_RX_DATA_BUF_LEN     1056  /* shall be 4 byte aligned */
#else
#define CONFIG_HCI_RX_DATA_BUF_LEN     288   /* shall be 4 byte aligned */
#endif
#define CONFIG_HCI_RX_DATA_NUM         8
/* RX HCI event */
#define CONFIG_HCI_RX_EVT_BUF_LEN      260   /* shall be 4 byte aligned */
#define CONFIG_HCI_RX_EVT_NUM          8

#define CONFIG_HCI_RX_TOTAL_NUM        (CONFIG_HCI_RX_DATA_NUM + CONFIG_HCI_RX_EVT_NUM)


static struct hci_h4_t {
	struct hci_transport_cb *cb;
	void    *tx_mutex;

	void    *rx_run_sema;
	void    *rx_thread_hdl;
	void    *rx_busy;
	void    *rx_data_free;
	uint8_t rx_data_buf[CONFIG_HCI_RX_DATA_NUM][CONFIG_HCI_RX_DATA_BUF_LEN]; /* buf start pionter shall be 4 byte aligned */
	void    *rx_evt_free;
	uint8_t rx_evt_buf[CONFIG_HCI_RX_EVT_NUM][CONFIG_HCI_RX_EVT_BUF_LEN]; /* buf start pionter shall be 4 byte aligned */
	struct hci_rx_packet_t rxpkts[CONFIG_HCI_RX_TOTAL_NUM];
	bool    rx_disabled;
	bool    rx_run;

	/* used in uart irq handler */
	uint8_t state;
	uint16_t remain;
	uint8_t hdr[HCI_HDR_MAX_SIZE + 1];
	uint8_t hlen;
	uint16_t body_len;
	struct hci_rx_packet_t *pkt;
	uint8_t type;
	bool is_le_meta_evt;
	bool discardable;
} *h4 = NULL;

/* Receiver states. */
enum {
	ST_IDLE = 0,    /* Waiting for packet type. */
	ST_HDR,         /* Receiving packet header. */
	ST_PAYLOAD,     /* Receiving packet payload. */
	ST_DISCARD,     /* Dropping packet. */
};

_WEAK void bt_vendor_process_tx_frame(uint8_t type, uint8_t *pdata, uint16_t len)
{
	(void)type;
	(void)pdata;
	(void)len;
}

uint16_t hci_rx_buf_size(uint8_t type)
{
	return ((HCI_EVT == type) ? CONFIG_HCI_RX_EVT_BUF_LEN : CONFIG_HCI_RX_DATA_BUF_LEN);
}

static bool hci_rx_pkt_alloc(struct hci_rx_packet_t **pkt, uint8_t type)
{
	void *rx_free_q = ((HCI_EVT == type) ? h4->rx_evt_free : h4->rx_data_free);
	return osif_msg_recv(rx_free_q, pkt, BT_TIMEOUT_NONE);
}

bool hci_rx_pkt_free(struct hci_rx_packet_t *pkt)
{
	void *rx_free_q = ((HCI_EVT == pkt->type) ? h4->rx_evt_free : h4->rx_data_free);

	if (!osif_msg_send(rx_free_q, &pkt, BT_TIMEOUT_NONE)) {
		return false;
	}

	if (h4->rx_disabled) {
		BT_LOGA("Uart Rx recover.\r\n");
		hci_uart_rx_irq_handler(false); /* just in case the paused packet has no body */
		h4->rx_disabled = false;
		hci_uart_rx_irq(true);
	}
	return true;
}

bool hci_rx_buf_find_free(uint8_t *buf)
{
	int i;

	for (i = 0; i < CONFIG_HCI_RX_TOTAL_NUM; i++) {
		if (h4->rxpkts[i].buf == buf) {
			return hci_rx_pkt_free(&h4->rxpkts[i]);
		}
	}
	BT_LOGE("rx buf find failed\r\n");
	return false;
}

void hci_uart_rx_irq_handler(bool from_irq)
{
	if (from_irq && h4->rx_disabled) {
		return;
	}

	switch (h4->state) {
	case ST_IDLE: { /* Read H4 Type */
		h4->discardable = false;
		h4->is_le_meta_evt = false;
		if (hci_uart_read(&h4->type, 1)) {
			h4->hlen = hci_get_hdr_len(h4->type);
			if (h4->hlen) {
				h4->remain = h4->hlen;
				h4->state = ST_HDR;
			}
		}
		break;
	}
	case ST_HDR: {
		h4->remain -= hci_uart_read(h4->hdr + h4->hlen - h4->remain, h4->remain);
		if ((h4->type == HCI_EVT) && (h4->hlen == hci_get_hdr_len(HCI_EVT)) && (h4->remain < h4->hlen)) {
			if ((BT_HCI_EVT_LE_META_EVENT == ((struct hci_evt_hdr *)h4->hdr)->evt)) {
				h4->is_le_meta_evt = true;
				h4->remain++; /* read subevent to check if discardable */
				h4->hlen++;
			}
		}

		if (h4->remain == 0) {
			h4->body_len = hci_get_body_len(h4->hdr, h4->type);
			if ((h4->body_len + h4->hlen) > hci_rx_buf_size(h4->type)) {
				BT_LOGE("ERROR!!!, type %d, len %d\r\n", h4->type, h4->body_len);
				h4->remain = h4->body_len;
				h4->state = ST_DISCARD;
				break;
			}

			if (h4->is_le_meta_evt) {
				h4->body_len--; /* First byte of le_meta_event has been read already */
				if (BT_HCI_EVT_LE_ADVERTISING_REPORT == h4->hdr[h4->hlen - 1] ||
					BT_HCI_EVT_LE_EXT_ADVERTISING_REPORT == h4->hdr[h4->hlen - 1]) {
					h4->discardable = true;
				}
			}

			if (hci_rx_pkt_alloc(&h4->pkt, h4->type)) { /* h4->pkt is valid */
				h4->pkt->type = h4->type;
				h4->pkt->discardable = h4->discardable;
				h4->pkt->len = h4->hlen + h4->body_len;
				memcpy(h4->pkt->buf, h4->hdr, h4->hlen);
				if (!h4->body_len) { /* some cmd/acl has no body */
					osif_msg_send(h4->rx_busy, &h4->pkt, BT_TIMEOUT_NONE);
					h4->state = ST_IDLE;
				} else {
					h4->remain = h4->body_len;
					h4->state = ST_PAYLOAD;
				}
			} else {
				if (h4->discardable) {
					h4->remain = h4->body_len;
					h4->state = ST_DISCARD;
				} else {
					hci_uart_rx_irq(false);
					h4->rx_disabled = true;
					BT_LOGA("Uart Rx pause!!!!!!\r\n");
				}
			}
		}
		break;
	}
	case ST_PAYLOAD: {
		h4->remain -= hci_uart_read(h4->pkt->buf + h4->hlen + h4->body_len - h4->remain, h4->remain);
		if (h4->remain == 0) {
			osif_msg_send(h4->rx_busy, &h4->pkt, BT_TIMEOUT_NONE);
			h4->state = ST_IDLE;
		}
		break;
	}
	case ST_DISCARD: {
		uint8_t discard[32];
		h4->remain -= hci_uart_read(discard, (h4->remain > 32) ? 32 : h4->remain);
		if (h4->remain == 0) {
			h4->state = ST_IDLE;
		}
		break;
	}
	default:
		break;
	}
}

static void rx_thread(void *context)
{
	(void)context;
	struct hci_rx_packet_t *packet;

	osif_sem_give(h4->rx_run_sema);

	while (osif_msg_recv(h4->rx_busy, &packet, BT_TIMEOUT_FOREVER)) {
		if (!packet) { /* exit flag */
			break;
		}

#ifndef CONFIG_BT_INIC
		if (!hci_is_mp_mode()) {
			bt_coex_process_rx_frame(packet->type, packet->buf, packet->len);
		}
#endif

		if (h4->cb->recv) { /* send to upperstack */
			h4->cb->recv(packet);  /* pkt will be freed in this callback or in upper layer when this pkt finish process */
		} else {
			hci_rx_pkt_free(packet);
		}
	}

	osif_sem_give(h4->rx_run_sema);
	osif_task_delete(NULL);
}

void hci_transport_register(struct hci_transport_cb *cb)
{
	h4->cb = cb;
}

uint16_t hci_transport_send(uint8_t type, uint8_t *buf, uint16_t len, bool has_rsvd_byte)
{
	uint16_t sent = 0;

	if (!h4 || !h4->rx_run) {
		return 0;
	}

	if (type < HCI_CMD || type > HCI_ISO) {
		return 0;
	}

#ifndef CONFIG_BT_INIC
	if (!hci_is_mp_mode()) {
		bt_coex_process_tx_frame(type, buf, len);
		bt_vendor_process_tx_frame(type, buf, len);
	}
#endif

	osif_mutex_take(h4->tx_mutex, BT_TIMEOUT_FOREVER);
	if (has_rsvd_byte) {
		*(buf - 1) = type;
		/* Caller only send size of 'len' bytes, so return 'len' */
		sent = hci_uart_send(buf - 1, len + 1) - 1;
	} else {
		if (sizeof(type) == hci_uart_send(&type, sizeof(type))) {
			sent = hci_uart_send(buf, len);
		}
	}
	osif_mutex_give(h4->tx_mutex);

	return sent;
}

uint8_t hci_transport_open(void)
{
	int i;
	void *pkt;

	if (!h4) {
		h4 = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct hci_h4_t));
		if (!h4) {
			return HCI_FAIL;
		}
		memset(h4, 0, sizeof(struct hci_h4_t));
	}

#ifndef CONFIG_BT_INIC
	if (!hci_is_mp_mode()) {
		bt_coex_init();
	}
#endif

	if (false == osif_sem_create(&h4->rx_run_sema, 0, 1)) {
		goto failed;
	}
	if (false == osif_mutex_create(&h4->tx_mutex)) {
		goto failed;
	}
	if (false == osif_msg_queue_create(&h4->rx_busy, CONFIG_HCI_RX_TOTAL_NUM + 1, sizeof(struct hci_rx_packet_t *))) {
		goto failed;
	}
	if (false == osif_msg_queue_create(&h4->rx_data_free, CONFIG_HCI_RX_DATA_NUM, sizeof(struct hci_rx_packet_t *))) {
		goto failed;
	}
	if (false == osif_msg_queue_create(&h4->rx_evt_free, CONFIG_HCI_RX_EVT_NUM, sizeof(struct hci_rx_packet_t *))) {
		goto failed;
	}
	for (i = 0; i < CONFIG_HCI_RX_TOTAL_NUM; i++) {
		struct hci_rx_packet_t *pkt = &h4->rxpkts[i];
		if (i < CONFIG_HCI_RX_DATA_NUM) {
			pkt->buf = &h4->rx_data_buf[i][0];
			if (false == osif_msg_send(h4->rx_data_free, &pkt, BT_TIMEOUT_NONE)) {
				goto failed;
			}
		} else {
			pkt->buf = &h4->rx_evt_buf[i - CONFIG_HCI_RX_DATA_NUM][0];
			if (false == osif_msg_send(h4->rx_evt_free, &pkt, BT_TIMEOUT_NONE)) {
				goto failed;
			}
		}
	}

	if (false == osif_task_create(&h4->rx_thread_hdl, "h4_rx_thread", rx_thread, 0, CONFIG_HCI_RX_STACK_SIZE, CONFIG_HCI_RX_PRIO)) {
		goto failed;
	}
	if (false == osif_sem_take(h4->rx_run_sema, BT_TIMEOUT_FOREVER)) {
		goto failed;
	}

	h4->rx_run = true;
	return HCI_SUCCESS;

failed:
#ifndef CONFIG_BT_INIC
	if (!hci_is_mp_mode()) {
		bt_coex_deinit();
	}
#endif
	if (h4->rx_thread_hdl) {
		osif_task_delete(h4->rx_thread_hdl);
	}
	if (h4->rx_busy) {
		while (osif_msg_recv(h4->rx_busy, &pkt, BT_TIMEOUT_NONE));
		osif_msg_queue_delete(h4->rx_busy);
	}
	if (h4->rx_data_free) {
		while (osif_msg_recv(h4->rx_data_free, &pkt, BT_TIMEOUT_NONE));
		osif_msg_queue_delete(h4->rx_data_free);
	}
	if (h4->rx_evt_free) {
		while (osif_msg_recv(h4->rx_evt_free, &pkt, BT_TIMEOUT_NONE));
		osif_msg_queue_delete(h4->rx_evt_free);
	}
	if (h4->rx_run_sema) {
		osif_sem_delete(h4->rx_run_sema);
	}
	if (h4->tx_mutex) {
		osif_mutex_delete(h4->tx_mutex);
	}
	if (h4) {
		osif_mem_free(h4);
		h4 = NULL;
	}

	return HCI_FAIL;
}

void hci_transport_close(void)
{
	void *null_pkt = NULL;

	if (!h4) {
		return;
	}

	h4->rx_run = false;
	osif_msg_send(h4->rx_busy, &null_pkt, BT_TIMEOUT_NONE);
	osif_sem_take(h4->rx_run_sema, BT_TIMEOUT_FOREVER);
#ifndef CONFIG_BT_INIC
	if (!hci_is_mp_mode()) {
		bt_coex_deinit();
	}
#endif
}

void hci_transport_free(void)
{
	void *pkt;
	if (!h4) {
		return;
	}

	while (osif_msg_recv(h4->rx_busy, &pkt, BT_TIMEOUT_NONE));
	while (osif_msg_recv(h4->rx_data_free, &pkt, BT_TIMEOUT_NONE));
	while (osif_msg_recv(h4->rx_evt_free, &pkt, BT_TIMEOUT_NONE));
	osif_msg_queue_delete(h4->rx_busy);
	osif_msg_queue_delete(h4->rx_data_free);
	osif_msg_queue_delete(h4->rx_evt_free);
	osif_sem_delete(h4->rx_run_sema);
	osif_mutex_delete(h4->tx_mutex);

	osif_mem_free(h4);
	h4 = NULL;
}