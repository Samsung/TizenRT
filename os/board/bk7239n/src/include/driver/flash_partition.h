/**
 *********************************************************************************
 * @file flash_partition.h
 * @brief This file provides all the headers of Flash operation functions..
 *********************************************************************************
 *
 *Copyright 2020-2025 Beken
 *
 *Licensed under the Apache License, Version 2.0 (the "License");
 *you may not use this file except in compliance with the License.
 *You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *Unless required by applicable law or agreed to in writing, software
 *distributed under the License is distributed on an "AS IS" BASIS,
 *WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *See the License for the specific language governing permissions and
 *limitations under the License.
 *********************************************************************************
*/
#pragma once
#include <common/bk_include.h>
#ifdef __cplusplus
extern "C" {
#endif


#define PAR_OPT_READ_POS     (0)
#define PAR_OPT_READ_DIS     (0x0u << PAR_OPT_READ_POS)
#define PAR_OPT_READ_EN     (0x1u << PAR_OPT_READ_POS)

#define PAR_OPT_WRITE_POS     (1)
#define PAR_OPT_WRITE_DIS     (0x0u << PAR_OPT_WRITE_POS)
#define PAR_OPT_WRITE_EN     (0x1u << PAR_OPT_WRITE_POS)

#define PAR_OPT_EXECUTE_POS     (2)
#define PAR_OPT_EXECUTE_DIS     (0x0u << PAR_OPT_EXECUTE_POS)
#define PAR_OPT_EXECUTE_EN     (0x1u << PAR_OPT_EXECUTE_POS)



typedef enum
{
    BK_FLASH_EMBEDDED = 0,
    BK_FLASH_SPI = 1,
    BK_FLASH_MAX = 2,
    BK_FLASH_NONE = 3,
}bk_flash_t;

typedef enum
{
    BK_PARTITION_BOOTLOADER = 0,
    BK_PARTITION_APPLICATION = 1,
    BK_PARTITION_OTA = 2,
    BK_PARTITION_APPLICATION1 = 3,
    BK_PARTITION_MATTER_FLASH = 4,
    BK_PARTITION_RF_FIRMWARE = 5,
    BK_PARTITION_NET_PARAM = 6,
    BK_PARTITION_USR_CONFIG = 7,
    BK_PARTITION_OTA_FINA_EXECUTIVE = 8,
    BK_PARTITION_APPLICATION2 = 9,
    BK_PARTITION_EASYFLASH = 10,
    BK_PARTITION_MAX,
}bk_partition_t;

typedef struct
{
    bk_flash_t partition_owner;			 	/**< flash partition owners */
    const char *partition_description;		/**< flash partition description */
    uint32_t partition_start_addr;			/**< flash partition start address */
    uint32_t partition_length;				/**< flash partition length */
    uint32_t partition_options;				/**< flash partition options */
}bk_logic_partition_t;

/**
 * @brief   Get the infomation of the specified flash area
 *
 * @param partition:  The target flash logical partition
 *
 * @return bk flash logic partition struct
 */
bk_logic_partition_t *bk_flash_partition_get_info(bk_partition_t partition);

/**
 * @brief   Erase an area on a Flash logical partition
 *
 * @note    Erase on an address will erase all data on a sector that the
 *          address is belonged to, this function does not save data that
 *          beyond the address area but in the affected sector, the data
 *          will be lost.
 *
 * @param  partition: The target flash logical partition which should be erased
 * @param  offset: Start address of the erased flash area
 * @param  size: Size of the erased flash area
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors
 */
bk_err_t bk_flash_partition_erase(bk_partition_t partition, uint32_t offset, uint32_t size);

/**
 * @brief  Write data to an area on a Flash logical partition
 *
 * @param  partition: The target flash logical partition which should be written
 * @param  buffer: Pointer to the data buffer that will be written to flash
 * @param  off_set: The offset of write address
 * @param  buffer_len: The length of the buffer
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors
 */
bk_err_t bk_flash_partition_write(bk_partition_t partition, const uint8_t *buffer, uint32_t offset, uint32_t buffer_len);

/**
 * @brief    Read data from an area on a Flash to data buffer in RAM
 *
 * @param    partition: The target flash logical partition which should be read
 * @param    out_buffer: Pointer to the data buffer that stores the data read from flash
 * @param    offsets: The offset of read address
 * @param    buffer_len: The length of the buffer
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors
 */
bk_err_t bk_flash_partition_read(bk_partition_t partition, uint8_t *out_buffer, uint32_t offset, uint32_t buffer_len);

/**
 * @brief     Write data to flash (only operating 4k flash space)
 *
 * @param partition partition to write  (eg:partition BK_PARTITION_RF_FIRMWARE)
 * @param user_buf the pointer to data which is to write
 * @param size size to write
 * @param offset offset to write
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_FLASH_ADDR_OUT_OF_RANGE: flash address is out of range
 *    - others: other errors.
 */
bk_err_t bk_spec_flash_write_bytes(bk_partition_t partition, const uint8_t *user_buf, uint32_t size,uint32_t offset);

#ifdef __cplusplus
}
#endif
