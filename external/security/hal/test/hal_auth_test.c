#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <security_hal.h>
#include <stress_tool/st_perf.h>

/*  Configuration */
#define HAL_AUTH_TEST_TRIAL 1
#define HAL_AUTH_TEST_LIMIT_TIME 100000000
/*
 * Desc: Generate random
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
static hal_data g_rand;
#define RAND_LEN 100

TEST_SETUP(generate_random)
{
	ST_START_TEST;
	ST_END_TEST;
}

TEST_TEARDOWN(generate_random)
{
	ST_START_TEST;

	hal_free_data(&g_rand);

	ST_END_TEST;
}

TEST_F(generate_random)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_generate_random(100, &g_rand));

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

	ST_EXPECT(0, hal_hmac_generate_key(HAL_HMAC_SHA256, HAL_TEST_HMAC_KEY_SLOT));
	g_hmac.data = NULL;
	g_hmac.data_len = 0;

	g_plain_text.data = "01234567890123456789";
    g_plain_text.data_len = 20;

	ST_END_TEST;
}

TEST_TEARDOWN(get_hmac)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_remove_key(HAL_HMAC_SHA256, HAL_TEST_HMAC_KEY_SLOT));

	ST_END_TEST;
}

TEST_F(get_hmac)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_get_hmac(HAL_HMAC_SHA256, &g_plain_text, &g_hmac));

	ST_END_TEST;
}

/*
 * Desc: Get RSA signagure
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
static hal_data g_hash;
static hal_data g_sign;
#define HAL_TEST_RSA_HASH_LEN 100
TEST_SETUP(rsa_sign)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_generate_key(HAL_KEY_RSA_1024, HAL_TEST_RSA_KEY_SLOT));

	g_hash.data = (uint8_t *)malloc(HAL_TEST_RSA_HASH_LEN);
	ST_EXPECT_NEQ(NULL, g_hash.data);
	memset(g_hash.data, 0xa5, 100);

	ST_END_TEST;
}

TEST_TEARDOWN(rsa_sign)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_remove_key(HAL_KEY_RSA_1024, HAL_TEST_RSA_KEY_SLOT));

	/*  g_hash is not allocated from hal API so it'd be better to free here */
	hal_test_free_buffer(&g_hash);
	hal_free_data(&g_sign);

	ST_END_TEST;
}

TEST_F(rsa_sign)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_rsa_sign_md(HAL_RSAES_PKCS1_OAEP_MGF1_SHA256, &g_hash, HAL_TEST_RSA_KEY_SLOT, &g_sign));

	ST_END_TEST;
}

/*
 * Desc: Verify RSA signature
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
TEST_SETUP(rsa_verify)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_generate_key(HAL_KEY_RSA_1024, HAL_TEST_RSA_KEY_SLOT));

	g_hash.data = (uint8_t *)malloc(HAL_TEST_RSA_HASH_LEN);
	ST_EXPECT_NEQ(NULL, g_hash.data);

	memset(g_hash.data, 0xa5, HAL_TEST_RSA_HASH_LEN);

	ST_EXPECT(0, hal_rsa_sign_md(HAL_RSAES_PKCS1_OAEP_MGF1_SHA256, &g_hash, HAL_TEST_RSA_KEY_SLOT, &g_sign));

	ST_END_TEST;
}

TEST_TEARDOWN(rsa_verify)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_remove_key(HAL_KEY_RSA_1024, HAL_TEST_RSA_KEY_SLOT));

	/*  g_hash is not allocated from hal API so it'd be better to free here */
	hal_test_free_buffer(&g_hash);

	hal_free_data(&g_sign);

	ST_END_TEST;
}

TEST_F(rsa_verify)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_rsa_verify(HAL_RSAES_PKCS1_OAEP_MGF1_SHA256, &g_hash, &g_sign));

	ST_END_TEST;
}

