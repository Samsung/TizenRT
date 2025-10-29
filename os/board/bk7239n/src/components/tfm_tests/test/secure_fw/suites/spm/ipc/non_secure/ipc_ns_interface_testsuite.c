/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include "ipc_ns_tests.h"
#include "psa/client.h"
#include "psa/framework_feature.h"
#include "test_framework_helpers.h"
#include "psa_manifest/sid.h"
#include "client_api_tests.h"
#if PSA_FRAMEWORK_HAS_MM_IOVEC
#include "mmiovec_test.h"
#endif

/* List of tests */
static void tfm_ipc_test_1001(struct test_result_t *ret);
static void tfm_ipc_test_1002(struct test_result_t *ret);
static void tfm_ipc_test_1003(struct test_result_t *ret);
static void tfm_ipc_test_1004(struct test_result_t *ret);

static void tfm_ipc_test_1010(struct test_result_t *ret);

static void tfm_ipc_test_1012(struct test_result_t *ret);

#if PSA_FRAMEWORK_HAS_MM_IOVEC
static void tfm_ipc_test_1013(struct test_result_t *ret);

static void tfm_ipc_test_1014(struct test_result_t *ret);

static void tfm_ipc_test_1015(struct test_result_t *ret);
#endif

static struct test_t ipc_veneers_tests[] = {
    {&tfm_ipc_test_1001, "TFM_NS_IPC_TEST_1001",
     "Get PSA framework version"},
    {&tfm_ipc_test_1002, "TFM_NS_IPC_TEST_1002",
     "Get version of an RoT Service"},
    {&tfm_ipc_test_1003, "TFM_NS_IPC_TEST_1003",
     "Connect to an RoT Service"},
    {&tfm_ipc_test_1004, "TFM_NS_IPC_TEST_1004",
     "Request connection-based RoT Service"},
    {&tfm_ipc_test_1010, "TFM_NS_IPC_TEST_1010",
     "Test psa_call with the status of PSA_ERROR_PROGRAMMER_ERROR"},
    {&tfm_ipc_test_1012, "TFM_NS_IPC_TEST_1012",
     "Request stateless service"},
#if PSA_FRAMEWORK_HAS_MM_IOVEC
    {&tfm_ipc_test_1013, "TFM_NS_IPC_TEST_1013",
     "Mapping input vectors and unmapping them"},
    {&tfm_ipc_test_1014, "TFM_NS_IPC_TEST_1014",
     "Mapping output vectors and unmapping them"},
    {&tfm_ipc_test_1015, "TFM_NS_IPC_TEST_1015",
     "Mapping output vectors and not unmapping them"},
#endif
};

void register_testsuite_ns_ipc_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(ipc_veneers_tests) / sizeof(ipc_veneers_tests[0]));

    set_testsuite("IPC non-secure interface test (TFM_NS_IPC_TEST_1XXX)",
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
 * \brief Connect to an RoT Service by its SID.
 */
static void tfm_ipc_test_1003(struct test_result_t *ret)
{
    psa_handle_t handle;

    handle = psa_connect(IPC_SERVICE_TEST_BASIC_SID,
                         IPC_SERVICE_TEST_BASIC_VERSION);
    if (handle > 0) {
        TEST_LOG("Connect success!\r\n");
    } else {
        TEST_FAIL("The RoT Service has refused the connection!\r\n");
        return;
    }
    psa_close(handle);
    ret->val = TEST_PASSED;
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
 * \brief Call IPC_SERVICE_TEST_CLIENT_PREGRAMMER_ERROR RoT Service to
 *  test psa_call with the status of PSA_ERROR_PROGRAMMER_ERROR.
 */
static void tfm_ipc_test_1010(struct test_result_t *ret)
{
    psa_handle_t handle;
    psa_status_t status;
    handle = psa_connect(IPC_SERVICE_TEST_CLIENT_PROGRAMMER_ERROR_SID,
                         IPC_SERVICE_TEST_CLIENT_PROGRAMMER_ERROR_VERSION);
    if (handle > 0) {
        TEST_LOG("Connect success!\r\n");
    } else {
        TEST_LOG("The RoT Service has refused the connection!\r\n");
        ret->val = TEST_FAILED;
        return;
    }
    status = psa_call(handle, PSA_IPC_CALL, NULL, 0, NULL, 0);
    if (status == PSA_ERROR_PROGRAMMER_ERROR) {
        TEST_LOG("The first time call success!\r\n");
    } else {
        TEST_LOG("The first time call failed!\r\n");
        ret->val = TEST_FAILED;
    }
    status = psa_call(handle, PSA_IPC_CALL, NULL, 0, NULL, 0);
    if (status == PSA_ERROR_PROGRAMMER_ERROR) {
        TEST_LOG("The second time call success!\r\n");
    } else {
        TEST_LOG("The second time call failed!\r\n");
        ret->val = TEST_FAILED;
    }

    psa_close(handle);
}

/**
 * \brief Accessing a stateless service
 *
 * \note Accessing stateless service from non-secure client.
 */
static void tfm_ipc_test_1012(struct test_result_t *ret)
{
    psa_handle_t handle;

    /* Connecting to a stateless service should fail. */
    handle = psa_connect(IPC_SERVICE_TEST_STATELESS_ROT_SID,
                         IPC_SERVICE_TEST_STATELESS_ROT_VERSION);
    if (PSA_HANDLE_IS_VALID(handle)) {
        TEST_FAIL("Connecting to stateless service test should fail.\r\n");
        return;
    }

    request_rot_service_test(IPC_SERVICE_TEST_STATELESS_ROT_HANDLE, ret);
}

#if PSA_FRAMEWORK_HAS_MM_IOVEC

/**
 * \brief Mapping input vectors and unmapping them.
 *
 * \note Test psa_map_invec() and psa_unmap_invec() functionality.
 */
static void tfm_ipc_test_1013(struct test_result_t *ret)
{
    invec_map_unmap_test(ret, IPC_SERVICE_TEST_MMIOVEC_HANDLE);
}

/**
 * \brief Mapping output vectors and unmapping them.
 *
 * \note Test psa_map_outvec() and psa_unmap_outvec() functionality.
 */
static void tfm_ipc_test_1014(struct test_result_t *ret)
{
    outvec_map_unmap_test(ret, IPC_SERVICE_TEST_MMIOVEC_HANDLE);
}

/**
 * \brief Mapping output vectors and not unmapping them.
 *
 * \note RoT services map outvecs and does not unmap outvecs, the service caller
 *       should get a zero out length.
 */
static void tfm_ipc_test_1015(struct test_result_t *ret)
{
    outvec_map_only_test(ret, IPC_SERVICE_TEST_MMIOVEC_HANDLE);
}

#endif
