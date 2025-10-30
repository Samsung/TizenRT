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

#include <common/bk_include.h>
#include <driver/hal/hal_uart_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BK_ERR_UART_NOT_INIT              (BK_ERR_UART_BASE - 1) /**< UART driver not init */
#define BK_ERR_UART_INVALID_ID            (BK_ERR_UART_BASE - 2) /**< UART id number is invalid */
#define BK_ERR_UART_INVALID_SIZE          (BK_ERR_UART_BASE - 3) /**< UART read bytes length is more than fifo length */
#define BK_ERR_UART_RX_TIMEOUT            (BK_ERR_UART_BASE - 4) /**< UART receive data timeout */
#define BK_ERR_UART_BAUD_RATE_NOT_SUPPORT (BK_ERR_UART_BASE - 5) /**< UART baud rate not support */
#define BK_ERR_UART_ID_NOT_INIT           (BK_ERR_UART_BASE - 6) /**< UART id not init */
#define BK_ERR_UART_MUTEX_INIT            (BK_ERR_UART_BASE - 7) /**< UART mutex init failed */

/**
 * @brief UART interrupt service routine
 */
typedef void (*uart_isr_t)(uart_id_t id, void *param);

#ifdef __cplusplus
}
#endif

