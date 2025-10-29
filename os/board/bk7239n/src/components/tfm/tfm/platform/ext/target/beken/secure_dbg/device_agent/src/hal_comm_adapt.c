/**
 * Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */
#include "hal.h"
#include "pal.h"
#include "string.h"
#include "hal_src_internal.h"
#include "driver/uart.h"
#include "sdkconfig.h"

static uart_id_t g_sec_dbg_id = UART_ID_0;

#if CONFIG_ENABLE_DELAY_IRAM
#define IRAM_SEC __attribute__((section(".iram")))
#else
#define IRAM_SEC
#endif

bk_err_t uart_read_ready(uart_id_t id);
int uart_read_byte(uart_id_t id);
bk_err_t uart_write_byte(uart_id_t id, uint8_t data);

int32_t hal_uart_rx_ready(void)
{
	return uart_read_ready(g_sec_dbg_id);
}

int32_t hal_uart_getc(uint8_t *s)
{
	return uart_read_byte(g_sec_dbg_id);
}

void hal_uart_putc(uint8_t c)
{
	uart_write_byte(g_sec_dbg_id, c);
}

IRAM_SEC void hal_udelay(uint32_t usecs)
{
    volatile    int     i           ;
    int cnt_us_max  ;

    cnt_us_max = (usecs * CONFIG_CPU_FREQ_HZ) / 5000;
    cnt_us_max = (cnt_us_max > 75) ? cnt_us_max - 75 : 0;

    for(i = 0; i < cnt_us_max; i ++)
	{
		;
	}
}

IRAM_SEC void hal_mdelay(uint32_t msecs)
{
    volatile int i;
    int cnt_ms_max;

    cnt_ms_max = (msecs * CONFIG_CPU_FREQ_HZ) / 5;
    cnt_ms_max = (cnt_ms_max > 75) ? cnt_ms_max - 75 : 0;
    for(i = 0; i < cnt_ms_max; i ++)
	{
		;
	}
}

// eof

