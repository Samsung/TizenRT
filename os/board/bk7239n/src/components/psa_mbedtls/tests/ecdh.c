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
#include <string.h>
#include <psa/crypto.h>
#include <psa/crypto_extra.h>

#define APP_SUCCESS		(0)
#define APP_ERROR		(-1)
#define APP_SUCCESS_MESSAGE "Example finished successfully!"
#define APP_ERROR_MESSAGE "Example exited with error!"

#define TAG "mbedtls"

#define CRYPTO_EXAMPLE_ECDH_KEY_BITS (256)
#define CRYPTO_EXAMPLE_ECDH_PUBLIC_KEY_SIZE (65)

/* Buffers to hold Bob's and Alice's public keys */
static uint8_t m_pub_key_bob[CRYPTO_EXAMPLE_ECDH_PUBLIC_KEY_SIZE];
static uint8_t m_pub_key_alice[CRYPTO_EXAMPLE_ECDH_PUBLIC_KEY_SIZE];

/* Buffers to hold Bob's and Alice's secret values */
static uint8_t m_secret_alice[32];
static uint8_t m_secret_bob[32];

psa_key_id_t key_id_alice;
psa_key_id_t key_id_bob;

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
	status = psa_destroy_key(key_id_alice);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_destroy_key failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Destroy the key handle */
	status = psa_destroy_key(key_id_bob);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_destroy_key failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	return APP_SUCCESS;
}

static int create_ecdh_keypair(psa_key_id_t *key_id)
{
	psa_status_t status;
	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;

	/* Crypto settings for ECDH using the SHA256 hashing algorithm,
	 * the secp256r1 curve
	 */
	psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_DERIVE);
	psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
	psa_set_key_algorithm(&key_attributes, PSA_ALG_ECDH);
	psa_set_key_type(&key_attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1));
	psa_set_key_bits(&key_attributes, 256);

	/* Generate a key pair */
	status = psa_generate_key(&key_attributes, key_id);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_generate_key failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	psa_reset_key_attributes(&key_attributes);

	BK_LOGI(TAG, "ECDH keypair created successfully!\r\n");

	return APP_SUCCESS;
}

static int export_ecdh_public_key(psa_key_id_t *key_id, uint8_t *buff, size_t buff_size)
{
	size_t olen;
	psa_status_t status;

	/* Export the public key */
	status = psa_export_public_key(*key_id, buff, buff_size, &olen);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_export_public_key failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	BK_LOGI(TAG, "ECDH public key exported successfully!\r\n");

	return APP_SUCCESS;
}

static int calculate_ecdh_secret(psa_key_id_t *key_id,
								 uint8_t *pub_key,
								 size_t pub_key_len,
								 uint8_t *secret,
								 size_t secret_len)
{
	uint32_t output_len;
	psa_status_t status;

	/* Perform the ECDH key exchange to calculate the secret */
	status = psa_raw_key_agreement(
		PSA_ALG_ECDH, *key_id, pub_key, pub_key_len, secret, secret_len, (size_t *)&output_len);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_raw_key_agreement failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	BK_LOGI(TAG, "ECDH secret calculated successfully!\r\n");

	return APP_SUCCESS;
}

static int compare_secrets(void)
{
	int status;

	BK_LOGI(TAG, "Comparing the secret values of Alice and Bob\r\n");

	status = memcmp(m_secret_bob, m_secret_alice, sizeof(m_secret_alice));
	if (status != 0) {
		BK_LOGI(TAG, "Error: Secret values don't match!\r\n");
		return APP_ERROR;
	}

	BK_LOGI(TAG, "The secret values of Alice and Bob match!\r\n");

	return APP_SUCCESS;
}

int ecdh_main(void)
{
	int status;

	BK_LOGI(TAG, "Starting ECDH example...\r\n");
	/* Init crypto */
	status = crypto_init();
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	/* Create the ECDH key pairs for Alice and Bob  */
	BK_LOGI(TAG, "Creating ECDH key pair for Alice\r\n");
	status = create_ecdh_keypair(&key_id_alice);
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	BK_LOGI(TAG, "Creating ECDH key pair for Bob\r\n");
	status = create_ecdh_keypair(&key_id_bob);
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	/* Export the ECDH public keys */
	BK_LOGI(TAG, "Export Alice's public key\r\n");
	status = export_ecdh_public_key(&key_id_alice, m_pub_key_alice, sizeof(m_pub_key_alice));
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	BK_LOGI(TAG, "Export Bob's public key\r\n");
	status = export_ecdh_public_key(&key_id_bob, m_pub_key_bob, sizeof(m_pub_key_bob));
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	/* Calculate the secret value for each participant */
	BK_LOGI(TAG, "Calculating the secret value for Alice\r\n");
	status = calculate_ecdh_secret(&key_id_alice,
				       m_pub_key_bob,
				       sizeof(m_pub_key_bob),
				       m_secret_alice,
				       sizeof(m_secret_alice));
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	BK_LOGI(TAG, "Calculating the secret value for Bob\r\n");
	status = calculate_ecdh_secret(&key_id_bob,
				       m_pub_key_alice,
				       sizeof(m_pub_key_alice),
				       m_secret_bob,
				       sizeof(m_secret_bob));
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	/* Verify that the calculated secrets match */
	status = compare_secrets();
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
