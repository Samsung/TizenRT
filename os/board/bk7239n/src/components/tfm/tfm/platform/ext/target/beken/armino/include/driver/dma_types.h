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
//

#pragma once

#include <driver/hal/hal_dma_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief DMA defines
 * @defgroup bk_api_dma_defs macos
 * @ingroup bk_api_dma
 * @{
 */
#define BK_ERR_DMA_ID             (BK_ERR_DMA_BASE - 1) /**< DMA id is invalid */
#define BK_ERR_DMA_NOT_INIT       (BK_ERR_DMA_BASE - 2) /**< DMA driver not init */
#define BK_ERR_DMA_ID_NOT_INIT    (BK_ERR_DMA_BASE - 3) /**< DMA id not init */
#define BK_ERR_DMA_ID_NOT_START   (BK_ERR_DMA_BASE - 4) /**< DMA id not start */
#define BK_ERR_DMA_INVALID_ADDR   (BK_ERR_DMA_BASE - 5) /**< DMA addr is invalid */
#define BK_ERR_DMA_ID_REINIT      (BK_ERR_DMA_BASE - 6) /**< DMA id has inited, if reinit,please de-init firstly */

/**
 * @brief DMA interrupt service routine
 */
typedef void (*dma_isr_t)(dma_id_t dma_id);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

