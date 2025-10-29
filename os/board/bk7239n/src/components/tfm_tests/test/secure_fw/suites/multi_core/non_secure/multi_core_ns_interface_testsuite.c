/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include "os_wrapper/mutex.h"
#include "os_wrapper/thread.h"
#include "os_wrapper/tick.h"
#include "psa/client.h"
#include "psa/internal_trusted_storage.h"
#include "psa_manifest/sid.h"
#include "test_framework_helpers.h"
#include "tfm_ns_mailbox_test.h"

#if (NUM_MAILBOX_QUEUE_SLOT > 1)
/* Max number of child threads for multiple outstanding PSA client call test */
#define NR_MULTI_CALL_CHILD                   (NUM_MAILBOX_QUEUE_SLOT * 2)
#else
#define NR_MULTI_CALL_CHILD                   0
#endif

/* The event flag to sync up between parent thread and child threads */
#define TEST_CHILD_EVENT_FLAG(x)              (uint32_t)(0x1UL << (x))

/* Max number of test rounds */
#define MAX_NR_LIGHT_TEST_ROUND               0x200
#define MAX_NR_HEAVY_TEST_ROUND               0x20

/* Default stack size for child thread */
#define MULTI_CALL_LIGHT_TEST_STACK_SIZE      0x200
#define MULTI_CALL_HEAVY_TEST_STACK_SIZE      0x300

/* Test UID copied from ITS test cases */
#define TEST_UID_1                            2U
/* ITS data for multiple PSA client call heavy tests */
#define ITS_DATA                               "ITSDataForMultiCore"
#define ITS_DATA_LEN                           sizeof(ITS_DATA)

/* Structure passed to test threads */
struct test_params {
    void *parent_handle;            /* The thread handle of parent thread */
    uint32_t child_idx;             /* The index of current child thread */
    uint32_t nr_rounds;             /* The number of test rounds */
    uint32_t nr_calls;              /* The number of PSA client calls */
    void *mutex_handle;             /* Mutex to protect is_complete flag */
    enum test_status_t ret;         /* The test result */
    uint32_t total_ticks;           /* The total ticks cost to complete tests */
    bool is_complete;               /* Whether current test thread completes */
    bool is_parent;                 /* Whether executed in parent thread */
};

/* List of tests */
static void multi_client_call_light_test(struct test_result_t *ret);
static void multi_client_call_heavy_test(struct test_result_t *ret);
static void multi_client_call_ooo_test(struct test_result_t *ret);

static struct test_t multi_core_tests[] = {
    {&multi_client_call_light_test,
     "MULTI_CLIENT_CALL_LIGHT_TEST",
     "Multiple outstanding NS PSA client calls lightweight test"},
    {&multi_client_call_heavy_test,
     "MULTI_CLIENT_CALL_HEAVY_TEST",
     "Multiple outstanding NS PSA client calls heavyweight test"},
    {&multi_client_call_ooo_test,
     "MULTI_CLIENT_CALL_OOO_TEST",
     "Multiple outstanding NS PSA client calls test with out-of-order calls"},
};

void register_testsuite_multi_core_ns_interface(
                                              struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    if (!sizeof(multi_core_tests)) {
        return;
    }

    list_size = (sizeof(multi_core_tests) / sizeof(multi_core_tests[0]));

    set_testsuite("TF-M test cases for multi-core topology",
                  multi_core_tests, list_size, p_test_suite);
}

static void wait_child_thread_completion(struct test_params *params_array,
                                         uint8_t child_idx)
{
    bool is_complete;
    uint8_t i;
    void *mutex = params_array[0].mutex_handle;

    for (i = 0; i < child_idx; i++) {
        while (1) {
            os_wrapper_mutex_acquire(mutex, OS_WRAPPER_WAIT_FOREVER);
            is_complete = params_array[i].is_complete;
            os_wrapper_mutex_release(mutex);

            if (is_complete) {
                break;
            }
        }
    }
}

