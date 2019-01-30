#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <security_hal.h>
#include <stress_tool/st_perf.h>

/*
 * Desc: Generate random
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
static hal_data g_rand;
#define RAND_LEN 100
TEST_SETUP(generate_random)
{
	ST_START_TEST;
	g_rand.data = (uint8_t *)malloc(sizeof(RAND_LEN));
	ST_EXPECT_NEQ(NULL, g_rand.data);
	ST_END_TEST;
}

TEST_TEARDOWN(generate_random)
{
	ST_START_TEST;
	free(g_rand.data);
	ST_END_TEST;
}

TEST_F(generate_random)
{
	ST_START_TEST;
	ST_EXPECT(0, hal_generate_random(100, &g_rand))
	ST_END_TEST;
}

/*
 * Desc: Get hash
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
static hal_data g_plain_text;
static hal_data g_hash;
TEST_SETUP(get_hash)
{
	ST_START_TEST;
	g_plain_text.data = "01234567890123456789";
	g_plain_text.length = 20;
	ST_END_TEST;
}

TEST_TEARDOWN(get_hash)
{
	ST_START_TEST;

	ST_END_TEST;
}

TEST_F(get_hash)
{
	ST_START_TEST;
	ST_EXPECT(0, hal_get_hash(HAL_HASH_SHA256, &g_plain_text, &g_hash));
	ST_END_TEST;
}

/*
 * Desc: Get hmac
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
#define HAL_TEST_HMAC_KEY_SLOT 1
static hal_data g_hmac_key;
static hal_data g_hmac;

TEST_SETUP(get_hmac)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_hmac_generate_key(HAL_HMAC_SHA256, HAL_TEST_HMAC_KEY_SLOT, &g_hmac_key));
	g_hmac.data = NULL;
	g_hmac.data_len = 0;

	g_plain_text.data = "01234567890123456789";
    g_plain_text.data_len = 20;

	ST_END_TEST;
}

TEST_TEARDOWN(get_hmac)
{
	ST_START_TEST;
	ST_EXPECT(0, hal_get_hmac(HAL_HMAC_SHA256, &g_plain_text, &g_hmac))
	ST_END_TEST;
}

TEST_F(get_hmac)
{
	ST_START_TEST;

	ST_END_TEST;
}

/*
 * Desc: Get RSA signagure
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
static hal_data g_rsa_pubkey;

TEST_SETUP()
{
	ST_START_TEST;

	ST_EXPECT(0, hal_rsa_generate_key(HAL_RSA_1024, HAL_TEST_RSA_KEY_SLOT, &g_rsa_pubkey));

	ST_END_TEST;
}

TEST_TEARDOWN()
{
	ST_START_TEST;

	ST_END_TEST;
}

TEST_F()
{
	ST_START_TEST;

	ST_END_TEST;
}
/* generate_random
 * get_hash
 * get_hmac
 * rsa_sign
 * rsa_verify
 * ecdsa_sign
 * ecdsa_verify
 * compute_ecdsh ??
 * dh_generate_param ??
 * dh_compute_shared_secret ??
 * get_get_factory
 */
