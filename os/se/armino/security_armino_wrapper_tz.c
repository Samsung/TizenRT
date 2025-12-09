/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
#include <tinyara/security_hal.h>
#include <tinyara/seclink_drv.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TFM NS Interface */
#include "tfm_ns_interface.h"
#include "tfm_dh_nsc.h"
// #include "components/log.h"
#include <debug.h>

/* PSA Crypto API */
#include "psa/crypto.h"
#include "psa/crypto_types.h"
#include "psa/crypto_values.h"
#include "psa/protected_storage.h"
#include "include/armino_adapter_common.h"
#include "tfm_sca_aes_nsc.h"

#define HWRAP_TAG "[ARMINO_HAL_WRAPPER]"
#define HWRAP_ENTER do { dbg(HWRAP_TAG"%s:%d\n", __FILE__, __LINE__); } while (0)

#define HAL_TO_PSA_HASH_ALG(hal_type) ( \
    (hal_type) == HAL_HASH_MD5 ? PSA_ALG_MD5 : \
    (hal_type) == HAL_HASH_SHA1 ? PSA_ALG_SHA_1 : \
    (hal_type) == HAL_HASH_SHA224 ? PSA_ALG_SHA_224 : \
    (hal_type) == HAL_HASH_SHA256 ? PSA_ALG_SHA_256 : \
    (hal_type) == HAL_HASH_SHA384 ? PSA_ALG_SHA_384 : \
    (hal_type) == HAL_HASH_SHA512 ? PSA_ALG_SHA_512 : \
    PSA_ALG_NONE                                      \
)

static int armino_index_chk(uint32_t key_idx)
{
    int ret = HAL_SUCCESS;

    /* Check if in valid Factory range */
    if (key_idx >= TOTAL_KEY_STORAGE_INDEX) {
        ret = HAL_INVALID_SLOT_RANGE;
    }

    return ret;
}

#define HEX_PRINT_BYTE_MAX 128
static void print_hex(char *name, uint8_t *data, uint32_t len) {
    char s_hex_buff[HEX_PRINT_BYTE_MAX * 2 + 1];

    if(len > HEX_PRINT_BYTE_MAX){
        len = HEX_PRINT_BYTE_MAX;
    }
    dbg("%s len %d\r\n", name, len);
    for (size_t i = 0; i < len; i++) {
        sprintf(s_hex_buff + (2 * i), "%02x", data[i]);
    }
    s_hex_buff[len * 2] = 0;
    dbg("%s\n", s_hex_buff);
}

// Define global array to record the mapping between key_idx and psa_key_id_t
psa_key_id_t g_key_idx_record[TOTAL_KEY_STORAGE_INDEX] = {0};

/**
 * Common
 */
int armino_hal_init(hal_init_param *params)
{
    HWRAP_ENTER;
    // Initialize TFM NS interface if needed
    int32_t ret = ns_interface_lock_init();
    if (ret != HAL_SUCCESS) {
        dbg("Failed to initialize NS interface lock\n");
        return HAL_FAIL;
    }

    // Initialize PSA Crypto
    psa_status_t psa_ret = psa_crypto_init();
    if (psa_ret != PSA_SUCCESS) {
        dbg("Failed to initialize PSA crypto: %d\n", psa_ret);
        return HAL_FAIL;
    }

    return HAL_SUCCESS;
}

int armino_hal_deinit(void)
{
    HWRAP_ENTER;
    // Deinitialize resources if needed
    return HAL_SUCCESS;
}

int armino_hal_free_data(hal_data *data)
{
    HWRAP_ENTER;
    if (data) {
        if (data->data) {
            kmm_free(data->data);
        }
        if (data->priv) {
            kmm_free(data->priv);
        }
    }
    return HAL_SUCCESS;
}

int armino_hal_get_status(void)
{
    HWRAP_ENTER;
    // Check TF-M status
    // For simplicity, assume it's always ready
    return HAL_SUCCESS;
}

/**
 * Key Manager
 */
int armino_hal_set_key(hal_key_type mode, uint32_t key_idx, hal_data *key, hal_data *prikey)
{
    HWRAP_ENTER;

    if (prikey == NULL && key == NULL) {
        return HAL_INVALID_ARGS;
    }

    if((armino_index_chk(key_idx)) != HAL_SUCCESS){
        return HAL_INVALID_SLOT_RANGE;
    }

    if(!is_psa_key_id_empty(key_idx)){
        return HAL_KEY_IN_USE;//not empty
    }

    if (key_idx < FACTORY_KEY_INDEX_MAX) {
        dbg("Key index %d (0-31)  for factory key\n", key_idx);
        return HAL_INVALID_REQUEST;
    }

    if((mode == HAL_KEY_AES_128)||(mode == HAL_KEY_AES_192)||(mode == HAL_KEY_AES_256)){
        psa_status_t ret = psa_sca_aes_set_key((aes_key_type)mode, key_idx, key);
        set_psa_key_id(key_idx, key_idx);
        return ret ;
    }

    // Create and initialize key attributes
    psa_key_attributes_t attributes = psa_key_attributes_init();
    psa_status_t status;
    psa_key_id_t imported_key_id;

    psa_set_key_lifetime(&attributes, PSA_KEY_LIFETIME_VOLATILE);

    // Set key type based on mode
    switch (mode) {
        case HAL_KEY_RSA_1024:
        case HAL_KEY_RSA_2048:
        case HAL_KEY_RSA_3072:
        case HAL_KEY_RSA_4096:
            if (prikey != NULL && prikey->data != NULL && prikey->data_len > 0) { //import private key
                psa_set_key_type(&attributes, PSA_KEY_TYPE_RSA_KEY_PAIR);
                psa_set_key_bits(&attributes, (mode == HAL_KEY_RSA_1024) ? 1024 : (mode == HAL_KEY_RSA_2048) ? 2048 : (mode == HAL_KEY_RSA_3072) ? 3072 : 4096);
                psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT | PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH);
                psa_set_key_algorithm(&attributes, PSA_ALG_RSA_PKCS1V15_CRYPT);
                status = psa_import_key(&attributes, prikey->data, prikey->data_len, &imported_key_id);
            }else if(key != NULL && key->data != NULL && key->data_len > 0){ //import public key
                psa_set_key_type(&attributes, PSA_KEY_TYPE_RSA_PUBLIC_KEY);
                psa_set_key_bits(&attributes, (mode == HAL_KEY_RSA_1024) ? 1024 : (mode == HAL_KEY_RSA_2048) ? 2048 : (mode == HAL_KEY_RSA_3072) ? 3072 : 4096);
                psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_VERIFY_HASH);
                psa_set_key_algorithm(&attributes, PSA_ALG_RSA_PKCS1V15_CRYPT);
                status = psa_import_key(&attributes, key->data, key->data_len, &imported_key_id);
            } else {
                psa_reset_key_attributes(&attributes);
                return HAL_INVALID_ARGS;
            }
            break;
        case HAL_KEY_ECC_BRAINPOOL_P256R1:
        case HAL_KEY_ECC_BRAINPOOL_P384R1:
        case HAL_KEY_ECC_BRAINPOOL_P512R1:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_BRAINPOOL_P_R1));
            psa_set_key_bits(&attributes, (mode == HAL_KEY_ECC_BRAINPOOL_P256R1) ? 256 : (mode == HAL_KEY_ECC_BRAINPOOL_P384R1) ? 384 : 512);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_VERIFY_HASH | PSA_KEY_USAGE_DERIVE);
            psa_set_key_algorithm(&attributes, PSA_ALG_ECDSA(PSA_ECC_FAMILY_BRAINPOOL_P_R1) | PSA_ALG_ECDH);
            status = psa_import_key(&attributes, key->data, key->data_len, &imported_key_id);
            break;
        case HAL_KEY_ECC_SEC_P192R1:
        case HAL_KEY_ECC_SEC_P224R1:
        case HAL_KEY_ECC_SEC_P256R1:
        case HAL_KEY_ECC_SEC_P384R1:
        case HAL_KEY_ECC_SEC_P512R1:
            if (mode == HAL_KEY_ECC_SEC_P192R1) {
                psa_set_key_bits(&attributes, 192);
                psa_set_key_algorithm(&attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_256) | PSA_ALG_ECDH);
            }
            else if (mode == HAL_KEY_ECC_SEC_P224R1) {
                psa_set_key_bits(&attributes, 224);
                psa_set_key_algorithm(&attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_224) | PSA_ALG_ECDH);
            } else if (mode == HAL_KEY_ECC_SEC_P256R1) {
                psa_set_key_bits(&attributes, 256);
                psa_set_key_algorithm(&attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_256) | PSA_ALG_ECDH);
            } else if (mode == HAL_KEY_ECC_SEC_P384R1) {
                psa_set_key_bits(&attributes, 384);
                psa_set_key_algorithm(&attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_384) | PSA_ALG_ECDH);
            } else if (mode == HAL_KEY_ECC_SEC_P512R1) {
                psa_set_key_bits(&attributes, 521);
                psa_set_key_algorithm(&attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_512) | PSA_ALG_ECDH);
            }

            // Import the key
            if(prikey != NULL && prikey->data != NULL && prikey->data_len > 0){
                psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_DERIVE);
                psa_set_key_type(&attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1));
                status = psa_import_key(&attributes, prikey->data, prikey->data_len, &imported_key_id);
            }else if (key != NULL && key->data != NULL && key->data_len > 0 && key->priv != NULL && key->priv_len > 0){
                uint8_t *pub_key;
                pub_key = (uint8_t *)kmm_malloc(key->data_len * 2 + 1);
                if (pub_key == NULL) {
                    return HAL_NOT_ENOUGH_MEMORY;
                }
                pub_key[0] = 0x4;
                memcpy(&pub_key[1], key->data, key->data_len);
                memcpy(&pub_key[1+key->data_len], key->priv, key->priv_len);
                psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_VERIFY_HASH | PSA_KEY_USAGE_DERIVE);
                psa_set_key_type(&attributes, PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1));
                status = psa_import_key(&attributes, pub_key, key->data_len * 2 + 1, &imported_key_id);
                kmm_free(pub_key);
            } else {
                psa_reset_key_attributes(&attributes);
                return HAL_INVALID_ARGS;
            }


            break;
        case HAL_KEY_ECC_25519:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_MONTGOMERY));
            psa_set_key_bits(&attributes, 255);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_DERIVE);
            psa_set_key_algorithm(&attributes, PSA_ALG_ECDH);
            // Import the key
            status = psa_import_key(&attributes, key->data, key->data_len, &imported_key_id);
            break;
        case HAL_KEY_ED_25519:
            // psa_set_key_type(&attributes, PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_EDWARDS));
            // psa_set_key_bits(&attributes, 255);
            // psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_VERIFY_MESSAGE);
            // psa_set_key_algorithm(&attributes, PSA_ALG_ED25519);
            break;
        case HAL_KEY_HMAC_MD5:
        case HAL_KEY_HMAC_SHA1:
        case HAL_KEY_HMAC_SHA224:
        case HAL_KEY_HMAC_SHA256:
        case HAL_KEY_HMAC_SHA384:
        case HAL_KEY_HMAC_SHA512:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_HMAC);
            psa_set_key_bits(&attributes, key->data_len*8);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE);
            if (mode == HAL_KEY_HMAC_MD5) {
                psa_set_key_algorithm(&attributes, PSA_ALG_HMAC(PSA_ALG_MD5));
            } else if (mode == HAL_KEY_HMAC_SHA1) {
                psa_set_key_algorithm(&attributes, PSA_ALG_HMAC(PSA_ALG_SHA_1));
            } else if (mode == HAL_KEY_HMAC_SHA224) {
                psa_set_key_algorithm(&attributes, PSA_ALG_HMAC(PSA_ALG_SHA_224));
            } else if (mode == HAL_KEY_HMAC_SHA256) {
                psa_set_key_algorithm(&attributes, PSA_ALG_HMAC(PSA_ALG_SHA_256));
            } else if (mode == HAL_KEY_HMAC_SHA384) {
                psa_set_key_algorithm(&attributes, PSA_ALG_HMAC(PSA_ALG_SHA_384));
            } else if (mode == HAL_KEY_HMAC_SHA512) {
                psa_set_key_algorithm(&attributes, PSA_ALG_HMAC(PSA_ALG_SHA_512));
            }
            // Import the key
            status = psa_import_key(&attributes, key->data, key->data_len, &imported_key_id);
            break;
        case HAL_KEY_DH_1024:
        case HAL_KEY_DH_2048:
        case HAL_KEY_DH_4096:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_DH_PUBLIC_KEY(PSA_DH_FAMILY_RFC7919));
            psa_set_key_bits(&attributes, (mode == HAL_KEY_DH_1024) ? 1024 : (mode == HAL_KEY_DH_2048) ? 2048 : 4096);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_DERIVE);
            psa_set_key_algorithm(&attributes, PSA_ALG_FFDH);
            // Import the key
            status = psa_import_key(&attributes, key->data, key->data_len, &imported_key_id);
            break;
        default:
            dbg("Unsupported key type: %d\n", mode);
            psa_reset_key_attributes(&attributes);
            return HAL_NOT_SUPPORTED;
    }

    if (status != PSA_SUCCESS) {
        dbg("Failed to import key_idx %d status %d: %d\n", key_idx, status);
        psa_reset_key_attributes(&attributes);
        return HAL_FAIL;
    }
    psa_reset_key_attributes(&attributes);
    // Record the mapping between key_idx and psa_key_id_t
    set_psa_key_id(key_idx, imported_key_id);
    dbg("Imported key with ID: %u, mapped to index: %u\n", imported_key_id, key_idx);
    return HAL_SUCCESS;
}

