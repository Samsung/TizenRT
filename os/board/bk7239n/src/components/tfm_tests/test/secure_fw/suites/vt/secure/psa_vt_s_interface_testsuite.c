/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "vt_s_tests.h"
#include "test_framework_helpers.h"
#include "../vt_tests_common.h"

#define TEST_OTP_WRITE 0
#define TEST_OTP_READ 0

static struct test_t psa_vt_s_tests[] = {
    /* Because the Lifecycle of all 5 BK7236 boards is CM, update it to DD via this case */
    {&tfm_vt_test_otp_write_000, "TFM_S_VT_TEST_OTP_WRITE_1000", "update LCS from DM/CM to DD"},

#if TEST_OTP_WRITE
    /* BK7236 SPE can write some OTP, it's very dangerous to enable following cases in SPE */
    {&tfm_vt_test_otp_write_001, "TFM_S_VT_TEST_OTP_WRITE_1001", "write root key"},
    {&tfm_vt_test_otp_write_002, "TFM_S_VT_TEST_OTP_WRITE_1002", "write bl1 public key hash"},
    {&tfm_vt_test_otp_write_003, "TFM_S_VT_TEST_OTP_WRITE_1003", "write bl2 public key hash"},
    {&tfm_vt_test_otp_write_004, "TFM_S_VT_TEST_OTP_WRITE_1004", "write bl1 lcs"},
    {&tfm_vt_test_otp_write_005, "TFM_S_VT_TEST_OTP_WRITE_1005", "write bl1 security counter"},
    {&tfm_vt_test_otp_write_006, "TFM_S_VT_TEST_OTP_WRITE_1006", "write bl2 security counter"},
    {&tfm_vt_test_otp_write_007, "TFM_S_VT_TEST_OTP_WRITE_1007", "write HUK"},
#endif

    /* When lifecycle of BK7236 is in DM/CM, the SPE can read any OTP, but when
     * lifecycle is in DD, following read lead the system to BF:
     * 
     *  - tfm_vt_test_otp_read_001 - root key
     *  - tfm_vt_test_otp_read_002 - bl1 public key hash
     *  - tfm_vt_test_otp_read_003 - bl2 public key hash
     *  - tfm_vt_test_otp_read_006 - bl2 security counter
     *  - tfm_vt_test_otp_read_007 - read HUK
     *  - tfm_vt_test_otp_read_008 - user security OTP
     *  - tfm_vt_test_otp_read_010 - user non-security OTP + 1
     *
     **/

    //{&tfm_vt_test_otp_read_001, "TFM_S_VT_TEST_OTP_READ_1001", "read root key"},
    {&tfm_vt_test_otp_read_002, "TFM_S_VT_TEST_OTP_READ_1002", "read bl1 public key hash"},
    {&tfm_vt_test_otp_read_003, "TFM_S_VT_TEST_OTP_READ_1003", "read bl2 public key hash"},
    {&tfm_vt_test_otp_read_004, "TFM_S_VT_TEST_OTP_READ_1004", "read bl1 lcs"},
    {&tfm_vt_test_otp_read_005, "TFM_S_VT_TEST_OTP_READ_1005", "read bl1 security counter"},
    {&tfm_vt_test_otp_read_006, "TFM_S_VT_TEST_OTP_READ_1006", "read bl2 security counter"},
    {&tfm_vt_test_otp_read_007, "TFM_S_VT_TEST_OTP_READ_1007", "read HUK"},
    {&tfm_vt_test_otp_read_008, "TFM_S_VT_TEST_OTP_READ_1008", "read all user S SPACE"},
    {&tfm_vt_test_otp_read_009, "TFM_S_VT_TEST_OTP_READ_1009", "read all user NS SPACE"},
    //{&tfm_vt_test_otp_read_010, "TFM_S_VT_TEST_OTP_READ_1010", "read all user NS+1 SPACE"},
};

void register_testsuite_s_psa_vt_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(psa_vt_s_tests) / sizeof(psa_vt_s_tests[0]));

    set_testsuite("PSA vulnerable tests " "(TFM_S_VT_TEST_1XXX)", psa_vt_s_tests, list_size, p_test_suite);
}