/*
 * Desc: Get ECDSA
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
#define HAL_TEST_ECC_KEY_SLOT 1
#define HAL_TEST_ECC_HASH_LEN 100
TEST_SETUP(ecdsa_sign)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_generate_key(HAL_KEY_ECC_BRAINPOOL_P256R1, HAL_TEST_ECC_KEY_SLOT));

	g_hash.data = (uint8_t *)malloc(HAL_TEST_ECC_HASH_LEN);
	ST_EXPECT_NEQ(NULL, g_hash.data);

	memset(g_hash.data, 0xa5, HAL_TEST_ECC_HASH_LEN);

	ST_END_TEST;
}

TEST_TEARDOWN(ecdsa_sign)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_remove_key(HAL_KEY_ECC_BRAINPOOL_P256R1, HAL_TEST_ECC_KEY_SLOT));

	hal_test_free_buffer(&g_hash);

	hal_free_data(&g_sign);

	ST_END_TEST;
}

TEST_F(ecdsa_sign)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_ecdsa_sign_md(HAL_ECDSA_BRAINPOOL_P256R1, &g_hash, HAL_TEST_ECC_KEY_SLOT, &g_sign));

	ST_END_TEST;
}


/*
 * Desc: Verify ECDSA
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
TEST_SETUP(ecdsa_verify)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_generate_key(HAL_KEY_ECC_BRAINPOOL_P256R1, HAL_TEST_ECC_KEY_SLOT));

	g_hash.data = (uint8_t *)malloc(HAL_TEST_ECC_HASH_LEN);
	ST_EXPECT_NEQ(NULL, g_hash.data);

	memset(g_hash.data, 0xa5, HAL_TEST_ECC_HASH_LEN);

	ST_EXPECT(0, hal_ecdsa_sign_md(HAL_ECDSA_BRAINPOOL_P256R1, &g_hash, HAL_TEST_ECC_KEY_SLOT, &g_sign));

	ST_END_TEST;
}

TEST_TEARDOWN(ecdsa_verify)
{
	ST_START_TEST;
	ST_EXPECT(0, hal_remove_key(HAL_KEY_ECC_BRAINPOOL_P256R1, HAL_TEST_ECC_KEY_SLOT));

	hal_test_free_buffer(&g_hash);

	hal_free_data(&g_sign);

	ST_END_TEST;
}

TEST_F(ecdsa_verify)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_ecdsa_verify_md(HAL_ECDSA_BRAINPOOL_P256R1, &g_hash, HAL_TEST_ECC_KEY_SLOT, &g_sign));

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
	g_dh_data.G.data = g_buf_1024;
	g_dh_data.G.data_len = sizeof(g_buf_1024);
	g_dh_data.P.data = p_buf_1024;
	g_dh_data.P.data_len = sizeof(p_buf_1024);

	ST_END_TEST;
}

TEST_TEARDOWN(dh_generate_param)
{
	ST_START_TEST;

	ST_EXPECT(hal_remove_key(HAL_TEST_DH_X_SLOT));

	hal_free_data(&g_dh_data.pubkey);

	ST_END_TEST;
}

TEST_F(dh_generate_param)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_dh_generate_param(HAL_TEST_DH_X_SLOT, &g_dh_data));

	ST_END_TEST;
}

/*
 * Desc: Compute DH shared secret
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
static hal_data g_shared_secret_a;
static hal_data g_shared_secret_b;
TEST_SETUP(dh_compute_shared_secret)
{
	ST_START_TEST;

	g_dh_data_a.mode = HAL_DH_1024;
	g_dh_data_a.G.data = g_buf_1024;
	g_dh_data_a.G.data_len = sizeof(g_buf_1024);
	g_dh_data_a.P.data = p_buf_1024;
	g_dh_data_a.P.data_len = sizeof(p_buf_1024);
	ST_EXPECT(0, hal_dh_generate_param(HAL_TEST_DH_X_SLOT, &g_dh_data_a));

	g_dh_data_b.mode = HAL_DH_1024;
	g_dh_data_b.G.data = g_buf_1024;
	g_dh_data_b.G.data_len = sizeof(g_buf_1024);
	g_dh_data_b.P.data = p_buf_1024;
	g_dh_data_b.P.data_len = sizeof(p_buf_1024);
	ST_EXPECT(0, hal_dh_generate_param(HAL_TEST_DH_Y_SLOT, &g_dh_data_b));

	ST_END_TEST;
}

TEST_TEARDOWN(dh_compute_shared_secret)
{
	ST_START_TEST;

	ST_EXPECT(hal_remove_key(HAL_TEST_DH_X_SLOT));
	ST_EXPECT(hal_remove_key(HAL_TEST_DH_Y_SLOT));

	hal_free_data(&g_dh_data_a.pubkey);
	hal_free_data(&g_dh_data_b.pubkey);
	hal_free_data(&g_shared_secret_a);
	hal_free_data(&g_shared_secret_b);

	ST_END_TEST;
}

TEST_F(dh_compute_shared_secret)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_dh_compute_shared_secret(&g_dh_data_a, HAL_TEST_DH_Y_SLOT, &g_shared_secret_a));
	ST_EXPECT(0, hal_dh_compute_shared_secret(&g_dh_data_b, HAL_TEST_DH_X_SLOT, &g_shared_secret_b));

	ST_EXPECT(g_shared_secret_a.data_len, g_shared_secret_b.data_len);
	ST_EXPECT(0, memcmp(&g_shared_secret_a.data, &g_shared_secret_b.data, g_shared_secret_a.data_len));

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
static hal_data pubkey_a;
static hal_data pubkey_b;
TEST_SETUP(ecdh_compute_shared_secret)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_generate_key(HAL_KEY_ECC_BRAINPOOL_P256R1, HAL_TEST_ECDH_KEY_SLOT_A));
	ST_EXPECT(0, hal_generate_key(HAL_KEY_ECC_BRAINPOOL_P256R1, HAL_TEST_ECDH_KEY_SLOT_B));
	ST_EXPECT(0, hal_get_key(HAL_TEST_ECDH_KEY_SLOT_A, &pubkey_a));
	ST_EXPECT(0, hal_get_key(HAL_TEST_ECDH_KEY_SLOT_B, &pubkey_b));

	ST_END_TEST;
}

TEST_TEARDOWN(ecdh_compute_shared_secret)
{
	ST_START_TEST;

	ST_EXPECT(hal_remove_key(HAL_TEST_ECDH_KEY_SLOT_A));
	ST_EXPECT(hal_remove_key(HAL_TEST_ECDH_KEY_SLOT_B));

	hal_free_data(&pubkey_a);
	hal_free_data(&pubkey_b);
	hal_free_data(&g_shared_secret_a);
	hal_free_data(&g_shared_secret_b);

	ST_END_TEST;
}

TEST_F(ecdh_compute_shared_secret)
{
	ST_START_TEST;
	// A<--B, B<--A
	ST_EXPECT(0, hal_ecdh_compute_shared_secret(&pubkey_a, HAL_TEST_ECDH_KEY_SLOT_A, &g_shared_secret_a));
	ST_EXPECT(0, hal_ecdh_compute_shared_secret(&pubkey_b, HAL_TEST_ECDH_KEY_SLOT_B, &g_shared_secret_b));
	ST_EXPECT(g_shared_secret_a.data_len, g_shared_secret_b.data_len);
	ST_EXPECT(0, memcmp(&g_shared_secret_a.data, &g_shared_secret_b.data, g_shared_secret_a.data_len));

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

	g_cert_in.data = (unsigned char *)malloc(sizeof(test_crt));
	ST_EXPECT_NEQ(NULL, g_cert_in.data);
	memcpy(g_cert_in.data, test_crt, sizeof(test_crt));
	g_cert_in.data_len = sizeof(test_crt);

	ST_END_TEST;
}

TEST_TEARDOWN(set_certificate)
{
	ST_START_TEST;

	hal_test_free_buffer(&g_cert_in);
	ST_EXPECT(0, hal_delete_certificate(HAL_TEST_CERT_SLOT);

	ST_END_TEST;
}

TEST_F(set_certificate)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_set_certificate(HAL_TEST_CERT_SLOT, &g_cert_in);

	ST_END_TEST;
}

/*
 * Desc: Load certificate in secure storage
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
TEST_SETUP(get_certificate)
{
	ST_START_TEST;

	g_cert_in.data = (unsigned char *)malloc(sizeof(test_crt));
	ST_EXPECT_NEQ(NULL, g_cert_in.data);
	memcpy(g_cert_in.data, test_crt, sizeof(test_crt));
	g_cert_in.data_len = sizeof(test_crt);
	ST_EXPECT(0, hal_set_certificate(HAL_TEST_CERT_SLOT, &g_cert_in);

	ST_END_TEST;
}

TEST_TEARDOWN(get_certificate)
{
	ST_START_TEST;

	hal_test_free_buffer(&g_cert_in);
	hal_free_data(&g_cert_out);
	ST_EXPECT(0, hal_delete_certificate(HAL_TEST_CERT_SLOT);

	ST_END_TEST;
}

TEST_F(get_certificate)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_get_certificate(HAL_TEST_CERT_SLOT, &g_cert_out));
	ST_EXPECT(0, memcmp(g_cert_out.data, g_cert_in.data, g_cert_out.data_len));

	ST_END_TEST;
}

/*
 * Desc: Delete certificate in secure storage
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
TEST_SETUP(delete_certificate)
{
	ST_START_TEST;

	g_cert_in.data = (unsigned char *)malloc(sizeof(test_crt));
	ST_EXPECT_NEQ(NULL, g_cert_in.data);
	memcpy(g_cert_in.data, test_crt, sizeof(test_crt));
	g_cert_in.data_len = sizeof(test_crt);
	ST_EXPECT(0, hal_set_certificate(HAL_TEST_CERT_SLOT, &g_cert_in);

	ST_END_TEST;
}

TEST_TEARDOWN(delete_certificate)
{
	ST_START_TEST;

	hal_test_free_buffer(&g_cert_in);

	ST_END_TEST;
}

TEST_F(delete_certificate)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_delete_certificate(HAL_TEST_CERT_SLOT));

	ST_END_TEST;
}

/*
 * Desc: Get factorykey data
 * Refered https://developer.artik.io/documentation/security-api/see-authentication-test_8c-example.html
 */
