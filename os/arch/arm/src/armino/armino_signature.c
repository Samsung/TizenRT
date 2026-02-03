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
#include <string.h>
#include <psa/crypto.h>
#include <psa/crypto_extra.h>
#include <os/os.h>
#include <debug.h>
#include "crc32.h"
#include "driver/flash.h"
#include "flash.h"

#define FIRMWARE_SIGNATURE_MAGIC    0x5349474E  /* 'SIGN' in ASCII */
#define SHA256_HASH_SIZE            (32)
#define ECDSA_PUBLIC_KEY_SIZE       (64)  /* ECDSA-256 public key size, byte[0] = 0x04 ,key format:0x04|x|y*/
#define ECDSA_SIGNATURE_SIZE        (64)  /* ECDSA-256 signature size */
#define SHA256_BUFFER_SIZE          (1024*4)

/****************************************************************************
 * kernel Image Format Definitions
 ****************************************************************************/
#define IMAGE_MAGIC                 0x96f3b83d
#define IMAGE_HEADER_SIZE           32
#define IMAGE_HEAD_OFFSET           16   /* Skip first 16 bytes of image header */

/* TLV Magic Numbers */
#define IMAGE_TLV_INFO_MAGIC        0x6907
#define IMAGE_TLV_PROT_INFO_MAGIC   0x6908

/* TLV Types */
#define IMAGE_TLV_KEYHASH           0x01   /* hash of the public key */
#define IMAGE_TLV_PUBKEY            0x02   /* public key */
#define IMAGE_TLV_SHA256            0x10   /* SHA256 of image hdr and body */
#define IMAGE_TLV_ECDSA256          0x22   /* ECDSA of hash output */

/* ECDSA P-256 public key in SubjectPublicKeyInfo DER format */
#define ECDSA_PUBKEY_DER_SIZE       91    /* DER encoded ECDSA P-256 public key size */
#define ECDSA_SIG_MAX_SIZE          72    /* Max DER encoded ECDSA signature size */
#define NUM_ECC_BYTES               32    /* ECDSA P-256 coordinate size */

/****************************************************************************
 * kernel Image Header Structure (32 bytes, little endian)
 ****************************************************************************/
struct kernel_image_version {
    uint8_t iv_major;
    uint8_t iv_minor;
    uint16_t iv_revision;
    uint32_t iv_build_num;
};

struct kernel_image_header {
    uint32_t ih_magic;
    uint32_t ih_load_addr;
    uint16_t ih_hdr_size;           /* Size of image header (bytes) */
    uint16_t ih_protect_tlv_size;   /* Size of protected TLV area (bytes) */
    uint32_t ih_img_size;           /* Does not include header */
    uint32_t ih_flags;
    struct kernel_image_version ih_ver;
    uint32_t _pad1;
};

/****************************************************************************
 * kernel TLV Structures
 ****************************************************************************/
struct kernel_tlv_info {
    uint16_t it_magic;
    uint16_t it_tlv_tot;  /* size of TLV area (including tlv_info header) */
};

struct kernel_tlv {
    uint16_t it_type;
    uint16_t it_len;      /* Data length (not including TLV header) */
};

/* External function declaration for reading flash instruction data */
extern int bk_security_flash_read_instruction(uint32_t address, uint8_t *user_buf, uint32_t size, uint32_t offset_flag);

typedef struct {
    uint32_t magic;               /* Magic number, used to verify the structure */
    uint32_t sig_version;         /* Signature version */
    uint32_t firmware_size;       /* Firmware size (excluding the signature structure) */
    uint8_t firmware_hash[SHA256_HASH_SIZE];  /* SHA256 hash value of the firmware */
    uint8_t public_key[ECDSA_PUBLIC_KEY_SIZE];  /* Public key used for verification */
    uint8_t signature[ECDSA_SIGNATURE_SIZE];    /* Signature data */
    uint8_t reserved[16];         /* padding to 32 bytes */
    uint32_t crc;                 /* CRC checksum of the structure, optional */
} firmware_signature_t;

