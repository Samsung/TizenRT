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

#include <stdio.h>
#include <string.h>
#include <os/os.h>
#include <os/mem.h>
#include "dubhe_gcm.h"
#include "dubhe_sca.h" 
#include "_otp.h"
#include "otp_s.h"
#include "psa/crypto.h"
#include "driver/flash.h"
#include "tfm_secure_api.h"
#include "tfm_secure_storage.h"

#define AES_GCM_KEY_LENGTH        (32)

/* Define partition size constants */
#define SS_KEY_PARTITION_SIZE        (4 * 1024)  // 4KB
#define SS_CERT_PARTITION_SIZE       (4 * 1024)  // 4KB
#define SS_DATA_PARTITION_SIZE       (4 * 1024)  // 4KB
#define SS_DATA_FIRST_PARTITION_SIZE (12 * 1024) // First data partition is 12KB
#define SS_DATA_ALIGN_SIZE           (4 * 1024)  // 4KB

/* Define data types */
#define SS_TYPE_KEY                 (1)
#define SS_TYPE_CERT                (2)
#define SS_TYPE_DATA                (3)

/* Define permission types */
#define SS_PERM_READ_ONLY           (1)
#define SS_PERM_READ_WRITE          (2)

/* Define magic value */
#define SS_MAGIC                    0x73736B62 // bkss

/* Define version number */
#define SS_VERSION                  (0)

#define SS_PARTITION_BASE_ADDR          (0x2E000) //BEKEN WORKAROUND TODO: need read from partition table
#define CONFIG_SS_KEY_PARTITION(addr)   (addr)
#define CONFIG_SS_CERT_PARTITION(addr)  (CONFIG_SS_KEY_PARTITION(addr) + SS_KEY_PARTITION_SIZE * SS_RAM_INDEX_START)
#define CONFIG_SS_DATA_PARTITION(addr)  (CONFIG_SS_CERT_PARTITION(addr) + SS_CERT_PARTITION_SIZE * SS_RAM_INDEX_START)

#define AES_GCM_IV_LEN              (12)
// RAM storage data structure definition
typedef struct {
    uint8_t  *data;       // Data pointer
    uint32_t data_len;    // Data length
    uint8_t  permission;  // Permission
    uint8_t  valid;       // Data validity flag
} ss_ram_data_t;

// Flash storage data structure
typedef struct {
	uint32_t magic;    // bkss
	uint8_t  version;
	uint8_t  permision;   // 1: read only  2: readwrite  3: NA
	uint8_t  type;        // 1: key 2: certification  3: data
	uint8_t  reserved;
	uint8_t  IV[AES_GCM_IV_LEN];
	uint8_t  tag[16];
	uint32_t cipher_len;  // As long as the plaintext
	uint8_t  cipher[0];
} ss_header_t;

// BEKEN WORKAROUND TODO:This key need get from OTP
static unsigned char s_aes_gcm_key_val[AES_GCM_KEY_LENGTH] = {
    0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
};

/* RAM storage arrays - for storing data with index 32-63 */
static ss_ram_data_t s_ram_key_storage[SS_RAM_INDEX_START] = {0};
static ss_ram_data_t s_ram_cert_storage[SS_RAM_INDEX_START] = {0};
extern int flash_op_enable_ctrl(uint32_t module,uint32_t enable);

/* Calling this API in tfm_s.bin to trigger the compiler to generate NSC symbols in tfm_s_veneers.o.
 **/
void psa_secure_storage_nsc_stub(void)
{
	return;
}

