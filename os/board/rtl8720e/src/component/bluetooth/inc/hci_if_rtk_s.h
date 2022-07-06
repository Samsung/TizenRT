/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef _HCI_IF_RTK_S_H_
#define _HCI_IF_RTK_S_H_

#include <stdio.h>
#include <stdbool.h>
#include "hci_config.h"
#include "hci/hci_common.h"

#ifdef __cplusplus
extern "C" {
#endif

void hci_tp_open(HCI_OPEN_CB open_cb, HCI_RECV_IND rx_ind);

void hci_tp_close(void);

void hci_tp_del(uint8_t param);

bool hci_tp_send(uint8_t *p_buf, uint16_t len, HCI_SEND_CB tx_cb);

uint16_t hci_tp_recv(uint8_t *p_buf, uint16_t size);

void hci_tp_config(uint8_t *p_buf, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif
