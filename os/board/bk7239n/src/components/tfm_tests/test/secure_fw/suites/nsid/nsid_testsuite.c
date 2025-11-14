/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "test_framework.h"
#include "tfm_ns_client_ext.h"
#include "nsid_svc_handler.h"
#include "tfm_nsid_manager.h"

/*
 * The following macros are defined for white-box test only.
 * Some of them are duplicated from secure side.
 * It is unnecessary to export these definitions to NS side in actual scenarios.
 */
#define MAKE_NS_CLIENT_TOKEN(tid, gid, idx)                         \
                             (uint32_t)((((uint32_t)tid & 0xff)     \
                             | (((uint32_t)gid & 0xff) << 8)        \
                             | (((uint32_t)idx & 0xff) << 16))      \
                             & 0x00ffffff)

#define VALID_INDEX                   0x00
#define VALID_GROUP_ID                0x00
#define VALID_THREAD_ID_MIN           0x00
#define VALID_THREAD_ID_MAX           UINT8_MAX
#define VALID_NSID_MAX                ((int32_t)-1)
#define VALID_NSID_MIN                INT32_MIN
#define INVALID_INDEX                 0x01
#define INVALID_GROUP_ID              0x01
#define INVALID_RESERVED_TOKEN_BIT    0x10000000
#define TFM_NS_CONTEXT_MAX            1

static uint8_t thread_id = 0x20;
static int32_t nsid = -500;

/*
 * This func is to be called at the beginning of test cases.
 * The current active token needs to be saved first
 * before calling ns_client_ext APIs through NS SVC.
 * Values of NSID and Thread ID are changed to make sure
 * every test case could be assigned with different tokens.
 */
static void nsid_test_case_init(struct test_result_t *ret)
{
    if (nsid_save_ctx_svc(current_active_token) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("SAVE fail when saving test thread ctx\r\n");
    }

    nsid--;
    thread_id++;

    return;
}

/*
 * This func is to be called at the end of test cases
 * to re-load the saved token at the beginning of the test case.
 */
static void nsid_test_case_finish(struct test_result_t *ret)
{
    if (nsid_load_ctx_svc(current_active_token,
                          nsid_mgr_query_nsid(current_active_token))
        != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("LOAD fail when re-loading test thread ctx\r\n");
    }
    return;
}

