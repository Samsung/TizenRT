/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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

/*
 * Key
 * Injected key slot range 0~31
 * tp1x: 0~7
 * RAM key slot range 32~63
 * tp1x: 32~63
 */
#define RO_SLOT_SIZE 32
#define RO_VALID_RANGE 8
#define RW_SLOT_SIZE 32
#define RW_VALID_RANGE 32

#define SL_TEST_KEY_MEM_SIZE 1024
#define SL_TEST_KEY_TRIAL 1
#define SL_TEST_KEY_LIMIT_TIME 1000000

// ToDo: key length will be fixed. it doesn't consider real key size.
#define SL_TEST_SYMKEY_LEN 32
#define SL_TEST_PUBKEY_LEN 32
#define SL_TEST_PRIKEY_LEN 32

static sl_ctx g_hnd;

static int g_ro_slot_index[RO_SLOT_SIZE] = {0,};
static int g_ro_expect[RO_SLOT_SIZE] = {0,};

static int g_rw_slot_index[RW_SLOT_SIZE] = {0,};
static int g_rw_expect[RW_SLOT_SIZE] = {0,};

static hal_data g_aes_key_in;
static hal_data g_aes_key_out;

static hal_data g_pubkey_in;
static hal_data g_pubkey_out;

static hal_data g_prikey_in;
static hal_data g_prikey_out;


static void _sl_init_keytest(void)
{
	for (int i = 0; i < RO_SLOT_SIZE; ++i) {
		g_ro_slot_index[i] = i;
		if (i < RO_VALID_RANGE) {
			g_ro_expect[i] = HAL_SUCCESS;
		} else {
			g_ro_expect[i] = HAL_FAIL;
		}
	}

	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		g_rw_slot_index[i] = i + RO_SLOT_SIZE;
		if (i < RW_VALID_RANGE) {
			g_rw_expect[i] = HAL_SUCCESS;
		} else {
			g_rw_expect[i] = HAL_INVALID_SLOT_RANGE;
		}
	}

	/*  Symmetric key */
	int res = sl_test_malloc_buffer(&g_aes_key_in, SL_TEST_SYMKEY_LEN);
	if (res < 0) {
		SL_TEST_ERR("memory alloc error");
		return;
	}
	g_aes_key_in.data_len = SL_TEST_SYMKEY_LEN;
	memset(g_aes_key_in.data, 0xa5, SL_TEST_SYMKEY_LEN);

	res = sl_test_malloc_buffer(&g_aes_key_out, SL_TEST_KEY_MEM_SIZE);
	if (res < 0) {
		SL_TEST_ERR("memory alloc error");
		return;
	}
	g_aes_key_out.data_len = 0;
	g_aes_key_out.priv_len = 0;

	/*  public key */
	res = sl_test_malloc_buffer(&g_pubkey_in, SL_TEST_PUBKEY_LEN);
	if (res < 0) {
		SL_TEST_ERR("memory alloc error");
		return;
	}
	g_pubkey_in.data_len = SL_TEST_PUBKEY_LEN;
	memset(g_pubkey_in.data, 0xa5, SL_TEST_PUBKEY_LEN); // To Do: set valid key
	g_pubkey_in.priv_len = 0;

	res = sl_test_malloc_buffer(&g_pubkey_out, SL_TEST_PUBKEY_LEN);
	if (res < 0) {
		SL_TEST_ERR("memory alloc error");
		return;
	}
	g_pubkey_out.data_len = SL_TEST_PUBKEY_LEN;
	g_pubkey_out.priv_len = 0;

	/*  Private key */
	res = sl_test_malloc_buffer(&g_prikey_in, SL_TEST_PRIKEY_LEN);
	if (res < 0) {
		SL_TEST_ERR("memory alloc error");
		return;
	}
	g_prikey_in.data_len = SL_TEST_PRIKEY_LEN;
	memset(g_prikey_in.data, 0xa6, SL_TEST_PRIKEY_LEN); // To Do: set valid key
	g_prikey_in.priv_len = 0;

	res = sl_test_malloc_buffer_priv(&g_prikey_out, SL_TEST_PRIKEY_LEN);
	if (res < 0) {
		SL_TEST_ERR("memory alloc error");
		return;
	}
	g_prikey_out.priv_len = SL_TEST_PRIKEY_LEN;
	g_prikey_out.data_len = 0;

	res = sl_init(&g_hnd);
	if (res != SECLINK_OK) {
		printf("initialize error\n");
	}
}

static void _sl_deinit_keytest(void)
{
	sl_test_free_buffer(&g_aes_key_in);
	sl_test_free_buffer(&g_aes_key_out);

	sl_test_free_buffer(&g_pubkey_in);
	sl_test_free_buffer(&g_pubkey_out);

	sl_test_free_buffer(&g_prikey_in);
	sl_test_free_buffer(&g_prikey_out);

	int res = sl_deinit(g_hnd);
	if (res != SECLINK_OK) {
		printf("deinitialize error\n");
	}
}

