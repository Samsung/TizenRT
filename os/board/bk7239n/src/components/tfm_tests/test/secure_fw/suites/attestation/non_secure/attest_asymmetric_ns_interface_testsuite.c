/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "attest_ns_tests.h"
#include "psa/initial_attestation.h"
#include "attest.h"
#include "../attest_tests_common.h"
#include "../attest_token_test_values.h"
#include "../attest_token_test.h"

static uint8_t token_buffer[TEST_TOKEN_SIZE];
static const uint8_t challenge_buffer[TEST_CHALLENGE_OBJ_SIZE] = {
                                      TOKEN_TEST_NONCE_BYTES};

/* Define test suite for attestation service tests */
/* List of tests */
#ifdef INCLUDE_TEST_CODE /* Remove them from release build */
static void tfm_attest_test_1001(struct test_result_t *ret);
static void tfm_attest_test_1002(struct test_result_t *ret);
static void tfm_attest_test_1003(struct test_result_t *ret);
#endif
static void tfm_attest_test_1004(struct test_result_t *ret);
static void tfm_attest_test_1005(struct test_result_t *ret);
static void tfm_attest_test_1006(struct test_result_t *ret);
static void tfm_attest_test_1007(struct test_result_t *ret);

static struct test_t attestation_interface_tests[] = {
#ifdef INCLUDE_TEST_CODE /* Remove them from release build */
    {&tfm_attest_test_1001, "TFM_NS_ATTEST_TEST_1001",
     "Minimal token test of attest token"},
    {&tfm_attest_test_1002, "TFM_NS_ATTEST_TEST_1002",
     "Minimal token size test of attest token"},
    {&tfm_attest_test_1003, "TFM_NS_ATTEST_TEST_1003",
     "Short circuit signature test of attest token"},
#endif
    {&tfm_attest_test_1004, "TFM_NS_ATTEST_TEST_1004",
     "ECDSA signature test of attest token"},
    {&tfm_attest_test_1005, "TFM_NS_ATTEST_TEST_1005",
     "Negative test cases for initial attestation service"},

    //{&tfm_attest_test_1006, "TFM_NS_ATTEST_TEST_1006", "Caict VT: 20000 times get token/size"},
    //{&tfm_attest_test_1007, "TFM_NS_ATTEST_TEST_1007", "Caict VT: collect timing data"},
};

void
register_testsuite_ns_attestation_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(attestation_interface_tests) /
                 sizeof(attestation_interface_tests[0]));

    set_testsuite("Initial Attestation Service non-secure interface tests"
                  "(TFM_NS_ATTEST_TEST_1XXX)",
                  attestation_interface_tests, list_size, p_test_suite);
}

#ifdef INCLUDE_TEST_CODE /* Remove them from release build */
/*!
 * \brief Get minimal token, only include a hard coded challenge, but omit the
 *        rest of the claims
 *
 * Calling the minimal_test, which just retrieves a specific token:
 *  - only hard coded challenge is included
 *  - token signature is the hash of the token concatenated twice
 */
static void tfm_attest_test_1001(struct test_result_t *ret)
{
    int32_t err;

    err = minimal_test();
    if (err != 0) {
        TEST_LOG("minimal_test() returned: %d\r\n", err);
        TEST_FAIL("Attest token minimal_test() has failed");
        return;
    }

    ret->val = TEST_PASSED;
}

/*!
 * \brief Get the size of the minimal token, only include a hard coded
 *        challenge, but omit the rest of the claims
 */
static void tfm_attest_test_1002(struct test_result_t *ret)
{
    int32_t err;

    err = minimal_get_size_test();
    if (err != 0) {
        TEST_LOG("minimal_get_size_test() returned: %d\r\n", err);
        TEST_FAIL("Attest token minimal_get_size_test() has failed");
        return;
    }

    ret->val = TEST_PASSED;
}

/*!
 * \brief Get an IAT with short circuit signature (signature is composed of
 *        hash of token). Parse the token, validate presence of claims and
 *        compare them against expected values in token_test_values.h
 *
 * More info in token_test.h
 */
static void tfm_attest_test_1003(struct test_result_t *ret)
{
    int32_t err;

    err = decode_test_short_circuit_sig();
    if (err != 0) {
        TEST_LOG("decode_test_short_circuit_sig() returned: %d\r\n", err);
        TEST_FAIL("Attest token decode_test_short_circuit_sig() has failed");
        return;
    }

    ret->val = TEST_PASSED;
}
#endif /* INCLUDE_TEST_CODE */

/*!
 * \brief Get an IAT with proper ECDSA signature. Parse the token, validate
 *        presence of claims and compare them against expected values in
 *        token_test_values.h
 *
 * ECDSA signing is currently not supported in TF_M.
 *
 * More info in token_test.h
 */
static void tfm_attest_test_1004(struct test_result_t *ret)
{
    int32_t err;

    err = decode_test_normal_sig();
    if (err != 0) {
        TEST_LOG("decode_test_normal_sig() returned: %d\r\n", err);
        TEST_FAIL("Attest token decode_test_normal_sig() has failed");
        return;
    }

    ret->val = TEST_PASSED;
}

