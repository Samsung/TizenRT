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
/*
    Copyright (c)  2015 Ineda Systems Pvt Ltd.

   This file is provided under strict non-disclosure agreements and remains
   the property of Ineda Systems Pvt Ltd. Any use of this file or the
   information it contains to develop products commercial or otherwise must
   be with the permission of Ineda Systems Pvt Ltd.

   IT IS A VIOLATION OF ANY AND ALL LICENSE AGREEMENTS TO REMOVE THIS HEADER
   FROM ANY AND ALL SOURCE CODE PROVIDED BY INEDA SYSTEMS PVT LTD.
*/

/**
\addtogroup driver_api
* @{
*/

/*!
@file ace_api.h
@brief This file includes the API's used to access dhanush crypto engine for data encryption/decryption and data integraty.
@author : Ineda
@version :
*/

/**
 * \defgroup ace_api CryptoEngine

* @{
* @brief DhanushCryptoEngine is supporting following crypto algorithms
*           Block ciphers:
*              AES with 128,192 & 256 and modes ECB,OFB,CFB1,CFB1,CBC(MAC & CTR) and XTS(128 & 256) modes.
*           RC4 - wih keysize of 8bits to 2048bits
*           Hash algorithms:
*                 SHA1,SHA2,SHA2-224,SHA2-256,SHA2-384,SHA2-512 and MD5
*
*/

#ifndef __DHANUSH_ACE_API_H__
#define __DHANUSH_ACE_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#define SUCCESS 0
/*! enum for all dhanush support algorithms and used by APIs. */
typedef enum _crypto_alg {
	DHANUSH_ACE_DES_ECB = 0x0,	/*!< DES ECB mode. */
	DHANUSH_ACE_DES_CBC = 0x1,	/*!< DES CBC mode. */
	DHANUSH_ACE_DES_CFB = 0x2,	/*!< DES CFB mode. */
	DHANUSH_ACE_DES_OFB = 0x3,	/*!< DES OFB mode. */
	DHANUSH_ACE_TDES_ECB = 0x4,	/*!< TDES ECB mode. */
	DHANUSH_ACE_TDES_CBC = 0x5,	/*!< TDES CBC mode. */
	DHANUSH_ACE_TDES_CFB = 0x6,	/*!< TDES CFB mode. */
	DHANUSH_ACE_TDES_OFB = 0x7,	/*!< TDES OFB mode. */
	DHANUSH_ACE_AES_128_ECB = 0x8,	/*!< AES-128 ECB mode. */
	DHANUSH_ACE_AES_128_CBC = 0x9,	/*!< AES-128 CBC mode. */
	DHANUSH_ACE_AES_128_CFB = 0xa,	/*!< AES-128 CFB mode. */
	DHANUSH_ACE_AES_128_OFB = 0xb,	/*!< AES-128 OFB mode. */
	DHANUSH_ACE_SHA1 = 0xc,	/*!< SHA1 mode. */
	DHANUSH_ACE_MD5 = 0x10,	/*!< MD5 mode. */
	DHANUSH_ACE_SHA2_224 = 0x14,	/*!< SHA2-224 mode. */
	DHANUSH_ACE_SHA2_256 = 0x15,	/*!< SHA2-256 mode. */
	DHANUSH_ACE_SHA2_384 = 0x16,	/*!< SHA2-384 mode. */
	DHANUSH_ACE_SHA2_512 = 0x17,	/*!< SHA2-512 mode. */
	DHANUSH_ACE_RC4 = 0x18,	/*!< RC4 mode. */
	DHANUSH_ACE_DES_CFB1 = 0x24,	/*!< DES CFB1 mode. */
	DHANUSH_ACE_DES_CFB8 = 0x25,	/*!< DES CFB8 mode. */
	DHANUSH_ACE_TDES_CFB1 = 0x26,	/*!< TDES CFB1 mode. */
	DHANUSH_ACE_TDES_CFB8 = 0x27,	/*!< TDES CFB8 mode. */
	DHANUSH_ACE_AES_192_ECB = 0x28,	/*!< AES-192 ECB mode. */
	DHANUSH_ACE_AES_192_CBC = 0x29,	/*!< AES-192 EBC mode. */
	DHANUSH_ACE_AES_192_CFB = 0x2a,	/*!< AES-192 CFB mode. */
	DHANUSH_ACE_AES_192_OFB = 0x2b,	/*!< AES-192 OFB mode. */
	DHANUSH_ACE_AES_256_ECB = 0x2c,	/*!< AES-256 ECB mode. */
	DHANUSH_ACE_AES_256_CBC = 0x2d,	/*!< AES-256 CBC mode. */
	DHANUSH_ACE_AES_256_CFB = 0x2e,	/*!< AES-256 CFB mode. */
	DHANUSH_ACE_AES_256_OFB = 0x2f,	/*!< AES-256 OFB mode. */
	DHANUSH_ACE_AES_CFB1_128 = 0x30,	/*!< AES-128 CFB1 mode. */
	DHANUSH_ACE_AES_CFB1_192 = 0x31,	/*!< AES-192 CFB1 mode. */
	DHANUSH_ACE_AES_CFB1_256 = 0x32,	/*!< AES-256 CFB1 mode. */
	DHANUSH_ACE_AES_CFB8_128 = 0x34,	/*!< AES-128 CFB8 mode. */
	DHANUSH_ACE_AES_CFB8_192 = 0x35,	/*!< AES-192 CFB1 mode. */
	DHANUSH_ACE_AES_CFB8_256 = 0x36,	/*!< AES-256 CFB1 mode. */
	ACE_EO_HW_MACROS = 0x37,	/*!< Internal macro don't use this. */

	DHANUSH_ACE_AES_128_CFB_CTR = (ACE_EO_HW_MACROS + DHANUSH_ACE_AES_128_CFB),	/*!< AES-128 CFB CTR mode. */
	DHANUSH_ACE_AES_192_CFB_CTR = (ACE_EO_HW_MACROS + DHANUSH_ACE_AES_192_CFB),	/*!< AES-192 CFB CTR mode. */
	DHANUSH_ACE_AES_256_CFB_CTR = (ACE_EO_HW_MACROS + DHANUSH_ACE_AES_256_CFB),	/*!< AES-256 CFB CTR mode. */
	DHANUSH_ACE_AES_128_ECB_XTS = (ACE_EO_HW_MACROS + DHANUSH_ACE_AES_128_ECB),	/*!< AES-128 ECB XTS mode :not supported in this release. */
	DHANUSH_ACE_AES_192_ECB_XTS = (ACE_EO_HW_MACROS + DHANUSH_ACE_AES_192_ECB),	/*!< AES-192 ECB XTS mode :not supported in this release. */
	DHANUSH_ACE_AES_256_ECB_XTS = (ACE_EO_HW_MACROS + DHANUSH_ACE_AES_256_ECB),	/*!< AES-256 ECB XTS mode :not supported in this release. */
	DHANUSH_ACE_AES_128_CBC_MAC = (ACE_EO_HW_MACROS + DHANUSH_ACE_AES_128_CBC),	/*!< AES-128 CBC MAC mode :not supported in this release */
	DHANUSH_ACE_AES_192_CBC_MAC = (ACE_EO_HW_MACROS + DHANUSH_ACE_AES_192_CBC),	/*!< AES-192 CBC MAC mode :not supported in this release */
	DHANUSH_ACE_AES_256_CBC_MAC = (ACE_EO_HW_MACROS + DHANUSH_ACE_AES_256_CBC)	/*!< AES-256 CBC MAC mode :not supported in this release */
} crypto_algo_t;

