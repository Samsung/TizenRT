/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef _TRACE_UART_H_
#define _TRACE_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

bool trace_print(void *data, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif

