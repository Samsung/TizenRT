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
#include <driver/sd_card_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Initializes the sd card
 *
 * This API init the sd card:
 *  - Initialize SDIO peripheral interface with default configuration
 *  - Identify card operating voltage
 *  - Card initialization
 *  - Set block size for card
 *
 * For details, please refer to SD Specifications Part1, chapter 4 Card Identification Mode
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_SDIO_HOST_NOT_INIT: sdio host driver not init
 *    - others: other errors.
 */
bk_err_t bk_sd_card_init(void);

/**
 * @brief     Deinit the sd card
 *
 * This API deinit the sdio card:
 *  - sdio host deinit
 *  - free related software resource
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_SDIO_HOST_NOT_INIT: sdio host driver not init
 *    - others: other errors.
 */
bk_err_t bk_sd_card_deinit(void);

/**
 * @brief     set sdcard clock
 *
 * @param clock frequency of sdcard
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sd_card_set_clock(uint32_t clock);

/**
 * @brief     Erase the specified memory area of the given sd card
 *
 * @param start_block_addr start block address
 * @param end_block_addr end block address
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sd_card_erase(uint32_t start_block_addr, uint32_t end_block_addr);

/**
 * @brief sd card read or write sync
 *        when read or write data to sd-card, sd card driver doesn't force do sync to sd-card.
 *        some sd-card has cache, it takes long time(more then 30ms) if do sync data from sd-card
 *        cache to sd-card flash memory.
 *        So add an API to APP do sync by itself.I.E: FATFS completes read or write will call this API
 *        to confirm data has flushed to sd-card's flash memory.
 *
 * @param 
 * @param end_block_addr end block address
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sd_card_rw_sync(void);

/**
 * @brief     Write block(s) to a specified address in a card.
 *            The data transfer is managed by blocking mode.
 *
 * @param data pointer to the buffer
 * @param block_addr block address where data will be written
 * @param block_num number of sd blocks to write
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sd_card_write_blocks(const uint8_t *data, uint32_t block_addr, uint32_t block_num);

/**
 * @brief     Read block(s) from a specified address in a card.
 *            The data transfer is managed by blocking mode.
 *
 * @param data pointer to the buffer
 * @param block_addr block address where data will be written
 * @param block_num number of sd blocks to write
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sd_card_read_blocks(uint8_t *data, uint32_t block_addr, uint32_t block_num);

/**
 * @brief     Get the sd card info
 *
 * @param card_info pointer to the sd_card_info_t structure that will contains the sd card status information
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sd_card_get_card_info(sd_card_info_t *card_info);

/**
 * @brief     Get the sd card size which unit is sector size(512Bytes)
 *
 * @param Compute the sd card size which base on card_info.csd.
 *
 * @return
 *    - >0: card size
 *    - 0: errors.
 */
uint32_t bk_sd_card_get_card_size(void);

/**
 * @brief     Get the current sd card data state
 *
 * @return card state
 */
sd_card_state_t bk_sd_card_get_card_state(void);

#ifdef __cplusplus
}
#endif


