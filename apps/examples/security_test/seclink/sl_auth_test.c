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
#include <tinyara/security_hal.h>
#include <stress_tool/st_perf.h>
#include "sl_test.h"
#include "sl_sample_key.h"
#include "sl_test_usage.h"

static char *g_command[] = {
#ifdef SL_AUTH_TEST_POOL
#undef SL_AUTH_TEST_POOL
#endif
#define SL_AUTH_TEST_POOL(command, type, func) command,
#include "sl_auth_table.h"
};

#ifdef SL_AUTH_TEST_POOL
#undef SL_AUTH_TEST_POOL
#endif
#define SL_AUTH_TEST_POOL(command, type, func) \
	extern void func(sl_options *opt);
#include "sl_auth_table.h"

static sl_test_func g_func_list[] = {
#ifdef SL_AUTH_TEST_POOL
#undef SL_AUTH_TEST_POOL
#endif
#define SL_AUTH_TEST_POOL(command, type, func) func,
#include "sl_auth_table.h"
};

typedef enum {
#ifdef SL_AUTH_TEST_POOL
#undef SL_AUTH_TEST_POOL
#endif
#define SL_AUTH_TEST_POOL(command, type, func) type,
#include "sl_auth_table.h"
	SL_AUTH_TYPE_MAX,
	SL_AUTH_TYPE_ERR = -1
} sl_auth_type_e;

#define SL_TEST_ECC_KEY_SLOT 32
#define RW_SLOT_ENTRY 32
#define SL_TEST_ECDH_KEY_SLOT_A 32
#define SL_TEST_ECDH_KEY_SLOT_B 33
#define SL_TEST_ECC_HASH_LEN 32
#define SL_TEST_AUTH_MEM_SIZE 4096
#define SL_TEST_AUTH_MEM_SIZE 4096
#define SL_TEST_RAND_LEN 128
#define SL_TEST_CERT_SLOT 32

static sl_ctx g_hnd;
static hal_data g_key_a = HAL_DATA_INITIALIZER;
static hal_data g_key_b = HAL_DATA_INITIALIZER;
static hal_data g_pubkey = HAL_DATA_INITIALIZER;
static hal_data g_privkey = HAL_DATA_INITIALIZER;
static hal_data g_shared_secret_a = HAL_DATA_INITIALIZER;
static hal_data g_shared_secret_b = HAL_DATA_INITIALIZER;
static hal_ecdh_data g_ecdh_a;
static hal_ecdh_data g_ecdh_b;
static hal_data g_ecdsa_signature = HAL_DATA_INITIALIZER;
static hal_data g_ecdsa_hash = HAL_DATA_INITIALIZER;
static hal_ecdsa_mode g_ecdsa_mode;
static hal_data g_rand = HAL_DATA_INITIALIZER;
static hal_data g_cert_in = HAL_DATA_INITIALIZER;
static hal_data g_cert_out = HAL_DATA_INITIALIZER;

ST_SET_PACK_GLOBAL(sl_auth);

START_TEST_F(gen_random)
{
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_rand, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(SECLINK_OK, sl_generate_random(g_hnd, SL_TEST_RAND_LEN, &g_rand));
	sl_test_free_buffer(&g_rand);
}
END_TEST_F

/*
 * Desc: Save certificate in secure storage
 */
TESTCASE_SETUP(set_certificate)
{
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_cert_in, sizeof(g_test_crt)));
	memcpy(g_cert_in.data, g_test_crt, sizeof(g_test_crt));
	g_cert_in.data_len = sizeof(g_test_crt);
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_cert_out, sizeof(g_test_crt)));
}
END_TEST_F

TESTCASE_TEARDOWN(set_certificate)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_certificate(g_hnd, SL_TEST_CERT_SLOT));
	sl_test_free_buffer(&g_cert_in);
	sl_test_free_buffer(&g_cert_out);
}
END_TEST_F

START_TEST_F(set_certificate)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_set_certificate(g_hnd, SL_TEST_CERT_SLOT, &g_cert_in));
}
END_TEST_F

/*
 * Desc: Load certificate in secure storage
 */
TESTCASE_SETUP(get_certificate)
{
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_cert_in, sizeof(g_test_crt)));
	memcpy(g_cert_in.data, g_test_crt, sizeof(g_test_crt));
	g_cert_in.data_len = sizeof(g_test_crt);

	ST_EXPECT_EQ(SECLINK_OK, sl_set_certificate(g_hnd, SL_TEST_CERT_SLOT, &g_cert_in));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_cert_out, sizeof(g_test_crt)));
}
END_TEST_F

