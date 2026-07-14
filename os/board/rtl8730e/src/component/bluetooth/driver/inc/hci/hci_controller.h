/*
 * Copyright (c) 2025 Realtek Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _HCI_CONTROLLER_H_
#define _HCI_CONTROLLER_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "hci_common.h"

#define PATCH_VERSION_INVALID   0
#define PATCH_VERSION_V1        1
#define PATCH_VERSION_V2        2
#define PATCH_VERSION_V3        3

/****** functions called by hci_process.c ******/
void hci_patch_set_chipid(uint8_t chipid);
void hci_set_work_baudrate(uint8_t *baudrate);
void hci_get_baudrate(uint8_t *baudrate, bool use_default_rate);
uint8_t hci_update_uart_baudrate(bool use_default_rate);
uint8_t hci_patch_get_patch_version(uint8_t **pp_patch_buf, uint32_t *p_patch_len);
/**********************************************/

/****** functions called by hci_download_vx.c ******/
uint8_t hci_patch_get_chipid(void);
/**********************************************/

/********** APIs called by hci driver *********/
bool hci_controller_open(void);
void hci_controller_close(void);
void hci_controller_free(void);
bool hci_controller_is_opened(void);
/**********************************************/

/********** APIs called by hci uart ***********/
void hci_uart_rx_irq_handler(bool from_irq);
/**********************************************/

#endif /* _HCI_CONTROLLER_H_ */
