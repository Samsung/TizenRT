#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if(NOT TEST_BL1_1)
    return()
endif()

add_subdirectory(suites/crypto)
add_subdirectory(suites/trng)
add_subdirectory(suites/integration)

add_library(bl1_1_tests STATIC)

target_sources(bl1_1_tests
    PRIVATE
        ./bl1_1_suites.c
)

target_include_directories(bl1_1_tests
    PUBLIC
        interface
)

target_link_libraries(bl1_1_tests
    PRIVATE
        tfm_test_framework_common
        platform_bl1
        bl1_1_shared_lib
        tfm_log
        bl1_1_test_suite_crypto
        bl1_1_test_suite_trng
        bl1_1_test_suite_integration
)
