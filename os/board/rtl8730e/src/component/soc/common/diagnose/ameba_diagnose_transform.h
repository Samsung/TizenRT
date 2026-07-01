/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBA_DIAGNOSE_TRANSFORM_H_
#define _AMEBA_DIAGNOSE_TRANSFORM_H_

#include "ameba_diagnose_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

int rtk_diag_uart_send(const u8 *data, u16 len);

#ifdef __cplusplus
}
#endif

#endif