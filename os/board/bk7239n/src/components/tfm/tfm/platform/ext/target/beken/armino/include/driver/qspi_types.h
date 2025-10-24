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

#include <driver/hal/hal_qspi_types.h>
#include "qspi_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BK_ERR_QSPI_NOT_INIT    (BK_ERR_QSPI_BASE - 1) /**< QSPI driver not init */
#define BK_ERR_QSPI_ID_NOT_INIT (BK_ERR_QSPI_BASE - 2) /**< QSPI id not init */

/**
 * @brief QSPI interrupt service routine
 */
typedef void (*qspi_isr_t)(qspi_id_t id, void  *param);

typedef struct {
	qspi_hal_t hal;
	uint8_t id_init_bits;
} qspi_driver_t;

typedef struct {
	qspi_isr_t callback;
	void *param;
} qspi_callback_t;

#ifdef __cplusplus
}
#endif

