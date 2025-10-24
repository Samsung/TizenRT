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

#include <driver/int_types.h>
#include <common/bk_include.h>
#include <driver/hal/hal_dma_types.h>
#include <driver/hal/hal_spi_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*spi_isr_t)(spi_id_t id, void *param); /**< SPI interrupt service routine */

typedef struct {
	spi_id_t id;           /**< SPI id */
	icu_int_src_t int_src; /**< int source */
	int_group_isr_t isr;   /**< isr function */
	dma_dev_t dma_dev;     /**< dma device */
} spi_int_config_t;

#ifdef __cplusplus
}
#endif

