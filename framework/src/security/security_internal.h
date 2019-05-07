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

#include <tinyara/seclink.h>

#define SECAPI_LOG printf

#define SECAPI_TAG "[SECAPI]"

#define SECAPI_ERR														\
	do {																\
		SECAPI_LOG("[ERR] %s\t%s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
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


#define SECAPI_ENTER													\
	do {																\
		SECAPI_LOG(SECAPI_TAG "%s\t%s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
	} while (0)

#define SECAPI_RETURN(res)												\
	do {																\
		SECAPI_LOG(SECAPI_TAG "%s\t%s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
		return res;														\
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

#define SECAPI_ISHANDLE_VALID(ctx)					\
	do {											\
		if (!ctx) {									\
			SECAPI_ERR;								\
			return SECURITY_INVALID_INPUT_PARAMS;	\
		}											\
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
		int c_res = secutils_convert_aesparam_s2h(sec, hal);			\
			if (c_res < 0) {									\
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

struct security_ctx {
	sl_ctx sl_hnd;
};

hal_key_type secutils_convert_key_s2h(security_key_type sec);
security_error secutils_convert_error_h2s(hal_result_e herr);
int secutils_convert_path_s2h(const char *path, uint32_t *slot);
int secutils_convert_aesparam_s2h(security_aes_param *sparam, hal_aes_param *hparam);
int secutils_convert_rsaparam_s2h(security_rsa_param *sparam, hal_rsa_mode *hparam);

#endif // _SECURITY_API_INTERNAL_H__
