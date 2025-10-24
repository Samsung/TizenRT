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

/**
 * @brief EFUSE defines
 * @defgroup bk_api_efuse_defs macos
 * @ingroup bk_api_efuse
 * @{
 */

#define BK_ERR_EFUSE_DRIVER_NOT_INIT    (BK_ERR_EFUSE_BASE - 1) /**< EFUSE driver not init */
#define BK_ERR_EFUSE_ADDR_OUT_OF_RANGE  (BK_ERR_EFUSE_BASE - 2) /**< EFUSE address is out of range */
#define BK_ERR_EFUSE_CANNOT_WRTIE       (BK_ERR_EFUSE_BASE - 3) /**< EFUSE cannot write, cannot change bit from 1 to 0 */
#define BK_ERR_EFUSE_WRTIE_NOT_EQUAL    (BK_ERR_EFUSE_BASE - 4) /**< EFUSE data read is not equal to the data written */
#define BK_ERR_EFUSE_READ_FAIL          (BK_ERR_EFUSE_BASE - 5) /**< EFUSE data read fail */

/**
 * efuse address defines
 * @Notice:   Each efuse address area has specific usage, please apply the area from PM
 *            before write data to the efuse address.
 */
#define EFUSE_ADDR_BYTE0      0
#define EFUSE_ADDR_BYTE1      1
#define EFUSE_ADDR_BYTE2      2
#define EFUSE_ADDR_BYTE3      3
#define EFUSE_ADDR_BYTE4      4
#define EFUSE_ADDR_BYTE5      5
#define EFUSE_ADDR_BYTE6      6
#define EFUSE_ADDR_BYTE7      7
#define EFUSE_ADDR_BYTE8      8
#define EFUSE_ADDR_BYTE9      9
#define EFUSE_ADDR_BYTE10     10
#define EFUSE_ADDR_BYTE11     11
#define EFUSE_ADDR_BYTE12     12
#define EFUSE_ADDR_BYTE13     13
#define EFUSE_ADDR_BYTE14     14
#define EFUSE_ADDR_BYTE15     15
#define EFUSE_ADDR_BYTE16     16
#define EFUSE_ADDR_BYTE17     17
#define EFUSE_ADDR_BYTE18     18
#define EFUSE_ADDR_BYTE19     19
#define EFUSE_ADDR_BYTE20     20
#define EFUSE_ADDR_BYTE21     21
#define EFUSE_ADDR_BYTE22     22
#define EFUSE_ADDR_BYTE23     23
#define EFUSE_ADDR_BYTE24     24
#define EFUSE_ADDR_BYTE25     25
#define EFUSE_ADDR_BYTE26     26
#define EFUSE_ADDR_BYTE27     27
#define EFUSE_ADDR_BYTE28     28
#define EFUSE_ADDR_BYTE29     29
#define EFUSE_ADDR_BYTE30     30
#define EFUSE_ADDR_BYTE31     31

/**
 * efuse device id defines
 * the device id stores in efuse address byte 0x18 ~ 0x1c
 * the efuse device id is set via ate cmds
 */
#define EFUSE_DEVICE_ID_BYTE_NUM     5
#define EFUSE_DEVICE_ID_SN_MINIMUM   48
#define EFUSE_DEVICE_ID_SN_MAXIMUM   122
#define EFUSE_WAFER_ID_MINIMUM       1
#define EFUSE_WAFER_ID_MAXIMUM       25

typedef uint8_t efuse_unit_t; /**< efuse uint id */


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

