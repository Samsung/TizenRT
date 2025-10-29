#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if (NOT TEST_NS_VT AND NOT TEST_S_VT)
    return()
endif()

####################### Non Secure #############################################

if (TEST_NS_VT)
    add_library(tfm_test_suite_vt_ns STATIC EXCLUDE_FROM_ALL)

    target_sources(tfm_test_suite_vt_ns
        PRIVATE
            vt_tests_common.c
            non_secure/psa_vt_ns_interface_testsuite.c
    )

    target_include_directories(tfm_test_suite_vt_ns
        PUBLIC
            non_secure
    )

    target_compile_definitions(tfm_test_suite_vt_ns
        PRIVATE
            DOMAIN_NS=1
        INTERFACE
            TEST_NS_VT
    )

    target_link_libraries(tfm_test_suite_vt_ns
        PRIVATE
            tfm_test_framework_ns
            CMSIS_5_tfm_ns
            platform_region_defs
    )

    target_link_libraries(tfm_ns_tests
        INTERFACE
            tfm_test_suite_vt_ns
    )
endif()

####################### Secure #################################################

if (TEST_S_VT)
    add_library(tfm_test_suite_vt_s STATIC EXCLUDE_FROM_ALL)

    target_sources(tfm_test_suite_vt_s
        PRIVATE
            vt_tests_common.c
            secure/psa_vt_s_interface_testsuite.c
    )

    target_include_directories(tfm_test_suite_vt_s
        PUBLIC
            secure
    )

    target_compile_definitions(tfm_test_suite_vt_s
        PRIVATE
            DOMAIN_S=1
        INTERFACE
            TEST_S_VT
    )

    target_link_libraries(tfm_test_suite_vt_s
        PRIVATE
            tfm_test_framework_s
            platform_region_defs
    )

    target_link_libraries(tfm_s_tests
        INTERFACE
            tfm_test_suite_vt_s
    )
endif()
