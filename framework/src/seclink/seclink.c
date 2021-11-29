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

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#ifndef LINUX
#include <debug.h>
#endif
#include <sys/ioctl.h>
#include <tinyara/seclink.h>
#include <tinyara/seclink_drv.h>
#include <tinyara/security_hal.h>
#include "seclink_log.h"

#define TAG "[SLC]"

#define SL_ERR(fd)                              \
	do {                                        \
		if (fd == -1) {                         \
			SLC_LOGE(TAG, "\n");                \
		} else {                                \
			SLC_LOGE(TAG, "ret(%d) code(%s)\n", \
					 fd, strerror(errno));      \
		}                                       \
	} while (0)

#ifdef LINUX
extern int sl_post_msg(int fd, int cmd, unsigned long arg);
#define ioctl sl_post_msg
#endif

#define SL_CALL(hnd, code, param)                                             \
	do {                                                                      \
		int i_res = ioctl(hnd->fd, code, (unsigned long)((uintptr_t)&param)); \
		if (i_res < 0) {                                                      \
			SL_ERR(i_res);                                                    \
			return SECLINK_ERROR;                                             \
		}                                                                     \
	} while (0)

#define SL_CALL2(hnd, code, param, error)                                     \
	do {                                                                      \
		int i_res = ioctl(hnd->fd, code, (unsigned long)((uintptr_t)&param)); \
		if (i_res < 0) {                                                      \
			SL_ERR(i_res);                                                    \
			error;                                                            \
			return SECLINK_ERROR;                                             \
		}                                                                     \
	} while (0)

#define SL_CALL_NORET(hnd, code, param)                                       \
	do {                                                                      \
		int i_res = ioctl(hnd->fd, code, (unsigned long)((uintptr_t)&param)); \
		if (i_res < 0) {                                                      \
			SL_ERR(i_res);                                                    \
		}                                                                     \
	} while (0)

#define SL_CLOSE(fd) \
	do {             \
		close(fd);   \
		fd = -1;     \
	} while (0)

#define SL_CHECK_VALID(hnd)                                 \
	do {                                                    \
		if (!hnd || ((struct _seclink_s_ *)hnd)->fd <= 0) { \
			return SECLINK_ERROR;                           \
		}                                                   \
	} while (0)

#define SL_FREE_HANDLE(handle) \
	do {                       \
		SL_CLOSE(handle->fd);  \
		free(handle);          \
	} while (0)

static const int g_sec_res_table[] = {
#ifdef SECLINK_RES_MAP_TABLE
#undef SECLINK_RES_MAP_TABLE
#endif
#define SECLINK_RES_MAP_TABLE(sres, hres) sres,
#include "seclink_res_table.h"
};

static const hal_result_e g_hal_res_table[] = {
#ifdef SECLINK_RES_MAP_TABLE
#undef SECLINK_RES_MAP_TABLE
#endif
#define SECLINK_RES_MAP_TABLE(sres, hres) hres,
#include "seclink_res_table.h"
};

/*
 * Structure
 */
struct _seclink_s_ {
	int fd;
};

static inline int _sl_convert_res(hal_result_e res)
{
	int list_size = sizeof(g_hal_res_table) / sizeof(hal_result_e);
	for (int i = 0; i < list_size; i++) {
		if (res == g_hal_res_table[i]) {
			return g_sec_res_table[i];
		}
	}
	assert(0); // shouldn't be happened
	return SECLINK_ERROR;
}

/*  Common */
int sl_init(sl_ctx *hnd)
{
	SLC_LOGI(TAG, "-->\n");

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

	struct seclink_req req = {.req_type.comm = NULL, 0};
	SL_CALL2(handle, SECLINKIOC_INIT, req, SL_FREE_HANDLE(handle));
	if (req.res != HAL_SUCCESS) {
		close(fd);
		free(handle);
		return SECLINK_ERROR;
	}

	*hnd = handle;
	return SECLINK_OK;
}

int sl_deinit(sl_ctx hnd)
{
	SLC_LOGI(TAG, "-->\n");

	if (!hnd || ((struct _seclink_s_ *)hnd)->fd <= 0) {
		SL_ERR(-1);
		return -1;
	}
	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;

	struct seclink_req req = {.req_type.comm = NULL, 0};
	SL_CALL_NORET(sl, SECLINKIOC_DEINIT, req);
	if (req.res != HAL_SUCCESS) {
		SLC_LOGE(TAG, "fail to deinit");
	}

	SL_CLOSE(sl->fd);

	free(sl);

	return SECLINK_OK;
}