enum {
    VERIFY_SUCCESS = 0,
    VERIFY_MAGIC_CHECK_FAIL,
    VERIFY_CRC_CHECK_FAIL,
    VERIFY_CRYPTO_INIT_FAIL,
    VERIFY_HASH_CAL_FAIL,
    VERIFY_HASH_CHECK_FAIL,
    VERIFY_SIGNATURE_VERIFY_FAIL,
    VERIFY_BUFFER_MALLOC_FAIL,
};

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
        return VERIFY_CRC_CHECK_FAIL;
    }

    return VERIFY_SUCCESS;
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
        dbg("failed to malloc buffer\r\n");
        return VERIFY_BUFFER_MALLOC_FAIL;
    }

    while (remain_size > 0)
    {
        len_in = remain_size > SHA256_BUFFER_SIZE ? SHA256_BUFFER_SIZE : remain_size;
        bk_instruction_read_app_or_common(data, buffer, len_in);
        status = psa_hash_update(&operation, buffer, len_in);
        if (status != PSA_SUCCESS) {
            psa_hash_abort(&operation);
            kmm_free(buffer);
            buffer = NULL;
            return VERIFY_HASH_CAL_FAIL;
        }
        remain_size -= len_in;
        data += len_in;
    }
    size_t hash_length;
    status = psa_hash_finish(&operation, hash, SHA256_HASH_SIZE, &hash_length);
    kmm_free(buffer);
    buffer = NULL;
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
    firmware_signature_t sig = {0};
    uint8_t pub_key[ECDSA_PUBLIC_KEY_SIZE + 1] = {0x04}; //pub_key[0]=0x04
    uint8_t calculated_hash[SHA256_HASH_SIZE] = {0x0};
    int result;

    bk_instruction_read_app_or_common(address, (uint8_t *)&sig, sizeof(firmware_signature_t));
    dbg("app signature: line %d magic %x, size %x\r\n", __LINE__, sig.magic, sig.firmware_size);
    if (sig.magic != FIRMWARE_SIGNATURE_MAGIC) {
        return VERIFY_MAGIC_CHECK_FAIL;
    }

    if (verify_crc32((uint32_t)&sig, sizeof(firmware_signature_t)-4) != VERIFY_SUCCESS) {
        return VERIFY_CRC_CHECK_FAIL;
    }

    if (psa_crypto_init_once() != PSA_SUCCESS) {
        return VERIFY_CRYPTO_INIT_FAIL;
    }
    result = calculate_sha256((uint8_t *)(address + sizeof(firmware_signature_t)), 
                              sig.firmware_size, calculated_hash);
    if (result != PSA_SUCCESS) {
        dbg("app hash calc fail, line %d result %d\r\n", __LINE__, result);
        return VERIFY_HASH_CAL_FAIL;
    }
    if (memcmp(calculated_hash, sig.firmware_hash, SHA256_HASH_SIZE) != 0) {
        dbg("app hash calc not equal\r\n");
        //print_hex("calculated_hash",calculated_hash,SHA256_HASH_SIZE);
        //print_hex("sig.firmware_hash",sig.firmware_hash,SHA256_HASH_SIZE);
        return VERIFY_HASH_CHECK_FAIL;
    }
    memcpy(&pub_key[1], sig.public_key, ECDSA_PUBLIC_KEY_SIZE);
    result = verify_signature(sig.firmware_hash, sig.signature, pub_key);
    if (result != PSA_SUCCESS) {
        dbg("app signature verify fail, line %d result %d\r\n", __LINE__, result);
        return VERIFY_SIGNATURE_VERIFY_FAIL;
    }
    return VERIFY_SUCCESS;
}

/****************************************************************************
 * Kernel Signature Verification Helper Functions
 ****************************************************************************/

/**
 * @brief Calculate SHA256 hash of data read from flash using secure read interface
 * @param address Flash address to read from
 * @param size Size of data to hash
 * @param hash Output buffer for 32-byte hash
 * @return PSA_SUCCESS on success, error code otherwise
 */
static psa_status_t kernel_calculate_flash_hash(uint32_t address, uint32_t size, uint8_t *hash, uint32_t flag)
{
    psa_status_t status;
    psa_hash_operation_t operation = PSA_HASH_OPERATION_INIT;
    uint8_t *buffer = NULL;
    uint32_t remain_size = size;
    uint32_t offset = 0;
    uint32_t len_in;
    int rc;
    buffer = (uint8_t *)kmm_malloc(SHA256_BUFFER_SIZE);
    if (buffer == NULL) {
        dbg("failed to malloc buffer\r\n");
        return -1;
    }
    status = psa_hash_setup(&operation, PSA_ALG_SHA_256);
    if (status != PSA_SUCCESS) {
        dbg("hash setup failed, status=%d\r\n", status);
        psa_hash_abort(&operation);
        kmm_free(buffer);
        buffer = NULL;
        return status;
    }

    while (remain_size > 0) {
        // If address is not 4K aligned, first packet only processes up to the 4K boundary.
        if (offset == 0 && (address % SHA256_BUFFER_SIZE != 0)) {
            uint32_t align_offset = SHA256_BUFFER_SIZE - (address % SHA256_BUFFER_SIZE);
            len_in = (remain_size > align_offset) ? align_offset : remain_size;
        } else {
            len_in = remain_size > SHA256_BUFFER_SIZE ? SHA256_BUFFER_SIZE : remain_size;
        }
        
        rc = bk_security_flash_read_instruction(address + offset, buffer, len_in, flag);
        if (rc != 0) {
            dbg("flash read failed at offset 0x%x, rc=%d\r\n", offset, rc);
            psa_hash_abort(&operation);
            kmm_free(buffer);
            buffer = NULL;
            return PSA_ERROR_STORAGE_FAILURE;
        }

        status = psa_hash_update(&operation, buffer, len_in);
        if (status != PSA_SUCCESS) {
            dbg("hash update failed, status=%d\r\n", status);
            psa_hash_abort(&operation);
            kmm_free(buffer);
            buffer = NULL;
            return status;
        }

        remain_size -= len_in;
        offset += len_in;
    }

    size_t hash_length;
    status = psa_hash_finish(&operation, hash, SHA256_HASH_SIZE, &hash_length);
    if (status != PSA_SUCCESS) {
        dbg("hash finish failed, status=%d\r\n", status);
    }

    return status;
}

