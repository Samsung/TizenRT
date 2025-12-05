
enum psa_storage_status_e{
	PS_STATUS_EMPTY = 0,
	PS_STATUS_INIT,
	PS_STATUS_ERR,
};

/* key storage */
#define KEY_STORAGE_NUM_MAX (16)
#define KEY_NAME_LENGTH_MAX (16)
/**
 *  @brief AES-CBC encrypt data function with a trustzone random key.
 *  @param[in]  names     key names
 *  @param[in]  pInBuf    input data
 *  @param[in]  in_len    length of the input data 
 *  @param[out] pOutBuf   encrypted data
 *  @param[in]  out_len   length of the output data 
 *  @note   Some details need to know before setting:
 *          - AES keylen must be 16bytes(128bits), 24bytes(192bits) or 32bytes(256bits).
 *
 *
 *  @return      value == 0     success
 *  @return      value < 0      fail(Refer to ERRNO)
 */
int tz_ps_aes_cbc_encrypt(char *names,uint8_t * pInBuf,const uint32_t in_len,uint8_t *pOutBuf,const uint32_t out_len);
/**
 *  @brief AES-CBC decrypt data function with a trustzone random key.
 *  @param[in]  names     key names
 *  @param[in]  pInBuf    input data
 *  @param[in]  in_len    length of the input data
 *  @param[out] pOutBuf   encrypted data 
 *  @param [in] out_len   length of the output data 
 *  @note   Some details need to know before setting:
 *          - AES keylen must be 16bytes(128bits), 24bytes(192bits) or 32bytes(256bits).
 *
 *
 *  @return      value == 0     success
 *  @return      value < 0      fail(Refer to ERRNO)
 */
int tz_ps_aes_cbc_decrypt(char *names,uint8_t * pInBuf,const uint32_t in_len,uint8_t *pOutBuf,const uint32_t out_len);
/**
 * @brief key protect storage
 *
 * key protect storage
 *
 * @attention
 * - This API is used to find key id by name
 *
 * @param
 * -names[in]: the key name
 * -id[out]  : if the key exists, return the keyid
 * @return
 *  - 0: succeed
 *  - others: other errors.
 */
int ps_key_get_id_by_name(char *names, uint32_t *id);

/**
 * @brief key protect storage
 *
 * key protect storage
 *
 * @attention
 * - This API is used to generate a key by attribute, and save the key name and ID in the ps module.
 *
 * @param
 * -names[in]  : specify key name, must less than KEY_NAME_LENGTH_MAX
 * -key_id[in] : specify key id, 0 on failure, generate number less than KEY_STORAGE_NUM_MAX
 * -type[in]   : The key type to write
 * -bits[in]   : The key size in bits.  Keys of size 0 are not supported
 * -usage[in]  : The usage flags to write, see macro psa_key_usage_t
 * -alg[in]    : The permitted algorithm policy to write, see macro psa_algorithm_t
 * @return
 *  - 0: succeed
 *  - others: other errors.
 */
int ps_key_generate_by_attribute(char *names, uint32_t key_id, uint16_t type, uint16_t bits, uint32_t usage, uint32_t alg);
/**
 * @brief key protect storage
 *
 * key protect storage
 *
 * @attention
 * - This API is used to destroy the key by id.
 *
 * @param
 * -id[in]  : key id
 * @return
 *  - 0: succeed
 *  - others: other errors.
 */
int ps_key_destroy_by_id(uint32_t id);
/**
 * @brief key protect storage
 *
 * key protect storage
 *
 * @attention
 * - This API is used to destroy the key by name.
 *
 * @param
 * -names[in]: the key name
 * @return
 *  - 0: succeed
 *  - others: other errors.
 */
int ps_key_destroy_by_names(char *names);