static void multi_client_call_test(struct test_result_t *ret,
                                   os_wrapper_thread_func test_runner,
                                   int32_t stack_size,
                                   int32_t nr_rounds,
                                   bool is_mixed)
{
    uint8_t i, nr_child;
    void *current_thread_handle;
    uint32_t current_thread_priority, err, total_ticks, total_calls, avg_ticks;
    void *mutex_handle;
    void *child_ids[NR_MULTI_CALL_CHILD];
    struct ns_mailbox_stats_res_t stats_res;
    struct test_params parent_params, params[NR_MULTI_CALL_CHILD];

    tfm_ns_mailbox_tx_stats_reinit();

    current_thread_handle = os_wrapper_thread_get_handle();
    if (!current_thread_handle) {
        TEST_FAIL("Failed to get current thread ID\r\n");
        return;
    }

    err = os_wrapper_thread_get_priority(current_thread_handle,
                                         &current_thread_priority);
    if (err == OS_WRAPPER_ERROR) {
        TEST_FAIL("Failed to get current thread priority\r\n");
        return;
    }

    /*
     * Create a mutex to protect the synchronization between child test thread
     * about the completion status.
     * The best way is to use os_wrapper_thread_wait/set_flag(). However, due to
     * the implementation of the wait event functions in some RTOS, if the
     * child test threads already exit before the main thread starts to wait for
     * event (main thread itself has to perform test too), the main thread
     * cannot receive the event flags.
     * As a result, use a flag and a mutex to make sure the main thread can
     * capture the completion event of child threads.
     */
    mutex_handle = os_wrapper_mutex_create();
    if (!mutex_handle) {
        TEST_FAIL("Failed to create a mutex\r\n");
        return;
    }

    /* Create test threads one by one */
    for (i = 0; i < NR_MULTI_CALL_CHILD; i++) {
        params[i].parent_handle = current_thread_handle;
        params[i].child_idx = i;
        params[i].nr_rounds = nr_rounds;
        params[i].mutex_handle = mutex_handle;
        params[i].is_complete = false;
        params[i].is_parent = false;

        child_ids[i] = os_wrapper_thread_new(NULL,
                                             stack_size,
                                             test_runner,
                                             &params[i],
                                             current_thread_priority);
        if (!child_ids[i]) {
            break;
        }
    }

    nr_child = i;
    TEST_LOG("Totally %d threads for test start\r\n", nr_child + 1);
    if (!is_mixed) {
        TEST_LOG("Each thread run 0x%x rounds tests\r\n", nr_rounds);
    }

    /*
     * Activate test threads one by one.
     * Try to make test threads to run together.
     */
    for (i = 0; i < nr_child; i++) {
        os_wrapper_thread_set_flag(child_ids[i], TEST_CHILD_EVENT_FLAG(i));
    }

    /* Use current thread to execute a test instance */
    parent_params.child_idx = nr_child;
    parent_params.nr_rounds = nr_rounds;
    parent_params.is_parent = true;
    test_runner(&parent_params);

    /* Wait for all the test threads completes */
    wait_child_thread_completion(params, nr_child);

    os_wrapper_mutex_delete(mutex_handle);

    if (parent_params.ret != TEST_PASSED) {
        ret->val = TEST_FAILED;
        return;
    }

    total_ticks = parent_params.total_ticks;
    total_calls = parent_params.nr_calls;

    /* Check the test result of each child thread */
    for (i = 0; i < nr_child; i++) {
        if (params[i].ret != TEST_PASSED) {
            ret->val = TEST_FAILED;
            return;
        }

        total_ticks += params[i].total_ticks;
        total_calls += params[i].nr_calls;
    }

    tfm_ns_mailbox_stats_avg_slot(&stats_res);
    TEST_LOG("Totally %d NS mailbox queue slots\r\n", NUM_MAILBOX_QUEUE_SLOT);
    TEST_LOG("%d.%d NS mailbox queue slots are occupied each time in average.\r\n",
             stats_res.avg_nr_slots, stats_res.avg_nr_slots_tenths);

    TEST_LOG("Cost %d ticks totally\r\n", total_ticks);
    avg_ticks = total_ticks / total_calls;
    total_ticks %= total_calls;
    TEST_LOG("Each PSA client call cost %d.%d ticks in average\r\n", avg_ticks,
             total_ticks * 10 / total_calls);

    ret->val = TEST_PASSED;
}

static inline
enum test_status_t multi_client_call_light_loop(struct test_params *params)
{
    uint32_t i, version, total_ticks;
    uint32_t nr_rounds = params->nr_rounds;

    total_ticks = os_wrapper_get_tick();

    for (i = 0; i < nr_rounds; i++) {
        version = psa_framework_version();
        if (version != PSA_FRAMEWORK_VERSION) {
            TEST_LOG("Incorrect PSA framework version!\r\n");
            return TEST_FAILED;
        }
    }

    params->total_ticks = os_wrapper_get_tick() - total_ticks;
    params->nr_calls = nr_rounds;

    return TEST_PASSED;
}

static void multi_client_call_light_runner(void *argument)
{
    struct test_params *params = (struct test_params *)argument;

    if (!params->is_parent) {
        /* Wait for the signal to kick-off the test */
        os_wrapper_thread_wait_flag(TEST_CHILD_EVENT_FLAG(params->child_idx),
                                    OS_WRAPPER_WAIT_FOREVER);
    }

    params->ret = multi_client_call_light_loop(params);

    if (!params->is_parent) {
        /* Mark this child thread has completed */
        os_wrapper_mutex_acquire(params->mutex_handle, OS_WRAPPER_WAIT_FOREVER);
        params->is_complete = true;
        os_wrapper_mutex_release(params->mutex_handle);
    }
}