/**
 * Description: Set symmetric key in RO area
 */
TEST_F(set_sym_key_ro)
{
	ST_START_TEST;

	for (int i = 0; i < RO_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_AES_256, g_ro_slot_index[i], &g_aes_key_in, NULL, &hres));
		ST_EXPECT_EQ(HAL_INVALID_REQUEST, hres);
	}

	ST_END_TEST;
}

/**
 * Description: Get symmetric key in RO area
 */
TEST_F(get_sym_key_ro)
{
	ST_START_TEST;

	// the result depends on what key type is stored in RO area. so modify return type latter
	for (int i = 0; i < RO_VALID_RANGE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_AES_256, g_ro_slot_index[i], &g_aes_key_out, &hres));
		ST_EXPECT_EQ(HAL_INVALID_REQUEST, hres);
	}

	for (int i = RO_VALID_RANGE; i < RO_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_AES_256, g_ro_slot_index[i], &g_aes_key_out, &hres));
		ST_EXPECT_EQ(HAL_INVALID_SLOT_RANGE, hres);
	}

	ST_END_TEST;
}

/**
 * Description: Remove symmetric key in RO area
 */
TEST_F(remove_sym_key_ro)
{
	ST_START_TEST;

	for (int i = 0; i < RO_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_AES_256, g_ro_slot_index[i], &hres));
		ST_EXPECT_EQ(HAL_INVALID_REQUEST, hres);
	}

	ST_END_TEST;
}

/**
 * Description: Generate a symmetric key in RO area
 */
TEST_F(generate_sym_key_ro)
{
	ST_START_TEST;

	for (int i = 0; i < RO_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_AES_256, g_ro_slot_index[i], &hres));
		ST_EXPECT_EQ(HAL_INVALID_REQUEST, hres);
	}

	ST_END_TEST;
}

/**
 * Description: Set public key in RO area
 */
TEST_F(set_public_key_ro)
{
	ST_START_TEST;

	for (int i = 0; i < RO_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_ro_slot_index[i], &g_pubkey_in, NULL, &hres));
		ST_EXPECT_EQ(HAL_INVALID_REQUEST, hres);
	}

	ST_END_TEST;
}

/**
 * Description: Get public key in RO area
 */
TEST_F(get_public_key_ro)
{
	ST_START_TEST;

	// the result depends on what key type is stored in RO area. so modify return type latter
	for (int i = 0; i < RO_VALID_RANGE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, g_ro_slot_index[i], &g_pubkey_out, &hres));
		ST_EXPECT_EQ(HAL_SUCCESS, hres);
	}

	for (int i = RO_VALID_RANGE; i < RO_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_ro_slot_index[i], &g_pubkey_out, &hres));
		ST_EXPECT_EQ(HAL_INVALID_SLOT_RANGE, hres);
	}

	ST_END_TEST;
}

/**
 * Description: Remove public key in RO area
 */
TEST_F(remove_public_key_ro)
{
	ST_START_TEST;

	for (int i = 0; i < RO_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_ro_slot_index[i], &hres));
		ST_EXPECT_EQ(HAL_INVALID_REQUEST, hres);
	}

	ST_END_TEST;
}

/**
 * Description: Generate a public key in RO area
 */

TEST_F(generate_public_key_ro)
{
	ST_START_TEST;

	for (int i = 0; i < RO_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_ro_slot_index[i], &hres));
		ST_EXPECT_EQ(HAL_INVALID_REQUEST, hres);
	}

	ST_END_TEST;
}

/**
 * Description: Set a private key in RO area
 */

TEST_F(set_private_key_ro)
{
	ST_START_TEST;

	for (int i = 0; i < RO_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_ro_slot_index[i], NULL, &g_prikey_in, &hres));
		ST_EXPECT_EQ(HAL_INVALID_REQUEST, hres);
	}

	ST_END_TEST;
}

/**
 * Description: Get a private key in RO area
 */
TEST_F(get_private_key_ro)
{
	ST_START_TEST;

	// the result depends on what key type is stored in RO area. so modify return type latter
	for (int i = 0; i < RO_VALID_RANGE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_ro_slot_index[i], &g_prikey_out, &hres));
		ST_EXPECT_EQ(HAL_INVALID_REQUEST, hres);
	}

	for (int i = RO_VALID_RANGE; i < RO_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_ro_slot_index[i], &g_prikey_out, &hres));
		ST_EXPECT_EQ(HAL_INVALID_SLOT_RANGE, hres);
	}

	ST_END_TEST;
}

/**
 * Description: Removt a private key in RO area
 */
