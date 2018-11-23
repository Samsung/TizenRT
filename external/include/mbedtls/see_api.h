/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
/// @file mbedtls/see_api.h
/// @brief SEE api is supporting security api for using secure storage.

#ifndef __SEE_API_H
#define __SEE_API_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sys/types.h>
#include <stdio.h>

#include "../../../os/arch/arm/src/s5j/sss/isp_custom.h"
#include "see_cert.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/*
 * optional definitions
 */
//#define SEE_API_DEBUG
//#define SEE_CHECK_SSS_BUSY

/*
 * Return values
 */
typedef enum {
	SEE_OK,

	SEE_ERROR,
	SEE_ALLOC_ERROR,
	SEE_INVALID_INPUT_PARAMS,
	SEE_INVALID_CERT_INDEX,
	SEE_INVALID_KEY_INDEX,
	SEE_INVALID_BUFFER_SIZE,
	SEE_MUTEX_INIT_ERROR,
	SEE_MUTEX_LOCK_ERROR,
	SEE_MUTEX_UNLOCK_ERROR,
	SEE_MUTEX_FREE_ERROR,

	SEE_WRITE_CERT_ERROR,
	SEE_READ_CERT_ERROR,
	SEE_GET_HASH_ERROR,
	SEE_GET_RANDOM_ERROR,
	SEE_ECDSA_SIGN_ERROR,
	SEE_ECDSA_VERIFY_ERROR,
	SEE_ECDH_COMPUTE_ERROR,
} see_error;

/*
 * Definitions
 */
#define SEE_MAX_RANDOM_SIZE		(256)
#define SEE_MAX_BUF_SIZE		(4096)
#define SEE_MAX_KEY_INDEX		(0x08)
#define SEE_MAX_CERT_INDEX		(0x08)
#define ECC_KEY				(0x040000)

#if defined(SEE_API_DEBUG)
#define SEE_DEBUG printf
#else
#define SEE_DEBUG(...) do {} while (0)
#endif

#if defined(SSS_CHECK_SSS_BUSY)
#define ISP_CHECKBUSY() while (isp_get_status()) {}
#else
#define ISP_CHECKBUSY()
#endif

#ifdef CONFIG_SUPPORT_FULL_SECURITY

/* Key type */
#define AES_KEY           0x010000
#define HMAC_KEY          0x020000
#define RSA_KEY           0x030000
#define ECC_KEY           0x040000

#define AES_KEY_128       ((AES_KEY)  | (0x1))
#define AES_KEY_256       ((AES_KEY)  | (0x2))
#define HMAC_KEY_128      ((HMAC_KEY) | (0x1))
#define HMAC_KEY_256      ((HMAC_KEY) | (0x2))
#define RSA_KEY_1024      ((RSA_KEY)  | (0xB1))
#define RSA_KEY_2048      ((RSA_KEY)  | (0xB2))

#define ECC_KEY_BP256     ((ECC_KEY) | (0x53))

#define ECC_KEY_NIST192   ((ECC_KEY) | (0x21))
#define ECC_KEY_NIST224   ((ECC_KEY) | (0x22))
#define ECC_KEY_NIST256   ((ECC_KEY) | (0x23))
#define ECC_KEY_NIST384   ((ECC_KEY) | (0x24))
#define ECC_KEY_NIST512   ((ECC_KEY) | (0x25))

/* cert type */
#define CERT_DER      0x01
#define CERT_PEM      0x02

/* index */
#define MAX_DATA_INDEX	32
#define MIN_DATA_INDEX	0
#define MAX_KEY_INDEX	8
#define MIN_KEY_INDEX	0
#define MAX_CERT_INDEX	6
#define MIN_CERT_INDEX	0

#define MAX_DATA_SIZE	(208)
#define SEE_BUF_MAX_SIZE	(4096)

#define SEE_IOTIVITY_MAXSIZE       (SEE_BUF_MAX_SIZE * 2)
#define SEE_IOTIVITY_SLOT          (7)

/**
 * @brief structure to contain options for making cert.
 */
