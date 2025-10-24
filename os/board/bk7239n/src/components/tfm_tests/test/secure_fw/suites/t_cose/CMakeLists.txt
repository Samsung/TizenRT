#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

############################ t_cose non secure #################################

add_library(tfm_t_cose_ns STATIC EXCLUDE_FROM_ALL)

target_link_libraries(tfm_t_cose_ns
    PUBLIC
        tfm_t_cose_common
        tfm_t_cose_defs
        tfm_qcbor_ns
        tfm_api_ns
)

if (NOT TEST_NS_T_COSE)
    return()
endif()

############################ t_cose test #######################################

add_library(tfm_t_cose_test STATIC EXCLUDE_FROM_ALL)

set(T_COSE_SRC_DIR ${CMAKE_SOURCE_DIR}/lib/ext/t_cose)

target_sources(tfm_t_cose_test
    PRIVATE
        ${T_COSE_SRC_DIR}/test/run_tests.c
        ${T_COSE_SRC_DIR}/test/t_cose_make_psa_test_key.c
        ${T_COSE_SRC_DIR}/test/t_cose_make_test_messages.c
        ${T_COSE_SRC_DIR}/test/t_cose_sign_verify_test.c
        ${T_COSE_SRC_DIR}/test/t_cose_test.c
)

target_include_directories(tfm_t_cose_test
    PUBLIC
        $<BUILD_INTERFACE:${T_COSE_SRC_DIR}/inc>
        $<BUILD_INTERFACE:${T_COSE_SRC_DIR}/test>
)

####################### Non Secure #############################################

add_library(tfm_test_suite_t_cose_ns STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_test_suite_t_cose_ns
    PRIVATE
        non_secure/t_cose_ns_testsuite.c
)

target_include_directories(tfm_test_suite_t_cose_ns
    PUBLIC
        non_secure
)

target_compile_definitions(tfm_test_suite_t_cose_ns
    PRIVATE
        DOMAIN_NS=1
    INTERFACE
        TEST_NS_T_COSE
)

target_link_libraries(tfm_test_suite_t_cose_ns
    PRIVATE
        tfm_test_framework_ns
        tfm_t_cose_test
)

target_link_libraries(tfm_ns_tests
    INTERFACE
        tfm_test_suite_t_cose_ns
)

target_link_libraries(tfm_t_cose_test
    PRIVATE
        tfm_t_cose_ns
)
