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

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t qspi_unit_t; /**< QSPI uint id */
typedef uint8_t qspi_id_t;   /**< QSPI id */

typedef enum {
	QSPI_READ = 0, /**< QSPI read operation */
	QSPI_WRITE,    /**< QSPI write operation */
} qspi_op_t;

typedef enum {
	QSPI_SCLK_DCO = 0,  /**< QSPI source clock dco */
	QSPI_SCLK_XTAL_26M, /**< QSPI source clock xtal 26M */
	QSPI_SCLK_80M,      /**< QSPI source clock 80M */
	QSPI_SCLK_120M,     /**< QSPI source clock 120M */
	QSPI_SCLK_320M,     /**< QSPI source clock 320M */
	QSPI_SCLK_480M,     /**< QSPI source clock 480M */
} qspi_src_clk_t;

typedef enum {
	MEMORY_MAPPED_MODE = 0, /* memory mapped mode, used for psram/flash memory device */
	INDIRECT_MODE,          /* indirect mode. used for OLED */
} qspi_work_mode;

typedef struct {
	qspi_src_clk_t src_clk; /**< QSPI source clock */
	uint32_t src_clk_div;   /**< QSPI source clock divide number*/
	uint32_t clk_div;       /**< QSPI controller clock divide number */
} qspi_config_t;

typedef enum {
	QSPI_1WIRE = 0, /**< QSPI 1_wire mode, standard SPI */
	QSPI_2WIRE,     /**< QSPI 2_wire mode, DUAL SPI */
	QSPI_4WIRE,     /**< QSPI 4_wire mode, QUAD SPI */
} qspi_wire_mode_t;

typedef enum {
	QSPI_PSRAM = 0, /**< QSPI PSRAM */
	QSPI_FLASH,     /**< QSPI FLASH */
	QSPI_OLED,      /**< QSPI OLED */
} qspi_device_t;

typedef struct {
	qspi_wire_mode_t wire_mode; /**< wire mode */
	qspi_work_mode work_mode;   /**< work mode */
	qspi_op_t op;               /**< QSPI operation */
	uint32_t cmd;               /**< QSPI command */
	uint32_t addr;              /**< QSPI address */
	uint32_t dummy_cycle;       /**< QSPI dummy cycle */
	uint32_t data_len;          /**< QSPI data length*/
	qspi_device_t device;       /**< QSPI slave device*/
} qspi_cmd_t;

#ifdef __cplusplus
}
#endif