TESTCASE_TEARDOWN(get_certificate)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_certificate(g_hnd, SL_TEST_CERT_SLOT));
	sl_test_free_buffer(&g_cert_in);
	sl_test_free_buffer(&g_cert_out);
}
END_TEST_F

START_TEST_F(get_certificate)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_get_certificate(g_hnd, SL_TEST_CERT_SLOT, &g_cert_out));
	ST_EXPECT_EQ(0, memcmp(g_cert_out.data, g_cert_in.data, g_cert_out.data_len));
}
END_TEST_F

/*
 * Desc: Delete certificate in secure storage
 */
TESTCASE_SETUP(remove_certificate)
{
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_cert_in, sizeof(g_test_crt)));
	memcpy(g_cert_in.data, g_test_crt, sizeof(g_test_crt));
	g_cert_in.data_len = sizeof(g_test_crt);

	ST_EXPECT_EQ(SECLINK_OK, sl_set_certificate(g_hnd, SL_TEST_CERT_SLOT, &g_cert_in));
}
END_TEST_F

TESTCASE_TEARDOWN(remove_certificate)
{
	sl_test_free_buffer(&g_cert_in);
}
END_TEST_F

START_TEST_F(remove_certificate)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_certificate(g_hnd, SL_TEST_CERT_SLOT));
}
END_TEST_F

TESTCASE_SETUP(x25519_compute)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_ECC_25519, SL_TEST_ECDH_KEY_SLOT_A));

	ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_ECC_25519, SL_TEST_ECDH_KEY_SLOT_B));

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_key_a, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer_priv(&g_key_a, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_key_b, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer_priv(&g_key_b, SL_TEST_AUTH_MEM_SIZE));

	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_25519, SL_TEST_ECDH_KEY_SLOT_A, &g_key_a));

	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_25519, SL_TEST_ECDH_KEY_SLOT_B, &g_key_b));

	g_ecdh_a.pubkey_x = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_ecdh_a.pubkey_x);
	g_ecdh_a.pubkey_y = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_ecdh_a.pubkey_y);
	g_ecdh_b.pubkey_x = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_ecdh_b.pubkey_x);
	g_ecdh_b.pubkey_y = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_ecdh_b.pubkey_y);

	//data: pubkey_x, priv: pubkey_y
	g_ecdh_a.curve = HAL_ECDSA_CURVE_25519;
	g_ecdh_a.pubkey_x->data = g_key_a.data;
	g_ecdh_a.pubkey_x->data_len = g_key_a.data_len;
	g_ecdh_a.pubkey_y->data = g_key_a.priv;
	g_ecdh_a.pubkey_y->data_len = g_key_a.priv_len;

	//data: pubkey_x, priv: pubkey_y
	g_ecdh_b.curve = HAL_ECDSA_CURVE_25519;
	g_ecdh_b.pubkey_x->data = g_key_b.data;
	g_ecdh_b.pubkey_x->data_len = g_key_b.data_len;
	g_ecdh_b.pubkey_y->data = g_key_b.priv;
	g_ecdh_b.pubkey_y->data_len = g_key_b.priv_len;

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_shared_secret_a, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_shared_secret_b, SL_TEST_AUTH_MEM_SIZE));
}
END_TEST_F

TESTCASE_TEARDOWN(x25519_compute)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_25519, SL_TEST_ECDH_KEY_SLOT_A));

	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_25519, SL_TEST_ECDH_KEY_SLOT_B));

	sl_test_free_buffer(&g_key_a);
	sl_test_free_buffer(&g_key_b);
	free(g_ecdh_a.pubkey_x);
	free(g_ecdh_a.pubkey_y);
	free(g_ecdh_b.pubkey_x);
	free(g_ecdh_b.pubkey_y);
	sl_test_free_buffer(&g_shared_secret_a);
	sl_test_free_buffer(&g_shared_secret_b);
}
END_TEST_F