struct cert_opt {
	unsigned int cert_index;	///<  certificate index
	unsigned int issuer_key_index;	///<  index number of the issuer key
	unsigned int subject_key_index;	///<  index number of the subject key
	unsigned int subject_pub_keylen;	///<  publickey length
	unsigned char *subject_pub_key;	///<  publickey data
	const char *subject_name;	///<  subject name for certificate
	const char *issuer_name;	///<  issuer name for certificate
	const char *not_before;		///<  validity period not before
	const char *not_after;		///<  validity period not after
	const unsigned char *subject_pwd;	///<  password for the subject key file
	const char *serial;			///<  serial number string
	unsigned char key_usage;	///<  key usage flags
	unsigned char ns_cert_type;	///<  NS cert type
	int is_ca;					///<  is a CA certificate
	int max_pathlen;			///<  maximum CA path length
	int selfsign;
};
#endif
/****************************************************************************
 * Public types
****************************************************************************/

typedef struct see_mutex_s {
	pthread_mutex_t mutex;
	int valid;
} see_mutex_t;

extern see_mutex_t m_handler;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/**
 * @brief see_init initializes security API context.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR will be returned.
 *
 */
int see_init(void);

/**
 * @brief see_free free security API context.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR will be returned.
 *
 */
int see_free(void);

/**
 * @brief Generate random number
 *
 * @param[out] data    Random number.
 * @param[in]  len     Random size (1 ~ 256).
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_generate_random(unsigned int *data, unsigned int len);

/**
 * @brief Get certificate in Secure Storage
 *
 * @param[out] cert        Cert data.
 * @param[out] cert_len    Cert length (1 ~ 4096).
 * @param[in]  cert_index  Index number of Secure Storage cert slot (0 ~ 5).
 * @param[in]  cert_type   Signature algorithm type (not used, CERT_PEM, CERT_DER)
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_get_certificate(unsigned char *cert, unsigned int *cert_len, unsigned int cert_index, unsigned int cert_type);

/**
 * @brief Get ECDSA signature from hash.
 *        Should set the target key using see_setup_key in Key Manager before this API.
 *
 * @param[in/out] ecc_sign   ECDSA signature.
 * @param[in]     hash       Hashed data for making signature.
 * @param[in]     hash_len   Length of hashed data.
 * @param[in]     key_index  Index number of Secure Storage for user key of ECDSA.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_get_ecdsa_signature(struct sECC_SIGN *ecc_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index);

/**
 * @brief Compute ecdh shared secret. P = d * Q
 *
 * @param[in]  ecc_pub    Structure for public key value.
 * @param[in]  key_index  Index number of SecureStorage key slot.
 * @param[out] output     Shared secret value.
 * @param[out] olen       Length of shared secret value.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_compute_ecdh_param(struct sECC_KEY *ecc_pub, unsigned int key_index, unsigned char *output, unsigned int *olen);

#ifdef CONFIG_SUPPORT_FULL_SECURITY
/**
 * @brief Set certificate in Secure Storage
 *
 * @param[out] cert        Cert data.
 * @param[in]  cert_len    Cert length (1 ~ 4096).
 * @param[in]  cert_index  Index number of Secure Storage cert slot (0 ~ 5)
 * @param[in]  cert_type   Signature algorithm type (not used, CERT_PEM, CERT_DER).
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_set_certificate(unsigned char *cert, unsigned int cert_len, unsigned int cert_index, unsigned int cert_type);

/****************************************************************************
 * Name: Secure Storage
 *
 * Description:
 *   - Data, Credential (exclude certificates) and Keys can be stored
 *     in Secure Storage.
 *   - Limitation
 *     a. Each File size : 208 Byte
 *     b. The number of all files : 32 ea
 *
 ****************************************************************************/

/**
 * @brief Write data in secure storage.
 *
 * @param[in]  data       Data.
 * @param[in]  data_len   Length of data (1 ~ 208).
 * @param[in]  key_index  Index number of SecureStorage data slot (0 ~ 31).
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_write_secure_storage(unsigned char *data, unsigned int data_len, unsigned int index);

/**
 * @brief Read data from secure storage.
 *
 * @param[out]     data       Data buf.
 * @param[in/out]  data_len   Sizeof buf or Length of data.
 * @param[in]      key_index  Index number of SecureStorage data slot.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_read_secure_storage(unsigned char *data, unsigned int *data_len, unsigned int index);

/**
 * @brief Get DA public key in Secure Storage.
 *
 * @param[out] key_der    Key data.
 * @param[out] key_len    Key length.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_get_publickey(unsigned char *key_der, unsigned int *key_len);

/**
 * @brief Get ECC public key in Secure Storage.
 *
 * @param[out] ecc_pub    Publickey buffer.
 * @param[in]  key_index  Index number of SecureStorage key slot (0 ~ 7 or factory keyid).
 * @param[in]  object_id  Ecc curve id. (reference : isp_oid.h)
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_get_ecc_publickey(struct sECC_KEY *ecc_pub, unsigned int key_index, unsigned int object_id);

/****************************************************************************
 * Name: Key Manager
 *
 * Description:
 *   - Manage symmetric and asymmetric keys.
 *   - Managed keys is not exposed to USER SPACE
 *
 ****************************************************************************/

