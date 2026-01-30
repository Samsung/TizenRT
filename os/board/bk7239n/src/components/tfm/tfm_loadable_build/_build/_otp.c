// Copyright 2022-2024 Beken
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

const otp_item_t otp_map_1[15] = {
    {OTP_MEMORY_CHECK_MARK,                   60,    0x0,    OTP_READ_ONLY,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_EFUSE,                                4,    0x3c,    OTP_READ_ONLY,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_AES_KEY,                             32,    0x40,    OTP_READ_ONLY,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_MODEL_ID,                             4,    0x100,    OTP_READ_ONLY,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_MODEL_KEY,                           16,    0x104,    OTP_READ_ONLY,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_ARM_DEVICE_ID,                        4,    0x114,    OTP_READ_ONLY,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_DEVICE_ROOT_KEY,                     16,    0x118,    OTP_READ_ONLY,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_BL1_BOOT_PUBLIC_KEY_HASH,            32,    0x128,    OTP_READ_ONLY,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_BL2_BOOT_PUBLIC_KEY_HASH,            32,    0x148,    OTP_READ_ONLY,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_ARM_LCS,                              4,    0x168,    OTP_READ_ONLY,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_LOCK_CONTROL,                         4,    0x17c,    OTP_READ_ONLY,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_BL1_SECURITY_COUNTER,                 4,    0x188,    OTP_READ_ONLY,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_SECURE_DEBUG_PK,                     32,    0x1cc,    OTP_READ_ONLY,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_BL2_SECURITY_COUNTER,                64,    0x228,    OTP_READ_ONLY,    OTP_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_HUK,                                 32,    0x268,    OTP_READ_ONLY,    OTP_SECURITY,    OTP_NO_NEED_CRC},
};

uint32_t otp_map_1_row(void)
{
    return sizeof(otp_map_1) / sizeof(otp_map_1[0]);
}

uint32_t otp_map_1_col(void)
{
    return sizeof(otp_map_1) / sizeof(otp_map_1[0]);
}

const otp_item_t otp_map_2[18] = {
    {OTP_PHY_PWR1,                       32,    0x0,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_PHY_PWR2,                       32,    0x20,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_RFCALI1,                       256,    0x40,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_RFCALI2,                       256,    0x140,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_RFCALI3,                       256,    0x240,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_RFCALI4,                       256,    0x340,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_MAC_ADDRESS1,                    8,    0x440,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_MAC_ADDRESS2,                    8,    0x448,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_MAC_ADDRESS3,                    8,    0x450,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_MAC_ADDRESS4,                    8,    0x458,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_FACTORY_ID,                      4,    0x588,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NEED_CRC},
    {OTP_PRODUCT_ID,                     16,    0x58c,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NEED_CRC},
    {OTP_GADC_CALIBRATION,               72,    0x59c,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NEED_CRC},
    {OTP_GADC_TEMPERATURE,                8,    0x5e4,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NEED_CRC},
    {OTP_VDDDIG_BANDGAP,                  4,    0x5ec,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NEED_CRC},
    {OTP_DIA,                             4,    0x5f0,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NEED_CRC},
    {OTP_MEMORY_CHECK_VDDDIG,             4,    0x5f4,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NO_NEED_CRC},
    {OTP_DEVICE_ID,                       8,    0x5f8,    OTP_READ_WRITE,    OTP_NON_SECURITY,    OTP_NEED_CRC},
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