TEST_F(remove_private_key_ro)
{
	ST_START_TEST;

	for (int i = 0; i < RO_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_ro_slot_index[i], &hres));
		ST_EXPECT_EQ(HAL_INVALID_REQUEST, hres);
	}

	ST_END_TEST;
}

/**
 * Description: Generate t a private key in RO area
 */

TEST_F(generate_private_key_ro)
{
	ST_START_TEST;

	for (int i = 0; i < RO_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_ro_slot_index[i], &hres));
		ST_EXPECT_EQ(HAL_INVALID_REQUEST, hres);
	}

	ST_END_TEST;
}

/**
 * Description: Set a symmetric key in RW area
 */
TEST_SETUP(set_sym_key_rw)
{
	ST_START_TEST;
	ST_END_TEST;
}

TEST_TEARDOWN(set_sym_key_rw)
{
	ST_START_TEST;

	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_AES_256, g_rw_slot_index[i], &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}

	ST_END_TEST;
}

TEST_F(set_sym_key_rw)
{
	ST_START_TEST;

	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_AES_256, g_rw_slot_index[i], &g_aes_key_in, NULL, &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}

	ST_END_TEST;
}

/**
 * Description: Get a symmetric key in RW area
 */
TEST_SETUP(get_sym_key_rw)
{
	ST_START_TEST;
	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_AES_256, g_rw_slot_index[i], &g_aes_key_in, NULL, &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}
	ST_END_TEST;
}

TEST_TEARDOWN(get_sym_key_rw)
{
	ST_START_TEST;
	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_AES_256, g_rw_slot_index[i], &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}

	ST_END_TEST;
}

TEST_F(get_sym_key_rw)
{
	ST_START_TEST;
	// it's symmetric key, so API should not return key value.
	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_AES_256, g_rw_slot_index[i], &g_aes_key_out, &hres));
		ST_EXPECT_EQ(HAL_INVALID_REQUEST, hres);
	}
	ST_END_TEST;
}

/**
 * Description: Remove a symmetric key in RW area
 */
TEST_SETUP(remove_sym_key_rw)
{
	ST_START_TEST;
	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_AES_256, g_rw_slot_index[i], &g_aes_key_in, NULL, &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}
	ST_END_TEST;
}

TEST_TEARDOWN(remove_sym_key_rw)
{
	ST_START_TEST;
	ST_END_TEST;
}

TEST_F(remove_sym_key_rw)
{
	ST_START_TEST;
	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_AES_256, g_rw_slot_index[i], &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}
	ST_END_TEST;
}

/**
 * Description: Generate a symmetric key in RW area
 */
TEST_SETUP(generate_sym_key_rw)
{
	ST_START_TEST;
	ST_END_TEST;
}

TEST_TEARDOWN(generate_sym_key_rw)
{
	ST_START_TEST;
	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_AES_256, g_rw_slot_index[i], &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}
	ST_END_TEST;
}

TEST_F(generate_sym_key_rw)
{
	ST_START_TEST;
	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_AES_256, g_rw_slot_index[i], &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}
	ST_END_TEST;
}

/**
 * Description: Set a public key in RW area
 */
TEST_SETUP(set_public_key_rw)
{
	ST_START_TEST;
	ST_END_TEST;
}

TEST_TEARDOWN(set_public_key_rw)
{
	ST_START_TEST;

	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_rw_slot_index[i], &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}

	ST_END_TEST;
}

TEST_F(set_public_key_rw)
{
	ST_START_TEST;

	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_rw_slot_index[i], &g_pubkey_in, NULL, &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}

	ST_END_TEST;
}

/**
 * Description: Get a public key in RW area
 */
TEST_SETUP(get_public_key_rw)
{
	ST_START_TEST;
	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_rw_slot_index[i], &g_pubkey_in, NULL, &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}
	ST_END_TEST;
}

TEST_TEARDOWN(get_public_key_rw)
{
	ST_START_TEST;
	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_rw_slot_index[i], &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}

	ST_END_TEST;
}

TEST_F(get_public_key_rw)
{
	ST_START_TEST;
	// it's symmetric key, so API should not return key value.
	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_rw_slot_index[i], &g_pubkey_out, &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}
	ST_END_TEST;
}

/**
 * Description: Remove a public key in RW area
 */
TEST_SETUP(remove_public_key_rw)
{
	ST_START_TEST;
	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_rw_slot_index[i], &g_pubkey_in, NULL, &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}
	ST_END_TEST;
}

TEST_TEARDOWN(remove_public_key_rw)
{
	ST_START_TEST;
	ST_END_TEST;
}

TEST_F(remove_public_key_rw)
{
	ST_START_TEST;
	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_rw_slot_index[i], &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}
	ST_END_TEST;
}

/**
 * Description: Generate a public key in RW area
 */
