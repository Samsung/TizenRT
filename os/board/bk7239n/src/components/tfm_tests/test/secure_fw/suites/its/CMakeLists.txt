#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if (NOT TEST_NS_ITS AND NOT TEST_S_ITS)
    return()
endif()

####################### Non Secure #############################################

if (TEST_NS_ITS)
    add_library(tfm_test_suite_its_ns STATIC EXCLUDE_FROM_ALL)

    target_sources(tfm_test_suite_its_ns
        PRIVATE
            its_tests_common.c
            non_secure/psa_its_ns_interface_testsuite.c
    )

    target_include_directories(tfm_test_suite_its_ns
        PUBLIC
            non_secure
    )

    target_compile_definitions(tfm_test_suite_its_ns
        PRIVATE
            DOMAIN_NS=1
            ITS_MAX_ASSET_SIZE=${ITS_MAX_ASSET_SIZE}
        INTERFACE
            TEST_NS_ITS
    )

    target_link_libraries(tfm_test_suite_its_ns
        PRIVATE
            tfm_test_framework_ns
            CMSIS_5_tfm_ns
            platform_region_defs
    )

    target_link_libraries(tfm_ns_tests
        INTERFACE
            tfm_test_suite_its_ns
    )
endif()

####################### Secure #################################################

if (TEST_S_ITS)
    target_sources(tfm_test_suite_its_s
        PRIVATE
           ${CMAKE_CURRENT_SOURCE_DIR}/its_tests_common.c
           ${CMAKE_CURRENT_SOURCE_DIR}/secure/psa_its_s_interface_testsuite.c
           ${CMAKE_CURRENT_SOURCE_DIR}/secure/psa_its_s_reliability_testsuite.c
    )

    target_include_directories(tfm_test_suite_its_s
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/secure
    )

    target_compile_definitions(tfm_test_suite_its_s
        PRIVATE
            ITS_MAX_ASSET_SIZE=${ITS_MAX_ASSET_SIZE}
        INTERFACE
            TEST_S_ITS
    )

    target_link_libraries(tfm_test_suite_its_s
        PRIVATE
            tfm_secure_api
            tfm_test_framework_s
            platform_region_defs
            tfm_app_rot_partition_secure_client_2
            platform_s
    )

    target_link_libraries(tfm_s_tests
        INTERFACE
            tfm_test_suite_its_s
    )
endif()
