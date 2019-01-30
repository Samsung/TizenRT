#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <security_hal.h>
#include <stress_tool/st_perf.h>

/*
 * Desc: Compute ECDH
 */
#define HAL_TEST_KEY_SLOT 1
hal_ecc_key g_ecc_key;
hal_data g_key_data;
#define HAL_TEST_ECC_PUBKEYX_LEN 128
#define HAL_TEST_ECC_PUBKEYY_LEN 128

TEST_SETUP(compute_ecdh)
{
	/*  Refer hw_ecdh_compute_shared() */
	ST_START_TEST;
	g_ecc_key.x_byte_len = HAL_TEST_ECC_PUBKEYX_LEN;
	g_ecc_key.y_byte_len = HAL_TEST_ECC_PUBKEYY_LEN;

	g_ecc_key.publickey_x = (unsigned char *)malloc(sizeof(g_ecc_key.x_byte_len));
	ST_EXPECT_NEQ(NULL, g_ecc_key.publickey_x);

	g_ecc_key.publickey_y = (unsigned char *)malloc(sizeof(g_ecc_key.y_byte_len));
	ST_EXPECT_NEQ(NULL, g_ecc_key.publickey_y);

	hal_test_init_buffer(&g_key_data);
	ST_END_TEST;
}

TEST_TEARDOWN(compute_ecdh)
{
	ST_START_TEST;

	free(g_ecc_key.publickey_x);
	free(g_ecc_key.publickey_y);
	g_ecc_key.x_byte_len = 0;
	g_exx_key.y_byte_len = 0;

	hal_test_free_buffer(&g_key_data);

	ST_END_TEST;
}

TEST_F(compute_ecdh)
{
	ST_START_TEST;
	ST_EXPECT(0, hal_compute_ecdh(&g_ecc_key, HAL_TEST_KEY_SLOT, &g_key_data));
	ST_END_TEST;
}


/*
 * Desc: Generate DH parameters
 */
