/** mbed Microcontroller Library
  ******************************************************************************
  * @file    efuse_api.h
  * @author
  * @version V1.0.0
  * @brief   This file provides mbed EFUSE API.
  ******************************************************************************
  * @attention
  *
  * Copyright(c) 2006 - 2022 Realtek Corporation. All rights reserved.
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  * Licensed under the Apache License, Version 2.0 (the License); you may
  * not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  * http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an AS IS BASIS, WITHOUT
  * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#ifndef MBED_EXT_EFUSE_API_EXT_H
#define MBED_EXT_EFUSE_API_EXT_H

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup efuse EFUSE
 *  @ingroup    hal
 *  @brief      efuse functions
 *  @{
 */

///@name Ameba Common
///@{

/**
  * @brief  Get remaining efuse length
  * @retval : remaining efuse length
  */
int efuse_get_remaining_length(void);

/**
  * @brief  Read efuse content of specified user logical OTP.
  * @param  data: buffer address to save the readback data.
  * @retval none
  * @note Read user logical OTP(0x170~0x19F).
  */
void efuse_mtp_read(uint8_t *data);

/**
  * @brief  Write user's content to user logical OTP.
  * @param  offset: Address offset of the programmed user logical OTP(base address: 0x170).
  * @param  data: Data buffer to be programmed.
  * @param  len: Data length to be programmed.
  *          - 0~32: Success
  *          - -1: Failure
  * @note Write user logical OTP(0x170~0x19F).
  */
int efuse_mtp_write(uint32_t offset, uint32_t len, uint8_t *data);


/**
  * @brief  Read OTP User-defined Physical content.
  * @param  Offset: Offset of the physical OTP (base address: 0x380)
  * @param  len: Length of readback data.
  * @param  buf: Address to save the readback data.
  * @return Read result. It can be one of the following values:
  * @retval 0: Success.
  * @retval -1: Error.
  * @note Read user OTP(0x380~0x4FF)
          0x380~0x3BF:UDF1 Secure Optional
          0x3C0~0x3FF:UDF2 Secure Optional
          0x400~0x4FF:Non-Secure.
  */
int efuse_otp_read(uint32_t offset, uint32_t len, uint8_t *buf);

/**
  * @brief  Write user's content to OTP User-defined Physical zone.
  * @param  offset: Offset of the programmed physical OTP(base address: 0x380).
  * @param  len: Data length of programmed data.
  * @param  buf: Data to be programmed.
  * @return Write result. It can be one of the following values:
  * @retval 0: Success.
  * @retval -1: Error.
  * @note Read user OTP(0x380~0x4FF)
          0x380~0x3BF:UDF1 Secure Optional
          0x3C0~0x3FF:UDF2 Secure Optional
          0x400~0x4FF:Non-Secure
  */
int efuse_otp_write(uint32_t offset, uint32_t len, uint8_t *buf);

/**
  * @brief  ckeck user's contant to OTP efuse
  * @param  buf: Specified the data to be programmed.
  * @param  len: Specifies the data length of programmed data.
  * @retval 0: Success
  * @retval -1: Failure
  */
int efuse_otp_chk(u8 len, u8 *buf);

/**
  * @brief  Disable jtag
  * @retval 0: Success
  */
int efuse_disable_jtag(void);


///@}

#if defined(CONFIG_PLATFORM_8195A) && (CONFIG_PLATFORM_8195A == 1)
///@name Ameba1 Only
///@{
/**
  * @brief  Write key1 to efuse
  * @param  address: Specifies the offset of the programmed efuse.
  * @param  len: Specifies the data length of programmed data.
  * @param  buf: Specified the data to be programmed.
  * @retval 0: Success
  * @retval -1: Failure
  */
int efuse_key1_write(u8 address, u8 len, u8 *buf);

/**
  * @brief  Write key2 to efuse
  * @param  address: Specifies the offset of the programmed efuse.
  * @param  len: Specifies the data length of programmed data.
  * @param  buf: Specified the data to be programmed.
  * @retval 0: Success
  * @retval -1: Failure
  */
int efuse_key2_write(u8 address, u8 len, u8 *buf);
///@}
#endif //CONFIG_PLATFORM_8195A

#if defined(CONFIG_PLATFORM_8711B) && (CONFIG_PLATFORM_8711B == 1)
///@name AmebaZ Only
///@{
/**
  * @brief  Set RDP Enable.
  * @param  none
  * @note  can not change or read back after write.
  */
void efuse_rdp_enable(void);