int armino_hal_get_key(hal_key_type mode, uint32_t key_idx, hal_data *key)
{
    HWRAP_ENTER;
    if (key == NULL || key->data == NULL) {
        return HAL_INVALID_ARGS;
    }

    if((armino_index_chk(key_idx)) != HAL_SUCCESS){
        return HAL_INVALID_SLOT_RANGE;
    }

    // Get key information
    psa_status_t   status;
    psa_key_id_t   key_id;
    uint8_t  pub_key[HAL_MAX_ECP_KEY_SIZE];
    uint32_t pub_key_len;

    if(key_idx < FACTORY_KEY_INDEX_MAX){
        if(mode >= HAL_KEY_ECC_SEC_P192R1 && mode <= HAL_KEY_ECC_SEC_P512R1){
            int ret = ss_read_key(key_idx, pub_key, &pub_key_len);
            if(ret != HAL_SUCCESS){
                dbg("Failed to read key: %d\n", ret);
                return ret;
            }
            memcpy(key->data, &pub_key[0], pub_key_len/2);
            key->data_len = pub_key_len/2;
            memcpy(key->priv, &pub_key[pub_key_len/2], pub_key_len/2);
            key->priv_len = pub_key_len/2;
            return HAL_SUCCESS;
        }
        else{
            dbg("Not allowed to export key_idx %d mode %d\n",key_idx, mode);
            return HAL_NOT_SUPPORTED;
        }
    }else{
        if(is_psa_key_id_empty(key_idx)){
            dbg("key_id %d is not generated yet\n", key_idx);
            return HAL_EMPTY_SLOT;
        }else{
            key_id = get_psa_key_id(key_idx);
        }
    }
    // Check key type
    switch (mode) {
        case HAL_KEY_ECC_BRAINPOOL_P256R1:
        case HAL_KEY_ECC_BRAINPOOL_P384R1:
        case HAL_KEY_ECC_BRAINPOOL_P512R1:
        case HAL_KEY_ECC_SEC_P192R1:
        case HAL_KEY_ECC_SEC_P224R1:
        case HAL_KEY_ECC_SEC_P256R1:
        case HAL_KEY_ECC_SEC_P384R1:
        case HAL_KEY_ECC_SEC_P512R1:
            // Export the public key
            status = psa_export_public_key(key_id, pub_key, HAL_MAX_ECP_KEY_SIZE, &pub_key_len);
            if (status != PSA_SUCCESS) {
                dbg("Failed to export key: %d\n", status);
                return HAL_FAIL;
            }
            //print_hex("get pub key", pub_key, pub_key_len);
            memcpy(key->data, &pub_key[1], pub_key_len/2);
            key->data_len = pub_key_len/2;
            memcpy(key->priv, &pub_key[pub_key_len/2+1], pub_key_len/2);
            key->priv_len = pub_key_len/2;
            break;
        case HAL_KEY_AES_128:
        case HAL_KEY_AES_192:
        case HAL_KEY_AES_256:
        case HAL_KEY_RSA_1024:
        case HAL_KEY_RSA_2048:
        case HAL_KEY_RSA_3072:
        case HAL_KEY_RSA_4096:
        case HAL_KEY_ECC_25519:
        case HAL_KEY_ED_25519:
        case HAL_KEY_HMAC_MD5:
        case HAL_KEY_HMAC_SHA1:
        case HAL_KEY_HMAC_SHA224:
        case HAL_KEY_HMAC_SHA256:
        case HAL_KEY_HMAC_SHA384:
        case HAL_KEY_HMAC_SHA512:
        case HAL_KEY_DH_1024:
        case HAL_KEY_DH_2048:
        case HAL_KEY_DH_4096:
            dbg("Not allowed to export key_idx %d mode %d\n",key_idx, mode);
            return HAL_NOT_SUPPORTED;
        default:
            dbg("Unsupported key type: %d\n", mode);
            return HAL_NOT_SUPPORTED;
    }

    return HAL_SUCCESS;
}

int armino_hal_remove_key(hal_key_type mode, uint32_t key_idx)
{
    psa_status_t status;

    HWRAP_ENTER;
    if((armino_index_chk(key_idx)) != HAL_SUCCESS){
        return HAL_INVALID_SLOT_RANGE;
    }
    if(key_idx < FACTORY_KEY_INDEX_MAX){
        dbg("Not allowed to remove factory key_idx %d\n", key_idx);
        return HAL_INVALID_SLOT_RANGE;
    }

    if((mode == HAL_KEY_AES_128)||(mode == HAL_KEY_AES_192)||(mode == HAL_KEY_AES_256)){
        status = psa_sca_aes_remove_key((aes_key_type)mode, key_idx);
        reset_psa_key_id(key_idx);
        if(status != SCA_AES_SUCCESS){
            dbg("line :%d, Failed to remove key: %d\n", __LINE__, status);
            return HAL_FAIL;
        }
        return HAL_SUCCESS ;
    }

    if(is_psa_key_id_empty(key_idx)) {
        dbg("key_id %d is not generated yet\n", key_idx);
        return HAL_EMPTY_SLOT;
    } else {
        status = psa_destroy_key(g_key_idx_record[key_idx]);
        if (status != PSA_SUCCESS) {
            dbg("Failed to remove key: %d\n", status);
            return HAL_FAIL;
        }
    }
    reset_psa_key_id(key_idx);
    return HAL_SUCCESS;
}

