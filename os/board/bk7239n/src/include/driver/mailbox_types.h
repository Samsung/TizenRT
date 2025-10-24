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
#include <common/bk_include.h>
#include <common/bk_err.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief MAILBOX APIs
 * @addtogroup bk_api_mailbox MAILBOX API group
 * @{
 */

/**
 * @brief MAILBOX defines
 * @defgroup bk_api_mailbox macos
 * @ingroup bk_api_mailbox
 * @{
 */

#define BK_ERR_MAILBOX_SRC_DST           (BK_ERR_MAILBOX_BASE - 1) /**< MAILBOX source address error*/
#define BK_ERR_MAILBOX_ID                (BK_ERR_MAILBOX_BASE - 2) /**< MAILBOX BOX number error */
#define BK_ERR_MAILBOX_FLAG              (BK_ERR_MAILBOX_BASE - 3) /**< MAILBOX flag error */
#define BK_ERR_MAILBOX_CALLBACK          (BK_ERR_MAILBOX_BASE - 4) /**< MAILBOX callback function error */
#define BK_ERR_MAILBOX_TIMEOUT           (BK_ERR_MAILBOX_BASE - 5) /**< MAILBOX waite timeout */
#define BK_ERR_MAILBOX_NOT_INIT          (BK_ERR_MAILBOX_BASE - 6) /**< MAILBOX isnt init */
#define BK_ERR_MAILBOX_REPEAT_INIT       (BK_ERR_MAILBOX_BASE - 7) /**< MAILBOX repeat init */


/**
 * @brief A MAILBOX has multiple boxes, one box has four spaces to fill in data
 *        Define the meaning of each space by user.
 *
 *                 |_____________________________|      |_____________________________|
 *                 |_____________________________|      |_____________________________|
 *                 | Mailbox0                    |      | Mailbox1                    |
 *                 |______________|______________|      |______________|______________|
 *                 |     box0     |     box1     |      |     box0     |     box1     |
 *                 | ____________ | ____________ |      | ____________ | ____________ |
 *                 | | param0   | | | param0   | |      | | param0   | | | param0   | |
 *                 | | param1   | | | param1   | |      | | param1   | | | param1   | |
 *                 | | param2   | | | param2   | |      | | param2   | | | param2   | |
 *                 | | param3   | | | param3   | |      | | param3   | | | param3   | |
 *                 | |__________| | |__________| |      | |__________| | |__________| |
 *                 | _____________|______________|      |______________|______________|
 *                 |_____________________________|      |_____________________________|
 */

typedef struct
{
  uint32_t param0;    /**< MAILBOX Space 0 */
  uint32_t param1;    /**< MAILBOX Space 1 */
  uint32_t param2;    /**< MAILBOX Space 2 */
  uint32_t param3;    /**< MAILBOX Space 3 */
} mailbox_data_t;

typedef enum{
  MAILBOX_CPU0 = 0,         /**< MAILBOX CPU0 endpoint */
  MAILBOX_CPU1 = 1,         /**< MAILBOX CPU1 endpoint */
  MAILBOX_CPU2 = 2,         /**< MAILBOX CPU2 endpoint */
  MAILBOX_DSP  = 2,         /**< MAILBOX DSP endpoint */
  MAILBOX_BT   = 3,         /**< MAILBOX BT endpoint */
  MAILBOX_ENDPOINT_INVALID, /**< MAILBOX invalid endpoint*/
} mailbox_endpoint_t;

/**
 * @brief MAILBOX interrupt service routine
 */

typedef void (*mailbox_callback_t)(mailbox_data_t *data);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
