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

static security_hash_mode g_hash_mode_table[] = {
	HASH_MD5,
	HASH_SHA1,
	HASH_SHA224,
	HASH_SHA256,
	HASH_SHA384,
	HASH_SHA512,
};

static security_hmac_mode g_hmac_mode_table[] = {
	HMAC_MD5,
	HMAC_SHA1,
	HMAC_SHA224,
	HMAC_SHA256,
	HMAC_SHA384,
	HMAC_SHA512,
};

static security_dh_mode g_dh_mode_table[] = {
	DH_1024,
	DH_2048,
	DH_4096,
};

static security_ecdsa_mode g_ecdsa_mode_table[] = {
	ECDSA_BRAINPOOL_P256R1,
	ECDSA_BRAINPOOL_P384R1,
	ECDSA_BRAINPOOL_P512R1,
	ECDSA_SEC_P192R1,
	ECDSA_SEC_P224R1,
	ECDSA_SEC_P256R1,
	ECDSA_SEC_P384R1,
	ECDSA_SEC_P512R1,
};

static security_rsa_mode g_rsa_mode_table[] = {
	RSASSA_PKCS1_V1_5,
	RSASSA_PKCS1_PSS_MGF1,
};

static security_handle g_hnd = NULL;
/**
 * @testcase         utc_auth_generate_random_p
 * @brief            Generate random
 * @scenario         Generate random
 * @apicovered       auth_generate_random
 * @precondition     none
 * @postcondition    noneauth_generate_random
 */
