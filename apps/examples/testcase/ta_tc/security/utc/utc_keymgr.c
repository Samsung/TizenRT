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

static security_key_type g_key_type_table[] = {
	KEY_AES_128,
	KEY_AES_192,
	KEY_AES_256,
	KEY_RSA_1024,
	KEY_RSA_2048,
	KEY_RSA_3072,
	KEY_RSA_4096,
	KEY_ECC_BRAINPOOL_P256R1,
	KEY_ECC_BRAINPOOL_P384R1,
	KEY_ECC_BRAINPOOL_P512R1,
	KEY_ECC_SEC_P192R1,
	KEY_ECC_SEC_P224R1,
	KEY_ECC_SEC_P256R1,
	KEY_ECC_SEC_P384R1,
	KEY_ECC_SEC_P512R1,
	KEY_HMAC_MD5,
	KEY_HMAC_SHA1,
	KEY_HMAC_SHA224,
	KEY_HMAC_SHA256,
	KEY_HMAC_SHA384,
	KEY_HMAC_SHA512,
};

static security_key_type g_sym_key_type_table[] = {
	KEY_AES_128,
	KEY_AES_192,
	KEY_AES_256,
	KEY_HMAC_MD5,
	KEY_HMAC_SHA1,
	KEY_HMAC_SHA224,
	KEY_HMAC_SHA256,
	KEY_HMAC_SHA384,
	KEY_HMAC_SHA512,
};

static security_key_type g_asym_key_type_table[] = {
	KEY_RSA_1024,
	KEY_RSA_2048,
	KEY_RSA_3072,
	KEY_RSA_4096,
	KEY_ECC_BRAINPOOL_P256R1,
	KEY_ECC_BRAINPOOL_P384R1,
	KEY_ECC_BRAINPOOL_P512R1,
	KEY_ECC_SEC_P192R1,
	KEY_ECC_SEC_P224R1,
	KEY_ECC_SEC_P256R1,
	KEY_ECC_SEC_P384R1,
	KEY_ECC_SEC_P512R1,
};

static security_handle g_hnd = NULL;

/**
 * @testcase         utc_keymgr_generate_key_p
 * @brief            key generation
 * @scenario         key generation
 * @apicovered       keymgr_generate_key
 * @precondition     none
 * @postcondition    none
 */
static void utc_keymgr_generate_key_p(void)
{
	int i = 0;
	for (; i < sizeof(g_key_type_table)/sizeof(security_key_type); i++) {
		security_error res = keymgr_generate_key(g_hnd, g_key_type_table[i], UTC_CRYPTO_KEY_NAME);
		TC_ASSERT_EQ("keymgr_generate_key_p", res, SECURITY_OK);
		TC_SUCCESS_RESULT();
	}
}

/**
 * @testcase         utc_keymgr_generate_key_hnd_n
 * @brief            key generation
 * @scenario         key generation
 * @apicovered       keymgr_generate_key
 * @precondition     none
 * @postcondition    none
 */