int armino_hal_generate_key(hal_key_type mode, uint32_t key_idx)
{
    HWRAP_ENTER;
    if((armino_index_chk(key_idx)) != HAL_SUCCESS){
        return HAL_INVALID_SLOT_RANGE;
    }

    if (key_idx < FACTORY_KEY_INDEX_MAX) {
        dbg("Key index %d for factory key\n", key_idx);
        return HAL_INVALID_SLOT_RANGE;
    }

    if(!is_psa_key_id_empty(key_idx)){
        return HAL_KEY_IN_USE;//not empty
    }

    if((mode == HAL_KEY_AES_128)||(mode == HAL_KEY_AES_192)||(mode == HAL_KEY_AES_256)){
        psa_status_t ret = psa_sca_aes_generate_key((aes_key_type)mode, key_idx);
        return ret ;
    }

    // Create and initialize key attributes
    psa_key_attributes_t attributes = psa_key_attributes_init();
    psa_set_key_lifetime(&attributes, PSA_KEY_LIFETIME_VOLATILE);

    // Set key type based on mode
    switch (mode) {
        case HAL_KEY_RSA_1024:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_RSA_KEY_PAIR);
            psa_set_key_bits(&attributes, 1024);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT | PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH);
            psa_set_key_algorithm(&attributes, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_256));
            break;
        case HAL_KEY_RSA_2048:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_RSA_KEY_PAIR);
            psa_set_key_bits(&attributes, 2048);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT | PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH);
            psa_set_key_algorithm(&attributes, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_256));
            break;
        case HAL_KEY_RSA_3072:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_RSA_KEY_PAIR);
            psa_set_key_bits(&attributes, 3072);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT | PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH);
            psa_set_key_algorithm(&attributes, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_256));
            break;
        case HAL_KEY_RSA_4096:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_RSA_KEY_PAIR);
            psa_set_key_bits(&attributes, 4096);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT | PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH);
            psa_set_key_algorithm(&attributes, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_256));
            break;
        case HAL_KEY_ECC_BRAINPOOL_P256R1:
        case HAL_KEY_ECC_BRAINPOOL_P384R1:
        case HAL_KEY_ECC_BRAINPOOL_P512R1:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_BRAINPOOL_P_R1));
            psa_set_key_bits(&attributes, mode == HAL_KEY_ECC_BRAINPOOL_P256R1 ? 256 : (mode == HAL_KEY_ECC_BRAINPOOL_P384R1) ? 384 : 512);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH | PSA_KEY_USAGE_DERIVE | PSA_KEY_USAGE_EXPORT);
            psa_set_key_algorithm(&attributes, PSA_ALG_ECDSA(PSA_ECC_FAMILY_BRAINPOOL_P_R1));
            break;
        case HAL_KEY_ECC_SEC_P192R1:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1));
            psa_set_key_bits(&attributes, 192);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH | PSA_KEY_USAGE_DERIVE | PSA_KEY_USAGE_EXPORT);
            psa_set_key_algorithm(&attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_256));
            break;
        case HAL_KEY_ECC_SEC_P224R1:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1));
            psa_set_key_bits(&attributes, 224);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH | PSA_KEY_USAGE_DERIVE | PSA_KEY_USAGE_EXPORT);
            psa_set_key_algorithm(&attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_224));
            break;
        case HAL_KEY_ECC_SEC_P256R1:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1));
            psa_set_key_bits(&attributes, 256);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH | PSA_KEY_USAGE_DERIVE | PSA_KEY_USAGE_EXPORT);
            psa_set_key_algorithm(&attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_256));
            break;
        case HAL_KEY_ECC_SEC_P384R1:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1));
            psa_set_key_bits(&attributes, 384);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH | PSA_KEY_USAGE_DERIVE | PSA_KEY_USAGE_EXPORT);
            psa_set_key_algorithm(&attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_384));
            break;
        case HAL_KEY_ECC_SEC_P512R1:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1));
            psa_set_key_bits(&attributes, 521);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH | PSA_KEY_USAGE_DERIVE | PSA_KEY_USAGE_EXPORT);
            psa_set_key_algorithm(&attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_512));
            break;
        case HAL_KEY_ECC_25519:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_MONTGOMERY));
            psa_set_key_bits(&attributes, 255);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_DERIVE);
            psa_set_key_algorithm(&attributes, PSA_ALG_ECDH);
            break;
        case HAL_KEY_ED_25519:
            // psa_set_key_type(&attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_EDWARDS));
            // psa_set_key_bits(&attributes, 255);
            // psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE);
            // psa_set_key_algorithm(&attributes, PSA_ALG_ED25519);
            break;
        case HAL_KEY_HMAC_MD5:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_HMAC);
            psa_set_key_bits(&attributes, 128);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE);
            psa_set_key_algorithm(&attributes, PSA_ALG_HMAC(PSA_ALG_MD5));
            break;
        case HAL_KEY_HMAC_SHA1:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_HMAC);
            psa_set_key_bits(&attributes, 160);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE);
            psa_set_key_algorithm(&attributes, PSA_ALG_HMAC(PSA_ALG_SHA_1));
            break;
        case HAL_KEY_HMAC_SHA224:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_HMAC);
            psa_set_key_bits(&attributes, 224);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE);
            psa_set_key_algorithm(&attributes, PSA_ALG_HMAC(PSA_ALG_SHA_224));
            break;
        case HAL_KEY_HMAC_SHA256:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_HMAC);
            psa_set_key_bits(&attributes, 256);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE);
            psa_set_key_algorithm(&attributes, PSA_ALG_HMAC(PSA_ALG_SHA_256));
            break;
        case HAL_KEY_HMAC_SHA384:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_HMAC);
            psa_set_key_bits(&attributes, 384);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE);
            psa_set_key_algorithm(&attributes, PSA_ALG_HMAC(PSA_ALG_SHA_384));
            break;
        case HAL_KEY_HMAC_SHA512:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_HMAC);
            psa_set_key_bits(&attributes, 512);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE);
            psa_set_key_algorithm(&attributes, PSA_ALG_HMAC(PSA_ALG_SHA_512));
            break;
        case HAL_KEY_DH_1024:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_DH_KEY_PAIR(PSA_DH_FAMILY_RFC7919));
            psa_set_key_bits(&attributes, 1024);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_DERIVE);
            psa_set_key_algorithm(&attributes, PSA_ALG_FFDH);
            break;
        case HAL_KEY_DH_2048:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_DH_KEY_PAIR(PSA_DH_FAMILY_RFC7919));
            psa_set_key_bits(&attributes, 2048);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_DERIVE);
            psa_set_key_algorithm(&attributes, PSA_ALG_FFDH);
            break;
        case HAL_KEY_DH_4096:
            psa_set_key_type(&attributes, PSA_KEY_TYPE_DH_KEY_PAIR(PSA_DH_FAMILY_RFC7919));
            psa_set_key_bits(&attributes, 4096);
            psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_DERIVE);
            psa_set_key_algorithm(&attributes, PSA_ALG_FFDH);
            break;
        default:
            dbg("Unsupported key type: %d\n", mode);
            psa_reset_key_attributes(&attributes);
            return HAL_NOT_SUPPORTED;
    }

    // Generate the key
    psa_key_id_t generated_key_id;
    psa_status_t status = psa_generate_key(&attributes, &generated_key_id);
    psa_reset_key_attributes(&attributes);
    if (status != PSA_SUCCESS) {
        dbg("Failed to generate key: %d\n", status);
        return HAL_FAIL;
    }

    set_psa_key_id(key_idx, generated_key_id);
    dbg("Generated key with ID: %u, mapped to index: %u\n", generated_key_id, key_idx);
    return HAL_SUCCESS;
}

/**
 * Authenticate
 */
int armino_hal_generate_random(uint32_t len, hal_data *random)
{
    HWRAP_ENTER;
    if (len > HAL_MAX_RANDOM_SIZE || random == NULL) {
        return HAL_INVALID_ARGS;
    }

    // Generate random data
    psa_status_t status = psa_generate_random(random->data, len);
    if (status != PSA_SUCCESS) {
        dbg("Failed to generate random data: %d\n", status);
        return HAL_FAIL;
    }

    random->data_len = len;
    return HAL_SUCCESS;
}

int armino_hal_get_hash(hal_hash_type mode, hal_data *input, hal_data *hash)
{
    HWRAP_ENTER;

    if (input == NULL || input->data == NULL || hash == NULL || hash->data == NULL) {
        return HAL_INVALID_ARGS;
    }

    psa_hash_operation_t operation = PSA_HASH_OPERATION_INIT;
    psa_algorithm_t alg;
    size_t hash_size;

    psa_status_t psa_ret = psa_crypto_init();
    if (psa_ret != PSA_SUCCESS) {
        dbg("Failed to initialize PSA crypto: %d\n", psa_ret);
        return HAL_FAIL;
    }

    // Determine hash algorithm based on mode
    switch (mode) {
        case HAL_HASH_MD5:
            alg = PSA_ALG_MD5;
            hash_size = PSA_HASH_LENGTH(PSA_ALG_MD5);
            break;
        case HAL_HASH_SHA1:
            alg = PSA_ALG_SHA_1;
            hash_size = PSA_HASH_LENGTH(PSA_ALG_SHA_1);
            break;
        case HAL_HASH_SHA224:
            alg = PSA_ALG_SHA_224;
            hash_size = PSA_HASH_LENGTH(PSA_ALG_SHA_224);
            break;
        case HAL_HASH_SHA256:
            alg = PSA_ALG_SHA_256;
            hash_size = PSA_HASH_LENGTH(PSA_ALG_SHA_256);
            break;
        case HAL_HASH_SHA384:
            alg = PSA_ALG_SHA_384;
            hash_size = PSA_HASH_LENGTH(PSA_ALG_SHA_384);
            break;
        case HAL_HASH_SHA512:
            alg = PSA_ALG_SHA_512;
            hash_size = PSA_HASH_LENGTH(PSA_ALG_SHA_512);
            break;
        // Add other hash modes as needed
        default:
            dbg("Unsupported hash mode: %d\n", mode);
            return HAL_NOT_SUPPORTED;
    }

    psa_ret = psa_hash_setup(&operation, alg);
    if (psa_ret != PSA_SUCCESS) {
        dbg("Hash setup failed: %d\n", psa_ret);
        return HAL_FAIL;
    }

    psa_ret = psa_hash_update(&operation, input->data, input->data_len);
    if (psa_ret != PSA_SUCCESS) {
        dbg("Hash update failed: %d\n", psa_ret);
        return HAL_FAIL;
    }

    psa_ret = psa_hash_finish(&operation, hash->data, hash_size, &hash->data_len);
    if (psa_ret != PSA_SUCCESS) {
        dbg("Hash finish failed: %d\n", psa_ret);
        return HAL_FAIL;
    }

    psa_hash_abort(&operation);

    return HAL_SUCCESS;
}