/*  key manager */
int sl_set_key(sl_ctx hnd,
			   hal_key_type mode,
			   uint32_t key_idx,
			   hal_data *key,
			   hal_data *prikey)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) mode(%d) idx(%d)\n", hnd, mode, key_idx);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_key_info info = {mode, key_idx, key, prikey};
	struct seclink_req req = {.req_type.key = &info, 0};

	SL_CALL(sl, SECLINKIOC_SETKEY, req);
	return _sl_convert_res(req.res);
}

int sl_get_key(sl_ctx hnd,
			   hal_key_type mode,
			   uint32_t key_idx,
			   _OUT_ hal_data *key)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) mode(%d) idx(%d)\n", hnd, mode, key_idx);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_key_info info = {mode, key_idx, key, NULL};
	struct seclink_req req = {.req_type.key = &info, 0};

	SL_CALL(sl, SECLINKIOC_GETKEY, req);
	return _sl_convert_res(req.res);
}

int sl_remove_key(sl_ctx hnd,
				  hal_key_type mode,
				  uint32_t key_idx)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) mode(%d) idx(%d)\n", hnd, mode, key_idx);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_key_info info = {mode, key_idx, NULL, NULL};
	struct seclink_req req = {.req_type.key = &info, 0};

	SL_CALL(sl, SECLINKIOC_REMOVEKEY, req);
	return _sl_convert_res(req.res);
}

int sl_generate_key(sl_ctx hnd,
					hal_key_type mode,
					uint32_t key_idx)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) mode(%d) idx(%d)\n", hnd, mode, key_idx);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_key_info info = {mode, key_idx, NULL, NULL};
	struct seclink_req req = {.req_type.key = &info, 0};

	SL_CALL(sl, SECLINKIOC_GENERATEKEY, req);
	return _sl_convert_res(req.res);
}

/*  Authenticate */
int sl_generate_random(sl_ctx hnd,
					   uint32_t len,
					   _OUT_ hal_data *random)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) len(%d)\n", hnd, len);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = len, -1, random, .auth_data.data = NULL};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_GENERATERANDOM, req);
	return _sl_convert_res(req.res);
}

int sl_get_hash(sl_ctx hnd,
				hal_hash_type mode,
				hal_data *input,
				_OUT_ hal_data *hash)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) mode(%d)\n", hnd, mode);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.hash_type = mode, -1, input, .auth_data.data = hash};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_GETHASH, req);
	return _sl_convert_res(req.res);
}

int sl_get_hmac(sl_ctx hnd,
				hal_hmac_type mode,
				hal_data *input,
				uint32_t key_idx,
				_OUT_ hal_data *hmac)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) mode(%d)\n", hnd, mode);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.hmac_type = mode, key_idx, input, .auth_data.data = hmac};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_GETHMAC, req);
	return _sl_convert_res(req.res);
}

int sl_rsa_sign_md(sl_ctx hnd,
				   hal_rsa_mode mode,
				   hal_data *hash,
				   uint32_t key_idx,
				   _OUT_ hal_data *sign)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) mode(%d %d %d %d) idx(%d)\n",
			 hnd, mode.rsa_a, mode.hash_t, mode.mgf, mode.salt_byte_len, key_idx);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.rsa_type = mode, key_idx, hash, .auth_data.data = sign};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_RSASIGNMD, req);
	return _sl_convert_res(req.res);
}

int sl_rsa_verify_md(sl_ctx hnd,
					 hal_rsa_mode mode,
					 hal_data *hash,
					 hal_data *sign,
					 uint32_t key_idx)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) mode(%d %d %d %d) idx(%d)\n",
			 hnd, mode.rsa_a, mode.hash_t, mode.mgf, mode.salt_byte_len, key_idx);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.rsa_type = mode, key_idx, hash, .auth_data.data = sign};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_RSAVERIFYMD, req);
	return _sl_convert_res(req.res);
}

int sl_ecdsa_sign_md(sl_ctx hnd,
					 hal_ecdsa_mode mode,
					 hal_data *hash,
					 uint32_t key_idx,
					 _OUT_ hal_data *sign)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) mode(%d %d) idx(%d)\n",
			 hnd, mode.curve, mode.hash_t, key_idx);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.ecdsa_type = mode, key_idx, hash, .auth_data.data = sign};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_ECDSASIGNMD, req);
	return _sl_convert_res(req.res);
}

int sl_ecdsa_verify_md(sl_ctx hnd,
					   hal_ecdsa_mode mode,
					   hal_data *hash,
					   hal_data *sign,
					   uint32_t key_idx)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) mode(%d %d) idx(%d)\n",
			 hnd, mode.curve, mode.hash_t, key_idx);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.ecdsa_type = mode,
									 key_idx,
									 hash,
									 .auth_data.data = sign};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_ECDSAVERIFYMD, req);

	return _sl_convert_res(req.res);
}