static psa_status_t aes_gcm_crypt(int32_t mode, unsigned char*iv, unsigned char*input, size_t in_len, unsigned char*output)
{
	arm_ce_gcm_ctx_t gcm_ctx;
	unsigned char  tag[AES_GCM_KEY_LENGTH/2];
    int ret = 0;

    ret = arm_ce_gcm_init(&gcm_ctx, ARM_CE_AES_GCM);
    if (ret != 0) {
        printf("arm_ce_gcm_init fail, ret = %d\r\n", ret);
        return ret;
    }
    ret = arm_ce_gcm_set_key(&gcm_ctx,  s_aes_gcm_key_val, sizeof(s_aes_gcm_key_val)*8);
    if (ret != 0) {
        arm_ce_gcm_free(&gcm_ctx);
        printf("arm_ce_gcm_set_key fail, ret = %d\r\n", ret);
        return ret;
    }
    ret = arm_ce_gcm_start(&gcm_ctx, mode, iv, AES_GCM_IV_LEN);
    if (ret != 0) {
        arm_ce_gcm_free(&gcm_ctx);
        printf("arm_ce_gcm_start fail, ret = %d\r\n", ret);
        return ret;
    }

    ret = arm_ce_gcm_update(&gcm_ctx, in_len, input, output);
    if (ret != 0) {
        arm_ce_gcm_free(&gcm_ctx);
        printf("arm_ce_gcm_update fail, ret = %d\r\n", ret);
        return ret;
    }

    ret = arm_ce_gcm_update(&gcm_ctx, 0, NULL, NULL);
    if (ret != 0) {
        arm_ce_gcm_free(&gcm_ctx);
        printf("arm_ce_gcm_update fail, ret = %d\r\n", ret);
        return ret;
    }

    ret = arm_ce_gcm_finish(&gcm_ctx, tag, sizeof(tag));
    if (ret != 0) {
        arm_ce_gcm_free(&gcm_ctx);
        printf("arm_ce_gcm_finish fail, ret = %d\r\n", ret);
        return ret;
    }

    arm_ce_gcm_free(&gcm_ctx);
    return 0;
}

/**
 * @brief Calculate the storage address in Flash for data of specified type and index
 *
 * @param type Data type (SS_TYPE_KEY/SS_TYPE_CERT/SS_TYPE_DATA)
 * @param index Data index (0-63)
 * @param addr Output parameter to store the calculated address
 *
 * @return 0 on success, other values on failure
 */
static int get_ss_address(uint8_t type, uint32_t index, uint32_t *addr)
{
    uint32_t base_addr = 0;
    uint32_t part_addr = SS_PARTITION_BASE_ADDR;
    uint32_t offset = 0;

    if ((type == SS_TYPE_DATA && index >= CONFIG_SS_DATA_INDEX_MAX) ||
        (type == SS_TYPE_KEY && index >= CONFIG_SS_KEY_INDEX_MAX) ||
        (type == SS_TYPE_CERT && index >= CONFIG_SS_CERT_INDEX_MAX)) {
        return SS_INVALID_SLOT_RANGE;
    }

    if ((type == SS_TYPE_KEY || type == SS_TYPE_CERT) && index >= SS_RAM_INDEX_START) {
            *addr = 0; // Special value indicating RAM storage
            return SS_SUCCESS;
    }

    // key, certify and data with index 0-31 (data 0-32) is stored in Flash
    if (type == SS_TYPE_KEY) {
        offset = index * SS_KEY_PARTITION_SIZE;
        base_addr = CONFIG_SS_KEY_PARTITION(part_addr);
    } else if (type == SS_TYPE_CERT) {
        offset = index * SS_CERT_PARTITION_SIZE;
        base_addr = CONFIG_SS_CERT_PARTITION(part_addr);
    } else if (type == SS_TYPE_DATA) {
        /* First data partition is 12KB, others are 4KB */
        if (index == 0) {
            offset = 0;
        } else {
            offset = SS_DATA_FIRST_PARTITION_SIZE + (index - 1) * SS_DATA_PARTITION_SIZE;
        }
        base_addr = CONFIG_SS_DATA_PARTITION(part_addr);
    } else {
        return SS_INVALID_SLOT_TYPE;
    }

    if (((base_addr + offset) % SS_DATA_ALIGN_SIZE) != 0) {
        printf("Invalid address alignment: %x, %d\n", base_addr + offset, __LINE__);
        return SS_FAIL;
    }

    *addr = base_addr + offset;
    return SS_SUCCESS;
}