START_TEST_F(x25519_compute)
{
	// A<--B, B<--A
	if (g_ecdh_a.pubkey_x != NULL && g_ecdh_b.pubkey_x != NULL) {
		ST_EXPECT_EQ(SECLINK_OK, sl_ecdh_compute_shared_secret(g_hnd, &g_ecdh_b, SL_TEST_ECDH_KEY_SLOT_A, &g_shared_secret_a));

		ST_EXPECT_EQ(SECLINK_OK, sl_ecdh_compute_shared_secret(g_hnd, &g_ecdh_a, SL_TEST_ECDH_KEY_SLOT_B, &g_shared_secret_b));

		ST_EXPECT_EQ(g_shared_secret_a.data_len, g_shared_secret_b.data_len);
		ST_EXPECT_EQ(0, memcmp(g_shared_secret_a.data, g_shared_secret_b.data, g_shared_secret_a.data_len));
	}
}
END_TEST_F

TESTCASE_SETUP(x25519_setkey_compute)
{
	g_pubkey.data = g_ecc_pubkey;
	g_pubkey.data_len = sizeof(g_ecc_pubkey);
	g_pubkey.priv = NULL;
	g_pubkey.priv_len = 0;
	g_privkey.data = g_ecc_privkey;
	g_privkey.data_len = sizeof(g_ecc_privkey);
	g_privkey.priv = NULL;
	g_privkey.priv_len = 0;

	ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_A, &g_pubkey, &g_privkey));

	ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_B));

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_key_a, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer_priv(&g_key_a, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_key_b, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer_priv(&g_key_b, SL_TEST_AUTH_MEM_SIZE));

	printf("before key a %d %d\n", g_key_a.data_len, g_key_a.priv_len);
	printf("before key b %d %d\n", g_key_b.data_len, g_key_b.priv_len);

	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_A, &g_key_a));

	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_B, &g_key_b));

	g_ecdh_a.pubkey_x = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_ecdh_a.pubkey_x);
	g_ecdh_a.pubkey_y = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_ecdh_a.pubkey_y);
	g_ecdh_b.pubkey_x = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_ecdh_b.pubkey_x);
	g_ecdh_b.pubkey_y = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_ecdh_b.pubkey_y);

	//data: pubkey_x, priv: pubkey_y
	g_ecdh_a.curve = HAL_ECDSA_SEC_P256R1;
	g_ecdh_a.pubkey_x->data = g_key_a.data;
	g_ecdh_a.pubkey_x->data_len = g_key_a.data_len;
	g_ecdh_a.pubkey_y->data = g_key_a.priv;
	g_ecdh_a.pubkey_y->data_len = g_key_a.priv_len;

	printf("key a %d %d\n", g_key_a.data_len, g_key_a.priv_len);
	printf("key b %d %d\n", g_key_b.data_len, g_key_b.priv_len);
	sl_test_print_buffer(g_key_a.data, g_key_a.data_len, "key A pub");
	sl_test_print_buffer(g_key_a.priv, g_key_a.priv_len, "key A priv");
	sl_test_print_buffer(g_key_b.data, g_key_b.data_len, "key B pub");
	sl_test_print_buffer(g_key_b.priv, g_key_b.priv_len, "key B priv");

	//data: pubkey_x, priv: pubkey_y
	g_ecdh_b.curve = HAL_ECDSA_SEC_P256R1;
	g_ecdh_b.pubkey_x->data = g_key_b.data;
	g_ecdh_b.pubkey_x->data_len = g_key_b.data_len;
	g_ecdh_b.pubkey_y->data = g_key_b.priv;
	g_ecdh_b.pubkey_y->data_len = g_key_b.priv_len;

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_shared_secret_a, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_shared_secret_b, SL_TEST_AUTH_MEM_SIZE));
}
END_TEST_F

TESTCASE_TEARDOWN(x25519_setkey_compute)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_A));
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_B));

	sl_test_free_buffer(&g_key_a);
	sl_test_free_buffer(&g_key_b);
	free(g_ecdh_a.pubkey_x);
	free(g_ecdh_a.pubkey_y);
	free(g_ecdh_b.pubkey_x);
	free(g_ecdh_b.pubkey_y);
	sl_test_free_buffer(&g_shared_secret_a);
	sl_test_free_buffer(&g_shared_secret_b);
}
END_TEST_F

