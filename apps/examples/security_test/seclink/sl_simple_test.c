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
 * software distributed under the License is distributed on an * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <tinyara/seclink.h>
#include <tinyara/seclink_drv.h>
#include <stress_tool/st_perf.h>
#include "sl_test.h"
#include "sl_sample_key.h"

#define REPEAT_COUNT 5
#define KEY_SLOT 32
#define SL_TEST_KEY_LEN 256
#define SL_TEST_SYM_KEY_LEN 32
static sl_ctx g_hnd;
static hal_data g_sym_key_in;
static hal_data g_asym_pubkey_in;
static hal_data g_asym_prikey_in;
static hal_data g_key_out;

TESTCASE_SETUP(simple_tc)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_init(&g_hnd));
} END_TESTCASE

TESTCASE_TEARDOWN(simple_tc)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_deinit(g_hnd));
} END_TESTCASE

/*
 * TestCase: Key
 */
TESTCASE_SETUP(key_testcase)
{
	/*  public key */
	g_asym_pubkey_in.data = g_ed25519_pubkey;
	g_asym_pubkey_in.data_len = sizeof(g_ed25519_pubkey);
	g_asym_pubkey_in.priv = NULL;
	g_asym_pubkey_in.priv_len = 0;

	g_asym_prikey_in.data = g_ed25519_privkey_only;
	g_asym_prikey_in.data_len = sizeof(g_ed25519_privkey_only);
	g_asym_prikey_in.priv = NULL;
	g_asym_prikey_in.priv_len = 0;

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_sym_key_in, SL_TEST_SYM_KEY_LEN));
	g_sym_key_in.priv = NULL;
	g_sym_key_in.priv_len = 0;

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_key_out, SL_TEST_KEY_LEN));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer_priv(&g_key_out, SL_TEST_KEY_LEN));
} END_TESTCASE

TESTCASE_TEARDOWN(key_testcase)
{
	sl_test_free_buffer(&g_sym_key_in);
	sl_test_free_buffer(&g_key_out);
} END_TESTCASE

/*
 * Description: Set key
 */
START_TEST_F(set_sym_key)
{
	hal_result_e hres = HAL_FAIL;

	ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_AES_256,
										KEY_SLOT, &g_sym_key_in, NULL, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_AES_256,
										KEY_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);
} END_TEST_F

START_TEST_F(set_asym_key)
{
	hal_result_e hres = HAL_FAIL;

	ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_ECC_25519,
										KEY_SLOT, &g_asym_pubkey_in, &g_asym_prikey_in, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_25519,
										KEY_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);
} END_TEST_F

/*
 * Description: Get key
 */
START_TEST_F(get_key)
{
	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_ECC_SEC_P256R1,
										KEY_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_SEC_P256R1,
										KEY_SLOT, &g_key_out, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_SEC_P256R1,
										KEY_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);
} END_TEST_F

/*
 * Description: generate key
 */
START_TEST_F(gen_key)
{
	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_ECC_SEC_P256R1,
										KEY_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_SEC_P256R1,
										KEY_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);
} END_TEST_F

/*
 * Description: generate random
 */
#define SL_TEST_AUTH_MEM_SIZE 4096
static hal_data g_rand;
#define SL_TEST_RAND_LEN 128
#define SL_TEST_ECC_KEY_SLOT 32
#define SL_TEST_ECC_HASH_LEN 32

static hal_data g_ecdsa_signature;
static hal_data g_ecdsa_hash;
#define SL_TEST_ECC_HASH_LEN 32
static hal_ecdsa_mode g_ecdsa_mode;

START_TEST_F(gen_random)
{
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_rand, SL_TEST_AUTH_MEM_SIZE));

	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_generate_random(g_hnd, SL_TEST_RAND_LEN, &g_rand, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	sl_test_free_buffer(&g_rand);
} END_TEST_F

/*
 * Description: sign ecdsa
 */