int armino_hal_get_hmac(hal_hmac_type mode, hal_data *input, uint32_t key_idx, hal_data *hmac)
{
    HWRAP_ENTER;
    if (input == NULL || input->data == NULL || hmac == NULL || hmac->data == NULL) {
        return HAL_INVALID_ARGS;
    }

    if((armino_index_chk(key_idx)) != HAL_SUCCESS){
        return HAL_INVALID_SLOT_RANGE;
    }

    if (key_idx < FACTORY_KEY_INDEX_MAX) {
        dbg("Key index %d for factory key\n", key_idx);
        return HAL_INVALID_SLOT_RANGE;
    }

    psa_key_id_t key_id = 0;
    psa_status_t status;
    psa_algorithm_t alg;
    size_t hmac_size;

    if(is_psa_key_id_empty(key_idx)) {
        dbg("key_id %d is not generated yet\n", key_idx);
        return HAL_EMPTY_SLOT;
    } else {
        key_id = get_psa_key_id(key_idx);
    }

    switch (mode) {
        case HAL_HMAC_MD5:
            alg = PSA_ALG_HMAC(PSA_ALG_MD5);
            hmac_size = PSA_HASH_LENGTH(PSA_ALG_MD5);
            break;
        case HAL_HMAC_SHA1:
            alg = PSA_ALG_HMAC(PSA_ALG_SHA_1);
            hmac_size = PSA_HASH_LENGTH(PSA_ALG_SHA_1);
            break;
        case HAL_HMAC_SHA224:
            alg = PSA_ALG_HMAC(PSA_ALG_SHA_224);
            hmac_size = PSA_HASH_LENGTH(PSA_ALG_SHA_224);
            break;
        case HAL_HMAC_SHA256:
            alg = PSA_ALG_HMAC(PSA_ALG_SHA_256);
            hmac_size = PSA_HASH_LENGTH(PSA_ALG_SHA_256);
            break;
        case HAL_HMAC_SHA384:
            alg = PSA_ALG_HMAC(PSA_ALG_SHA_384);
            hmac_size = PSA_HASH_LENGTH(PSA_ALG_SHA_384);
            break;
        case HAL_HMAC_SHA512:
            alg = PSA_ALG_HMAC(PSA_ALG_SHA_512);
            hmac_size = PSA_HASH_LENGTH(PSA_ALG_SHA_512);
            break;
        // Add other HMAC modes as needed
        default:
            dbg("Unsupported HMAC mode: %d\n", mode);
            return HAL_NOT_SUPPORTED;
    }

    // Compute HMAC
    status = psa_mac_compute(key_id, alg, input->data, input->data_len, hmac->data, hmac_size, &hmac->data_len);

    if (status != PSA_SUCCESS) {
        dbg("HMAC computation failed: %d\n", status);
        return HAL_FAIL;
    }

    return HAL_SUCCESS;
}

int armino_hal_rsa_sign_md(hal_rsa_mode mode, hal_data *hash, uint32_t key_idx, hal_data *sign)
{
    HWRAP_ENTER;

    if (hash == NULL || hash->data == NULL || sign == NULL) {
        return HAL_INVALID_ARGS;
    }

    if((armino_index_chk(key_idx)) != HAL_SUCCESS){
        return HAL_INVALID_SLOT_RANGE;
    }

    if (key_idx < FACTORY_KEY_INDEX_MAX) {
        dbg("Key index %d for factory key\n", key_idx);
        return HAL_INVALID_SLOT_RANGE;
    }

    psa_key_id_t key_id = 0;
    psa_status_t status;
    psa_algorithm_t alg;

    if(is_psa_key_id_empty(key_idx)) {
        dbg("key_id %d is not generated yet\n", key_idx);
        return HAL_EMPTY_SLOT;
    } else {
        key_id = get_psa_key_id(key_idx);
    }
    // Determine RSA algorithm based on mode
    switch (mode.rsa_a) {
        case HAL_RSASSA_PKCS1_V1_5:
            // Map hash type to PSA algorithm
            switch (mode.hash_t) {
                case HAL_HASH_MD5:
                    alg = PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_MD5);
                    break;
                case HAL_HASH_SHA1:
                    alg = PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_1);
                    break;
                case HAL_HASH_SHA224:
                    alg = PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_224);
                    break;
                case HAL_HASH_SHA256:
                    alg = PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_256);
                    break;
                case HAL_HASH_SHA384:
                    alg = PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_384);
                    break;
                case HAL_HASH_SHA512:
                    alg = PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_512);
                    break;
                default:
                    dbg("Unsupported hash type for PKCS1 v1.5: %d\n", mode.hash_t);
                    return HAL_NOT_SUPPORTED;
            }
            break;
        case HAL_RSASSA_PKCS1_PSS_MGF1:
            // Map hash type to PSA algorithm for PSS
            switch (mode.hash_t) {
                case HAL_HASH_MD5:
                    alg = PSA_ALG_RSA_PSS_ANY_SALT(PSA_ALG_MD5);
                    break;
                case HAL_HASH_SHA1:
                    alg = PSA_ALG_RSA_PSS_ANY_SALT(PSA_ALG_SHA_1);
                    break;
                case HAL_HASH_SHA224:
                    alg = PSA_ALG_RSA_PSS_ANY_SALT(PSA_ALG_SHA_224);
                    break;
                case HAL_HASH_SHA256:
                    alg = PSA_ALG_RSA_PSS_ANY_SALT(PSA_ALG_SHA_256);
                    break;
                case HAL_HASH_SHA384:
                    alg = PSA_ALG_RSA_PSS_ANY_SALT(PSA_ALG_SHA_384);
                    break;
                case HAL_HASH_SHA512:
                    alg = PSA_ALG_RSA_PSS_ANY_SALT(PSA_ALG_SHA_512);
                    break;
                default:
                    dbg("Unsupported hash type for PSS: %d\n", mode.hash_t);
                    return HAL_NOT_SUPPORTED;
            }
            break;
        default:
            dbg("Unsupported RSA algorithm: %d\n", mode.rsa_a);
            return HAL_NOT_SUPPORTED;
    }

    // Get key attributes to determine signature size
    psa_key_attributes_t attributes;
    status = psa_get_key_attributes(key_id, &attributes);
    if (status != PSA_SUCCESS) {
        dbg("Failed to get key attributes: %d\n", status);
        return HAL_FAIL;
    }
    size_t key_bits = psa_get_key_bits(&attributes);
    size_t signature_size = PSA_BITS_TO_BYTES(key_bits);
    psa_reset_key_attributes(&attributes);

    // Perform signature
    status = psa_sign_message(key_id, alg, hash->data, hash->data_len, sign->data, signature_size, &sign->data_len);

    if (status != PSA_SUCCESS) {
        dbg("RSA signature failed: %d\n", status);
        return HAL_FAIL;
    }

    return HAL_SUCCESS;
}

int armino_hal_rsa_verify_md(hal_rsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx)
{
    HWRAP_ENTER;

    if (hash == NULL || hash->data == NULL || sign == NULL || sign->data == NULL) {
        return HAL_INVALID_ARGS;
    }

    if((armino_index_chk(key_idx)) != HAL_SUCCESS){
        return HAL_INVALID_SLOT_RANGE;
    }

    if (key_idx < FACTORY_KEY_INDEX_MAX) {
        dbg("Key index %d for factory key\n", key_idx);
        return HAL_INVALID_SLOT_RANGE;
    }

    // Open the key
    psa_key_id_t key_id = 0;
    psa_status_t status;
    psa_algorithm_t alg;

    if(is_psa_key_id_empty(key_idx)) {
        dbg("key_id %d is not generated yet\n", key_idx);
        return HAL_EMPTY_SLOT;
    } else {
        key_id = get_psa_key_id(key_idx);
    }

    switch (mode.rsa_a) {
        case HAL_RSASSA_PKCS1_V1_5:
            // Map hash type to PSA algorithm
            switch (mode.hash_t) {
                case HAL_HASH_MD5:
                    alg = PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_MD5);
                    break;
                case HAL_HASH_SHA1:
                    alg = PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_1);
                    break;
                case HAL_HASH_SHA224:
                    alg = PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_224);
                    break;
                case HAL_HASH_SHA256:
                    alg = PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_256);
                    break;
                case HAL_HASH_SHA384:
                    alg = PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_384);
                    break;
                case HAL_HASH_SHA512:
                    alg = PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_512);
                    break;
                default:
                    dbg("Unsupported hash type for PKCS1 v1.5: %d\n", mode.hash_t);
                    return HAL_NOT_SUPPORTED;
            }
            break;
        case HAL_RSASSA_PKCS1_PSS_MGF1:
            // Map hash type to PSA algorithm for PSS
            switch (mode.hash_t) {
                case HAL_HASH_MD5:
                    alg = PSA_ALG_RSA_PSS_ANY_SALT(PSA_ALG_MD5);
                    break;
                case HAL_HASH_SHA1:
                    alg = PSA_ALG_RSA_PSS_ANY_SALT(PSA_ALG_SHA_1);
                    break;
                case HAL_HASH_SHA224:
                    alg = PSA_ALG_RSA_PSS_ANY_SALT(PSA_ALG_SHA_224);
                    break;
                case HAL_HASH_SHA256:
                    alg = PSA_ALG_RSA_PSS_ANY_SALT(PSA_ALG_SHA_256);
                    break;
                case HAL_HASH_SHA384:
                    alg = PSA_ALG_RSA_PSS_ANY_SALT(PSA_ALG_SHA_384);
                    break;
                case HAL_HASH_SHA512:
                    alg = PSA_ALG_RSA_PSS_ANY_SALT(PSA_ALG_SHA_512);
                    break;
                default:
                    dbg("Unsupported hash type for PSS: %d\n", mode.hash_t);
                    return HAL_NOT_SUPPORTED;
            }
            break;
        default:
            dbg("Unsupported RSA algorithm: %d\n", mode.rsa_a);
            return HAL_NOT_SUPPORTED;
    }

    // Perform verification
    status = psa_verify_message(key_id, alg, hash->data, hash->data_len, sign->data, sign->data_len);
    if (status != PSA_SUCCESS) {
        dbg("RSA verification failed: %d\n", status);
        return HAL_FAIL;
    }

    return HAL_SUCCESS;
}

