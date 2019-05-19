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
 * either express or implied. SEC the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef _SECURITY_API_INTERNAL_H__
#define _SECURITY_API_INTERNAL_H__
#include <stdio.h>
#include <string.h>
#include <tinyara/seclink.h>

#ifdef LINUX
#define SECAPI_LOG printf
#else
#define SECAPI_LOG sfdbg
#endif

#define SECAPI_TAG "[SECAPI]"

#define SECAPI_ERR                                                   \
	do {                                                             \
		SECAPI_LOG(SECAPI_TAG"ERR\t%s:%d\n", __FILE__, __LINE__);    \
	} while (0)

#define SECAPI_DUMMY

#define SECAPI_CALL3(func, res, rsc_clean)		\
	do {										\
		int secapi_res = func;					\
		if (secapi_res != SECLINK_OK) {			\
			SECAPI_ERR;							\
			rsc_clean;							\
			return res;							\
		}										\
	} while (0)

#define SECAPI_CALL2(func, res)					\
	do {										\
		SECAPI_CALL3(func, res, SECAPI_DUMMY);	\
	} while (0)

#define SECAPI_CALL(func)						\
	do {										\
		SECAPI_CALL2(func, SECURITY_ERROR);		\
	} while (0)


#define SECAPI_ENTER                                                 \
	do {                                                             \
		SECAPI_LOG(SECAPI_TAG "---> %s:%d\n", __FILE__, __LINE__);   \
	} while (0)

#define SECAPI_RETURN(res)                                           \
	do {                                                             \
		SECAPI_LOG(SECAPI_TAG "<--- %s:%d\n", __FILE__, __LINE__);   \
		return res;                                             \
	} while (0)

/* convert a value from seclink then return it
 */
#define SECAPI_HAL_RETURN(res)	\
	do {\
	SECAPI_RETURN(secutils_convert_error_h2s(res));\
	} while (0)

#define SECAPI_DATA_SCOPY(hal, sec)				\
	do {										\
		sec->data = hal.data;					\
		sec->length = hal.data_len;				\
	} while (0)

#define SECAPI_DATA_DCOPY(hal, sec)					\
	do {											\
		memcpy(sec->data, hal.data, hal.data_len);	\
		sec->length = hal.data_len;					\
	} while (0)

#define SECAPI_DATA_ALLOC_HAL(hal, sec)				\
	do {											\
		sec->data = (char *)malloc(hal->data_len);	\
		if (!sec->data) {							\
			return -2;								\
		}											\
	} while (0)

#define SECAPI_PRIV_DCOPY(hal, sec)					\
	do {											\
		memcpy(sec->data, hal.priv, hal.priv_len);	\
		sec->length = hal.priv_len;					\
	} while (0)

#define SECAPI_FREE(hal)						\
	do {										\
		free(hal->data);						\
		hal->data = NULL;						\
		hal->length = 0;						\
	} while (0)

#define SECAPI_ISHANDLE_VALID(ctx)							\
	do {													\
		if (!ctx) {											\
			SECAPI_ERR;										\
			SECAPI_RETURN(SECURITY_INVALID_INPUT_PARAMS);	\
		}													\
	} while (0)

#define SECAPI_CONVERT_PATH(path, idx)							\
	do {														\
		int32_t c_res = secutils_convert_path_s2h(path, idx);	\
		if (c_res < 0) {										\
			SECAPI_RETURN(SECURITY_INVALID_KEY_INDEX);			\
		}														\
	} while (0)

#define SECAPI_CONVERT_AESPARAM(sec, hal)						\
	do {														\
		int c_res = secutils_convert_aesparam_s2h(sec, hal);	\
			if (c_res < 0) {									\
				SECAPI_RETURN(SECURITY_INVALID_INPUT_PARAMS);	\
			}													\
	} while (0)

#define SECAPI_CONVERT_KEYTYPE(sec, hal)					\
	do {													\
		hal = secutils_convert_key_s2h(sec);				\
		if (hal == HAL_KEY_UNKNOWN) {						\
			SECAPI_RETURN(SECURITY_INVALID_INPUT_PARAMS);	\
		}													\
	} while (0)

