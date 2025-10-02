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
#include <stdlib.h>
#include <pthread.h>
#include <tinyara/seclink_drv.h>
#include <tinyara/security_hal.h>
#include <stress_tool/st_perf.h>
#include "hal_test_utils.h"
#include "hal_test.h"

/*
 * Desc: Generate random
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
static hal_data g_rand;
#define RAND_LEN 100

extern struct sec_lowerhalf_s *se_get_device(void);

static struct sec_lowerhalf_s *g_se = NULL;


TEST_SETUP(generate_random)
{
	ST_START_TEST;

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_rand, HAL_AUTH_TEST_MEM_SIZE));

	ST_END_TEST;
}

TEST_TEARDOWN(generate_random)
{
	ST_START_TEST;

	hal_test_free_buffer(&g_rand);

	ST_END_TEST;
}

TEST_F(generate_random)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->generate_random(128, &g_rand));

	ST_END_TEST;
}

/*
 * Desc: Get hash
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
static hal_data g_plain_text;
static hal_data g_hash;
#define HASH_INPUT_SIZE 64
TEST_SETUP(get_hash)
{
	ST_START_TEST;

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_plain_text, HASH_INPUT_SIZE));
	memset(g_plain_text.data, 1, HASH_INPUT_SIZE);
	g_plain_text.data_len = HASH_INPUT_SIZE;

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_hash, HAL_AUTH_TEST_MEM_SIZE));

	ST_END_TEST;
}

TEST_TEARDOWN(get_hash)
{
	ST_START_TEST;

	hal_test_free_buffer(&g_plain_text);
	hal_test_free_buffer(&g_hash);

	ST_END_TEST;
}

TEST_F(get_hash)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->get_hash(HAL_HASH_SHA256, &g_plain_text, &g_hash));

	ST_END_TEST;
}

/*
 * Desc: Get hmac
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
#define HAL_TEST_HMAC_KEY_SLOT 3
static hal_data g_hmac;
TEST_SETUP(get_hmac)
{
	ST_START_TEST;

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_plain_text, HASH_INPUT_SIZE));
	memset(g_plain_text.data, 1, HASH_INPUT_SIZE);
	g_plain_text.data_len = HASH_INPUT_SIZE;

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_hmac, HAL_AUTH_TEST_MEM_SIZE));

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->generate_key(HAL_KEY_HMAC_SHA256, HAL_TEST_HMAC_KEY_SLOT));

	ST_END_TEST;
}

TEST_TEARDOWN(get_hmac)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->remove_key(HAL_KEY_HMAC_SHA256, HAL_TEST_HMAC_KEY_SLOT));
	hal_test_free_buffer(&g_plain_text);
	hal_test_free_buffer(&g_hmac);

	ST_END_TEST;
}

TEST_F(get_hmac)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->get_hmac(HAL_HMAC_SHA256, &g_plain_text, HAL_TEST_HMAC_KEY_SLOT, &g_hmac));

	ST_END_TEST;
}

/*
 * Desc: Get RSA signagure
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
static hal_data g_rsa_hash;
static hal_data g_rsa_signature;
hal_rsa_mode g_rsa_mode;
#define HAL_TEST_RSA_HASH_LEN 32
#define HAL_TEST_RSA_KEY_SLOT 1
TEST_SETUP(rsa_sign)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->generate_key(HAL_KEY_RSA_1024, HAL_TEST_RSA_KEY_SLOT));

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_rsa_signature, HAL_AUTH_TEST_MEM_SIZE));

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_rsa_hash, HAL_TEST_RSA_HASH_LEN));
	g_rsa_hash.data_len = HAL_TEST_RSA_HASH_LEN;
	memset(g_rsa_hash.data, 0xa5, HAL_TEST_RSA_HASH_LEN);

	g_rsa_mode.rsa_a = HAL_RSASSA_PKCS1_PSS_MGF1;
	g_rsa_mode.hash_t = HAL_HASH_SHA256;
	g_rsa_mode.mgf = HAL_HASH_SHA256;
	g_rsa_mode.salt_byte_len = 64;

	ST_END_TEST;
}

TEST_TEARDOWN(rsa_sign)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->remove_key(HAL_KEY_RSA_1024, HAL_TEST_RSA_KEY_SLOT));

	/*  g_rsa_hash is not allocated from hal API so it'd be better to free here */
	hal_test_free_buffer(&g_rsa_hash);

	hal_test_free_buffer(&g_rsa_signature);

	ST_END_TEST;
}