START_TEST_F(x25519_setkey_compute)
{
	// A<--B, B<--A
	if (g_ecdh_a.pubkey_x != NULL && g_ecdh_b.pubkey_x != NULL) {
		ST_EXPECT_EQ(SECLINK_OK, sl_ecdh_compute_shared_secret(g_hnd, &g_ecdh_a, SL_TEST_ECDH_KEY_SLOT_B, &g_shared_secret_b));
		ST_EXPECT_EQ(SECLINK_OK, sl_ecdh_compute_shared_secret(g_hnd, &g_ecdh_b, SL_TEST_ECDH_KEY_SLOT_A, &g_shared_secret_a));

		ST_EXPECT_EQ(g_shared_secret_a.data_len, g_shared_secret_b.data_len);
		ST_EXPECT_EQ(0, memcmp(g_shared_secret_a.data, g_shared_secret_b.data, g_shared_secret_a.data_len));
	}
}
END_TEST_F

TESTCASE_SETUP(ecdh_compute)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_A));

	ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_B));

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_key_a, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer_priv(&g_key_a, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_key_b, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer_priv(&g_key_b, SL_TEST_AUTH_MEM_SIZE));

	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_A, &g_key_a));

	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_B, &g_key_b));

	g_ecdh_a.pubkey_x = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_ecdh_a.pubkey_x);
	g_ecdh_a.pubkey_y = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_ecdh_a.pubkey_y);
	g_ecdh_b.pubkey_x = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_ecdh_b.pubkey_x);
	g_ecdh_b.pubkey_y = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_ecdh_b.pubkey_y);

	//data: pubkey_x, priv: pubkey_y
	g_ecdh_a.curve = HAL_ECDSA_SEC_P256R1;
	g_ecdh_a.pubkey_x->data = g_key_a.data;
	g_ecdh_a.pubkey_x->data_len = g_key_a.data_len;
	g_ecdh_a.pubkey_y->data = g_key_a.priv;
	g_ecdh_a.pubkey_y->data_len = g_key_a.priv_len;

	//data: pubkey_x, priv: pubkey_y
	g_ecdh_b.curve = HAL_ECDSA_SEC_P256R1;
	g_ecdh_b.pubkey_x->data = g_key_b.data;
	g_ecdh_b.pubkey_x->data_len = g_key_b.data_len;
	g_ecdh_b.pubkey_y->data = g_key_b.priv;
	g_ecdh_b.pubkey_y->data_len = g_key_b.priv_len;

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_shared_secret_a, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_shared_secret_b, SL_TEST_AUTH_MEM_SIZE));
}
END_TEST_F

TESTCASE_TEARDOWN(ecdh_compute)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_A));

	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_B));

	sl_test_free_buffer(&g_key_a);
	sl_test_free_buffer(&g_key_b);
	free(g_ecdh_a.pubkey_x);
	free(g_ecdh_a.pubkey_y);
	free(g_ecdh_b.pubkey_x);
	free(g_ecdh_b.pubkey_y);
	sl_test_free_buffer(&g_shared_secret_a);
	sl_test_free_buffer(&g_shared_secret_b);
}
END_TEST_F

START_TEST_F(ecdh_compute)
{
	// A<--B, B<--A
	if (g_ecdh_a.pubkey_x != NULL && g_ecdh_b.pubkey_x != NULL) {
		ST_EXPECT_EQ(SECLINK_OK, sl_ecdh_compute_shared_secret(g_hnd, &g_ecdh_b, SL_TEST_ECDH_KEY_SLOT_A, &g_shared_secret_a));

		ST_EXPECT_EQ(SECLINK_OK, sl_ecdh_compute_shared_secret(g_hnd, &g_ecdh_a, SL_TEST_ECDH_KEY_SLOT_B, &g_shared_secret_b));

		ST_EXPECT_EQ(g_shared_secret_a.data_len, g_shared_secret_b.data_len);
		ST_EXPECT_EQ(0, memcmp(g_shared_secret_a.data, g_shared_secret_b.data, g_shared_secret_a.data_len));
	}
}
END_TEST_F

