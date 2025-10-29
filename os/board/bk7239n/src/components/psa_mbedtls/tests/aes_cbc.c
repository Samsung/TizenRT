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

#include <string.h>
#include <os/os.h>
#include <os/mem.h>
#include "components/log.h"
#include "common/bk_include.h"
#include "psa/crypto.h"
#include "psa/crypto_extra.h"

#define APP_SUCCESS    (0)
#define APP_ERROR      (-1)
#define APP_SUCCESS_MESSAGE "Example finished successfully!"
#define APP_ERROR_MESSAGE "Example exited with error!"

#define TAG "mbedtls"

#define CRYPTO_EXAMPLE_AES_MAX_TEXT_SIZE (64)
#define CRYPTO_EXAMPLE_AES_BLOCK_SIZE (16)

static uint8_t m_iv[CRYPTO_EXAMPLE_AES_BLOCK_SIZE];

/* Below text is used as plaintext for encryption/decryption */
static uint8_t m_plain_text[CRYPTO_EXAMPLE_AES_MAX_TEXT_SIZE] = {
	"Example string to demonstrate basic usage of AES CBC mode."
};

static uint8_t m_encrypted_text[CRYPTO_EXAMPLE_AES_MAX_TEXT_SIZE];
static uint8_t m_decrypted_text[CRYPTO_EXAMPLE_AES_MAX_TEXT_SIZE];

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

static int generate_key(void)
{
	psa_status_t status;

	BK_LOGI(TAG, "Generating random AES key...\r\n");

	/* Configure the key attributes */
	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;

	psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
	psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
	psa_set_key_algorithm(&key_attributes, PSA_ALG_CBC_NO_PADDING);
	psa_set_key_type(&key_attributes, PSA_KEY_TYPE_AES);
	psa_set_key_bits(&key_attributes, 128);

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

	BK_LOGI(TAG, "AES key generated successfully!\r\n");

	return APP_SUCCESS;
}

static int encrypt_cbc_aes(void)
{
	uint32_t olen;
	psa_status_t status;
	psa_cipher_operation_t operation = PSA_CIPHER_OPERATION_INIT;

	BK_LOGI(TAG, "Encrypting using AES CBC MODE...\r\n");

	/* Setup the encryption operation */
	status = psa_cipher_encrypt_setup(&operation, key_id, PSA_ALG_CBC_NO_PADDING);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_cipher_encrypt_setup failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Generate an IV */
	status = psa_cipher_generate_iv(&operation, m_iv, sizeof(m_iv), (size_t *)&olen);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_cipher_generate_iv failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Perform the encryption */
	status = psa_cipher_update(&operation, m_plain_text,
				   sizeof(m_plain_text), m_encrypted_text,
				   sizeof(m_encrypted_text), (size_t *)&olen);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_cipher_update failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Finalize the encryption */
	status = psa_cipher_finish(&operation, m_encrypted_text + olen,
				   sizeof(m_encrypted_text) - olen,
				   (size_t *)&olen);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_cipher_finish failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	BK_LOGI(TAG, "Encryption successful!\r\n");

	/* Clean up cipher operation context */
	psa_cipher_abort(&operation);

	return APP_SUCCESS;
}

static int decrypt_cbc_aes(void)
{
	uint32_t olen;
	psa_status_t status;
	psa_cipher_operation_t operation = PSA_CIPHER_OPERATION_INIT;

	BK_LOGI(TAG, "Decrypting using AES CBC MODE...\r\n");

	/* Setup the decryption operation */
	status = psa_cipher_decrypt_setup(&operation, key_id, PSA_ALG_CBC_NO_PADDING);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_cipher_decrypt_setup failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Set the IV generated in encryption */
	status = psa_cipher_set_iv(&operation, m_iv, sizeof(m_iv));
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_cipher_set_iv failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Perform the decryption */
	status = psa_cipher_update(&operation, m_encrypted_text,
				   sizeof(m_encrypted_text), m_decrypted_text,
				   sizeof(m_decrypted_text), (size_t *)&olen);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_cipher_update failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Finalize the decryption */
	status = psa_cipher_finish(&operation, m_decrypted_text + olen,
				   sizeof(m_decrypted_text) - olen,
				   (size_t *)&olen);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_cipher_finish failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Check the validity of the decryption */
	if (memcmp(m_decrypted_text,
				m_plain_text,
				CRYPTO_EXAMPLE_AES_MAX_TEXT_SIZE) != 0){

		BK_LOGI(TAG, "Error: Decrypted text doesn't match the plaintext\r\n");
		return APP_ERROR;
	}

	BK_LOGI(TAG, "Decryption successful!\r\n");

	/*  Clean up cipher operation context */
	psa_cipher_abort(&operation);

	return APP_SUCCESS;
}

int aes_cbc_main(void)
{
	int status;

    BK_LOGI(TAG, "Starting AES-CBC-NO-PADDING example...\r\n");

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

	status = encrypt_cbc_aes();
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	status = decrypt_cbc_aes();
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
