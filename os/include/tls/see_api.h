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
/// @file see/see_api.h
/// @brief SEE api is supporting security api for using secure storage.

#ifndef __SEE_API_H
#define __SEE_API_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>

#include "../../arch/arm/src/s5j/soc/sss/mb_cmd_dh.h"
#include "../../arch/arm/src/s5j/soc/sss/mb_cmd_hash.h"
#include "../../arch/arm/src/s5j/soc/sss/mb_cmd_rsa.h"

#include "../../arch/arm/src/s5j/soc/sss/isp_define.h"
#include "../../arch/arm/src/s5j/soc/sss/isp_driver_hash.h"
#include "../../arch/arm/src/s5j/soc/sss/isp_driver_rng.h"
#include "../../arch/arm/src/s5j/soc/sss/isp_driver_secure_storage_factorykey.h"
#include "../../arch/arm/src/s5j/soc/sss/isp_driver_secure_storage.h"
#include "../../arch/arm/src/s5j/soc/sss/isp_driver_secure_storage_key.h"
#include "../../arch/arm/src/s5j/soc/sss/isp_driver_aes_securekey.h"
#include "../../arch/arm/src/s5j/soc/sss/isp_driver_dh_securekey.h"
#include "../../arch/arm/src/s5j/soc/sss/isp_driver_rsa_securekey.h"
#include "../../arch/arm/src/s5j/soc/sss/isp_driver_hmac_securekey.h"
#include "../../arch/arm/src/s5j/soc/sss/isp_driver_ecdsa_securekey.h"
#include "../../arch/arm/src/s5j/soc/sss/mb_cmd_secure_storage_data.h"
#include "../../arch/arm/src/s5j/soc/sss/isp_driver_error.h"
#include "../../arch/arm/src/s5j/soc/sss/isp_oid.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

//#define SEE_API_DEBUG
#if defined(SEE_API_DEBUG)
#define SEE_DEBUG printf
#else
#define SEE_DEBUG(...) do {} while (0)
#endif

/* Return value */
#define SEE_OK                   0
#define SEE_ERROR                1
#define SEE_ALLOC_ERROR          2
#define SEE_INVALID_INPUT_PARAMS 3

/* Key index */
#define SEE_AES_KEY_INDEX        1
#define SEE_RSA_PUBLIC_INDEX     1
#define SEE_RSA_PRIVATE_INDEX    2
#define SEE_ECC_PUBLIC_INDEX     3
#define SEE_ECC_PRIVATE_INDEX    4
#define SEE_DH_KEY_INDEX         5
#define SEE_HMAC_KEY_INDEX       6

#define KEY_RSA_LEN_1024    128
#define KEY_RSA_LEN_2048    256

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

#define ECC_KEY_BP192     ((ECC_KEY) | (0x51))
#define ECC_KEY_BP224     ((ECC_KEY) | (0x52))
#define ECC_KEY_BP256     ((ECC_KEY) | (0x53))
#define ECC_KEY_BP384     ((ECC_KEY) | (0x54))
#define ECC_KEY_BP512     ((ECC_KEY) | (0x55))

#define ECC_KEY_NIST192   ((ECC_KEY) | (0x21))
#define ECC_KEY_NIST224   ((ECC_KEY) | (0x22))
#define ECC_KEY_NIST256   ((ECC_KEY) | (0x23))
#define ECC_KEY_NIST384   ((ECC_KEY) | (0x24))
#define ECC_KEY_NIST512   ((ECC_KEY) | (0x25))

#define SEE_MAX_RANDOM_SIZE		(256)

/* cert type */
#define CERT_DER      0x01
#define CERT_PEM      0x02

/* index */
#define MAX_DATA_INDEX	0x20
#define MAX_KEY_INDEX	0x08
#define MAX_CERT_INDEX	0x08

/****************************************************************************
 * Public types
****************************************************************************/

typedef struct see_mutex_s {
	pthread_mutex_t mutex;
	int valid;
} see_mutex_t;

