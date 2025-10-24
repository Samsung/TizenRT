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
#include <psa/crypto.h>
#include <psa/crypto_extra.h>

#define APP_SUCCESS		(0)
#define APP_ERROR		(-1)
#define APP_SUCCESS_MESSAGE "Example finished successfully!"
#define APP_ERROR_MESSAGE "Example exited with error!"

#define TAG "mbedtls"

#define CRYPTO_EXAMPLE_SHA256_TEXT_SIZE (150)
#define CRYPTO_EXAMPLE_SHA256_SIZE (32)

/* Below text is used as plaintext for computing/verifying the hash. */
static uint8_t m_plain_text[CRYPTO_EXAMPLE_SHA256_TEXT_SIZE] = {
	"Example string to demonstrate basic usage of SHA256."
	"That uses single and multi-part PSA crypto API's to "
	"perform a SHA-256 hashing operation."
};

static uint8_t m_hash[CRYPTO_EXAMPLE_SHA256_SIZE];

static int crypto_init(void)
{
	psa_status_t status;

	/* Initialize PSA Crypto */
	status = psa_crypto_init();
	if (status != PSA_SUCCESS)
		return APP_ERROR;

	return APP_SUCCESS;
}

static int hash_singlepart_sha256(void)
{
	uint32_t olen;
	psa_status_t status;

	BK_LOGI(TAG, "Hashing using SHA256...\r\n");

	/* Calculate the SHA256 hash */
	status = psa_hash_compute(
		PSA_ALG_SHA_256, m_plain_text, sizeof(m_plain_text), m_hash, sizeof(m_hash), (size_t *)&olen);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_hash_compute failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	BK_LOGI(TAG, "Hashing successful!\r\n");

	return APP_SUCCESS;
}

static int hash_multipart_sha256(void)
{
	uint32_t olen;
	psa_status_t status;
	uint8_t *input_ptr = m_plain_text;
	psa_hash_operation_t hash_operation = {0};

	BK_LOGI(TAG, "Hashing using multi-part SHA256...\r\n");

	/* Setup a multipart hash operation */
	status = psa_hash_setup(&hash_operation, PSA_ALG_SHA_256);
	if (status != PSA_SUCCESS) {
		BK_LOGE(TAG, "Could not setup the hash operation! Error %d\r\n", status);
		return APP_ERROR;
	}

	/* Feed the chunks of the input data to the PSA driver */
	status = psa_hash_update(&hash_operation, input_ptr, 42);
	if (status != PSA_SUCCESS) {
		BK_LOGE(TAG, "Could not hash the next chunk! Error %d\r\n", status);
		return APP_ERROR;

	}
	BK_LOGI(TAG, "Added %d bytes\r\n", 42);
	input_ptr += 42;


	status = psa_hash_update(&hash_operation, input_ptr, 58);
	if (status != PSA_SUCCESS) {
		BK_LOGE(TAG, "Could not hash the next chunk! Error %d\r\n", status);
		return APP_ERROR;

	}
	BK_LOGI(TAG, "Added %d bytes\r\n", 58);
	input_ptr += 58;

	status = psa_hash_update(&hash_operation, input_ptr, 50);
	if (status != PSA_SUCCESS) {
		BK_LOGE(TAG, "Could not hash the next chunk! Error %d\r\n", status);
		return APP_ERROR;
	}
	BK_LOGI(TAG, "Added %d bytes\r\n", 50);

	status = psa_hash_finish(&hash_operation, m_hash, sizeof(m_hash), (size_t *)&olen);
	if (status != PSA_SUCCESS) {
		BK_LOGE(TAG, "Could not finish the hash operation! Error %d\r\n", status);
		return APP_ERROR;
	}

	BK_LOGI(TAG, "Hashing successful!\r\n");

	return APP_SUCCESS;
}

static int verify_sha256(void)
{
	psa_status_t status;

	BK_LOGI(TAG, "Verifying the SHA256 hash...\r\n");

	/* Verify the hash */
	status = psa_hash_compare(
		PSA_ALG_SHA_256, m_plain_text, sizeof(m_plain_text), m_hash, sizeof(m_hash));
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_hash_compare failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	BK_LOGI(TAG, "SHA256 verification successful!\r\n");

	return APP_SUCCESS;
}

int sha256_main(void)
{
	int status;

	BK_LOGI(TAG, "Starting SHA256 example...\r\n");

	status = crypto_init();
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	status = hash_singlepart_sha256();
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	status = verify_sha256();
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	/* Reset the hash */
	memset(m_hash, 0, sizeof(m_hash));

	status = hash_multipart_sha256();
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	status = verify_sha256();
	if (status != APP_SUCCESS) {
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	BK_LOGI(TAG, APP_SUCCESS_MESSAGE);

	return APP_SUCCESS;
}
