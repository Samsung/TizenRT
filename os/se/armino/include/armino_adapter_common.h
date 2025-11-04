#ifndef ARMINO_ADAPT_COMMON_H
#define ARMINO_ADAPT_COMMON_H

#include "psa/crypto_types.h" 
#include <tinyara/security_hal.h>

#define HAL_MAX_RANDOM_SIZE 256
#define HAL_MAX_ECP_KEY_SIZE 133
#define HAL_MAX_ECDSA_LEN 256
#define HAL_MAX_BUF_SIZE 2048
#define HAL_MAX_SHARED_SECRET_LEN 256

/* Max Factory Key and usable Key Index (Only allocate 8 slot) */
#define FACTORY_KEY_INDEX_MAX 32

/* Max RAM Key and Total Key Index */
#define KEY_STORAGE_INDEX_MAX 32
#define TOTAL_KEY_STORAGE_INDEX (FACTORY_KEY_INDEX_MAX + KEY_STORAGE_INDEX_MAX)

extern psa_key_id_t g_key_idx_record[TOTAL_KEY_STORAGE_INDEX];

static inline psa_key_id_t get_psa_key_id(uint32_t key_idx)
{
    return g_key_idx_record[key_idx];
}

/* Function to set psa key ID by index */
static inline void set_psa_key_id(uint32_t key_idx, psa_key_id_t psa_key_id)
{
    g_key_idx_record[key_idx] = psa_key_id;
}

extern uint32_t aes_get_padded_len(size_t data_len);
extern int aes_padding_adapter(hal_aes_algo mode, const uint8_t *input,  size_t input_len,  uint8_t *output, size_t *output_len, int operation) ;  // 0: pad, 1: unpad

#endif