int armino_hal_ecdsa_sign_md(hal_ecdsa_mode mode, hal_data *hash, uint32_t key_idx, hal_data *sign)
{
    HWRAP_ENTER;
    if (hash == NULL || hash->data == NULL || sign == NULL || sign->data == NULL) {
        return HAL_INVALID_ARGS;
    }

    if((armino_index_chk(key_idx)) != HAL_SUCCESS){
        return HAL_INVALID_SLOT_RANGE;
    }

    psa_key_id_t key_id = 0;
    psa_status_t status;
    psa_algorithm_t alg;

    if (key_idx < FACTORY_KEY_INDEX_MAX) {
#if CONFIG_TFM_ASYM_ALGO_NSC
        status = psa_ecdsa_sign_p256(hash, key_idx, sign);
        if(status != HAL_SUCCESS){
            dbg("ECDSA sign failed: %d\n", status);
            return HAL_FAIL;
        }
        return HAL_SUCCESS;
#else
        dbg("unsupported factory curve: %d key_idx = %d\r\n", mode.curve, key_idx);
        return HAL_NOT_SUPPORTED;
#endif
    }
    /* key stored in SE RAM */
    if(is_psa_key_id_empty(key_idx)) {
        dbg("key_id %d is not generated yet\n", key_idx);
        return HAL_EMPTY_SLOT;
    } else {
        key_id = get_psa_key_id(key_idx);
    }

    switch (mode.hash_t) {
        case HAL_HASH_SHA1:
            alg = PSA_ALG_ECDSA(PSA_ALG_SHA_1);
            break;
        case HAL_HASH_SHA224:
            alg = PSA_ALG_ECDSA(PSA_ALG_SHA_224);
            break;
        case HAL_HASH_SHA256:
            alg = PSA_ALG_ECDSA(PSA_ALG_SHA_256);
            break;
        case HAL_HASH_SHA384:
            alg = PSA_ALG_ECDSA(PSA_ALG_SHA_384);
            break;
        case HAL_HASH_SHA512:
            alg = PSA_ALG_ECDSA(PSA_ALG_SHA_512);
            break;
        default:
            dbg("Unsupported ECDSA hash type: %d\n", mode.hash_t);
            return HAL_NOT_SUPPORTED;
    }

    // Get key attributes to determine signature size
    psa_key_attributes_t attributes;
    status = psa_get_key_attributes(key_id, &attributes);
    if (status != PSA_SUCCESS) {
        dbg("Failed to get key attributes: %d\n", status);
        return HAL_FAIL;
    }
    size_t key_bits = psa_get_key_bits(&attributes);
    size_t signature_size = ((key_bits + 7) / 8) * 2; // ECDSA signature is R + S
    psa_reset_key_attributes(&attributes);

    // Perform signature
    status = psa_sign_message(key_id, alg, hash->data, hash->data_len, sign->data, signature_size, &sign->data_len);

    if (status != PSA_SUCCESS) {
        dbg("ECDSA signature failed: %d\n", status);
        return HAL_FAIL;
    }

    return HAL_SUCCESS;
}

int armino_hal_ecdsa_verify_md(hal_ecdsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx)
{
    HWRAP_ENTER;
    if (hash == NULL || hash->data == NULL || sign == NULL || sign->data == NULL) {
        return HAL_INVALID_ARGS;
    }

    if((armino_index_chk(key_idx)) != HAL_SUCCESS){
        return HAL_INVALID_SLOT_RANGE;
    }

    psa_key_id_t key_id = 0;
    psa_status_t status;
    psa_algorithm_t alg;

    if (key_idx < FACTORY_KEY_INDEX_MAX) {
#if CONFIG_TFM_ASYM_ALGO_NSC
    if(mode.curve == HAL_ECDSA_SEC_P256R1){
        status = psa_ecdsa_verify_p256(hash, key_idx, sign);
        if(status != HAL_SUCCESS){
            dbg("ECDSA verify failed: %d\n", status);
            return HAL_FAIL;
        }
        return HAL_SUCCESS;
    }else{
        dbg("unsupported factory curve: %d key_idx = %d\r\n", mode.curve, key_idx);
        return HAL_NOT_SUPPORTED;
    }
#else
        dbg("unsupported factory curve: %d key_idx = %d\r\n", mode.curve, key_idx);
        return HAL_NOT_SUPPORTED;
#endif
    }

    if(is_psa_key_id_empty(key_idx)) {
        dbg("key_id %d is not generated yet\n", key_idx);
        return HAL_EMPTY_SLOT;
    } else {
        key_id = get_psa_key_id(key_idx);
    }

    switch (mode.hash_t) {
        case HAL_HASH_SHA1:
            alg = PSA_ALG_ECDSA(PSA_ALG_SHA_1);
            break;
        case HAL_HASH_SHA224:
            alg = PSA_ALG_ECDSA(PSA_ALG_SHA_224);
            break;
        case HAL_HASH_SHA256:
            alg = PSA_ALG_ECDSA(PSA_ALG_SHA_256);
            break;
        case HAL_HASH_SHA384:
            alg = PSA_ALG_ECDSA(PSA_ALG_SHA_384);
            break;
        case HAL_HASH_SHA512:
            alg = PSA_ALG_ECDSA(PSA_ALG_SHA_512);
            break;
        default:
            dbg("Unsupported ECDSA hash type: %d\n", mode.hash_t);
            return HAL_NOT_SUPPORTED;
    }

    // Perform verification
    status = psa_verify_message(key_id, alg, hash->data, hash->data_len, sign->data, sign->data_len);

    if (status != PSA_SUCCESS) {
        dbg("ECDSA verification failed: %d\n", status);
        return HAL_FAIL;
    }

    return HAL_SUCCESS;
}

int armino_hal_dh_generate_param(uint32_t dh_idx, hal_dh_data *dh_param)
{
     HWRAP_ENTER;
    if (dh_param == NULL || dh_param->P == NULL || dh_param->P->data == NULL || dh_param->G == NULL ||
            dh_param->G->data == NULL || dh_param->pubkey == NULL || dh_param->pubkey->data == NULL) {
        dbg("Invalid DH parameter\n");
        return HAL_INVALID_ARGS;
    }

    if((armino_index_chk(dh_idx)) != HAL_SUCCESS){
        return HAL_INVALID_SLOT_RANGE;
    }

    if (dh_idx < FACTORY_KEY_INDEX_MAX) {
        dbg("Key index %d for factory key\n", dh_idx);
        return HAL_INVALID_SLOT_RANGE;
    }

    size_t key_bytes = 0;
    psa_key_id_t key_id;

    switch(dh_param->mode) {
        case HAL_DH_1024:
            dh_param->pubkey->data_len = 128;
            break;
        case HAL_DH_2048:
            dh_param->pubkey->data_len = 256;
            break;
        case HAL_DH_4096:
            dh_param->pubkey->data_len = 512;
            break;
        default:
            dbg("Unsupported DH mode: %d\n", dh_param->mode);
            return HAL_NOT_SUPPORTED;
    }

    // input the G & P
    dh_type_t dh_param_t;
    dh_param_t.G = dh_param->G->data;
    dh_param_t.G_size = dh_param->G->data_len;
    dh_param_t.P = dh_param->P->data;
    dh_param_t.P_size = dh_param->P->data_len;

    psa_status_t status = psa_dh_import_p_g(&dh_param_t);
    if (status != PSA_SUCCESS) {
        dbg("Failed to import P & G: %d\n", status);
        return HAL_FAIL;
    }

    if (is_psa_key_id_empty(dh_idx)) {
        // generate the secret key from dh_idx
        psa_key_attributes_t priv_attributes = psa_key_attributes_init();
        psa_set_key_lifetime(&priv_attributes, PSA_KEY_LIFETIME_VOLATILE);
        psa_set_key_usage_flags(&priv_attributes, PSA_KEY_USAGE_DERIVE);
        psa_set_key_type(&priv_attributes, PSA_KEY_TYPE_DH_KEY_PAIR(PSA_DH_FAMILY_RFC7919));
        psa_set_key_algorithm(&priv_attributes, PSA_ALG_FFDH);
        psa_set_key_bits(&priv_attributes, dh_param->pubkey->data_len * 8);

        status = psa_generate_key(&priv_attributes, &key_id);
        psa_reset_key_attributes(&priv_attributes);

        if (status != PSA_SUCCESS) {
            dbg("Failed to generate DH key pair: %d\n", status);
            return HAL_FAIL;
        }
        set_psa_key_id(dh_idx, key_id);
    } else {
        dbg("now the key_id is valid\n");
        key_id = get_psa_key_id(dh_idx);
    }

    status = psa_export_public_key(key_id, dh_param->pubkey->data, dh_param->pubkey->data_len, &key_bytes);
    psa_dh_mem_free();

    if (status != PSA_SUCCESS) {
        dbg("Failed to export DH public key: %d\n", status);
        return HAL_FAIL;
    }
    dh_param->pubkey->data_len = key_bytes;

    return HAL_SUCCESS;
}