typedef struct cert_options {
	/* mandatory */
	unsigned int cert_index;
	unsigned int issuer_key_index;	/* index number of the issuer key       */
	unsigned int subject_key_index;	/* index number of the subject key      */
	unsigned int subject_pub_keylen;
	unsigned char *subject_pub_key;
	const char *subject_name;	/* subject name for certificate         */
	const char *issuer_name;	/* issuer name for certificate          */
	const char *not_before;		/* validity period not before           */
	const char *not_after;		/* validity period not after            */

	/* optional */
	const unsigned char *subject_pwd;	/* password for the subject key file    */
	const char *serial;			/* serial number string                 */
	unsigned char key_usage;	/* key usage flags                      */
	unsigned char ns_cert_type;	/* NS cert type                         */
	int is_ca;					/* is a CA certificate                  */
	int max_pathlen;			/* maximum CA path length               */
	int selfsign;
} cert_opt;

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

/****************************************************************************
 * Name: Key Manager
 *
 * Description:
 *   - Manage symmetric and asymmetric keys.
 *   - Managed keys is not exposed to USER SPACE
 *
 ****************************************************************************/
int see_generate_key(unsigned int key_type, unsigned int key_index, unsigned int key_len, unsigned int pukey_e);
int see_setup_key(unsigned char *key_der, unsigned int key_len, unsigned int key_type, unsigned int key_index);
int see_remove_key(unsigned int key_index, unsigned int key_type);

/****************************************************************************
 * Name: Authentication
 *
 * Description:
 *
 ****************************************************************************/
int see_generate_random(unsigned int *data, unsigned int len);
int see_generate_certificate(cert_opt opt, unsigned char *out_buf, unsigned int *out_buflen);
int see_get_certificate(unsigned char *cert, unsigned int *cert_len, unsigned int cert_index, unsigned int cert_type);
int see_set_certificate(unsigned char *cert, unsigned int cert_len, unsigned int cert_index, unsigned int cert_type);
int see_get_rsa_signature(struct sRSA_SIGN *rsa_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index);
int see_verify_rsa_signature(struct sRSA_SIGN *rsa_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index);
int see_get_ecdsa_signature(struct sECC_SIGN *ecc_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index);
int see_verify_ecdsa_signature(struct sECC_SIGN *ecc_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index);
int see_get_publickey(unsigned char *key_der, unsigned int *key_len);
int see_generate_dhm_params(struct sDH_PARAM *d_param, unsigned int key_index);
int see_compute_dhm_param(struct sDH_PARAM *d_param, unsigned int key_index, unsigned char *output, unsigned int *olen);

/****************************************************************************
 * Name: Secure Storage
 *
 * Description:
 *   - Data, Credential (exclude certificates) and Keys can be stored
 *     in Secure Storage.
 *   - Limitation
 *     a. Each File size : 200B
 *     b. The number of all files : 32 ea
 *
 ****************************************************************************/
int see_write_secure_storage(unsigned char *data, unsigned int data_len, unsigned int index);
int see_read_secure_storage(unsigned char *data, unsigned int *data_len, unsigned int index);

/****************************************************************************
 * Name: Encrypt and Decrypt
 *
 * Description:
 *   - Data can be encrypted and decrypted using a key in secure storage
 *
 ****************************************************************************/
int see_aes_encrypt(unsigned int key_index, struct sAES_PARAM *aes_param);
int see_aes_decrypt(unsigned int key_index, struct sAES_PARAM *aes_param);
int see_get_hmac(struct sHMAC_MSG *hmac_msg, unsigned char *output, unsigned int object_id, unsigned int key_index);
int see_get_hash(struct sHASH_MSG *h_param, unsigned char *hash, unsigned int mode);
int see_rsa_decryption(unsigned int key_index, unsigned int pad_type, unsigned char *output, unsigned int *outlen, unsigned char *input, unsigned int inlen);
int see_rsa_encryption(unsigned int key_index, unsigned int pad_type, unsigned char *output, unsigned int *outlen, unsigned char *input, unsigned int inlen);

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
#endif							/* __SEE_API_H */