static hal_dh_algo g_dh_algo;
static hal_dh_data g_dh_param;
static unsigned char g_p_buf_1024[] = {
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

static unsigned char g_g_buf_1024[] = {
	0xa4, 0xd1, 0xcb, 0xd5, 0xc3, 0xfd, 0x34, 0x12, 0x67, 0x65, 0xa4, 0x42, 0xef, 0xb9, 0x99,
	0x05, 0xf8, 0x10, 0x4d, 0xd2, 0x58, 0xac, 0x50, 0x7f, 0xd6, 0x40, 0x6c, 0xff, 0x14, 0x26,
	0x6d, 0x31, 0x26, 0x6f, 0xea, 0x1e, 0x5c, 0x41, 0x56, 0x4b, 0x77, 0x7e, 0x69, 0x0f, 0x55,
	0x04, 0xf2, 0x13, 0x16, 0x02, 0x17, 0xb4, 0xb0, 0x1b, 0x88, 0x6a, 0x5e, 0x91, 0x54, 0x7f,
	0x9e, 0x27, 0x49, 0xf4, 0xd7, 0xfb, 0xd7, 0xd3, 0xb9, 0xa9, 0x2e, 0xe1, 0x90, 0x9d, 0x0d,
	0x22, 0x63, 0xf8, 0x0a, 0x76, 0xa6, 0xa2, 0x4c, 0x08, 0x7a, 0x09, 0x1f, 0x53, 0x1d, 0xbf,
	0x0a, 0x01, 0x69, 0xb6, 0xa2, 0x8a, 0xd6, 0x62, 0xa4, 0xd1, 0x8e, 0x73, 0xaf, 0xa3, 0x2d,
	0x77, 0x9d, 0x59, 0x18, 0xd0, 0x8b, 0xc8, 0x85, 0x8f, 0x4d, 0xce, 0xf9, 0x7c, 0x2a, 0x24,
	0x85, 0x5e, 0x6e, 0xeb, 0x22, 0xb3, 0xb2, 0xe5
};

TEST_SETUP(dh_generate_param)
{
	ST_START_TEST;
	g_dh_algo = HAL_DH_1024;
	memset(&g_dh_param, 0, sizeof(hal_dh_data));

	g_dh_param.modules_p_byte_len = 1024/8;
	g_dh_param.modules_p = g_p_buf_1024;

	g_dh_param.generator_g_byte_len = 1024/8;
	g_dh_param.generator_g = g_g_buf_1024;

	g_dh_param.publickey_byte_len = g_dh_param.modules_p_byte_len;
	g_dh_param.publickey = (unsigned char *)malloc(sizeof(g_dh_param.publickey_byte_len));
	ST_EXPECT_NEQ(NULL, g_dh_param.publickey);

	ST_END_TEST;
}

TEST_TEARDOWN(dh_generate_param)
{
	ST_START_TEST;

	free(g_dh_param.publickey);

	ST_END_TEST;
}

TEST_F(dh_generate_param)
{
	ST_START_TEST;
	ST_EXPECT(0, hal_dh_generate_param(g_dh_algo, HAL_TEST_KEY_SLOT, &g_dh_param));
	ST_END_TEST;
}

/*
 * Desc: Compute DH shared secret
 */
TEST_SETUP(dh_compute_shared_secret)
{
	ST_START_TEST;

	g_dh_algo = HAL_DH_1024;
	memset(&g_dh_param, 0, sizeof(hal_dh_data));

	g_dh_param.modules_p_byte_len = 1024/8;
	g_dh_param.modules_p = g_p_buf_1024;

	g_dh_param.generator_g_byte_len = 1024/8;
	g_dh_param.generator_g = g_g_buf_1024;

	g_dh_param.publickey_byte_len = g_dh_param.modules_p_byte_len;
	g_dh_param.publickey = (unsigned char *)malloc(sizeof(g_dh_param.publickey_byte_len));
	ST_EXPECT_NEQ(NULL, g_dh_param.publickey);

	ST_EXPECT(0, hal_dh_generate_param(g_dh_algo, HAL_TEST_KEY_SLOT, &g_dh_param));

	ST_END_TEST;
}

TEST_TEARDOWN(dh_compute_shared_secret)
{
	ST_START_TEST;

	free(g_dh_param.publickey);

	ST_END_TEST;
}

TEST_F(dh_compute_shared_secret)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_dh_compute_shared_secret(g_dh_algo, &g_dh_param, HAL_TEST_KEY_SLOT, g_key_data));

	ST_END_TEST;
}

/*
 * Desc: Get ECDSA Signature
 */
static unsigned char test_ecdsa_dev_bp[122] = {
	0x30, 0x78, 0x02, 0x01, 0x01, 0x04, 0x20, 0x1e, 0x20, 0xd3, 0xa6, 0xaa, 0x38, 0xf6, 0xf1, 0x65,
	0x19, 0xb8, 0xae, 0x31, 0x86, 0x7a, 0x47, 0x3b, 0xaf, 0x5e, 0x54, 0x93, 0xb1, 0x46, 0xba, 0x8d,
	0x39, 0x25, 0xa9, 0xe4, 0xd2, 0x41, 0x66, 0xa0, 0x0b, 0x06, 0x09, 0x2b, 0x24, 0x03, 0x03, 0x02,
	0x08, 0x01, 0x01, 0x07, 0xa1, 0x44, 0x03, 0x42, 0x00, 0x04, 0x1a, 0xbc, 0xb3, 0x24, 0xa4, 0x1e,
	0x89, 0x79, 0x6e, 0xbe, 0x75, 0x7f, 0x78, 0xa7, 0x32, 0x21, 0xae, 0x7b, 0xb5, 0xe4, 0xa4, 0x4d,
	0x07, 0x44, 0x0e, 0x07, 0x5a, 0x77, 0x3d, 0xb3, 0xd8, 0xfc, 0x65, 0x02, 0xfc, 0xd7, 0x1a, 0x3c,
	0xcb, 0x9a, 0x4e, 0x34, 0xde, 0x32, 0xe8, 0x3e, 0x2e, 0xda, 0x09, 0xe0, 0x5f, 0x24, 0x7b, 0x86,
	0x83, 0x08, 0xc3, 0xf6, 0x7f, 0xe3, 0x81, 0xbb, 0xda, 0x62
};
#define TEST_ECDSA_DEV_BP_LEN 122

