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

#include <tinyara/config.h>

#include <stdio.h>
#include <string.h>
#include <security/security_api.h>
#include "tc_common.h"
#include "utc_security.h"

#define ITER_COUNT		10

static security_rsa_mode g_rsa_mode_table[] = {
	RSASSA_PKCS1_V1_5,
	RSASSA_PKCS1_PSS_MGF1,
};

static security_hash_mode g_hash_mode_table[] = {
	HASH_MD5,
	HASH_SHA1,
	HASH_SHA224,
	HASH_SHA256,
	HASH_SHA384,
	HASH_SHA512,
};

static security_aes_mode g_aes_mode_table[] = {
	AES_ECB_NOPAD,
	AES_ECB_ISO9797_M1,
	AES_ECB_ISO9797_M2,
	AES_ECB_PKCS5,
	AES_ECB_PKCS7,
	AES_CBC_NOPAD,
	AES_CBC_ISO9797_M1,
	AES_CBC_ISO9797_M2,
	AES_CBC_PKCS5,
	AES_CBC_PKCS7,
	AES_CTR,
};

static security_handle g_hnd = NULL;

/**
 * @testcase         utc_crypto_aes_encryption_input_iv_p
 * @brief            encrypt AES with user input IV
 * @scenario         encrypt AES with user input IV
 * @apicovered       crypto_aes_encryption
 * @precondition     AES key should be set in Secure Storage
 * @postcondition    none
 */
static void utc_crypto_aes_encryption_input_iv_p(void)
{
	unsigned char iv[] = { 0x1a, 0x2b, 0x3c, 0x4d, 0x5e, 0x6f, 0x70, 0x81, 
							0x92, 0xa3, 0xb4, 0xc5, 0xd6, 0xe7, 0xf8, 0x00 };

	unsigned char plain_text[] = { 0x4d, 0x79, 0x20, 0x42, 0x79, 0x74, 0x65, 0x20, 
									0x50, 0x72, 0x69, 0x6e, 0x74, 0x00, 0x00, 0x00 };

	security_data plain = {plain_text, sizeof(plain_text)};
	security_data enc = {NULL, 0};

	for (int aes_mode = 0; aes_mode < sizeof(g_aes_mode_table) / sizeof(security_aes_mode); aes_mode++) {
		/* Check whether hang occurs during repeated encrypt function */
		for (int iter = 0; iter < ITER_COUNT; iter++) {
			security_aes_mode mode = g_aes_mode_table[aes_mode];
			security_aes_param param = {mode, iv, sizeof(iv)};

			security_error res = crypto_aes_encryption(g_hnd, &param, UTC_CRYPTO_KEY_NAME,
													&plain, &enc);
			TC_ASSERT_EQ("crypto_aes_encryption_p", res, SECURITY_OK);
			TC_SUCCESS_RESULT();
		}
	}
}

/**
 * @testcase         utc_crypto_aes_encryption_iv_null_p
 * @brief            encrypt AES with IV in secure storage (If user input IV is null, it uses pre-set IV in secure storage)
 * @scenario         encrypt AES without user input IV
 * @apicovered       crypto_aes_encryption
 * @precondition     AES key and IV should be set in Secure Storage
 * @postcondition    none
 */
static void utc_crypto_aes_encryption_iv_null_p(void)
{
	unsigned char *iv = NULL; 
	unsigned int iv_len = 0;

	unsigned char plain_text[] = { 0x4d, 0x79, 0x20, 0x42, 0x79, 0x74, 0x65, 0x20, 
									0x50, 0x72, 0x69, 0x6e, 0x74, 0x00, 0x00, 0x00 };

	security_data plain = {plain_text, sizeof(plain_text)};
	security_data enc = {NULL, 0};

	for (int aes_mode = 0; aes_mode < sizeof(g_aes_mode_table) / sizeof(security_aes_mode); aes_mode++) {	
		/* Check whether hang occurs during repeated encrypt function */
		for (int iter = 0; iter < ITER_COUNT; iter++) {
			security_aes_mode mode = g_aes_mode_table[aes_mode];
			security_aes_param param = {mode, iv, iv_len};

			security_error res = crypto_aes_encryption(g_hnd, &param, UTC_CRYPTO_KEY_NAME,
													&plain, &enc);
			TC_ASSERT_EQ("utc_crypto_aes_encryption_iv_null_p", res, SECURITY_OK);
			TC_SUCCESS_RESULT();
		}
	}
}

