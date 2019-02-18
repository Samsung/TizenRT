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

#define MBEDTLS_MAX_ECP_KEY_SIZE_ALT                  (68)
#define MBEDTLS_MAX_BUF_SIZE_ALT                      (4096)

#define DHM_1024 (128)
#define DHM_2048 (256)
#define DHM_4096 (512)

// Secure Storage
#define SECURE_STORAGE_TYPE_FACTORYKEY_KEY			(0x00)
#define SECURE_STORAGE_TYPE_FACTORYKEY_DATA			(0x01)
#define SECURE_STORAGE_TYPE_KEY_AES				(0x0A)
#define SECURE_STORAGE_TYPE_KEY_HMAC			(0x0B)
#define SECURE_STORAGE_TYPE_KEY_RSA				(0x0C)
#define SECURE_STORAGE_TYPE_KEY_DH				(0x0D)
#define SECURE_STORAGE_TYPE_KEY_ECC				(0x0E)
#define SECURE_STORAGE_TYPE_DATA				(0x10)
#define SECURE_STORAGE_TYPE_CERT				(0x11)

/* Key type */
#define AES_KEY		0x010000
#define HMAC_KEY	0x020000
#define RSA_KEY		0x030000
#define ECC_KEY		0x040000

#define AES_KEY_128       ((AES_KEY)  | (0x1))
#define AES_KEY_256       ((AES_KEY)  | (0x2))
#define HMAC_KEY_128      ((HMAC_KEY) | (0x1))
#define HMAC_KEY_256      ((HMAC_KEY) | (0x2))
#define RSA_KEY_1024      ((RSA_KEY)  | (0xB1))
#define RSA_KEY_2048      ((RSA_KEY)  | (0xB2))

#define ECC_KEY_BP256     ((ECC_KEY) | (0x53))

#define ECC_KEY_NIST192   ((ECC_KEY) | (0x21))
#define ECC_KEY_NIST224   ((ECC_KEY) | (0x22))
#define ECC_KEY_NIST256   ((ECC_KEY) | (0x23))
#define ECC_KEY_NIST384   ((ECC_KEY) | (0x24))
#define ECC_KEY_NIST512   ((ECC_KEY) | (0x25))

/* cert type */
#define CERT_DER      0x01
#define CERT_PEM      0x02

// ======================================
// Structure
// ======================================
//! @struct mbedtls_sECC_KEY
//! @brief struct of ECDSA Key parameter
struct mbedtls_sECC_KEY {
	unsigned int curve;
	unsigned char *privatekey;
	unsigned char *publickey_x;
	unsigned char x_byte_len;
	unsigned char *publickey_y;
	unsigned char y_byte_len;
};

//! @struct mbedtls_sECC_SIGN
//! @brief struct of ECDSA Signature
struct mbedtls_sECC_SIGN {
	unsigned int sign_type;
	unsigned char *r;
	unsigned char r_byte_len;
	unsigned char *s;
	unsigned char s_byte_len;
};

//! @struct mbedtls_sRSA_KEY
//! @brief struct of RSA Key parameter
struct mbedtls_sRSA_KEY {
	unsigned int object_id;
	unsigned char *modules;
	unsigned int modules_byte_len;
	unsigned char *publickey;
	unsigned int publickey_byte_len;
	unsigned char *privatekey;
};

//! @struct mbedtls_sRSA_SIGN
//! @brief struct of RSA Signature
struct mbedtls_sRSA_SIGN {
	unsigned int signature_byte_len;
	unsigned char *signature;
	unsigned int salt_byte_len;
	unsigned int alg_type;
};

//! @struct mbedtls_sAES_PARAM
//! @brief struct of AES parameter
struct mbedtls_sAES_PARAM {
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

//! @struct mbedtls_sHMAC_MSG
//! @brief struct of message for HMAC
struct mbedtls_sHMAC_MSG {
	unsigned int addr_low;
	unsigned int addr_high;
	unsigned int descriptor_byte_len;
	unsigned int msg_byte_len;
	unsigned int msg_type;
};

//! @struct mbedtls_sHASH_MSG
//! @brief struct of message for Hash
struct mbedtls_sHASH_MSG {
	unsigned int addr_low;
	unsigned int addr_high;
	unsigned int descriptor_byte_len;
	unsigned int msg_byte_len;
	unsigned int msg_type;
};

//! @struct mbedtls_sDH_PARAM
//! @brief struct of DH parameter
struct mbedtls_sDH_PARAM {
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