/*!
@brief Dhanush Crypto API for ciphers.
@param [in]  int8_t *inbuf          - Input buffer and aligned to blcok size of the alogrithm,
@param [out]  int8_t *outbuf        - Output buffer
@param [in]  int8_t *key            - Key
@param [in]  int8_t *iv              - Initail vector
@param [in]  int8_t keysize         - Key size based on alogrithms
@param [in]  uint32_t datasize        - Data size
@param [in]  uint32_t enc_dec        - Encrypt-0 or Decrypt-1
@param [in]  uint32_t alg_type        - Cipher algorithm type
@return int32_t
<pre>
SUCCESS             - For Successfull encryption and decryption
</pre>
*/

int32_t Crypto_ACE_CipherEncryptDecrypt(int8_t *inbuf, int8_t *outbuf, int8_t *key, int8_t *iv, int8_t keysize, uint32_t datasize, uint32_t enc_dec, uint32_t alg_type);
/*!
@brief Dhanush Crypto API for Hash functions.
@param [in]  int8_t *inbuf          - Input buffer and aligned to blcok size of the alogrithm,
@param [out]  int8_t *outbuf        - Output hash
@param [in]  int8_t *iv              - Initail vector
@param [in]  uint32_t datasize        - Data size
@param [in]  uint32_t alg_type        - Hash algorithm type
@return int32_t
<pre>
SUCCESS             - For Successfull encryption and decryption
</pre>
*/
int32_t Crypto_ACE_Hash_Function(int8_t *inbuf, int8_t *outbuf, int8_t *iv, uint32_t datasize, uint32_t alg_type);

#ifdef __cplusplus
}
#endif
#endif							/* __DHANUSH_ACE_API_H__ */
/**@} *//**@} */