/**
 * @testcase         utc_crypto_aes_encryption_hnd_n
 * @brief            encrypt AES
 * @scenario         encrypt AES
 * @apicovered       crypto_aes_encryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_aes_encryption_hnd_n(void)
{
	unsigned char iv[] = "temp_iv_value";
	unsigned int iv_len = strlen((const char*)iv) + 1;
	security_aes_param param = {AES_ECB_NOPAD, iv, iv_len};

	unsigned char plain_text[] = "plain text";
	unsigned int plain_text_len = strlen((const char*)plain_text) + 1;

	security_data plain = {plain_text, plain_text_len};
	security_data enc = {NULL, 0};

	security_error res = crypto_aes_encryption(NULL, &param, UTC_CRYPTO_KEY_NAME,
											   &plain, &enc);

	TC_ASSERT_EQ("crypto_aes_encryption_hnd", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_crypto_aes_encryption_param_n
 * @brief            encrypt AES
 * @scenario         encrypt AES
 * @apicovered       crypto_aes_encryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_aes_encryption_param_n(void)
{
	unsigned char plain_text[] = "plain text";
	unsigned int plain_text_len = strlen((const char*)plain_text) + 1;
	security_data plain = {plain_text, plain_text_len};
	security_data enc = {NULL, 0};

	unsigned char iv[] = "temp_iv_value";
	unsigned int iv_len = strlen((const char*)iv) + 1;
	security_aes_param param = {AES_UNKNOWN, iv, iv_len};
	security_error res = crypto_aes_encryption(g_hnd, &param, UTC_CRYPTO_KEY_NAME,
											   &plain, &enc);
	TC_ASSERT_EQ("crypto_aes_encryption_param_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_crypto_aes_encryption_key_n
 * @brief            encrypt AES
 * @scenario         encrypt AES
 * @apicovered       crypto_aes_encryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_aes_encryption_key_n(void)
{
	unsigned char plain_text[] = "plain text";
	unsigned int plain_text_len = strlen((const char*)plain_text) + 1;

	security_data plain = {plain_text, plain_text_len};
	security_data enc = {NULL, 0};

	unsigned char iv[] = "temp_iv_value";
	unsigned int iv_len = strlen((const char*)iv) + 1;
	security_aes_param param = {AES_ECB_NOPAD, iv, iv_len};

	security_error res = crypto_aes_encryption(g_hnd, &param, NULL, &plain, &enc);

	TC_ASSERT_EQ("crypto_aes_encryption_key_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_crypto_aes_encryption_input_n
 * @brief            encrypt AES
 * @scenario         encrypt AES
 * @apicovered       crypto_aes_encryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_aes_encryption_input_n(void)
{
	security_data enc = {NULL, 0};

	unsigned char iv[] = "temp_iv_value";
	unsigned int iv_len = strlen((const char*)iv) + 1;
	security_aes_param param = {AES_ECB_NOPAD, iv, iv_len};

	security_error res = crypto_aes_encryption(g_hnd, &param, UTC_CRYPTO_KEY_NAME,
											   NULL, &enc);

	TC_ASSERT_EQ("crypto_aes_encryption_input_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_crypto_aes_encryption_output_n
 * @brief            encrypt AES
 * @scenario         encrypt AES
 * @apicovered       crypto_aes_encryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_aes_encryption_output_n(void)
{
	security_data enc = {NULL, 0};

	unsigned char iv[] = "temp_iv_value";
	unsigned int iv_len = strlen((const char*)iv) + 1;
	security_aes_param param = {AES_ECB_NOPAD, iv, iv_len};

	security_error res = crypto_aes_encryption(g_hnd, &param,
											   UTC_CRYPTO_KEY_NAME, &enc, NULL);

	TC_ASSERT_EQ("crypto_aes_encryption_output_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_crypto_aes_decryption_input_iv_p
 * @brief            decrypt AES with IV user input IV
 * @scenario         decrypt AES with IV user input IV
 * @apicovered       crypto_aes_decryption
 * @precondition     AES key should be set in Secure Storage
 * @postcondition    none
 */
