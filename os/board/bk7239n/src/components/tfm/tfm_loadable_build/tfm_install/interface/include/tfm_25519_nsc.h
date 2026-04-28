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

#include "psa/crypto_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	PSA_25519_SUCCESS,
	PSA_25519_NOT_INITIALIZED,
	PSA_25519_INVALID_ARGS,
	PSA_25519_LENGTH_NO_ALIGN,
	PSA_25519_MALLOC_FAIL,
	PSA_25519_PARAMETER_FAIL,
	PSA_25519_FAIL,
} psa_25519_result_e;

typedef struct _key_data_s {
	void *data;
	uint32_t data_len;
	void *priv;
	uint32_t priv_len;
} key25519_data_t;

int psa_25519_generate_key(uint32_t key_idx);
int psa_25519_set_key(uint32_t key_idx, key25519_data_t *key, key25519_data_t *prikey);
int psa_25519_get_key(uint32_t key_idx, key25519_data_t *key);
int psa_25519_remove_key(uint32_t key_idx);
int psa_25519_sign_hash(uint32_t key_idx, key25519_data_t *hash, key25519_data_t *sign);
int psa_25519_verify_hash(uint32_t key_idx, key25519_data_t *hash, key25519_data_t *sign);

#ifdef __cplusplus
}
#endif