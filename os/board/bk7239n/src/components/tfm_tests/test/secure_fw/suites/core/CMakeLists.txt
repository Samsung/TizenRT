#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if (NOT TEST_NS_CORE)
    return()
endif()

####################### Non Secure #############################################

add_library(tfm_test_suite_core_ns STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_test_suite_core_ns
    PRIVATE
        non_secure/core_ns_positive_testsuite.c
        non_secure/core_test_api.c
)

target_include_directories(tfm_test_suite_core_ns
    PUBLIC
        non_secure
)

target_compile_definitions(tfm_test_suite_core_ns
    PRIVATE
        DOMAIN_NS=1
    INTERFACE
        TEST_NS_CORE
)

target_link_libraries(tfm_test_suite_core_ns
    PRIVATE
        tfm_test_framework_ns
        tfm_ns_integration_test
        CMSIS_5_tfm_ns
        tfm_partition_defs
        platform_ns
        tfm_psa_rot_partition_core_test_defs
)

target_link_libraries(tfm_ns_tests
    INTERFACE
        tfm_test_suite_core_ns
)
