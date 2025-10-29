#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if(NOT TEST_BL1_2)
    return()
endif()

add_subdirectory(suites/integration)

add_library(bl1_2_tests STATIC)

target_sources(bl1_2_tests
    PRIVATE
        ./bl1_2_suites.c
)

target_include_directories(bl1_2_tests
    PUBLIC
        interface
)

target_link_libraries(bl1_2_tests
    PRIVATE
        tfm_test_framework_common
        platform_bl1_interface
        bl1_2_lib
        bl1_2_test_suite_integration
)