#define SL_TEST_ECC_KEY_SLOT 32
#define SL_TEST_ECC_HASH_LEN 32

#define SL_TEST_ECDH_KEY_SLOT_A 32
#define SL_TEST_ECDH_KEY_SLOT_B 33
START_TEST_F(ecdsa_sign)
{
	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECC_KEY_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_ecdsa_signature, SL_TEST_AUTH_MEM_SIZE));

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_ecdsa_hash, SL_TEST_ECC_HASH_LEN));
	g_ecdsa_hash.data_len = SL_TEST_ECC_HASH_LEN;
	memset(g_ecdsa_hash.data, 0xa5, SL_TEST_ECC_HASH_LEN);

	g_ecdsa_mode.curve = HAL_ECDSA_SEC_P256R1;
	g_ecdsa_mode.hash_t = HAL_HASH_SHA256;

	ST_EXPECT_EQ(SECLINK_OK, sl_ecdsa_sign_md(g_hnd, g_ecdsa_mode,
											  &g_ecdsa_hash, SL_TEST_ECC_KEY_SLOT, &g_ecdsa_signature, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECC_KEY_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	sl_test_free_buffer(&g_ecdsa_hash);
	sl_test_free_buffer(&g_ecdsa_signature);
} END_TEST_F

/*
 * Description: verify ECDSA signature
 */
START_TEST_F(ecdsa_verify)
{
	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECC_KEY_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_ecdsa_signature, SL_TEST_AUTH_MEM_SIZE));

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_ecdsa_hash, SL_TEST_ECC_HASH_LEN));
	g_ecdsa_hash.data_len = SL_TEST_ECC_HASH_LEN;
	memset(g_ecdsa_hash.data, 0xa5, SL_TEST_ECC_HASH_LEN);

	g_ecdsa_mode.curve = HAL_ECDSA_SEC_P256R1;
	g_ecdsa_mode.hash_t = HAL_HASH_SHA256;

	ST_EXPECT_EQ(SECLINK_OK, sl_ecdsa_sign_md(g_hnd, g_ecdsa_mode, &g_ecdsa_hash, SL_TEST_ECC_KEY_SLOT, &g_ecdsa_signature, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(SECLINK_OK, sl_ecdsa_verify_md(g_hnd, g_ecdsa_mode, &g_ecdsa_hash, &g_ecdsa_signature, SL_TEST_ECC_KEY_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECC_KEY_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);


	sl_test_print_buffer(g_ecdsa_hash.data, g_ecdsa_hash.data_len, "ECDSA hash");
	sl_test_print_buffer(g_ecdsa_signature.data, g_ecdsa_signature.data_len, "ECDSA hash");

	sl_test_free_buffer(&g_ecdsa_hash);
	sl_test_free_buffer(&g_ecdsa_signature);
} END_TEST_F

/*
 * Description: Compute ECDH
 */
#define SL_TEST_CERT_SLOT 32

static hal_data g_key_a;
static hal_data g_key_b;
static hal_data g_shared_secret_a;
static hal_data g_shared_secret_b;
static hal_ecdh_data ecdh_a;
static hal_ecdh_data ecdh_b;

static hal_data g_cert_in;
static hal_data g_cert_out;

START_TEST_F(ecdh_compute)
{
	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_A, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);
	ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_B, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_key_a, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer_priv(&g_key_a, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_key_b, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer_priv(&g_key_b, SL_TEST_AUTH_MEM_SIZE));

	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_A, &g_key_a, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_B, &g_key_b, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

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
	ecdh_a.pubkey_x->data = g_key_a.data;
	ecdh_a.pubkey_x->data_len = g_key_a.data_len;
	ecdh_a.pubkey_y->data = g_key_a.priv;
	ecdh_a.pubkey_y->data_len = g_key_a.priv_len;

	//data: pubkey_x, priv: pubkey_y
	ecdh_b.curve = HAL_ECDSA_SEC_P256R1;
	ecdh_b.pubkey_x->data = g_key_b.data;
	ecdh_b.pubkey_x->data_len = g_key_b.data_len;
	ecdh_b.pubkey_y->data = g_key_b.priv;
	ecdh_b.pubkey_y->data_len = g_key_b.priv_len;

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_shared_secret_a, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_shared_secret_b, SL_TEST_AUTH_MEM_SIZE));

	// A<--B, B<--A
	if (ecdh_a.pubkey_x != NULL && ecdh_b.pubkey_x != NULL) {
		ST_EXPECT_EQ(SECLINK_OK, sl_ecdh_compute_shared_secret(g_hnd, &ecdh_b, SL_TEST_ECDH_KEY_SLOT_A, &g_shared_secret_a, &hres));
		ST_EXPECT_EQ(HAL_SUCCESS, hres);
		ST_EXPECT_EQ(SECLINK_OK, sl_ecdh_compute_shared_secret(g_hnd, &ecdh_a, SL_TEST_ECDH_KEY_SLOT_B, &g_shared_secret_b, &hres));
		ST_EXPECT_EQ(HAL_SUCCESS, hres);

		ST_EXPECT_EQ(g_shared_secret_a.data_len, g_shared_secret_b.data_len);
		ST_EXPECT_EQ(0, memcmp(g_shared_secret_a.data, g_shared_secret_b.data, g_shared_secret_a.data_len));
	}

	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_A, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_B, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	sl_test_free_buffer(&g_key_a);
	sl_test_free_buffer(&g_key_b);
	free(ecdh_a.pubkey_x);
	free(ecdh_a.pubkey_y);
	free(ecdh_b.pubkey_x);
	free(ecdh_b.pubkey_y);
	sl_test_free_buffer(&g_shared_secret_a);
	sl_test_free_buffer(&g_shared_secret_b);

} END_TEST_F

