/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "ipc_s_tests.h"
#include "psa/client.h"
#include "psa_manifest/sid.h"
#include "test_framework.h"
#include "client_api_tests.h"

/* List of tests */
static void tfm_ipc_test_1001(struct test_result_t *ret);
static void tfm_ipc_test_1002(struct test_result_t *ret);
static void tfm_ipc_test_1004(struct test_result_t *ret);
static void tfm_ipc_test_1006(struct test_result_t *ret);

#ifdef TFM_IPC_ISOLATION_2_TEST_READ_ONLY_MEM
static void tfm_ipc_test_1007(struct test_result_t *ret);
#endif

#ifdef TFM_IPC_ISOLATION_2_APP_ACCESS_PSA_MEM
static void tfm_ipc_test_1008(struct test_result_t *ret);
#endif

#ifdef TFM_IPC_ISOLATION_2_MEM_CHECK
static void tfm_ipc_test_1009(struct test_result_t *ret);
#endif

#ifdef TFM_IPC_ISOLATION_3_RETRIEVE_APP_MEM
static void tfm_ipc_test_1011(struct test_result_t *ret);
#endif

static void tfm_ipc_test_1012(struct test_result_t *ret);

static struct test_t ipc_veneers_tests[] = {
    {&tfm_ipc_test_1001, "TFM_S_IPC_TEST_1001",
     "Get PSA framework version"},
    {&tfm_ipc_test_1002, "TFM_S_IPC_TEST_1002",
     "Get version of an RoT Service"},
    {&tfm_ipc_test_1004, "TFM_S_IPC_TEST_1004",
     "Request connection-based RoT Service"},
    {&tfm_ipc_test_1006, "TFM_S_IPC_TEST_1006",
     "Call PSA RoT access APP RoT memory test service"},
#ifdef TFM_IPC_ISOLATION_2_TEST_READ_ONLY_MEM
    {&tfm_ipc_test_1007, "TFM_S_IPC_TEST_1007",
     "Call PSA RoT access APP RoT readonly memory test service"},
#endif
#ifdef TFM_IPC_ISOLATION_2_APP_ACCESS_PSA_MEM
    {&tfm_ipc_test_1008, "TFM_S_IPC_TEST_1008",
     "Call APP RoT access PSA RoT memory test service"},
#endif
#ifdef TFM_IPC_ISOLATION_2_MEM_CHECK
    {&tfm_ipc_test_1009, "TFM_S_IPC_TEST_1009",
     "Call APP RoT memory check test service"},
#endif
#ifdef TFM_IPC_ISOLATION_3_RETRIEVE_APP_MEM
    {&tfm_ipc_test_1011, "TFM_S_IPC_TEST_1011",
     "Call APP RoT access another APP RoT memory test service"},
#endif
    {&tfm_ipc_test_1012, "TFM_S_IPC_TEST_1012",
     "Request stateless service"},
};

void register_testsuite_s_ipc_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(ipc_veneers_tests) / sizeof(ipc_veneers_tests[0]));

    set_testsuite("IPC secure interface test (TFM_S_IPC_TEST_1XXX)",
                  ipc_veneers_tests, list_size, p_test_suite);
}

/**
 * \brief Retrieve the version of the PSA Framework API.
 *
 * \note This is a functional test only and doesn't
 *       mean to test all possible combinations of
 *       input parameters and return values.
 */
static void tfm_ipc_test_1001(struct test_result_t *ret)
{
    psa_framework_version_test(ret);
}

/**
 * \brief Retrieve the version of an RoT Service.
 */
static void tfm_ipc_test_1002(struct test_result_t *ret)
{
    psa_version_test(IPC_SERVICE_TEST_BASIC_SID, ret);
}

/**
 * \brief Call a Connection-based RoT Service.
 */
static void tfm_ipc_test_1004(struct test_result_t *ret)
{
    psa_handle_t handle;

    handle = psa_connect(IPC_SERVICE_TEST_BASIC_SID,
                         IPC_SERVICE_TEST_BASIC_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        TEST_FAIL("Connection to service failed!\r\n");
        return;
    }

    request_rot_service_test(handle, ret);

    psa_close(handle);
}

/**
 * \brief Call IPC_CLIENT_TEST_PSA_ACCESS_APP_MEM_SID RoT Service
 *  to run the IPC PSA access APP mem test.
 */
static void tfm_ipc_test_1006(struct test_result_t *ret)
{
    psa_handle_t handle;
    psa_status_t status;
    int test_result;
    struct psa_outvec outvecs[1] = {{&test_result, sizeof(test_result)}};

    handle = psa_connect(IPC_CLIENT_TEST_PSA_ACCESS_APP_MEM_SID,
                         IPC_CLIENT_TEST_PSA_ACCESS_APP_MEM_VERSION);
    if (handle > 0) {
        TEST_LOG("Connect success!\r\n");
    } else {
        TEST_LOG("The RoT Service has refused the connection!\r\n");
        ret->val = TEST_FAILED;
        return;
    }

    status = psa_call(handle, PSA_IPC_CALL, NULL, 0, outvecs, 1);
    if (status >= 0) {
        TEST_LOG("Call success!\r\n");
        if (test_result > 0) {
            ret->val = TEST_PASSED;
        } else {
            ret->val = TEST_FAILED;
        }
    } else {
        TEST_LOG("Call failed!\r\n");
        ret->val = TEST_FAILED;
    }

    psa_close(handle);
}