/**
 * @brief Generate random IV value
 *
 * @param iv Output parameter to store the generated IV
 * @param iv_len IV length
 *
 * @return 0 on success, other values on failure
 */
static int generate_random_iv(uint8_t *iv, uint32_t iv_len)
{
    extern int arm_ce_seed_read( unsigned char *buf, size_t buf_len );
    arm_ce_seed_read(iv, iv_len);

    return SS_SUCCESS;
}

/**
 * @brief Verify the stored magic value
 *
 * @param header Pointer to ss_header_t structure
 *
 * @return 0 on verification success, other values on verification failure
 */
static int verify_magic(ss_header_t *header)
{
    if (header->magic != SS_MAGIC) {
        return SS_FAIL;
    }
    return SS_SUCCESS;
}

static int tfm_flash_erase(uint32_t addr)
{
    int ret;
    flash_op_enable_ctrl(1,1);
    ret = bk_flash_erase_sector(addr);
    flash_op_enable_ctrl(1,0);

    return ret;
}

static int tfm_flash_write(uint32_t addr, uint8_t *data, uint32_t data_len)
{
    int ret = 0;
    flash_op_enable_ctrl(1, 1);
    ret = bk_flash_write_bytes(addr, data, data_len);
    flash_op_enable_ctrl(1, 0);

    return ret;
}

static int tfm_flash_read(uint32_t addr, uint8_t *data, uint32_t data_len)
{
    int ret = 0;
    ret = bk_flash_read_bytes(addr, data, data_len);

    return ret;
}

/**
 * @brief Secure storage write function
 *
 * @param type Data type (SS_TYPE_KEY/SS_TYPE_CERT/SS_TYPE_DATA)
 * @param index Data index (0-63)
 * @param data Data to be written
 * @param data_len Data length
 * @param permission Permission (SS_PERM_READ_ONLY/SS_PERM_READ_WRITE)
 *
 * @return 0 on success, other values on failure
 */
