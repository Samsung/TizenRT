/*
 * Copyright (c) 2017-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "test_framework_integ_test.h"
#include "test_framework_integ_test_helper.h"
#include "test_framework.h"

/* Service specific includes */
#ifdef TEST_NS_PS
#include "ps_ns_tests.h"
#endif
#ifdef TEST_NS_ITS
#include "its_ns_tests.h"
#endif
#ifdef TEST_NS_CRYPTO
#include "crypto_ns_tests.h"
#endif
#ifdef TEST_NS_FWU
#include "fwu_ns_tests.h"
#endif
#ifdef TEST_NS_ATTESTATION
#include "attest_ns_tests.h"
#endif
#ifdef TEST_NS_QCBOR
#include "qcbor_ns_tests.h"
#endif
#ifdef TEST_NS_T_COSE
#include "t_cose_ns_tests.h"
#endif
#ifdef TEST_NS_PLATFORM
#include "platform_ns_tests.h"
#endif
#ifdef TEST_NS_CORE
#include "core_ns_tests.h"
#endif
#ifdef TEST_NS_IPC
#include "ipc_ns_tests.h"
#else
#ifdef TEST_NS_AUDIT
#include "audit_ns_tests.h"
#endif
#endif /* TFM_PSA_API */
#ifdef TEST_NS_MULTI_CORE
#include "multi_core_ns_test.h"
#endif /* TFM_MULTI_CORE_TOPOLOGY */
#ifdef TFM_FUZZER_TOOL_TESTS
#include "tf_fuzz_testsuite.h"
#endif /* TFM_FUZZER_TOOL_TESTS */
#ifdef TEST_NS_MANAGE_NSID
#include "nsid_testsuite.h"
#endif /* TEST_NS_MANAGE_NSID */
#if defined(TEST_NS_SLIH_IRQ) || defined(TEST_NS_FLIH_IRQ)
#include "irq_testsuite.h"
#endif
#ifdef TEST_NS_SFN_BACKEND
#include "sfn_ns_tests.h"
#endif
#ifdef TEST_NS_FPU
#include "fpu_ns_tests.h"
#endif
#ifdef EXTRA_NS_TEST_SUITE
#include "extra_ns_tests.h"
#endif
#ifdef TEST_NS_VT
#include "vt_ns_tests.h"
#endif


static struct test_suite_t test_suites[] = {
    /* List test cases which are compliant with level 1 isolation */
#ifdef TEST_NS_CORE
    /* Non-secure core test cases */
    {&register_testsuite_ns_core_positive, 0, 0, 0},
#endif

#ifdef TEST_NS_IPC
    /* Non-secure IPC test cases */
    {&register_testsuite_ns_ipc_interface, 0, 0, 0},
#endif

#ifdef TEST_NS_SFN_BACKEND
    {&register_testsuite_ns_sfn_interface, 0, 0, 0},
#endif

#ifdef TEST_NS_PS
    {&register_testsuite_ns_psa_ps_interface, 0, 0, 0},
#endif

#ifdef TEST_NS_ITS
    /* Non-secure ITS test cases */
    {&register_testsuite_ns_psa_its_interface, 0, 0, 0},
#endif

#ifdef TEST_NS_CRYPTO
    /* Non-secure Crypto test cases */
    {&register_testsuite_ns_crypto_interface, 0, 0, 0},
#endif

#ifdef TEST_NS_PLATFORM
    /* Non-secure platform service test cases */
    {&register_testsuite_ns_platform_interface, 0, 0, 0},
#endif

#ifdef TEST_NS_ATTESTATION
    /* Non-secure initial attestation service test cases */
    {&register_testsuite_ns_attestation_interface, 0, 0, 0},
#endif

#ifdef TEST_NS_QCBOR
    /* Non-secure QCBOR library test cases */
    {&register_testsuite_ns_qcbor, 0, 0, 0},
#endif

#ifdef TEST_NS_T_COSE
    /* Non-secure T_COSE library test cases */
    {&register_testsuite_ns_t_cose, 0, 0, 0},
#endif

#ifdef TEST_NS_AUDIT
    /* Non-secure Audit Logging test cases */
    {&register_testsuite_ns_audit_interface, 0, 0, 0},
#endif

#ifdef TEST_NS_FWU
    /* Non-secure Firmware Update test cases */
    {&register_testsuite_ns_psa_fwu_interface, 0, 0, 0},
#endif

#ifdef TEST_NS_MULTI_CORE
    /* Multi-core topology test cases */
    {&register_testsuite_multi_core_ns_interface, 0, 0, 0},
#endif

#ifdef EXTRA_NS_TEST_SUITE
    /* Non-secure extra test cases */
    {&register_testsuite_extra_ns_interface, 0, 0, 0},
#endif

#ifdef TFM_FUZZER_TOOL_TESTS
    {&register_testsuite_tf_fuzz_test, 0, 0, 0},
#endif /* TFM_FUZZER_TOOL_TESTS */

#ifdef TEST_NS_MANAGE_NSID
    {&register_testsuite_nsid_test, 0, 0, 0},
#endif /* TEST_NS_MANAGE_NSID */

#if defined(TEST_NS_SLIH_IRQ) || defined(TEST_NS_FLIH_IRQ)
    {&register_testsuite_irq_test, 0, 0, 0},
#endif

#ifdef TEST_NS_FPU
    {&register_testsuite_ns_fpu_interface, 0, 0, 0},
#endif

#ifdef TEST_NS_VT
    /* Non-secure VT test cases */
    {&register_testsuite_ns_psa_vt_interface, 0, 0, 0},
#endif


    /* End of test suites */
    {0, 0, 0, 0}
};

volatile int g_test_suite_flag = 0;

uint32_t test_suite_is_testing(void)
{
	return g_test_suite_flag;
}

#define REG_GET(a)    *(volatile uint32_t *)(a)
#define REG_SET(a, v) *(volatile uint32_t *)(a) = (v)
#define R(i)      (0x54000200 + (i << 2))
#define B(i)      (1 << (i))

//for caict test
uint64_t get_32k_timer(void)
{
        uint32_t low = REG_GET(R(3));
        uint32_t high = REG_GET(R(10));
        return (low + (high << 32));
}

static void init_time(void)
{
        uint32_t v = REG_GET(R(0));

        REG_SET(R(0), B(0));
        volatile uint32_t cnt = 1000000;
        while(--cnt);

        REG_SET(R(0), 0);

        v = REG_GET(R(0)); 
        REG_SET(R(0), (v | B(6)));

        REG_SET(R(1), 0xFFFFFFFF);
        REG_SET(R(6), 0xFFFFFFFF);

        v = REG_GET(R(0));
        v &= ~B(1);
        REG_SET(R(0), v);
}

extern int dubhe_driver_init(unsigned long dhb_base_addr);
enum test_suite_err_t start_integ_test(void)
{
	enum test_suite_err_t ret;

	dubhe_driver_init(0x5b110000);

	g_test_suite_flag = 2;
    ret = integ_test("Non-secure", test_suites);
	g_test_suite_flag = 0;

	return ret;
}

/* Service stand-in for NS tests. To be called from a non-secure context */
enum test_suite_err_t tfm_non_secure_client_run_tests(void)
{
    return start_integ_test();
}
