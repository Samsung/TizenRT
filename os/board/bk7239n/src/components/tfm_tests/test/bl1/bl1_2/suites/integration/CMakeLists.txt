#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

add_library(bl1_2_test_suite_integration)

target_sources(bl1_2_test_suite_integration
    PRIVATE
        ./bl1_2_integration_tests.c
)

target_include_directories(bl1_2_test_suite_integration
    PUBLIC
        .
)

target_link_libraries(bl1_2_test_suite_integration
    PRIVATE
        tfm_test_framework_common
        bl1_2_lib
        bl1_1_shared_lib_interface
        platform_region_defs
)
