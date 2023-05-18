/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * include/crc16.h
 *
 *   Copyright (C) 2013 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/**
 * @addtogroup CRC_KERNEL
 * @{
 */
///@file crc16.h
///@brief Crc16 APIs

#ifndef __INCLUDE_CRC16_H
#define __INCLUDE_CRC16_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sys/types.h>
#include <stdint.h>

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/**
 * The result value of crc16part and crc16 function does not match any CRC method.
 * Please be careful if use these functions. 
 * 
 *      uint16_t crc16part(FAR const uint8_t *src, size_t len, uint16_t crc16val);
 *      uint16_t crc16(FAR const uint8_t *src, size_t len);
*/

/**
 * @brief  Continue CRC calculation on a part of the buffer.
 *
 * @details @b #include <crc16.h>
 * @param[in] src source number for crc16
 * @param[in] len length for calculation
 * @param[in] crc16val value for calculation
 * @return On success, calculated 16-bit CRC is returned.
 * @since TizenRT v1.0
 */

uint16_t crc16part(FAR const uint8_t *src, size_t len, uint16_t crc16val);

/**
 * @brief  Return a 16-bit CRC of the contents of the 'src' buffer, length 'len'
 *
 * @details @b #include <crc16.h>
 * @param[in] src source number for crc16
 * @param[in] len length for calculation
 * @return On success, calculated 16-bit CRC is returned.
 * @since TizenRT v1.0
 */

uint16_t crc16(FAR const uint8_t *src, size_t len);

/**
 * @brief  Continue CRC16-CCITT calculation on a part of the buffer.
 *
 * @details @b #include <crc16_ccitt.h>
 * @param[in] src source number for crc16_ccitt
 * @param[in] len length for calculation
 * @param[in] crc16val value for calculation
 * @return On success, calculated 16-bit CRC-CCITT is returned. (calculated by polynomial 0x1021)
 * @since TizenRT v3.1
 */

uint16_t crc16_ccitt_part(FAR const uint8_t *src, size_t len, uint16_t crc16val);

/**
 * @brief  Continue CRC16-CCITT calculation on a part of the buffer.
 *
 * @details @b #include <crc16_ccitt.h>
 * @param[in] src source number for crc16_ccitt
 * @param[in] len length for calculation
 * @param[in] crc16val value for calculation
 * @return On success, calculated 16-bit CRC-CCITT is returned. (calculated by polynomial 0x8408)
 * @since TizenRT v3.1
 */

uint16_t crc16_ccitt_rev_part(FAR const uint8_t *src, size_t len, uint16_t crc16val);

/**
 * @brief  Return a 16-bit CRC16-CCITT/Xmodem of the contents of the 'src' buffer, length 'len'
 *
 * @details @b #include <crc16_ccitt.h>
 * @param[in] src source number for crc16_ccitt_xmodem
 * @param[in] len length for calculation
 * @return On success, calculated 16-bit CRC (Xmodem) is returned.
 * @since TizenRT v3.1
 */

uint16_t crc16_ccitt_xmodem(FAR const uint8_t *src, size_t len);

/**
 * @brief  Return a 16-bit CRC16-CCITT/False of the contents of the 'src' buffer, length 'len'
 *
 * @details @b #include <crc16_ccitt.h>
 * @param[in] src source number for crc16_ccitt_false
 * @param[in] len length for calculation
 * @return On success, calculated 16-bit CRC (False) is returned.
 * @since TizenRT v3.1
 */

uint16_t crc16_ccitt_false(FAR const uint8_t *src, size_t len);

/**
 * @brief  Return a 16-bit CRC16-CCITT/Aug of the contents of the 'src' buffer, length 'len'
 *
 * @details @b #include <crc16_ccitt.h>
 * @param[in] src source number for crc16_ccitt_aug
 * @param[in] len length for calculation
 * @return On success, calculated 16-bit CRC (Aug) is returned.
 * @since TizenRT v3.1
 */

uint16_t crc16_ccitt_aug(FAR const uint8_t *src, size_t len);

/**
 * @brief  Return a 16-bit CRC16-CCITT/Kermit of the contents of the 'src' buffer, length 'len'
 *
 * @details @b #include <crc16_ccitt.h>
 * @param[in] src source number for crc16_ccitt_kermit
 * @param[in] len length for calculation
 * @return On success, calculated 16-bit CRC (Kermit) is returned.
 * @since TizenRT v3.1
 */

uint16_t crc16_ccitt_kermit(FAR const uint8_t *src, size_t len);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __INCLUDE_CRC16_H */

/**
 * @}
 */