static void utc_keymgr_generate_key_hnd_n(void)
{
	security_error res = keymgr_generate_key(NULL, KEY_AES_128, UTC_CRYPTO_KEY_NAME);

	TC_ASSERT_EQ("keymgr_generate_key_", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_keymgr_generate_key_algo_n
 * @brief            key generation
 * @scenario         key generation
 * @apicovered       keymgr_generate_key
 * @precondition     none
 * @postcondition    none
 */
static void utc_keymgr_generate_key_algo_n(void)
{
	security_error res = keymgr_generate_key(g_hnd, KEY_UNKNOWN, UTC_CRYPTO_KEY_NAME);

	TC_ASSERT_EQ("keymgr_generate_key_algo_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_keymgr_generate_key_name_n
 * @brief            key generation
 * @scenario         key generation
 * @apicovered       keymgr_generate_key
 * @precondition     none
 * @postcondition    none
 */
static void utc_keymgr_generate_key_name_n(void)
{
	security_error res = keymgr_generate_key(g_hnd, KEY_AES_128, NULL);

	TC_ASSERT_EQ("keymgr_generate_key_keyname_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_keymgr_set_key_p
 * @brief            Set key
 * @scenario         Set key
 * @apicovered       keymgr_set_key
 * @precondition     none
 * @postcondition    none
 */
static void utc_keymgr_set_key_p(void)
{
	char *pubkey_text = "public key";
	int pubkey_len = strlen(pubkey_text) + 1;
	security_data pubkey = {pubkey_text, pubkey_len};

	char *privkey_text = "private key";
	int privkey_len = strlen(privkey_text) + 1;
	security_data privkey = {privkey_text, privkey_len};

	int i = 0;
	for (; i < sizeof(g_sym_key_type_table)/sizeof(security_key_type); i++) {
		security_error res = keymgr_set_key(g_hnd, g_sym_key_type_table[i], UTC_CRYPTO_KEY_NAME,
											&pubkey, NULL);
		TC_ASSERT_EQ("keymgr_set_key_p", res, SECURITY_OK);
		TC_SUCCESS_RESULT();
	}

	for (i = 0; i < sizeof(g_asym_key_type_table)/sizeof(security_key_type); i++) {
		security_error res = keymgr_set_key(g_hnd, g_asym_key_type_table[i], UTC_CRYPTO_KEY_NAME,
											&pubkey, &privkey);
		TC_ASSERT_EQ("keymgr_set_key_p", res, SECURITY_OK);
		TC_SUCCESS_RESULT();
	}
}

/**
 * @testcase         utc_keymgr_set_key_hnd_n
 * @brief            Set key
 * @scenario         Set key
 * @apicovered       keymgr_set_key
 * @precondition     none
 * @postcondition    none
 */
static void utc_keymgr_set_key_hnd_n(void)
{
	US_DEFINE_DATA(plain, "plain text");

	security_error res = keymgr_set_key(NULL, g_sym_key_type_table[0], UTC_CRYPTO_KEY_NAME, &plain, NULL);

	TC_ASSERT_EQ("keymgr_set_key_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_keymgr_set_key_type_n
 * @brief            Set key
 * @scenario         Set key
 * @apicovered       keymgr_set_key
 * @precondition     none
 * @postcondition    none
 */
static void utc_keymgr_set_key_type_n(void)
{
	US_DEFINE_DATA(plain, "plain text");

	security_error res = keymgr_set_key(g_hnd, KEY_UNKNOWN, UTC_CRYPTO_KEY_NAME, &plain, NULL);

	TC_ASSERT_EQ("keymgr_set_key_type_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_keymgr_set_key_name_n
 * @brief            Set key
 * @scenario         Set key
 * @apicovered       keymgr_set_key
 * @precondition     none
 * @postcondition    none
 */
static void utc_keymgr_set_key_name_n(void)
{
	US_DEFINE_DATA(plain, "plain text");

	security_error res = keymgr_set_key(g_hnd, g_sym_key_type_table[0], NULL, &plain, NULL);

	TC_ASSERT_EQ("keymgr_set_key_name_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_keymgr_set_key_data_n
 * @brief            Set key
 * @scenario         Set key
 * @apicovered       keymgr_set_key
 * @precondition     none
 * @postcondition    none
 */
static void utc_keymgr_set_key_data_n(void)
{
	security_error res = keymgr_set_key(g_hnd, g_sym_key_type_table[0], UTC_CRYPTO_KEY_NAME, NULL, NULL);

	TC_ASSERT_EQ("keymgr_set_key_data_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_keymgr_get_key_p
 * @brief            Get key
 * @scenario         Get key
 * @apicovered       keymgr_get_key
 * @precondition     none
 * @postcondition    none
 */
static void utc_keymgr_get_key_p(void)
{
	security_data pubkey = {NULL, 0};
	security_data privkey = {NULL, 0};

	int i = 0;
	for (; i < sizeof(g_sym_key_type_table)/sizeof(security_key_type); i++) {
		security_error res = keymgr_get_key(g_hnd, g_sym_key_type_table[i], UTC_CRYPTO_KEY_NAME, &pubkey, NULL);
		TC_ASSERT_EQ("keymgr_get_key_p", res, SECURITY_OK);
		TC_SUCCESS_RESULT();
	}

	for (i = 0; i < sizeof(g_asym_key_type_table)/sizeof(security_key_type); i++) {
		security_error res = keymgr_get_key(g_hnd, g_asym_key_type_table[i], UTC_CRYPTO_KEY_NAME, &pubkey, &privkey);
		TC_ASSERT_EQ("keymgr_get_key_p", res, SECURITY_OK);
		TC_SUCCESS_RESULT();
	}
}

/**
 * @testcase         utc_keymgr_get_key_hnd_n
 * @brief            Get key
 * @scenario         Get key
 * @apicovered       keymgr_get_key
 * @precondition     none
 * @postcondition    none
 */
static void utc_keymgr_get_key_hnd_n(void)
{
	security_data pubkey = {NULL, 0};
	security_error res = keymgr_get_key(NULL, g_sym_key_type_table[0], UTC_CRYPTO_KEY_NAME, &pubkey, NULL);

	TC_ASSERT_EQ("keymgr_get_key_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_keymgr_get_key_type_n
 * @brief            Get key
 * @scenario         Get key
 * @apicovered       keymgr_get_key
 * @precondition     none
 * @postcondition    none
 */
static void utc_keymgr_get_key_type_n(void)
{
	security_data pubkey = {NULL, 0};
	security_data privkey = {NULL, 0};
	security_error res = keymgr_get_key(g_hnd, KEY_UNKNOWN, UTC_CRYPTO_KEY_NAME, &pubkey, &privkey);

	TC_ASSERT_EQ("keymgr_get_key_type_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_keymgr_get_key_name_n
 * @brief            Get key
 * @scenario         Get key
 * @apicovered       keymgr_get_key
 * @precondition     none
 * @postcondition    none
 */
static void utc_keymgr_get_key_name_n(void)
{
	security_data pubkey = {NULL, 0};
	security_data privkey = {NULL, 0};
	security_error res = keymgr_get_key(g_hnd, g_sym_key_type_table[0], NULL, &pubkey, &privkey);

	TC_ASSERT_EQ("keymgr_get_key_name_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_keymgr_get_key_input_n
 * @brief            Get key
 * @scenario         Get key
 * @apicovered       keymgr_get_key
 * @precondition     none
 * @postcondition    none
 */
static void utc_keymgr_get_key_input_n(void)
{
	security_error res = keymgr_get_key(g_hnd, g_sym_key_type_table[0], UTC_CRYPTO_KEY_NAME, NULL, NULL);

	TC_ASSERT_EQ("keymgr_get_key_input_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_keymgr_remove_key_p
 * @brief            Remove key
 * @scenario         Remove key
 * @apicovered       keymgr_remove_key
 * @precondition     none
 * @postcondition    none
 */
static void utc_keymgr_remove_key_p(void)
{
	US_DEFINE_DATA(pubkey, "public key data");
	US_DEFINE_DATA(privkey, "private key data");

	int i = 0;
	for (; i < sizeof(g_sym_key_type_table)/sizeof(security_key_type); i++) {
		security_error res = keymgr_set_key(g_hnd, g_sym_key_type_table[i], UTC_CRYPTO_KEY_NAME, &pubkey, NULL);
		TC_ASSERT_EQ("keymgr_remove_key_p", res, SECURITY_OK);
		res = keymgr_remove_key(g_hnd, g_sym_key_type_table[i], UTC_CRYPTO_KEY_NAME);
		TC_ASSERT_EQ("keymgr_remove_key_p", res, SECURITY_OK);
		TC_SUCCESS_RESULT();
	}

	for (i = 0; i < sizeof(g_asym_key_type_table)/sizeof(security_key_type); i++) {
		security_error res = keymgr_set_key(g_hnd, g_asym_key_type_table[i], UTC_CRYPTO_KEY_NAME, &pubkey, &privkey);
		TC_ASSERT_EQ("keymgr_remove_key_p", res, SECURITY_OK);

		res = keymgr_remove_key(g_hnd, g_asym_key_type_table[i], UTC_CRYPTO_KEY_NAME);
		TC_ASSERT_EQ("keymgr_remove_key_p", res, SECURITY_OK);
		TC_SUCCESS_RESULT();
	}
}

/**
 * @testcase         utc_keymgr_remove_key_hnd_n
 * @brief            Remove key
 * @scenario         Remove key
 * @apicovered       keymgr_remove_key
 * @precondition     none
 * @postcondition    none
 */
static void utc_keymgr_remove_key_hnd_n(void)
{
	security_error res = keymgr_remove_key(NULL, g_sym_key_type_table[0], UTC_CRYPTO_KEY_NAME);

	TC_ASSERT_EQ("keymgr_remove_key_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_keymgr_remove_key_type_n
 * @brief            Remove key
 * @scenario         Remove key
 * @apicovered       keymgr_remove_key
 * @precondition     none
 * @postcondition    none
 */
static void utc_keymgr_remove_key_type_n(void)
{
	security_error res = keymgr_remove_key(g_hnd, KEY_UNKNOWN, UTC_CRYPTO_KEY_NAME);

	TC_ASSERT_EQ("keymgr_remove_key_type_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_keymgr_remove_key_name_n
 * @brief            Remove key
 * @scenario         Remove key
 * @apicovered       keymgr_remove_key
 * @precondition     none
 * @postcondition    none
 */
static void utc_keymgr_remove_key_name_n(void)
{
	security_error res = keymgr_remove_key(g_hnd, g_sym_key_type_table[0], NULL);

	TC_ASSERT_EQ("keymgr_remove_key_name_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

void utc_keymgr_main(void)
{
	security_error res = security_init(&g_hnd);
	if (res < 0) {
		US_ERROR;
	}

	utc_keymgr_generate_key_p();
	utc_keymgr_generate_key_hnd_n();
	utc_keymgr_generate_key_algo_n();
	utc_keymgr_generate_key_name_n();
	utc_keymgr_set_key_p();
	utc_keymgr_set_key_hnd_n();
	utc_keymgr_set_key_type_n();
	utc_keymgr_set_key_name_n();
	utc_keymgr_set_key_data_n();
	utc_keymgr_get_key_p();
	utc_keymgr_get_key_hnd_n();
	utc_keymgr_get_key_type_n();
	utc_keymgr_get_key_name_n();
	utc_keymgr_get_key_input_n();
	utc_keymgr_remove_key_p();
	utc_keymgr_remove_key_hnd_n();
	utc_keymgr_remove_key_type_n();
	utc_keymgr_remove_key_name_n();

	res = security_deinit(g_hnd);
	if (res != SECURITY_OK) {
		US_ERROR;
	}
}
