/*
 * Copyright (c) 2015-2020, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef CRYPTO_MOD_H
#define CRYPTO_MOD_H

/* Return values */
enum crypto_ret_value {
	CRYPTO_SUCCESS = 0,
	CRYPTO_ERR_INIT,
	CRYPTO_ERR_HASH,
	CRYPTO_ERR_SIGNATURE,
	CRYPTO_ERR_DECRYPTION,
	CRYPTO_ERR_UNKNOWN
};

#define CRYPTO_MAX_IV_SIZE		16U
#define CRYPTO_MAX_TAG_SIZE		16U

/* Decryption algorithm */
enum crypto_dec_algo {
	CRYPTO_GCM_DECRYPT = 0
};

/*
 * Cryptographic library descriptor
 */
typedef struct crypto_lib_desc_s {
	const char *name;

	/* Initialize library. This function is not expected to fail. All errors
	 * must be handled inside the function, asserting or panicing in case of
	 * a non-recoverable error */
	void (*init)(void);

	/* Verify a digital signature. Return one of the
	 * 'enum crypto_ret_value' options */
	int (*verify_signature)(void *data_ptr, unsigned int data_len,
				void *sig_ptr, unsigned int sig_len,
				void *sig_alg, unsigned int sig_alg_len,
				void *pk_ptr, unsigned int pk_len);

	/* Verify a hash. Return one of the 'enum crypto_ret_value' options */
	int (*verify_hash)(void *data_ptr, unsigned int data_len,
			   void *digest_info_ptr, unsigned int digest_info_len);

#if MEASURED_BOOT
	/* Calculate a hash. Return hash value */
	int (*calc_hash)(unsigned int alg, void *data_ptr,
			 unsigned int data_len, unsigned char *output);
#endif /* MEASURED_BOOT */

	/*
	 * Authenticated decryption. Return one of the
	 * 'enum crypto_ret_value' options.
	 */
	int (*auth_decrypt)(enum crypto_dec_algo dec_algo, void *data_ptr,
			    size_t len, const void *key, unsigned int key_len,
			    unsigned int key_flags, const void *iv,
			    unsigned int iv_len, const void *tag,
			    unsigned int tag_len);
} crypto_lib_desc_t;

/* Public functions */
void crypto_mod_init(void);
int crypto_mod_verify_signature(void *data_ptr, unsigned int data_len,
				void *sig_ptr, unsigned int sig_len,
				void *sig_alg_ptr, unsigned int sig_alg_len,
				void *pk_ptr, unsigned int pk_len);
int crypto_mod_verify_hash(void *data_ptr, unsigned int data_len,
			   void *digest_info_ptr, unsigned int digest_info_len);
int crypto_mod_auth_decrypt(enum crypto_dec_algo dec_algo, void *data_ptr,
			    size_t len, const void *key, unsigned int key_len,
			    unsigned int key_flags, const void *iv,
			    unsigned int iv_len, const void *tag,
			    unsigned int tag_len);

#if MEASURED_BOOT
int crypto_mod_calc_hash(unsigned int alg, void *data_ptr,
			 unsigned int data_len, unsigned char *output);

/* Macro to register a cryptographic library */
#define REGISTER_CRYPTO_LIB(_name, _init, _verify_signature, _verify_hash, \
			    _calc_hash, _auth_decrypt) \
	const crypto_lib_desc_t crypto_lib_desc = { \
		.name = _name, \
		.init = _init, \
		.verify_signature = _verify_signature, \
		.verify_hash = _verify_hash, \
		.calc_hash = _calc_hash, \
		.auth_decrypt = _auth_decrypt \
	}
#else
#define REGISTER_CRYPTO_LIB(_name, _init, _verify_signature, _verify_hash, \
			    _auth_decrypt) \
	const crypto_lib_desc_t crypto_lib_desc = { \
		.name = _name, \
		.init = _init, \
		.verify_signature = _verify_signature, \
		.verify_hash = _verify_hash, \
		.auth_decrypt = _auth_decrypt \
	}
#endif	/* MEASURED_BOOT */

extern const crypto_lib_desc_t crypto_lib_desc;

#endif /* CRYPTO_MOD_H */
