#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if (NOT TEST_NS_MANAGE_NSID)
    return()
endif()

####################### Non Secure #############################################

add_library(tfm_test_suite_nsid STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_test_suite_nsid
    PRIVATE
        ./nsid_testsuite.c
        ./nsid_svc_handler.c
)

target_include_directories(tfm_test_suite_nsid
    PUBLIC
        ./
)

target_compile_definitions(tfm_test_suite_nsid
    PUBLIC
        $<$<BOOL:${TEST_NS_MANAGE_NSID}>:TEST_NS_MANAGE_NSID>
)

target_link_libraries(tfm_test_suite_nsid
    PRIVATE
        tfm_test_framework_ns
        platform_ns
)

target_link_libraries(tfm_ns_tests
    INTERFACE
        tfm_test_suite_nsid
)
