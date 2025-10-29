#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.13)

add_library(tfm_test_framework_common INTERFACE)

target_sources(tfm_test_framework_common
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/framework/test_framework.c
        ${CMAKE_CURRENT_SOURCE_DIR}/framework/test_framework_helpers.c
        ${CMAKE_CURRENT_SOURCE_DIR}/framework/test_framework_integ_test_helper.c
)

target_include_directories(tfm_test_framework_common
    INTERFACE
        framework
)

target_link_libraries(tfm_test_framework_common
    INTERFACE
        tfm_log_interface
)

if(TEST_FRAMEWORK_S OR TEST_FRAMEWORK_NS)
    add_subdirectory(secure_fw)
endif()

if(TEST_BL2)
    add_subdirectory(bl2)
endif()

if(TEST_BL1_1 OR TEST_BL1_2)
    add_subdirectory(bl1)
endif()