int armino_hal_dh_compute_shared_secret(hal_dh_data *dh_param, uint32_t dh_idx, hal_data *shared_secret)
{
    HWRAP_ENTER;
    if (dh_param == NULL || dh_param->pubkey == NULL||dh_param->pubkey->data == NULL || shared_secret == NULL || shared_secret->data == NULL) {
        return HAL_INVALID_ARGS;
    }

    psa_key_id_t key_id;
    size_t pub_key_len;
    uint8_t *pub_key;
    psa_status_t status;

    if((armino_index_chk(dh_idx)) != HAL_SUCCESS){
        return HAL_INVALID_SLOT_RANGE;
    }

    if(is_psa_key_id_empty(dh_idx)) {
        dbg("dh_idx %d is not generated yet\n", dh_idx);
        return HAL_EMPTY_SLOT;
    } else {
        key_id = get_psa_key_id(dh_idx);
    }

    // get the secret key from key_id
    switch(dh_param->mode) {
        case HAL_DH_1024:
            shared_secret->data_len = 128;
            break;
        case HAL_DH_2048:
            shared_secret->data_len = 256;
            break;
        case HAL_DH_4096:
            shared_secret->data_len = 512;
            break;
        default:
            dbg("Unsupported DH mode: %d\n", dh_param->mode);
            return HAL_NOT_SUPPORTED;
    }

    pub_key_len = shared_secret->data_len;
    pub_key = (uint8_t *)kmm_malloc(pub_key_len);
    if (pub_key == NULL) {
        return HAL_NOT_ENOUGH_MEMORY;
    }

    memset(pub_key, 0, pub_key_len);
    memcpy(pub_key + (pub_key_len - dh_param->pubkey->data_len), dh_param->pubkey->data, dh_param->pubkey->data_len);

    dh_type_t dh_param_t;
    dh_param_t.G = dh_param->G->data;
    dh_param_t.G_size = dh_param->G->data_len;
    dh_param_t.P = dh_param->P->data;
    dh_param_t.P_size = dh_param->P->data_len;

    status = psa_dh_import_p_g(&dh_param_t);
    if (status != PSA_SUCCESS) {
        dbg("Failed to import P & G: %d\n", status);
        kmm_free(pub_key);
        return HAL_FAIL;
    }

    // Perform DH key agreement
    status = psa_raw_key_agreement(PSA_ALG_FFDH, key_id, pub_key, pub_key_len, shared_secret->data, shared_secret->data_len, &shared_secret->data_len);

    // Cleanup
    psa_dh_mem_free();
    kmm_free(pub_key);

    if (status != PSA_SUCCESS) {
        dbg("DH shared secret key agreement failed: %d\n", status);
        return HAL_FAIL;
    }

    return HAL_SUCCESS;
}

int armino_hal_ecdh_compute_shared_secret(hal_ecdh_data *ecdh_param, uint32_t key_idx, hal_data *shared_secret)
{
    HWRAP_ENTER;

    if (ecdh_param == NULL || shared_secret == NULL || shared_secret->data == NULL ||
    ecdh_param->pubkey_x == NULL || ecdh_param->pubkey_x->data == NULL ||
    ecdh_param->pubkey_y == NULL || ecdh_param->pubkey_y->data == NULL) {
        return HAL_INVALID_ARGS;
    }

    if((armino_index_chk(key_idx)) != HAL_SUCCESS){
        return HAL_INVALID_SLOT_RANGE;
    }

    // Open the key
    psa_key_id_t key_id = 0;
    psa_status_t status;
    size_t pub_key_len;
    uint8_t *pub_key;

    switch(ecdh_param->curve) {
        case HAL_ECDSA_SEC_P192R1:
            pub_key_len = 48;
            break;
        case HAL_ECDSA_SEC_P224R1:
            pub_key_len = 56;
            break;
        case HAL_ECDSA_SEC_P256R1:
        case HAL_ECDSA_BRAINPOOL_P256R1:
            pub_key_len = 64;
            break;
        case HAL_ECDSA_SEC_P384R1:
        case HAL_ECDSA_BRAINPOOL_P384R1:
            pub_key_len = 96;
            break;
        case HAL_ECDSA_BRAINPOOL_P512R1:
            pub_key_len = 128;
            break;
        case HAL_ECDSA_SEC_P521R1:
            pub_key_len = 132;
            break;
        case HAL_ECDSA_CURVE_25519:
            return HAL_NOT_SUPPORTED;
        default:
            return HAL_INVALID_ARGS;
    }

    if (key_idx < FACTORY_KEY_INDEX_MAX) {
#if CONFIG_TFM_ASYM_ALGO_NSC
        uint32_t priv_key_len = pub_key_len/2;

        status = ecdh_factory_key_agreement(ecdh_param, priv_key_len, key_idx, shared_secret);
        if (status != PSA_SUCCESS) {
            dbg("ECDH key in flash agreement failed: %d\n", status);
            return HAL_FAIL;
        }

#else
        dbg("unsupported factory curve: %d key_idx = %d\r\n", ecdh_param->curve, key_idx);
        return HAL_NOT_SUPPORTED;
#endif
    } else {
        if (is_psa_key_id_empty(key_idx)) {
            dbg("key_id %d is not generated yet\n", key_idx);
            return HAL_EMPTY_SLOT;
        } else {
            key_id = get_psa_key_id(key_idx);
        }

        pub_key = (uint8_t *)kmm_malloc(pub_key_len + 1);
        if (pub_key == NULL) {
            return HAL_NOT_ENOUGH_MEMORY;
        }

        shared_secret->data_len = pub_key_len;

        pub_key[0] = 0x4;
        memcpy(&pub_key[1], ecdh_param->pubkey_x->data, ecdh_param->pubkey_x->data_len);
        memcpy(&pub_key[1+ecdh_param->pubkey_x->data_len], ecdh_param->pubkey_y->data, ecdh_param->pubkey_y->data_len);
        pub_key_len = 1+ecdh_param->pubkey_x->data_len+ecdh_param->pubkey_y->data_len;

        status = psa_raw_key_agreement(PSA_ALG_ECDH, key_id, pub_key, pub_key_len, shared_secret->data, shared_secret->data_len, &shared_secret->data_len);
        if (status != PSA_SUCCESS) {
            dbg("ECDH key agreement failed: %d %d\n", status, __LINE__);
            kmm_free(pub_key);
            return HAL_FAIL;
        }

        kmm_free(pub_key);
    }

    return HAL_SUCCESS;
}

int armino_hal_set_certificate(uint32_t cert_idx, hal_data *cert_in)
{
    HWRAP_ENTER;
    if (cert_in == NULL || cert_in->data == NULL) {
        return HAL_INVALID_ARGS;
    }

    int ret;
    ret = ss_write_cert(cert_idx, cert_in->data, cert_in->data_len);
    if (ret != 0) {
        dbg("Failed to store certificate: %d\n", ret);
        return HAL_FAIL;
    }

    return HAL_SUCCESS;
}

int armino_hal_get_certificate(uint32_t cert_idx, hal_data *cert_out)
{
    HWRAP_ENTER;
    if (cert_out == NULL || cert_out->data == NULL) {
        return HAL_INVALID_ARGS;
    }

    int ret;
    ret = ss_read_cert(cert_idx, cert_out->data, &cert_out->data_len);
    if (ret != 0) {
        dbg("Failed to read certificate: %d\n", ret);
        return HAL_FAIL;
    }

    return HAL_SUCCESS;
}

int armino_hal_remove_certificate(uint32_t cert_idx)
{
    HWRAP_ENTER;
    int ret;
    ret = ss_delete_cert(cert_idx);
    if (ret != 0) {
        dbg("Failed to delete certificate: %d\n", ret);
        return HAL_FAIL;
    }

    return HAL_SUCCESS;
}

int armino_hal_get_factory_key(uint32_t key_idx, hal_data *key)
{
    HWRAP_ENTER;
    // TODO: Implement using TF-M PSA API
    return HAL_NOT_SUPPORTED;
}

int armino_hal_get_factory_cert(uint32_t cert_idx, hal_data *cert)
{
    HWRAP_ENTER;
    // TODO: Implement using TF-M PSA API
    return HAL_NOT_SUPPORTED;
}

int armino_hal_get_factory_data(uint32_t data_idx, hal_data *data)
{
    HWRAP_ENTER;
    // TODO: Implement using TF-M PSA API
    return HAL_NOT_SUPPORTED;
}

/**
 * Crypto
 */