static hal_data g_factory_key_data;
#define HAL_TEST_FACTORYKEY_DATA_SLOT 5
TEST_SETUP(get_factorykey_data)
{
	ST_START_TEST;

	ST_END_TEST;
}

TEST_TEARDOWN(get_factorykey_data)
{
	ST_START_TEST;

	hal_free_data(&g_factorykey_data);

	ST_END_TEST;
}

TEST_F(get_factorykey_data)
{
	ST_START_TEST;

	ST_EXPECT(0, hal_get_factorykey_data(HAL_TEST_FACTORYKEY_DATA_SLOT, &g_factorykey_data));

	ST_END_TEST;
}

ST_SET_SMOKE_TAIL(HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "Generate_random", generate_random);
ST_SET_SMOKE(HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "Get hash", get_hash, generate_random);
ST_SET_SMOKE(HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "Get hmac", get_hmac, get_hash);
ST_SET_SMOKE(HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "RSA signature", rsa_sign, get_hmac);
ST_SET_SMOKE(HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "RSA verification", rsa_verify, rsa_sign);
ST_SET_SMOKE(HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "ECDSA signature", ecdsa_sign, rsa_verify);
ST_SET_SMOKE(HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "ECDSA verification", ecdsa_verify, ecdsa_sign);
ST_SET_SMOKE(HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "Generate DH parameters", dh_generate_param, ecdsa_verify);
ST_SET_SMOKE(HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "Compute DH shared secret", dh_compute_shared_secret, dh_generate_param);
ST_SET_SMOKE(HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "Compute ECDH shared secret", ecdh_compute_shared_secret, dh_compute_shared_secret);
ST_SET_SMOKE(HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "Set certificate", set_certificate, ecdh_compute_shared_secret);
ST_SET_SMOKE(HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "Get certificate", get_certificate, set_certificate);
ST_SET_SMOKE(HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "Delete certificate", delete_certificate, get_certificate);
ST_SET_SMOKE(HAL_AUTH_TEST_TRIAL, HAL_AUTH_TEST_LIMIT_TIME, "Get factorykey data", get_factorykey_data, delete_certificate);
ST_SET_PACK(hal_auth, get_factorykey_data);

int hal_auth_test(void)
{
	ST_RUN_TEST(hal_auth);
	ST_RESULT_TEST(hal_auth);

	return 0;
}_
