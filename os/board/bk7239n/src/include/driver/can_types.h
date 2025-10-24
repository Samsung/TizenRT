// Copyright 2023-2024 Beken
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

#include <driver/hal/hal_can_types.h>
#include "gpio_driver.h"
#include "spinlock.h"
#include "bk_fifo.h"

#ifdef __cplusplus
extern "C" {

#endif

#define BK_ERR_CAN_NOT_INIT             (BK_ERR_CAN_BASE - 1) /**< CAN driver not init */
#define BK_ERR_CAN_INT_TYPE             (BK_ERR_CAN_BASE - 2) /**< CAN interrupte type error */
#define BK_ERR_CAN_BIT_ERROR			(BK_ERR_CAN_BASE - 3) /**< CAN bit error  */
#define BK_ERR_CAN_CHK_ERROR			(BK_ERR_CAN_BASE - 4) /**< CAN checksum error */
#define BK_ERR_CAN_TIMEOUT_ERROR		(BK_ERR_CAN_BASE - 5) /**< CAN timeout error */
#define BK_ERR_CAN_PARITY_ERROR			(BK_ERR_CAN_BASE - 6) /**< CAN identifier parity error */
#define BK_ERR_CAN_HAL_INVALID_ADDR		(BK_ERR_CAN_BASE - 7) /**< CAN invalid addr */
#define BK_ERR_CAN_HAL_INVALID_ARG		(BK_ERR_CAN_BASE - 8) /**< CAN invalid arg */

typedef struct {
	gpio_id_t           id;
	gpio_dev_t          dev;
} can_gpio_map_t;

typedef struct {
    can_gpio_map_t      tx;
    can_gpio_map_t      rx;
    can_gpio_map_t      standby;
} can_gpio_t;

enum can_status_e {
    CAN_STATUS_CLOSED = 0,
    CAN_STATUS_IDLE,
    CAN_STATUS_OPENED,
};

typedef struct {
    uint16_t            status;
    kfifo_t             *rx;
    kfifo_t             *tx;
} can_fifo_t;

typedef struct {
    uint8_t             status;
    beken_semaphore_t	rx_semphr;
    beken_semaphore_t	tx_semphr;
    // beken_mutex_t       rx_mutex;
    // beken_mutex_t       tx_mutex;
	spinlock_t			rx_spin;
	spinlock_t			tx_spin;
    can_fifo_t          can_f;
} can_env_t;


typedef struct {
    can_protocol_e       protocol;
    can_bit_rate_e      s_speed;
    can_bit_rate_e      f_speed;
    uint16_t            tx_size;        // must be pow(2, n)
    uint16_t            rx_size;
} can_config_t;

typedef struct {
    can_config_t        config;         /* can config */
    can_callback_des_t  err_cb;
    void                *priv;          /* priv data */
} can_dev_t;


#ifdef __cplusplus
}
#endif

