// Copyright 2020-2021 Beken
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

#include <common/bk_include.h>
#include <driver/uart_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Init the UART driver
 *
 * This API init the resoure common to all UART id:
 *   - Init UART driver control memory
 *
 * @attention 1. This API should be called before any other UART APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_driver_init(void);

/**
 * @brief     Deinit the UART driver
 *
 * This API free all resource related to UART and power down all UART id.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_driver_deinit(void);

/**
 * @brief     Get the UART whether in used
 *
 * @param id UART id
 *
 * @return
 *    - 1: in used
 *    - 0: not in used
 */
int bk_uart_is_in_used(uart_id_t id);

/**
 * @brief     Init the UART id
 *
 * This API init the UART id:
 *  - Power up the UART id
 *  - Configure the UART id clock
 *  - Map the UART id to dedicated GPIO port
 *  - Set the UART baud rate, data bits, parity, stop bits,flow ctrl and source clock
 *  - Start the UART id
 *
 * @param id UART id
 * @param config UART parameter settings
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_UART_NOT_INIT: UART driver not init
 *    - BK_ERR_NULL_PARAM: UART config parameter is NULL
 *    - BK_ERR_UART_INVALID_ID: UART id is invalid
 *    - BK_ERR_UART_BAUD_RATE_NOT_SUPPORT: UART baud rate is not support
 *    - others: other errors.
 */
bk_err_t bk_uart_init(uart_id_t id, const uart_config_t *config);

/**
 * @brief     Deinit the UART id
 *
 * This API deinit the UART id:
 *   - Stop the UART id
 *   - Reset all configuration of UART id to default value
 *   - Disable the UART id interrupt
 *   - Power down the UART id
 *
 * @param id UART id
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_deinit(uart_id_t id);

/**
 * @brief     Set the UART baud rate
 *
 * @param id UART id
 * @param baud_rate UART baud rate
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_set_baud_rate(uart_id_t id, uint32_t baud_rate);

/**
 * @brief     Set the UART data bits
 *
 * @param id UART id
 * @param data_bits UART data bits
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_set_data_bits(uart_id_t id, uart_data_bits_t data_bits);

/**
 * @brief     Set the UART stop bits
 *
 * @param id UART id
 * @param stop_bits UART stop bits
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_set_stop_bits(uart_id_t id, uart_stop_bits_t stop_bits);

/**
 * @brief     Set the UART partiy
 *
 * @param id UART id
 * @param partiy UART partiy
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_set_parity(uart_id_t id, uart_parity_t partiy);

/**
 * @brief     Set the UART hardware flow control
 *
 * @param id UART id
 * @param rx_threshold Threshold of Hardware RX flow control
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_set_hw_flow_ctrl(uart_id_t id, uint8_t rx_threshold);

/**
 * @brief     Set the UART threshold value for RX fifo full
 *
 * @param id UART id
 * @param threshold Threshold value above which RX fifo full interrupt is generated
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_set_rx_full_threshold(uart_id_t id, uint8_t threshold);

/**
 * @brief     Set the UART threshold value for TX fifo empty
 *
 * @param id UART id
 * @param threshold Threshold value below which TX fifo empty interrupt is generated
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_set_tx_empty_threshold(uart_id_t id, uint8_t threshold);

/**
 * @brief     Set the UART threshold timeout for receive data, unit is bit
 *
 * UART finish receiving data when the periods is more than timeout threshold.
 * If the time is expired, the UART_RX_STOP_END interrupt is triggered.
 *
 * @param id UART id
 * @param timeout_thresh timeout threshold
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_set_rx_timeout(uart_id_t id, uart_rx_stop_detect_time_t timeout_thresh);

/**
 * @brief     Disable the UART hardware flow control
 *
 * @param id UART id
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_disable_hw_flow_ctrl(uart_id_t id);

/**
 * @brief     Enable TX interrupt of UART id
 *
 * @param id UART id
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_enable_tx_interrupt(uart_id_t id);

/**
 * @brief     Disable TX interrupt of UART id
 *
 * @param id UART id
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_disable_tx_interrupt(uart_id_t id);

/**
 * @brief     Enable RX interrupt of UART id
 *
 * @param id UART id
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_enable_rx_interrupt(uart_id_t id);

/**
 * @brief     Disable RX interrupt of UART id
 *
 * @param id UART id
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_disable_rx_interrupt(uart_id_t id);

/**
 * @brief     Register the RX interrupt service routine for UART id
 *
 * @param id UART id
 * @param isr UART RX callback
 * @param param UART RX callback parameter
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_register_rx_isr(uart_id_t id, uart_isr_t isr, void *param);

/**
 * @brief     Register the TX interrupt service routine for UART id
 *
 * @param id UART id
 * @param isr UART TX callback
 * @param param UART TX callback parameter
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_register_tx_isr(uart_id_t id, uart_isr_t isr, void *param);

/**
 * @brief     Send data to the UART port from a given buffer and length
 *
 * @param id UART id
 * @param data data buffer address
 * @param size data length to send
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_write_bytes(uart_id_t id, const void *data, uint32_t size);

/**
 * @brief     UART read bytes from UART buffer
 *
 * @param id UART id
 * @param data pointer to the buffer
 * @param size data length to read
 * @param timeout_ms timeout ms, if set BEKEN_WAIT_FOREVER, read will wait forever
 *
 * @return
 *    - BK_ERR_UART_NOT_INIT: UART driver not init
 *    - BK_ERR_UART_INVALID_ID: UART id number is invalid
 *    - BK_ERR_UART_ID_NOT_INIT: UART id not init
 *    - BK_ERR_UART_RX_TIMEOUT: UART receive data timeout
 *    - others (>=0): The number of bytes read from UART FIFO
 */
int bk_uart_read_bytes(uart_id_t id, void *data, uint32_t size, uint32_t timeout_ms);

/**
 * @brief     Disable UART TX pin and set as GPIO
 *
 * @param id UART id
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_disable_rx(uart_id_t id);

/**
 * @brief     Disable UART TX pin and set as GPIO
 *
 * @param id UART id
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_disable_tx(uart_id_t id);

/**
 * @brief     Just set UART RX enable/disable, doesn't set pin
 *
 * @param id UART id
 * @param enable: true, enable, flase disable
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_set_enable_rx(uart_id_t id, bool enable);

/**
 * @brief     Just set UART TX enable/disable, doesn't set pin
 *
 * @param id UART id
 * @param enable: true, enable, flase disable
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_set_enable_tx(uart_id_t id, bool enable);

/**
 * @brief     Enable UART software fifo
 *
 * @param id UART id
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_enable_sw_fifo(uart_id_t id);

/**
 * @brief     Disable UART software fifo
 *
 * @param id UART id
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_disable_sw_fifo(uart_id_t id);

/**
 * @brief     Get the gpio pin when in ate detect
 *
 * @return the ate detect gpio pin
 */
uint32_t bk_uart_get_ate_detect_gpio(void);

/**
 * @brief     Register uart rx isr and disable software fifo
 *
 * @param id UART id
 * @param isr UART RX callback
 * @param param UART RX callback parameter
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_take_rx_isr(uart_id_t id, uart_isr_t isr, void *param);

/**
 * @brief     Recover uart rx isr
 *
 * @param id UART id
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_uart_recover_rx_isr(uart_id_t id);

/**
 * @brief     Get whether the uart tx over(tx fifo empty)
 *
 * @param id UART id
 *
 * @return
 *    - true: uart tx over
 *    - false: uart tx not over
 */
bool bk_uart_is_tx_over(uart_id_t id);

#ifdef __cplusplus
}
#endif

