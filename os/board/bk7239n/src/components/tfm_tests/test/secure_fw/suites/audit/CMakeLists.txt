#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if (NOT TEST_NS_AUDIT AND NOT TEST_S_AUDIT)
    return()
endif()

####################### Non Secure #############################################

if (TEST_NS_AUDIT)
    add_library(tfm_test_suite_audit_ns STATIC)

    target_sources(tfm_test_suite_audit_ns
        PRIVATE
            non_secure/audit_ns_interface_testsuite.c
    )

    target_include_directories(tfm_test_suite_audit_ns
        PUBLIC
            non_secure
    )

    target_compile_definitions(tfm_test_suite_audit_ns
        PRIVATE
            DOMAIN_NS=1
            $<$<BOOL:${TEST_FRAMEWORK_S}>:AUDIT_TEST_S_ENABLE>
        INTERFACE
            TEST_NS_AUDIT
    )

    target_link_libraries(tfm_test_suite_audit_ns
        PRIVATE
            tfm_test_framework_ns
            tfm_audit_logging_defs
    )

    target_link_libraries(tfm_ns_tests
        INTERFACE
            tfm_test_suite_audit_ns
    )
endif()

####################### Secure #################################################

if (TEST_S_AUDIT)
    target_sources(tfm_test_suite_audit_s
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/secure/audit_s_interface_testsuite.c
    )

    target_include_directories(tfm_test_suite_audit_s
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/secure
    )

    target_compile_definitions(tfm_test_suite_audit_s
        INTERFACE
            TEST_S_AUDIT
    )

    target_link_libraries(tfm_test_suite_audit_s
        PRIVATE
            tfm_test_framework_s
            tfm_psa_rot_partition_audit
    )

    target_link_libraries(tfm_s_tests
        INTERFACE
            tfm_test_suite_audit_s
    )
endif()