TEST_F(rsa_sign)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->rsa_sign_md(g_rsa_mode, &g_rsa_hash, HAL_TEST_RSA_KEY_SLOT, &g_rsa_signature));

	ST_END_TEST;
}

/*
 * Desc: Verify RSA signature
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
TEST_SETUP(rsa_verify)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->generate_key(HAL_KEY_RSA_1024, HAL_TEST_RSA_KEY_SLOT));

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_rsa_signature, HAL_AUTH_TEST_MEM_SIZE));

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_rsa_hash, HAL_TEST_RSA_HASH_LEN));
	g_rsa_hash.data_len = HAL_TEST_RSA_HASH_LEN;
	memset(g_rsa_hash.data, 0xa5, HAL_TEST_RSA_HASH_LEN);

	g_rsa_mode.rsa_a = HAL_RSASSA_PKCS1_PSS_MGF1;
	g_rsa_mode.hash_t = HAL_HASH_SHA256;
	g_rsa_mode.mgf = HAL_HASH_SHA256;
	g_rsa_mode.salt_byte_len = 64;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->rsa_sign_md(g_rsa_mode, &g_rsa_hash, HAL_TEST_RSA_KEY_SLOT, &g_rsa_signature));

	ST_END_TEST;
}

TEST_TEARDOWN(rsa_verify)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->remove_key(HAL_KEY_RSA_1024, HAL_TEST_RSA_KEY_SLOT));

	/*  g_rsa_hash is not allocated from hal API so it'd be better to free here */
	hal_test_free_buffer(&g_rsa_hash);

	hal_test_free_buffer(&g_rsa_signature);

	ST_END_TEST;
}

TEST_F(rsa_verify)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->rsa_verify_md(g_rsa_mode, &g_rsa_hash, &g_rsa_signature, HAL_TEST_RSA_KEY_SLOT));

	ST_END_TEST;
}

