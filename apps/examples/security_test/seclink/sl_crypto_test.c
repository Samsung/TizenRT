/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#include <stdlib.h>
#include <tinyara/seclink.h>
#include <tinyara/seclink_drv.h>
#include <stress_tool/st_perf.h>
#include "sl_test.h"
#include "sl_test_usage.h"

ST_SET_PACK_GLOBAL(sl_crypto);

static char *g_command[] = {
#ifdef SL_CRYPTO_TEST_POOL
#undef SL_CRYPTO_TEST_POOL
#endif
#define SL_CRYPTO_TEST_POOL(command, type, func) command,
#include "sl_crypto_table.h"
};

#ifdef SL_CRYPTO_TEST_POOL
#undef SL_CRYPTO_TEST_POOL
#endif
#define SL_CRYPTO_TEST_POOL(command, type, func) \
	extern void func(sl_options *opt);
#include "sl_crypto_table.h"

static sl_test_func g_func_list[] = {
#ifdef SL_CRYPTO_TEST_POOL
#undef SL_CRYPTO_TEST_POOL
#endif
#define SL_CRYPTO_TEST_POOL(command, type, func) func,
#include "sl_crypto_table.h"
};

typedef enum {
#ifdef SL_CRYPTO_TEST_POOL
#undef SL_CRYPTO_TEST_POOL
#endif
#define SL_CRYPTO_TEST_POOL(command, type, func) type,
#include "sl_crypto_table.h"
	SL_CRYPTO_TYPE_MAX,
	SL_CRYPTO_TYPE_ERR = -1
} sl_crypto_type_e;

#define ST_AES_ENC_KEY_IDX 32
#define ST_AES_DEC_KEY_IDX 33
sl_ctx g_hnd;
char g_key_128[16] = {0,};
char g_key_192[24] = {0,};
char g_key_256[32] = {0,};
unsigned char g_plaintext[128] = {0,};
unsigned char g_ciphertext[128] = {0,};
unsigned char g_iv[16] = {
	0,
};

TESTCASE_SETUP(sl_crypto_global)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_init(&g_hnd));
}
END_TESTCASE

TESTCASE_TEARDOWN(sl_crypto_global)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_deinit(g_hnd));
}
END_TESTCASE

START_TEST_F(aes_ecb)
{
	hal_data aes_key = HAL_DATA_INITIALIZER;
	hal_data enc = HAL_DATA_INITIALIZER;
	hal_data dec = HAL_DATA_INITIALIZER;
	HAL_INIT_AES_PARAM(param);

	aes_key.data = g_key_128;
	aes_key.data_len = 16;
	param.mode = HAL_AES_ECB_NOPAD;
	enc.data = g_plaintext;
	enc.data_len = 16;
	dec.data = g_ciphertext;
	dec.data_len = 16;
	ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_AES_128, ST_AES_ENC_KEY_IDX, &aes_key, NULL));
	ST_EXPECT_EQ(SECLINK_OK, sl_aes_encrypt(g_hnd, &dec, &param, ST_AES_ENC_KEY_IDX, &enc));
	sl_test_print_buffer(enc.data, enc.data_len, "AES-ECB plaintext");
	sl_test_print_buffer(dec.data, dec.data_len, "AES-ECB ciphertext");

	ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_AES_128, ST_AES_DEC_KEY_IDX, &aes_key, NULL));
	ST_EXPECT_EQ(SECLINK_OK, sl_aes_decrypt(g_hnd, &dec, &param, ST_AES_DEC_KEY_IDX, &enc));

	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_AES_128, ST_AES_ENC_KEY_IDX));
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_AES_128, ST_AES_DEC_KEY_IDX));
}
END_TEST_F

START_TEST_F(aes_cbc)
{
	hal_data aes_key = HAL_DATA_INITIALIZER;
	hal_data enc = HAL_DATA_INITIALIZER;
	hal_data dec = HAL_DATA_INITIALIZER;
	HAL_INIT_AES_PARAM(param);

	aes_key.data = g_key_128;
	aes_key.data_len = 16;
	param.mode = HAL_AES_CBC_NOPAD;
	param.iv = (unsigned char *)g_iv;
	param.iv_len = 16;

	enc.data = g_plaintext;
	enc.data_len = 16;
	dec.data = g_ciphertext;
	dec.data_len = 16;

	ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_AES_128, ST_AES_ENC_KEY_IDX, &aes_key, NULL));
	ST_EXPECT_EQ(SECLINK_OK, sl_aes_encrypt(g_hnd, &dec, &param, ST_AES_ENC_KEY_IDX, &enc));
	sl_test_print_buffer(enc.data, enc.data_len, "AES-ECB plaintext");
	sl_test_print_buffer(dec.data, dec.data_len, "AES-ECB ciphertext");
	sl_test_print_buffer((char *)g_iv, 16, "IV");

	ST_EXPECT_EQ(SECLINK_OK, sl_aes_decrypt(g_hnd, &dec, &param, ST_AES_ENC_KEY_IDX, &enc));

	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_AES_128, ST_AES_ENC_KEY_IDX));
}
END_TEST_F