static void utc_crypto_aes_decryption_input_iv_p(void)
{
	unsigned char iv[] = { 0x1a, 0x2b, 0x3c, 0x4d, 0x5e, 0x6f, 0x70, 0x81, 
							0x92, 0xa3, 0xb4, 0xc5, 0xd6, 0xe7, 0xf8, 0x00 };

	unsigned char enc_text[] = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 
									0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a };

	security_data enc = {enc_text, sizeof(enc_text)};
	security_data dec = {NULL, 0};

	for (int aes_mode = 0; aes_mode < sizeof(g_aes_mode_table) / sizeof(security_aes_mode); aes_mode++) {
		/* Check whether hang occurs during repeated decrypt function */
		for (int iter = 0; iter < ITER_COUNT; iter++) {
			security_aes_mode mode = g_aes_mode_table[aes_mode];
			security_aes_param param = {mode, iv, sizeof(iv)};

			security_error res = crypto_aes_decryption(g_hnd, &param, UTC_CRYPTO_KEY_NAME,
												   &enc, &dec);
			TC_ASSERT_EQ("crypto_aes_decryption_p", res, SECURITY_OK);
			TC_SUCCESS_RESULT();
		}
	}
}

/**
 * @testcase         utc_crypto_aes_decryption_iv_null_p
 * @brief            decrypt AES with IV in secure storage (If user input IV is null, it uses pre-set IV in secure storage)
 * @scenario         decrypt AES without user input IV 
 * @apicovered       crypto_aes_decryption
 * @precondition     AES key and IV should be set in Secure Storage
 * @postcondition    none
 */
static void utc_crypto_aes_decryption_iv_null_p(void)
{
	unsigned char *iv = NULL;
	unsigned int iv_len = 0;

	unsigned char enc_text[] = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 
									0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a };

	security_data enc = { enc_text, sizeof(enc_text) };
	security_data dec = { NULL, 0 };

	for (int aes_mode = 0; aes_mode < sizeof(g_aes_mode_table) / sizeof(security_aes_mode); aes_mode++) {
		/* Check whether hang occurs during repeated decrypt function */
		for (int iter = 0; iter < ITER_COUNT; iter++) {
			security_aes_mode mode = g_aes_mode_table[aes_mode];
			security_aes_param param = {mode, iv, iv_len};

			security_error res = crypto_aes_decryption(g_hnd, &param, UTC_CRYPTO_KEY_NAME,
												   &enc, &dec);
			TC_ASSERT_EQ("utc_crypto_aes_encryption_iv_null_p", res, SECURITY_OK);
			TC_SUCCESS_RESULT();
		}
	}
}

