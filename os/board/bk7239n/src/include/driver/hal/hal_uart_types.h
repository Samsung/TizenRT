// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <common/bk_err.h>

#ifdef __cplusplus
extern "C" {
#endif

//UART BAUDRATE:User can add special baudrate and set it as the really value.
//I.E: #define UART_BARDRATE_USER_XXX 1234
#define UART_BAUDRATE_3250000        3250000
#define UART_BAUDRATE_2000000        2000000
#define UART_BAUDRATE_921600         921600
#define UART_BAUDRATE_460800         460800
#define UART_BAUDRATE_230400         230400
#define UART_BAUDRATE_115200         115200  //default
#define UART_BAUDRATE_3000           3250
#define UART_BAUDRATE_19200          19200
#define UART_BAUDRATE_9600           9600

#ifndef CONFIG_UART_PRINT_BAUD_RATE
#define UART_BAUD_RATE               UART_BAUDRATE_115200
#else
#define UART_BAUD_RATE               CONFIG_UART_PRINT_BAUD_RATE
#endif

#define UART_CLOCK_FREQ_10M          10000000
#define UART_CLOCK_FREQ_48M          48000000
#define UART_CLOCK_FREQ_24M          24000000
#define UART_CLOCK_FREQ_26M          26000000
#define UART_CLOCK_FREQ_52M          52000000
#define UART_CLOCK_FREQ_50M          50000000
#define UART_CLOCK_FREQ_120M         120000000
#define UART_CLOCK_FREQ_80M          80000000

#define UART_CLOCK                   CONFIG_XTAL_FREQ

#define UART_TX_FIFO_THRESHOLD       (0x20)
#define UART_RX_FIFO_THRESHOLD       (0x40)

typedef uint8_t uart_unit_t; /**< uart uint id */

typedef enum {
	UART_ID_0 = 0, /**< UART id 0 */
	UART_ID_1,     /**< UART id 1 */
	UART_ID_2,     /**< UART id 2 */
#if (SOC_UART_ID_NUM_PER_UNIT  >= 4)
	UART_ID_3,     /**< UART id 3 */
#endif
	UART_ID_MAX    /**< UART id max */
} uart_id_t;

typedef enum {
	UART_SCLK_DCO = 0,  /**< UART source clock dco */
	UART_SCLK_XTAL_26M, /**< UART source clock xtal 26M */
	UART_SCLK_APLL,     /**< UART source clock apll */
	UART_SCLK_80M,      /**< UART source clock 80M */
} uart_src_clk_t;

typedef enum {
	UART_MODE_UART = 0, /**< UART frame mode uart */
	UART_MODE_IDRA,     /**< UART frame mode idra */
} uart_frame_mode_t;

typedef enum {
	UART_DATA_5_BITS = 0, /**< UART data bits 5bits */
	UART_DATA_6_BITS,     /**< UART data bits 6bits */
	UART_DATA_7_BITS,     /**< UART data bits 7bits */
	UART_DATA_8_BITS,     /**< UART data bits 8bits */
} uart_data_bits_t;

typedef enum {
	UART_PARITY_NONE = 0, /**< UART parity none */
	UART_PARITY_ODD,      /**< UART parity odd */
	UART_PARITY_EVEN,     /**< UART parity even */
} uart_parity_t;

typedef enum {
	UART_STOP_BITS_1 = 0, /**< UART stop bits 1bit */
	UART_STOP_BITS_2,     /**< UART stop bits 2bits */
} uart_stop_bits_t;

typedef enum {
	UART_FLOWCTRL_DISABLE = 0, /**< UART flow control disable */
	UART_FLOWCTRL_CTS_RTS,     /**< UART flow control cts rts */
} uart_flow_control_t;

typedef enum {
	UART_RX_STOP_DETECT_TIME_32_BITS = 0, /**< UART rx stop time 32 bits */
	UART_RX_STOP_DETECT_TIME_64_BITS,     /**< UART rx stop time 64 bits  */
	UART_RX_STOP_DETECT_TIME_128_BITS,    /**< UART rx stop time 128 bits */
	UART_RX_STOP_DETECT_TIME_256_BITS     /**< UART rx stop time 256 bits */
} uart_rx_stop_detect_time_t;

typedef enum {
	UART_DMA_DISABLE,
	UART_DMA_ENABLE,
}uart_dma_enable_t;

typedef struct {
	uint32_t baud_rate;            /**< UART baud rate */
	uart_data_bits_t data_bits;    /**< UART data bits */
	uart_parity_t parity;          /**< UART parity */
	uart_stop_bits_t stop_bits;    /**< UART stop bits */
	uart_flow_control_t flow_ctrl; /**< UART flow control  */
	uart_src_clk_t src_clk;        /**< UART source clock */

/*
 * !!! WARNING !!! !!! WARNING !!!
 * If APP wants to set rx_dma_en, please enable macro of CONFIG_UART_RX_DMA.
 *
 * Header file forces enable this elem(not controlled by CONFIG_UART_RX_DMA).
 * When build libs(the lib function calls this struct) it does not enable the macro of CONFIG_UART_RX_DMA,
 * but build UART Driver codes, it enables the macro of CONFIG_UART_RX_DMA.
 * Then the lib function call UART DRIVER hasn't set param of rx_dma_en, but UART driver API
 * init function will check this item and maybe set RX dma enable.
 * If defined these elems, the value will be 0 if APP not set it.
 */
#if 1
	/*
	 * Special feature used for UART RX data.
	 * When erase Flash, CPU can't get instructions from flash then it can't work.
	 * NOTES:
	 * 1. The parameter of rx_dma_en,should not be changed after UART init.
	 * 2. If enable rx_dma, it will hold a DMA channel and not release it.
	 * 3. It will force enable sw_fifo, which relate with "bk_uart_enable_sw_fifo"
	 *    please don't call "bk_uart_disable_sw_fifo" after enabled rx_dma_en.
	 * 4. Does not support UART TX DMA,because UART TX data isn't too large and CPU speed is enough.
	 */
	uart_dma_enable_t rx_dma_en;
	uart_dma_enable_t tx_dma_en;
#endif
} uart_config_t;

#ifdef __cplusplus
}
#endif