/* Initialize with valid params */
static void tfm_nsid_test_case_1(struct test_result_t *ret)
{
    nsid_test_case_init(ret);

    if (nsid_ext_init_svc(0) != TFM_NS_CONTEXT_MAX) {
        TEST_FAIL("INITIALIZE shall return TFM_NS_CONTEXT_MAX\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_ext_init_svc(1) != 1) {
        TEST_FAIL("INITIALIZE shall return 1\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_ext_init_svc(TFM_NS_CONTEXT_MAX + 1) != TFM_NS_CONTEXT_MAX) {
        TEST_FAIL("INITIALIZE shall return TFM_NS_CONTEXT_MAX\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    ret->val = TEST_PASSED;

    nsid_test_case_finish(ret);
}

/* Normal full sequence */
static void tfm_nsid_test_case_2(struct test_result_t *ret)
{
    uint32_t token;

    nsid_test_case_init(ret);

    token = nsid_acquire_ctx_svc(VALID_GROUP_ID, thread_id);

    if (token != MAKE_NS_CLIENT_TOKEN(thread_id, VALID_GROUP_ID, VALID_INDEX)) {
        TEST_FAIL("ACQUIRE shall return a valid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_load_ctx_svc(token, nsid) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("LOAD shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_save_ctx_svc(token) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("SAVE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    ret->val = TEST_PASSED;

    nsid_test_case_finish(ret);
}

/* Normal acquire-release sequence */
static void tfm_nsid_test_case_3(struct test_result_t *ret)
{
    uint32_t token;

    nsid_test_case_init(ret);

    token = nsid_acquire_ctx_svc(VALID_GROUP_ID, thread_id);

    if (token != MAKE_NS_CLIENT_TOKEN(thread_id, VALID_GROUP_ID, VALID_INDEX)) {
        TEST_FAIL("ACQUIRE shall return a valid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    ret->val = TEST_PASSED;

    nsid_test_case_finish(ret);
}

/*  Normal acquire-load-release sequence */
static void tfm_nsid_test_case_4(struct test_result_t *ret)
{
    uint32_t token;

    nsid_test_case_init(ret);

    token = nsid_acquire_ctx_svc(VALID_GROUP_ID, thread_id);

    if (token != MAKE_NS_CLIENT_TOKEN(thread_id, VALID_GROUP_ID, VALID_INDEX)) {
        TEST_FAIL("ACQUIRE shall return a valid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_load_ctx_svc(token, nsid) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("LOAD shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    ret->val = TEST_PASSED;

    nsid_test_case_finish(ret);
}

/* Switch contexts */
static void tfm_nsid_test_case_5(struct test_result_t *ret)
{
    uint32_t token_a;
    uint32_t token_b;

    nsid_test_case_init(ret);

    token_a = nsid_acquire_ctx_svc(VALID_GROUP_ID, thread_id);
    token_b = nsid_acquire_ctx_svc(VALID_GROUP_ID, thread_id + 1);

    if (token_a != MAKE_NS_CLIENT_TOKEN(thread_id, \
                                        VALID_GROUP_ID, \
                                        VALID_INDEX) || \
        token_b != MAKE_NS_CLIENT_TOKEN(thread_id + 1, \
                                        VALID_GROUP_ID, \
                                        VALID_INDEX)) {
        TEST_FAIL("ACQUIRE shall return a valid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_load_ctx_svc(token_a, nsid) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("LOAD shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_save_ctx_svc(token_a) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("SAVE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_load_ctx_svc(token_b, nsid) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("LOAD shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_save_ctx_svc(token_b) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("SAVE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token_a) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token_b) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    ret->val = TEST_PASSED;

    nsid_test_case_finish(ret);
}

/* Release inactive context */
static void tfm_nsid_test_case_6(struct test_result_t *ret)
{
    uint32_t token_a;
    uint32_t token_b;

    nsid_test_case_init(ret);

    token_a = nsid_acquire_ctx_svc(VALID_GROUP_ID, thread_id);
    token_b = nsid_acquire_ctx_svc(VALID_GROUP_ID, thread_id + 1);

    if (token_a != MAKE_NS_CLIENT_TOKEN(thread_id, \
                                        VALID_GROUP_ID,
                                        VALID_INDEX) || \
        token_b != MAKE_NS_CLIENT_TOKEN(thread_id + 1, \
                                        VALID_GROUP_ID, \
                                        VALID_INDEX)) {
        TEST_FAIL("ACQUIRE shall return a valid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_load_ctx_svc(token_a, nsid) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("LOAD shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token_b) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token_a) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    ret->val = TEST_PASSED;

    nsid_test_case_finish(ret);
}

/* Max NSID */
static void tfm_nsid_test_case_7(struct test_result_t *ret)
{
    uint32_t token;

    nsid_test_case_init(ret);

    token = nsid_acquire_ctx_svc(VALID_GROUP_ID, thread_id);

    if (token != MAKE_NS_CLIENT_TOKEN(thread_id, VALID_GROUP_ID, VALID_INDEX)) {
        TEST_FAIL("ACQUIRE shall return a valid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_load_ctx_svc(token, VALID_NSID_MAX)
        != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("LOAD shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_save_ctx_svc(token) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("SAVE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    ret->val = TEST_PASSED;

    nsid_test_case_finish(ret);
}

/* Min NSID */
static void tfm_nsid_test_case_8(struct test_result_t *ret)
{
    uint32_t token;

    nsid_test_case_init(ret);

    token = nsid_acquire_ctx_svc(VALID_GROUP_ID, thread_id);

    if (token != MAKE_NS_CLIENT_TOKEN(thread_id, VALID_GROUP_ID, VALID_INDEX)) {
        TEST_FAIL("ACQUIRE shall return a valid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_load_ctx_svc(token, VALID_NSID_MIN)
        != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("LOAD shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_save_ctx_svc(token) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("SAVE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    ret->val = TEST_PASSED;

    nsid_test_case_finish(ret);
}

/* Min TID */
static void tfm_nsid_test_case_9(struct test_result_t *ret)
{
    uint32_t token;

    nsid_test_case_init(ret);

    token = nsid_acquire_ctx_svc(VALID_GROUP_ID, VALID_THREAD_ID_MIN);

    if (token != MAKE_NS_CLIENT_TOKEN(0x00, VALID_GROUP_ID, VALID_INDEX)) {
        TEST_FAIL("ACQUIRE shall return a valid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_load_ctx_svc(token, nsid) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("LOAD shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_save_ctx_svc(token) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("SAVE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    ret->val = TEST_PASSED;

    nsid_test_case_finish(ret);
}

/* Max TID */
static void tfm_nsid_test_case_10(struct test_result_t *ret)
{
    uint32_t token;

    nsid_test_case_init(ret);

    token = nsid_acquire_ctx_svc(VALID_GROUP_ID, VALID_THREAD_ID_MAX);

    if (token != MAKE_NS_CLIENT_TOKEN(0xFF, VALID_GROUP_ID, VALID_INDEX)) {
        TEST_FAIL("ACQUIRE shall return a valid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_load_ctx_svc(token, nsid) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("LOAD shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_save_ctx_svc(token) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("SAVE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    ret->val = TEST_PASSED;

    nsid_test_case_finish(ret);
}

/* Load Thread B without saving Thread A */
static void tfm_nsid_test_case_11(struct test_result_t *ret)
{
    uint32_t token_a;
    uint32_t token_b;

    nsid_test_case_init(ret);

    token_a = nsid_acquire_ctx_svc(VALID_GROUP_ID, thread_id);
    token_b = nsid_acquire_ctx_svc(VALID_GROUP_ID, thread_id + 1);

    if (token_a != MAKE_NS_CLIENT_TOKEN(thread_id, \
                                        VALID_GROUP_ID, \
                                        VALID_INDEX) || \
        token_b != MAKE_NS_CLIENT_TOKEN(thread_id + 1, \
                                        VALID_GROUP_ID, \
                                        VALID_INDEX)) {
        TEST_FAIL("ACQUIRE shall return a valid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_load_ctx_svc(token_a, nsid) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("LOAD shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_load_ctx_svc(token_b, nsid) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("LOAD shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token_a) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token_b) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    ret->val = TEST_PASSED;

    nsid_test_case_finish(ret);
}

/* Load with non-negative NSID */
static void tfm_nsid_test_case_12(struct test_result_t *ret)
{
    uint32_t token;

    nsid_test_case_init(ret);

    thread_id++;

    token = nsid_acquire_ctx_svc(VALID_GROUP_ID, thread_id);

    if (token != MAKE_NS_CLIENT_TOKEN(thread_id, VALID_GROUP_ID, VALID_INDEX)) {
        TEST_FAIL("ACQUIRE shall return a valid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_load_ctx_svc(token, 0) != TFM_NS_CLIENT_ERR_INVALID_NSID) {
        TEST_FAIL("LOAD shall fail with non-negative NSID\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_load_ctx_svc(token, 1) != TFM_NS_CLIENT_ERR_INVALID_NSID) {
        TEST_FAIL("LOAD shall fail with non-negative NSID\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_load_ctx_svc(token, 50) != TFM_NS_CLIENT_ERR_INVALID_NSID) {
        TEST_FAIL("LOAD shall fail with non-negative NSID\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    nsid_test_case_finish(ret);

    ret->val = TEST_PASSED;
}

/* Load with invalid token */
static void tfm_nsid_test_case_13(struct test_result_t *ret)
{
    uint32_t token;

    nsid_test_case_init(ret);

    token = nsid_acquire_ctx_svc(VALID_GROUP_ID, thread_id);

    if (token != MAKE_NS_CLIENT_TOKEN(thread_id, VALID_GROUP_ID, VALID_INDEX)) {
        TEST_FAIL("ACQUIRE shall return a valid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_load_ctx_svc(TFM_NS_CLIENT_INVALID_TOKEN, nsid)
        != TFM_NS_CLIENT_ERR_INVALID_TOKEN) {
        TEST_FAIL("LOAD shall fail with invalid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    ret->val = TEST_PASSED;

    nsid_test_case_finish(ret);
}

/* Save with invalid token */
static void tfm_nsid_test_case_14(struct test_result_t *ret)
{
    uint32_t token;

    nsid_test_case_init(ret);

    token = nsid_acquire_ctx_svc(VALID_GROUP_ID, thread_id);

    if (token != MAKE_NS_CLIENT_TOKEN(thread_id, VALID_GROUP_ID, VALID_INDEX)) {
        TEST_FAIL("ACQUIRE shall return a valid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_load_ctx_svc(token, nsid) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("LOAD shall pass with valid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_save_ctx_svc(TFM_NS_CLIENT_INVALID_TOKEN)
        != TFM_NS_CLIENT_ERR_INVALID_TOKEN) {
        TEST_FAIL("SAVE shall fail with invalid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    ret->val = TEST_PASSED;

    nsid_test_case_finish(ret);
}

/* Release with invalid token */
static void tfm_nsid_test_case_15(struct test_result_t *ret)
{
    uint32_t token;

    nsid_test_case_init(ret);

    token = nsid_acquire_ctx_svc(VALID_GROUP_ID, thread_id);

    if (token != MAKE_NS_CLIENT_TOKEN(thread_id, VALID_GROUP_ID, VALID_INDEX)) {
        TEST_FAIL("ACQUIRE shall return a valid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(TFM_NS_CLIENT_INVALID_TOKEN)
        != TFM_NS_CLIENT_ERR_INVALID_TOKEN) {
        TEST_FAIL("RELEASE shall fail with invalid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    ret->val = TEST_PASSED;

    nsid_test_case_finish(ret);
}

/* Acquire with undefault GID */
static void tfm_nsid_test_case_16(struct test_result_t *ret)
{
    nsid_test_case_init(ret);

    if (nsid_acquire_ctx_svc(0x01, INVALID_GROUP_ID) \
        != TFM_NS_CLIENT_INVALID_TOKEN) {
        TEST_FAIL("ACQUIRE shall fail with invalid GID\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_acquire_ctx_svc(0x50, INVALID_GROUP_ID) \
        != TFM_NS_CLIENT_INVALID_TOKEN) {
        TEST_FAIL("ACQUIRE shall fail with invalid GID\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_acquire_ctx_svc(0xFF, INVALID_GROUP_ID) \
        != TFM_NS_CLIENT_INVALID_TOKEN) {
        TEST_FAIL("ACQUIRE shall fail with invalid GID\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    ret->val = TEST_PASSED;

    nsid_test_case_finish(ret);
}

/* Load, save and release with token containing wrong info */
static void tfm_nsid_test_case_17(struct test_result_t *ret)
{
    uint32_t token;

    nsid_test_case_init(ret);

    token = nsid_acquire_ctx_svc(VALID_GROUP_ID, thread_id);

    if (token != MAKE_NS_CLIENT_TOKEN(thread_id, VALID_GROUP_ID, VALID_INDEX)) {
        TEST_FAIL("ACQUIRE shall return a valid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_load_ctx_svc(MAKE_NS_CLIENT_TOKEN(thread_id, \
                                               INVALID_GROUP_ID, \
                                               VALID_INDEX), nsid) \
        != TFM_NS_CLIENT_ERR_INVALID_TOKEN) {
        TEST_FAIL("LOAD shall fail with token with wrong GID\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_load_ctx_svc(MAKE_NS_CLIENT_TOKEN(thread_id, \
                                               VALID_GROUP_ID, \
                                               INVALID_INDEX), nsid) \
        != TFM_NS_CLIENT_ERR_INVALID_TOKEN) {
        TEST_FAIL("LOAD shall fail with token with wrong index\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_load_ctx_svc(MAKE_NS_CLIENT_TOKEN(thread_id, \
                                               VALID_GROUP_ID, \
                                               VALID_INDEX) \
                          | INVALID_RESERVED_TOKEN_BIT, nsid) \
        != TFM_NS_CLIENT_ERR_INVALID_TOKEN) {
        TEST_FAIL("LOAD shall fail with token with wrong reserved bits\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_load_ctx_svc(token, nsid) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("LOAD shall pass with valid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_save_ctx_svc(MAKE_NS_CLIENT_TOKEN(0xF0, \
                                               VALID_GROUP_ID, \
                                               VALID_INDEX)) \
        != TFM_NS_CLIENT_ERR_INVALID_TOKEN) {
        TEST_FAIL("SAVE shall fail with token with wrong TID\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_save_ctx_svc(MAKE_NS_CLIENT_TOKEN(thread_id, \
                                               INVALID_GROUP_ID, \
                                               VALID_INDEX)) \
        != TFM_NS_CLIENT_ERR_INVALID_TOKEN) {
        TEST_FAIL("SAVE shall fail with token with wrong GID\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_save_ctx_svc(MAKE_NS_CLIENT_TOKEN(thread_id, \
                                               VALID_GROUP_ID, \
                                               INVALID_INDEX)) \
        != TFM_NS_CLIENT_ERR_INVALID_TOKEN) {
        TEST_FAIL("SAVE shall fail with token with wrong index\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_save_ctx_svc(MAKE_NS_CLIENT_TOKEN(thread_id, \
                                               VALID_GROUP_ID, \
                                               VALID_INDEX) \
                          | INVALID_RESERVED_TOKEN_BIT) \
        != TFM_NS_CLIENT_ERR_INVALID_TOKEN) {
        TEST_FAIL("SAVE shall fail with token with wrong reserved bits\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_save_ctx_svc(token) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("SAVE shall pass with valid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(MAKE_NS_CLIENT_TOKEN(thread_id, \
                                                  INVALID_GROUP_ID, \
                                                  VALID_INDEX)) \
        != TFM_NS_CLIENT_ERR_INVALID_TOKEN) {
        TEST_FAIL("RELEASE shall fail with token with wrong GID\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(MAKE_NS_CLIENT_TOKEN(thread_id, \
                                                  VALID_GROUP_ID, \
                                                  INVALID_INDEX)) \
        != TFM_NS_CLIENT_ERR_INVALID_TOKEN) {
        TEST_FAIL("RELEASE shall fail with token with wrong index\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(MAKE_NS_CLIENT_TOKEN(thread_id, \
                                               VALID_GROUP_ID, \
                                               VALID_INDEX) \
                          | INVALID_RESERVED_TOKEN_BIT) \
        != TFM_NS_CLIENT_ERR_INVALID_TOKEN) {
        TEST_FAIL("RELEASE shall fail with token with wrong reserved bits\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    ret->val = TEST_PASSED;

    nsid_test_case_finish(ret);
}

/* Save without loading */
static void tfm_nsid_test_case_18(struct test_result_t *ret)
{
    uint32_t token;

    nsid_test_case_init(ret);

    token = nsid_acquire_ctx_svc(VALID_GROUP_ID, thread_id);

    if (token != MAKE_NS_CLIENT_TOKEN(thread_id, VALID_GROUP_ID, VALID_INDEX)) {
        TEST_FAIL("ACQUIRE shall return a valid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_save_ctx_svc(token) != TFM_NS_CLIENT_ERR_INVALID_TOKEN) {
        TEST_FAIL("SAVE shall fail when not loaded\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    ret->val = TEST_PASSED;

    nsid_test_case_finish(ret);
}

/* Save released context */
static void tfm_nsid_test_case_19(struct test_result_t *ret)
{
    uint32_t token;

    nsid_test_case_init(ret);

    token = nsid_acquire_ctx_svc(VALID_GROUP_ID, thread_id);

    if (token != MAKE_NS_CLIENT_TOKEN(thread_id, VALID_GROUP_ID, VALID_INDEX)) {
        TEST_FAIL("ACQUIRE shall return a valid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_save_ctx_svc(token) != TFM_NS_CLIENT_ERR_INVALID_TOKEN) {
        TEST_FAIL("SAVE shall fail with released token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    ret->val = TEST_PASSED;

    nsid_test_case_finish(ret);
}

/* Save inactive context */
static void tfm_nsid_test_case_20(struct test_result_t *ret)
{
    uint32_t token_a;
    uint32_t token_b;

    nsid_test_case_init(ret);

    token_a = nsid_acquire_ctx_svc(VALID_GROUP_ID, thread_id);
    token_b = nsid_acquire_ctx_svc(VALID_GROUP_ID, thread_id + 1);

    if (token_a != MAKE_NS_CLIENT_TOKEN(thread_id, \
                                        VALID_GROUP_ID, \
                                        VALID_INDEX) || \
        token_b != MAKE_NS_CLIENT_TOKEN(thread_id + 1, \
                                        VALID_GROUP_ID, \
                                        VALID_INDEX)) {
        TEST_FAIL("ACQUIRE shall return a valid token\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_load_ctx_svc(token_a, nsid) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("LOAD shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_save_ctx_svc(token_b) != TFM_NS_CLIENT_ERR_INVALID_TOKEN) {
        TEST_FAIL("SAVE shall fail with inactive token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token_a) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (nsid_release_ctx_svc(token_b) != TFM_NS_CLIENT_ERR_SUCCESS) {
        TEST_FAIL("RELEASE shall success with valid token value\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    ret->val = TEST_PASSED;

    nsid_test_case_finish(ret);
}

/* Call in thread mode */
static void tfm_nsid_test_case_21(struct test_result_t *ret)
{
    uint32_t token;

    nsid_test_case_init(ret);

    if (tfm_nsce_init(0) != 0) {
        TEST_FAIL("INITIALIZE shall not success in thread mode\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    token = tfm_nsce_acquire_ctx(VALID_GROUP_ID, thread_id);

    if (token != TFM_NS_CLIENT_ERR_INVALID_ACCESS) {
        TEST_FAIL("ACQUIRE shall not success in thread mode\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (tfm_nsce_load_ctx(token, nsid) != TFM_NS_CLIENT_ERR_INVALID_ACCESS) {
        TEST_FAIL("LOAD shall not success in thread mode\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (tfm_nsce_save_ctx(token) != TFM_NS_CLIENT_ERR_INVALID_ACCESS) {
        TEST_FAIL("SAVE shall not success in thread mode\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    if (tfm_nsce_release_ctx(token) != TFM_NS_CLIENT_ERR_INVALID_ACCESS) {
        TEST_FAIL("RELEASE shall not success in thread mode\r\n");
        nsid_test_case_finish(ret);
        return;
    }

    ret->val = TEST_PASSED;

    nsid_test_case_finish(ret);
}

static struct test_t nsid_test_cases[] = {
    /* Normal test */
    {&tfm_nsid_test_case_1, "TFM_NS_NSID_TEST_1001",
     "NSID management initialize ctx pass"},
    {&tfm_nsid_test_case_2, "TFM_NS_NSID_TEST_1002",
     "NSID management normal full sequence pass"},
    {&tfm_nsid_test_case_3, "TFM_NS_NSID_TEST_1003",
     "NSID management acquire-release sequence pass"},
    {&tfm_nsid_test_case_4, "TFM_NS_NSID_TEST_1004",
     "NSID management acquire-load-release sequence pass"},
    {&tfm_nsid_test_case_5, "TFM_NS_NSID_TEST_1005",
     "NSID management switching contexts pass"},
    {&tfm_nsid_test_case_6, "TFM_NS_NSID_TEST_1006",
     "NSID management releasing inactive context pass"},
    /* Corner case test */
    {&tfm_nsid_test_case_7, "TFM_NS_NSID_TEST_1007",
     "NSID management pass with max valid NSID"},
    {&tfm_nsid_test_case_8, "TFM_NS_NSID_TEST_1008",
     "NSID management pass with min valid NSID"},
    {&tfm_nsid_test_case_9, "TFM_NS_NSID_TEST_1009",
     "NSID management pass with min TID"},
    {&tfm_nsid_test_case_10, "TFM_NS_NSID_TEST_1010",
     "NSID management pass with max TID"},
    {&tfm_nsid_test_case_11, "TFM_NS_NSID_TEST_1011",
     "NSID management pass when loading B without saving A"},
    /* Error test */
    /* Wrong params */
    {&tfm_nsid_test_case_12, "TFM_NS_NSID_TEST_1012",
     "NSID management loading fail with non-negative NSID"},
    {&tfm_nsid_test_case_13, "TFM_NS_NSID_TEST_1013",
     "NSID management fail when loading with invalid token"},
    {&tfm_nsid_test_case_14, "TFM_NS_NSID_TEST_1014",
     "NSID management fail when saving with invalid token"},
    {&tfm_nsid_test_case_15, "TFM_NS_NSID_TEST_1015",
     "NSID management fail when releasing with invalid token"},
    {&tfm_nsid_test_case_16, "TFM_NS_NSID_TEST_1016",
     "NSID management fail when acquiring with undefault GID"},
    {&tfm_nsid_test_case_17, "TFM_NS_NSID_TEST_1017",
     "NSID management fail with token containing wrong info"},
    /* Wrong sequence */
    {&tfm_nsid_test_case_18, "TFM_NS_NSID_TEST_1018",
     "NSID management fail when saving without loading"},
    {&tfm_nsid_test_case_19, "TFM_NS_NSID_TEST_1019",
     "NSID management fail when saving released token"},
    {&tfm_nsid_test_case_20, "TFM_NS_NSID_TEST_1020",
     "NSID management fail when saving inactive token"},
    /* Other tests */
    {&tfm_nsid_test_case_21, "TFM_NS_NSID_TEST_1021",
     "NSID management fail when called in thread mode"},
};

void register_testsuite_nsid_test(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(nsid_test_cases) / sizeof(nsid_test_cases[0]));

    set_testsuite("TFM NSID Test (TFM_NSID_TEST_1xxx)",
                  nsid_test_cases, list_size, p_test_suite);
}
