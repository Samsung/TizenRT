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

#include <common/bk_err.h>
#include <driver/hal/hal_dma_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BK_ERR_SPI_NOT_INIT              (BK_ERR_SPI_BASE - 1) /**< SPI driver not init */
#define BK_ERR_SPI_INVALID_ID            (BK_ERR_SPI_BASE - 2) /**< SPI id number is invalid */
#define BK_ERR_SPI_ID_NOT_INIT           (BK_ERR_SPI_BASE - 3) /**< SPI ID not init */
#define BK_ERR_SPI_RX_TIMEOUT            (BK_ERR_SPI_BASE - 4) /**< SPI receive data timeout */
#define BK_ERR_SPI_TX_TIMEOUT            (BK_ERR_SPI_BASE - 5) /**< SPI send data timeout */
#define BK_ERR_SPI_FIFO_WR_NOT_READY     (BK_ERR_SPI_BASE - 6) /**< SPI tx fifo write not ready */
#define BK_ERR_SPI_FIFO_RD_NOT_READY     (BK_ERR_SPI_BASE - 6) /**< SPI rx fifo read not ready */
#define BK_ERR_SPI_DUPLEX_SIZE_NOT_EQUAL (BK_ERR_SPI_BASE - 7) /**< SPI duplex tx rx size not equal */

typedef uint8_t spi_unit_t; /**< spi uint id */

typedef enum {
	SPI_ID_0 = 0, /**< SPI id 1 */
#if (SOC_SPI_UNIT_NUM > 1)
	SPI_ID_1,     /**< SPI id 2 */
#endif
#if (SOC_SPI_UNIT_NUM > 2)
	SPI_ID_2,     /**< SPI id 3 */
#endif
	SPI_ID_MAX    /**< SPI id max */
} spi_id_t;

typedef enum {
	SPI_ROLE_SLAVE = 0, /**< SPI as slave */
	SPI_ROLE_MASTER,    /**< SPI as master */
} spi_role_t;

typedef enum {
	SPI_BIT_WIDTH_8BITS = 0, /**< SPI bit width 8bits */
	SPI_BIT_WIDTH_16BITS,    /**< SPI bit width 16bits */
} spi_bit_width_t;

typedef enum {
	SPI_POLARITY_LOW = 0, /**< SPI clock polarity low */
	SPI_POLARITY_HIGH,    /**< SPI clock polarity high */
} spi_polarity_t;

typedef enum {
	SPI_PHASE_1ST_EDGE = 0, /**< SPI clock phase the first edge */
	SPI_PHASE_2ND_EDGE,     /**< SPI clock phase the second edge */
} spi_phase_t;

typedef enum {
	SPI_POL_MODE_0 = 0, /**< SPI mode 0 */
	SPI_POL_MODE_1,     /**< SPI mode 1 */
	SPI_POL_MODE_2,     /**< SPI mode 2 */
	SPI_POL_MODE_3,     /**< SPI mode 3 */
} spi_mode_t;

typedef enum {
	SPI_4WIRE_MODE = 0, /**< SPI four wire mode */
	SPI_3WIRE_MODE,     /**< SPI three wire mode */
} spi_wire_mode_t;

typedef enum {
	SPI_MSB_FIRST = 0, /**< SPI MSB first */
	SPI_LSB_FIRST,     /**< SPI LSB first */
} spi_bit_order_t;

typedef enum {
	SPI_FIFO_INT_LEVEL_1 = 0, /**< SPI fifo int level 1 */
	SPI_FIFO_INT_LEVEL_16,    /**< SPI fifo int level 16 */
	SPI_FIFO_INT_LEVEL_32,    /**< SPI fifo int level 32 */
	SPI_FIFO_INT_LEVEL_48,    /**< SPI fifo int level 48 */
} spi_fifo_int_level;

typedef enum {
	SPI_CLK_XTAL = 0,
	SPI_CLK_APLL,
	SPI_CLK_UNKNOW = 0xff
} spi_src_clk_t;

#if (CONFIG_SPI_DMA)

typedef enum {
	SPI_DMA_MODE_DISABLE = 0, /**< SPI disable dma */
	SPI_DMA_MODE_ENABLE,      /**< SPI enable dma */
} spi_dma_mode_t;

#endif

typedef struct {
	spi_role_t role;           /**< SPI as master or slave */
	spi_bit_width_t bit_width; /**< SPI data bit witdth */
	spi_polarity_t polarity;   /**< SPI clock polarity */
	spi_phase_t phase;         /**< SPI clock phase */
	spi_wire_mode_t wire_mode; /**< SPI wire mode */
	uint32_t baud_rate;        /**< SPI transmit and receive SCK clock */
	spi_bit_order_t bit_order; /**< SPI bit order, MSB/LSB */
#if (CONFIG_SPI_DMA)
	spi_dma_mode_t dma_mode;   /**< SPI whether use dma */
	dma_id_t spi_tx_dma_chan;  /**< SPI tx dma channel */
	dma_id_t spi_rx_dma_chan;  /**< SPI rx dma channel */
#endif
} spi_config_t;

#ifdef __cplusplus
}
#endif