/**
 * @brief Parse DER-encoded ECDSA signature to raw r||s format
 * @param der_sig DER encoded signature
 * @param der_len DER signature length
 * @param raw_sig Output raw signature (64 bytes: r||s)
 * @return 0 on success, -1 on failure
 */
static int kernel_decode_ecdsa_sig(const uint8_t *der_sig, uint16_t der_len, uint8_t *raw_sig)
{
    const uint8_t *p = der_sig;
    const uint8_t *end = der_sig + der_len;
    size_t seq_len, int_len;

    /* Check SEQUENCE tag */
    if (*p++ != 0x30) {
        return -1;
    }

    /* Parse SEQUENCE length */
    if (*p & 0x80) {
        return -1;  /* Long form length not expected */
    }
    seq_len = *p++;
    if (p + seq_len > end) {
        return -1;
    }

    /* Parse r INTEGER */
    if (*p++ != 0x02) {
        return -1;
    }
    int_len = *p++;
    
    /* Handle leading zeros in r */
    if (int_len >= NUM_ECC_BYTES) {
        memcpy(raw_sig, p + int_len - NUM_ECC_BYTES, NUM_ECC_BYTES);
    } else {
        memset(raw_sig, 0, NUM_ECC_BYTES - int_len);
        memcpy(raw_sig + NUM_ECC_BYTES - int_len, p, int_len);
    }
    p += int_len;

    /* Parse s INTEGER */
    if (*p++ != 0x02) {
        return -1;
    }
    int_len = *p++;

    /* Handle leading zeros in s */
    if (int_len >= NUM_ECC_BYTES) {
        memcpy(raw_sig + NUM_ECC_BYTES, p + int_len - NUM_ECC_BYTES, NUM_ECC_BYTES);
    } else {
        memset(raw_sig + NUM_ECC_BYTES, 0, NUM_ECC_BYTES - int_len);
        memcpy(raw_sig + NUM_ECC_BYTES + NUM_ECC_BYTES - int_len, p, int_len);
    }

    return 0;
}

/**
 * @brief Parse SubjectPublicKeyInfo DER format to extract raw EC point
 * @param der_key DER encoded public key
 * @param der_len DER key length
 * @param raw_key Output raw key (65 bytes: 0x04||x||y)
 * @return 0 on success, -1 on failure
 */
static int kernel_parse_ec_pubkey(const uint8_t *der_key, uint16_t der_len, uint8_t *raw_key)
{
    const uint8_t *p = der_key;
    const uint8_t *end = der_key + der_len;
    size_t len;

    /* Parse outer SEQUENCE */
    if (*p++ != 0x30) {
        return -1;
    }
    if (*p & 0x80) {
        if ((*p & 0x7f) != 1) return -1;
        p++;
    }
    len = *p++;
    if (p + len > end) {
        return -1;
    }

    if (*p++ != 0x30) {
        return -1;
    }
    len = *p++;
    p += len;

    if (*p++ != 0x03) {
        return -1;
    }
    len = *p++;
    if (*p++ != 0x00) {
        return -1;
    }
    len--;

    if (len != 65 || *p != 0x04) {
        return -1;
    }

    memcpy(raw_key, p, 65);
    return 0;
}

/**
 * @brief Verify ECDSA P-256 signature using PSA Crypto API
 * @param hash Hash to verify (32 bytes)
 * @param signature DER encoded signature
 * @param sig_len Signature length
 * @param pubkey DER encoded public key
 * @param pubkey_len Public key length
 * @return PSA_SUCCESS on success, error code otherwise
 */
