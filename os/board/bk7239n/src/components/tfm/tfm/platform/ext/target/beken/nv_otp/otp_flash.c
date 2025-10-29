/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* NOTE: For the security of the protected storage system, the bootloader
 * rollback protection, and the protection of cryptographic material  it is
 * CRITICAL to use a internal (in-die) persistent memory for the implementation
 * of the OTP_NV_COUNTERS flash area (see flash_otp_nv_layout.c).
 */

#include "tfm_plat_otp.h"

#include "flash_layout.h"
#include <string.h>
#include <stddef.h>
#include "nv_otp_log.h"
#include "otp.h"
#include "mbedtls/otp.h"

#define TAG "otp"
#if 0//define at nv_otp_log.h
#define OTP_LOGD BK_LOGD
#define OTP_LOGI BK_LOGI
#define OTP_LOGW BK_LOGW
#define OTP_LOGE BK_LOGE
#endif
#define OTP_LOG_RAW(...) 

#define OTP_ROTPK_ID             5
#define OTP_SECURITY_COUNTER_ID  0

enum tfm_plat_err_t tfm_plat_otp_init(void)
{
	OTP_LOGI(TAG, "init\r\n");
	return TFM_PLAT_ERR_SUCCESS;
}

#define HUK_LEN (16)
enum tfm_plat_err_t tfm_plat_otp_read(enum tfm_otp_element_id_t id,
                                      size_t out_len, uint8_t *out)
{
	int ret;

	if (id == PLAT_OTP_ID_HUK) {
		memset(out, 0, out_len);
		ret = mbedtls_get_otp_info(3, out, (out_len > HUK_LEN ? HUK_LEN : out_len), 0);
	} else if (id == PLAT_OTP_ID_BL2_ROTPK_0) {
		ret = mbedtls_get_otp_info(5, out, out_len, 0);
	} else if (id == PLAT_OTP_ID_NV_COUNTER_BL2_0) {
		ret = mbedtls_get_otp_info(9, out, out_len, 0);
	} else {
		memset(out, 0, out_len);
		OTP_LOGI(TAG, "otp read stub, id=%d\r\n", id);
		return TFM_PLAT_ERR_SUCCESS;
	}

	if (ret != 0) {
		OTP_LOGE(TAG, "mbedtls get otp failed, ret=%d\r\n", ret);
		return -1;
	}
#if 0
	OTP_LOGI(TAG, "dump otp, id=%d, size=%d\r\n", id, out_len);
	for (int i = 0; i < out_len; i++) {
		OTP_LOG_RAW("%02x ", out[i]);
	}
	OTP_LOG_RAW("\r\n");
#endif
	OTP_LOGI(TAG, "read, id=%d, out_len=%d\r\n", id, out_len);
	return TFM_PLAT_ERR_SUCCESS;
}

#if defined(OTP_WRITEABLE)
enum tfm_plat_err_t tfm_plat_otp_write(enum tfm_otp_element_id_t id,
                                       size_t in_len, const uint8_t *in)
{
	int ret;

	if (id == PLAT_OTP_ID_NV_COUNTER_BL2_0) {
		ret = mbedtls_set_otp_info(9, in, in_len, 0);
	} else {
		OTP_LOGE(TAG, "otp write stub, id=%d\r\n", id);
		return TFM_PLAT_ERR_SUCCESS;
	}

	if (ret != 0) {
		OTP_LOGE(TAG, "mbedtls write otp failed, ret=%d\r\n", ret);
		return -1;
	}

	OTP_LOGI(TAG, "write, id=%d, in_len=%d\r\n", id, in_len);
	return TFM_PLAT_ERR_SUCCESS;
}
#else
enum tfm_plat_err_t tfm_plat_otp_write(enum tfm_otp_element_id_t id,
                                       size_t in_len, const uint8_t *in)
{
	(void)id;
	(void)in_len;
	(void)in;
	return TFM_PLAT_ERR_UNSUPPORTED;
}
#endif

enum tfm_plat_err_t tfm_plat_otp_get_size(enum tfm_otp_element_id_t id,
                                          size_t *size)
{
	if (id == PLAT_OTP_ID_NV_COUNTER_BL2_0) {
		*size = 32;
	} else if (id == PLAT_OTP_ID_BL2_ROTPK_0) {
		*size = 32;
	} else {
		OTP_LOGE(TAG, "unsupported id=%d\r\n", id);
		return TFM_PLAT_ERR_UNSUPPORTED;
	}

	return TFM_PLAT_ERR_SUCCESS;
}