int sl_dh_generate_param(sl_ctx hnd,
						 uint32_t dh_idx,
						 _INOUT_ hal_dh_data *dh_param)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) dh_type(%d) idx(%d)\n", hnd, dh_param->mode, dh_idx);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, dh_idx, NULL, .auth_data.dh_param = dh_param};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_DHGENERATEPARAM, req);
	return _sl_convert_res(req.res);
}

int sl_dh_compute_shared_secret(sl_ctx hnd,
								hal_dh_data *dh_param,
								uint32_t dh_idx,
								_OUT_ hal_data *shared_secret)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) dh_type(%d) idx(%d)\n", hnd, dh_param->mode, dh_idx);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, dh_idx, shared_secret, .auth_data.dh_param = dh_param};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_DHCOMPUTESHAREDSECRET, req);
	return _sl_convert_res(req.res);
}

int sl_ecdh_compute_shared_secret(sl_ctx hnd,
								  hal_ecdh_data *ecdh_mode,
								  uint32_t key_idx,
								  _OUT_ hal_data *shared_secret)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) curve(%d) idx(%d)\n", hnd, ecdh_mode->curve, key_idx);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, key_idx, shared_secret, .auth_data.ecdh_param = ecdh_mode};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_ECDHCOMPUTESHAREDSECRET, req);
	return _sl_convert_res(req.res);
}

int sl_set_certificate(sl_ctx hnd, uint32_t cert_idx, hal_data *cert_in)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) idx(%d)\n", hnd, cert_idx);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, cert_idx, cert_in, .auth_data.data = NULL};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_SETCERTIFICATE, req);
	return _sl_convert_res(req.res);
}

int sl_get_certificate(sl_ctx hnd, uint32_t cert_idx, _OUT_ hal_data *cert_out)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) idx(%d)\n", hnd, cert_idx);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, cert_idx, cert_out, .auth_data.data = NULL};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_GETCERTIFICATE, req);
	return _sl_convert_res(req.res);
}

int sl_remove_certificate(sl_ctx hnd, uint32_t cert_idx)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) idx(%d)\n", hnd, cert_idx);
	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, cert_idx, 0, .auth_data.data = NULL};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_REMOVECERTIFICATE, req);
	return _sl_convert_res(req.res);
}

int sl_get_factory_key(sl_ctx hnd, uint32_t key_idx, hal_data *key)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) idx(%d)\n", hnd, key_idx);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, key_idx, key, .auth_data.data = NULL};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_GETFACTORY_KEY, req);
	return _sl_convert_res(req.res);
}

int sl_get_factory_cert(sl_ctx hnd, uint32_t cert_idx, hal_data *cert)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) idx(%d)\n", hnd, cert_idx);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, cert_idx, cert, .auth_data.data = NULL};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_GETFACTORY_CERT, req);
	return _sl_convert_res(req.res);
}

int sl_get_factory_data(sl_ctx hnd, uint32_t data_idx, hal_data *data)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) idx(%d)\n", hnd, data_idx);
	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, data_idx, data, .auth_data.data = NULL};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_GETFACTORY_DATA, req);
	return _sl_convert_res(req.res);
}

/*  Crypto */
int sl_aes_encrypt(sl_ctx hnd, hal_data *dec_data,
				   hal_aes_param *aes_param,
				   uint32_t key_idx,
				   _OUT_ hal_data *enc_data)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) idx(%d) aes mode(%d) iv len(%d)\n",
			 hnd, key_idx, aes_param->mode, aes_param->iv_len);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_crypto_info info = {key_idx, dec_data, enc_data, aes_param, NULL};
	struct seclink_req req = {.req_type.crypto = &info, 0};

	SL_CALL(sl, SECLINKIOC_AESENCRYPT, req);
	return _sl_convert_res(req.res);
}

int sl_aes_decrypt(sl_ctx hnd, hal_data *enc_data,
				   hal_aes_param *aes_param,
				   uint32_t key_idx,
				   _OUT_ hal_data *dec_data)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) idx(%d) aes mode(%d) iv len(%d)\n",
			 hnd, key_idx, aes_param->mode, aes_param->iv_len);
	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_crypto_info info = {key_idx, enc_data, dec_data, aes_param, NULL};
	struct seclink_req req = {.req_type.crypto = &info, 0};

	SL_CALL(sl, SECLINKIOC_AESDECRYPT, req);
	return _sl_convert_res(req.res);
}

