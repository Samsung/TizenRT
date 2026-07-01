/*
 * Copyright (c) 2025 Realtek Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _HCI_IF_RTK_H_
#define _HCI_IF_RTK_H_

#include <stdio.h>
#include <stdbool.h>
#include "hci_common.h"

void     hci_if_cfg_bdaddr(uint8_t *bdaddr);

#if defined(CONFIG_UPSTACK_HAS_HCI) && CONFIG_UPSTACK_HAS_HCI
uint8_t  hci_if_open(void);
uint8_t  hci_if_close(void);
#else

#define HCI_RX_ACL_PKT_BUF_OFFSET      (19 + 3*(sizeof(void*)-4))
#define HCI_TX_ACL_PKT_BUF_OFFSET      (11 + 2*(sizeof(void*)-4))
#define HCI_RX_ISO_PKT_BUF_OFFSET      (19 + 3*(sizeof(void*)-4))
#define HCI_TX_ISO_PKT_BUF_OFFSET      (11 + 2*(sizeof(void*)-4))
#define HCI_RX_SCO_PKT_BUF_OFFSET      (20 + 3*(sizeof(void*)-4))
#define HCI_TX_SCO_PKT_BUF_OFFSET      (0)

#define HCI_IF_RX_ACL_PKT_BUF_OFFSET   (HCI_RX_ACL_PKT_BUF_OFFSET)
#define HCI_IF_TX_ACL_PKT_BUF_OFFSET   (8)

#define HCI_IF_RX_SCO_PKT_BUF_OFFSET   (HCI_RX_SCO_PKT_BUF_OFFSET)
#define HCI_IF_TX_SCO_PKT_BUF_OFFSET   (HCI_TX_SCO_PKT_BUF_OFFSET)

typedef enum {
	HCI_IF_EVT_OPENED,     /* hci I/F open completed */
	HCI_IF_EVT_CLOSED,     /* hci I/F close completed */
	HCI_IF_EVT_DATA_IND,   /* hci I/F rx data indicated */
	HCI_IF_EVT_DATA_XMIT,  /* hci I/F tx data transmitted */
	HCI_IF_EVT_ERROR,      /* hci I/F error occurred */
} HCI_IF_EVT;

typedef bool (*HCI_IF_CALLBACK)(HCI_IF_EVT evt, bool status, uint8_t *buf, uint32_t len);

bool hci_if_open(HCI_IF_CALLBACK callback);
bool hci_if_close(void);
bool hci_if_write(uint8_t *buf, uint32_t len);
bool hci_if_confirm(uint8_t *buf);
void hci_if_deinit(void);

#endif

#endif