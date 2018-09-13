/*
 * security_sdk_test.h
 *
 *  Created on: Sep 14, 2018
 *      Author: ydoh
 */

#ifndef APPS_EXAMPLES_SECURITY_SDK_SECURITY_SDK_TEST_H_
#define APPS_EXAMPLES_SECURITY_SDK_SECURITY_SDK_TEST_H_

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <artik_module.h>
#include <artik_security.h>

#define see_selfprintf printf
#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))

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

int security_sdk_aes_encrypt_decrypt(void);
int security_sdk_generate_key(void);
int security_sdk_set_compute_dhm_params(void);
int security_sdk_set_compute_ecdh_params(void);
int security_sdk_rsassa_sign_verify(void);
int security_sdk_get_hash(void);
int security_sdk_setup_remove_key(void);
int security_sdk_rsa_encrypt_decrypt(void);
int security_sdk_get_hmac(void);
int security_sdk_ecdsa_sign_verify(void);
int security_sdk_get_publickey(void);
void security_sdk_print_existence(void);
void security_sdk_init_existence(void);
void security_sdk_factory_key_test(int debug_mode);
int security_sdk_get_certificates(void);
int security_sdk_cert_publickey(void);
int security_sdk_serial(void);
int security_sdk_secure_storage(void);
int security_sdk_random(void);

#endif /* APPS_EXAMPLES_SECURITY_SDK_SECURITY_SDK_TEST_H_ */
