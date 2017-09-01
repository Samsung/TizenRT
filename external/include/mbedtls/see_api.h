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

#include "../../arch/arm/src/s5j/sss/mb_cmd_dh.h"
#include "../../arch/arm/src/s5j/sss/mb_cmd_hash.h"
#include "../../arch/arm/src/s5j/sss/mb_cmd_rsa.h"

#include "../../arch/arm/src/s5j/sss/isp_define.h"
#include "../../arch/arm/src/s5j/sss/isp_driver_hash.h"
#include "../../arch/arm/src/s5j/sss/isp_driver_rng.h"
#include "../../arch/arm/src/s5j/sss/isp_driver_secure_storage_factorykey.h"
#include "../../arch/arm/src/s5j/sss/isp_driver_secure_storage.h"
#include "../../arch/arm/src/s5j/sss/isp_driver_secure_storage_key.h"
#include "../../arch/arm/src/s5j/sss/isp_driver_dh_securekey.h"
#include "../../arch/arm/src/s5j/sss/isp_driver_rsa_securekey.h"
#include "../../arch/arm/src/s5j/sss/isp_driver_hmac_securekey.h"
#include "../../arch/arm/src/s5j/sss/isp_driver_ecdsa_securekey.h"
#include "../../arch/arm/src/s5j/sss/mb_cmd_secure_storage_data.h"
#include "../../arch/arm/src/s5j/sss/isp_driver_error.h"
#include "../../arch/arm/src/s5j/sss/isp_oid.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/*
 * optional definitions
 */
//#define SEE_API_DEBUG
//#define SEE_CHECK_SSS_BUSY
//#define SEE_SUPPORT_USERCERT
//#define SEE_SUPPORT_USERKEY

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

/****************************************************************************
 * Name: Authentication
 *
 * Description:
 *
 ****************************************************************************/
int see_generate_random(unsigned int *data, unsigned int len);
int see_get_certificate(unsigned char *cert, unsigned int *cert_len, unsigned int cert_index, unsigned int cert_type);
int see_set_certificate(unsigned char *cert, unsigned int cert_len, unsigned int cert_index, unsigned int cert_type);
int see_get_ecdsa_signature(struct sECC_SIGN *ecc_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index);
int see_verify_ecdsa_signature(struct sECC_SIGN *ecc_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index);
int see_get_hash(struct sHASH_MSG *h_param, unsigned char *hash, unsigned int mode);
int see_compute_ecdh_param(struct sECC_KEY *ecc_pub, unsigned int key_index, unsigned char *output, unsigned int *olen);

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
