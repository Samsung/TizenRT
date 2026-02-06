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
#include <tinyara/seclink.h>
#include <tinyara/security_hal.h>
#include <tinyara/seclink_drv.h>
#include <stress_tool/st_perf.h>
#include "sl_test.h"
#include "sl_sample_key.h"
#include "sl_test_usage.h"

static char *g_command[] = {
#ifdef SL_KEY_TEST_POOL
#undef SL_KEY_TEST_POOL
#endif
#define SL_KEY_TEST_POOL(command, type, func) command,
#include "sl_key_table.h"
};

#ifdef SL_KEY_TEST_POOL
#undef SL_KEY_TEST_POOL
#endif
#define SL_KEY_TEST_POOL(command, type, func) \
	void func(sl_options *opt);
#include "sl_key_table.h"

static sl_test_func g_func_list[] = {
#ifdef SL_KEY_TEST_POOL
#undef SL_KEY_TEST_POOL
#endif
#define SL_KEY_TEST_POOL(command, type, func) func,
#include "sl_key_table.h"
};

typedef enum {
#ifdef SL_KEY_TEST_POOL
#undef SL_KEY_TEST_POOL
#endif
#define SL_KEY_TEST_POOL(command, type, func) type,
#include "sl_key_table.h"
	SL_KEY_TYPE_MAX,
	SL_KEY_TYPE_ERR = -1
} sl_key_type_e;

#define SL_RW_KEY_SLOT 32
#define SL_TEST_KEY_LEN 256
#define SL_TEST_SYM_KEY_LEN 32
static sl_ctx g_hnd;
static hal_data g_sym_key_in = HAL_DATA_INITIALIZER;
static hal_data g_asym_pubkey_in = HAL_DATA_INITIALIZER;
static hal_data g_asym_prikey_in = HAL_DATA_INITIALIZER;
static hal_data g_ecc_pubkey_in = HAL_DATA_INITIALIZER;
static hal_data g_ecc_prikey_in = HAL_DATA_INITIALIZER;
static hal_data g_key_out = HAL_DATA_INITIALIZER;

ST_SET_PACK_GLOBAL(sl_key);

TESTCASE_SETUP(sl_key_global)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_init(&g_hnd));
}
END_TESTCASE

TESTCASE_TEARDOWN(sl_key_global)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_deinit(g_hnd));
}
END_TESTCASE

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

	g_ecc_pubkey_in.data = g_ecc_pubkey;
	g_ecc_pubkey_in.data_len = sizeof(g_ecc_pubkey);
	g_ecc_pubkey_in.priv = NULL;
	g_ecc_pubkey_in.priv_len = 0;

	g_ecc_prikey_in.data = g_ecc_privkey;
	g_ecc_prikey_in.data_len = sizeof(g_ecc_privkey);
	g_ecc_prikey_in.priv = NULL;
	g_ecc_prikey_in.priv_len = 0;

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_sym_key_in, SL_TEST_SYM_KEY_LEN));
	g_sym_key_in.priv = NULL;
	g_sym_key_in.priv_len = 0;

	ST_EXPECT_EQ(0, sl_test_malloc_buffer(&g_key_out, SL_TEST_KEY_LEN));
	ST_EXPECT_EQ(0, sl_test_malloc_buffer_priv(&g_key_out, SL_TEST_KEY_LEN));
}
END_TESTCASE

TESTCASE_TEARDOWN(key_testcase)
{
	sl_test_free_buffer(&g_sym_key_in);
	sl_test_free_buffer(&g_key_out);
}
END_TESTCASE

/*
 * Desc: set asymmetric key
 */
static char key_a_buf[1024];
static char key_b_buf[1024];

START_TEST_F(set_ed25519_key)
{
	ST_ASSERT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_ED_25519,
										SL_RW_KEY_SLOT, &g_asym_pubkey_in, &g_asym_prikey_in));

	hal_data key_a = HAL_DATA_INITIALIZER;
  key_a.data = &key_a_buf;
  key_a.data_len = 1024;
  key_a.priv = &key_b_buf;
  key_a.priv_len = 1024;

	ST_EXPECT_EQ(SECLINK_OK, sl_get_key(g_hnd, HAL_KEY_ED_25519, SL_RW_KEY_SLOT, &key_a));
	if (key_a.data_len > 0) {
		sl_test_print_buffer(key_a.data, key_a.data_len, "ED25519 pubkey");
	}

	ST_EXPECT_EQ(sizeof(g_ed25519_pubkey), key_a.data_len);
	ST_EXPECT_EQ(0, memcmp(g_ed25519_pubkey, key_a.data, sizeof(g_ed25519_pubkey)));
	/* sl_get_key shouldn't return private key*/
	ST_EXPECT_NEQ(sizeof(g_ed25519_privkey_only), key_a.priv_len);

	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ED_25519,
										   SL_RW_KEY_SLOT));
}
END_TEST_F

/*
 * Desc: set ecc p256r1 key
 */
START_TEST_F(set_ecc_256_key)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_set_key(g_hnd, HAL_KEY_ECC_SEC_P256R1,
										SL_RW_KEY_SLOT, &g_ecc_pubkey_in, &g_ecc_prikey_in));

	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_SEC_P256R1,
										   SL_RW_KEY_SLOT));
}
END_TEST_F

/*
 * Desc: Read data in secure storage
 */
START_TEST_F(gen_asym_key)
{
	ST_EXPECT_EQ(SECLINK_OK, sl_generate_key(g_hnd, HAL_KEY_ECC_SEC_P256R1,
											 SL_RW_KEY_SLOT));
	ST_EXPECT_EQ(SECLINK_OK, sl_remove_key(g_hnd, HAL_KEY_ECC_SEC_P256R1,
										   SL_RW_KEY_SLOT));
}
END_TEST_F

/*
 * Parser
 */
void sl_handle_key_set_asym_rw(sl_options *opt)
{
	ST_TC_SET_SMOKE(sl_key, opt->count, 0, "set a asymmetric key",
					key_testcase, set_ed25519_key);
}

void sl_handle_key_set_ecc_rw(sl_options *opt)
{
	ST_TC_SET_SMOKE(sl_key, opt->count, 0, "set a asymmetric ECC key",
					key_testcase, set_ecc_256_key);
}

void sl_handle_key_gen_asym_rw(sl_options *opt)
{
	ST_TC_SET_SMOKE(sl_key, opt->count, 0, "generate a asymmetric key",
					key_testcase, gen_asym_key);
}

/*
 * Key
 * Injected key slot range 0~31
 * tp1x: 0~7
 * RAM key slot range 32~63
 * tp1x: 32~63
 */
void sl_handle_key(sl_options *opt)
{
	ST_TC_SET_GLOBAL(sl_key, sl_key_global);
	SL_PARSE_MESSAGE(opt, g_command, sl_key_type_e,
					 g_func_list, SL_KEY_TYPE_MAX, SL_KEY_TYPE_ERR);
	ST_RUN_TEST(sl_key);
	ST_RESULT_TEST(sl_key);
}