/**
  * @brief  Set 16B RDP key into EFUSE.
  * @param  rdp_key: 16B EFUSE KEY
  * @note  can not change or read back after write.
  */
void efuse_rdp_keyset(u8 *rdp_key);

/**
  * @brief  Set 16B OTF key into EFUSE.
  * @param  OtfKey: 16B EFUSE KEY
  * @note  can not change or read back after write.
  */

void efuse_otf_keyset(u8 *otf_key);
///@}
#endif //CONFIG_PLATFORM_8711B

#if defined(CONFIG_PLATFORM_8195BLP) && (CONFIG_PLATFORM_8195BLP == 1)
///@name AmebaPro Only
///@{
/**
  * @brief  Disable LP jtag
  * @retval 0: Success
  */
int efuse_disable_lp_jtag(void);

/**
  * @brief  Disable HS secure jtag
  * @retval 0: Success
  */
int efuse_disable_sec_jtag(void);

/**
  * @brief  Disable HS nonsecure jtag
  * @retval 0: Success
  */
int efuse_disable_nonsec_jtag(void);
///@}
#endif  // end of "#if defined(CONFIG_PLATFORM_8195BLP)"

#if defined(CONFIG_PLATFORM_8195BHP) && (CONFIG_PLATFORM_8195BHP == 1)
///@name AmebaPro Only
///@{
/**
  * @brief  Write secure key to efuse
  * @param  buf: Specified the 32-byte security key to be programmed.
  * @retval 0: Success
  * @retval -1: Failure
  */
int efuse_sec_key_write(u8 *buf);

/**
  * @brief  Write super secure key to efuse
  * @param  buf: Specified the 32-byte super security key to be programmed.
  * @retval 0: Success
  * @retval -1: Failure
  */
int efuse_susec_key_write(u8 *buf);
///@}
#endif  // end of "#if defined(CONFIG_PLATFORM_8195BHP)"

#if (defined(CONFIG_PLATFORM_8710C) && (CONFIG_PLATFORM_8710C == 1))
///@name AmebaZII Only
///@{
/**
  * @brief  Disable secure jtag
  * @retval 0: Success
  */
int efuse_disable_sec_jtag(void);

/**
  * @brief  Disable nonsecure jtag
  * @retval 0: Success
  */
int efuse_disable_nonsec_jtag(void);

/**
  * @brief  Write secure key to efuse
  * @param  buf: Specified the 32-byte security key to be programmed.
  * @param  key_num: select key number.
  * @retval 0: Success
  * @retval -1: Failure
  */
int efuse_sec_key_write(u8 *buf, u8 key_num);

/**
  * @brief  Write super secure key to efuse
  * @param  buf: Specified the 32-byte super security key to be programmed.
  * @retval 0: Success
  * @retval -1: Failure
  */
int efuse_susec_key_write(u8 *buf);

/**
  * @brief  Write secure j-tag key to efuse
  * @param  buf: Specified the 32-byte security key to be programmed.
  * @retval 0: Success
  * @retval -1: Failure
  */
int efuse_s_jtag_key_write(u8 *buf);

/**
  * @brief  Write non-secure j-tag key to efuse
  * @param  buf: Specified the 32-byte security key to be programmed.
  * @retval 0: Success
  * @retval -1: Failure
  */
int efuse_ns_jtag_key_write(u8 *buf);

/**
  * @brief  lock super secure key
  * @retval 0: Success
  * @retval -1: Failure
  */
int efuse_lock_susec_key(void);
///@}
#endif  // end of "#if defined(CONFIG_PLATFORM_8710C)"

#if (defined(CONFIG_PLATFORM_8735B) && (CONFIG_PLATFORM_8735B == 1))
/**
  * @brief  Get crytpo key from efuse
  * @param  pkey:
  * @param  key_num:
  * @retval 0: Success
  * @retval -1: Failure
  */
int efuse_crypto_key_get(uint8_t *pkey, uint8_t key_num);

/**
  * @brief  Get crytpo key from efuse
  * @param  pkey:
  * @param  key_num:
  * @param  w_lock_en:
  * @retval 0: Success
  * @retval -1: Failure
  */
int efuse_crypto_key_write(uint8_t *pkey, uint8_t key_num, uint8_t w_lock_en);
///@}
#endif  // end of "#if (defined(CONFIG_PLATFORM_8735B) && (CONFIG_PLATFORM_8735B == 1))"

/*\@}*/

#ifdef __cplusplus
}
#endif

#endif // MBED_EXT_EFUSE_API_EXT_H
