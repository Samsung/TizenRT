#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

####################### Non Secure #############################################

if (TEST_NS_FPU)
    add_library(tfm_test_suite_fpu_ns STATIC)

    target_sources(tfm_test_suite_fpu_ns
        PRIVATE
            fpu_tests_common.c
            non_secure/fpu_ns_interface_testsuite.c
    )

    target_include_directories(tfm_test_suite_fpu_ns
        PUBLIC
            non_secure
    )

    target_compile_definitions(tfm_test_suite_fpu_ns
        PRIVATE
            DOMAIN_NS=1
        INTERFACE
            TEST_NS_FPU
    )

    target_link_libraries(tfm_test_suite_fpu_ns
        PRIVATE
            tfm_test_framework_ns
            platform_ns
    )

    target_link_libraries(tfm_ns_tests
        INTERFACE
            tfm_test_suite_fpu_ns
    )
endif()

####################### Secure #################################################

if (TEST_S_FPU)
    target_sources(tfm_test_suite_fpu_s
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/fpu_tests_common.c
            ${CMAKE_CURRENT_SOURCE_DIR}/secure/fpu_s_interface_testsuite.c
    )

    target_include_directories(tfm_test_suite_fpu_s
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/secure
    )

    target_compile_definitions(tfm_test_suite_fpu_s
        INTERFACE
            TEST_S_FPU
    )

    target_link_libraries(tfm_test_suite_fpu_s
        PRIVATE
            tfm_test_framework_s
            platform_s
    )

    target_link_libraries(tfm_s_tests
        INTERFACE
            tfm_test_suite_fpu_s
    )
endif()
