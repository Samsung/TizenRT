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
#include <driver/hal/hal_sdio_host_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BK_ERR_SDIO_HOST_NOT_INIT            (BK_ERR_SDIO_HOST_BASE - 1) /**< sdio host driver not init */
#define BK_ERR_SDIO_HOST_CMD_RSP_TIMEOUT     (BK_ERR_SDIO_HOST_BASE - 2) /**< sdio host wait slave command over time */
#define BK_ERR_SDIO_HOST_CMD_RSP_CRC_FAIL    (BK_ERR_SDIO_HOST_BASE - 3) /**< sdio host wait slave command crc fail */
#define BK_ERR_SDIO_HOST_TX_FIFO_FULL        (BK_ERR_SDIO_HOST_BASE - 4) /**< sdio host tx fifo full */
#define BK_ERR_SDIO_HOST_RX_FIFO_EMPTY       (BK_ERR_SDIO_HOST_BASE - 5) /**< sdio host rx fifo empty */
#define BK_ERR_SDIO_HOST_DATA_TIMEOUT        (BK_ERR_SDIO_HOST_BASE - 6) /**< sdio host wait slave data timeout */
#define BK_ERR_SDIO_HOST_DATA_CRC_FAIL       (BK_ERR_SDIO_HOST_BASE - 7) /**< sdio host receive slave data crc fail */
#define BK_ERR_SDIO_HOST_INVALID_VOLT_RANGE  (BK_ERR_SDIO_HOST_BASE - 8) /**< sd card invalid voltage range */
#define BK_ERR_SDIO_HOST_UNSUPPORTED_FEATURE (BK_ERR_SDIO_HOST_BASE - 9) /**< sd card unsupport feature */
#define BK_ERR_SDIO_HOST_READ_DATA_FAIL (BK_ERR_SDIO_HOST_BASE - 10) /**< sdio host read data from FIFO fail */

typedef void (*sdio_host_isr_t)(void *param); /**< SDIO host interrupt service routine */

typedef enum {
	SDIO_HOST_DATA_DIR_RD = 0, /**< sdio host data direction read */
	SDIO_HOST_DATA_DIR_WR, 	   /**< sdio host data direction write */
} sdio_host_data_direction_t;

typedef struct {
	sdio_host_clock_freq_t clock_freq; /**< sdio host clock frequency */
	sdio_host_bus_width_t bus_width;   /**< sdio host data bus width */
	uint8_t dma_tx_en;	//write data to SDIO FIFO whether enable DMA(it depend on CONFIG_SDIO_GDMA_EN)
	uint8_t dma_rx_en;	//read data from SDIO FIFO whether enable DMA(it depend on CONFIG_SDIO_GDMA_EN)
} sdio_host_config_t;

typedef struct {
	uint32_t data_timeout;               /**< sdio host data timeout */
	uint32_t data_len;                   /**< sdio host data length */
	uint32_t data_block_size;            /**< sdio host data block size */
	sdio_host_data_direction_t data_dir; /**< sdio host data direction */
} sdio_host_data_config_t;

#ifdef __cplusplus
}
#endif

