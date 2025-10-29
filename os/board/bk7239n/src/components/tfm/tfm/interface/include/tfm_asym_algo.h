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

/** 
 * @brief Perform ECDSA signature using P256r1 elliptic curve
 * 
 * This function generates an ECDSA signature for the given message data using
 * the private key identified by key_idx and the P256 elliptic curve algorithm.
 * 
 * @param message Pointer to the message data to be signed
 * @param key_idx Index of the private key to use for signing
 * @param sign Pointer to store the generated signature
 * @return int Returns 0 on success, negative error code on failure
 */
int psa_ecdsa_sign_p256(hal_data_type_t *message, uint32_t key_idx, hal_data_type_t *sign);

/**
 * @brief Verify ECDSA signature using P256r1 elliptic curve
 * 
 * This function verifies an ECDSA signature for the given message data using
 * the public key identified by key_idx and the P256 elliptic curve algorithm.
 * 
 * @param message Pointer to the message data to be verified
 * @param key_idx Index of the public key to use for verification
 * @param sign Pointer to the signature to be verified
 * @return int Returns 0 on successful verification, negative error code on failure
 */
int psa_ecdsa_verify_p256( hal_data_type_t *message, uint32_t key_idx, hal_data_type_t *sign);