static psa_status_t kernel_verify_ecdsa_signature(const uint8_t *hash,
                                                    const uint8_t *signature, uint16_t sig_len,
                                                    const uint8_t *pubkey, uint16_t pubkey_len)
{
    psa_status_t status;
    psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t key_id = 0;
    uint8_t raw_sig[64];       /* r||s format */
    uint8_t raw_pubkey[65];    /* 0x04||x||y format */

    if (kernel_decode_ecdsa_sig(signature, sig_len, raw_sig) != 0) {
        dbg("failed to decode ECDSA signature\r\n");
        return PSA_ERROR_INVALID_SIGNATURE;
    }

    if (kernel_parse_ec_pubkey(pubkey, pubkey_len, raw_pubkey) != 0) {
        dbg("failed to parse EC public key\r\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_VERIFY_HASH);
    psa_set_key_algorithm(&attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_256));
    psa_set_key_type(&attributes, PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1));
    psa_set_key_bits(&attributes, 256);
    psa_set_key_lifetime(&attributes, PSA_KEY_LIFETIME_VOLATILE);

    status = psa_import_key(&attributes, raw_pubkey, 65, &key_id);
    if (status != PSA_SUCCESS || key_id == 0) {
        dbg("failed to import public key, status=%d\r\n", status);
        psa_reset_key_attributes(&attributes);
        return status;
    }

    status = psa_verify_message(key_id, PSA_ALG_ECDSA(PSA_ALG_SHA_256), 
                             hash, SHA256_HASH_SIZE, raw_sig, 64);

    psa_destroy_key(key_id);
    psa_reset_key_attributes(&attributes);

    if (status != PSA_SUCCESS) {
        dbg("signature verification failed, status=%d\r\n", status);
    }

    return status;
}

