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
#define APP_SUCCESS_MESSAGE "Example finished successfully!"
#define APP_ERROR_MESSAGE "Example exited with error!"

#define CRYPTO_EXAMPLE_HMAC_TEXT_SIZE (100)
#define CRYPTO_EXAMPLE_HMAC_KEY_SIZE (32)

#define TAG "mbedtls"

/* Below text is used as plaintext for signing/verifiction */
static uint8_t m_plain_text[CRYPTO_EXAMPLE_HMAC_TEXT_SIZE] = {
	"Example string to demonstrate basic usage of HMAC signing/verification."
};

static uint8_t hmac[CRYPTO_EXAMPLE_HMAC_KEY_SIZE];

static psa_key_id_t key_id;

static int crypto_init(void)
{
	psa_status_t status;

	/* Initialize PSA Crypto */
	status = psa_crypto_init();
	if (status != PSA_SUCCESS)
		return APP_ERROR;

	return APP_SUCCESS;
}

static int crypto_finish(void)
{
	psa_status_t status;

	/* Destroy the key handle */
	status = psa_destroy_key(key_id);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_destroy_key failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	return APP_SUCCESS;
}

int generate_key(void)
{
	psa_status_t status;

	BK_LOGI(TAG, "Generating random HMAC key...\r\n");

	/* Configure the key attributes */
	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;

	psa_set_key_usage_flags(&key_attributes,
				PSA_KEY_USAGE_VERIFY_HASH | PSA_KEY_USAGE_SIGN_HASH);
	psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
	psa_set_key_algorithm(&key_attributes, PSA_ALG_HMAC(PSA_ALG_SHA_256));
	psa_set_key_type(&key_attributes, PSA_KEY_TYPE_HMAC);
	psa_set_key_bits(&key_attributes, 256);

	/* Generate a random key. The key is not exposed to the application,
	 * we can use it to encrypt/decrypt using the key handle
	 */
	status = psa_generate_key(&key_attributes, &key_id);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_generate_key failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* After the key handle is acquired the attributes are not needed */
	psa_reset_key_attributes(&key_attributes);

	BK_LOGI(TAG, "HMAC key generated successfully!\r\n");

	return APP_SUCCESS;
}

int hmac_sign(void)
{
	uint32_t olen;
	psa_status_t status;
	psa_mac_operation_t operation = PSA_MAC_OPERATION_INIT;

	BK_LOGI(TAG, "Signing using HMAC ...\r\n");

	/* Initialize the HMAC signing operation */
	status = psa_mac_sign_setup(&operation, key_id, PSA_ALG_HMAC(PSA_ALG_SHA_256));
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_mac_sign_setup failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Perform the HMAC signing */
	status = psa_mac_update(&operation, m_plain_text, sizeof(m_plain_text));
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_mac_update failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Finalize the HMAC signing */
	status = psa_mac_sign_finish(&operation, hmac, sizeof(hmac), (size_t *)&olen);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_mac_sign_finish failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	BK_LOGI(TAG, "Signing successful!\r\n");

	return APP_SUCCESS;
}

int hmac_verify(void)
{
	psa_status_t status;
	psa_mac_operation_t operation = PSA_MAC_OPERATION_INIT;

	BK_LOGI(TAG, "Verifying the HMAC signature...\r\n");

	/* Initialize the HMAC verification operation */
	status = psa_mac_verify_setup(&operation, key_id, PSA_ALG_HMAC(PSA_ALG_SHA_256));
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_mac_verify_setup failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Perform the HMAC verification */
	status = psa_mac_update(&operation, m_plain_text, sizeof(m_plain_text));
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_mac_update failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Finalize the HMAC verification */
	status = psa_mac_verify_finish(&operation, hmac, sizeof(hmac));
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_mac_verify_finish failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	BK_LOGI(TAG, "HMAC verified successfully!\r\n");

	return APP_SUCCESS;
}

int tz_hmac_test_main(void)
{
	int status;

	BK_LOGI(TAG, "Starting HMAC example...\r\n");

	status = crypto_init();
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	status = generate_key();
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	status = hmac_sign();
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	status = hmac_verify();
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
