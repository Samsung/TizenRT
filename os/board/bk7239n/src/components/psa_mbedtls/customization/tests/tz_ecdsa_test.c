// Copyright 2024-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <stdlib.h>
#include <psa/crypto.h>
#include <psa/crypto_extra.h>

#define APP_SUCCESS		(0)
#define APP_ERROR		(-1)
#define APP_SUCCESS_MESSAGE "Example finished successfully!\r\n"
#define APP_ERROR_MESSAGE "Example exited with error!\r\n"

#define TAG "mbedtls"

#define CRYPTO_EXAMPLE_ECDSA_TEXT_SIZE (100)

#define CRYPTO_EXAMPLE_ECDSA_PUBLIC_KEY_SIZE (65)
#define CRYPTO_EXAMPLE_ECDSA_SIGNATURE_SIZE (64)
#define CRYPTO_EXAMPLE_ECDSA_HASH_SIZE (32)

/* Below text is used as plaintext for signing/verification */
static uint8_t m_plain_text[CRYPTO_EXAMPLE_ECDSA_TEXT_SIZE] = {
	"Example string to demonstrate basic usage of ECDSA."
};

static uint8_t m_pub_key[CRYPTO_EXAMPLE_ECDSA_PUBLIC_KEY_SIZE];

static uint8_t m_signature[CRYPTO_EXAMPLE_ECDSA_SIGNATURE_SIZE];
static uint8_t m_hash[CRYPTO_EXAMPLE_ECDSA_HASH_SIZE];

static psa_key_id_t keypair_id;
static psa_key_id_t pub_key_id;

static int crypto_init(void)
{
	psa_status_t status;

	/* Initialize PSA Crypto */
	status = psa_crypto_init();
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_crypto_init failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	return APP_SUCCESS;
}

static int crypto_finish(void)
{
	psa_status_t status;

	/* Destroy the key handle */
	status = psa_destroy_key(keypair_id);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_destroy_key failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	status = psa_destroy_key(pub_key_id);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_destroy_key failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	return APP_SUCCESS;
}

static int generate_ecdsa_keypair(void)
{
	psa_status_t status;
	size_t olen;

	BK_LOGI(TAG, "Generating random ECDSA keypair...\r\n");

	/* Configure the key attributes */
	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;

	/* Configure the key attributes */
	psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_SIGN_HASH);
	psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
	psa_set_key_algorithm(&key_attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_256));
	psa_set_key_type(&key_attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1));
	psa_set_key_bits(&key_attributes, 256);

	/* Generate a random keypair. The keypair is not exposed to the application,
	 * we can use it to sign hashes.
	 */
	status = psa_generate_key(&key_attributes, &keypair_id);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_generate_key failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Export the public key */
	status = psa_export_public_key(keypair_id, m_pub_key, sizeof(m_pub_key), &olen);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_export_public_key failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Reset key attributes and free any allocated resources. */
	psa_reset_key_attributes(&key_attributes);

	return APP_SUCCESS;
}

static int import_ecdsa_pub_key(void)
{
	/* Configure the key attributes */
	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
	psa_status_t status;

	/* Configure the key attributes */
	psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_VERIFY_HASH);
	psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
	psa_set_key_algorithm(&key_attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_256));
	psa_set_key_type(&key_attributes, PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1));
	psa_set_key_bits(&key_attributes, 256);

	status = psa_import_key(&key_attributes, m_pub_key, sizeof(m_pub_key), &pub_key_id);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_import_key failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Reset key attributes and free any allocated resources. */
	psa_reset_key_attributes(&key_attributes);

	return APP_SUCCESS;
}

static int sign_message(void)
{
	uint32_t output_len;
	psa_status_t status;

	BK_LOGI(TAG, "Signing a message using ECDSA...\r\n");

	/* Compute the SHA256 hash*/
	status = psa_hash_compute(PSA_ALG_SHA_256,
				  m_plain_text,
				  sizeof(m_plain_text),
				  m_hash,
				  sizeof(m_hash),
				  (size_t *)&output_len);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_hash_compute failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Sign the hash */
	status = psa_sign_hash(keypair_id,
			       PSA_ALG_ECDSA(PSA_ALG_SHA_256),
			       m_hash,
			       sizeof(m_hash),
			       m_signature,
			       sizeof(m_signature),
			       (size_t *)&output_len);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_sign_hash failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	BK_LOGI(TAG, "Message signed successfully!\r\n");

	return APP_SUCCESS;
}

static int verify_message(void)
{
	psa_status_t status;

	BK_LOGI(TAG, "Verifying ECDSA signature...\r\n");

	/* Verify the signature of the hash */
	status = psa_verify_hash(pub_key_id,
				 PSA_ALG_ECDSA(PSA_ALG_SHA_256),
				 m_hash,
				 sizeof(m_hash),
				 m_signature,
				 sizeof(m_signature));
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_verify_hash failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	BK_LOGI(TAG, "Signature verification was successful!\r\n");

	return APP_SUCCESS;
}

int tz_ecdsa_test_main(void)
{
	int status;

	BK_LOGI(TAG, "Starting ECDSA example...\r\n");

	status = crypto_init();
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	status = generate_ecdsa_keypair();
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	status = import_ecdsa_pub_key();
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	status = sign_message();
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	status = verify_message();
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	status = crypto_finish();
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	BK_LOGI(TAG, APP_SUCCESS_MESSAGE);

	return APP_SUCCESS;
}
