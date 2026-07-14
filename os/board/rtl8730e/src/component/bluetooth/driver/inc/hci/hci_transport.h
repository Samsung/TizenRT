/*
 * Copyright (c) 2025 Realtek Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _HCI_TRANSPORTS_H_
#define _HCI_TRANSPORTS_H_

struct hci_rx_packet_t {
	uint8_t *buf;
	uint16_t len;
	uint8_t type;
	bool discardable;
};

struct hci_transport_cb {
	/** @brief Reception of the new packet is completed.
	 *
	 *  This callback indicates the stack that the new packet is receved correctly.
	 *
	 *  @param packet Packet received.
	 */
	void (*recv)(struct hci_rx_packet_t *packet);
};

uint16_t hci_rx_buf_size(uint8_t type);
bool hci_rx_pkt_free(struct hci_rx_packet_t *pkt);
bool hci_rx_buf_find_free(uint8_t *buf);
void hci_transport_register(struct hci_transport_cb *cb);
uint16_t hci_transport_send(uint8_t type, uint8_t *buf, uint16_t len, bool has_rsvd_byte);
uint8_t hci_transport_open(void);
void hci_transport_close(void);
void hci_transport_free(void);

extern struct hci_transport_cb hci_sa_cb;
uint8_t hci_sa_open(void);
void hci_sa_free(void);
uint8_t hci_sa_send(uint8_t type, uint8_t *buf, uint16_t len, bool is_sync);
uint8_t hci_sa_send_cmd_sync(uint16_t opcode, uint8_t *buf, uint16_t len);
#endif