// Copyright 2022-2023 Beken
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
//#include <driver/hal/hal_sdio_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	SDIO_CLK_XTL = 0,
	SDIO_CLK_320M,
}SDIO_CLK_SEL_T;

//NOTICE:SDIO Clock divider hasn't DIV_128, so can't use (1<<(x+1)) as the clock divider
typedef enum
{
	SDIO_CLK_DIV_1 = 0,
	SDIO_CLK_DIV_2,
	SDIO_CLK_DIV_4,
	SDIO_CLK_DIV_8,
	SDIO_CLK_DIV_16,
	SDIO_CLK_DIV_32,
	SDIO_CLK_DIV_64,
	SDIO_CLK_DIV_256,
	SDIO_CLK_DIV_ERR,
}SDIO_CLK_DIV_T;

/**
 * @brief SDIO defines
 * @defgroup bk_api_sdio_defs macos
 * @ingroup bk_api_sdio
 * @{
 */
#define BK_ERR_SDIO_NOT_INIT              (BK_ERR_SDIO_BASE - 1) /**< SDIO driver not init */
#define BK_ERR_SDIO_PIN          		  (BK_ERR_SDIO_BASE - 2) /**< SDIO config PIN fail */
#define BK_ERR_SDIO_SLAVE_TX_TIMEOUT      (BK_ERR_SDIO_BASE - 3) /**< SDIO slave send data timeout */
#define BK_ERR_SDIO_MUTEX_INIT            (BK_ERR_SDIO_BASE - 4) /**< SDIO mutex init failed */
#define BK_ERR_SDIO_CHAN_ID_INVALID            (BK_ERR_SDIO_BASE - 5) /**< SDIO chan id is invalid */
#define BK_ERR_SDIO_DIRECT_INVALID            (BK_ERR_SDIO_BASE - 6) /**< SDIO chan direct is invalid */
#define BK_ERR_SDIO_LIST_INIT            (BK_ERR_SDIO_BASE - 7) /**< SDIO chan direct is invalid */
#define BK_ERR_SDIO_NO_MEM            (BK_ERR_SDIO_BASE - 8) /**< SDIO chan malloc memory fail */
#define BK_ERR_SDIO_NO_BUFFER            (BK_ERR_SDIO_BASE - 9) /**< SDIO chan buffer used finish */
#define BK_ERR_SDIO_NULL_POINTER_PARAM            (BK_ERR_SDIO_BASE - 10) /**< Err pointer param */
#define BK_ERR_SDIO_LIST            (BK_ERR_SDIO_BASE - 11) /**< Err link list */
#define BK_ERR_SDIO_NO_TRANSACTION_FINISH_NODE            (BK_ERR_SDIO_BASE - 12) /**< Err link list */
#define BK_ERR_SDIO_CHAN_DIRECT_NOT_INIT (BK_ERR_SDIO_BASE - 13) /**< The selected direct isn't init */


/**
 * @brief SDIO interrupt service routine
 */
typedef void (*sdio_isr_t)(void  *param);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

