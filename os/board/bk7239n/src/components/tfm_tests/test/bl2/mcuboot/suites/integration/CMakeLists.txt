#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

add_library(mcuboot_test_suite_integration)

target_sources(mcuboot_test_suite_integration
    PRIVATE
        ./mcuboot_integration_tests.c
)

target_include_directories(mcuboot_test_suite_integration
    PUBLIC
        .
    PRIVATE
        ${CMAKE_SOURCE_DIR}/bl2/ext/mcuboot/include
        ${MCUBOOT_PATH}/boot/bootutil/src
)

target_link_libraries(mcuboot_test_suite_integration
    PUBLIC
        tfm_test_framework_common
        bootutil
        tfm_log
        platform_bl2
)