#ifdef TFM_IPC_ISOLATION_2_TEST_READ_ONLY_MEM
/**
 * \brief Call IPC_CLIENT_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_SID RoT Service
 *  to run the IPC PSA access APP readonly mem test.
 */
static void tfm_ipc_test_1007(struct test_result_t *ret)
{
    psa_handle_t handle;
    psa_status_t status;
    int test_result;
    struct psa_outvec outvecs[1] = {{&test_result, sizeof(test_result)}};

    handle = psa_connect(IPC_CLIENT_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_SID,
                         IPC_CLIENT_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_VERSION);
    if (handle > 0) {
        TEST_LOG("Connect success!\r\n");
    } else {
        TEST_LOG("The RoT Service has refused the connection!\r\n");
        ret->val = TEST_FAILED;
        return;
    }

    status = psa_call(handle, PSA_IPC_CALL, NULL, 0, outvecs, 1);
    if (status == PSA_SUCCESS) {
        ret->val = TEST_PASSED;
    } else {
        ret->val = TEST_FAILED;
    }

    psa_close(handle);
}
#endif

#ifdef TFM_IPC_ISOLATION_2_APP_ACCESS_PSA_MEM
/**
 * \brief Call IPC_CLIENT_TEST_APP_ACCESS_PSA_MEM_SID RoT Service
 *  to run the IPC APP access PSA mem test.
 */
static void tfm_ipc_test_1008(struct test_result_t *ret)
{
    psa_handle_t handle;
    int test_result;
    struct psa_outvec outvecs[1] = {{&test_result, sizeof(test_result)}};

    handle = psa_connect(IPC_CLIENT_TEST_APP_ACCESS_PSA_MEM_SID,
                         IPC_CLIENT_TEST_APP_ACCESS_PSA_MEM_VERSION);
    if (handle > 0) {
        TEST_LOG("Connect success!\r\n");
    } else {
        TEST_LOG("The RoT Service has refused the connection!\r\n");
        ret->val = TEST_FAILED;
        return;
    }

    psa_call(handle, PSA_IPC_CALL, NULL, 0, outvecs, 1);

    /* The system should panic in psa_call. If runs here, the test fails. */
    ret->val = TEST_FAILED;
    psa_close(handle);
}
#endif

#ifdef TFM_IPC_ISOLATION_2_MEM_CHECK
/**
 * \brief Call IPC_CLIENT_TEST_MEM_CHECK_SID RoT Service
 *  to run the IPC mem check test.
 */
static void tfm_ipc_test_1009(struct test_result_t *ret)
{
    psa_handle_t handle;
    int test_result;
    struct psa_outvec outvecs[1] = {{&test_result, sizeof(test_result)}};

    handle = psa_connect(IPC_CLIENT_TEST_MEM_CHECK_SID,
                         IPC_CLIENT_TEST_MEM_CHECK_VERSION);
    if (handle > 0) {
        TEST_LOG("Connect success!\r\n");
    } else {
        TEST_LOG("The RoT Service has refused the connection!\r\n");
        ret->val = TEST_FAILED;
        return;
    }

    psa_call(handle, PSA_IPC_CALL, NULL, 0, outvecs, 1);

    /* The system should panic in psa_call. If runs here, the test fails. */
    ret->val = TEST_FAILED;
    psa_close(handle);
}
#endif

#ifdef TFM_IPC_ISOLATION_3_RETRIEVE_APP_MEM
/**
 * \brief Call IPC_CLIENT_TEST_RETRIEVE_APP_MEM_SID RoT Service
 * to run the ARoT access another ARoT mem test.
 */
static void tfm_ipc_test_1011(struct test_result_t *ret)
{
    psa_handle_t handle;
    psa_status_t status;
    int test_result;
    struct psa_outvec outvecs[1] = {{&test_result, sizeof(test_result)}};

    handle = psa_connect(IPC_CLIENT_TEST_RETRIEVE_APP_MEM_SID,
                         IPC_CLIENT_TEST_RETRIEVE_APP_MEM_VERSION);
    if (handle > 0) {
        TEST_LOG("Connect success!\r\n");
    } else {
        TEST_LOG("The RoT Service has refused the connection!\r\n");
        ret->val = TEST_FAILED;
        return;
    }

    status = psa_call(handle, PSA_IPC_CALL, NULL, 0, outvecs, 1);
    if (status == PSA_SUCCESS) {
        ret->val = TEST_PASSED;
    } else {
        ret->val = TEST_FAILED;
    }

    psa_close(handle);
}
#endif

/**
 * \brief Accessing a stateless service
 *
 * \note Accessing stateless service from a secure partition.
 */
static void tfm_ipc_test_1012(struct test_result_t *ret)
{
    request_rot_service_test(IPC_SERVICE_TEST_STATELESS_ROT_HANDLE, ret);
}
