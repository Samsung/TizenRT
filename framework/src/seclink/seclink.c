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

#ifdef SECLINK_PATH
#undef SECLINK_PATH
#endif

#ifndef LINUX
#define SECLINK_PATH "/dev/seclink"
#define SL_LOG sedbg
#else
#define SECLINK_PATH "./seclink"
#define SL_LOG printf
#endif

#define SL_TAG "[SECLINK]"

#define SL_ERR(fd)														\
	do {																\
		SL_LOG(SL_TAG"%s:%d ret(%d) code(%s)\n",			            \
			   __FILE__, __LINE__, fd, strerror(errno));                \
	} while (0)

#ifdef LINUX
extern int sl_post_msg(int fd, int cmd, unsigned long arg);

#define SL_CALL(hnd, code, param)										\
	do {																\
		int i_res = sl_post_msg(hnd->fd, code, (unsigned long)((uintptr_t)&param)); \
		if (i_res < 0) {												\
			SL_ERR(i_res);												\
			return SECLINK_ERROR;										\
		}																\
	} while (0)
#else
#define SL_CALL(hnd, code, param)										\
	do {																\
		int i_res = ioctl(hnd->fd, code, (unsigned long)((uintptr_t)&param)); \
		if (i_res < 0) {												\
			SL_ERR(i_res);												\
			return SECLINK_ERROR;										\
		}																\
	} while (0)
#endif


#define SL_ENTER														\
	do {																\
		SL_LOG(SL_TAG"%s\t%s:%d\n", __FUNCTION__, __FILE__, __LINE__);	\
	} while (0)

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

/*
 * Structure
 */
struct _seclink_s_ {
	int fd;
};

/* global */

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

	struct seclink_req req = {.req_type.comm = NULL, 0};
	SL_CALL(handle, SECLINKIOC_INIT, req);
	if (req.res != HAL_SUCCESS) {
		close(fd);
		free(handle);
		return SECLINK_ERROR;
	}

	return SECLINK_OK;
}

int sl_deinit(sl_ctx hnd)
{
	SL_ENTER;

	if (!hnd || ((struct _seclink_s_ *)hnd)->fd <= 0) {
		SL_ERR(((struct _seclink_s_ *)hnd)->fd);
		return -1;
	}
	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;

	struct seclink_req req = {.req_type.comm = NULL, 0};
	SL_CALL(sl, SECLINKIOC_DEINIT, req);
	if (req.res != HAL_SUCCESS) {
		SL_LOG("fail to deinit");
	}

	SL_CLOSE(sl->fd);

	free(sl);

	return 0;
}