int ss_write(uint8_t type, uint32_t index, uint8_t *data, uint32_t data_len, uint8_t permission)
{
    int ret = 0;
    uint32_t addr = 0;

    /* Check parameter validity */
    if (data == NULL || data_len == 0) {
        return SS_INVALID_ARGS;
    }

    /* Calculate storage address */
    ret = get_ss_address(type, index, &addr);
    if (ret != 0) {
        return ret;
    }

    // Handle RAM storage (index 32-63)
    if (addr == 0) {
        uint32_t ram_index = index - SS_RAM_INDEX_START;
        ss_ram_data_t *storage = NULL;

        // Select corresponding RAM storage array
        switch (type) {
            case SS_TYPE_KEY:
                storage = &s_ram_key_storage[ram_index];
                break;
            case SS_TYPE_CERT:
                storage = &s_ram_cert_storage[ram_index];
                break;
            case SS_TYPE_DATA:
                break;
            default:
                return SS_INVALID_SLOT_TYPE;
        }

        // Free existing data if valid
        if (storage->valid && storage->data != NULL) {
            os_free(storage->data);
        }

        // Allocate memory and copy data (no encryption)
        storage->data = (uint8_t *)os_malloc(data_len);
        if (storage->data == NULL) {
            return SS_ALLOC_FAIL;
        }

        memcpy(storage->data, data, data_len);
        storage->data_len = data_len;
        storage->permission = permission;
        storage->valid = 1;

        return SS_SUCCESS;
    }

    // Handle Flash storage (index 0-31), keep existing encryption logic
    ss_header_t header;
    uint8_t *cipher_data = NULL;
    uint32_t partition_size = 0;

    /* Calculate partition size */
    if (type == SS_TYPE_KEY) {
        partition_size = SS_KEY_PARTITION_SIZE;
    } else if (type == SS_TYPE_CERT) {
        partition_size = SS_CERT_PARTITION_SIZE;
    } else if (type == SS_TYPE_DATA) {
        partition_size = (index == 0) ? SS_DATA_FIRST_PARTITION_SIZE : SS_DATA_PARTITION_SIZE;
    }

    /* Check if data length exceeds partition size */
    if ((sizeof(ss_header_t) + data_len) > partition_size) {
        return SS_NOT_ENOUGH_MEMORY;
    }

    /* Allocate space to store ciphertext data */
    cipher_data = (uint8_t *)os_malloc(data_len);
    if (cipher_data == NULL) {
        return SS_ALLOC_FAIL;
    }

    memcpy(cipher_data, data, data_len);

    /* Initialize header */
    header.magic      = SS_MAGIC;
    header.version    = SS_VERSION;
    header.permision  = permission;
    header.type       = type;
    header.reserved   = 0;
    header.cipher_len = data_len;

    /* Generate random IV */
    ret = generate_random_iv(header.IV, sizeof(header.IV));
    if (ret != 0) {
        os_free(cipher_data);
        return SS_CRYPTO_FAIL;
    }
    aes_gcm_crypt(1, header.IV, cipher_data, data_len, cipher_data);
#if 0
    /* Initialize mbedtls GCM context */
    mbedtls_gcm_context mbedtls_ctx;
    mbedtls_gcm_init(&mbedtls_ctx);

    /* Set encryption key */
    ret = mbedtls_gcm_setkey(&mbedtls_ctx, MBEDTLS_CIPHER_ID_AES, s_aes_gcm_key_val, AES_GCM_KEY_LENGTH * 8);
    if (ret != 0) {
        mbedtls_gcm_free(&mbedtls_ctx);
        os_free(cipher_data);
        return SS_FAIL;
    }

    /* Encrypt data and generate authentication tag using mbedtls_gcm_crypt_and_tag */
    ret = mbedtls_gcm_crypt_and_tag(&mbedtls_ctx,
                                  MBEDTLS_GCM_ENCRYPT,
                                  data_len,
                                  header.IV,
                                  sizeof(header.IV),
                                  NULL, /* No additional authenticated data */
                                  0, /* No additional authenticated data */
                                  data,
                                  cipher_data,
                                  sizeof(header.tag),
                                  header.tag);

    /* Release GCM context */
    mbedtls_gcm_free(&mbedtls_ctx);

    if (ret != 0) {
        os_free(cipher_data);
        return SS_FAIL;
    }
#endif
    /* Write to Flash */
    ret = tfm_flash_erase(addr);
    if (ret != 0) {
        os_free(cipher_data);
        return SS_FLASH_FAIL;
    }

    ret = tfm_flash_write(addr, (uint8_t *)&header, sizeof(ss_header_t));
    if (ret != 0) {
        os_free(cipher_data);
        return SS_FLASH_FAIL;
    }

    ret = tfm_flash_write(addr + sizeof(ss_header_t), cipher_data, data_len);
    if (ret != 0) {
        os_free(cipher_data);
        return SS_FLASH_FAIL;
    }
    /* Release memory */
    os_free(cipher_data);

    return SS_SUCCESS;
}

/**
 * @brief Secure storage read function
 *
 * @param type Data type (SS_TYPE_KEY/SS_TYPE_CERT/SS_TYPE_DATA)
 * @param index Data index (0-63)
 * @param data Output parameter to store the read data
 * @param data_len Input parameter: data buffer size; Output parameter: actual read data length
 *
 * @return 0 on success, other values on failure
 */
