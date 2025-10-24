// Copyright 2020-2025 Beken
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

#include "sdkconfig.h"

typedef enum _ota_bootloader_boot_flag
{
    OTA_BOOTLOADER_BOOT_FLAG_INVALID  	= 0,
    OTA_BOOTLOADER_BOOT_FLAG_PRIMARY  	= 1,
    OTA_BOOTLOADER_BOOT_FLAG_SECONDARY  = 2,
    OTA_BOOTLOADER_BOOT_FLAG_MAX,
}ota_bootloader_boot_flag_e;

typedef enum _ota_security_area
{
    OTA_SECURITY_AREA_A  = 0,
    OTA_SECURITY_AREA_B,
    OTA_SECURITY_AREA_MAX,
}ota_security_area_e;


#define BOOT_FLAG_MAGIC_WORD "\x63\x54\x72\x4C"
#define FLASH_DEFAULT_VALUE_WITH_16_0xFF "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
#define OTA_BOOTLAODER_OPERATE_BOOT_FLAG_LEN    (32)
#define OPERATE_PARTITION_LEN    (16)
#define ALLOCATED_VERSION_LEN    (32)
#define OTA_XIP_FLAG_OFFSET      (4096)
/**
 * @brief get the download client id
 *
 * get the download client id
 *
 * @attention
 *  - This API is used to get the download client id.
 * @param
 * - client_id
 * 
 * @return 
 * - BK_OK: succeed
 * - others: other errors.
 */
uint32 bk_ota_download_client_get();
/**
 * @brief set the download client id
 *
 * set the download client id
 *
 * @attention
 *  - This API is used to set the download client id.
 * @param
 * - client_id
 * 
 * @return 
 * - BK_OK: succeed
 * - others: other errors.
 */
int bk_ota_download_client_set(uint32 client_id);
/**
 * @brief firmware area switch
 *
 * excute firmware area switch
 *
 * @attention
 *  - This API is used to excute firmware area switch.
 * @param
 * - 0x1: area A
 * - 0X2: area B.
 * @return 
 * - BK_OK: succeed
 * - others: other errors.
 */
int bk_ota_security_excute_fw_switch(uint32_t fw_idx);
/**
 * @brief get current firmware idx
 *
 * get current firmware idx
 *
 * @attention
 *  - This API is used to get current firmware idx.
 * @param
 * void
 * @return 
 * - 0x1: area A
 * - 0X2: area B.
 */
uint32_t bk_ota_security_get_curr_fw_idx();
/**
 * @brief write the xip status
 *
 * write the xip status.
 *
 * @attention
 *  - This API is used to write the xip status.
 * @param
 * -area id:0x0:AREA_A;0X1:AREA_B
 * -type id:0x1:XIP_MAGIC_TYPE; 0x2:XIP_COPY_DONE_TYPE; 0x3:XIP_IMAGE_OK_TYPE
 * -status:the value needed to write
 * @return 
 * - BK_OK: succeed
 * - others: other errors.
 */
int bk_ota_security_write_xip_status(ota_security_area_e area_id, uint32_t type, uint32_t status);
/**
 * @brief get the xip status
 *
 * get the xip status.
 *
 * @attention
 *  - This API is used to get the xip status.
 * @param
 * -area id:0x0:AREA_A;0X1:AREA_B
 * -type id:0x1:XIP_MAGIC_TYPE; 0x2:XIP_COPY_DONE_TYPE; 0x3:XIP_IMAGE_OK_TYPE
 * @return 
 * - 1 : xip status
 */
uint32_t bk_ota_security_read_xip_status(ota_security_area_e area_id, uint32_t type);
/**
 * @brief get boot flag
 *
 * get the current bootloader exec partition.
 *
 * @attention
 *  - This API is used to get boot flag
 *  - if current bootloader is 1,represent bootloader is execting on the part A.
 *  - if current bootloader is 2,represent bootloader is execting on the part B.
 * @param
 * -void
 * @return 
 * - 1 : represent bootloader is execting on the part A
 * - 2 : represent bootloader is execting on the part B.
 */
ota_bootloader_boot_flag_e ota_bootloader_get_boot_flag_value(void);

/**
 * @brief set boot flag
 *
 * After finish upgrading bootloader,it need set boot flag
 *
 * @attention
 *  - This API is used to set boot flag
 *  - if current bootloader is A,finish upgrading bootloader,boot flag will set bootloader B.
 * @param
 * -void
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 */
int ota_bootloader_set_boot_flag_value(void);

/**
 * @brief get curr bootloader firmware index
 *
 * get curr bootloader firmware index.
 *
 * @attention
 *  - This API is used to get curr bootloader firmware index
 *  - if current bootloader is 1,represent bootloader is execting on the part A.
 *  - if current bootloader is 2,represent bootloader is execting on the part B.
 * @param
 * -void
 * @return 
 * - 1 : represent bootloader is execting on the part A
 * - 2 : represent bootloader is execting on the part B.
 */
ota_bootloader_boot_flag_e bk_ota_security_get_curr_bl2_fw_idx(void);

/**
 * @brief swap bootloader firmware area
 *
 * excute swap bootloader firmware area
 *
 * @attention
 *  - This API is used to swap bootloader firmware area
 * @param
 * -target_fw_idx:  swap to the area
 * -target_fw_idx = 0x1: swap to area A
 * -target_fw_idx = 0X2: swap to area B
 * @return
 * - BK_OK=0: succeed
 * - others: other errors.
 */
int bk_ota_security_swap_bl2_fw(uint32 target_fw_idx);

/**
 * @brief get bootloader firmware version number
 *
 * excute get bootloader firmware version number
 *
 * @attention
 *  - This API is used to get bootloader firmware version number
 * @param
 * -target_fw_idx:  get the target bootloader version (A/B)
 * -target_fw_idx = 0x1: bootloader part_A
 * -target_fw_idx = 0X2: bootloader part_B
 * @return
 * - ver: such as clip_bl2_v1.1.1
 * - NULL: is fail.
 */
void* bk_ota_security_get_current_bl2_ver(uint32 target_fw_idx);

