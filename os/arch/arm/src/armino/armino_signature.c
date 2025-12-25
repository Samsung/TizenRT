/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/signature.h>
#include <stdint.h>
#include <psa/crypto.h>
#include <psa/crypto_extra.h>
#include <debug.h>
#include "crc32.h"

#define FIRMWARE_SIGNATURE_MAGIC    0x5349474E  /* 'SIGN' in ASCII */
#define SHA256_HASH_SIZE            (32)
#define ECDSA_PUBLIC_KEY_SIZE       (64)  /* ECDSA-256 public key size, byte[0] = 0x04 ,key format:0x04|x|y*/
#define ECDSA_SIGNATURE_SIZE        (64)  /* ECDSA-256 signature size */
#define SHA256_BUFFER_SIZE          (1024*4)

extern int bk_flash_read_bytes(size_t addr, void *buf, size_t length);

typedef struct {
    uint32_t magic;               /* Magic number, used to verify the structure */
    uint32_t sig_version;         /* Signature version */
    uint32_t firmware_size;       /* Firmware size (excluding the signature structure) */
    uint8_t firmware_hash[SHA256_HASH_SIZE];  /* SHA256 hash value of the firmware */
    uint8_t public_key[ECDSA_PUBLIC_KEY_SIZE];  /* Public key used for verification */
    uint8_t signature[ECDSA_SIGNATURE_SIZE];    /* Signature data */
    uint32_t crc;                 /* CRC checksum of the structure, optional */
} firmware_signature_t;

static void print_hex(const char *label, const uint8_t *data, size_t size)
{
    dbg("%s  size:%d \r\n ",label, size);
    for(int i=0;i<size;i++){
        dbg("%02x ",data[i]);
    }
}

static int verify_crc32(uint32_t address, uint32_t size)
{
    uint32_t cal_crc32 = 0xffffffff;
    cal_crc32 = crc32((const uint8_t *)address, size);

    if(memcmp(&cal_crc32, (uint8_t *)address + size, 4) != 0){
        dbg("app hash calc fail, line %d cal_crc32 %x , flash_crc32 %x \r\n", __LINE__, cal_crc32, *((uint32_t *)((uint8_t *)address + size)));
        return SIGNATURE_INVALID;
    }

    return SIGNATURE_VAILD;
}

static int psa_crypto_init_once(void)
{
    psa_status_t status = psa_crypto_init();

    return status;
}

static int calculate_sha256(const uint8_t *data, size_t data_size, uint8_t *hash)
{
    psa_status_t status;
    uint32_t remain_size = data_size;
    uint32_t len_in;
    uint8_t *buffer = NULL;
    psa_hash_operation_t operation = PSA_HASH_OPERATION_INIT;
    status = psa_hash_setup(&operation, PSA_ALG_SHA_256);
    if (status != PSA_SUCCESS) {
        return status;
    }
    buffer = (uint8_t *)kmm_malloc(SHA256_BUFFER_SIZE);
    if (buffer == NULL) {
        psa_hash_abort(&operation);
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }

    while (remain_size > 0)
    {
        len_in = remain_size > SHA256_BUFFER_SIZE ? SHA256_BUFFER_SIZE : remain_size;
        bk_flash_read_bytes(data, buffer, len_in);
        status = psa_hash_update(&operation, buffer, len_in);
        if (status != PSA_SUCCESS) {
            psa_hash_abort(&operation);
            kmm_free(buffer);
            return status;
        }
        remain_size -= len_in;
        data += len_in;
    }
    size_t hash_length;
    status = psa_hash_finish(&operation, hash, SHA256_HASH_SIZE, &hash_length);
    kmm_free(buffer);

    return status;
}

static int verify_signature(const uint8_t *hash, const uint8_t *signature, const uint8_t *public_key)
{
    psa_status_t status;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t key_id = 0;

    //psa_key_attributes_init();
    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_VERIFY_HASH);
    psa_set_key_algorithm(&attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_256));
    psa_set_key_type(&attributes, PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1));
    psa_set_key_bits(&attributes, 256);
    psa_set_key_lifetime(&attributes, PSA_KEY_LIFETIME_VOLATILE);
    status = psa_import_key(&attributes, public_key, ECDSA_PUBLIC_KEY_SIZE+1, &key_id);
    if (status != PSA_SUCCESS || key_id == 0) {
        psa_reset_key_attributes(&attributes);
        return status;
    }
    status = psa_verify_message(key_id, PSA_ALG_ECDSA(PSA_ALG_SHA_256), hash, SHA256_HASH_SIZE, signature, ECDSA_SIGNATURE_SIZE);
 
    psa_destroy_key(key_id);
    psa_reset_key_attributes(&attributes);
    return status;
}

static int verify_firmware_signature(uint32_t address)
{
    firmware_signature_t sig = {0x0};
    uint8_t pub_key[ECDSA_PUBLIC_KEY_SIZE + 1] = {0x04}; //pub_key[0]=0x04
    uint8_t calculated_hash[SHA256_HASH_SIZE] = {0x0};
    int result = SIGNATURE_INVALID;

    bk_flash_read_bytes(address, (uint8_t *)&sig, sizeof(firmware_signature_t));
    dbg("app signature: line %d magic %x, size %x\r\n", __LINE__, sig.magic, sig.firmware_size);
    if (sig.magic != FIRMWARE_SIGNATURE_MAGIC) {
        return SIGNATURE_INVALID;
    }

    if (verify_crc32((uint32_t)&sig, sizeof(firmware_signature_t)-4) != SIGNATURE_VAILD) {
        return SIGNATURE_INVALID;
    }

    if (psa_crypto_init_once() != SIGNATURE_VAILD) {
        return SIGNATURE_INVALID;
    }
    result = calculate_sha256((uint8_t *)(address + sizeof(firmware_signature_t)), 
                              sig.firmware_size, calculated_hash);
    if (result != SIGNATURE_VAILD) {
        dbg("app hash calc fail, line %d result %d\r\n", __LINE__, result);
        return SIGNATURE_INVALID;
    }
    if (memcmp(calculated_hash, sig.firmware_hash, SHA256_HASH_SIZE) != 0) {
        dbg("app hash calc not eque\r\n");
        //print_hex("calculated_hash",calculated_hash,SHA256_HASH_SIZE);
        //print_hex("sig->firmware_hash",sig->firmware_hash,SHA256_HASH_SIZE);
        return SIGNATURE_INVALID;
    }
    memcpy(&pub_key[1], sig.public_key, ECDSA_PUBLIC_KEY_SIZE);
    result = verify_signature(sig.firmware_hash, sig.signature, pub_key);
    if (result != SIGNATURE_VAILD) {
        dbg("app signature verify fail, line %d result %d\r\n", __LINE__, result);
        return SIGNATURE_INVALID;
    }
    return SIGNATURE_VAILD;
}

int up_verify_kernelsignature(uint32_t address)
{
	return SIGNATURE_VAILD;
}

#ifdef CONFIG_APP_BINARY_SEPARATION
int up_verify_usersignature(uint32_t address)
{
    int result = SIGNATURE_INVALID;

    result = verify_firmware_signature(address);
    if (result != SIGNATURE_VAILD) {
        dbg("app partition addr %p\r\n", (uint32_t *)address);
        return SIGNATURE_INVALID;
    }

    return SIGNATURE_VAILD;
}
#endif