int ss_read(uint8_t type, uint32_t index, uint8_t *data, uint32_t *data_len)
{
    int ret = 0;
    uint32_t addr = 0;
    if (data == NULL) {
        return SS_INVALID_ARGS;
    }

    ret = get_ss_address(type, index, &addr);
    if (ret != 0) {
        return ret;
    }

    // Handle RAM storage (index 32-63)
    if (addr == 0) {
        uint32_t ram_index = index - SS_RAM_INDEX_START;
        ss_ram_data_t *storage = NULL;

        // Select corresponding RAM storage array
        switch (type) {
            case SS_TYPE_KEY:
                storage = &s_ram_key_storage[ram_index];
                break;
            case SS_TYPE_CERT:
                storage = &s_ram_cert_storage[ram_index];
                break;
            case SS_TYPE_DATA:
            default:
                return SS_INVALID_SLOT_TYPE;
        }

        // Check if data is valid
        if (!storage->valid || storage->data == NULL) {
            return SS_EMPTY_SLOT;
        }

        memcpy(data, storage->data, storage->data_len);
        *data_len = storage->data_len;

        return SS_SUCCESS;
    }

    // Handle Flash storage (index 0-31), keep existing decryption logic
    ss_header_t header;
    uint8_t *cipher_data = NULL;

    /* Read header from Flash */
    ret = tfm_flash_read(addr, (uint8_t *)&header, sizeof(ss_header_t));
    if (ret != 0) {
        return SS_FLASH_FAIL;
    }

    ret = verify_magic(&header);
    if (ret != 0) {
        return SS_FLASH_FAIL;
    }

    if (header.type != type) {
        return SS_INVALID_SLOT_TYPE;
    }

    cipher_data = (uint8_t *)os_malloc(header.cipher_len);
    if (cipher_data == NULL) {
        return SS_ALLOC_FAIL;
    }

    ret = tfm_flash_read(addr + sizeof(ss_header_t), cipher_data, header.cipher_len);
    if (ret != 0) {
        os_free(cipher_data);
        return SS_FLASH_FAIL;
    }

    aes_gcm_crypt(0, header.IV, cipher_data, header.cipher_len, cipher_data);

#if 0
    mbedtls_gcm_context mbedtls_ctx;
    /* Initialize mbedtls GCM context */
    mbedtls_gcm_init(&mbedtls_ctx);

    /* Set decryption key */
    ret = mbedtls_gcm_setkey(&mbedtls_ctx, MBEDTLS_CIPHER_ID_AES, &s_aes_gcm_key_val, AES_GCM_KEY_LENGTH * 8);
    if (ret != 0) {
        mbedtls_gcm_free(&mbedtls_ctx);
        os_free(cipher_data);
        return SS_FAIL+4;
    }

    /* Decrypt data and verify tag using mbedtls_gcm_auth_decrypt */
    ret = mbedtls_gcm_auth_decrypt(&mbedtls_ctx,
                                  header.cipher_len,
                                  header.IV,
                                  sizeof(header.IV),
                                  NULL, /* No additional authenticated data */
                                  0, /* No additional authenticated data */
                                  header.tag,
                                  sizeof(header.tag),
                                  cipher_data,
                                  data);
    if (ret != 0) {
        mbedtls_gcm_free(&mbedtls_ctx);
        os_free(cipher_data);
        return SS_FAIL+5;
    }

    /* Release GCM context */
    mbedtls_gcm_free(&mbedtls_ctx);

#endif

    memcpy(data, cipher_data, header.cipher_len);
    /* Release memory */
    os_free(cipher_data);

    /* Set actual read data length */
    *data_len = header.cipher_len;

    return SS_SUCCESS;
}

int ss_delete(uint8_t type, uint32_t index)
{
    int ret = 0;
    uint32_t addr = 0;

    ret = get_ss_address(type, index, &addr);
    if (ret != 0) {
        return ret;
    }

    flash_op_enable_ctrl(1,1);
    bk_flash_erase_sector(addr);
    flash_op_enable_ctrl(1,0);

    return SS_SUCCESS;
}

/**
 * @brief Key write function
 *
 * @param index Key index (0-63)
 * @param key Key data
 * @param key_len Key length
 * @param permission Permission (SS_PERM_READ_ONLY/SS_PERM_READ_WRITE)
 *
 * @return 0 on success, other values on failure
 */
__tfm_psa_secure_gateway_no_naked_attributes__ int ss_write_key(uint32_t index, uint8_t *key, uint32_t key_len)
{
    uint8_t permission = SS_PERM_READ_WRITE;
    return ss_write(SS_TYPE_KEY, index, key, key_len, permission);
}

/**
 * @brief Key read function
 *
 * @param index Key index (0-63)
 * @param key Output parameter to store the read key
 * @param key_len Input parameter: key buffer size; Output parameter: actual read key length
 *
 * @return 0 on success, other values on failure
 */
