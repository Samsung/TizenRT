/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

/// @file mbedtls/alt/common.h
// @brief common ALT header

#ifndef MBEDTLS_COMMON_ALT_H
#define MBEDTLS_COMMON_ALT_H

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define MBEDTLS_MAX_ENCRYPTED_KEY_SIZE_ALT  (1280)
#define MBEDTLS_MAX_ECP_KEY_SIZE_ALT  (68)

#define DHM_1024 (128)
#define DHM_2048 (256)
#define DHM_4096 (512)

// ======================================
// Structure
// ======================================
//! @struct sECC_KEY
//! @brief struct of ECDSA Key parameter
struct sECC_KEY {
	unsigned int curve;
	unsigned char *privatekey;
	unsigned char *publickey_x;
	unsigned char x_byte_len;
	unsigned char *publickey_y;
	unsigned char y_byte_len;
};

//! @struct sECC_SIGN
//! @brief struct of ECDSA Signature
struct sECC_SIGN {
	unsigned int sign_type;
	unsigned char *r;
	unsigned char r_byte_len;
	unsigned char *s;
	unsigned char s_byte_len;
};

//! @struct sRSA_KEY
//! @brief struct of RSA Key parameter
struct sRSA_KEY {
	unsigned int object_id;
	unsigned char *modules;
	unsigned int modules_byte_len;
	unsigned char *publickey;
	unsigned int publickey_byte_len;
	unsigned char *privatekey;
};

//! @struct sRSA_SIGN
//! @brief struct of RSA Signature
struct sRSA_SIGN {
	unsigned int signature_byte_len;
	unsigned char *signature;
	unsigned int salt_byte_len;
	unsigned int alg_type;
};

//! @struct sAES_PARAM
//! @brief struct of AES parameter
struct sAES_PARAM {
	unsigned char *pu8Plaintext;
	unsigned int u32Plaintext_byte_len;
	unsigned char *pu8Ciphertext;
	unsigned int u32Ciphertext_byte_len;

	unsigned char *pu8iv;
	unsigned int u32iv_byte_len;

	unsigned char *pu8aad;
	unsigned int u32aad_byte_len;

	unsigned char *pu8Tag;
	unsigned int u32Tag_byte_len;

	unsigned int u32Mode;
};

//! @struct sHMAC_MSG
//! @brief struct of message for HMAC
struct sHMAC_MSG {
	unsigned int addr_low;
	unsigned int addr_high;
	unsigned int descriptor_byte_len;
	unsigned int msg_byte_len;
	unsigned int msg_type;
};

//! @struct sHASH_MSG
//! @brief struct of message for Hash
struct sHASH_MSG {
	unsigned int addr_low;
	unsigned int addr_high;
	unsigned int descriptor_byte_len;
	unsigned int msg_byte_len;
	unsigned int msg_type;
};

//! @struct sDH_PARAM
//! @brief struct of DH parameter
struct sDH_PARAM {
	unsigned int object_id;
	unsigned int modules_p_byte_len;
	unsigned char *modules_p;
	unsigned int generator_g_byte_len;
	unsigned char *generator_g;
	unsigned int order_q_byte_len;
	unsigned char *order_q;
	unsigned int publickey_byte_len;
	unsigned char *publickey;
};


#endif /* common.h */
