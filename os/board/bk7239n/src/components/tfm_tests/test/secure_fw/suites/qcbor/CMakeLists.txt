#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
cmake_policy(SET CMP0079 NEW)

######################### qcbor ns ############################

set(QCBOR_SRC_DIR ${CMAKE_SOURCE_DIR}/lib/ext/qcbor)

add_library(tfm_qcbor_ns STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_qcbor_ns
    PRIVATE
        ${QCBOR_SRC_DIR}/src/ieee754.c
        ${QCBOR_SRC_DIR}/src/qcbor_encode.c
        ${QCBOR_SRC_DIR}/src/qcbor_decode.c
        ${QCBOR_SRC_DIR}/src/UsefulBuf.c
)

target_include_directories(tfm_qcbor_ns
    PUBLIC
        $<BUILD_INTERFACE:${QCBOR_SRC_DIR}/inc>
)

target_link_libraries(tfm_qcbor_ns
    PRIVATE
        tfm_t_cose_defs
)

target_compile_options(tfm_qcbor_ns
    PUBLIC
        ${COMPILER_CP_FLAG}
)

####################### Non Secure Test #############################################

if (NOT TEST_NS_QCBOR)
    return()
endif()

add_library(tfm_test_suite_qcbor_ns STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_test_suite_qcbor_ns
    PRIVATE
        non_secure/qcbor_ns_testsuite.c
)

target_include_directories(tfm_test_suite_qcbor_ns
    PUBLIC
        non_secure
        ${QCBOR_SRC_DIR}/test/
)

target_compile_definitions(tfm_test_suite_qcbor_ns
    PRIVATE
        DOMAIN_NS=1
    INTERFACE
        TEST_NS_QCBOR
)

target_link_libraries(tfm_test_suite_qcbor_ns
    PRIVATE
        tfm_test_framework_ns
        tfm_qcbor_test
)

target_link_libraries(tfm_ns_tests
    INTERFACE
        tfm_test_suite_qcbor_ns
)

############################ qcbor test ########################################

add_library(tfm_qcbor_test STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_qcbor_test
    PRIVATE
        ${QCBOR_SRC_DIR}/test/float_tests.c
        ${QCBOR_SRC_DIR}/test/half_to_double_from_rfc7049.c
        ${QCBOR_SRC_DIR}/test/qcbor_decode_tests.c
        ${QCBOR_SRC_DIR}/test/qcbor_encode_tests.c
        ${QCBOR_SRC_DIR}/test/run_tests.c
        ${QCBOR_SRC_DIR}/test/UsefulBuf_Tests.c
)

target_include_directories(tfm_qcbor_test
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/test>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
)

target_link_libraries(tfm_qcbor_test
    PRIVATE
        tfm_qcbor_ns
)