__tfm_psa_secure_gateway_no_naked_attributes__ int ss_read_key(uint32_t index, uint8_t *key, uint32_t *key_len)
{
    return ss_read(SS_TYPE_KEY, index, key, key_len);
}

/**
 * @brief Certificate write function
 *
 * @param index Certificate index (0-63)
 * @param cert Certificate data
 * @param cert_len Certificate length
 *
 * @return 0 on success, other values on failure
 */
__tfm_psa_secure_gateway_no_naked_attributes__ int ss_write_cert(uint32_t index, uint8_t *cert, uint32_t cert_len)
{
    uint8_t permission = SS_PERM_READ_WRITE;
    return ss_write(SS_TYPE_CERT, index, cert, cert_len, permission);
}

/**
 * @brief Certificate read function
 *
 * @param index Certificate index (0-63)
 * @param cert Output parameter to store the read certificate
 * @param cert_len Input parameter: cert buffer size; Output parameter: actual read certificate length
 *
 * @return 0 on success, other values on failure
 */
__tfm_psa_secure_gateway_no_naked_attributes__ int ss_read_cert(uint32_t index, uint8_t *cert, uint32_t *cert_len)
{
    return ss_read(SS_TYPE_CERT, index, cert, cert_len);
}

/**
 * @brief Certificate delete function
 *
 * @param index Certificate index (0-63)
 *
 * @return 0 on success, other values on failure
 */
__tfm_psa_secure_gateway_no_naked_attributes__ int ss_delete_cert(uint32_t index)
{
    int ret = 0;
    uint32_t addr = 0;

    /* Calculate storage address */
    ret = get_ss_address(SS_TYPE_CERT, index, &addr);
    if (ret != 0) {
        return ret;
    }

    // Handle RAM storage (index 32-63)
    if (addr == 0) {
        uint32_t ram_index;

        ram_index = index - SS_RAM_INDEX_START;

        // Free existing data if valid
        if (s_ram_cert_storage[ram_index].valid && s_ram_cert_storage[ram_index].data != NULL) {
            os_free(s_ram_cert_storage[ram_index].data);
        }

        // Mark as invalid
        s_ram_cert_storage[ram_index].data = NULL;
        s_ram_cert_storage[ram_index].valid = 0;
        s_ram_cert_storage[ram_index].data_len = 0;
        s_ram_cert_storage[ram_index].permission = 0;

        return SS_SUCCESS;
    } else {
        // Handle Flash storage (index 0-31) by erasing the partition
        ret = ss_delete(SS_TYPE_CERT, index);
    }

    return ret;
}

/**
 * @brief Data write function
 *
 * @param index Data index (0-63)
 * @param data Data content
 * @param data_len Data length
 *
 * @return 0 on success, other values on failure
 */
__tfm_psa_secure_gateway_no_naked_attributes__ int ss_write_data(uint32_t index, uint8_t *data, uint32_t data_len)
{
    uint8_t permission = SS_PERM_READ_WRITE;
    return ss_write(SS_TYPE_DATA, index, data, data_len, permission);
}

/**
 * @brief Data read function
 *
 * @param index Data index (0-63)
 * @param data Output parameter to store the read data
 * @param data_len Input parameter: data buffer size; Output parameter: actual read data length
 *
 * @return 0 on success, other values on failure
 */
__tfm_psa_secure_gateway_no_naked_attributes__  int ss_read_data(uint32_t index, uint8_t *data, uint32_t *data_len)
{
    return ss_read(SS_TYPE_DATA, index, data, data_len);
}

/**
 * @brief Data delete function
 *
 * @param index Data index (0-63)
 *
 * @return 0 on success, other values on failure
 */
__tfm_psa_secure_gateway_no_naked_attributes__ int ss_delete_data(uint32_t index)
{
    if (index >= CONFIG_SS_DATA_INDEX_MAX) {
        return SS_INVALID_SLOT_RANGE;
    }

    return ss_delete(SS_TYPE_DATA, index);

}