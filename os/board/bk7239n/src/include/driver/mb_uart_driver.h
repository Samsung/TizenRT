// Copyright 2020-2022 Beken
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

#ifndef _mb_uart_driver_h_
#define _mb_uart_driver_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <common/bk_typedef.h>
#include <common/bk_err.h>

enum
{
	MB_UART0 = 0,
	MB_UART_MAX,
};

#define MB_UART_STATUS_OVF			0x01
#define MB_UART_STATUS_PARITY_ERR	0x02

typedef void (* mb_uart_isr_t)(void * param);

#if (CONFIG_CPU_CNT > 1)

/**
 * @brief	  initialize the mailbox UART device.
 *
 * Initialize the mailbox-emulated UART.
 *
 * @id     mailbox uart ID (MB_UART0, MB_UART1).
 *
 * @return
 *  - BK_OK: initialized success.
 *  - other: failed, fail code.
 */
bk_err_t bk_mb_uart_dev_init(u8 id);

/**
 * @brief	  de-initialize the mailbox UART device.
 *
 * De-initialize the mailbox-emulated UART.
 *
 * @id     mailbox uart ID (MB_UART0, MB_UART1).
 *
 * @return
 *  - BK_OK: initialized success.
 *  - other: failed, fail code.
 */
bk_err_t bk_mb_uart_dev_deinit(u8 id);

/**
 * @brief	  register rx ISR callback to the mailbox UART device.
 *
 *
 * @id     mailbox uart ID (MB_UART0, MB_UART1).
 *
 * @isr    rx isr, prototype is void (* mb_uart_isr_t)(void * param)
 *
 * @param  will be passed to rx isr callback.
 *
 * @attention   isr callback is called in interrupt disabled context, when received data.
 *
 * @return
 *  - BK_OK: success.
 *  - other: failed, fail code.
 */
bk_err_t bk_mb_uart_register_rx_isr(u8 id, mb_uart_isr_t isr, void *param);

/**
 * @brief	  register tx ISR callback to the mailbox UART device.
 *
 *
 * @id     mailbox uart ID (MB_UART0, MB_UART1).
 *
 * @isr    tx isr, prototype is void (* mb_uart_isr_t)(void * param)
 *
 * @param  will be passed to tx isr callback.
 *
 * @attention   isr callback is called in interrupt disabled context, after sent cmpelete.
 *
 * @return
 *  - BK_OK: success.
 *  - other: failed, fail code.
 */
bk_err_t bk_mb_uart_register_tx_isr(u8 id, mb_uart_isr_t isr, void *param);

/**
 * @brief	  get free FIFO space in UART device.
 *
 * @id     mailbox uart ID (MB_UART0, MB_UART1).
 *
 * @return
 *  - free FIFO space size in byte.
 */
u16 bk_mb_uart_write_ready(u8 id);

/**
 * @brief	  get data bytes in FIFO of UART device.
 *
 * @id     mailbox uart ID (MB_UART0, MB_UART1).
 *
 * @return
 *  - data bytes ready in mailbox-UART FIFO.
 */
u16 bk_mb_uart_read_ready(u8 id);

/**
 * @brief	  send one byte data via UART device.
 *
 * @id     mailbox uart ID (MB_UART0, MB_UART1).
 * @data   data byte to be sent.
 *
 * @return
 *  - data bytes sent via mailbox-UART (1 or 0).
 */
u16 bk_mb_uart_write_byte(u8 id, u8 data);

/**
 * @brief	  send data in buffer via UART device.
 *
 * @id     mailbox uart ID (MB_UART0, MB_UART1).
 * @data_buf   data to be sent.
 * @data_len   data bytes in buffer.
 *
 * @return
 *  - data bytes sent via mailbox-UART (0 ~ data_len).
 */
u16 bk_mb_uart_write(u8 id, u8 *data_buf, u16 data_len);

/**
 * @brief	  receive one byte data via UART device.
 *
 * @id     mailbox uart ID (MB_UART0, MB_UART1).
 * @data   data buffer to receive one byte.
 *
 * @return
 *  - data bytes received via mailbox-UART (1 or 0).
 */
u16 bk_mb_uart_read_byte(u8 id, u8 * data);

/**
 * @brief	  receive data via UART device.
 *
 * @id     mailbox uart ID (MB_UART0, MB_UART1).
 * @data_buf   used to receive data.
 * @buf_len    buffer length.
 *
 * @return
 *  - data bytes received via mailbox-UART (0 ~ buf_len).
 */
u16 bk_mb_uart_read(u8 id, u8 *data_buf, u16 buf_len);

/**
 * @brief	  check whether data has been flushed out to UART device.
 *
 * @id     mailbox uart ID (MB_UART0, MB_UART1).
 *
 * @return
 *  - 1 : no data is pending in FIFO.
 *  - 0 : there are data pending in FIFO.
 */
bool bk_mb_uart_is_tx_over(u8 id);

/**
 * @brief	  flush data in buffer via UART device in sync mode.
 *
 * @id     mailbox uart ID (MB_UART0, MB_UART1).
 * @data_buf   data to be sent.
 * @data_len   data bytes in buffer.
 *
 * @attention   this API is called in interrupt disabled context.
 *
 * @return
 *  - BK_OK: success.
 *  - other: failed, fail code.
 */
bk_err_t bk_mb_uart_dump(u8 id, u8 *data_buf, u16 data_len);

/**
 * @brief	  get status of UART device.
 *
 * @id     mailbox uart ID (MB_UART0, MB_UART1).
 *
 * @return
 *  - uart rx/tx status (0, MB_UART_STATUS_OVF, MB_UART_STATUS_PARITY_ERR, or bitwise of these 2 status.).
 */
u16 bk_mb_uart_get_status(u8 id);

#endif  /* (CONFIG_CPU_CNT > 1) */

/**    ============================    RPC end   ============================   **/

#ifdef __cplusplus
}
#endif

#endif /* _mb_uart_driver_h_ */