/*!
 * \brief Negative tests for initial attestation service
 *
 *    - Calling initial attestation service with bigger challenge object than
 *      allowed.
 *    - Calling initial attestation service with smaller buffer size than the
 *      expected size of the token.
 */
static void tfm_attest_test_1005(struct test_result_t *ret)
{
    psa_status_t err;
    size_t token_buf_size = TEST_TOKEN_SIZE;
    size_t token_size;

    /* Call with with bigger challenge object than allowed */
    err = psa_initial_attest_get_token(challenge_buffer,
                                       INVALID_CHALLENGE_OBJECT_SIZE,
                                       token_buffer,
                                       token_buf_size,
                                       &token_size);

    if (err != PSA_ERROR_INVALID_ARGUMENT) {
        TEST_FAIL("Attestation should fail with too big challenge object");
        return;
    }

    /* Call with smaller buffer size than size of test token */
    token_buf_size = TOO_SMALL_TOKEN_BUFFER;
    err = psa_initial_attest_get_token(challenge_buffer,
                                       TEST_CHALLENGE_OBJ_SIZE,
                                       token_buffer,
                                       token_buf_size,
                                       &token_size);

    if (err != PSA_ERROR_BUFFER_TOO_SMALL) {
        TEST_FAIL("Attestation should fail with too small token buffer");
        return;
    }

    ret->val = TEST_PASSED;
}

static void tfm_attest_test_1006(struct test_result_t *ret)
{
    int32_t err;

    uint32_t cnt = 0;

    while(cnt++ < 20000) {

        if ((cnt % 1000) == 0) {
            TEST_LOG("get_token/size test, cnt: %d\r\n", cnt);
        }
    
        err = minimal_test();
        if (err != 0) {
            TEST_LOG("minimal_test() returned: %d\r\n", err);
            TEST_FAIL("Attest token minimal_test() has failed");
            return;
        }
        err = minimal_get_size_test();
        if (err != 0) {
            TEST_LOG("minimal_get_size_test() returned: %d\r\n", err);
            TEST_FAIL("Attest token minimal_get_size_test() has failed");
            return;
        }
    }

    ret->val = TEST_PASSED;
}

#define COLLECTIONTIME 400000
static uint8_t big_token_buffer[PSA_INITIAL_ATTEST_MAX_TOKEN_SIZE];
static void tfm_attest_test_1007(struct test_result_t *ret)
{
	int32_t err;
	TEST_LOG("########################################################\r\n");	
	TEST_LOG("########################################################\r\n");	
	TEST_LOG("We start to collect data for timing attack.\r\n");
	int32_t         i, num_checks = COLLECTIONTIME;
    	int32_t     status;
    	size_t      token_buffer_size, token_size;
    	
    	uint8_t     challenge[PSA_INITIAL_ATTEST_CHALLENGE_SIZE_64];

    	
    	uint64_t starttime = 0;
    	uint64_t endtime = 0;
    	uint64_t costtime = 0;
    	uint32_t costtimelow=0;
        uint32_t costtimehigh=0;
        
	for (i = 0; i < num_checks; i++)
   	{

        memset(challenge, 0x2a, sizeof(challenge));
        memset(big_token_buffer, 0, sizeof(big_token_buffer));
	TEST_LOG("before get size########################################################\r\n");
        status = psa_initial_attest_get_token_size(PSA_INITIAL_ATTEST_CHALLENGE_SIZE_64, &token_buffer_size);
	TEST_LOG("after get size########################################################\r\n");                     
        if (status != PSA_SUCCESS)
        {
		TEST_LOG("get token size failed, stop test.\n", 0);
		return;
        }

        if (token_buffer_size > PSA_INITIAL_ATTEST_MAX_TOKEN_SIZE)
        {
 		TEST_LOG("Insufficient token buffer size. stop test.\n", 0);
		return;
        }
        
	starttime = get_32k_timer();
    	TEST_LOG("before get token########################################################\r\n");
	status = psa_initial_attest_get_token(challenge, PSA_INITIAL_ATTEST_CHALLENGE_SIZE_64, big_token_buffer, token_buffer_size, &token_size);
	TEST_LOG("after get token########################################################\r\n");

	endtime = get_32k_timer();
    
	costtime = endtime - starttime;
    
	costtimelow = costtime & 0xFFFFFFFF;
	costtimehigh = (costtime>>32) & 0xFFFFFFFF;
   	TEST_LOG("Verified OK,");
    	//print r,s
    	//printfsignature(token_buffer, token_size);
	//DiagPrintf(",");
	TEST_LOG(",");
	//DiagPrintf("100,101,");
	TEST_LOG("100,%d,", i);
	//DiagPrintf("%d,\n", costtime);
	//TEST_LOG("%ld,\r\n", dwTime);
	TEST_LOG("%ld%ld,\r\n", costtimehigh,costtimelow);
	if (status != PSA_SUCCESS)
	{
		TEST_LOG("Get token failed.\r\n");
         	return;
	}
	}//end of for loop
	TEST_LOG("Collecting data is completed.\r\n");	
	TEST_LOG("########################################################\r\n");
}