/**
 * @testcase         utc_crypto_aes_decryption_hnd_n
 * @brief            decrypt AES
 * @scenario         decrypt AES
 * @apicovered       crypto_aes_decryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_aes_decryption_hnd_n(void)
{
	unsigned char iv[] = "temp_iv_value";
	unsigned int iv_len = strlen((const char*)iv) + 1;
	security_aes_param param = {AES_ECB_NOPAD, iv, iv_len};

	unsigned char enc_text[] = "encrypted data";
	unsigned int enc_text_len = strlen((const char*)enc_text) + 1;

	security_data plain = {enc_text, enc_text_len};
	security_data enc = {NULL, 0};

	security_error res = crypto_aes_decryption(NULL, &param, UTC_CRYPTO_KEY_NAME,
											   &plain, &enc);

	TC_ASSERT_EQ("crypto_aes_decryption_hnd", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_crypto_aes_decryption_param_n
 * @brief            decrypt AES
 * @scenario         decrypt AES
 * @apicovered       crypto_aes_decryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_aes_decryption_param_n(void)
{
	unsigned char plain_text[] = "plain text";
	unsigned int plain_text_len = strlen((const char*)plain_text) + 1;
	security_data plain = {plain_text, plain_text_len};
	security_data enc = {NULL, 0};

	unsigned char iv[] = "temp_iv_value";
	unsigned int iv_len = strlen((const char*)iv) + 1;
	security_aes_param param = {AES_UNKNOWN, iv, iv_len};

	security_error res = crypto_aes_decryption(g_hnd, &param, UTC_CRYPTO_KEY_NAME,
											   &plain, &enc);
	TC_ASSERT_EQ("crypto_aes_decryption_param_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_crypto_aes_decryption_key_n
 * @brief            decrypt AES
 * @scenario         decrypt AES
 * @apicovered       crypto_aes_decryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_aes_decryption_key_n(void)
{
	unsigned char enc_text[] = "encrypted data";
	unsigned int enc_text_len = strlen((const char*)enc_text) + 1;

	security_data plain = {enc_text, enc_text_len};
	security_data enc = {NULL, 0};

	unsigned char iv[] = "temp_iv_value";
	unsigned int iv_len = strlen((const char*)iv) + 1;
	security_aes_param param = {AES_ECB_NOPAD, iv, iv_len};

	security_error res = crypto_aes_decryption(g_hnd, &param, NULL, &plain, &enc);

	TC_ASSERT_EQ("crypto_aes_decryption_key_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_crypto_aes_decryption_input_n
 * @brief            decrypt AES
 * @scenario         decrypt AES
 * @apicovered       crypto_aes_decryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_aes_decryption_input_n(void)
{
	security_data enc = {NULL, 0};

	unsigned char iv[] = "temp_iv_value";
	unsigned int iv_len = strlen((const char*)iv) + 1;
	security_aes_param param = {AES_ECB_NOPAD, iv, iv_len};

	security_error res = crypto_aes_decryption(g_hnd, &param, UTC_CRYPTO_KEY_NAME,
											   NULL, &enc);

	TC_ASSERT_EQ("crypto_aes_decryption_input_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_crypto_aes_decryption_output_n
 * @brief            decrypt AES
 * @scenario         decrypt AES
 * @apicovered       crypto_aes_decryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_aes_decryption_output_n(void)
{
	unsigned char enc_text[] = "encrypted data";
	unsigned int enc_text_len = strlen((const char*)enc_text) + 1;
	security_data plain = {enc_text, enc_text_len};

	unsigned char iv[] = "temp_iv_value";
	unsigned int iv_len = strlen((const char*)iv) + 1;
	security_aes_param param = {AES_ECB_NOPAD, iv, iv_len};

	security_error res = crypto_aes_decryption(g_hnd, &param, UTC_CRYPTO_KEY_NAME,
											   &plain, NULL);

	TC_ASSERT_EQ("crypto_aes_decryption_output_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_crypto_rsa_encryption_p
 * @brief            encrypt RSA
 * @scenario         encrypt RSA
 * @apicovered       crypto_rsa_encryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_rsa_encryption_p(void)
{
	unsigned char plain_text[] = "plain text";
	unsigned int plain_text_len = strlen((const char*)plain_text) + 1;

	security_data plain = {plain_text, plain_text_len};
	security_data enc = {NULL, 0};

	int i = 0, j = 0, k = 0;
	for (; i < sizeof(g_rsa_mode_table)/sizeof(security_rsa_mode); i++) {
		for (; j < sizeof(g_hash_mode_table)/sizeof(security_hash_mode); j++) {
			for (; k < sizeof(g_hash_mode_table)/sizeof(security_hash_mode); k++) {
				security_rsa_param param = {g_rsa_mode_table[i], g_hash_mode_table[j],
										   g_hash_mode_table[k], 0};
				security_error res = crypto_rsa_encryption(g_hnd, &param, UTC_CRYPTO_KEY_NAME,
														   &plain, &enc);

				TC_ASSERT_EQ("crypto_rsa_encryption_p", res, SECURITY_OK);
				TC_SUCCESS_RESULT();
			}
		}
	}
}

/**
 * @testcase         utc_crypto_rsa_encryption_hnd_n
 * @brief            encrypt RSA
 * @scenario         encrypt RSA
 * @apicovered       crypto_rsa_encryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_rsa_encryption_hnd_n(void)
{
	unsigned char plain_text[] = "plain text";
	unsigned int plain_text_len = strlen((const char*)plain_text) + 1;

	security_data plain = {plain_text, plain_text_len};
	security_data enc = {NULL, 0};
	security_rsa_param param = {RSASSA_PKCS1_V1_5, HASH_MD5, HASH_MD5, 0};

	security_error res = crypto_rsa_encryption(NULL, &param, UTC_CRYPTO_KEY_NAME, &plain, &enc);

	TC_ASSERT_EQ("crypto_rsa_encryption_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_crypto_rsa_encryption_param_n
 * @brief            encrypt RSA
 * @scenario         encrypt RSA
 * @apicovered       crypto_rsa_encryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_rsa_encryption_param_n(void)
{
	unsigned char plain_text[] = "plain text";
	unsigned int plain_text_len = strlen((const char*)plain_text) + 1;

	security_data plain = {plain_text, plain_text_len};
	security_data enc = {NULL, 0};
	security_rsa_param param = {RSASSA_UNKNOWN, HASH_MD5, HASH_MD5, 0};
	security_error res = crypto_rsa_encryption(g_hnd, &param, UTC_CRYPTO_KEY_NAME, &plain, &enc);

	TC_ASSERT_EQ("crypto_rsa_encryption_param", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_crypto_rsa_encryption_param2_n
 * @brief            encrypt RSA
 * @scenario         encrypt RSA
 * @apicovered       crypto_rsa_encryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_rsa_encryption_param2_n(void)
{
	unsigned char plain_text[] = "plain text";
	unsigned int plain_text_len = strlen((const char*)plain_text) + 1;

	security_data plain = {plain_text, plain_text_len};
	security_data enc = {NULL, 0};
	security_rsa_param param = {RSASSA_PKCS1_V1_5, HASH_UNKNOWN, HASH_MD5, 0};
	security_error res = crypto_rsa_encryption(g_hnd, &param, UTC_CRYPTO_KEY_NAME, &plain, &enc);

	TC_ASSERT_EQ("crypto_rsa_encryption_param2", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_crypto_rsa_encryption_param3_n
 * @brief            encrypt RSA
 * @scenario         encrypt RSA
 * @apicovered       crypto_rsa_encryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_rsa_encryption_param3_n(void)
{
	unsigned char plain_text[] = "plain text";
	unsigned int plain_text_len = strlen((const char*)plain_text) + 1;

	security_data plain = {plain_text, plain_text_len};
	security_data enc = {NULL, 0};
	security_rsa_param param = {RSASSA_PKCS1_V1_5, HASH_MD5, HASH_UNKNOWN, 0};
	security_error res = crypto_rsa_encryption(g_hnd, &param, UTC_CRYPTO_KEY_NAME, &plain, &enc);

	TC_ASSERT_EQ("crypto_rsa_encryption_param3", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_crypto_rsa_encryption_input_n
 * @brief            encrypt RSA
 * @scenario         encrypt RSA
 * @apicovered       crypto_rsa_encryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_rsa_encryption_input_n(void)
{
	security_data enc = {NULL, 0};
	security_rsa_param param = {RSASSA_PKCS1_V1_5, HASH_MD5, HASH_MD5, 0};
	security_error res = crypto_rsa_encryption(g_hnd, &param, UTC_CRYPTO_KEY_NAME, NULL, &enc);

	TC_ASSERT_EQ("crypto_rsa_encryption_input", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_crypto_rsa_encryption_output_n
 * @brief            encrypt RSA
 * @scenario         encrypt RSA
 * @apicovered       crypto_rsa_encryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_rsa_encryption_output_n(void)
{
	unsigned char plain_text[] = "plain text";
	unsigned int plain_text_len = strlen((const char*)plain_text) + 1;

	security_data plain = {plain_text, plain_text_len};
	security_rsa_param param = {RSASSA_PKCS1_V1_5, HASH_UNKNOWN, HASH_MD5, 0};
	security_error res = crypto_rsa_encryption(g_hnd, &param, UTC_CRYPTO_KEY_NAME, &plain, NULL);

	TC_ASSERT_EQ("crypto_rsa_encryption_output", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_crypto_rsa_decryption_p
 * @brief            decrypt RSA
 * @scenario         decrypt RSA
 * @apicovered       crypto_rsa_decryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_rsa_decryption_p(void)
{
	unsigned char enc_text[] = "RSA encrypted message";
	unsigned int enc_text_len = strlen((const char*)enc_text) + 1;

	security_data enc = {enc_text, enc_text_len};
	security_data dec = {NULL, 0};

	int i = 0, j = 0, k = 0;
	for (; i < sizeof(g_rsa_mode_table)/sizeof(security_rsa_mode); i++) {
		for (; j < sizeof(g_hash_mode_table)/sizeof(security_hash_mode); j++) {
			for (; k < sizeof(g_hash_mode_table)/sizeof(security_hash_mode); k++) {
				security_rsa_param param = {g_rsa_mode_table[i], g_hash_mode_table[j],
										   g_hash_mode_table[k], 0};
				security_error res = crypto_rsa_decryption(g_hnd, &param, UTC_CRYPTO_KEY_NAME,
														   &enc, &dec);

				TC_ASSERT_EQ("crypto_rsa_decryption_p", res, SECURITY_OK);
				TC_SUCCESS_RESULT();
			}
		}
	}
}

/**
 * @testcase         utc_crypto_rsa_decryption_hnd_n
 * @brief            decrypt RSA
 * @scenario         decrypt RSA
 * @apicovered       crypto_rsa_decryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_rsa_decryption_hnd_n(void)
{
	unsigned char enc_text[] = "RSA encrypted message";
	unsigned int enc_text_len = strlen((const char*)enc_text) + 1;

	security_data enc = {enc_text, enc_text_len};
	security_data dec = {NULL, 0};
	security_rsa_param param = {RSASSA_PKCS1_V1_5, HASH_MD5, HASH_MD5, 0};

	security_error res = crypto_rsa_decryption(NULL, &param, UTC_CRYPTO_KEY_NAME, &enc, &dec);

	TC_ASSERT_EQ("crypto_rsa_decryption_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_crypto_rsa_decryption_param_n
 * @brief            decrypt RSA
 * @scenario         decrypt RSA
 * @apicovered       crypto_rsa_decryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_rsa_decryption_param_n(void)
{
	unsigned char enc_text[] = "RSA encrypted message";
	unsigned int enc_text_len = strlen((const char*)enc_text) + 1;

	security_data enc = {enc_text, enc_text_len};
	security_data dec = {NULL, 0};
	security_rsa_param param = {RSASSA_UNKNOWN, HASH_MD5, HASH_MD5, 0};
	security_error res = crypto_rsa_decryption(g_hnd, &param, UTC_CRYPTO_KEY_NAME, &enc, &dec);

	TC_ASSERT_EQ("crypto_rsa_decryption_param", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_crypto_rsa_decryption_param2_n
 * @brief            decrypt RSA
 * @scenario         decrypt RSA
 * @apicovered       crypto_rsa_decryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_rsa_decryption_param2_n(void)
{
	unsigned char enc_text[] = "RSA encrypted message";
	unsigned int enc_text_len = strlen((const char*)enc_text) + 1;

	security_data enc = {enc_text, enc_text_len};
	security_data dec = {NULL, 0};
	security_rsa_param param = {RSASSA_PKCS1_V1_5, HASH_UNKNOWN, HASH_MD5, 0};
	security_error res = crypto_rsa_decryption(g_hnd, &param, UTC_CRYPTO_KEY_NAME, &enc, &dec);

	TC_ASSERT_EQ("crypto_rsa_decryption_param2", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_crypto_rsa_decryption_param3_n
 * @brief            decrypt RSA
 * @scenario         decrypt RSA
 * @apicovered       crypto_rsa_decryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_rsa_decryption_param3_n(void)
{
	unsigned char enc_text[] = "RSA encrypted message";
	unsigned int enc_text_len = strlen((const char*)enc_text) + 1;

	security_data enc = {enc_text, enc_text_len};
	security_data dec = {NULL, 0};
	security_rsa_param param = {RSASSA_PKCS1_V1_5, HASH_MD5, HASH_UNKNOWN, 0};
	security_error res = crypto_rsa_decryption(g_hnd, &param, UTC_CRYPTO_KEY_NAME, &enc, &dec);

	TC_ASSERT_EQ("crypto_rsa_decryption_param3", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_crypto_rsa_decryption_input_n
 * @brief            decrypt RSA
 * @scenario         decrypt RSA
 * @apicovered       crypto_rsa_decryption
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_rsa_decryption_input_n(void)
{
	security_data dec = {NULL, 0};
	security_rsa_param param = {RSASSA_PKCS1_V1_5, HASH_MD5, HASH_MD5, 0};
	security_error res = crypto_rsa_decryption(g_hnd, &param, UTC_CRYPTO_KEY_NAME, NULL, &dec);

	TC_ASSERT_EQ("crypto_rsa_decryption_input", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_crypto_rsa_decryption_output_n
 * @brief            decrypt RSA
 * @scenario         decrypt RSA
 * @apicovered       crypto_rsa_decryption_output
 * @precondition     none
 * @postcondition    none
 */