static void utc_auth_generate_random_p(void)
{
	security_data random = {NULL, 0};

	security_error res = auth_generate_random(g_hnd, 12, &random);

	TC_ASSERT_EQ("auth_generate_random_p", res, SECURITY_OK);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_generate_random_hnd_n
 * @brief            Generate random
 * @scenario         Generate random
 * @apicovered       auth_generate_random
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_generate_random_hnd_n(void)
{
	security_data random = {NULL, 0};
	security_error res = auth_generate_random(NULL, 12, &random);

	TC_ASSERT_EQ("auth_generate_random_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_generate_random_input_n
 * @brief            Generate random
 * @scenario         Generate random
 * @apicovered       auth_generate_random
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_generate_random_input_n(void)
{
	security_error res = auth_generate_random(g_hnd, 12, NULL);

	TC_ASSERT_EQ("auth_generate_random_input_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_generate_certificate_p
 * @brief            Generate a certificate
 * @scenario         Generate a certificate
 * @apicovered       auth_generate_certificate
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_generate_certificate_p(void)
{
	security_data cert = {NULL, 0};
	security_csr csr;
	security_error res = auth_generate_certificate(g_hnd, UTC_CERT_NAME, &csr, &cert);

	TC_ASSERT_EQ("auth_generate_certificate_p", res, SECURITY_OK);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_generate_certificate_hnd_n
 * @brief            Generate a certificate
 * @scenario         Generate a certificate
 * @apicovered       auth_generate_certificate
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_generate_certificate_hnd_n(void)
{
	security_data cert = {NULL, 0};
	security_csr csr;
	security_error res = auth_generate_certificate(NULL, UTC_CERT_NAME, &csr, &cert);

	TC_ASSERT_EQ("auth_generate_certificate_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_generate_certificate_name_n
 * @brief            Generate a certificate
 * @scenario         Generate a certificate
 * @apicovered       auth_generate_certificate
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_generate_certificate_name_n(void)
{
	security_data cert = {NULL, 0};
	security_csr csr;
	security_error res = auth_generate_certificate(g_hnd, NULL, &csr, &cert);

	TC_ASSERT_EQ("auth_generate_certificate_name_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_generate_certificate_name_n
 * @brief            Generate a certificate
 * @scenario         Generate a certificate
 * @apicovered       auth_generate_certificate
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_generate_certificate_input_n(void)
{
	security_csr csr;
	security_error res = auth_generate_certificate(g_hnd, UTC_CERT_NAME, &csr, NULL);

	TC_ASSERT_EQ("auth_generate_certificate_input_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_set_certificate_p
 * @brief            Set a certificate
 * @scenario         Set a certificate
 * @apicovered       auth_set_certificate
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_set_certificate_p(void)
{
	US_DEFINE_DATA(cert, "certificate data");

	security_error res = auth_set_certificate(g_hnd, UTC_CERT_NAME, &cert);

	TC_ASSERT_EQ("auth_set_certificate_p", res, SECURITY_OK);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_set_certificate_hnd_n
 * @brief            Set a certificate
 * @scenario         Set a certificate
 * @apicovered       auth_set_certificate
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_set_certificate_hnd_n(void)
{
	US_DEFINE_DATA(cert, "certificate data");
	security_error res = auth_set_certificate(NULL, UTC_CERT_NAME, &cert);

	TC_ASSERT_EQ("auth_set_certificate_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_set_certificate_name_n
 * @brief            Set a certificate
 * @scenario         Set a certificate
 * @apicovered       auth_set_certificate
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_set_certificate_name_n(void)
{
	US_DEFINE_DATA(cert, "certificate data");
	security_error res = auth_set_certificate(g_hnd, NULL, &cert);

	TC_ASSERT_EQ("auth_set_certificate_name_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_set_certificate_input_n
 * @brief            Set a certificate
 * @scenario         Set a certificate
 * @apicovered       auth_set_certificate
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_set_certificate_input_n(void)
{
	security_error res = auth_set_certificate(g_hnd, UTC_CERT_NAME, NULL);

	TC_ASSERT_EQ("auth_set_certificate_input_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_certificate_p
 * @brief            Get a certificate
 * @scenario         Get a certificate
 * @apicovered       auth_get_certificate
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_certificate_p(void)
{
	security_data cert = {NULL, 0};
	security_error res = auth_get_certificate(g_hnd, UTC_CERT_NAME, &cert);

	TC_ASSERT_EQ("auth_get_certificate_p", res, SECURITY_OK);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_certificate_hnd_n
 * @brief            Get a certificate
 * @scenario         Get a certificate
 * @apicovered       auth_get_certificate
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_certificate_hnd_n(void)
{
	security_data cert = {NULL, 0};
	security_error res = auth_get_certificate(NULL, UTC_CERT_NAME, &cert);

	TC_ASSERT_EQ("auth_get_certificate_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_certificate_name_n
 * @brief            Get a certificate
 * @scenario         Get a certificate
 * @apicovered       auth_get_certificate
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_certificate_name_n(void)
{
	security_data cert = {NULL, 0};
	security_error res = auth_get_certificate(g_hnd, NULL, &cert);

	TC_ASSERT_EQ("auth_get_certificate_name_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_certificate_input_n
 * @brief            Get a certificate
 * @scenario         Get a certificate
 * @apicovered       auth_get_certificate
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_certificate_input_n(void)
{
	security_error res = auth_get_certificate(g_hnd, UTC_CERT_NAME, NULL);

	TC_ASSERT_EQ("auth_get_certificate_input_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_remove_certificate_p
 * @brief            Remove a certificate
 * @scenario         Remove a certificate
 * @apicovered       auth_remove_certificate
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_remove_certificate_p(void)
{
	US_DEFINE_DATA(cert, "certificate data");
	security_error res = auth_set_certificate(g_hnd, NULL, &cert);
	TC_ASSERT_NEQ("auth_remove_certificate_p", res, SECURITY_OK);
	TC_SUCCESS_RESULT();

	res = auth_remove_certificate(g_hnd, UTC_CERT_NAME);
	TC_ASSERT_EQ("auth_remove_certificate_p", res, SECURITY_OK);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_remove_certificate_hnd_n
 * @brief            Remove a certificate
 * @scenario         Remove a certificate
 * @apicovered       auth_remove_certificate
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_remove_certificate_hnd_n(void)
{
	US_DEFINE_DATA(cert, "certificate data");
	security_error res = auth_set_certificate(g_hnd, NULL, &cert);
	TC_ASSERT_NEQ("auth_remove_certificate_p", res, SECURITY_OK);
	TC_SUCCESS_RESULT();

	res = auth_remove_certificate(NULL, UTC_CERT_NAME);

	TC_ASSERT_EQ("auth_remove_certificate_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_remove_certificate_name_n
 * @brief            Remove a certificate
 * @scenario         Remove a certificate
 * @apicovered       auth_remove_certificate
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_remove_certificate_name_n(void)
{
	US_DEFINE_DATA(cert, "certificate data");
	security_error res = auth_set_certificate(g_hnd, NULL, &cert);
	TC_ASSERT_NEQ("auth_remove_certificate_p", res, SECURITY_OK);
	TC_SUCCESS_RESULT();

	res = auth_remove_certificate(g_hnd, NULL);

	TC_ASSERT_EQ("auth_remove_certificate_name_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_rsa_signature_p
 * @brief            Get RSA signature
 * @scenario         Get RSA signature
 * @apicovered       auth_get_rsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_rsa_signature_p(void)
{
	US_DEFINE_DATA(hash, "hashed data");
	security_data sign = {NULL, 0};
	int i = 0, j = 0, k = 0;

	for (; i < sizeof(g_rsa_mode_table)/sizeof(security_rsa_mode); i++) {
		for (; j < sizeof(g_hash_mode_table)/sizeof(security_hash_mode); j++) {
			for (; k < sizeof(g_hash_mode_table)/sizeof(security_hash_mode); k++) {
				security_rsa_param param = {g_rsa_mode_table[i], g_hash_mode_table[j], g_hash_mode_table[k], 0};
				security_error res = auth_get_rsa_signature(g_hnd, &param, UTC_CRYPTO_KEY_NAME, &hash, &sign);
				TC_ASSERT_EQ("auth_get_rsa_signature_p", res, SECURITY_OK);
				TC_SUCCESS_RESULT();
			}
		}
	}
}

/**
 * @testcase         utc_auth_get_rsa_signature_hnd_n
 * @brief            Get RSA Signature
 * @scenario         Get RSA Signature
 * @apicovered       auth_get_rsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_rsa_signature_hnd_n(void)
{
	US_DEFINE_DATA(hash, "hashed data");
	security_data sign = {NULL, 0};
	security_rsa_param param = {g_rsa_mode_table[0], g_hash_mode_table[0], g_hash_mode_table[0], 0};

	security_error res = auth_get_rsa_signature(NULL, &param, UTC_CRYPTO_KEY_NAME, &hash, &sign);

	TC_ASSERT_EQ("auth_get_rsa_signature_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_rsa_signature_param_n
 * @brief            Get RSA Signature
 * @scenario         Get RSA Signature
 * @apicovered       auth_get_rsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_rsa_signature_param_n(void)
{
	US_DEFINE_DATA(hash, "hashed data");
	security_data sign = {NULL, 0};

	security_error res = auth_get_rsa_signature(g_hnd, NULL, UTC_CRYPTO_KEY_NAME, &hash, &sign);

	TC_ASSERT_EQ("auth_get_rsa_signature_param_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_rsa_signature_name_n
 * @brief            Get RSA Signature
 * @scenario         Get RSA Signature
 * @apicovered       auth_get_rsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_rsa_signature_name_n(void)
{
	US_DEFINE_DATA(hash, "hashed data");
	security_data sign = {NULL, 0};
	security_rsa_param param = {g_rsa_mode_table[0], g_hash_mode_table[0], g_hash_mode_table[0], 0};

	security_error res = auth_get_rsa_signature(g_hnd, &param, NULL, &hash, &sign);

	TC_ASSERT_EQ("auth_get_rsa_signature_name_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_rsa_signature_hash_n
 * @brief            Get RSA Signature
 * @scenario         Get RSA Signature
 * @apicovered       auth_get_rsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_rsa_signature_hash_n(void)
{
	security_data sign = {NULL, 0};
	security_rsa_param param = {g_rsa_mode_table[0], g_hash_mode_table[0], g_hash_mode_table[0], 0};
	security_error res = auth_get_rsa_signature(g_hnd, &param, UTC_CRYPTO_KEY_NAME, NULL, &sign);

	TC_ASSERT_EQ("auth_get_rsa_signature_hash_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_rsa_signature_sign_n
 * @brief            Get RSA Signature
 * @scenario         Get RSA Signature
 * @apicovered       auth_get_rsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_rsa_signature_sign_n(void)
{
	US_DEFINE_DATA(hash, "hashed data");
	security_rsa_param param = {g_rsa_mode_table[0], g_hash_mode_table[0], g_hash_mode_table[0], 0};
	security_error res = auth_get_rsa_signature(g_hnd, &param, UTC_CRYPTO_KEY_NAME, &hash, NULL);

	TC_ASSERT_EQ("auth_get_rsa_signature_sign_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_verify_rsa_signature_p
 * @brief            Verify RSA signature
 * @scenario         Verify RSA signature
 * @apicovered       auth_verify_rsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_verify_rsa_signature_p(void)
{
	US_DEFINE_DATA(hash, "hashed data");
	US_DEFINE_DATA(sign, "signed data");
	int i = 0, j = 0, k = 0;

	for (; i < sizeof(g_rsa_mode_table)/sizeof(security_rsa_mode); i++) {
		for (; j < sizeof(g_hash_mode_table)/sizeof(security_hash_mode); j++) {
			for (; k < sizeof(g_hash_mode_table)/sizeof(security_hash_mode); k++) {
				security_rsa_param param = {g_rsa_mode_table[i], g_hash_mode_table[j], g_hash_mode_table[k], 0};
				security_error res = auth_verify_rsa_signature(g_hnd, &param, UTC_CRYPTO_KEY_NAME, &hash, &sign);

				TC_ASSERT_EQ("auth_verify_rsa_signature_p", res, SECURITY_OK);
				TC_SUCCESS_RESULT();
			}
		}
	}
}

/**
 * @testcase         utc_auth_verify_rsa_signature_hnd_n
 * @brief            Verify RSA signature
 * @scenario         Verify RSA signature
 * @apicovered       auth_verify_rsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_verify_rsa_signature_hnd_n(void)
{
	US_DEFINE_DATA(hash, "hashed data");
	US_DEFINE_DATA(sign, "signed data");
	security_rsa_param param = {g_rsa_mode_table[0], g_hash_mode_table[0], g_hash_mode_table[0], 0};

	security_error res = auth_verify_rsa_signature(NULL, &param, UTC_CRYPTO_KEY_NAME, &hash, &sign);

	TC_ASSERT_EQ("auth_verify_rsa_signature_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_verify_rsa_signature_param_n
 * @brief            Verify RSA signature
 * @scenario         Verify RSA signature
 * @apicovered       auth_verify_rsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_verify_rsa_signature_param_n(void)
{
	US_DEFINE_DATA(hash, "hashed data");
	US_DEFINE_DATA(sign, "signed data");
	security_error res = auth_verify_rsa_signature(g_hnd, NULL, UTC_CRYPTO_KEY_NAME, &hash, &sign);

	TC_ASSERT_EQ("auth_verify_rsa_signature_param_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_verify_rsa_signature_name_n
 * @brief            Verify RSA signature
 * @scenario         Verify RSA signature
 * @apicovered       auth_verify_rsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_verify_rsa_signature_name_n(void)
{
	US_DEFINE_DATA(hash, "hashed data");
	US_DEFINE_DATA(sign, "signed data");
	security_rsa_param param = {g_rsa_mode_table[0], g_hash_mode_table[0], g_hash_mode_table[0], 0};

	security_error res = auth_verify_rsa_signature(g_hnd, &param, NULL, &hash, &sign);

	TC_ASSERT_EQ("auth_verify_rsa_signature_name_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_verify_rsa_signature_hash_n
 * @brief            Verify RSA signature
 * @scenario         Verify RSA signature
 * @apicovered       auth_verify_rsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_verify_rsa_signature_hash_n(void)
{
	US_DEFINE_DATA(sign, "signed data");
	security_rsa_param param = {g_rsa_mode_table[0], g_hash_mode_table[0], g_hash_mode_table[0], 0};
	security_error res = auth_verify_rsa_signature(g_hnd, &param, UTC_CRYPTO_KEY_NAME, NULL, &sign);

	TC_ASSERT_EQ("auth_verify_rsa_signature_hash_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_verify_rsa_signature_sign_n
 * @brief            Verify RSA signature
 * @scenario         Verify RSA signature
 * @apicovered       auth_verify_rsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_verify_rsa_signature_sign_n(void)
{
	US_DEFINE_DATA(hash, "hashed data");
	security_rsa_param param = {g_rsa_mode_table[0], g_hash_mode_table[0], g_hash_mode_table[0], 0};

	security_error res = auth_verify_rsa_signature(g_hnd, &param, UTC_CRYPTO_KEY_NAME, &hash, NULL);

	TC_ASSERT_EQ("auth_verify_rsa_signature_sign_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_ecdsa_signature_p
 * @brief            Get ECDSA signature
 * @scenario         Get ECDSA signature
 * @apicovered       auth_get_ecdsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_ecdsa_signature_p(void)
{
	US_DEFINE_DATA(hash, "hashed data");
	security_data sign = {NULL, 0};
	int i = 0, j = 0;

	for (; i < sizeof(g_ecdsa_mode_table)/sizeof(security_ecdsa_mode); i++) {
		for (; j < sizeof(g_hash_mode_table)/sizeof(security_hash_mode); j++) {
			security_ecdsa_param param = {g_ecdsa_mode_table[i], g_hash_mode_table[j]};
			security_error res = auth_get_ecdsa_signature(g_hnd, &param, UTC_CRYPTO_KEY_NAME, &hash, &sign);

			TC_ASSERT_EQ("auth_get_ecdsa_signature_p", res, SECURITY_OK);
			TC_SUCCESS_RESULT();
		}
	}
}

/**
 * @testcase         utc_auth_get_ecdsa_signature_hnd_n
 * @brief            Get ECDSA signature
 * @scenario         Get ECDSA signature
 * @apicovered       auth_get_ecdsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_ecdsa_signature_hnd_n(void)
{
	US_DEFINE_DATA(hash, "hashed data");
	security_data sign = {NULL, 0};
	security_ecdsa_param param = {g_ecdsa_mode_table[0], g_hash_mode_table[0]};

	security_error res = auth_get_ecdsa_signature(NULL, &param, UTC_CRYPTO_KEY_NAME, &hash, &sign);

	TC_ASSERT_EQ("auth_get_ecdsa_signature_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_ecdsa_signature_param_n
 * @brief            Get ECDSA signature
 * @scenario         Get ECDSA signature
 * @apicovered       auth_get_ecdsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_ecdsa_signature_param_n(void)
{
	US_DEFINE_DATA(hash, "hashed data");
	security_data sign = {NULL, 0};

	security_error res = auth_get_ecdsa_signature(g_hnd, NULL, UTC_CRYPTO_KEY_NAME, &hash, &sign);

	TC_ASSERT_EQ("auth_get_ecdsa_signature_param_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_ecdsa_signature_name_n
 * @brief            Get ECDSA signature
 * @scenario         Get ECDSA signature
 * @apicovered       auth_get_ecdsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_ecdsa_signature_name_n(void)
{
	US_DEFINE_DATA(hash, "hashed data");
	security_data sign = {NULL, 0};
	security_ecdsa_param param = {g_ecdsa_mode_table[0], g_hash_mode_table[0]};

	security_error res = auth_get_ecdsa_signature(g_hnd, &param, NULL, &hash, &sign);

	TC_ASSERT_EQ("auth_get_ecdsa_signature_name_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_ecdsa_signature_hash_n
 * @brief            Get ECDSA signature
 * @scenario         Get ECDSA signature
 * @apicovered       auth_get_ecdsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_ecdsa_signature_hash_n(void)
{
	security_data sign = {NULL, 0};
	security_ecdsa_param param = {g_ecdsa_mode_table[0], g_hash_mode_table[0]};
	security_error res = auth_get_ecdsa_signature(g_hnd, &param, UTC_CRYPTO_KEY_NAME, NULL, &sign);

	TC_ASSERT_EQ("auth_get_ecdsa_signature_hash_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}


/**
 * @testcase         utc_auth_get_ecdsa_signature_sign_n
 * @brief            Get ECDSA signature
 * @scenario         Get ECDSA signature
 * @apicovered       auth_get_ecdsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_ecdsa_signature_sign_n(void)
{
	US_DEFINE_DATA(hash, "hashed data");
	security_ecdsa_param param = {g_ecdsa_mode_table[0], g_hash_mode_table[0]};

	security_error res = auth_get_ecdsa_signature(g_hnd, &param, UTC_CRYPTO_KEY_NAME, &hash, NULL);

	TC_ASSERT_EQ("auth_get_ecdsa_signature_sign_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_verify_ecdsa_signature_p
 * @brief            Get ECDSA signature
 * @scenario         Verify ECDSA signature
 * @apicovered       auth_verify_ecdsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_verify_ecdsa_signature_p(void)
{
	US_DEFINE_DATA(hash, "hashed data");
	US_DEFINE_DATA(sign, "signed data");
	int i = 0, j = 0;

	for (; i < sizeof(g_ecdsa_mode_table)/sizeof(security_ecdsa_mode); i++) {
		for (; j < sizeof(g_hash_mode_table)/sizeof(security_hash_mode); j++) {
			security_ecdsa_param param = {g_ecdsa_mode_table[i], g_hash_mode_table[j]};
			security_error res = auth_verify_ecdsa_signature(g_hnd, &param, UTC_CRYPTO_KEY_NAME, &hash, &sign);

			TC_ASSERT_EQ("auth_verify_ecdsa_signature_p", res, SECURITY_OK);
			TC_SUCCESS_RESULT();
		}
	}
}

/**
 * @testcase         utc_auth_verify_ecdsa_signature_hnd_n
 * @brief            Verify ECDSA signature
 * @scenario         Verify ECDSA signature
 * @apicovered       auth_verify_ecdsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_verify_ecdsa_signature_hnd_n(void)
{
	US_DEFINE_DATA(hash, "hashed data");
	US_DEFINE_DATA(sign, "signed data");
	security_ecdsa_param param = {g_ecdsa_mode_table[0], g_hash_mode_table[0]};

	security_error res = auth_verify_ecdsa_signature(NULL, &param, UTC_CRYPTO_KEY_NAME, &hash, &sign);

	TC_ASSERT_EQ("auth_verify_ecdsa_signature_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_verify_ecdsa_signature_param_n
 * @brief            Verify ECDSA signature
 * @scenario         Verify ECDSA signature
 * @apicovered       auth_verify_ecdsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_verify_ecdsa_signature_param_n(void)
{
	US_DEFINE_DATA(hash, "hashed data");
	US_DEFINE_DATA(sign, "signed data");

	security_error res = auth_verify_ecdsa_signature(g_hnd, NULL, UTC_CRYPTO_KEY_NAME, &hash, &sign);

	TC_ASSERT_EQ("auth_verify_ecdsa_signature_param_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_verify_ecdsa_signature_name_n
 * @brief            Verify ECDSA signature
 * @scenario         Verify ECDSA signature
 * @apicovered       auth_verify_ecdsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_verify_ecdsa_signature_name_n(void)
{
	US_DEFINE_DATA(hash, "hashed data");
	US_DEFINE_DATA(sign, "signed data");
	security_ecdsa_param param = {g_ecdsa_mode_table[0], g_hash_mode_table[0]};

	security_error res = auth_verify_ecdsa_signature(g_hnd, &param, NULL, &hash, &sign);

	TC_ASSERT_EQ("auth_verify_ecdsa_signature_name_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_verify_ecdsa_signature_hash_n
 * @brief            Verify ECDSA signature
 * @scenario         Verify ECDSA signature
 * @apicovered       auth_verify_ecdsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_verify_ecdsa_signature_hash_n(void)
{
	US_DEFINE_DATA(sign, "signed data");
	security_ecdsa_param param = {g_ecdsa_mode_table[0], g_hash_mode_table[0]};
	security_error res = auth_verify_ecdsa_signature(g_hnd, &param, UTC_CRYPTO_KEY_NAME, NULL, &sign);

	TC_ASSERT_EQ("auth_verify_ecdsa_signature_hash_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}


/**
 * @testcase         utc_auth_verify_ecdsa_signature_sign_n
 * @brief            Verify ECDSA signature
 * @scenario         Verify ECDSA signature
 * @apicovered       auth_verify_ecdsa_signature
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_verify_ecdsa_signature_sign_n(void)
{
	US_DEFINE_DATA(hash, "hashed data");
	security_ecdsa_param param = {g_ecdsa_mode_table[0], g_hash_mode_table[0]};

	security_error res = auth_verify_ecdsa_signature(g_hnd, &param, UTC_CRYPTO_KEY_NAME, &hash, NULL);

	TC_ASSERT_EQ("auth_verify_ecdsa_signature_sign_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_hash_p
 * @brief            Get Hash
 * @scenario         Get Hash
 * @apicovered       auth_get_hash
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_hash_p(void)
{
	US_DEFINE_DATA(plain, "Plain text");
	security_data hash = {NULL, 0};
	int i = 0;

	for (; i < sizeof(g_hash_mode_table)/sizeof(security_hash_mode); i++) {
		security_error res = auth_get_hash(g_hnd, g_hash_mode_table[i], &plain, &hash);

		TC_ASSERT_EQ("auth_get_hash_p", res, SECURITY_OK);
		TC_SUCCESS_RESULT();
	}
}

/**
 * @testcase         utc_auth_get_hash_hnd_n
 * @brief            Get Hash
 * @scenario         Get Hash
 * @apicovered       auth_get_hash
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_hash_hnd_n(void)
{
	US_DEFINE_DATA(plain, "Plain text");
	security_data hash = {NULL, 0};
	security_hash_mode mode = g_hash_mode_table[0];

	security_error res = auth_get_hash(NULL, mode, &plain, &hash);

	TC_ASSERT_EQ("auth_get_hash_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_hash_mode_n
 * @brief            Get Hash
 * @scenario         Get Hash
 * @apicovered       auth_get_hash
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_hash_mode_n(void)
{
	US_DEFINE_DATA(plain, "Plain text");
	security_data hash = {NULL, 0};
	security_error res = auth_get_hash(g_hnd, HASH_UNKNOWN, &plain, &hash);

	TC_ASSERT_EQ("auth_get_hash_mode_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_hash_input_n
 * @brief            Get Hash
 * @scenario         Get Hash
 * @apicovered       auth_get_hash
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_hash_input_n(void)
{
	security_data hash = {NULL, 0};
	security_hash_mode mode = g_hash_mode_table[0];
	security_error res = auth_get_hash(g_hnd, mode, NULL, &hash);

	TC_ASSERT_EQ("auth_get_hash_input_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_hash_hash_n
 * @brief            Get Hash
 * @scenario         Get Hash
 * @apicovered       auth_get_hash
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_hash_hash_n(void)
{
	US_DEFINE_DATA(plain, "Plain text");
	security_hash_mode mode = g_hash_mode_table[0];

	security_error res = auth_get_hash(g_hnd, mode, &plain, NULL);

	TC_ASSERT_EQ("auth_get_hash_hash_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_hmac_p
 * @brief            Get HMAC
 * @scenario         Get HMAC
 * @apicovered       auth_get_hmac
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_hmac_p(void)
{
	US_DEFINE_DATA(plain, "Plain Data");
	security_data hmac = {NULL, 0};
	int i = 0;
	for (; i < sizeof(g_hmac_mode_table)/sizeof(security_hmac_mode); i++) {
		security_hmac_mode mode = g_hmac_mode_table[i];

		security_error res = auth_get_hmac(g_hnd, mode, UTC_CRYPTO_KEY_NAME, &plain, &hmac);

		TC_ASSERT_EQ("auth_get_hmac_p", res, SECURITY_OK);
		TC_SUCCESS_RESULT();
	}
}

/**
 * @testcase         utc_auth_get_hmac_hnd_n
 * @brief            Get HMAC
 * @scenario         Get HMAC
 * @apicovered       auth_get_hmac
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_hmac_hnd_n(void)
{
	US_DEFINE_DATA(plain, "Plain Data");
	security_data hmac = {NULL, 0};
	security_hmac_mode mode = g_hmac_mode_table[0];
	security_error res = auth_get_hmac(NULL, mode, UTC_CRYPTO_KEY_NAME, &plain, &hmac);

	TC_ASSERT_EQ("auth_get_hmac_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_hmac_mode_n
 * @brief            Get HMAC
 * @scenario         Get HMAC
 * @apicovered       auth_get_hmac
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_hmac_mode_n(void)
{
	US_DEFINE_DATA(plain, "Plain Data");
	security_data hmac = {NULL, 0};
	security_error res = auth_get_hmac(g_hnd, HMAC_UNKNOWN, UTC_CRYPTO_KEY_NAME, &plain, &hmac);

	TC_ASSERT_EQ("auth_get_hmac_mode_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_hmac_name_n
 * @brief            Get HMAC
 * @scenario         Get HMAC
 * @apicovered       auth_get_hmac
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_hmac_name_n(void)
{
	US_DEFINE_DATA(plain, "Plain Data");
	security_data hmac = {NULL, 0};
	security_hmac_mode mode = g_hmac_mode_table[0];
	security_error res = auth_get_hmac(g_hnd, mode, NULL, &plain, &hmac);

	TC_ASSERT_EQ("auth_get_hmac_name_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_hmac_input_n_
 * @brief            Get HMAC
 * @scenario         Get HMAC
 * @apicovered       auth_get_hmac_input_n
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_hmac_input_n(void)
{
	security_data hmac = {NULL, 0};
	security_hmac_mode mode = g_hmac_mode_table[0];
	security_error res = auth_get_hmac(g_hnd, mode, UTC_CRYPTO_KEY_NAME, NULL, &hmac);

	TC_ASSERT_EQ("auth_get_hmac_input_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_get_hmac_hmac_n
 * @brief            Get HMAC
 * @scenario         Get HMAC
 * @apicovered       auth_get_hmac
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_get_hmac_hmac_n(void)
{
	US_DEFINE_DATA(plain, "Plain Data");
	security_hmac_mode mode = g_hmac_mode_table[0];
	security_error res = auth_get_hmac(g_hnd, mode, UTC_CRYPTO_KEY_NAME, &plain, NULL);

	TC_ASSERT_EQ("auth_get_hmac_hmac_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_generate_dhparams_p
 * @brief            Generate DH parameter
 * @scenario         Generate DH parameter
 * @apicovered       auth_generate_dhparams
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_generate_dhparams_p(void)
{
	security_data h_G = {NULL, 0};
	security_data h_P = {NULL, 0};
	security_data h_pubkey = {NULL, 0};
	security_dh_param param = {DH_1024, &h_G, &h_P, &h_pubkey};

	security_error res = auth_generate_dhparams(g_hnd, UTC_CRYPTO_KEY_NAME, &param);

	TC_ASSERT_EQ("auth_generate_dhparams_p", res, SECURITY_OK);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_generate_dhparams_hnd_n
 * @brief            Generate DH parameter
 * @scenario         Generate DH parameter
 * @apicovered       auth_generate_dhparams
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_generate_dhparams_hnd_n(void)
{
	security_dh_param param;
	security_error res = auth_generate_dhparams(NULL, UTC_CRYPTO_KEY_NAME, &param);

	TC_ASSERT_EQ("auth_generate_dhparams_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_generate_dhparams_name_n
 * @brief            Generate DH parameter
 * @scenario         Generate DH parameter
 * @apicovered       auth_generate_dhparams
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_generate_dhparams_name_n(void)
{
	security_data G;
	security_data P;
	security_data pubkey;
	security_dh_param param = {DH_1024, &G, &P, &pubkey};
	security_error res = auth_generate_dhparams(g_hnd, NULL, &param);

	TC_ASSERT_EQ("auth_generate_dhparams_name_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_generate_dhparams_param_n
 * @brief            Generate DH parameter
 * @scenario         Generate DH parameter
 * @apicovered       auth_generate_dhparams
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_generate_dhparams_param_n(void)
{
	security_error res = auth_generate_dhparams(g_hnd, UTC_CRYPTO_KEY_NAME, NULL);

	TC_ASSERT_EQ("auth_generate_dhparams_param_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_compute_dhparams_p
 * @brief            Compute DH parameter
 * @scenario         Compute DH parameter
 * @apicovered       auth_compute_dhparams
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_compute_dhparams_p(void)
{
	US_DEFINE_DATA(dh_g, "G data");
	US_DEFINE_DATA(dh_p, "P data");
	US_DEFINE_DATA(pubkey, "pubkey");
	security_data secret = {NULL, 0};

	int i = 0;
	for (; i < sizeof(g_dh_mode_table)/sizeof(security_dh_mode); i++) {
		security_dh_param param = {g_dh_mode_table[i], &dh_g, &dh_p, &pubkey};
		security_error res = auth_compute_dhparams(g_hnd, UTC_CRYPTO_KEY_NAME, &param, &secret);

		TC_ASSERT_EQ("auth_compute_dhparams_p", res, SECURITY_OK);
		TC_SUCCESS_RESULT();
	}
}

/**
 * @testcase         utc_auth_compute_dhparams_hnd_n
 * @brief            Compute DH parameter
 * @scenario         Compute DH parameter
 * @apicovered       auth_compute_dhparams
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_compute_dhparams_hnd_n(void)
{
	US_DEFINE_DATA(dh_g, "G data");
	US_DEFINE_DATA(dh_p, "P data");
	US_DEFINE_DATA(pubkey, "pubkey");
	security_dh_param param = {g_dh_mode_table[0], &dh_g, &dh_p, &pubkey};
	security_data secret = {NULL, 0};

	security_error res = auth_compute_dhparams(NULL, UTC_CRYPTO_KEY_NAME, &param, &secret);

	TC_ASSERT_EQ("auth_compute_dhparams_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_compute_dhparams_name_n
 * @brief            Compute DH parameter
 * @scenario         Compute DH parameter
 * @apicovered       auth_compute_dhparams
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_compute_dhparams_name_n(void)
{
	US_DEFINE_DATA(dh_g, "G data");
	US_DEFINE_DATA(dh_p, "P data");
	US_DEFINE_DATA(pubkey, "pubkey");
	security_dh_param param = {g_dh_mode_table[0], &dh_g, &dh_p, &pubkey};
	security_data secret = {NULL, 0};

	security_error res = auth_compute_dhparams(g_hnd, NULL, &param, &secret);

	TC_ASSERT_EQ("auth_compute_dhparams_name_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_compute_dhparams_param_n
 * @brief            Compute DH parameter
 * @scenario         Compute DH parameter
 * @apicovered       auth_compute_dhparams
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_compute_dhparams_param_n(void)
{
	security_data secret = {NULL, 0};
	security_error res = auth_compute_dhparams(g_hnd, UTC_CRYPTO_KEY_NAME, NULL, &secret);

	TC_ASSERT_EQ("auth_compute_dhparams_param_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_compute_dhparams_secret_n
 * @brief            Compute DH parameter
 * @scenario         Compute DH parameter
 * @apicovered       auth_compute_dhparams
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_compute_dhparams_secret_n(void)
{
	US_DEFINE_DATA(dh_g, "G data");
	US_DEFINE_DATA(dh_p, "P data");
	US_DEFINE_DATA(pubkey, "pubkey");
	security_dh_param param = {g_dh_mode_table[0], &dh_g, &dh_p, &pubkey};
	security_error res = auth_compute_dhparams(g_hnd, UTC_CRYPTO_KEY_NAME, &param, NULL);

	TC_ASSERT_EQ("auth_compute_dhparams_secret_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_generate_ecdhkey_p
 * @brief            Generate DH parameter
 * @scenario         Generate DH parameter
 * @apicovered       auth_generate_ecdhkey
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_generate_ecdhkey_p(void)
{
	security_ecdsa_mode curve = ECDSA_UNKNOWN;
	security_data px;
	security_data py;
	security_ecdh_param param = {curve, &px, &py};
	security_error res = auth_generate_ecdhkey(g_hnd, UTC_CRYPTO_KEY_NAME, &param);

	TC_ASSERT_EQ("auth_generate_ecdhkey_p", res, SECURITY_OK);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_generate_ecdhkey_hnd_n
 * @brief            Generate DH parameter
 * @scenario         Generate DH parameter
 * @apicovered       auth_generate_ecdhkey
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_generate_ecdhkey_hnd_n(void)
{
	security_ecdsa_mode curve = ECDSA_UNKNOWN;
	security_data px;
	security_data py;
	security_ecdh_param param = {curve, &px, &py};
	security_error res = auth_generate_ecdhkey(NULL, UTC_CRYPTO_KEY_NAME, &param);

	TC_ASSERT_EQ("auth_generate_ecdhkey_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_generate_ecdhkey_name_n
 * @brief            Generate DH parameter
 * @scenario         Generate DH parameter
 * @apicovered       auth_generate_ecdhkey
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_generate_ecdhkey_name_n(void)
{
	security_ecdsa_mode curve = ECDSA_UNKNOWN;
	security_data px;
	security_data py;
	security_ecdh_param param = {curve, &px, &py};
	security_error res = auth_generate_ecdhkey(g_hnd, NULL, &param);

	TC_ASSERT_EQ("auth_generate_ecdhkey_name_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_generate_ecdhkey_param_n
 * @brief            Generate DH parameter
 * @scenario         Generate DH parameter
 * @apicovered       auth_generate_ecdhkey
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_generate_ecdhkey_param_n(void)
{
	security_error res = auth_generate_ecdhkey(g_hnd, UTC_CRYPTO_KEY_NAME, NULL);

	TC_ASSERT_EQ("auth_generate_ecdhkey_param_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_compute_ecdhkey_p
 * @brief            Compute ECDH key
 * @scenario         Compute ECDH key
 * @apicovered       auth_compute_ecdhkey
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_compute_ecdhkey_p(void)
{
	US_DEFINE_DATA(pubkey_x, "Public key data");
	US_DEFINE_DATA(pubkey_y, "Public Y key data");
	security_data secret = {NULL, 0};

	int i = 0;
	for (; i < sizeof(g_ecdsa_mode_table)/sizeof(security_ecdsa_mode); i++) {
		security_ecdh_param param = {g_ecdsa_mode_table[i], &pubkey_x, &pubkey_y};
		security_error res = auth_compute_ecdhkey(g_hnd, UTC_CRYPTO_KEY_NAME, &param, &secret);

		TC_ASSERT_EQ("auth_compute_ecdhkey_p", res, SECURITY_OK);
		TC_SUCCESS_RESULT();
	}
}

/**
 * @testcase         utc_auth_compute_ecdhkey_hnd_n
 * @brief            Compute ECDH key
 * @scenario         Compute ECDH key
 * @apicovered       auth_compute_ecdhkey
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_compute_ecdhkey_hnd_n(void)
{
	US_DEFINE_DATA(pubkey_x, "Public key data");
	US_DEFINE_DATA(pubkey_y, "Public Y key data");
	security_ecdh_param param = {g_ecdsa_mode_table[0], &pubkey_x, &pubkey_y};
	security_data secret = {NULL, 0};

	security_error res = auth_compute_ecdhkey(NULL, UTC_CRYPTO_KEY_NAME, &param, &secret);

	TC_ASSERT_EQ("auth_compute_ecdhkey_hnd_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_compute_ecdhkey_name_n
 * @brief            Compute ECDH key
 * @scenario         Compute ECDH key
 * @apicovered       auth_compute_ecdhkey
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_compute_ecdhkey_name_n(void)
{
	US_DEFINE_DATA(pubkey_x, "Public key data");
	US_DEFINE_DATA(pubkey_y, "Public Y key data");
	security_ecdh_param param = {g_ecdsa_mode_table[0], &pubkey_x, &pubkey_y};
	security_data secret = {NULL, 0};

	security_error res = auth_compute_ecdhkey(g_hnd, NULL, &param, &secret);

	TC_ASSERT_EQ("auth_compute_ecdhkey_name_n", res, SECURITY_INVALID_KEY_INDEX);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_compute_ecdhkey_param_n
 * @brief            Compute ECDH key
 * @scenario         Compute ECDH key
 * @apicovered       auth_compute_ecdhkey
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_compute_ecdhkey_param_n(void)
{
	security_data secret = {NULL, 0};
	security_error res = auth_compute_ecdhkey(g_hnd, UTC_CRYPTO_KEY_NAME, NULL, &secret);

	TC_ASSERT_EQ("auth_compute_ecdhkey_param_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         utc_auth_compute_ecdhkey_secret_n
 * @brief            Compute ECDH key
 * @scenario         Compute ECDH key
 * @apicovered       auth_compute_ecdhkey
 * @precondition     none
 * @postcondition    none
 */
static void utc_auth_compute_ecdhkey_secret_n(void)
{
	US_DEFINE_DATA(pubkey_x, "Public key data");
	US_DEFINE_DATA(pubkey_y, "Public Y key data");
	security_ecdh_param param = {g_ecdsa_mode_table[0], &pubkey_x, &pubkey_y};
	security_error res = auth_compute_ecdhkey(g_hnd, UTC_CRYPTO_KEY_NAME, &param, NULL);

	TC_ASSERT_EQ("auth_compute_ecdhkey_secret_n", res, SECURITY_INVALID_INPUT_PARAMS);
	TC_SUCCESS_RESULT();
}

void utc_auth_main(void)
{
	security_error res = security_init(&g_hnd);
	if (res < 0) {
		US_ERROR;
	}

	utc_auth_generate_random_p();
	utc_auth_generate_random_hnd_n();
	utc_auth_generate_random_input_n();
	utc_auth_generate_certificate_p();
	utc_auth_generate_certificate_hnd_n();
	utc_auth_generate_certificate_name_n();
	utc_auth_generate_certificate_name_n();
	utc_auth_set_certificate_p();
	utc_auth_set_certificate_hnd_n();
	utc_auth_set_certificate_name_n();
	utc_auth_set_certificate_input_n();
	utc_auth_get_certificate_p();
	utc_auth_get_certificate_hnd_n();
	utc_auth_get_certificate_name_n();
	utc_auth_get_certificate_input_n();
	utc_auth_remove_certificate_p();
	utc_auth_remove_certificate_hnd_n();
	utc_auth_remove_certificate_name_n();
	utc_auth_get_rsa_signature_p();
	utc_auth_get_rsa_signature_hnd_n();
	utc_auth_get_rsa_signature_param_n();
	utc_auth_get_rsa_signature_name_n();
	utc_auth_get_rsa_signature_hash_n();
	utc_auth_get_rsa_signature_sign_n();
	utc_auth_verify_rsa_signature_p();
	utc_auth_verify_rsa_signature_hnd_n();
	utc_auth_verify_rsa_signature_param_n();
	utc_auth_verify_rsa_signature_name_n();
	utc_auth_verify_rsa_signature_hash_n();
	utc_auth_verify_rsa_signature_sign_n();
	utc_auth_get_ecdsa_signature_p();
	utc_auth_get_ecdsa_signature_hnd_n();
	utc_auth_get_ecdsa_signature_param_n();
	utc_auth_get_ecdsa_signature_name_n();
	utc_auth_get_ecdsa_signature_hash_n();
	utc_auth_get_ecdsa_signature_sign_n();
	utc_auth_verify_ecdsa_signature_p();
	utc_auth_verify_ecdsa_signature_hnd_n();
	utc_auth_verify_ecdsa_signature_param_n();
	utc_auth_verify_ecdsa_signature_name_n();
	utc_auth_verify_ecdsa_signature_hash_n();
	utc_auth_verify_ecdsa_signature_sign_n();
	utc_auth_get_hash_p();
	utc_auth_get_hash_hnd_n();
	utc_auth_get_hash_mode_n();
	utc_auth_get_hash_input_n();
	utc_auth_get_hash_hash_n();
	utc_auth_get_hmac_p();
	utc_auth_get_hmac_hnd_n();
	utc_auth_get_hmac_mode_n();
	utc_auth_get_hmac_name_n();
	utc_auth_get_hmac_input_n();
	utc_auth_get_hmac_hmac_n();
	utc_auth_generate_dhparams_p();
	utc_auth_generate_dhparams_hnd_n();
	utc_auth_generate_dhparams_name_n();
	utc_auth_generate_dhparams_param_n();
	utc_auth_compute_dhparams_p();
	utc_auth_compute_dhparams_hnd_n();
	utc_auth_compute_dhparams_name_n();
	utc_auth_compute_dhparams_param_n();
	utc_auth_compute_dhparams_secret_n();
	utc_auth_generate_ecdhkey_p();
	utc_auth_generate_ecdhkey_hnd_n();
	utc_auth_generate_ecdhkey_name_n();
	utc_auth_generate_ecdhkey_param_n();
	utc_auth_compute_ecdhkey_p();
	utc_auth_compute_ecdhkey_hnd_n();
	utc_auth_compute_ecdhkey_name_n();
	utc_auth_compute_ecdhkey_param_n();
	utc_auth_compute_ecdhkey_secret_n();

	res = security_deinit(g_hnd);
	if (res != SECURITY_OK) {
		US_ERROR;
	}
}