/*
 * Desc: Get ECDSA
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
#define HAL_TEST_ECC_KEY_SLOT 1
#define HAL_TEST_ECC_HASH_LEN 32
static hal_data g_ecdsa_hash;
static hal_data g_ecdsa_signature;
hal_ecdsa_mode ecdsa_mode;
TEST_SETUP(ecdsa_sign)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->generate_key(HAL_KEY_ECC_BRAINPOOL_P256R1, HAL_TEST_ECC_KEY_SLOT));

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_ecdsa_signature, HAL_AUTH_TEST_MEM_SIZE));

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_ecdsa_hash, HAL_TEST_ECC_HASH_LEN));
	g_ecdsa_hash.data_len = HAL_TEST_ECC_HASH_LEN;
	memset(g_ecdsa_hash.data, 0xa5, HAL_TEST_ECC_HASH_LEN);

	ecdsa_mode.curve = HAL_ECDSA_BRAINPOOL_P256R1;
	ecdsa_mode.hash_t = HAL_HASH_SHA256;

	ST_END_TEST;
}

TEST_TEARDOWN(ecdsa_sign)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->remove_key(HAL_KEY_ECC_BRAINPOOL_P256R1, HAL_TEST_ECC_KEY_SLOT));

	hal_test_free_buffer(&g_ecdsa_hash);
	hal_test_free_buffer(&g_ecdsa_signature);

	ST_END_TEST;
}

TEST_F(ecdsa_sign)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->ecdsa_sign_md(ecdsa_mode, &g_ecdsa_hash, HAL_TEST_ECC_KEY_SLOT, &g_ecdsa_signature));

	ST_END_TEST;
}


/*
 * Desc: Verify ECDSA
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
TEST_SETUP(ecdsa_verify)
{
	ST_START_TEST;
	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->generate_key(HAL_KEY_ECC_BRAINPOOL_P256R1, HAL_TEST_ECC_KEY_SLOT));

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_ecdsa_signature, HAL_AUTH_TEST_MEM_SIZE));

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_ecdsa_hash, HAL_TEST_ECC_HASH_LEN));
	g_ecdsa_hash.data_len = HAL_TEST_ECC_HASH_LEN;
	memset(g_ecdsa_hash.data, 0xa5, HAL_TEST_ECC_HASH_LEN);

	ecdsa_mode.curve = HAL_ECDSA_BRAINPOOL_P256R1;
	ecdsa_mode.hash_t = HAL_HASH_SHA256;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->ecdsa_sign_md(ecdsa_mode, &g_ecdsa_hash, HAL_TEST_ECC_KEY_SLOT, &g_ecdsa_signature));

	ST_END_TEST;
}

TEST_TEARDOWN(ecdsa_verify)
{
	ST_START_TEST;
	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->remove_key(HAL_KEY_ECC_BRAINPOOL_P256R1, HAL_TEST_ECC_KEY_SLOT));

	hal_test_free_buffer(&g_ecdsa_hash);
	hal_test_free_buffer(&g_ecdsa_signature);

	ST_END_TEST;
}

TEST_F(ecdsa_verify)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->ecdsa_verify_md(ecdsa_mode, &g_ecdsa_hash, &g_ecdsa_signature, HAL_TEST_ECC_KEY_SLOT));

	ST_END_TEST;
}

/*
 * Desc: Generate DH parameters
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
/* Generate G, P, GX (G^X mod P) */
#define HAL_TEST_DH_X_SLOT 1
#define HAL_TEST_DH_Y_SLOT 2
static hal_dh_data g_dh_data;
unsigned char p_buf_1024[] = {
	0xb1, 0x0b, 0x8f, 0x96, 0xa0, 0x80, 0xe0, 0x1d, 0xde, 0x92, 0xde, 0x5e, 0xae,
	0x5d, 0x54, 0xec, 0x52, 0xc9, 0x9f, 0xbc, 0xfb, 0x06, 0xa3, 0xc6, 0x9a, 0x6a,
	0x9d, 0xca, 0x52, 0xd2, 0x3b, 0x61, 0x60, 0x73, 0xe2, 0x86, 0x75, 0xa2, 0x3d,
	0x18, 0x98, 0x38, 0xef, 0x1e, 0x2e, 0xe6, 0x52, 0xc0, 0x13, 0xec, 0xb4, 0xae,
	0xa9, 0x06, 0x11, 0x23, 0x24, 0x97, 0x5c, 0x3c, 0xd4, 0x9b, 0x83, 0xbf, 0xac,
	0xcb, 0xdd, 0x7d, 0x90, 0xc4, 0xbd, 0x70, 0x98, 0x48, 0x8e, 0x9c, 0x21, 0x9a,
	0x73, 0x72, 0x4e, 0xff, 0xd6, 0xfa, 0xe5, 0x64, 0x47, 0x38, 0xfa, 0xa3, 0x1a,
	0x4f, 0xf5, 0x5b, 0xcc, 0xc0, 0xa1, 0x51, 0xaf, 0x5f, 0x0d, 0xc8, 0xb4, 0xbd,
	0x45, 0xbf, 0x37, 0xdf, 0x36, 0x5c, 0x1a, 0x65, 0xe6, 0x8c, 0xfd, 0xa7, 0x6d,
	0x4d, 0xa7, 0x08, 0xdf, 0x1f, 0xb2, 0xbc, 0x2e, 0x4a, 0x43, 0x71
};
unsigned char g_buf_1024[] = {
	0xa4, 0xd1, 0xcb, 0xd5, 0xc3, 0xfd, 0x34, 0x12, 0x67, 0x65, 0xa4, 0x42, 0xef,
	0xb9, 0x99, 0x05, 0xf8, 0x10, 0x4d, 0xd2, 0x58, 0xac, 0x50, 0x7f, 0xd6, 0x40,
	0x6c, 0xff, 0x14, 0x26,	0x6d, 0x31, 0x26, 0x6f, 0xea, 0x1e, 0x5c, 0x41, 0x56,
	0x4b, 0x77, 0x7e, 0x69, 0x0f, 0x55, 0x04, 0xf2, 0x13, 0x16, 0x02, 0x17, 0xb4,
	0xb0, 0x1b, 0x88, 0x6a, 0x5e, 0x91, 0x54, 0x7f,	0x9e, 0x27, 0x49, 0xf4, 0xd7,
	0xfb, 0xd7, 0xd3, 0xb9, 0xa9, 0x2e, 0xe1, 0x90, 0x9d, 0x0d, 0x22, 0x63, 0xf8,
	0x0a, 0x76, 0xa6, 0xa2, 0x4c, 0x08, 0x7a, 0x09, 0x1f, 0x53, 0x1d, 0xbf,	0x0a,
	0x01, 0x69, 0xb6, 0xa2, 0x8a, 0xd6, 0x62, 0xa4, 0xd1, 0x8e, 0x73, 0xaf, 0xa3,
	0x2d, 0x77, 0x9d, 0x59, 0x18, 0xd0, 0x8b, 0xc8, 0x85, 0x8f, 0x4d, 0xce, 0xf9,
	0x7c, 0x2a, 0x24, 0x85, 0x5e, 0x6e, 0xeb, 0x22, 0xb3, 0xb2, 0xe5
};
TEST_SETUP(dh_generate_param)
{
	ST_START_TEST;

	g_dh_data.mode = HAL_DH_1024;
	g_dh_data.G = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_dh_data.G);
	g_dh_data.G->data = g_buf_1024;
	g_dh_data.G->data_len = sizeof(g_buf_1024);

	g_dh_data.P = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_dh_data.P);
	g_dh_data.P->data = p_buf_1024;
	g_dh_data.P->data_len = sizeof(p_buf_1024);

	g_dh_data.pubkey = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_dh_data.pubkey);
	ST_EXPECT_EQ(0, hal_test_malloc_buffer(g_dh_data.pubkey, HAL_AUTH_TEST_MEM_SIZE));

	ST_END_TEST;
}