TESTCASE_SETUP(ecdh_setkey_compute)
{
	g_pubkey.data = g_ecc_pubkey;
	g_pubkey.data_len = sizeof(g_ecc_pubkey);
	g_pubkey.priv = NULL;
	g_pubkey.priv_len = 0;
	g_privkey.data = g_ecc_privkey;
	g_privkey.data_len = sizeof(g_ecc_privkey);
	g_privkey.priv = NULL;
	g_privkey.priv_len = 0;

	ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_A, &g_pubkey, &g_privkey));

	ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_B));

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_key_a, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer_priv(&g_key_a, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_key_b, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer_priv(&g_key_b, SL_TEST_AUTH_MEM_SIZE));

	printf("before key a %d %d\n", g_key_a.data_len, g_key_a.priv_len);
	printf("before key b %d %d\n", g_key_b.data_len, g_key_b.priv_len);

	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_A, &g_key_a));

	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_B, &g_key_b));

	g_ecdh_a.pubkey_x = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_ecdh_a.pubkey_x);
	g_ecdh_a.pubkey_y = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_ecdh_a.pubkey_y);
	g_ecdh_b.pubkey_x = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_ecdh_b.pubkey_x);
	g_ecdh_b.pubkey_y = (hal_data *)zalloc(sizeof(hal_data));
	ST_EXPECT_NEQ(NULL, g_ecdh_b.pubkey_y);

	//data: pubkey_x, priv: pubkey_y
	g_ecdh_a.curve = HAL_ECDSA_SEC_P256R1;
	g_ecdh_a.pubkey_x->data = g_key_a.data;
	g_ecdh_a.pubkey_x->data_len = g_key_a.data_len;
	g_ecdh_a.pubkey_y->data = g_key_a.priv;
	g_ecdh_a.pubkey_y->data_len = g_key_a.priv_len;

	printf("key a %d %d\n", g_key_a.data_len, g_key_a.priv_len);
	printf("key b %d %d\n", g_key_b.data_len, g_key_b.priv_len);
	sl_test_print_buffer(g_key_a.data, g_key_a.data_len, "key A pub");
	sl_test_print_buffer(g_key_a.priv, g_key_a.priv_len, "key A priv");
	sl_test_print_buffer(g_key_b.data, g_key_b.data_len, "key B pub");
	sl_test_print_buffer(g_key_b.priv, g_key_b.priv_len, "key B priv");

	//data: pubkey_x, priv: pubkey_y
	g_ecdh_b.curve = HAL_ECDSA_SEC_P256R1;
	g_ecdh_b.pubkey_x->data = g_key_b.data;
	g_ecdh_b.pubkey_x->data_len = g_key_b.data_len;
	g_ecdh_b.pubkey_y->data = g_key_b.priv;
	g_ecdh_b.pubkey_y->data_len = g_key_b.priv_len;

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_shared_secret_a, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_shared_secret_b, SL_TEST_AUTH_MEM_SIZE));
}
END_TEST_F

TESTCASE_TEARDOWN(ecdh_setkey_compute)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_A));
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECDH_KEY_SLOT_B));

	sl_test_free_buffer(&g_key_a);
	sl_test_free_buffer(&g_key_b);
	free(g_ecdh_a.pubkey_x);
	free(g_ecdh_a.pubkey_y);
	free(g_ecdh_b.pubkey_x);
	free(g_ecdh_b.pubkey_y);
	sl_test_free_buffer(&g_shared_secret_a);
	sl_test_free_buffer(&g_shared_secret_b);
}
END_TEST_F

START_TEST_F(ecdh_setkey_compute)
{
	// A<--B, B<--A
	if (g_ecdh_a.pubkey_x != NULL && g_ecdh_b.pubkey_x != NULL) {
		ST_EXPECT_EQ(SECLINK_OK, sl_ecdh_compute_shared_secret(g_hnd, &g_ecdh_a, SL_TEST_ECDH_KEY_SLOT_B, &g_shared_secret_b));
		ST_EXPECT_EQ(SECLINK_OK, sl_ecdh_compute_shared_secret(g_hnd, &g_ecdh_b, SL_TEST_ECDH_KEY_SLOT_A, &g_shared_secret_a));

		ST_EXPECT_EQ(g_shared_secret_a.data_len, g_shared_secret_b.data_len);
		ST_EXPECT_EQ(0, memcmp(g_shared_secret_a.data, g_shared_secret_b.data, g_shared_secret_a.data_len));
	}
}
END_TEST_F

TESTCASE_SETUP(ecdsa_sign)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECC_KEY_SLOT));

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_ecdsa_signature, SL_TEST_AUTH_MEM_SIZE));

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_ecdsa_hash, SL_TEST_ECC_HASH_LEN));
	g_ecdsa_hash.data_len = SL_TEST_ECC_HASH_LEN;
	g_ecdsa_mode.hash_t = HAL_HASH_SHA256;
}
END_TEST_F