/**
 * \brief Lightweight test case to verify multiple outstanding PSA client calls
 *        feature.
 */
static void multi_client_call_light_test(struct test_result_t *ret)
{
    multi_client_call_test(ret, multi_client_call_light_runner,
                           MULTI_CALL_LIGHT_TEST_STACK_SIZE,
                           MAX_NR_LIGHT_TEST_ROUND,
                           false);
}

static inline
enum test_status_t multi_client_call_heavy_loop(const psa_storage_uid_t uid,
                                                struct test_params *params)
{
    uint32_t i, total_ticks, rounds = params->nr_rounds;
    psa_status_t status;
    size_t rd_data_len;
    char rd_data[ITS_DATA_LEN];
    const psa_storage_create_flags_t flags = PSA_STORAGE_FLAG_NONE;

    total_ticks = os_wrapper_get_tick();

    for (i = 0; i < rounds; i++) {
        /* Set a data in the asset */
        status = psa_its_set(uid, ITS_DATA_LEN, ITS_DATA, flags);
        if (status != PSA_SUCCESS) {
            TEST_LOG("Fail to write ITS asset\r\n");
            return TEST_FAILED;
        }

        /* Get data from the asset */
        status = psa_its_get(uid, 0, ITS_DATA_LEN, rd_data, &rd_data_len);
        if (status != PSA_SUCCESS) {
            TEST_LOG("Fail to read ITS asset\r\n");
            return TEST_FAILED;
        }

        /* Remove the asset to clean up storage */
        status = psa_its_remove(uid);
        if (status != PSA_SUCCESS) {
            TEST_LOG("Fail to remove ITS asset\r\n");
            return TEST_FAILED;
        }
    }

    params->total_ticks = os_wrapper_get_tick() - total_ticks;
    params->nr_calls = rounds * 3;

    return TEST_PASSED;
}

static void multi_client_call_heavy_runner(void *argument)
{
    struct test_params *params = (struct test_params *)argument;
    const psa_storage_uid_t uid = TEST_UID_1 + params->child_idx;

    if (!params->is_parent) {
        /* Wait for the signal to kick-off the test */
        os_wrapper_thread_wait_flag(TEST_CHILD_EVENT_FLAG(params->child_idx),
                                    OS_WRAPPER_WAIT_FOREVER);
    }

    params->ret = multi_client_call_heavy_loop(uid, params);

    if (!params->is_parent) {
        /* Mark this child thread has completed */
        os_wrapper_mutex_acquire(params->mutex_handle, OS_WRAPPER_WAIT_FOREVER);
        params->is_complete = true;
        os_wrapper_mutex_release(params->mutex_handle);
    }
}

/**
 * \brief NS interface to verify multiple outstanding PSA client calls feature
 *        by calling heavyweight secure services.
 */
static void multi_client_call_heavy_test(struct test_result_t *ret)
{
    multi_client_call_test(ret, multi_client_call_heavy_runner,
                           MULTI_CALL_HEAVY_TEST_STACK_SIZE,
                           MAX_NR_HEAVY_TEST_ROUND,
                           false);
}

static void multi_client_call_ooo_runner(void *argument)
{
    struct test_params *params = (struct test_params *)argument;
    const psa_storage_uid_t uid = TEST_UID_1 + params->child_idx;

    if (!params->is_parent) {
        /* Wait for the signal to kick-off the test */
        os_wrapper_thread_wait_flag(TEST_CHILD_EVENT_FLAG(params->child_idx),
                                    OS_WRAPPER_WAIT_FOREVER);
    }

    if (!params->child_idx % 2) {
        params->ret = multi_client_call_heavy_loop(uid, params);
    } else {
        params->nr_rounds *= 20;
        params->ret = multi_client_call_light_loop(params);
    }

    if (!params->is_parent) {
        /* Mark this child thread has completed */
        os_wrapper_mutex_acquire(params->mutex_handle, OS_WRAPPER_WAIT_FOREVER);
        params->is_complete = true;
        os_wrapper_mutex_release(params->mutex_handle);
    }
}

/**
 * \brief Mix lightweight test and heavyweight test to verify multiple
 *        outstanding PSA client calls feature with out-of-order calls.
 */
static void multi_client_call_ooo_test(struct test_result_t *ret)
{
    multi_client_call_test(ret, multi_client_call_ooo_runner,
                           MULTI_CALL_HEAVY_TEST_STACK_SIZE,
                           MAX_NR_HEAVY_TEST_ROUND,
                           true);
}
