/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <tinyara/seclink.h>

#ifdef SECLINK_PATH
#undef SECLINK_PATH
#endif
#define SECLINK_PATH "/dev/seclink"

#define SL_LOG printf

#define SL_TAG "[SECLINK]"

#define SL_ERR(fd)														\
	do {																\
		SL_LOG(SL_TAG"[ERR:%s] %s %s:%d ret(%d) code(%s)\n",			\
			   SL_TAG, __FUNCTION__, __FILE__, __LINE__, fd, strerror(errno)); \
	} while (0)

#define SL_CALL(hnd, code, param)										\
	do {																\
		int res = ioctl(hnd->fd, code, (unsigned long)((uintptr_t)&param)); \
		if (res < 0) {													\
			SL_ERR(res);												\
			return SECLINK_ERROR;										\
		}																\
	} while (0)

#define SL_ENTER														\
	do {																\
		SL_LOG(SL_TAG"%s\t%s:%d\n", __FUNCTION__, __FILE__, __LINE__);	\
	} while (0)

struct _seclink_s_ {
	int fd;
};

#define SL_CLOSE(fd)							 \
	do {										 \
		close(fd);								 \
		fd = -1;								 \
	} while (0)


#define SL_CHECK_VALID(hnd)									 \
	do {													 \
		if (!hnd || ((struct _seclink_s_ *)hnd)->fd <= 0) {	 \
			return SECLINK_ERROR;							 \
		}													 \
	} while (0)

#define SL_SET_MEMPOOL(hdata)                                 \
	do {                                                      \
		memset(g_mem.mem, 0, SECLINK_MEM_MAX_SIZE);           \
		hdata->data = g_mem.mem;                              \
		hdata->data_len = SECLINK_MEM_MAX_SIZE;               \
	} while (0)

#define SL_SET_MEMPOOL_PRIV(hdata)                            \
	do {                                                      \
		memset(g_mem.mem_priv, 0, SECLINK_MEM_PRIV_MAX_SIZE); \
		hdata->priv = g_mem.mem_priv;                         \
		hdata->priv_len = SECLINK_MEM_PRIV_MAX_SIZE;          \
	} while (0)

/* global */
seclink_mem g_mem = {0, };

/*  Common */
int sl_init(sl_ctx *hnd)
{
	SL_ENTER;

	int fd = open(SECLINK_PATH, O_RDWR);
	if (fd < 0) {
		SL_ERR(fd);
		return SECLINK_ERROR;
	}

	struct _seclink_s_ *handle = (struct _seclink_s_ *)malloc(sizeof(struct _seclink_s_));
	if (!handle) {
		SL_ERR(fd);
		close(fd);
		return SECLINK_ERROR;
	}

	handle->fd = fd;
	*hnd = handle;

	g_mem.mem = (void *)malloc(SECLINK_MEM_MAX_SIZE);
	g_mem.mem_priv = (void *)malloc(SECLINK_MEM_PRIV_MAX_SIZE);

	return SECLINK_OK;
}

int sl_deinit(sl_ctx hnd)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	SL_CLOSE(sl->fd);

	free(sl);
	free(g_mem.mem);
	free(g_mem.mem_priv);

	return SECLINK_OK;
}

/*  key manager */
int sl_set_key(sl_ctx hnd, hal_key_type mode, uint32_t key_idx, hal_data *key, hal_data *prikey)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_key_info info = {mode, key_idx, key, prikey};
	struct seclink_req req = {.req_type.key = &info, 0};

	SL_CALL(sl, SECLINK_HAL_SETKEY, req);

	return req.res;
}

int sl_get_key(sl_ctx hnd, hal_key_type mode, uint32_t key_idx, _OUT_ hal_data *key)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);
	SL_SET_MEMPOOL(key);
	SL_SET_MEMPOOL_PRIV(key);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_key_info info = {mode, key_idx, key, NULL};
	struct seclink_req req = {.req_type.key = &info, 0};

	SL_CALL(sl, SECLINK_HAL_GETKEY, req);

	return req.res;
}

int sl_remove_key(sl_ctx hnd, hal_key_type mode, uint32_t key_idx)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_key_info info = {mode, key_idx, NULL, NULL};
	struct seclink_req req = {.req_type.key = &info, 0};

	SL_CALL(sl, SECLINK_HAL_REMOVEKEY, req);

	return req.res;
}