TEST_TEARDOWN(dh_generate_param)
{
	ST_START_TEST;

	free(g_dh_data.G);
	free(g_dh_data.P);
	hal_test_free_buffer(g_dh_data.pubkey);
	free(g_dh_data.pubkey);

	ST_END_TEST;
}

TEST_F(dh_generate_param)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->dh_generate_param(HAL_TEST_DH_X_SLOT, &g_dh_data));

	ST_END_TEST;
}

/*
 * Desc: Compute DH shared secret
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
static hal_data g_dh_shared_secret;
TEST_SETUP(dh_compute_shared_secret)
{
	ST_START_TEST;

	g_dh_data.mode = HAL_DH_1024;

	g_dh_data.G = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_dh_data.G);
	g_dh_data.G->data = g_buf_1024;
	g_dh_data.G->data_len = sizeof(g_buf_1024);

	g_dh_data.P = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_dh_data.P);
	g_dh_data.P->data = p_buf_1024;
	g_dh_data.P->data_len = sizeof(p_buf_1024);

	g_dh_data.pubkey = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_dh_data.pubkey);
	ST_EXPECT_EQ(0, hal_test_malloc_buffer(g_dh_data.pubkey, HAL_AUTH_TEST_MEM_SIZE));

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->dh_generate_param(HAL_TEST_DH_X_SLOT, &g_dh_data));

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_dh_shared_secret, HAL_AUTH_TEST_MEM_SIZE));

	ST_END_TEST;
}

TEST_TEARDOWN(dh_compute_shared_secret)
{
	ST_START_TEST;

	free(g_dh_data.G);
	free(g_dh_data.P);
	hal_test_free_buffer(g_dh_data.pubkey);
	free(g_dh_data.pubkey);
	hal_test_free_buffer(&g_dh_shared_secret);

	ST_END_TEST;
}

TEST_F(dh_compute_shared_secret)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->dh_compute_shared_secret(&g_dh_data, HAL_TEST_DH_X_SLOT, &g_dh_shared_secret));

	ST_END_TEST;
}


/*
 * Desc: Compute ECDH shared secret
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
#define HAL_TEST_ECDH_KEY_SLOT_A 1
#define HAL_TEST_ECDH_KEY_SLOT_B 2
static hal_data g_shared_secret_a;
static hal_data g_shared_secret_b;
static hal_ecdh_data ecdh_a;
static hal_ecdh_data ecdh_b;
static hal_data key_a;
static hal_data key_b;

TEST_SETUP(ecdh_compute_shared_secret)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->generate_key(HAL_KEY_ECC_SEC_P256R1, HAL_TEST_ECDH_KEY_SLOT_A));
	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->generate_key(HAL_KEY_ECC_SEC_P256R1, HAL_TEST_ECDH_KEY_SLOT_B));

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&key_a, HAL_AUTH_TEST_MEM_SIZE));
	ST_EXPECT_EQ(0, hal_test_malloc_buffer_priv(&key_a, HAL_AUTH_TEST_MEM_SIZE));
	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&key_b, HAL_AUTH_TEST_MEM_SIZE));
	ST_EXPECT_EQ(0, hal_test_malloc_buffer_priv(&key_b, HAL_AUTH_TEST_MEM_SIZE));

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->get_key(HAL_KEY_ECC_SEC_P256R1, HAL_TEST_ECDH_KEY_SLOT_A, &key_a));
	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->get_key(HAL_KEY_ECC_SEC_P256R1, HAL_TEST_ECDH_KEY_SLOT_B, &key_b));

	ecdh_a.pubkey_x = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, ecdh_a.pubkey_x);
	ecdh_a.pubkey_y = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, ecdh_a.pubkey_y);
	ecdh_b.pubkey_x = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, ecdh_b.pubkey_x);
	ecdh_b.pubkey_y = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, ecdh_b.pubkey_y);

	//data: pubkey_x, priv: pubkey_y
	ecdh_a.curve = HAL_ECDSA_SEC_P256R1;
	ecdh_a.pubkey_x->data = key_a.data;
	ecdh_a.pubkey_x->data_len = key_a.data_len;
	ecdh_a.pubkey_y->data = key_a.priv;
	ecdh_a.pubkey_y->data_len = key_a.priv_len;

	//data: pubkey_x, priv: pubkey_y
	ecdh_b.curve = HAL_ECDSA_SEC_P256R1;
	ecdh_b.pubkey_x->data = key_b.data;
	ecdh_b.pubkey_x->data_len = key_b.data_len;
	ecdh_b.pubkey_y->data = key_b.priv;
	ecdh_b.pubkey_y->data_len = key_b.priv_len;

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_shared_secret_a, HAL_AUTH_TEST_MEM_SIZE));
	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_shared_secret_b, HAL_AUTH_TEST_MEM_SIZE));
	ST_END_TEST;
}

TEST_TEARDOWN(ecdh_compute_shared_secret)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->remove_key(HAL_KEY_ECC_SEC_P256R1, HAL_TEST_ECDH_KEY_SLOT_A));
	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->remove_key(HAL_KEY_ECC_SEC_P256R1, HAL_TEST_ECDH_KEY_SLOT_B));

	hal_test_free_buffer(&key_a);
	hal_test_free_buffer(&key_b);
	free(ecdh_a.pubkey_x);
	free(ecdh_a.pubkey_y);
	free(ecdh_b.pubkey_x);
	free(ecdh_b.pubkey_y);
	hal_test_free_buffer(&g_shared_secret_a);
	hal_test_free_buffer(&g_shared_secret_b);

	ST_END_TEST;
}

TEST_F(ecdh_compute_shared_secret)
{
	ST_START_TEST;

	// A<--B, B<--A
	if (ecdh_a.pubkey_x != NULL && ecdh_b.pubkey_x != NULL) {
		ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->ecdh_compute_shared_secret(&ecdh_b, HAL_TEST_ECDH_KEY_SLOT_A, &g_shared_secret_a));
		ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->ecdh_compute_shared_secret(&ecdh_a, HAL_TEST_ECDH_KEY_SLOT_B, &g_shared_secret_b));
		ST_EXPECT_EQ(g_shared_secret_a.data_len, g_shared_secret_b.data_len);
		//ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, memcmp(&g_shared_secret_a.data, &g_shared_secret_b.data, g_shared_secret_a.data_len));
	}

	ST_END_TEST;
}

/*
 * Desc: Save certificate in secure storage
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
#define HAL_TEST_CERT_SLOT 2
static const char test_crt[] =
		"-----BEGIN CERTIFICATE-----\r\n"
		"MIICaDCCAgygAwIBAgIBAjAMBggqhkjOPQQDAgUAMHAxLTArBgNVBAMTJFNhbXN1\r\n"
		"bmcgRWxlY3Ryb25pY3MgT0NGIFJvb3QgQ0EgVEVTVDEUMBIGA1UECxMLT0NGIFJv\r\n"
		"b3QgQ0ExHDAaBgNVBAoTE1NhbXN1bmcgRWxlY3Ryb25pY3MxCzAJBgNVBAYTAktS\r\n"
		"MCAXDTE2MTEyNDAyNDcyN1oYDzIwNjkxMjMxMTQ1OTU5WjBwMS0wKwYDVQQDEyRT\r\n"
		"YW1zdW5nIEVsZWN0cm9uaWNzIE9DRiBSb290IENBIFRFU1QxFDASBgNVBAsTC09D\r\n"
		"RiBSb290IENBMRwwGgYDVQQKExNTYW1zdW5nIEVsZWN0cm9uaWNzMQswCQYDVQQG\r\n"
		"EwJLUjBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABBzzury7p8HANVn+v4CIa2h/\r\n"
		"R/SAt3VVst+vTv4/kR+lgU1OEiT3t9+mOWE7J+oddpRofFW2DdeJkpfQUVOn4NOj\r\n"
		"gZIwgY8wDgYDVR0PAQH/BAQDAgHGMC4GA1UdHwQnMCUwI6AhoB+GHWh0dHA6Ly9j\r\n"
		"YS5zYW1zdW5naW90cy5jb20vY3JsMA8GA1UdEwEB/wQFMAMBAf8wPAYIKwYBBQUH\r\n"
		"AQEEMDAuMCwGCCsGAQUFBzABhiBodHRwOi8vb2NzcC10ZXN0LnNhbXN1bmdpb3Rz\r\n"
		"LmNvbTAMBggqhkjOPQQDAgUAA0gAMEUCIQCIsi3BcOQMXO/pCiUA+S75bYFWS27E\r\n"
		"GAq9e2E3+hQ2TAIgWrTieFAZ5xRH3BnSHG+XEF2HPD99y/SYSa6T59YW+jE=\r\n"
		"-----END CERTIFICATE-----\r\n\0";
static hal_data g_cert_in;
static hal_data g_cert_out;
TEST_SETUP(set_certificate)
{
	ST_START_TEST;

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_cert_in, sizeof(test_crt)));
	memcpy(g_cert_in.data, test_crt, sizeof(test_crt));
	g_cert_in.data_len = sizeof(test_crt);

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_cert_out, sizeof(test_crt)));

	ST_END_TEST;
}

TEST_TEARDOWN(set_certificate)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->remove_certificate(HAL_TEST_CERT_SLOT));

	hal_test_free_buffer(&g_cert_in);
	hal_test_free_buffer(&g_cert_out);

	ST_END_TEST;
}

TEST_F(set_certificate)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->set_certificate(HAL_TEST_CERT_SLOT, &g_cert_in));

	ST_END_TEST;
}

/*
 * Desc: Load certificate in secure storage
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
TEST_SETUP(get_certificate)
{
	ST_START_TEST;

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_cert_in, sizeof(test_crt)));
	memcpy(g_cert_in.data, test_crt, sizeof(test_crt));
	g_cert_in.data_len = sizeof(test_crt);

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->set_certificate(HAL_TEST_CERT_SLOT, &g_cert_in));

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_cert_out, sizeof(test_crt)));

	ST_END_TEST;
}

TEST_TEARDOWN(get_certificate)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->remove_certificate(HAL_TEST_CERT_SLOT));
	hal_test_free_buffer(&g_cert_in);
	hal_test_free_buffer(&g_cert_out);

	ST_END_TEST;
}

TEST_F(get_certificate)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->get_certificate(HAL_TEST_CERT_SLOT, &g_cert_out));
	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, memcmp(g_cert_out.data, g_cert_in.data, g_cert_out.data_len));

	ST_END_TEST;
}

/*
 * Desc: Delete certificate in secure storage
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
TEST_SETUP(remove_certificate)
{
	ST_START_TEST;


	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_cert_in, sizeof(test_crt)));
	memcpy(g_cert_in.data, test_crt, sizeof(test_crt));
	g_cert_in.data_len = sizeof(test_crt);

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->set_certificate(HAL_TEST_CERT_SLOT, &g_cert_in));

	ST_END_TEST;
}

TEST_TEARDOWN(remove_certificate)
{
	ST_START_TEST;

	hal_test_free_buffer(&g_cert_in);

	ST_END_TEST;
}

TEST_F(remove_certificate)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->remove_certificate(HAL_TEST_CERT_SLOT));

	ST_END_TEST;
}

/*
 * Desc: Get factorykey data
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
static hal_data g_factory_key;
static hal_data g_factory_cert;
static hal_data g_factory_data;

#define HAL_TEST_FACTORY_KEY_SLOT 3
#define HAL_TEST_FACTORY_CERT_SLOT 4
#define HAL_TEST_FACTORY_DATA_SLOT 5


TEST_SETUP(get_factory)
{
	ST_START_TEST;

	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_factory_key, HAL_AUTH_TEST_MEM_SIZE));
	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_factory_cert, HAL_AUTH_TEST_MEM_SIZE));
	ST_EXPECT_EQ(0, hal_test_malloc_buffer(&g_factory_data, HAL_AUTH_TEST_MEM_SIZE));

	ST_END_TEST;
}

TEST_TEARDOWN(get_factory)
{
	ST_START_TEST;

	hal_test_free_buffer(&g_factory_key);
	hal_test_free_buffer(&g_factory_cert);
	hal_test_free_buffer(&g_factory_data);

	ST_END_TEST;
}

TEST_F(get_factory)
{
	ST_START_TEST;

	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->get_factory_key(HAL_TEST_FACTORY_KEY_SLOT, &g_factory_key));
	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->get_factory_cert(HAL_TEST_FACTORY_CERT_SLOT, &g_factory_cert));
	ST_EXPECT_EQ2(HAL_SUCCESS, HAL_NOT_SUPPORTED, g_se->ops->get_factory_data(HAL_TEST_FACTORY_DATA_SLOT, &g_factory_data));

	ST_END_TEST;
}


pthread_addr_t hal_auth_test(void)
{
	g_se = se_get_device();

	ST_SET_PACK(hal_auth);

	ST_SET_SMOKE(hal_auth, HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "Generate_random", generate_random);
	ST_SET_SMOKE(hal_auth, HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "Get hash", get_hash);
	ST_SET_SMOKE(hal_auth, HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "Get hmac", get_hmac);
	ST_SET_SMOKE(hal_auth, HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "RSA signature", rsa_sign);
	ST_SET_SMOKE(hal_auth, HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "RSA verification", rsa_verify);
	ST_SET_SMOKE(hal_auth, HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "ECDSA signature", ecdsa_sign);
	ST_SET_SMOKE(hal_auth, HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "ECDSA verification", ecdsa_verify);
	ST_SET_SMOKE(hal_auth, HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "Generate DH parameters", dh_generate_param);
	ST_SET_SMOKE(hal_auth, HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "Compute DH shared secret", dh_compute_shared_secret);
	ST_SET_SMOKE(hal_auth, HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "Compute ECDH shared secret", ecdh_compute_shared_secret);
	ST_SET_SMOKE(hal_auth, HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "Set certificate", set_certificate);
	ST_SET_SMOKE(hal_auth, HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "Get certificate", get_certificate);
	ST_SET_SMOKE(hal_auth, HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "Remove certificate", remove_certificate);
	ST_SET_SMOKE(hal_auth, HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "Get factory key/cert/data", get_factory);

	ST_RUN_TEST(hal_auth);
	ST_RESULT_TEST(hal_auth);

	return 0;
}
