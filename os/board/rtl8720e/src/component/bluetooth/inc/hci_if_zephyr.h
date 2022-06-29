/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef _HCI_IF_ZEPHYR_H_
#define _HCI_IF_ZEPHYR_H_

#include <stdio.h>
#include <stdbool.h>

int hci_drv_init(void);
void hci_drv_stop(void);
int hci_drv_deinit(void);

#endif