int armino_hal_aes_encrypt(hal_data *dec_data, hal_aes_param *aes_param, uint32_t key_idx, hal_data *enc_data)
{
    int ret ;
    psa_algorithm_t alg;
    sca_aes_context_t cipher_param;

    HWRAP_ENTER;
    if ((dec_data == NULL) || (dec_data->data == NULL) || (enc_data == NULL) || (enc_data->data == NULL) || (aes_param == NULL))
    {
        return HAL_INVALID_ARGS;
    }

    if((armino_index_chk(key_idx)) != HAL_SUCCESS){
        return HAL_INVALID_SLOT_RANGE;
    }

    uint32_t padded_len = aes_get_padded_len(dec_data->data_len);
    uint8_t * output_data = (uint8_t *)kmm_malloc(padded_len);
    if (output_data == NULL) {
        return HAL_NOT_ENOUGH_MEMORY;
    }

    ret = aes_padding_adapter(aes_param->mode, dec_data->data, dec_data->data_len, output_data, &padded_len, 0);
    if (ret != 0) {
        dbg("Failed to pad input data: %d\n", ret);
        kmm_free(output_data);
        return HAL_FAIL;
    }

    os_memset(&cipher_param, 0 , sizeof(sca_aes_context_t));
    cipher_param.input_data    = output_data;
    cipher_param.input_len     = padded_len;
    cipher_param.output_data   = enc_data->data;
    cipher_param.output_len    = padded_len;
    cipher_param.iv_len        = aes_param->iv_len;
    cipher_param.key_idx       = key_idx;
    if(aes_param->iv_offset != NULL)
    {
        cipher_param.iv_offset = aes_param->iv_offset;
    }
    if(aes_param->iv != NULL)
    {
        cipher_param.iv = aes_param->iv;
    }
    if(aes_param->nc_off != NULL)
    {
        cipher_param.nc_off = aes_param->nc_off;
    }
    if(aes_param->nonce_counter != NULL)
    {
        cipher_param.nonce_counter = aes_param->nonce_counter;
    }
    if(aes_param->stream_block != NULL)
    {
        cipher_param.stream_block = aes_param->stream_block;
    }

    dbg("line :%d, aes_param->mode :%d \r\n", __LINE__, aes_param->mode);
    switch (aes_param->mode) {
        case HAL_AES_ECB_NOPAD:
        case HAL_AES_ECB_ISO9797_M1:
        case HAL_AES_ECB_ISO9797_M2:
        case HAL_AES_ECB_PKCS5:
        case HAL_AES_ECB_PKCS7:
            alg = PSA_ALG_ECB_NO_PADDING;
            cipher_param.mode = SCA_AES_ECB_ENCRYPT_MODE;
            ret = psa_sca_aes_ecb_crypt((sca_aes_context_t *)&cipher_param, cipher_param.input_data, cipher_param.input_len, cipher_param.output_data);
            break;
        case HAL_AES_CBC_NOPAD:
        case HAL_AES_CBC_ISO9797_M1:
        case HAL_AES_CBC_ISO9797_M2:
        case HAL_AES_CBC_PKCS5:
        case HAL_AES_CBC_PKCS7:
            alg = PSA_ALG_CBC_NO_PADDING; 
            cipher_param.mode = SCA_AES_CBC_ENCRYPT_MODE;
            ret = psa_sca_aes_cbc_crypt((sca_aes_context_t *)&cipher_param, cipher_param.input_data, cipher_param.input_len, cipher_param.output_data);
            break;
        case HAL_AES_CTR:   //CTR mode no padding
            alg = PSA_ALG_CTR;
            cipher_param.mode = SCA_AES_CTR_ENCRYPT_MODE;
            ret = psa_sca_aes_ctr_crypt((sca_aes_context_t *)&cipher_param, cipher_param.input_data, cipher_param.input_len, cipher_param.output_data);
            break;
        case HAL_AES_CFB128: //CFB mode no padding
            alg = PSA_ALG_CFB;
            cipher_param.mode = SCA_AES_CFB_ENCRYPT_MODE;
            ret = psa_sca_aes_cfb_crypt((sca_aes_context_t *)&cipher_param, cipher_param.input_data, cipher_param.input_len, cipher_param.output_data);
            break;
        default:
            dbg("line :%d, Unsupported AES mode: %d\n", __LINE__, aes_param->mode);
            kmm_free(output_data);
            return HAL_NOT_SUPPORTED;
    }

    kmm_free(output_data);
    if (ret != PSA_SUCCESS) {
        dbg("AES encryption failed: %d\n", ret);
        return HAL_FAIL;
    }

    enc_data->data_len = cipher_param.output_len;
    return HAL_SUCCESS;
}

int armino_hal_aes_decrypt(hal_data *enc_data, hal_aes_param *aes_param, uint32_t key_idx, hal_data *dec_data)
{
    int ret ;
    psa_algorithm_t alg;
    sca_aes_context_t cipher_param;

    HWRAP_ENTER;

    if ((dec_data == NULL) ||  (dec_data->data == NULL) || (enc_data == NULL) || (enc_data->data == NULL) || (aes_param == NULL))
    {
        return HAL_INVALID_ARGS;
    }

    if((armino_index_chk(key_idx)) != HAL_SUCCESS){
        return HAL_INVALID_SLOT_RANGE;
    }

    os_memset(&cipher_param, 0 , sizeof(sca_aes_context_t));
    cipher_param.input_data    = enc_data->data;
    cipher_param.input_len     = enc_data->data_len;
    cipher_param.output_data   = dec_data->data;
    cipher_param.output_len    = enc_data->data_len;
    cipher_param.iv_len        = aes_param->iv_len;
    cipher_param.key_idx       = key_idx;
    if(aes_param->iv_offset != NULL)
    {
        cipher_param.iv_offset = aes_param->iv_offset;
    }
    if(aes_param->iv != NULL)
    {
        cipher_param.iv = aes_param->iv;
    }
    if(aes_param->nc_off != NULL)
    {
        cipher_param.nc_off = aes_param->nc_off;
    }
    if(aes_param->nonce_counter != NULL)
    {
        cipher_param.nonce_counter = aes_param->nonce_counter;
    }
    if(aes_param->stream_block != NULL)
    {
        cipher_param.stream_block = aes_param->stream_block;
    }

    dbg("line :%d ,cipher_param.mode :%d \r\n", __LINE__,cipher_param.mode);
    switch (aes_param->mode) {
        case HAL_AES_ECB_NOPAD:
        case HAL_AES_ECB_ISO9797_M1:
        case HAL_AES_ECB_ISO9797_M2:
        case HAL_AES_ECB_PKCS5:
        case HAL_AES_ECB_PKCS7:
            alg = PSA_ALG_ECB_NO_PADDING;
            cipher_param.mode = SCA_AES_ECB_DECRYPT_MODE;
            ret = psa_sca_aes_ecb_crypt((sca_aes_context_t *)&cipher_param, cipher_param.input_data, cipher_param.input_len, cipher_param.output_data);
            break;
        case HAL_AES_CBC_NOPAD:
        case HAL_AES_CBC_ISO9797_M1:
        case HAL_AES_CBC_ISO9797_M2:
        case HAL_AES_CBC_PKCS5:
        case HAL_AES_CBC_PKCS7:
            alg = PSA_ALG_CBC_NO_PADDING;
            cipher_param.mode = SCA_AES_CBC_DECRYPT_MODE;
            ret = psa_sca_aes_cbc_crypt((sca_aes_context_t *)&cipher_param, cipher_param.input_data, cipher_param.input_len, cipher_param.output_data);
            break;
        case HAL_AES_CTR:
            alg = PSA_ALG_CTR;
            cipher_param.mode = SCA_AES_CTR_DECRYPT_MODE;
            ret = psa_sca_aes_ctr_crypt((sca_aes_context_t *)&cipher_param, cipher_param.input_data, cipher_param.input_len, cipher_param.output_data);
            break;
        case HAL_AES_CFB128:
            alg = PSA_ALG_CFB;
            cipher_param.mode = SCA_AES_CFB_DECRYPT_MODE;
            ret = psa_sca_aes_cfb_crypt((sca_aes_context_t *)&cipher_param, cipher_param.input_data, cipher_param.input_len, cipher_param.output_data);
            break;
        default:
            dbg("line :%d, Unsupported AES mode: %d\n", __LINE__, aes_param->mode);
            return HAL_NOT_SUPPORTED;
    }

    if (ret != PSA_SUCCESS) {
        dbg("AES encryption failed: %d\n", ret);
        return HAL_FAIL;
    }

    dec_data->data_len = cipher_param.output_len;
    return HAL_SUCCESS;
}

int armino_hal_rsa_encrypt(hal_data *dec_data, hal_rsa_mode *mode, uint32_t key_idx, hal_data *enc_data)
{
    HWRAP_ENTER;
    if (dec_data == NULL || dec_data->data == NULL || mode == NULL || enc_data == NULL) {
        return HAL_INVALID_ARGS;
    }

    if((armino_index_chk(key_idx)) != HAL_SUCCESS){
        return HAL_INVALID_SLOT_RANGE;
    }

    if (key_idx < FACTORY_KEY_INDEX_MAX) {
        dbg("Key index %d for factory key\n", key_idx); 
        return HAL_INVALID_SLOT_RANGE;
    }

    // Open the key
    psa_key_id_t key_id = 0;
    psa_status_t status;
    psa_algorithm_t alg;
    psa_key_attributes_t attributes;

    if(is_psa_key_id_empty(key_idx)) {
        dbg("key_id %d is not generated yet\n", key_idx);
        return HAL_EMPTY_SLOT;
    } else {
        key_id = get_psa_key_id(key_idx);
    }

    switch (mode->rsa_a) {
        case HAL_RSASSA_PKCS1_V1_5:
            alg = PSA_ALG_RSA_PKCS1V15_CRYPT;
            break;
        case HAL_RSASSA_PKCS1_PSS_MGF1:
            alg = PSA_ALG_RSA_OAEP(HAL_TO_PSA_HASH_ALG(mode->hash_t));
            break;
        default:
            dbg("Unsupported RSA mode: %d\n", mode->rsa_a);
            return HAL_NOT_SUPPORTED;
    }

    status = psa_get_key_attributes(key_id, &attributes);
    if (status != PSA_SUCCESS) {
        dbg("Failed to get key attributes: %d\n", status);
        return HAL_FAIL;
    }
    size_t key_bits = psa_get_key_bits(&attributes);
    size_t output_size = PSA_BITS_TO_BYTES(key_bits);
    psa_reset_key_attributes(&attributes);

    status = psa_asymmetric_encrypt(key_id, alg, dec_data->data, dec_data->data_len, NULL, 0, enc_data->data, output_size, &enc_data->data_len);

    if (status != PSA_SUCCESS) {
        dbg("RSA encryption failed: %d\n", status);
        return HAL_FAIL;
    }

    return HAL_SUCCESS;
}