TEST_SETUP(generate_public_key_rw)
{
	ST_START_TEST;
	ST_END_TEST;
}

TEST_TEARDOWN(generate_public_key_rw)
{
	ST_START_TEST;
	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_rw_slot_index[i], &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}
	ST_END_TEST;
}

TEST_F(generate_public_key_rw)
{
	ST_START_TEST;
	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_rw_slot_index[i], &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}
	ST_END_TEST;
}

/**
 * Description: Set a private key in RW area
 */
TEST_SETUP(set_private_key_rw)
{
	ST_START_TEST;
	ST_END_TEST;
}

TEST_TEARDOWN(set_private_key_rw)
{
	ST_START_TEST;

	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_rw_slot_index[i], &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}

	ST_END_TEST;
}

TEST_F(set_private_key_rw)
{
	ST_START_TEST;

	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_rw_slot_index[i], NULL, &g_prikey_in, &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}

	ST_END_TEST;
}

/**
 * Description: Get a private key in RW area
 */
TEST_SETUP(get_private_key_rw)
{
	ST_START_TEST;
	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_rw_slot_index[i], NULL, &g_prikey_in, &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}
	ST_END_TEST;
}

TEST_TEARDOWN(get_private_key_rw)
{
	ST_START_TEST;
	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_rw_slot_index[i], &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}

	ST_END_TEST;
}

TEST_F(get_private_key_rw)
{
	ST_START_TEST;
	// it's symmetric key, so API should not return key value.
	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_rw_slot_index[i], &g_prikey_out, &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}
	ST_END_TEST;
}

/**
 * Description: Remove a private key in RW area
 */
TEST_SETUP(remove_private_key_rw)
{
	ST_START_TEST;
	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_rw_slot_index[i], NULL, &g_prikey_in, &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}
	ST_END_TEST;
}

TEST_TEARDOWN(remove_private_key_rw)
{
	ST_START_TEST;
	ST_END_TEST;
}

TEST_F(remove_private_key_rw)
{
	ST_START_TEST;
	for (int i = 0; i < RW_SLOT_SIZE; ++i) {
		hal_result_e hres = HAL_FAIL;
		ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_BRAINPOOL_P256R1, g_rw_slot_index[i], &hres));
		ST_EXPECT_EQ(g_rw_expect[i], hres);
	}
	ST_END_TEST;
}

void sl_keymgr_test(void)
{
	_sl_init_keytest();

	ST_SET_PACK(sl_keymgr);

	ST_SET_SMOKE1(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Set symmetric key in RO area", set_sym_key_ro);
	ST_SET_SMOKE1(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Get symmetric key in RO area", get_sym_key_ro);
	ST_SET_SMOKE1(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Remove symmetric key in RO area", remove_sym_key_ro);
	ST_SET_SMOKE1(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Generate symmetric key in RO area", generate_sym_key_ro);

	ST_SET_SMOKE1(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Set public key in RO area", set_public_key_ro);
	ST_SET_SMOKE1(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Get public key in RO area", get_public_key_ro);
	ST_SET_SMOKE1(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Remove public key in RO area", remove_public_key_ro);
	ST_SET_SMOKE1(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Generate public key in RO area", generate_public_key_ro);

	ST_SET_SMOKE1(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Set private key in RO area", set_private_key_ro);
	ST_SET_SMOKE1(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Get private key in RO area", get_private_key_ro);
	ST_SET_SMOKE1(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Remove private key in RO area", remove_private_key_ro);
	ST_SET_SMOKE1(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Generate private key in RO area", generate_private_key_ro);

	ST_SET_SMOKE(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Set symmetric key in RW area", set_sym_key_rw);
	ST_SET_SMOKE(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Get symmetric key in RW area", get_sym_key_rw);
	ST_SET_SMOKE(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Remove symmetric key in RW area", remove_sym_key_rw);
	ST_SET_SMOKE(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Generate symmetric key in RW area", generate_sym_key_rw);

	ST_SET_SMOKE(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Set public key in RW area", set_public_key_rw);
	ST_SET_SMOKE(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Get public key in RW area", get_public_key_rw);
	ST_SET_SMOKE(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Remove public key in RW area", remove_public_key_rw);
	ST_SET_SMOKE(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Generate public key in RW area", generate_public_key_rw);

	ST_SET_SMOKE(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Set private key in RW area", set_private_key_rw);
	ST_SET_SMOKE(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Get private key in RW area", get_private_key_rw);
	ST_SET_SMOKE(sl_keymgr, SL_TEST_KEY_TRIAL, SL_TEST_KEY_LIMIT_TIME, "Remove private key in RW area", remove_private_key_rw);

	ST_RUN_TEST(sl_keymgr);
	ST_RESULT_TEST(sl_keymgr);

	_sl_deinit_keytest();
}