#define SECAPI_CONVERT_RSAPARAM(sec, hal)						\
	do {														\
		int c_res = secutils_convert_rsaparam_s2h(sec, hal);	\
		if (c_res < 0) {										\
			SECAPI_RETURN(SECURITY_INVALID_INPUT_PARAMS);		\
		}														\
	} while (0)

#define SECAPI_CONVERT_ECDSAPARAM(sec, hal)						\
	do {														\
		int c_res = secutils_convert_ecdsaparam_s2h(sec, hal);	\
		if (c_res < 0) {										\
			SECAPI_RETURN(SECURITY_INVALID_INPUT_PARAMS);		\
		}														\
	} while (0)

#define SECAPI_CONVERT_ECDHPARAM(sec, hal)						\
	do {														\
		int c_res = secutils_convert_ecdhparam_s2h(sec, hal);	\
		if (c_res < 0) {										\
			SECAPI_RETURN(SECURITY_INVALID_INPUT_PARAMS);		\
		}														\
	} while (0)

#define SECAPI_CONVERT_DHPARAM_H2S(hal, sec)					\
	do {														\
		int c_res = secutils_convert_dhparam_h2s(hal, sec);		\
		if (c_res == -1) {										\
			SECAPI_RETURN(SECURITY_INVALID_INPUT_PARAMS);		\
		} else if (c_res == -2)									\
			SECAPI_RETURN(SECURITY_ALLOC_ERROR);				\
	} while (0)

#define SECAPI_CONVERT_DHPARAM(sec, hal)						\
	do {														\
		int c_res = secutils_convert_dhparam_s2h(sec, hal);		\
		if (c_res < 0) {										\
			SECAPI_RETURN(SECURITY_INVALID_INPUT_PARAMS);		\
		}														\
	} while (0)

#define SECAPI_CONVERT_HASHMODE(mode, type)						\
	do {														\
		type = secutils_convert_hashmode_s2h(mode);				\
		if (type == HAL_HASH_UNKNOWN) {							\
			SECAPI_RETURN(SECURITY_INVALID_INPUT_PARAMS);		\
		}														\
	} while (0)

#define SECAPI_CONVERT_HMACMODE(mode, type)						\
	do {														\
		type = secutils_convert_hmacmode_s2h(mode);				\
		if (type == HAL_HMAC_UNKNOWN) {							\
			SECAPI_RETURN(SECURITY_INVALID_INPUT_PARAMS);		\
		}														\
	} while (0)

struct security_ctx {
	sl_ctx sl_hnd;
	void *data1; // used to get data from HAL
	unsigned int dlen1;
	void *data2;
	unsigned int dlen2;
	void *data3;
	unsigned int dlen3;
};

hal_key_type secutils_convert_key_s2h(security_key_type sec);
hal_hash_type secutils_convert_hashmode_s2h(security_hash_mode mode);
hal_hmac_type secutils_convert_hmacmode_s2h(security_hmac_mode mode);
hal_aes_algo secutils_convert_aesmode_s2h(security_aes_mode mode);
hal_rsa_algo secutils_convert_rsamode_s2h(security_rsa_mode mode);
hal_ecdsa_curve secutils_convert_ecdsamode_s2h(security_ecdsa_mode mode);
hal_dh_key_type secutils_convert_dhmode_s2h(security_dh_mode mode);
hal_key_type secutils_convert_ecdsamode_to_key_s2h(security_ecdsa_mode mode);

security_error secutils_convert_error_h2s(hal_result_e herr);

int secutils_convert_path_s2h(const char *path, uint32_t *slot);
int secutils_convert_aesparam_s2h(security_aes_param *sparam, hal_aes_param *hparam);
int secutils_convert_rsaparam_s2h(security_rsa_param *sparam, hal_rsa_mode *hparam);
int secutils_convert_ecdsaparam_s2h(security_ecdsa_param *eparam, hal_ecdsa_mode *hmode);
int secutils_convert_dhparam_s2h(security_dh_param *dparam, hal_dh_data *hdata);
int secutils_convert_ecdhparam_s2h(security_ecdh_param *eparam, hal_ecdh_data *hdata);

int secutils_convert_dhparam_h2s(hal_dh_data *hdata, security_dh_param *dparam);

#endif // _SECURITY_API_INTERNAL_H__
