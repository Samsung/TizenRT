/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "vt_tests_common.h"
#include "psa/internal_trusted_storage.h"
#if DOMAIN_NS == 1
#include <string.h>
#else
#include "tfm_memory_utils.h"
#endif

int mbedtls_get_otp_info_ex(int type, void *output, size_t size, size_t offset);
int mbedtls_set_otp_info_ex(int type, const void *input, size_t ilen, size_t offset);

static void dump_otp(const char* str, const uint8_t *data, uint32_t size)
{
	printf("%s\r\n", str);
	for (int i = 0; i < size; i++) {
		if ((i != 0) && ((i % 16) == 0)) {
			printf("\r\n");
		}
		printf("%02x ", data[i]);
	}
	printf("\r\n");
}

static const uint8_t s_otp_input[0x40] = {0xFF};
static uint8_t s_otp_output[0x200] = {0};
static void tfm_vt_test_otp_write(const char* str, uint32_t id, uint8_t *input, uint32_t size, uint32_t offset)
{
#if TEST_S_VT
	printf("writing %s\r\n", str);
#else
	printf("writing %s, system Bus Fault in SPE!!!!!\r\n", str);
#endif
	mbedtls_set_otp_info_ex(id, input, size, offset);//Never return
}

static void tfm_vt_test_otp_read(const char* str, uint32_t id, uint8_t *output, uint32_t size, uint32_t offset)
{
	int err;

	memset(output, 0, size);
	printf("read %s, id=%d, size=0x%x, offset=%d\r\n", str, id, size, offset);
	err = mbedtls_get_otp_info_ex(id, output, size, offset);
	if (err != 0) {
		printf("otp read failed, err=%x\r\n", err);
		return err;
	}

	dump_otp(str, output, size);
}

void tfm_vt_test_otp_write_000(struct test_result_t *ret)
{
	uint8_t input[4] = {0x3, 0, 0, 0};

	tfm_vt_test_otp_write("BL1 LCS", OTP_LIFE_CYCLE, input, OTP_LIFE_CYCLE_LEN, 0);
	ret->val = TEST_PASSED;
}

void tfm_vt_test_otp_write_001(struct test_result_t *ret)
{
	tfm_vt_test_otp_write("root key", OTP_DEV_ROOT_KEY, s_otp_input, OTP_DEV_ROOT_KEY_LEN, 0);
	ret->val = TEST_PASSED;
}

void tfm_vt_test_otp_write_002(struct test_result_t *ret)
{
	tfm_vt_test_otp_write("bl1 public key", OTP_BL1_PK_HASH, s_otp_input, OTP_BL1_PK_HASH_LEN, 0);
	ret->val = TEST_PASSED;
}

void tfm_vt_test_otp_write_003(struct test_result_t *ret)
{
	tfm_vt_test_otp_write("bl2 public key", OTP_BL2_PK_HASH, s_otp_input, OTP_BL2_PK_HASH_LEN, 0);
	ret->val = TEST_PASSED;
}

void tfm_vt_test_otp_write_004(struct test_result_t *ret)
{
	tfm_vt_test_otp_write("bl1 LCS", OTP_LIFE_CYCLE, s_otp_input, OTP_LIFE_CYCLE_LEN, 0);
	ret->val = TEST_PASSED;
}

void tfm_vt_test_otp_write_005(struct test_result_t *ret)
{
	tfm_vt_test_otp_write("bl1 security counter", OTP_USR_NON_SEC_REGION, s_otp_input, OTP_LIFE_CYCLE_LEN, BL1_SECURITY_COUNTER_OFFSET);
	ret->val = TEST_PASSED;
}

void tfm_vt_test_otp_write_006(struct test_result_t *ret)
{
	tfm_vt_test_otp_write("bl2 security counter", OTP_USR_SEC_REGION, s_otp_input, BL2_SECURITY_COUNTER_LEN, BL2_SECURITY_COUNTER_OFFSET);
	ret->val = TEST_PASSED;
}

void tfm_vt_test_otp_write_007(struct test_result_t *ret)
{
	tfm_vt_test_otp_write("write HUK", OTP_USR_SEC_REGION, s_otp_input, HUK_LEN, HUK_OFFSET);
	ret->val = TEST_PASSED;
}


void tfm_vt_test_otp_read_001(struct test_result_t *ret)
{
	tfm_vt_test_otp_read("root key", OTP_DEV_ROOT_KEY, s_otp_output, OTP_DEV_ROOT_KEY_LEN, 0);
	ret->val = TEST_PASSED;
}

void tfm_vt_test_otp_read_002(struct test_result_t *ret)
{
	tfm_vt_test_otp_read("bl1 public key", OTP_BL1_PK_HASH, s_otp_output, OTP_BL1_PK_HASH_LEN, 0);
	ret->val = TEST_PASSED;
}

void tfm_vt_test_otp_read_003(struct test_result_t *ret)
{
	tfm_vt_test_otp_read("bl2 public key", OTP_BL2_PK_HASH, s_otp_output, OTP_BL2_PK_HASH_LEN, 0);
	ret->val = TEST_PASSED;
}

void tfm_vt_test_otp_read_004(struct test_result_t *ret)
{
	tfm_vt_test_otp_read("bl1 LCS", OTP_LIFE_CYCLE, s_otp_output, OTP_LIFE_CYCLE_LEN, 0);
	ret->val = TEST_PASSED;
}

void tfm_vt_test_otp_read_005(struct test_result_t *ret)
{
	tfm_vt_test_otp_read("bl1 security counter", OTP_USR_NON_SEC_REGION, s_otp_output, OTP_LIFE_CYCLE_LEN, BL1_SECURITY_COUNTER_OFFSET);
	ret->val = TEST_PASSED;
}

void tfm_vt_test_otp_read_006(struct test_result_t *ret)
{
	tfm_vt_test_otp_read("bl2 security counter", OTP_USR_SEC_REGION, s_otp_output, BL2_SECURITY_COUNTER_LEN, BL2_SECURITY_COUNTER_OFFSET);
	ret->val = TEST_PASSED;
}

void tfm_vt_test_otp_read_007(struct test_result_t *ret)
{
	tfm_vt_test_otp_read("read HUK", OTP_USR_SEC_REGION, s_otp_output, HUK_LEN, HUK_OFFSET);
	ret->val = TEST_PASSED;
}

void tfm_vt_test_otp_read_008(struct test_result_t *ret)
{
	tfm_vt_test_otp_read("read all user S space", OTP_USR_SEC_REGION, s_otp_output, OTP_USR_SEC_REGION_LEN, 0);
	ret->val = TEST_PASSED;
}

void tfm_vt_test_otp_read_009(struct test_result_t *ret)
{
	tfm_vt_test_otp_read("read all user NS space", OTP_USR_NON_SEC_REGION, s_otp_output, OTP_USR_NON_SEC_REGION_LEN, 0);
	ret->val = TEST_PASSED;
}

void tfm_vt_test_otp_read_010(struct test_result_t *ret)
{
	tfm_vt_test_otp_read("read all user NS space + 1", OTP_USR_NON_SEC_REGION, s_otp_output, OTP_USR_NON_SEC_REGION_LEN + 1, 0);
	ret->val = TEST_PASSED;
}