TESTCASE_TEARDOWN(ecdsa_sign)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECC_KEY_SLOT));

	sl_test_free_buffer(&g_ecdsa_hash);
	sl_test_free_buffer(&g_ecdsa_signature);
}
END_TEST_F

START_TEST_F(ecdsa_sign)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_ecdsa_sign_md(g_hnd, g_ecdsa_mode,
											  &g_ecdsa_hash,
											  SL_TEST_ECC_KEY_SLOT,
											  &g_ecdsa_signature));
}
END_TEST_F

START_TEST_F(ecdsa_verify)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECC_KEY_SLOT));

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_ecdsa_signature, SL_TEST_AUTH_MEM_SIZE));

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_ecdsa_hash, SL_TEST_ECC_HASH_LEN));
	g_ecdsa_hash.data_len = SL_TEST_ECC_HASH_LEN;
	memset(g_ecdsa_hash.data, 0xa5, SL_TEST_ECC_HASH_LEN);

	g_ecdsa_mode.curve = HAL_ECDSA_SEC_P256R1;
	g_ecdsa_mode.hash_t = HAL_HASH_SHA256;

	ST_EXPECT_EQ(SECLINK_OK, sl_ecdsa_sign_md(g_hnd, g_ecdsa_mode, &g_ecdsa_hash, SL_TEST_ECC_KEY_SLOT, &g_ecdsa_signature));

	ST_EXPECT_EQ(SECLINK_OK, sl_ecdsa_verify_md(g_hnd, g_ecdsa_mode, &g_ecdsa_hash, &g_ecdsa_signature, SL_TEST_ECC_KEY_SLOT));

	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_SEC_P256R1, SL_TEST_ECC_KEY_SLOT));

	sl_test_print_buffer(g_ecdsa_hash.data, g_ecdsa_hash.data_len, "ECDSA hash");
	sl_test_print_buffer(g_ecdsa_signature.data, g_ecdsa_signature.data_len, "ECDSA hash");

	sl_test_free_buffer(&g_ecdsa_hash);
	sl_test_free_buffer(&g_ecdsa_signature);
}
END_TEST_F

TESTCASE_SETUP(ed25519_sign)
{
	hal_data pub_key = {&g_ed25519_pubkey, sizeof(g_ed25519_pubkey), NULL, 0};
	hal_data priv_key = {&g_ed25519_privkey_only, sizeof(g_ed25519_privkey_only), NULL, 0};

	ST_EXPECT_EQ(0, sl_set_key(g_hnd, HAL_KEY_ED_25519, RW_SLOT_ENTRY, &pub_key, &priv_key));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_ecdsa_signature, SL_TEST_AUTH_MEM_SIZE));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_ecdsa_hash, SL_TEST_ECC_HASH_LEN));
	g_ecdsa_hash.data_len = SL_TEST_ECC_HASH_LEN;
	memset(g_ecdsa_hash.data, 0xa5, SL_TEST_ECC_HASH_LEN);

	g_ecdsa_mode.curve = HAL_ECDSA_CURVE_25519;
	g_ecdsa_mode.hash_t = HAL_HASH_SHA512;
}
END_TEST_F

TESTCASE_TEARDOWN(ed25519_sign)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ED_25519, RW_SLOT_ENTRY));

	sl_test_free_buffer(&g_ecdsa_hash);
	sl_test_free_buffer(&g_ecdsa_signature);
}
END_TEST_F

START_TEST_F(ed25519_sign)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_ecdsa_sign_md(g_hnd, g_ecdsa_mode, &g_ecdsa_hash, RW_SLOT_ENTRY, &g_ecdsa_signature));
}
END_TEST_F

