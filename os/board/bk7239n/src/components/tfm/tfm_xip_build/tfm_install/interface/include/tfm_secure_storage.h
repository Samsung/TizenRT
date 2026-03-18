// Copyright 2022-2025 Beken
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

#pragma once

#include <soc/soc.h>

/* Define data types */
#define SS_TYPE_KEY                 (1)
#define SS_TYPE_CERT                (2)
#define SS_TYPE_DATA                (3)

/* Define permission types */
#define SS_PERM_READ_ONLY           (1)
#define SS_PERM_READ_WRITE          (2)
#define SS_PERM_EXPOSED             (3)

#define HAL_MAX_ECP_KEY_SIZE      (132)

//key and certifications : first 32 stored in Flash, next 32 stored in RAM; data : total 33 slots and stored in flash.
#define CONFIG_SS_KEY_INDEX_MAX   (64)
#define CONFIG_SS_CERT_INDEX_MAX  (64)
#define CONFIG_SS_DATA_INDEX_MAX  (33)
#define SS_RAM_INDEX_START        (32)  // Starting index for RAM storage

typedef enum {
	SS_SUCCESS,
	SS_NOT_INITIALIZED,
	SS_INVALID_ARGS,
	SS_INVALID_REQUEST,
	SS_INVALID_SLOT_RANGE,
	SS_INVALID_SLOT_TYPE, // ex. request to save key into cert slot
	SS_EMPTY_SLOT,
	SS_BAD_KEY,	  // only public key can be returned
	SS_BAD_KEY_PAIR, // public and private keys do not match
	SS_BAD_KEY_TYPE, // the type when key was set and the type to get key are not matched.
	SS_BAD_CERT,
	SS_BAD_CERTKEY_PAIR, // certificate and key do not match
	SS_NOT_ENOUGH_MEMORY,
	SS_ALLOC_FAIL,
	SS_KEY_IN_USE,
	SS_CERT_IN_USE,
	SS_DATA_IN_USE,
	SS_NOT_SUPPORTED,
	SS_NOT_IMPLEMENTED,
	SS_BUSY,
	SS_FAIL,
    SS_CRYPTO_FAIL,
    SS_FLASH_FAIL,
} SS_result_e;

typedef struct {
	void *data;
	uint32_t data_len;
	void *priv;
	uint32_t priv_len;
} hal_data_type_t;

/**
 * @brief Key read function for security
 *
 * @param index Key index (0-63)
 * @param key Key data
 * @param key_len Key length
 *
 * @return 0 on success, other values on failure
 */
int ss_read_key_security(uint32_t index, uint8_t *key, uint32_t *key_len);

/**
 * @brief Key read function
 *
 * @param index Key index (0-63)
 * @param key Output parameter to store the read key
 * @param key_len Input parameter: key buffer size; Output parameter: actual read key length
 *
 * @return 0 on success, other values on failure
 */
int ss_read_key(uint32_t index, uint8_t *key, uint32_t *key_len);

/**
 * @brief Certificate write function
 *
 * @param index Certificate index (32-63)
 * @param cert Certificate data
 * @param cert_len Certificate length
 *
 * @return 0 on success, other values on failure
 */
int ss_write_cert(uint32_t index, uint8_t *cert, uint32_t cert_len);

/**
 * @brief Certificate read function
 *
 * @param index Certificate index (0-63)
 * @param cert Output parameter to store the read certificate
 * @param cert_len Input parameter: cert buffer size; Output parameter: actual read certificate length
 *
 * @return 0 on success, other values on failure
 */
int ss_read_cert(uint32_t index, uint8_t *cert, uint32_t *cert_len);

/**
 * @brief Certificate delete function
 *
 * @param index Certificate index (32-63)
 *
 * @return 0 on success, other values on failure
 */
int ss_delete_cert(uint32_t index);

/**
 * @brief Data write function
 *
 * @param index Data index (0-32)
 * @param data Data content
 * @param data_len Data length
 *
 * @return 0 on success, other values on failure
 */
int ss_write_data(uint32_t index, uint8_t *data, uint32_t data_len);

/**
 * @brief Data read function
 *
 * @param index Data index (0-32)
 * @param data Output parameter to store the read data
 * @param data_len Input parameter: data buffer size; Output parameter: actual read data length
 *
 * @return 0 on success, other values on failure
 */
int ss_read_data(uint32_t index, uint8_t *data, uint32_t *data_len);

/**
 * @brief Data delete function
 *
 * @param index Data index (0-32)
 *
 * @return 0 on success, other values on failure
 */
int ss_delete_data(uint32_t index);

int ss_write_key(uint32_t index, uint8_t *key, uint32_t key_len);
int ss_delete_key(uint32_t index);