START_TEST_F(aes_cfb128)
{
	hal_data aes_key = HAL_DATA_INITIALIZER;
	hal_data enc = HAL_DATA_INITIALIZER;
	hal_data dec = HAL_DATA_INITIALIZER;
	HAL_INIT_AES_PARAM(param);
	unsigned int iv_offset = 0;

	aes_key.data = g_key_128;
	aes_key.data_len = 16;
	param.mode = HAL_AES_CFB128;
	param.iv = g_iv;
	param.iv_len = 16;
	param.iv_offset = &iv_offset;

	enc.data = g_plaintext;
	enc.data_len = 64;
	dec.data = g_ciphertext;
	dec.data_len = 64;

	ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_AES_128, ST_AES_ENC_KEY_IDX, &aes_key, NULL));
	ST_EXPECT_EQ(SECLINK_OK, sl_aes_encrypt(g_hnd, &dec, &param, ST_AES_ENC_KEY_IDX, &enc));
	sl_test_print_buffer(enc.data, enc.data_len, "AES-CFB128 plaintext");
	sl_test_print_buffer(dec.data, dec.data_len, "AES-CFB128 ciphertext");
	sl_test_print_buffer((char *)g_iv, 16, "IV");
	printf("iv offset %d\n", iv_offset);

	ST_EXPECT_EQ(SECLINK_OK, sl_aes_decrypt(g_hnd, &dec, &param, ST_AES_ENC_KEY_IDX, &enc));
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_AES_128, ST_AES_ENC_KEY_IDX));
}
END_TEST_F

START_TEST_F(aes_ctr)
{
	hal_data aes_key = HAL_DATA_INITIALIZER;
	hal_data enc = HAL_DATA_INITIALIZER;
	hal_data dec = HAL_DATA_INITIALIZER;
	HAL_INIT_AES_PARAM(param);
	unsigned int nc_offset = 0;
	unsigned char nonce_counter[16] = {0,};
	unsigned char stream_block[16] = {0,};
	aes_key.data = g_key_128;
	aes_key.data_len = 16;
	param.mode = HAL_AES_CTR;
	param.nc_off = &nc_offset;
	param.nonce_counter = nonce_counter;
	param.stream_block = stream_block;

	enc.data = g_plaintext;
	enc.data_len = 64;
	dec.data = g_ciphertext;
	dec.data_len = 64;

	ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_AES_128, ST_AES_ENC_KEY_IDX, &aes_key, NULL));
	ST_EXPECT_EQ(SECLINK_OK, sl_aes_encrypt(g_hnd, &dec, &param, ST_AES_ENC_KEY_IDX, &enc));
	sl_test_print_buffer(enc.data, enc.data_len, "AES-CTR plaintext");
	sl_test_print_buffer(dec.data, dec.data_len, "AES-CTR ciphertext");
	sl_test_print_buffer((char *)nonce_counter, 16, "nonce counter");
	sl_test_print_buffer((char *)stream_block, 16, "stream block");
	printf("nc offset %d\n", nc_offset);

	ST_EXPECT_EQ(SECLINK_OK, sl_aes_decrypt(g_hnd, &dec, &param, ST_AES_ENC_KEY_IDX, &enc));
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_AES_128, ST_AES_ENC_KEY_IDX));
}
END_TEST_F

void sl_handle_crypto_aes_ecb(sl_options *opt)
{
	ST_SET_SMOKE1(sl_crypto, opt->count, 0, "aes test", aes_ecb);
}

void sl_handle_crypto_aes_cbc(sl_options *opt)
{
	ST_SET_SMOKE1(sl_crypto, opt->count, 0, "aes test", aes_cbc);
}

void sl_handle_crypto_aes_cfb128(sl_options *opt)
{
	ST_SET_SMOKE1(sl_crypto, opt->count, 0, "aes test", aes_cfb128);
}

void sl_handle_crypto_aes_ctr(sl_options *opt)
{
	ST_SET_SMOKE1(sl_crypto, opt->count, 0, "aes test", aes_ctr);
}

void sl_handle_crypto(sl_options *opt)
{
	ST_TC_SET_GLOBAL(sl_crypto, sl_crypto_global);

	SL_PARSE_MESSAGE(opt, g_command, sl_crypto_type_e,
					 g_func_list, SL_CRYPTO_TYPE_MAX, SL_CRYPTO_TYPE_ERR);
	ST_RUN_TEST(sl_crypto);
	ST_RESULT_TEST(sl_crypto);
}
