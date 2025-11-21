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

#ifdef __cplusplus
extern "C" {
#endif

typedef int32_t psa_status_t;

typedef struct {
	const unsigned char *salt;
	size_t salt_len;
	const unsigned char *ikm;
	size_t ikm_len;
	const unsigned char *info;
	size_t info_len;
}hkdf_input_t;

typedef struct {
	unsigned char *okm;
	size_t okm_len;
}hkdf_output_t;

psa_status_t psa_mbedtls_hkdf_nsc( uint8_t type, hkdf_input_t *in, hkdf_output_t *out );

#ifdef __cplusplus
}
#endif