int sl_generate_key(sl_ctx hnd, hal_key_type mode, uint32_t key_idx)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_key_info info = {mode, key_idx, NULL, NULL};
	struct seclink_req req = {.req_type.key = &info, 0};

	SL_CALL(sl, SECLINK_HAL_GENERATEKEY, req);

	return req.res;
}


/*  Authenticate */
int sl_generate_random(sl_ctx hnd, uint32_t len, _OUT_ hal_data *random)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);
	SL_SET_MEMPOOL(random);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = len, -1, random, .auth_data.data = NULL};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINK_HAL_GENERATERANDOM, info);

	return req.res;
}

int sl_get_hash(sl_ctx hnd, hal_hash_type mode, hal_data *input, _OUT_ hal_data *hash)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);
	SL_SET_MEMPOOL(hash);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.hash_type = mode, -1, input, .auth_data.data = hash};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINK_HAL_GETHASH, req);

	return req.res;
}

int sl_get_hmac(sl_ctx hnd, hal_hmac_type mode, hal_data *input, uint32_t key_idx, _OUT_ hal_data *hmac)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);
	SL_SET_MEMPOOL(hmac);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.hmac_type = mode, key_idx, input, .auth_data.data = hmac};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINK_HAL_GETHMAC, req);

	return req.res;
}

int sl_rsa_sign_md(sl_ctx hnd, hal_rsa_mode mode, hal_data *hash, uint32_t key_idx, _OUT_ hal_data *sign)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);
	SL_SET_MEMPOOL(sign);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.rsa_type = mode, key_idx, hash, .auth_data.data = sign};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINK_HAL_RSASIGNMD, req);

	return req.res;
}

int sl_rsa_verify_md(sl_ctx hnd, hal_rsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.rsa_type = mode, key_idx, hash, .auth_data.data = sign};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINK_HAL_RSAVERIFYMD, req);

	return req.res;
}

int sl_ecdsa_sign_md(sl_ctx hnd, hal_ecdsa_mode mode, hal_data *hash, uint32_t key_idx, _OUT_ hal_data *sign)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);
	SL_SET_MEMPOOL(sign);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.ecdsa_type = mode, key_idx, hash, .auth_data.data = sign};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINK_HAL_ECDSASIGNMD, req);

	return req.res;
}

int sl_ecdsa_verify_md(sl_ctx hnd, hal_ecdsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.ecdsa_type = mode, key_idx, hash, .auth_data.data = sign};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINK_HAL_ECDSAVERIFYMD, req);

	return req.res;
}

int sl_dh_generate_param(sl_ctx hnd, uint32_t dh_idx, _INOUT_ hal_dh_data *dh_param)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);
	SL_SET_MEMPOOL(dh_param->pubkey);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, dh_idx, NULL, .auth_data.dh_param = dh_param};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINK_HAL_DHGENERATEPARAM, req);

	return req.res;
}

int sl_dh_compute_shared_secret(sl_ctx hnd, hal_dh_data *dh_param, uint32_t dh_idx, _OUT_ hal_data *shared_secret)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);
	SL_SET_MEMPOOL(shared_secret);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, dh_idx, shared_secret, .auth_data.dh_param = dh_param};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINK_HAL_DHCOMPUTESHAREDSECRET, req);

	return req.res;
}

int sl_ecdh_compute_shared_secret(sl_ctx hnd, hal_ecdh_data *ecdh_mode, uint32_t key_idx, _OUT_ hal_data *shared_secret)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);
	SL_SET_MEMPOOL(shared_secret);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, key_idx, shared_secret, .auth_data.ecdh_param = ecdh_mode};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINK_HAL_ECDHCOMPUTESHAREDSECRET, req);

	return req.res;
}

int sl_set_certificate(sl_ctx hnd, uint32_t cert_idx, hal_data *cert_in)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, cert_idx, cert_in, .auth_data.data = NULL};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINK_HAL_SETCERTIFICATE, req);

	return req.res;
}

