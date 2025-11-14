#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if (NOT EXTRA_NS_TEST_SUITES_PATHS AND NOT EXTRA_S_TEST_SUITES_PATHS)
    return()
endif()

####################### Common library #########################################

add_library(tfm_test_suite_extra_common STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_test_suite_extra_common
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/extra_tests_common.c
)

target_include_directories(tfm_test_suite_extra_common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

####################### Non-Secure extra test library ##########################

if (EXTRA_NS_TEST_SUITES_PATHS)
    add_library(tfm_test_suite_extra_ns STATIC EXCLUDE_FROM_ALL)

    target_sources(tfm_test_suite_extra_ns
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/non_secure/extra_ns_tests.c
    )

    target_include_directories(tfm_test_suite_extra_ns
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/non_secure
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    target_compile_definitions(tfm_test_suite_extra_ns
        INTERFACE
            EXTRA_NS_TEST_SUITE
    )

    target_link_libraries(tfm_test_suite_extra_ns
        PRIVATE
            tfm_test_framework_ns
            tfm_test_suite_extra_common
    )

    target_link_libraries(tfm_ns_tests
        INTERFACE
            tfm_test_suite_extra_ns
    )
endif()

####################### Secure extra test library ##############################

if (EXTRA_S_TEST_SUITES_PATHS)
    target_sources(tfm_test_suite_extra_s
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/secure/extra_s_tests.c
    )

    target_include_directories(tfm_test_suite_extra_s
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/secure
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    target_compile_definitions(tfm_test_suite_extra_s
        INTERFACE
            EXTRA_S_TEST_SUITE
    )

    target_link_libraries(tfm_test_suite_extra_s
        PRIVATE
            tfm_test_framework_s
            tfm_secure_api
            platform_s
            tfm_test_suite_extra_common
    )

    target_link_libraries(tfm_s_tests
        INTERFACE
            tfm_test_suite_extra_s
    )
endif()

# Add extra test suites source from outside
include(extra_tests_config.cmake)