/*  key manager */
int sl_set_key(sl_ctx hnd, hal_key_type mode, uint32_t key_idx, hal_data *key, hal_data *prikey, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_key_info info = {mode, key_idx, key, prikey};
	struct seclink_req req = {.req_type.key = &info, 0};

	SL_CALL(sl, SECLINKIOC_SETKEY, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_get_key(sl_ctx hnd, hal_key_type mode, uint32_t key_idx, _OUT_ hal_data *key, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_key_info info = {mode, key_idx, key, NULL};
	struct seclink_req req = {.req_type.key = &info, 0};

	SL_CALL(sl, SECLINKIOC_GETKEY, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_remove_key(sl_ctx hnd, hal_key_type mode, uint32_t key_idx, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_key_info info = {mode, key_idx, NULL, NULL};
	struct seclink_req req = {.req_type.key = &info, 0};

	SL_CALL(sl, SECLINKIOC_REMOVEKEY, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_generate_key(sl_ctx hnd, hal_key_type mode, uint32_t key_idx, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_key_info info = {mode, key_idx, NULL, NULL};
	struct seclink_req req = {.req_type.key = &info, 0};

	SL_CALL(sl, SECLINKIOC_GENERATEKEY, req);
	*hres = req.res;

	return SECLINK_OK;
}


/*  Authenticate */
int sl_generate_random(sl_ctx hnd, uint32_t len, _OUT_ hal_data *random, hal_result_e *hres)
{
	SL_ENTER;
	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = len, -1, random, .auth_data.data = NULL};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_GENERATERANDOM, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_get_hash(sl_ctx hnd, hal_hash_type mode, hal_data *input, _OUT_ hal_data *hash, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.hash_type = mode, -1, input, .auth_data.data = hash};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_GETHASH, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_get_hmac(sl_ctx hnd, hal_hmac_type mode, hal_data *input, uint32_t key_idx, _OUT_ hal_data *hmac, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.hmac_type = mode, key_idx, input, .auth_data.data = hmac};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_GETHMAC, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_rsa_sign_md(sl_ctx hnd, hal_rsa_mode mode, hal_data *hash, uint32_t key_idx, _OUT_ hal_data *sign, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.rsa_type = mode, key_idx, hash, .auth_data.data = sign};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_RSASIGNMD, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_rsa_verify_md(sl_ctx hnd, hal_rsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.rsa_type = mode, key_idx, hash, .auth_data.data = sign};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_RSAVERIFYMD, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_ecdsa_sign_md(sl_ctx hnd, hal_ecdsa_mode mode, hal_data *hash, uint32_t key_idx, _OUT_ hal_data *sign, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.ecdsa_type = mode, key_idx, hash, .auth_data.data = sign};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_ECDSASIGNMD, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_ecdsa_verify_md(sl_ctx hnd, hal_ecdsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.ecdsa_type = mode, key_idx, hash, .auth_data.data = sign};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_ECDSAVERIFYMD, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_dh_generate_param(sl_ctx hnd, uint32_t dh_idx, _INOUT_ hal_dh_data *dh_param, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, dh_idx, NULL, .auth_data.dh_param = dh_param};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_DHGENERATEPARAM, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_dh_compute_shared_secret(sl_ctx hnd, hal_dh_data *dh_param, uint32_t dh_idx, _OUT_ hal_data *shared_secret, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, dh_idx, shared_secret, .auth_data.dh_param = dh_param};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_DHCOMPUTESHAREDSECRET, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_ecdh_compute_shared_secret(sl_ctx hnd, hal_ecdh_data *ecdh_mode, uint32_t key_idx, _OUT_ hal_data *shared_secret, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, key_idx, shared_secret, .auth_data.ecdh_param = ecdh_mode};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_ECDHCOMPUTESHAREDSECRET, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_set_certificate(sl_ctx hnd, uint32_t cert_idx, hal_data *cert_in, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, cert_idx, cert_in, .auth_data.data = NULL};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_SETCERTIFICATE, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_get_certificate(sl_ctx hnd, uint32_t cert_idx, _OUT_ hal_data *cert_out, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, cert_idx, cert_out, .auth_data.data = NULL};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_GETCERTIFICATE, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_remove_certificate(sl_ctx hnd, uint32_t cert_idx, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, cert_idx, 0, .auth_data.data = NULL};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_REMOVECERTIFICATE, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_get_factory_key(sl_ctx hnd, uint32_t key_idx, hal_data *key, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, key_idx, key, .auth_data.data = NULL};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_GETFACTORY_KEY, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_get_factory_cert(sl_ctx hnd, uint32_t cert_idx, hal_data *cert, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, cert_idx, cert, .auth_data.data = NULL};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_GETFACTORY_CERT, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_get_factory_data(sl_ctx hnd, uint32_t data_idx, hal_data *data, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_auth_info info = {.auth_type.random_len = 0, data_idx, data, .auth_data.data = NULL};
	struct seclink_req req = {.req_type.auth = &info, 0};

	SL_CALL(sl, SECLINKIOC_GETFACTORY_DATA, req);
	*hres = req.res;

	return SECLINK_OK;
}

/*  Crypto */
int sl_aes_encrypt(sl_ctx hnd, hal_data *dec_data, hal_aes_param *aes_param, uint32_t key_idx, _OUT_ hal_data *enc_data, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_crypto_info info = {key_idx, dec_data, enc_data, aes_param, NULL};
	struct seclink_req req = {.req_type.crypto = &info, 0};

	SL_CALL(sl, SECLINKIOC_AESENCRYPT, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_aes_decrypt(sl_ctx hnd, hal_data *enc_data, hal_aes_param *aes_param, uint32_t key_idx, _OUT_ hal_data *dec_data, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_crypto_info info = {key_idx, enc_data, dec_data, aes_param, NULL};
	struct seclink_req req = {.req_type.crypto = &info, 0};

	SL_CALL(sl, SECLINKIOC_AESDECRYPT, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_rsa_encrypt(sl_ctx hnd, hal_data *dec_data, hal_rsa_mode *rsa_mode, uint32_t key_idx, _OUT_ hal_data *enc_data, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_crypto_info info = {key_idx, dec_data, enc_data, NULL, rsa_mode};
	struct seclink_req req = {.req_type.crypto = &info, 0};

	SL_CALL(sl, SECLINKIOC_RSAENCRYPT, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_rsa_decrypt(sl_ctx hnd, hal_data *enc_data, hal_rsa_mode *rsa_mode, uint32_t key_idx, _OUT_ hal_data *dec_data, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_crypto_info info = {key_idx, enc_data, dec_data, NULL, rsa_mode};
	struct seclink_req req = {.req_type.crypto = &info, 0};

	SL_CALL(sl, SECLINKIOC_RSADECRYPT, req);
	*hres = req.res;

	return SECLINK_OK;
}


/*  Secure Storage */
int sl_write_storage(sl_ctx hnd, uint32_t ss_idx, hal_data *data, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_ss_info info = {ss_idx, data};
	struct seclink_req req = {.req_type.ss = &info, 0};

	SL_CALL(sl, SECLINKIOC_WRITESTORAGE, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_read_storage(sl_ctx hnd, uint32_t ss_idx, _OUT_ hal_data *data, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_ss_info info = {ss_idx, data};
	struct seclink_req req = {.req_type.ss = &info, 0};

	SL_CALL(sl, SECLINKIOC_READSTORAGE, req);
	*hres = req.res;

	return SECLINK_OK;
}

int sl_delete_storage(sl_ctx hnd, uint32_t ss_idx, hal_result_e *hres)
{
	SL_ENTER;

	SL_CHECK_VALID(hnd);

	struct _seclink_s_ *sl = (struct _seclink_s_ *)hnd;
	struct seclink_ss_info info = {ss_idx, NULL};
	struct seclink_req req = {.req_type.ss = &info, 0};

	SL_CALL(sl, SECLINKIOC_DELETESTORAGE, req);
	*hres = req.res;

	return SECLINK_OK;
}
