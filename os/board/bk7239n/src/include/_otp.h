// Copyright 2022-2025 Beken
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

//This is a generated file, don't modify it!

#pragma once



#include <stdint.h>
#define BK_ERR_OTP_READ_BUFFER_NULL   (BK_ERR_OTP_BASE - 1) /**< The pointed buff is NULL */
#define BK_ERR_OTP_ADDR_OUT_OF_RANGE  (BK_ERR_OTP_BASE - 2) /**< OTP address is out of range */
#define BK_ERR_NO_READ_PERMISSION     (BK_ERR_OTP_BASE - 3) /**< Not have read privilege  */
#define BK_ERR_NO_WRITE_PERMISSION    (BK_ERR_OTP_BASE - 4) /**< Not have write privilege */
#define BK_ERR_OTP_PERMISSION_WRONG   (BK_ERR_OTP_BASE - 5) /**< OTP permission value error*/
#define BK_ERR_OTP_NOT_EMPTY          (BK_ERR_OTP_BASE - 6) /**< try to write area already has value*/
#define BK_ERR_OTP_UPDATE_NOT_EQUAL   (BK_ERR_OTP_BASE - 7) /**< after update not equal to expected*/
#define BK_ERR_OTP_OPERATION_ERROR    (BK_ERR_OTP_BASE - 8) /**< otp operation error*/
#define BK_ERR_OTP_OPERATION_WARNING  (BK_ERR_OTP_BASE - 9) /**< otp operation warning*/
#define BK_ERR_OTP_OPERATION_WRONG    (BK_ERR_OTP_BASE - 10) /**< otp operation wrong*/
#define BK_ERR_OTP_OPERATION_FORBID   (BK_ERR_OTP_BASE - 11) /**< otp operation forbid*/
#define BK_ERR_OTP_OPERATION_FAIL     (BK_ERR_OTP_BASE - 12) /** otp operation other error*/
#define BK_ERR_OTP_INIT_FAIL          (BK_ERR_OTP_BASE - 13) /** otp init fail*/
#define BK_ERR_OTP_INDEX_WRONG        (BK_ERR_OTP_BASE - 14) /**< OTP item index error*/
#define BK_ERR_OTP_CRC_WRONG          (BK_ERR_OTP_BASE - 15) /**< OTP item index error*/

typedef enum{
    OTP_READ_WRITE = 0,
    OTP_READ_ONLY = 0x3,
    OTP_NO_ACCESS = 0xF,
} otp_privilege_t;

typedef enum{
    OTP_SECURITY = 0,
    OTP_NON_SECURITY,
} otp_security_t;

typedef enum{
    OTP_NEED_CRC = 0,
    OTP_NO_NEED_CRC,
} otp_crc_t;

typedef struct
{
    uint32_t  name;
    uint16_t  allocated_size;
    uint16_t  offset;
    otp_privilege_t privilege;
    otp_security_t  security;
    otp_crc_t  crc_en;
} otp_item_t;

typedef enum{
    OTP_MEMORY_CHECK_MARK,
    OTP_EFUSE,
    OTP_AES_KEY,
    OTP_MODEL_ID,
    OTP_MODEL_KEY,
    OTP_ARM_DEVICE_ID,
    OTP_DEVICE_ROOT_KEY,
    OTP_BL1_BOOT_PUBLIC_KEY_HASH,
    OTP_BL2_BOOT_PUBLIC_KEY_HASH,
    OTP_ARM_LCS,
    OTP_LOCK_CONTROL,
    OTP_BL1_SECURITY_COUNTER,
    OTP_BL2_SECURITY_COUNTER,
    OTP_HUK,
    OTP_IAK,
    OTP_IAK_LEN,
    OTP_IAK_TYPE,
    OTP_IAK_ID,
    OTP_BOOT_SEED,
    OTP_LCS,
    OTP_IMPLEMENTATION_ID,
    OTP_HW_VERSION,
    OTP_VERIFICATION_SERVICE_URL,
    OTP_PROFILE_DEFINITION,
    OTP_ENTROPY_SEED,
    OTP_SECURE_DEBUG_PK,
    OTP_MAC_ADDRESS,
    OTP_VDDDIG_BANDGAP,
    OTP_DIA,
    OTP_GADC_CALIBRATION,
    OTP_SDMADC_CALIBRATION,
    OTP_DEVICE_ID,
    OTP_MEMORY_CHECK_VDDDIG,
    OTP_GADC_TEMPERATURE,
    OTP_GADC_CALIBRATION_EXT,
    OTP1_MAX_ID,
} otp1_id_t;

extern const otp_item_t otp_map_1[35];

uint32_t otp_map_1_row(void);

uint32_t otp_map_1_col(void);

typedef enum{
    OTP_PHY_PWR1,
    OTP_PHY_PWR2,
    OTP_RFCALI1,
    OTP_RFCALI2,
    OTP_RFCALI3,
    OTP_RFCALI4,
    OTP_MAC_ADDRESS1,
    OTP_MAC_ADDRESS2,
    OTP_MAC_ADDRESS3,
    OTP_MAC_ADDRESS4,
    OTP_EK_2,
    OTP_EK_3,
    OTP_EK_1,
    OTP_EK_11,
    OTP_EK_12,
    OTP_EK_13,
    OTP_EK_14,
    OTP_EK_15,
    OTP_EK_1X,
    OTP2_MAX_ID,
} otp2_id_t;

extern const otp_item_t otp_map_2[19];

uint32_t otp_map_2_row(void);

uint32_t otp_map_2_col(void);

extern const otp_item_t *otp_map;