/**
 * @brief Generate symmetric/asymmetric key and store it in Secure Storage.
 *
 * @param[in]  key_type    Key algorithm type (KEY_TYPE | OBJECT_ID).
 * @param[in]  key_index   Index number of SecureStorage key slot (0 ~ 7).
 * @param[in]  key_len     Length of key.
 * @param[in]  pukey_e     Generate exponent value When it is 0.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_generate_key(unsigned int key_type, unsigned int key_index, unsigned int key_len, unsigned int pukey_e);

/**
 * @brief Set a key that was generated from external
 *
 * @param[in]  key_der      Key data.
 * @param[in]  key_len      Key length.
 * @param[in]  key_type     Key algorithm type.
 * @param[in]  key_index    Index number of SecureStorage key slot.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_setup_key(unsigned char *key_der, unsigned int key_len, unsigned int key_type, unsigned int key_index);

/**
 * @brief Remove a key in Secure Storage
 *
 * @param[in]  key_index    Index number of SecureStorage key slot.
 * @param[in]  key_type     Key algorithm type.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_remove_key(unsigned int key_index, unsigned int key_type);

/****************************************************************************
 * Name: Encrypt and Decrypt
 *
 * Description:
 *   - Data can be encrypted and decrypted using a key in secure storage
 *
 ****************************************************************************/

