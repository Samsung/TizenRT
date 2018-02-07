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
 * include/crc8.h
 *
 *   Copyright (C) 2014 Ken Pettit. All rights reserved.
 *   Author: Ken Pettit <pettitkd@gmail.com>
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
 * @defgroup CRC_KERNEL CRC
 * @brief Provides APIs for Cyclic Redundancy Check
 * @ingroup KERNEL
 *
 * @{
 */

/// @file crc8.h
/// @brief Crc8 APIs

#ifndef __INCLUDE_CRC8_H
#define __INCLUDE_CRC8_H

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
 * @brief  Continue CRC calculation on a part of the buffer.
 *
 * @details @b #include <crc8.h>
 * @param[in] src source number for crc8
 * @param[in] len length for calculation
 * @param[in] crc8val value for calculation
 * @return On success, calculated 8-bit CRC is returned.
 * @since TizenRT v1.0
 */
uint8_t crc8part(FAR const uint8_t *src, size_t len, uint8_t crc8val);

/**
 * @brief  Return an 8-bit CRC of the contents of the 'src' buffer, length 'len'
 *   using the polynomial x^8+x^6+x^3+x^2+1 (Koopman, et al. "0xA6" poly).
 *
 * @details @b #include <crc8.h>
 * @param[in] src source number for crc8
 * @param[in] len length for calculation
 * @return On success, calculated 8-bit CRC is returned.
 * @since TizenRT v1.0
 */

uint8_t crc8(FAR const uint8_t *src, size_t len);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __INCLUDE_CRC8_H */

/**
 * @}
 */