int sl_get_certificate(sl_ctx hnd, uint32_t cert_idx, _OUT_ hal_data *cert_out)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);
	SL_SET_MEMPOOL(cert_out);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, cert_idx, cert_out, .auth_data.data = NULL};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINK_HAL_GETCERTIFICATE, req);

	return req.res;
}

int sl_remove_certificate(sl_ctx hnd, uint32_t cert_idx)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, cert_idx, 0, .auth_data.data = NULL};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINK_HAL_REMOVECERTIFICATE, req);

	return req.res;
}

int sl_get_factory_key(sl_ctx hnd, uint32_t key_idx, hal_data *key)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, key_idx, key, .auth_data.data = NULL};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINK_HAL_GETFACTORY_KEY, req);

	return req.res;
}

int sl_get_factory_cert(sl_ctx hnd, uint32_t cert_idx, hal_data *cert)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, cert_idx, cert, .auth_data.data = NULL};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINK_HAL_GETFACTORY_CERT, req);

	return req.res;
}

int sl_get_factory_data(sl_ctx hnd, uint32_t data_idx, hal_data *data)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, data_idx, data, .auth_data.data = NULL};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINK_HAL_GETFACTORY_DATA, req);

	return req.res;
}

/*  Crypto */
int sl_aes_encrypt(sl_ctx hnd, hal_data *dec_data, hal_aes_param *aes_param, uint32_t key_idx, _OUT_ hal_data *enc_data)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);
	SL_SET_MEMPOOL(enc_data);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_crypto_info info = {key_idx, dec_data, enc_data, aes_param, NULL};
	struct seclink_req req = {.req_type.crypto = &info, 0};

	SL_CALL(sl, SECLINK_HAL_AESENCRYPT, req);

	return req.res;
}

int sl_aes_decrypt(sl_ctx hnd, hal_data *enc_data, hal_aes_param *aes_param, uint32_t key_idx, _OUT_ hal_data *dec_data)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);
	SL_SET_MEMPOOL(dec_data);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_crypto_info info = {key_idx, enc_data, dec_data, aes_param, NULL};
	struct seclink_req req = {.req_type.crypto = &info, 0};

	SL_CALL(sl, SECLINK_HAL_AESDECRYPT, req);

	return req.res;
}

int sl_rsa_encrypt(sl_ctx hnd, hal_data *dec_data, hal_rsa_mode *rsa_mode, uint32_t key_idx, _OUT_ hal_data *enc_data)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);
	SL_SET_MEMPOOL(enc_data);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_crypto_info info = {key_idx, dec_data, enc_data, NULL, rsa_mode};
	struct seclink_req req = {.req_type.crypto = &info, 0};

	SL_CALL(sl, SECLINK_HAL_RSADECRYPT, req);

	return req.res;
}

int sl_rsa_decrypt(sl_ctx hnd, hal_data *enc_data, hal_rsa_mode *rsa_mode, uint32_t key_idx, _OUT_ hal_data *dec_data)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);
	SL_SET_MEMPOOL(dec_data);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_crypto_info info = {key_idx, enc_data, dec_data, NULL, rsa_mode};
	struct seclink_req req = {.req_type.crypto = &info, 0};

	SL_CALL(sl, SECLINK_HAL_RSADECRYPT, req);

	return req.res;
}


/*  Secure Storage */
int sl_write_storage(sl_ctx hnd, uint32_t ss_idx, hal_data *data)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_ss_info info = {ss_idx, data};
	struct seclink_req req = {.req_type.ss = &info, 0};

	SL_CALL(sl, SECLINK_HAL_WRITESTORAGE, req);

	return req.res;
}

int sl_read_storage(sl_ctx hnd, uint32_t ss_idx, _OUT_ hal_data *data)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);
	SL_SET_MEMPOOL(data);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_ss_info info = {ss_idx, data};
	struct seclink_req req = {.req_type.ss = &info, 0};

	SL_CALL(sl, SECLINK_HAL_READSTORAGE, req);

	return req.res;
}

int sl_delete_storage(sl_ctx hnd, uint32_t ss_idx)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_ss_info info = {ss_idx, NULL};
	struct seclink_req req = {.req_type.ss = &info, 0};

	SL_CALL(sl, SECLINK_HAL_DELETESTORAGE, req);

	return req.res;
}
