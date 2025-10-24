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

#include <components/log.h>

#define SD_CARD_TAG "sd_card"
#define SD_CARD_LOGI(...) BK_LOGI(SD_CARD_TAG, ##__VA_ARGS__)
#define SD_CARD_LOGW(...) BK_LOGW(SD_CARD_TAG, ##__VA_ARGS__)
#define SD_CARD_LOGE(...) BK_LOGE(SD_CARD_TAG, ##__VA_ARGS__)
#define SD_CARD_LOGD(...) BK_LOGD(SD_CARD_TAG, ##__VA_ARGS__)

/* sd card supported version */
#define SD_CARD_V1_X     0x0
#define SD_CARD_V2_X     0x1

/* sd supported memory cards */
#define SD_CARD_TYPE_SDSC       0
#define SD_CARD_TYPE_SDHC_SDXC  1

/* standard sd commands */
#define SD_CMD_GO_IDLE_STATE         0
#define SD_CMD_ALL_SEND_CID          2
#define SD_CMD_SEND_RELATIVE_ADDR    3   /* R6 */
#define SD_CMD_SELECT_DESELECT_CARD  7   /* R1b */
#define SD_CMD_SEND_IF_COND          8   /* R7 */
#define SD_CMD_SEND_CSD              9   /* R2 */
#define SD_CMD_STOP_TRANSMISSION     12  /* R1b */
#define SD_CMD_SEND_STATUS           13
#define SD_CMD_SET_BLOCKLEN          16  /* R1 */
#define SD_CMD_READ_SINGLE_BLOCK     17  /* R1 */
#define SD_CMD_READ_MULTIPLE_BLOCK   18  /* R1 */
#define SD_CMD_WRITE_BLOCK           24  /* R1 */
#define SD_CMD_WRITE_MULTIPLE_BLOCK  25  /* R1 */
#define SD_CMD_ERASE_WR_BLK_START    32
#define SD_CMD_ERASE_WR_BLK_END      33
#define SD_CMD_ERASE                 38
#define SD_CMD_SD_SEND_OP_COND       41  /* R3 */
#define SD_CMD_APP_CMD               55  /* R1 */
#define SD_CMD_APP_CMD6_SET_BUS_WIDTH 6  /* After CMD55 */

#define SD_CHECK_PATTERN          0x1AA

/* OCR register */
#define SD_OCR_BUSY               BIT(31) /* bit[31]: busy */
#define SD_OCR_HIGH_CAPACITY      BIT(30) /* bit[30] */
#define SD_OCR_VOLTAGE_3_2V_3_3V  0x00100000 // Voltage 3.2V to 3.3V flag
#define SD_OCR_VOLTAGE_ALL        0x00FF8000 // All Voltage flag
#define SD_DEFAULT_OCR            (SD_OCR_VOLTAGE_ALL | SD_OCR_HIGH_CAPACITY)

#define SD_BLOCK_SIZE             512

/* Command class support */
#define SD_SUPPORT_BASIC_CMD      	BIT(0)
#define SD_SUPPORT_RESERVED_0_CMD   BIT(1)
#define SD_SUPPORT_BLOCK_READ_CMD   BIT(2)
#define SD_SUPPORT_RESERVED_1_CMD   BIT(3)
#define SD_SUPPORT_BLOCK_WRITE_CMD  BIT(4)
#define SD_SUPPORT_ERASE_CMD      	BIT(5)
#define SD_SUPPORT_WRITE_PROTECTION_CMD   BIT(6)
#define SD_SUPPORT_LOCK_CARD_CMD   	BIT(7)
#define SD_SUPPORT_APPLICATION_CMD  BIT(8)
#define SD_SUPPORT_RESERVED_2_CMD   (BIT(9) | BIT(10) | BIT(11))