static void utc_crypto_rsa_decryption_output_n(void)
{
	unsigned char enc_text[] = "RSA encrypted message";
	unsigned int enc_text_len = strlen((const char*)enc_text) + 1;

	security_data enc = {enc_text, enc_text_len};
	security_rsa_param param = {RSASSA_PKCS1_V1_5, HASH_UNKNOWN, HASH_MD5, 0};
	security_error res = crypto_rsa_decryption(g_hnd, &param, UTC_CRYPTO_KEY_NAME, &enc, NULL);

	TC_ASSERT_EQ("crypto_rsa_decryption_output", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}


void utc_crypto_main(void)
{
	security_error res = security_init(&g_hnd);
	if (res < 0) {
		US_ERROR;
	}

	/*  AES */
	utc_crypto_aes_encryption_input_iv_p();
	utc_crypto_aes_encryption_iv_null_p();
	utc_crypto_aes_encryption_hnd_n();
	utc_crypto_aes_encryption_param_n();
	utc_crypto_aes_encryption_key_n();
	utc_crypto_aes_encryption_input_n();
	utc_crypto_aes_encryption_output_n();
	utc_crypto_aes_decryption_input_iv_p();
	utc_crypto_aes_decryption_iv_null_p();
	utc_crypto_aes_decryption_hnd_n();
	utc_crypto_aes_decryption_param_n();
	utc_crypto_aes_decryption_key_n();
	utc_crypto_aes_decryption_input_n();
	utc_crypto_aes_decryption_output_n();
	/*  RSA */
	utc_crypto_rsa_encryption_p();
	utc_crypto_rsa_encryption_hnd_n();
	utc_crypto_rsa_encryption_param_n();
	utc_crypto_rsa_encryption_param2_n();
	utc_crypto_rsa_encryption_param3_n();
	utc_crypto_rsa_encryption_input_n();
	utc_crypto_rsa_encryption_output_n();
	utc_crypto_rsa_decryption_p();
	utc_crypto_rsa_decryption_hnd_n();
	utc_crypto_rsa_decryption_param_n();
	utc_crypto_rsa_decryption_param2_n();
	utc_crypto_rsa_decryption_param3_n();
	utc_crypto_rsa_decryption_input_n();
	utc_crypto_rsa_decryption_output_n();

	res = security_deinit(g_hnd);
	if (res != SECURITY_OK) {
		US_ERROR;
	}
}
