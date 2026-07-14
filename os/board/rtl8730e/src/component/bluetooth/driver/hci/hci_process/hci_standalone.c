/*
 * Copyright (c) 2025 Realtek Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <osif.h>
#include <string.h>
#include "hci_transport.h"
#include "hci_common.h"

static void *recv_sem = NULL;
static uint8_t *hci_buf;

void hci_sa_recv(struct hci_rx_packet_t *pkt)
{
	memcpy(hci_buf, pkt->buf, pkt->len);
	osif_sem_give(recv_sem);
	hci_rx_pkt_free(pkt);
}

struct hci_transport_cb hci_sa_cb = {
	.recv = hci_sa_recv,
};

uint8_t hci_sa_open(void)
{
	if (false == osif_sem_create(&recv_sem, 0, 1)) {
		return HCI_FAIL;
	}

	return HCI_SUCCESS;
}

void hci_sa_free(void)
{
	osif_sem_delete(recv_sem);
	recv_sem = NULL;
}

uint8_t hci_sa_send(uint8_t type, uint8_t *buf, uint16_t len, bool is_sync)
{
	if (!is_sync) {
		if (len != hci_transport_send(type, buf, len, true)) {
			return HCI_FAIL;
		}

		return HCI_SUCCESS;
	}

	hci_buf = buf;

	if (len != hci_transport_send(type, buf, len, true)) {
		return HCI_FAIL;
	}

	osif_sem_take(recv_sem, BT_TIMEOUT_FOREVER);
	hci_buf = NULL;

	/* Then We can process Response */
	return HCI_SUCCESS;
}

/* 1 byte must be reserved ahead 'buf' */
uint8_t hci_sa_send_cmd_sync(uint16_t opcode, uint8_t *buf, uint16_t len)
{
	buf[0] = (uint8_t)(opcode >> 0);
	buf[1] = (uint8_t)(opcode >> 8);

	if (HCI_SUCCESS != hci_sa_send(HCI_CMD, buf, len, true)) {
		return HCI_FAIL;
	}

	/* Check Resp: OpCode and Status */
	if (buf[3] != (uint8_t)(opcode >> 0) || buf[4] != (uint8_t)(opcode >> 8) || buf[5] != 0x00) {
		return HCI_FAIL;
	}

	return HCI_SUCCESS;
}