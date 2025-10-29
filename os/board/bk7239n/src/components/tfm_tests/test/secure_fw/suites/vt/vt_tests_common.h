/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __VT_TESTS_COMMON_H__
#define __VT_TESTS_COMMON_H__

#include "test_framework_helpers.h"

#ifdef __cplusplus
extern "C" {
#endif

enum {
	OTP_MODEL_ID,          //0
	OTP_MODEL_KEY,         //1
	OTP_DEV_ID,            //2
	OTP_DEV_ROOT_KEY,      //3
	OTP_BL1_PK_HASH,       //4
	OTP_BL2_PK_HASH,       //5
	OTP_LIFE_CYCLE,        //6
	OTP_LOCK_CTRL,         //7
	OTP_USR_NON_SEC_REGION,//8
	OTP_USR_SEC_REGION,    //9
	OTP_TEST_REGION,       //10
};

#define OTP_MODEL_ID_LEN           0x4
#define OTP_MODEL_KEY_LEN          0x10
#define OTP_DEV_ID_LEN             0x4
#define OTP_DEV_ROOT_KEY_LEN       0x10
#define OTP_BL1_PK_HASH_LEN        0x20
#define OTP_BL2_PK_HASH_LEN        0x20
#define OTP_LIFE_CYCLE_LEN         0x4
#define OTP_LOCK_CTRL_LEN          0x4
#define OTP_USR_NON_SEC_REGION_LEN 0x100
#define OTP_USR_SEC_REGION_LEN     0x180
#define OTP_TEST_REGION_LEN        0x0

#define BL1_SECURITY_COUNTER_LEN   0x4
#define BL1_SECURITY_COUNTER_OFFSET 0x8

#define BL2_SECURITY_COUNTER_LEN    0x20
#define BL2_SECURITY_COUNTER_OFFSET 0x0

#define HUK_LEN 0x20
#define HUK_OFFSET 0x20

void tfm_vt_test_otp_write_000(struct test_result_t *ret);
void tfm_vt_test_otp_write_001(struct test_result_t *ret);
void tfm_vt_test_otp_write_002(struct test_result_t *ret);
void tfm_vt_test_otp_write_003(struct test_result_t *ret);
void tfm_vt_test_otp_write_004(struct test_result_t *ret);
void tfm_vt_test_otp_write_005(struct test_result_t *ret);
void tfm_vt_test_otp_write_006(struct test_result_t *ret);
void tfm_vt_test_otp_write_007(struct test_result_t *ret);

void tfm_vt_test_otp_read_001(struct test_result_t *ret);
void tfm_vt_test_otp_read_002(struct test_result_t *ret);
void tfm_vt_test_otp_read_003(struct test_result_t *ret);
void tfm_vt_test_otp_read_004(struct test_result_t *ret);
void tfm_vt_test_otp_read_005(struct test_result_t *ret);
void tfm_vt_test_otp_read_006(struct test_result_t *ret);
void tfm_vt_test_otp_read_007(struct test_result_t *ret);
void tfm_vt_test_otp_read_008(struct test_result_t *ret);
void tfm_vt_test_otp_read_009(struct test_result_t *ret);
void tfm_vt_test_otp_read_010(struct test_result_t *ret);


#ifdef __cplusplus
}
#endif

#endif /* __VT_TESTS_COMMON_H__ */