static hal_ecdsa_curve g_curve;
static hal_ecc_sign g_ecc_sign;
TEST_SETUP(ecdsa_sign_md)
{
	ST_START_TEST;
	g_curve = HAL_ECDSA_BRAINPOOL_P256R1;
	memset(&g_ecc_sign, 0, sizeof(g_ecc_sign));

	g_ecc_sign.r_byte_len = 100;
	g_ecc_sign.r = (unsigned char *)malloc(ecc_sign.r_byte_len);
	ST_EXPECT_NEQ(0, g_ecc_sign.r);

	g_ecc_sign.s_byte_len = 100;
	g_ecc_sign.s = (unsigned char *)malloc(ecc_sign.s_byte_len);
	ST_EXPECT_NEQ(0, g_ecc_sign.s);

	hal_data key;
	key.data = test_ecdsa_dev_bp;
	key_length = TEST_ECDSA_DEV_BP_LEN;

	ST_EXPECT(0, hal_set_key(&key, HAL_KEY_ECC_BRAINPOOL_P256R1, HAL_TEST_KEY_SLOT));

	ST_END_TEST;
}

TEST_TEARDOWN(ecdsa_sign_md)
{
	ST_START_TEST;

	ST_END_TEST;
}

TEST_F(ecdsa_sign_md)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_ecdsa_sign_md(g_curve, HAL_TEST_KEY_SLOT, &g_ecc_sign));

	ST_END_TEST;
}

/*
 * Desc: Verify ECDSA signature
 */
TEST_SETUP(ecdsa_verify_md)
{
	ST_START_TEST;

	g_curve = HAL_ECDSA_BRAINPOOL_P256R1;
	memset(&g_ecc_sign, 0, sizeof(g_ecc_sign));

	g_ecc_sign.r_byte_len = 100;
	g_ecc_sign.r = (unsigned char *)malloc(ecc_sign.r_byte_len);
	ST_EXPECT_NEQ(0, g_ecc_sign.r);

	g_ecc_sign.s_byte_len = 100;
	g_ecc_sign.s = (unsigned char *)malloc(ecc_sign.s_byte_len);
	ST_EXPECT_NEQ(0, g_ecc_sign.s);

	hal_data key;
	key.data = test_ecdsa_dev_bp;
	key_length = TEST_ECDSA_DEV_BP_LEN;

	ST_EXPECT(0, hal_set_key(&key, HAL_KEY_ECC_BRAINPOOL_P256R1, HAL_TEST_KEY_SLOT));

	ST_EXPECT(0, hal_ecdsa_sign_md(g_curve, HAL_TEST_KEY_SLOT, &g_ecc_sign));

	ST_END_TEST;
}

TEST_TEARDOWN(ecdsa_verify_md)
{
	ST_START_TEST;

	ST_END_TEST;
}

TEST_F(ecdsa_verify_md)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_ecdsa_verify_md(g_curve, HA))
	ST_END_TEST;
}


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

/*
 * compute_ecdh
 * dh_generate_param
 * dh_compute_shared_secret
 * generate_ramdom
 * get_hash
 * get_hmac
 * rsa_sign_md
 * rsa_verify_md
 * ecdsa_sign_md
 * ecdsa_verify_md
 * get_factorykey_data
 */
int hal_auth_test(void)
{
	return 0;
}
