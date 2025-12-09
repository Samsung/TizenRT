#pragma once

#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <os/os.h>
#include "cli.h"
#include "cli_nist.h"

#include <tinyara/config.h>
#include <tinyara/security_hal.h>
#include <stdint.h>

// TODO ADD CASE callback func
typedef enum {
    NIST_HASH  = 0x00,
    NIST_HASH_MONTE = 0x01,
    NIST_AES   = 0x10,
    NIST_AES_ECB = 0x11,
    NIST_AES_CBC = 0x12,
    NIST_AES_CFB = 0x13,
    NIST_AES_MCT_ECB = 0x14,
    NIST_AES_MCT_CBC = 0x15,
    NIST_AES_MCT_CFB = 0x16,
    NIST_HMAC  = 0x20,
    NIST_ECDSA = 0x30,
    NIST_ECDSA_KEY_PAIR = 0x31,
    NIST_ECDSA_PUB_KEY = 0x32,
    NIST_ECDSA_SIGN = 0x33,
    NIST_ECDSA_VERIFY = 0x34,
    NIST_RSA   = 0x40,
    NIST_RSA_KEY_PAIR = 0x41,
    NIST_RSA_PUB_KEY = 0x42,
    NIST_RSA_SIGN_V15 = 0x43,
    NIST_RSA_SIGN_PSS = 0x44,
    NIST_RSA_VERIFY_V15 = 0x45,
    NIST_RSA_VERIFY_PSS = 0x46,
    NIST_RSA_ENC = 0x47,
    NIST_RSA_DEC = 0x48,
    NIST_DH    = 0x50,
    NIST_ECDH  = 0x60,
    NIST_RNG   = 0x70,
    NIST_GCM_DEC   = 0x80,
    NIST_GCM_ENC = 0x81,
    NIST_UNDEFINE = 0xff,
} nist_algo_type_t;


void hal_nist_free_buffer(hal_data *data);
int hal_nist_malloc_buffer(hal_data *data, int buf_len);
void hal_nist_init_buffer(hal_data *data);
void hal_nist_xor_bit_arrays(const uint8_t *array1, const uint8_t *array2, uint8_t *result, uint32_t len);
void print_hex(uint8_t *data, uint32_t len);