TESTCASE_SETUP(ed25519_verify)
{
	hal_data pub_key = {&g_ed25519_pubkey, sizeof(g_ed25519_pubkey), NULL, 0};
	hal_data priv_key = {&g_ed25519_privkey_only, sizeof(g_ed25519_privkey_only), NULL, 0};

	ST_EXPECT_EQ(0, sl_set_key(g_hnd, HAL_KEY_ED_25519, RW_SLOT_ENTRY, &pub_key, &priv_key));

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_ecdsa_signature, SL_TEST_AUTH_MEM_SIZE));

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_ecdsa_hash, SL_TEST_ECC_HASH_LEN));
	g_ecdsa_hash.data_len = SL_TEST_ECC_HASH_LEN;
	memset(g_ecdsa_hash.data, 0xa5, SL_TEST_ECC_HASH_LEN);

	g_ecdsa_mode.curve = HAL_ECDSA_CURVE_25519;
	g_ecdsa_mode.hash_t = HAL_HASH_SHA512;

	ST_EXPECT_EQ(SECLINK_OK, sl_ecdsa_sign_md(g_hnd, g_ecdsa_mode, &g_ecdsa_hash, RW_SLOT_ENTRY, &g_ecdsa_signature));
}
END_TEST_F

TESTCASE_TEARDOWN(ed25519_verify)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ED_25519, RW_SLOT_ENTRY));

	sl_test_print_buffer(g_ecdsa_hash.data, g_ecdsa_hash.data_len,
						 "ed25519 hash");
	sl_test_print_buffer(g_ecdsa_signature.data, g_ecdsa_signature.data_len,
						 "ed25519_signature");

	sl_test_free_buffer(&g_ecdsa_hash);
	sl_test_free_buffer(&g_ecdsa_signature);
}
END_TEST_F

START_TEST_F(ed25519_verify)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_ecdsa_verify_md(g_hnd, g_ecdsa_mode, &g_ecdsa_hash, &g_ecdsa_signature, RW_SLOT_ENTRY));
}
END_TEST_F

TESTCASE_SETUP(sl_auth_global)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_init(&g_hnd));
}
END_TESTCASE

TESTCASE_TEARDOWN(sl_auth_global)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_deinit(g_hnd));
}
END_TESTCASE

void sl_handle_auth_ecdh(sl_options *opt)
{
	ST_SET_SMOKE(sl_auth, opt->count, 0, "ecdh compute", ecdh_compute);
}

void sl_handle_auth_setkey_ecdh(sl_options *opt)
{
	ST_SET_SMOKE(sl_auth, opt->count, 0, "ecdh compute with setkey", ecdh_setkey_compute);
}

void sl_handle_auth_x25519(sl_options *opt)
{
	ST_SET_SMOKE(sl_auth, opt->count, 0, "ecdh compute", x25519_compute);
}

void sl_handle_auth_setkey_x25519(sl_options *opt)
{
	ST_SET_SMOKE(sl_auth, opt->count, 0, "x25519 setkey compute", x25519_setkey_compute);
}

void sl_handle_auth_ecdsa_signature(sl_options *opt)
{
	ST_SET_SMOKE(sl_auth, opt->count, 0, "ecdsa sign", ecdsa_sign);
}

void sl_handle_auth_ecdsa_verify(sl_options *opt)
{
	ST_SET_SMOKE1(sl_auth, opt->count, 0, "ecdsa verify", ecdsa_verify);
}

void sl_handle_auth_ed25519_signature(sl_options *opt)
{
	ST_SET_SMOKE(sl_auth, opt->count, 0, "ED25519 signauture", ed25519_sign);
}

void sl_handle_auth_ed25519_verify(sl_options *opt)
{
	ST_SET_SMOKE(sl_auth, opt->count, 0, "ED25519 verify", ed25519_verify);
}

void sl_handle_auth_gen_random(sl_options *opt)
{
	ST_SET_SMOKE1(sl_auth, opt->count, 0, "Generate random", gen_random);
}

void sl_handle_auth_set_cert(sl_options *opt)
{
	ST_SET_SMOKE(sl_auth, opt->count, 0, "Set certificate", set_certificate);
}

void sl_handle_auth_get_cert(sl_options *opt)
{
	ST_SET_SMOKE(sl_auth, opt->count, 0, "Get certificate", get_certificate);
}

void sl_handle_auth_remove_cert(sl_options *opt)
{
	ST_SET_SMOKE(sl_auth, opt->count, 0, "Remove certificate", remove_certificate);
}

void sl_handle_auth(sl_options *opt)
{
	ST_TC_SET_GLOBAL(sl_auth, sl_auth_global);

	SL_PARSE_MESSAGE(opt, g_command, sl_auth_type_e,
					 g_func_list, SL_AUTH_TYPE_MAX, SL_AUTH_TYPE_ERR);
	ST_RUN_TEST(sl_auth);
	ST_RESULT_TEST(sl_auth);
}
