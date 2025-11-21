#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if (NOT TEST_NS_PLATFORM AND NOT TEST_S_PLATFORM)
    return()
endif()

####################### Non Secure #############################################

if (TEST_NS_PLATFORM)
    add_library(tfm_test_suite_platform_ns STATIC EXCLUDE_FROM_ALL)

    target_sources(tfm_test_suite_platform_ns
        PRIVATE
            ./platform_tests_common.c
            ./non_secure/platform_ns_interface_testsuite.c
    )

    target_include_directories(tfm_test_suite_platform_ns
        PUBLIC
            ./non_secure
        PRIVATE
            .
    )

    target_compile_definitions(tfm_test_suite_platform_ns
        INTERFACE
            TEST_NS_PLATFORM
    )

    target_link_libraries(tfm_test_suite_platform_ns
        PRIVATE
            tfm_test_framework_ns
    )

    target_link_libraries(tfm_ns_tests
        INTERFACE
            tfm_test_suite_platform_ns
    )
endif()

####################### Secure #################################################

if (TEST_S_PLATFORM)
    target_sources(tfm_test_suite_platform_s
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/platform_tests_common.c
            ${CMAKE_CURRENT_SOURCE_DIR}/secure/platform_s_interface_testsuite.c
    )

    target_include_directories(tfm_test_suite_platform_s
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/secure
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    target_compile_definitions(tfm_test_suite_platform_s
        INTERFACE
            TEST_S_PLATFORM
    )

    target_link_libraries(tfm_test_suite_platform_s
        PRIVATE
            tfm_test_framework_s
            tfm_secure_api
            platform_s
    )

    target_link_libraries(tfm_s_tests
        INTERFACE
            tfm_test_suite_platform_s
    )
endif()