/**
 * @brief Encrypt using AES key that was stored in Secure Storage.
 *
 * @param[in]     key_index  Index number of SecureStorage key slot.
 * @param[in/out] aes_param  All parameter about aes except key.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_aes_encrypt(unsigned int key_index, struct sAES_PARAM *aes_param);

/**
 * @brief Decrypt using AES key that was stored in Secure Storage.
 *
 * @param[in]     key_index  Index number of SecureStorage key slot.
 * @param[in/out] aes_param  All parameter about aes except key.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_aes_decrypt(unsigned int key_index, struct sAES_PARAM *aes_param);

/**
 * @brief Hash with message authentication code.
 *
 * @param[in/out]  hmac_msg   Structure contains msg address and length.
 * @param[out]     output     Generic HMAC checksum result.
 * @param[in]      object_id  Hash id.
 * @param[in]      key_index  Index number of SecureStorage key slot.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_get_hmac(struct sHMAC_MSG *hmac_msg, unsigned char *output, unsigned int object_id, unsigned int key_index);

/**
 * @brief Hash function.
 *
 * @param[in/out]  h_param   Structure contains msg address and length.
 * @param[out]     hash      Hashed data output.
 * @param[in]      mode      Selected hash algorithm id.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_get_hash(struct sHASH_MSG *h_param, unsigned char *hash, unsigned int mode);

/**
 * @brief Generate DH parameter and store secret in secure storage.
 *
 * @param[out] d_param    DH param struct, It is configured as p, q, g number.
 * @param[in]  key_index  Index number of SecureStorage key slot.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_generate_dhm_params(struct sDH_PARAM *d_param, unsigned int key_index);

/**
 * @brief Compute shared secret value = GXY ((G^Y)^X mod P).
 *
 * @param[out] d_param    DH param struct, It is configured as p, q, g number.
 * @param[in]  key_index  Index number of SecureStorage key slot.
 * @param[out] output     Shared secret value (GXY).
 * @param[out] olen       Length of shared secret value.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_compute_dhm_param(struct sDH_PARAM *d_param, unsigned int key_index, unsigned char *output, unsigned int *olen);

/**
 * @brief  Encrypt using RSA key that was stored in Secure Storage.
 *
 * @param[in]   key_index  Index number of SecureStorage key slot.
 * @param[in]   pad_type   Padding type.
 * @param[out]  output     Plain text output.
 * @param[out]  outlen     Length of output.
 * @param[in]   input      Cipher text input.
 * @param[in]   inlen      Length of cipher text input.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_rsa_decryption(unsigned int key_index, unsigned int pad_type, unsigned char *output, unsigned int *outlen, unsigned char *input, unsigned int inlen);

/**
 * @brief Decrypt using RSA key that was stored in Secure Storage.
 *
 * @param[in]   key_index  Index number of SecureStorage key slot.
 * @param[in]   pad_type   Padding type.
 * @param[out]  output     Cipher text output.
 * @param[out]  outlen     Length of output.
 * @param[in]   input      Plain text input.
 * @param[in]   inlen      Length of cipher text input.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_rsa_encryption(unsigned int key_index, unsigned int pad_type, unsigned char *output, unsigned int *outlen, unsigned char *input, unsigned int inlen);

/**
 * @brief Get RSA signature from hash.
 *        Should set the target key using see_setup_key in Key Manager before this API.
 *
 * @param[in/out] rsa_sign   RSA signature.
 * @param[in]     hash       Hashed data for making signature.
 * @param[in]     hash_len   Length of hashed data.
 * @param[in]     key_index  Index number of Secure Storage for user key of RSA.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_get_rsa_signature(struct sRSA_SIGN *rsa_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index);

/**
 * @brief Verify RSA signature.
 *        Should set the target key using see_setup_key in Key Manager before this API.
 *
 * @param[in/out] rsa_sign   RSA signature.
 * @param[in]     hash       Hashed data.
 * @param[in]     hash_len   Length of hashed data.
 * @param[in]     key_index  Index number of Secure Storage for user key of RSA.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_verify_rsa_signature(struct sRSA_SIGN *rsa_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index);

/**
 * @brief Verify ECDSA signature.
 *        Should set the target key using see_setup_key in Key Manager before this API.
 *
 * @param[in/out] ecc_sign   ECDSA signature.
 * @param[in]     hash       Hashed data.
 * @param[in]     hash_len   Length of hashed data.
 * @param[in]     key_index  Index number of Secure Storage for user key of ECDSA.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_verify_ecdsa_signature(struct sECC_SIGN *ecc_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index);

/****************************************************************************
 * Name: Iotivity Storage
 *
 * Description:
 *   - Data of large size for Iotivity can be stored in Secure Storage.
 *   - Limitation
 *     a. File size : 8 Kbyte
 *
 ****************************************************************************/
/**
 * @brief Write data in secure storage.
 *
 * @param[in]  buf        Data.
 * @param[in]  buflen     Length of data (1 ~ 8176 byte).
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_write_iotivity_storage(unsigned char *buf, unsigned int buflen);

/**
 * @brief Read data from secure storage.
 *
 * @param[out]  buf        Data buf.
 * @param[in]   buflen     Sizeof buf or Length of data.
 * @param[out]  olen       Data size.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_read_iotivity_storage(unsigned char *buf, unsigned int buflen, unsigned int *olen);

/**
 * @brief Generate certificate and store the generated certificate in Secure Storage.
 *        Only support RSA signed certificate.
 *
 * @param[in]  opt         Options for generate certificate.
 *                         Please check cert_opt structure before using it.
 * @param[out] out_buf     Cert data (PEM).
 * @param[out] out_buflen  Cert length.
 *
 * @return On success, SEE_OK will be returned.
 *         On failure, SEE_ERROR or SEE_INVALID_INPUT_PARAMS will be returned.
 */
int see_generate_certificate(struct cert_opt opt, unsigned char *out_buf, unsigned int *out_buflen);
#endif

/****************************************************************************
 * Name: Internal functions
 *
 * Description:
 *
 ****************************************************************************/
int see_mutex_init(see_mutex_t *m);
int see_mutex_free(see_mutex_t *m);
int see_mutex_lock(see_mutex_t *m);
int see_mutex_unlock(see_mutex_t *m);

unsigned int see_get_keyindex(unsigned int key_type);
unsigned int see_free_keyindex(unsigned int key_type, unsigned int key_index);
int see_check_certindex(unsigned int cert_index);
int see_check_keyindex(unsigned int key_index);
#endif							/* __SEE_API_H */
