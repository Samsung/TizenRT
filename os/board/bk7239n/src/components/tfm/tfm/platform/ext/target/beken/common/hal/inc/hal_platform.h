/*
 * Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#ifndef __HAL_PLATFORM_H__
#define __HAL_PLATFORM_H__

#include "types.h"

void hal_uart_putc(uint8_t c);
void hal_uart_putc_strict(uint8_t c);
size_t hal_uart_puts(const uint8_t *c);
int32_t hal_uart_getc(uint8_t *s);
int32_t hal_uart_rx_ready(void);

int32_t hal_platform_early_init(void);
int32_t hal_platform_init(void);
int32_t hal_platform_post_init(void);

void hal_udelay(uint32_t usecs);
void hal_mdelay(uint32_t msecs);
uint32_t hal_get_timestamp_second(void);
uint32_t hal_get_timestamp_counter(void);
uint32_t hal_get_rand();
void hal_reset(void);
void hal_led_on_off(int32_t led_id, bool is_on);

void hal_jtag_enable(void);
void hal_jtag_disable(void);

#endif /* __HAL_PLATFORM_H__ */
