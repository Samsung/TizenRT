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

#include <components/log.h>
#include <driver/hal/hal_spi_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint32_t spi_isr_cnt;
	uint32_t tx_fifo_isr_cnt;
	uint32_t rx_fifo_isr_cnt;
	uint32_t tx_finish_isr_cnt;
	uint32_t rx_finish_isr_cnt;
	uint32_t tx_underflow_isr_cnt;
	uint32_t rx_overflow_isr_cnt;
	uint32_t slave_release_isr_cnt;
} spi_statis_t;

#if defined(CONFIG_SPI_STATIS)

#define SPI_STATIS_DEC() spi_statis_t * spi_statis = NULL
#define SPI_STATIS_GET(_statis, _id) (_statis) = spi_statis_get_statis(_id)
#define SPI_STATIS_SET(_statis, _v) (_statis) = (_v)
#define SPI_STATIS_ADD(_statis, _v) (_statis) += (_v);
#define SPI_STATIS_INC(_statis) SPI_STATIS_ADD((_statis), 1)
bk_err_t spi_statis_init(void);
spi_statis_t* spi_statis_get_statis(spi_id_t id);
void spi_statis_dump(spi_id_t id);

#else

#define SPI_STATIS_DEC()
#define SPI_STATIS_GET(_statis, _id)
#define SPI_STATIS_SET(_id, _v)
#define SPI_STATIS_ADD(_statis, _v)
#define SPI_STATIS_INC(_statis)
#define spi_statis_init()
#define spi_statis_dump(id)

#endif

#ifdef __cplusplus
}
#endif

