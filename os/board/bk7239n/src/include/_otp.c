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



#include "_otp.h"

#include <stddef.h>

const otp_item_t otp_map_1[35] = {
    {OTP_MEMORY_CHECK_MARK,                   60,    0x0,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_EFUSE,                                4,    0x3c,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_AES_KEY,                             32,    0x40,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_MODEL_ID,                             4,    0x100,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_MODEL_KEY,                           16,    0x104,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_ARM_DEVICE_ID,                        4,    0x114,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_DEVICE_ROOT_KEY,                     16,    0x118,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_BL1_BOOT_PUBLIC_KEY_HASH,            32,    0x128,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_BL2_BOOT_PUBLIC_KEY_HASH,            32,    0x148,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_ARM_LCS,                              4,    0x168,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_LOCK_CONTROL,                         4,    0x16c,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_BL1_SECURITY_COUNTER,                 4,    0x188,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_BL2_SECURITY_COUNTER,                64,    0x200,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_HUK,                                 32,    0x240,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_IAK,                                 32,    0x260,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_IAK_LEN,                              4,    0x280,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_IAK_TYPE,                             4,    0x284,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_IAK_ID,                              32,    0x288,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_BOOT_SEED,                           32,    0x2a8,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_LCS,                                  4,    0x2c8,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_IMPLEMENTATION_ID,                   32,    0x2cc,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_HW_VERSION,                          32,    0x2ec,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_VERIFICATION_SERVICE_URL,            32,    0x30c,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_PROFILE_DEFINITION,                  32,    0x32c,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_ENTROPY_SEED,                        64,    0x34c,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_SECURE_DEBUG_PK,                      2,    0x38c,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_MAC_ADDRESS,                          8,    0x3ac,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_VDDDIG_BANDGAP,                       2,    0x3b4,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_DIA,                                  2,    0x3b6,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_GADC_CALIBRATION,                     4,    0x3b8,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_SDMADC_CALIBRATION,                   4,    0x3bc,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_DEVICE_ID,                            8,    0x3c0,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_MEMORY_CHECK_VDDDIG,                  4,    0x3c8,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_GADC_TEMPERATURE,                     2,    0x3cc,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_GADC_CALIBRATION_EXT,                 4,    0x3d0,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
};

uint32_t otp_map_1_row(void)
{
    return sizeof(otp_map_1) / sizeof(otp_map_1[0]);
}

uint32_t otp_map_1_col(void)
{
    return sizeof(otp_map_1) / sizeof(otp_map_1[0]);
}

const otp_item_t otp_map_2[19] = {
    {OTP_PHY_PWR1,                32,    0x0,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_PHY_PWR2,                32,    0x20,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_RFCALI1,                256,    0x40,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_RFCALI2,                256,    0x140,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_RFCALI3,                256,    0x240,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_RFCALI4,                256,    0x340,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_MAC_ADDRESS1,             8,    0x440,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_MAC_ADDRESS2,             8,    0x448,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_MAC_ADDRESS3,             8,    0x450,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_MAC_ADDRESS4,             8,    0x458,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_EK_2,                    16,    0x800,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_EK_3,                    16,    0x810,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_EK_1,                    32,    0x820,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_EK_11,                   32,    0x840,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_EK_12,                   32,    0x860,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_EK_13,                   32,    0x880,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_EK_14,                   32,    0x8a0,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_EK_15,                   32,    0x8c0,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_EK_1X,                   32,    0x8e0,    OTP_READ_WRITE,    OTP_SECURITY,    OTP_NO_NEED_CRC},
};

uint32_t otp_map_2_row(void)
{
    return sizeof(otp_map_2) / sizeof(otp_map_2[0]);
}

uint32_t otp_map_2_col(void)
{
    return sizeof(otp_map_2) / sizeof(otp_map_2[0]);
}

const otp_item_t *otp_map = NULL;