int up_verify_kernelsignature(uint32_t address)
{
    struct kernel_image_header hdr;
    struct kernel_tlv_info tlv_info;
    struct kernel_tlv tlv;
    uint8_t calc_hash[SHA256_HASH_SIZE];
    uint8_t stored_hash[SHA256_HASH_SIZE];
    uint8_t pubkey_buf[ECDSA_PUBKEY_DER_SIZE];
    uint8_t sig_buf[ECDSA_SIG_MAX_SIZE];
    uint32_t hash_size;
    uint32_t tlv_off;
    uint32_t tlv_end;
    uint32_t prot_tlv_end;
    uint16_t pubkey_len = 0;
    uint16_t sig_len = 0;
    int sha256_valid = 0;
    int pubkey_found = 0;
    int signature_found = 0;
    int rc;
    uint32_t flag = 0;
    psa_status_t status;

    dbg("starting verification at address 0x%x\r\n", address);
    rc = bk_security_flash_addr_translate(&address, &flag);
    if (rc != 0) {
        dbg("starting address invalid 0x%x\r\n", address);
        return SIGNATURE_INVALID;
    }

    status = psa_crypto_init();
    if (status != PSA_SUCCESS) {
        dbg("PSA crypto init failed, status=%d\r\n", status);
        return SIGNATURE_INVALID;
    }

    rc = bk_security_flash_read_instruction(address + IMAGE_HEAD_OFFSET, 
                                            (uint8_t *)&hdr, sizeof(hdr), flag);
    if (rc != 0) {
        dbg("failed to read image header, rc=%d address = 0x%x = flag %d\r\n", rc, address, flag);
        return SIGNATURE_INVALID;
    }

    if (hdr.ih_magic != IMAGE_MAGIC) {
        dbg("invalid magic 0x%x, expected 0x%x\r\n", 
            hdr.ih_magic, IMAGE_MAGIC);
        return SIGNATURE_INVALID;
    }

    dbg("header valid - hdr_size=%d, img_size=%d, prot_tlv_size=%d\r\n",
        hdr.ih_hdr_size, hdr.ih_img_size, hdr.ih_protect_tlv_size);

    hash_size = (hdr.ih_hdr_size - IMAGE_HEAD_OFFSET) + hdr.ih_img_size + hdr.ih_protect_tlv_size;
    
    status = kernel_calculate_flash_hash(address + IMAGE_HEAD_OFFSET, hash_size, calc_hash, flag);
    if (status != PSA_SUCCESS) {
        dbg("failed to calculate image hash %d\r\n", status);
        return SIGNATURE_INVALID;
    }

    tlv_off = address + IMAGE_HEAD_OFFSET + (hdr.ih_hdr_size - IMAGE_HEAD_OFFSET) + hdr.ih_img_size;
    prot_tlv_end = tlv_off + hdr.ih_protect_tlv_size;

    if (hdr.ih_protect_tlv_size > 0) {
        rc = bk_security_flash_read_instruction(tlv_off, (uint8_t *)&tlv_info, sizeof(tlv_info), flag);
        if (rc != 0 || tlv_info.it_magic != IMAGE_TLV_PROT_INFO_MAGIC) {
            dbg("invalid protected TLV info\r\n");
            return SIGNATURE_INVALID;
        }
        tlv_off = prot_tlv_end;
    }

    /* Read unprotected TLV info */
    rc = bk_security_flash_read_instruction(tlv_off, (uint8_t *)&tlv_info, sizeof(tlv_info), flag);
    if (rc != 0 || tlv_info.it_magic != IMAGE_TLV_INFO_MAGIC) {
        dbg("invalid TLV info, magic=0x%x\r\n", tlv_info.it_magic);
        return SIGNATURE_INVALID;
    }

    tlv_end = tlv_off + tlv_info.it_tlv_tot;
    tlv_off += sizeof(struct kernel_tlv_info);

    while (tlv_off < tlv_end) {
        rc = bk_security_flash_read_instruction(tlv_off, (uint8_t *)&tlv, sizeof(tlv), flag);
        if (rc != 0) {
            dbg("failed to read TLV at offset 0x%x\r\n", tlv_off);
            return SIGNATURE_INVALID;
        }

        tlv_off += sizeof(struct kernel_tlv);

        switch (tlv.it_type) {
        case IMAGE_TLV_SHA256:
            /* Verify stored hash matches calculated hash */
            if (tlv.it_len != SHA256_HASH_SIZE) {
                dbg("invalid SHA256 TLV length %d\r\n", tlv.it_len);
                return SIGNATURE_INVALID;
            }
            rc = bk_security_flash_read_instruction(tlv_off, stored_hash, SHA256_HASH_SIZE, flag);
            if (rc != 0) {
                return SIGNATURE_INVALID;
            }
            if (memcmp(calc_hash, stored_hash, SHA256_HASH_SIZE) != 0) {
                dbg("hash mismatch\r\n");
                return SIGNATURE_INVALID;
            }
            sha256_valid = 1;
            dbg("SHA256 hash verified\r\n");
            break;

        case IMAGE_TLV_PUBKEY:
            /* Store public key for later signature verification */
            if (tlv.it_len > sizeof(pubkey_buf)) {
                dbg("public key too large %d\r\n", tlv.it_len);
                return SIGNATURE_INVALID;
            }
            rc = bk_security_flash_read_instruction(tlv_off, pubkey_buf, tlv.it_len, flag);
            if (rc != 0) {
                return SIGNATURE_INVALID;
            }
            pubkey_len = tlv.it_len;
            pubkey_found = 1;
            dbg("found public key, len=%d\r\n", pubkey_len);
            break;

        case IMAGE_TLV_ECDSA256:
            if (tlv.it_len > sizeof(sig_buf)) {
                dbg("signature too large %d\r\n", tlv.it_len);
                return SIGNATURE_INVALID;
            }
            rc = bk_security_flash_read_instruction(tlv_off, sig_buf, tlv.it_len, flag);
            if (rc != 0) {
                return SIGNATURE_INVALID;
            }
            sig_len = tlv.it_len;
            signature_found = 1;
            dbg("found signature, len=%d\r\n", sig_len);
            break;

        default:
            /* Skip unknown TLV types */
            break;
        }

        tlv_off += tlv.it_len;
    }

    if (!sha256_valid) {
        dbg("SHA256 hash not found or invalid\r\n");
        return SIGNATURE_INVALID;
    }

    if (!pubkey_found || !signature_found) {
        dbg("missing public key or signature\r\n");
        return SIGNATURE_INVALID;
    }

    status = kernel_verify_ecdsa_signature(calc_hash, sig_buf, sig_len, 
                                             pubkey_buf, pubkey_len);
    if (status != PSA_SUCCESS) {
        dbg("ECDSA signature verification failed %d\r\n", status);
        return SIGNATURE_INVALID;
    }

    dbg("signature verification SUCCESS\r\n");
    return SIGNATURE_VAILD;
}

#ifdef CONFIG_APP_BINARY_SEPARATION
int up_verify_usersignature(uint32_t address)
{
    int result = SIGNATURE_INVALID;

    result = verify_firmware_signature(address);
    if (result != VERIFY_SUCCESS) {
        dbg("app partition addr %p, result %d\r\n", (uint32_t *)address, result);
        return SIGNATURE_INVALID;
    }

    return SIGNATURE_VAILD;
}
#endif
