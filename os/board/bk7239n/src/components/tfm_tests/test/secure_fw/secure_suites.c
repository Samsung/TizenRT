/*
 * Copyright (c) 2017-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "test_framework_integ_test.h"
#include "test_framework_integ_test_helper.h"
#include "test_framework.h"

/* Service specific includes */
#ifdef TEST_S_PS
#include "ps_tests.h"
#endif
#ifdef TEST_S_ITS
#include "its_s_tests.h"
#endif
#ifdef TEST_S_ATTESTATION
#include "attest_s_tests.h"
#endif
#ifdef TEST_S_CRYPTO
#include "crypto_s_tests.h"
#endif
#ifdef TEST_S_FWU
#include "fwu_s_tests.h"
#endif
#ifdef TEST_S_PLATFORM
#include "platform_s_tests.h"
#endif
#ifdef TEST_S_AUDIT
#include "audit_s_tests.h"
#endif
#ifdef TEST_S_IPC
#include "ipc_s_tests.h"
#endif
#ifdef TEST_S_SFN_BACKEND
#include "sfn_s_tests.h"
#endif
#if defined (TEST_S_FPU)
#include "fpu_s_tests.h"
#endif
#ifdef EXTRA_S_TEST_SUITE
#include "extra_s_tests.h"
#endif
#ifdef TEST_S_VT
#include "vt_s_tests.h"
#endif

static struct test_suite_t test_suites[] = {
#ifdef TEST_S_IPC
    /* Secure IPC test cases */
    {&register_testsuite_s_ipc_interface, 0, 0, 0},
#endif

#ifdef TEST_S_SFN_BACKEND
    /* Secure SFN backend test cases */
    {&register_testsuite_s_sfn_interface, 0, 0, 0},
#endif

#ifdef TEST_S_PS
    {&register_testsuite_s_psa_ps_interface, 0, 0, 0},
    {&register_testsuite_s_psa_ps_reliability, 0, 0, 0},

#ifdef PS_TEST_NV_COUNTERS
    {&register_testsuite_s_rollback_protection, 0, 0, 0},
#endif
#endif

#ifdef TEST_S_ITS
    /* Secure ITS test cases */
    {&register_testsuite_s_psa_its_interface, 0, 0, 0},
    {&register_testsuite_s_psa_its_reliability, 0, 0, 0},
#endif

#ifdef TEST_S_CRYPTO
    /* Crypto test cases */
    {&register_testsuite_s_crypto_interface, 0, 0, 0},
#endif

#ifdef TEST_S_ATTESTATION
    /* Secure initial attestation service test cases */
    {&register_testsuite_s_attestation_interface, 0, 0, 0},
#endif

#ifdef TEST_S_PLATFORM
    /* Secure platform service test cases */
    {&register_testsuite_s_platform_interface, 0, 0, 0},
#endif

#ifdef TEST_S_AUDIT
    /* Secure Audit Logging test cases */
    {&register_testsuite_s_audit_interface, 0, 0, 0},
#endif

#ifdef TEST_S_FWU
    /* Secure Firmware Update test cases */
    {&register_testsuite_s_psa_fwu_interface, 0, 0, 0},
#endif

#ifdef TEST_S_FPU
    /* Secure FPU test cases */
    {&register_testsuite_s_fpu_interface, 0, 0, 0},
#endif

#ifdef EXTRA_S_TEST_SUITE
    /* Secure extra test cases */
    {&register_testsuite_extra_s_interface, 0, 0, 0},
#endif

#ifdef TEST_S_VT
    /* Secure VT test cases */
    {&register_testsuite_s_psa_vt_interface, 0, 0, 0},
#endif


    /* End of test suites */
    {0, 0, 0, 0}
};

static void setup_integ_test(void)
{
    /* Left empty intentionally, currently implemented
     * test suites require no setup
     */
}

static void tear_down_integ_test(void)
{
    /* Left empty intentionally, currently implemented
     * test suites require no tear down
     */
}

volatile int g_test_suite_flag = 0;
uint32_t test_suite_is_testing(void)
{
	return g_test_suite_flag;
}

enum test_suite_err_t start_integ_test(void)
{
    enum test_suite_err_t retval;

	g_test_suite_flag = 1;
    setup_integ_test();
    retval = integ_test("Secure", test_suites);
    tear_down_integ_test();
	g_test_suite_flag = 0;

    return retval;
}