/*
 * Description: set certificate
 */
START_TEST_F(set_certificate)
{
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_cert_in, sizeof(g_test_crt)));
	memcpy(g_cert_in.data, g_test_crt, sizeof(g_test_crt));
	g_cert_in.data_len = sizeof(g_test_crt);

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_cert_out, sizeof(g_test_crt)));

	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_set_certificate(g_hnd, SL_TEST_CERT_SLOT, &g_cert_in, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(SECLINK_OK, sl_remove_certificate(g_hnd, SL_TEST_CERT_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	sl_test_free_buffer(&g_cert_in);
	sl_test_free_buffer(&g_cert_out);
} END_TEST_F

/*
 * Description: Get certificate
 */
START_TEST_F(get_certificate)
{
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_cert_in, sizeof(g_test_crt)));
	memcpy(g_cert_in.data, g_test_crt, sizeof(g_test_crt));
	g_cert_in.data_len = sizeof(g_test_crt);

	hal_result_e hres = HAL_FAIL;
	ST_EXPECT_EQ(SECLINK_OK, sl_set_certificate(g_hnd, SL_TEST_CERT_SLOT, &g_cert_in, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_cert_out, sizeof(g_test_crt)));

	ST_EXPECT_EQ(SECLINK_OK, sl_get_certificate(g_hnd, SL_TEST_CERT_SLOT, &g_cert_out, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(0, memcmp(g_cert_out.data, g_cert_in.data, g_cert_out.data_len));

	ST_EXPECT_EQ(SECLINK_OK, sl_remove_certificate(g_hnd, SL_TEST_CERT_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	sl_test_free_buffer(&g_cert_in);
	sl_test_free_buffer(&g_cert_out);

} END_TEST_F

static hal_data g_input;
static hal_data g_output;
static unsigned char *g_ss_data = NULL;
#define SS_SIZE 256
#define SS_SLOT 0
#define SL_TEST_START_INDEX 0
/*
 * Description: write secure storage
 */
START_TEST_F(write_storage)
{
	g_ss_data = (unsigned char *)malloc(SS_SIZE);
	memset(g_ss_data, 0x11, SS_SIZE);
	g_input.data = g_ss_data;
	g_input.data_len = SS_SIZE;

	hal_result_e hres = HAL_SUCCESS;
	ST_EXPECT_EQ(SECLINK_OK, sl_write_storage(g_hnd, SS_SLOT, &g_input, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(SECLINK_OK, sl_delete_storage(g_hnd, SS_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	free(g_ss_data);
} END_TEST_F

/*
 * Description: Read from storage
 */
START_TEST_F(read_storage)
{
	g_ss_data = (unsigned char *)malloc(SS_SIZE);
	memset(g_ss_data, 0x11, SS_SIZE);
	g_input.data = g_ss_data;
	g_input.data_len = SS_SIZE;

	hal_result_e hres = HAL_SUCCESS;
	ST_EXPECT_EQ(SECLINK_OK, sl_write_storage(g_hnd, SS_SLOT, &g_input, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_output, SS_SIZE));
	ST_EXPECT_EQ(SECLINK_OK, sl_read_storage(g_hnd, SS_SLOT, &g_output, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(SECLINK_OK, sl_delete_storage(g_hnd, SS_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	free(g_ss_data);
} END_TEST_F

/*
 * Description: Delete storage
 */
START_TEST_F(delete_storage)
{
	g_ss_data = (unsigned char *)malloc(SS_SIZE);
	memset(g_ss_data, 0x11, SS_SIZE);
	g_input.data = g_ss_data;
	g_input.data_len = SS_SIZE;

	hal_result_e hres = HAL_SUCCESS;
	ST_EXPECT_EQ(SECLINK_OK, sl_write_storage(g_hnd, SS_SLOT, &g_input, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	ST_EXPECT_EQ(SECLINK_OK, sl_delete_storage(g_hnd, SS_SLOT, &hres));
	ST_EXPECT_EQ(HAL_SUCCESS, hres);

	free(g_ss_data);
} END_TEST_F

void sl_simple_test(void)
{
	ST_SET_PACK(sl_simple);

	ST_TC_SET_GLOBAL(sl_simple, simple_tc);
	ST_TC_SET_SMOKE(sl_simple, REPEAT_COUNT, 0, "set key", key_testcase, set_sym_key);
	ST_TC_SET_SMOKE(sl_simple, REPEAT_COUNT, 0, "set key", key_testcase, set_asym_key);
	ST_TC_SET_SMOKE(sl_simple, REPEAT_COUNT, 0, "get key", key_testcase, get_key);
	ST_TC_SET_SMOKE(sl_simple, REPEAT_COUNT, 0, "generate key", key_testcase, gen_key);

	ST_SET_SMOKE1(sl_simple, 1, 0, "generate random", gen_random);

	ST_SET_SMOKE1(sl_simple, REPEAT_COUNT, 0, "write secure storage", write_storage);
	ST_SET_SMOKE1(sl_simple, REPEAT_COUNT, 0, "read secure storage", read_storage);
	ST_SET_SMOKE1(sl_simple, REPEAT_COUNT, 0, "delete secure storage", delete_storage);

	ST_SET_SMOKE1(sl_simple, REPEAT_COUNT, 0, "ecdsa sign", ecdsa_sign);
	ST_SET_SMOKE1(sl_simple, REPEAT_COUNT, 0, "ecdsa verify", ecdsa_verify);
	ST_SET_SMOKE1(sl_simple, REPEAT_COUNT, 0, "ecdh compute", ecdh_compute);
	ST_SET_SMOKE1(sl_simple, REPEAT_COUNT, 0, "set certificate", set_certificate);
	ST_SET_SMOKE1(sl_simple, REPEAT_COUNT, 0, "get certificate", get_certificate);

	ST_RUN_TEST(sl_simple);
	ST_RESULT_TEST(sl_simple);
}