int sl_rsa_encrypt(sl_ctx hnd, hal_data *dec_data,
				   hal_rsa_mode *rsa_mode,
				   uint32_t key_idx,
				   _OUT_ hal_data *enc_data)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) idx(%d) RSA mode(%d) hash(%d) mgf(%d) salt len(%d)\n",
			 hnd, key_idx, rsa_mode->rsa_a, rsa_mode->hash_t, rsa_mode->mgf, rsa_mode->salt_byte_len);
	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_crypto_info info = {key_idx, dec_data, enc_data, NULL, rsa_mode};
	struct seclink_req req = {.req_type.crypto = &info, 0};

	SL_CALL(sl, SECLINKIOC_RSAENCRYPT, req);
	return _sl_convert_res(req.res);
}

int sl_rsa_decrypt(sl_ctx hnd,
				   hal_data *enc_data,
				   hal_rsa_mode *rsa_mode,
				   uint32_t key_idx,
				   _OUT_ hal_data *dec_data)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) idx(%d) RSA mode(%d) hash(%d) mgf(%d) salt len(%d)\n",
			 hnd, key_idx, rsa_mode->rsa_a, rsa_mode->hash_t, rsa_mode->mgf, rsa_mode->salt_byte_len);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_crypto_info info = {key_idx, enc_data, dec_data, NULL, rsa_mode};
	struct seclink_req req = {.req_type.crypto = &info, 0};

	SL_CALL(sl, SECLINKIOC_RSADECRYPT, req);
	return _sl_convert_res(req.res);
}

/*  Secure Storage */
int sl_write_storage(sl_ctx hnd, uint32_t ss_idx, hal_data *data)
{
	SL_CHECK_VALID(hnd);
	if (!data || !data->data) {
		return SECLINK_INVALID_ARGS;
	}
	SLC_LOGI(TAG, "--> hnd(%p) size(%d) idx(%d)\n", hnd, data->data_len, ss_idx);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_ss_info info = {ss_idx, data};
	struct seclink_req req = {.req_type.ss = &info, 0};

	SL_CALL(sl, SECLINKIOC_WRITESTORAGE, req);
	return _sl_convert_res(req.res);
}

int sl_read_storage(sl_ctx hnd, uint32_t ss_idx, _OUT_ hal_data *data)
{
	SL_CHECK_VALID(hnd);
	if (!data || !data->data) {
		return SECLINK_INVALID_ARGS;
	}
	SLC_LOGI(TAG, "--> hnd(%p) idx(%d)\n", hnd, ss_idx);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_ss_info info = {ss_idx, data};
	struct seclink_req req = {.req_type.ss = &info, 0};

	SL_CALL(sl, SECLINKIOC_READSTORAGE, req);
	return _sl_convert_res(req.res);
}

int sl_delete_storage(sl_ctx hnd, uint32_t ss_idx)
{
	SL_CHECK_VALID(hnd);
	SLC_LOGI(TAG, "--> hnd(%p) idx(%d)\n", hnd, ss_idx);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_ss_info info = {ss_idx, NULL};
	struct seclink_req req = {.req_type.ss = &info, 0};

	SL_CALL(sl, SECLINKIOC_DELETESTORAGE, req);
	return _sl_convert_res(req.res);
}

char *sl_strerror(int error)
{
	switch (error) {
	case SECLINK_OK:
		return "It's ok";
	case SECLINK_ERROR:
		return "Error";
	case SECLINK_INVALID_ARGS:
		return "Invalid arguments";
	case SECLINK_INVALID_REQUEST:
		return "Invalid request";
	case SECLINK_INVALID_SLOT_RANGE:
		return "Invalid slot range";
	case SECLINK_INVALID_SLOT_TYPE:
		return "Invalid slot type";
	case SECLINK_EMPTY_SLOT:
		return "Empty slot";
	case SECLINK_BAD_KEY:
		return "Bad key";
	case SECLINK_BAD_KEY_PAIR:
		return "Bad key pair";
	case SECLINK_BAD_CERT:
		return "Bad certificate ";
	case SECLINK_BAD_CERTKEY_PAIR:
		return "Bad certificate and key pair";
	case SECLINK_NOT_ENOUGH_MEMORY:
		return "Memory is not enough";
	case SECLINK_ALLOC_FAIL:
		return "Allocation fail in SE";
	case SECLINK_KEY_IN_USE:
		return "Key is in use";
	case SECLINK_CERT_IN_USE:
		return "Certificate is in use";
	case SECLINK_DATA_IN_USE:
		return "Data is in use";
	case SECLINK_NOT_SUPPORTED:
		return "Not supported operation";
	case SECLINK_NOT_IMPLEMENTED:
		return "Not implemented in the SE";
	case SECLINK_BUSY:
		return "Busy";
	case SECLINK_NOT_INITIALIZED:
		return "Not initialized";
	}
	return "Unknown error(this should be handled carefully)";
}
