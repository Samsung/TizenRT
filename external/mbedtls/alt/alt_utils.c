/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#include <tinyara/seclink.h>
#include <tinyara/security_hal.h>
#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif
#if defined(MBEDTLS_ECP_C)
#include "mbedtls/ecp.h"
#endif

hal_key_type alt_get_keytype(unsigned int curve)
{
switch (curve) {
	//case MBEDTLS_ECP_DP_SECP192R1:
	//case MBEDTLS_ECP_DP_SECP224R1:
	case MBEDTLS_ECP_DP_SECP256R1:
		return HAL_KEY_ECC_SEC_P256R1;
	case MBEDTLS_ECP_DP_SECP384R1:
		return HAL_KEY_ECC_SEC_P384R1;
	case MBEDTLS_ECP_DP_SECP521R1:
		return HAL_KEY_ECC_SEC_P512R1;
	case MBEDTLS_ECP_DP_BP256R1:
		return HAL_KEY_ECC_BRAINPOOL_P256R1;
	case MBEDTLS_ECP_DP_BP384R1:
		return HAL_KEY_ECC_BRAINPOOL_P384R1;
	case MBEDTLS_ECP_DP_BP512R1:
		return HAL_KEY_ECC_BRAINPOOL_P512R1;
	//case MBEDTLS_ECP_DP_CURVE25519:
	//case MBEDTLS_ECP_DP_SECP192K1:
	//case MBEDTLS_ECP_DP_SECP224K1:
	//case MBEDTLS_ECP_DP_SECP256K1:
	default:
		break;
	}
	return HAL_KEY_UNKNOWN;
}

hal_ecdsa_curve alt_get_curve(mbedtls_ecp_group_id curve)
{
	switch (curve) {
	case MBEDTLS_ECP_DP_SECP192R1:
		return HAL_ECDSA_SEC_P192R1;
	case MBEDTLS_ECP_DP_SECP224R1:
		return HAL_ECDSA_SEC_P224R1;
	case MBEDTLS_ECP_DP_SECP256R1:
		return HAL_ECDSA_SEC_P256R1;
	case MBEDTLS_ECP_DP_SECP384R1:
		return HAL_ECDSA_SEC_P384R1;
	case MBEDTLS_ECP_DP_SECP521R1:
		return HAL_ECDSA_SEC_P521R1;
	case MBEDTLS_ECP_DP_BP256R1:
		return HAL_ECDSA_BRAINPOOL_P256R1;
	default:
		break;
	}
	return HAL_ECDSA_UNKNOWN;
}

int alt_set_key(sl_ctx shnd,
				hal_key_type key_type,
				hal_data *pubkey,
				hal_data *prikey, int from)
{
	int ret = 0;
	int key_idx = from;

	while (1) {
		ret = sl_set_key(shnd, key_type, key_idx, pubkey, prikey);
		if (ret != SECLINK_OK) {
			if (ret == SECLINK_KEY_IN_USE) {
				key_idx++;
				if (key_idx == SECLINK_MAX_RAM_KEY) {
					return -1;
				}
				continue;
			}
		}
		break;
	}
	return key_idx;
}

int alt_gen_key(sl_ctx shnd, hal_key_type key_type, int from)
{
	int ret = 0;
	int key_idx = from;

	while (1) {
		ret = sl_generate_key(shnd, key_type, key_idx);
		if (ret != SECLINK_OK) {
			if (ret == SECLINK_KEY_IN_USE) {
				key_idx++;
				if (key_idx == SECLINK_MAX_RAM_KEY) {
					return -1;
				}
				continue;
			}
		}
		break;
	}
	return key_idx;
}

void alt_print_buffer(char *data, int buf_len, const char *message)
{
	printf("================================\n");
	printf("%s length(%d)\n", message, buf_len);
	printf("--------------------------------\n");
	for (int i = 0; i < buf_len; i++) {
		if (i != 0 && i % 16 == 0) {
			printf("\n");
		}
		printf("%02x, ", data[i]);
	}
	printf("\n");
	printf("================================\n");
}
