#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

add_subdirectory(suites/integration)

add_library(mcuboot_tests STATIC)

target_sources(mcuboot_tests
    PRIVATE
        ./mcuboot_suites.c
)

target_include_directories(mcuboot_tests
    PUBLIC
        .
)

target_link_libraries(mcuboot_tests
    PUBLIC
        tfm_test_framework_common
    PRIVATE
        platform_bl2
        mcuboot_test_suite_integration
)
