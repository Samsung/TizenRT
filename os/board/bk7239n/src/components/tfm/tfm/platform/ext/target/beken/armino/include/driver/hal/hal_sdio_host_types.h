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

typedef uint8_t sdio_host_unit_t; /**< sdio host uint id */

#if (CONFIG_SDIO_V2P0)
typedef enum {
	SDIO_HOST_CLK_13M = 0,    /**< 2 division of clock frequency */
	SDIO_HOST_CLK_6_5M = 1,    /**< 4 division of clock frequency */
	SDIO_HOST_CLK_4_3M  = 2, /**< 6 division of clock frequency */
	SDIO_HOST_CLK_3_2M  = 3, /**< 8 division of clock frequency */

	SDIO_HOST_CLK_2_6M = 4,    /**< 10 division of clock frequency */
	SDIO_HOST_CLK_2_2M = 5,    /**< 12 division of clock frequency */

	SDIO_HOST_CLK_1_6M = 6,     /**< 16 division of clock frequency */
	SDIO_HOST_CLK_100K = 7,     /**< 256 division of clock frequency */

	SDIO_HOST_CLK_80M = 9,     /**< 320M/4 division of clock frequency */
	SDIO_HOST_CLK_40M = 11,     /**< 320M/8 division of clock frequency */
	SDIO_HOST_CLK_20M = 14,     /**< 320M/16 division of clock frequency */

} sdio_host_clock_freq_t;
#else
typedef enum {
	SDIO_HOST_CLK_26M = 26000000,    /**< 1 division of clock frequency */
	SDIO_HOST_CLK_13M = 13000000,    /**< 2 division of clock frequency */
	SDIO_HOST_CLK_6_5M = 6500000,    /**< 4 division of clock frequency */
	SDIO_HOST_CLK_3_2_5M  = 3250000, /**< 8 division of clock frequency */
	SDIO_HOST_CLK_1_6M = 1600000,    /**< 16 division of clock frequency */
	SDIO_HOST_CLK_800K = 800000,     /**< 32 division of clock frequency */
	SDIO_HOST_CLK_400K = 400000,     /**< 64 division of clock frequency */
	SDIO_HOST_CLK_200K = 200000,     /**< 128 division of clock frequency */
	SDIO_HOST_CLK_100K = 100000,     /**< 256 division of clock frequency */
} sdio_host_clock_freq_t;
#endif

typedef enum {
	SDIO_HOST_CMD_RSP_NONE = 0, /**< sdio host not need slave respond the command */
	SDIO_HOST_CMD_RSP_SHORT,    /**< sdio host need slave long response */
	SDIO_HOST_CMD_RSP_LONG,     /**< sdio host need slave short response */
} sdio_host_cmd_rsp_t;

typedef enum {
	SDIO_HOST_BUS_WIDTH_1LINE = 0, /**< sdio host bus width 1 line */
	SDIO_HOST_BUS_WIDTH_4LINE,     /**< sdio host bus width 4 line */
} sdio_host_bus_width_t;

typedef enum {
	SDIO_HOST_RSP0 = 0, /**< sdio host response regiseter 0 */
	SDIO_HOST_RSP1,     /**< sdio host response regiseter 1 */
	SDIO_HOST_RSP2,     /**< sdio host response regiseter 2 */
	SDIO_HOST_RSP3,     /**< sdio host response regiseter 3 */
} sdio_host_response_t;

typedef struct {
	uint32_t cmd_index;           /**< sdio command index, between 0 and 63 */
	uint32_t argument;            /**< sdio command argument */
	sdio_host_cmd_rsp_t response; /**< sdio command response type */
	uint32_t wait_rsp_timeout;    /**< sdio host wait for slave command response timeout */
	bool crc_check;			      /**< sdio host whether need to check slave response crc value */
} sdio_host_cmd_cfg_t;

#ifdef __cplusplus
}
#endif