int armino_hal_rsa_decrypt(hal_data *enc_data, hal_rsa_mode *mode, uint32_t key_idx, hal_data *dec_data)
{
    HWRAP_ENTER;
    if (enc_data == NULL || enc_data->data == NULL || mode == NULL || dec_data == NULL) {
        return HAL_INVALID_ARGS;
    }

    if((armino_index_chk(key_idx)) != HAL_SUCCESS){
        return HAL_INVALID_SLOT_RANGE;
    }

    if (key_idx < FACTORY_KEY_INDEX_MAX) {
        dbg("Key index %d for factory key\n", key_idx);
        return HAL_INVALID_SLOT_RANGE;
    }

    // Open the key
    psa_key_id_t key_id = 0;
    psa_status_t status;
    psa_algorithm_t alg;
    psa_key_attributes_t attributes;

    if(is_psa_key_id_empty(key_idx)) {
        dbg("key_id %d is not generated yet\n", key_idx);
        return HAL_EMPTY_SLOT;
    } else {
        key_id = get_psa_key_id(key_idx);
    }

    switch (mode->rsa_a) {
        case HAL_RSASSA_PKCS1_V1_5:
            alg = PSA_ALG_RSA_PKCS1V15_CRYPT;
            break;
        case HAL_RSASSA_PKCS1_PSS_MGF1:
            alg = PSA_ALG_RSA_OAEP(HAL_TO_PSA_HASH_ALG(mode->hash_t));
            break;
        default:
            dbg("Unsupported RSA mode: %d\n", mode->rsa_a);
            return HAL_NOT_SUPPORTED;
    }

    status = psa_get_key_attributes(key_id, &attributes);
    if (status != PSA_SUCCESS) {
        dbg("Failed to get key attributes: %d\n", status);
        return HAL_FAIL;
    }
    size_t key_bits = psa_get_key_bits(&attributes);
    size_t output_size = PSA_BITS_TO_BYTES(key_bits);
    psa_reset_key_attributes(&attributes);

    // Perform decryption
    status = psa_asymmetric_decrypt(key_id, alg, enc_data->data, enc_data->data_len, NULL, 0, dec_data->data, output_size, &dec_data->data_len);

    if (status != PSA_SUCCESS) {
        dbg("RSA decryption failed: %d\n", status);
        return HAL_FAIL;
    }

    return HAL_SUCCESS;
}

int armino_hal_gcm_encrypt(hal_data *dec_data, hal_gcm_param *gcm_param, uint32_t key_idx, hal_data *enc_data)
{
    HWRAP_ENTER;
    if (dec_data == NULL || gcm_param == NULL ||
            enc_data == NULL || gcm_param->tag == NULL) {
        dbg("Invalid args: dec_data, gcm_param, enc_data, tag\n");
        return HAL_INVALID_ARGS;
    }

    // Set GCM algorithm
    if (gcm_param->cipher != HAL_GCM_AES) {
        dbg("Unsupported cipher: %d\n", gcm_param->cipher);
        return HAL_NOT_SUPPORTED;
    }

    psa_status_t status;

    //import the info from gcm_param to gcm_param_t
    gcm_type_t gcm_param_t;
    gcm_param_t.iv = gcm_param->iv;
    gcm_param_t.iv_len = gcm_param->iv_len;
    gcm_param_t.aad = gcm_param->aad;
    gcm_param_t.aad_len = gcm_param->aad_len;
    gcm_param_t.dec_data = dec_data->data;
    gcm_param_t.dec_data_len = dec_data->data_len;
    gcm_param_t.alg = PSA_ALG_GCM;
    gcm_param_t.key_id = key_idx;
    gcm_param_t.enc_data = enc_data->data;
    gcm_param_t.enc_data_len = gcm_param_t.dec_data_len;
    gcm_param_t.tag = gcm_param->tag;

    // Perform GCM encryption
    status = armino_hal_psa_gcm_encrypt(&gcm_param_t);
    if (status != PSA_SUCCESS) {
        dbg("GCM encryption failed: %d\n", status);
        return HAL_FAIL;
    }

    //get the output data from gcm_param_t
    enc_data->data_len = gcm_param_t.enc_data_len;
    gcm_param->tag_len = gcm_param_t.tag_len;

    return HAL_SUCCESS;
}

int armino_hal_gcm_decrypt(hal_data *enc_data, hal_gcm_param *gcm_param, uint32_t key_idx, hal_data *dec_data)
{
    HWRAP_ENTER;
    if (enc_data == NULL|| gcm_param == NULL || dec_data == NULL ) {
        return HAL_INVALID_ARGS;
    }

    psa_status_t status;

   // Set GCM algorithm
    if (gcm_param->cipher != HAL_GCM_AES) {
        dbg("Unsupported cipher: %d\n", gcm_param->cipher);
        return HAL_NOT_SUPPORTED;
    }

    //import the info from gcm_param to gcm_param_t
    gcm_type_t gcm_param_t;
    gcm_param_t.iv = gcm_param->iv;
    gcm_param_t.iv_len = gcm_param->iv_len;
    gcm_param_t.aad = gcm_param->aad;
    gcm_param_t.aad_len = gcm_param->aad_len;
    gcm_param_t.enc_data = enc_data->data;
    gcm_param_t.enc_data_len = enc_data->data_len;
    gcm_param_t.alg = PSA_ALG_GCM;
    gcm_param_t.key_id = key_idx;
    gcm_param_t.dec_data = dec_data->data;

    gcm_param_t.dec_data_len = gcm_param_t.enc_data_len;

    gcm_param_t.tag = gcm_param->tag;
    gcm_param_t.tag_len = gcm_param->tag_len;

    // Perform GCM decryption
    status = armino_hal_psa_gcm_decrypt(&gcm_param_t);
    if (status != PSA_SUCCESS) {
        dbg("GCM decryption failed: %d\n", status);
        return HAL_FAIL;
    }

    dec_data->data_len = gcm_param_t.dec_data_len;

    return HAL_SUCCESS;
}

/**
 * Secure Storage
 */
int armino_hal_write_storage(uint32_t ss_idx, hal_data *data)
{
    HWRAP_ENTER;
    if (data == NULL || data->data == NULL) {
        return HAL_INVALID_ARGS;
    }

    int ret;
    ret = ss_write_data(ss_idx, data->data, data->data_len);
    if (ret != 0) {
        dbg("Failed to write storage: %d\n", ret);
        return HAL_FAIL;
    }

    return HAL_SUCCESS;
}

int armino_hal_read_storage(uint32_t ss_idx, hal_data *data)
{
    HWRAP_ENTER;
    if (data == NULL || data->data == NULL) {
        return HAL_INVALID_ARGS;
    }

    int ret;
    ret = ss_read_data(ss_idx, data->data, &data->data_len);
    if (ret != 0) {
        dbg("Failed to read storage: %d\n", ret);
        return HAL_FAIL;
    }

    return HAL_SUCCESS;
}

int armino_hal_delete_storage(uint32_t ss_idx)
{
    HWRAP_ENTER;

    int ret;
    ret = ss_delete_data(ss_idx);
    if (ret != 0) {
        dbg("Failed to delete storage: %d\n", ret);
        return HAL_FAIL;
    }

    return HAL_SUCCESS;
}

static struct sec_ops_s g_armino_ops = {
    armino_hal_init,
    armino_hal_deinit,
    armino_hal_free_data,
    armino_hal_get_status,
    armino_hal_set_key,
    armino_hal_get_key,
    armino_hal_remove_key,
    armino_hal_generate_key,
    armino_hal_generate_random,
    armino_hal_get_hash,
    armino_hal_get_hmac,
    armino_hal_rsa_sign_md,
    armino_hal_rsa_verify_md,
    armino_hal_ecdsa_sign_md,
    armino_hal_ecdsa_verify_md,
    armino_hal_dh_generate_param,
    armino_hal_dh_compute_shared_secret,
    armino_hal_ecdh_compute_shared_secret,
    armino_hal_set_certificate,
    armino_hal_get_certificate,
    armino_hal_remove_certificate,
    armino_hal_get_factory_key,
    armino_hal_get_factory_cert,
    armino_hal_get_factory_data,
    armino_hal_aes_encrypt,
    armino_hal_aes_decrypt,
    armino_hal_rsa_encrypt,
    armino_hal_rsa_decrypt,
    armino_hal_write_storage,
    armino_hal_read_storage,
    armino_hal_delete_storage,
    armino_hal_gcm_encrypt,
    armino_hal_gcm_decrypt,
};

static struct sec_lowerhalf_s g_armino_lower = {&g_armino_ops, NULL};

int se_initialize(void)
{
    int res = se_register(SECLINK_PATH, &g_armino_lower);
    if (res != 0) {
        return -1;
    }

    return 0;
}

/*
 * It should not be used to general case.
 * It's for testing HAL APIs in user space.
 * Furthermore it wouldn't be used if binary separation is enabled.
 * Because user space couldn't access kernel space variable directly.
 * So it is protected by CONFIG after binary separation is applied.
 */
struct sec_lowerhalf_s *se_get_device(void)
{
    return &g_armino_lower;
}