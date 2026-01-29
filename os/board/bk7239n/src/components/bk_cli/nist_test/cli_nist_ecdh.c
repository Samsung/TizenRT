// Copyright 2020-2025 Beken
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

#include "cli_nist_common.h"

#define MAX_SHARED_SECURE_LEN 66
#define HAL_MAX_ECP_DER_KEY   288

extern int armino_hal_ecdh_compute_shared_secret(hal_ecdh_data *ecdh_param, uint32_t key_idx, hal_data *shared_secret);
extern int armino_hal_be_der_ecdsa_private_key(hal_key_type mode, hal_data *prikey, hal_data *pubkey, hal_data *key, uint32_t priv_key_len, uint32_t pub_key_len);

uint8_t nist_ecdh_verify_callback(nist_handle_t *nist_handle)
{
    uint8_t sub_type = nist_handle->sub_type; //curve type
    BK_LOGI(NULL,"sub_type = %u\n", sub_type);

    hal_data QCAVSx = {0};
    hal_data QCAVSy = {0};
    hal_data dIUT   = {0};
    hal_data QIUTx  = {0};
    hal_data QIUTy  = {0};
    hal_data ZIUT   = {0}; //shared secret
    uint32_t key_idx = 32;
    hal_ecdh_data ecdh_param = {0};
    int result;

    QCAVSx.data = get_param_by_index(0, (uint16_t *)&QCAVSx.data_len);
    QCAVSy.data = get_param_by_index(1, (uint16_t *)&QCAVSy.data_len);
    dIUT.data   = get_param_by_index(2, (uint16_t *)&dIUT.data_len);
    QIUTx.data  = get_param_by_index(3, (uint16_t *)&QIUTx.data_len);
    QIUTy.data  = get_param_by_index(4, (uint16_t *)&QIUTy.data_len);
    ZIUT.data   = get_param_by_index(5, (uint16_t *)&ZIUT.data_len);


    ecdh_param.curve = HAL_ECDSA_SEC_P192R1 + sub_type;
    ecdh_param.pubkey_x = &QCAVSx;
    ecdh_param.pubkey_y = &QCAVSy;

    hal_data prikey = {0};
    prikey.data = os_malloc(HAL_MAX_ECP_DER_KEY);
    if (prikey.data == NULL) {
        BK_LOGI(NULL, "malloc failed\n");
        return NIST_FAIL;
    }

    result = armino_hal_be_der_ecdsa_private_key(HAL_KEY_ECC_SEC_P192R1 + sub_type, &dIUT, NULL, &prikey, dIUT.data_len, 0);
    if (result != HAL_SUCCESS) {
        BK_LOGI(NULL, "be der ecdsa private key failed\n");
        os_free(prikey.data);
        return NIST_FAIL;
    }

    result = armino_hal_set_key(HAL_KEY_ECC_SEC_P192R1 + sub_type, key_idx, NULL, &prikey);
    if (result != HAL_SUCCESS) {
        BK_LOGI(NULL, "set key failed\n");
        os_free(prikey.data);
        return NIST_FAIL;
    }

    hal_data shared_secret = {0};
    shared_secret.data = os_malloc(MAX_SHARED_SECURE_LEN);
    if (shared_secret.data == NULL) {
        armino_hal_remove_key(HAL_KEY_ECC_SEC_P192R1 + sub_type, key_idx);
        BK_LOGI(NULL, "malloc failed\n");
        os_free(prikey.data);
        return NIST_FAIL;
    }
    shared_secret.data_len = MAX_SHARED_SECURE_LEN;

    result = armino_hal_ecdh_compute_shared_secret(&ecdh_param, key_idx, &shared_secret);
    if (result != HAL_SUCCESS) {
        BK_LOGI(NULL, "compute shared secret failed %d\n", result);
        hal_nist_free_buffer(&shared_secret);
        armino_hal_remove_key(HAL_KEY_ECC_SEC_P192R1 + sub_type, key_idx);
        os_free(prikey.data);
        return NIST_FAIL;
    }

    // print_hex(shared_secret.data, shared_secret.data_len);
    // print_hex(ZIUT.data, ZIUT.data_len);

    if (shared_secret.data_len != ZIUT.data_len) {
        BK_LOGI(NULL, "shared secret length mismatch: %u != %u\n", shared_secret.data_len, ZIUT.data_len);
        hal_nist_free_buffer(&shared_secret);
        armino_hal_remove_key(HAL_KEY_ECC_SEC_P192R1 + sub_type, key_idx);
        os_free(prikey.data);
        return NIST_FAIL;
    }

    result = memcmp(shared_secret.data, ZIUT.data, shared_secret.data_len);
    if(result == 0){
        result = NIST_OK;
    } else {
        result = NIST_FAIL;
    }
    hal_nist_free_buffer(&shared_secret);
    armino_hal_remove_key(HAL_KEY_ECC_SEC_P192R1 + sub_type, key_idx);
    os_free(prikey.data);
    return result;
}

