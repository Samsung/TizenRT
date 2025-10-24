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

#include <driver/hal/hal_lin_types.h>
#include "gpio_driver.h"

#ifdef __cplusplus
extern "C" {

#endif

#define BK_ERR_LIN_NOT_INIT              (BK_ERR_LIN_BASE - 1) /**< LIN driver not init */
#define BK_ERR_LIN_INT_TYPE              (BK_ERR_LIN_BASE - 2) /**< LIN interrupte type error */
#define BK_ERR_LIN_BIT_ERROR             (BK_ERR_LIN_BASE - 3) /**< LIN bit error  */
#define BK_ERR_LIN_CHK_ERROR             (BK_ERR_LIN_BASE - 4) /**< LIN checksum error */
#define BK_ERR_LIN_TIMEOUT_ERROR         (BK_ERR_LIN_BASE - 5) /**< LIN timeout error */
#define BK_ERR_LIN_PARITY_ERROR          (BK_ERR_LIN_BASE - 6) /**< LIN identifier parity error */
#define BK_ERR_LIN_HAL_INVALID_ADDR      (BK_ERR_LIN_BASE - 7) /**< LIN invalid addr */
#define BK_ERR_LIN_HAL_INVALID_ARG       (BK_ERR_LIN_BASE - 8) /**< LIN invalid arg */

typedef void (*lin_isr_t)(void *arg);

typedef struct {
	lin_isr_t isr;
	void *arg;
} lin_callback_t;

typedef enum{
	LIN_INT_COMPLETE,
	LIN_INT_WAKEUP,
	LIN_INT_IDLE_TIMEOUT,
	LIN_INT_ABORTED,
	LIN_INT_DATA_REQ,
	LIN_INT_BIT_ERR,
	LIN_INT_CHK_ERR,
	LIN_INT_TIMEOUT_ERR,
	LIN_INT_PARITY_ERR,
	LIN_INT_MAX,
} lin_int_type_t;

typedef enum {
	LIN_RECV = 0,
	LIN_SEND = 1,
} lin_trans_t;
typedef enum {
	LIN_DISABLE = 0,
	LIN_ENABLE,
} lin_en_t;

typedef enum {
    LIN_BUS_INACTIVITY_4S,
	LIN_BUS_INACTIVITY_6S,
	LIN_BUS_INACTIVITY_8S,
	LIN_BUS_INACTIVITY_10S,
} lin_bus_inactivity_time_t;

typedef enum {
	LIN_WUP_REPEAT_180MS,
	LIN_WUP_REPEAT_200MS,
	LIN_WUP_REPEAT_220MS,
	LIN_WUP_REPEAT_240MS,
} lin_wup_repeat_time_t;

typedef enum {
	LIN_IDENT_MIN = -1,
	LIN_IDENT0,
	LIN_IDENT1,
	LIN_IDENT2,
	LIN_IDENT3,
	LIN_IDENT4,
	LIN_IDENT5,
	LIN_IDENT6,
	LIN_IDENT7,
	LIN_IDENT8,
	LIN_IDENT9,
	LIN_IDENT10,
	LIN_IDENT11,
	LIN_IDENT12,
	LIN_IDENT13,
	LIN_IDENT14,
	LIN_IDENT15,
	LIN_IDENT_MAX,
} lin_id_t;

typedef enum {
	LIN_CLASSIC,
	LIN_ENHANCED
} lin_checksum_t;

typedef struct {
	gpio_id_t id;
	gpio_dev_t dev;
} lin_gpio_map_t;

typedef struct {
	lin_gpio_map_t tx_gpio;
	lin_gpio_map_t rx_gpio;
	lin_gpio_map_t sleep_gpio;
} lin_gpio_t;

typedef enum {
	LIN_DATA_LEN_MIN = 0,
	LIN_DATA_LEN_1BYTES,
	LIN_DATA_LEN_2BYTES,
	LIN_DATA_LEN_3BYTES,
	LIN_DATA_LEN_4BYTES,
	LIN_DATA_LEN_5BYTES,
	LIN_DATA_LEN_6BYTES,
	LIN_DATA_LEN_7BYTES,
	LIN_DATA_LEN_8BYTES,
	LIN_DATA_LEN_MAX,
} lin_data_len_t;

typedef struct {
	uint8_t id;
	uint8_t tx_buf[LIN_DATA_LEN_MAX];
	uint8_t rx_buf[LIN_DATA_LEN_MAX];
	uint32_t len;
} lin_buf_t;

typedef struct {
	lin_channel_t chn;
	lin_dev_t dev;
	lin_data_len_t length;
	lin_checksum_t checksum;
	double rate;				/** master only **/
	lin_bus_inactivity_time_t bus_inactiv_time;	/** slave only **/
	lin_wup_repeat_time_t wup_repeat_time;		/** slave only **/
} lin_config_t;

#ifdef __cplusplus
}
#endif

