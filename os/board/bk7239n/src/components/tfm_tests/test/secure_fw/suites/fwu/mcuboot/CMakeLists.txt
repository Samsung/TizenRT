#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (NOT TEST_NS_FWU AND NOT TEST_S_FWU)
    return()
endif()

####################### Non Secure #############################################

if (TEST_NS_FWU)
    add_library(tfm_test_suite_fwu_ns STATIC EXCLUDE_FROM_ALL)

    target_sources(tfm_test_suite_fwu_ns
        PRIVATE
            ./fwu_tests_common.c
            ./non_secure/psa_fwu_ns_interface_testsuite.c
    )

    target_include_directories(tfm_test_suite_fwu_ns
        PUBLIC
            ./non_secure
    )

    target_link_libraries(tfm_test_suite_fwu_ns
        PRIVATE
            tfm_test_framework_ns
            platform_region_defs
    )

    target_compile_definitions(tfm_test_suite_fwu_ns
        PRIVATE
            DOMAIN_NS=1
            $<$<BOOL:${TFM_PSA_API}>:TFM_PSA_API>
            $<$<BOOL:${TFM_FWU_TEST_WRITE_WITH_NULL}>:TFM_FWU_TEST_WRITE_WITH_NULL>
            $<$<BOOL:${TFM_FWU_TEST_REQUEST_REBOOT}>:TFM_FWU_TEST_REQUEST_REBOOT>
            $<$<BOOL:${TFM_FWU_TEST_QUERY_WITH_NULL}>:TFM_FWU_TEST_QUERY_WITH_NULL>
            $<$<BOOL:${MCUBOOT_IMAGE_NUMBER}>:MCUBOOT_IMAGE_NUMBER=${MCUBOOT_IMAGE_NUMBER}>
        INTERFACE
            TEST_NS_FWU
    )

    target_link_libraries(tfm_ns_tests
        INTERFACE
            tfm_test_suite_fwu_ns
    )
endif()

####################### Secure #################################################

if (TEST_S_FWU)
    target_sources(tfm_test_suite_fwu_s
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/fwu_tests_common.c
            ${CMAKE_CURRENT_SOURCE_DIR}/secure/psa_fwu_s_interface_testsuite.c
    )

    target_include_directories(tfm_test_suite_fwu_s
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/secure
    )

    target_link_libraries(tfm_test_suite_fwu_s
        PRIVATE
            tfm_secure_api
            tfm_test_framework_s
            platform_s
    )

    target_compile_definitions(tfm_test_suite_fwu_s
        PRIVATE
            $<$<BOOL:${TFM_PSA_API}>:TFM_PSA_API>
            $<$<BOOL:${TFM_FWU_TEST_WRITE_WITH_NULL}>:TFM_FWU_TEST_WRITE_WITH_NULL>
            $<$<BOOL:${TFM_FWU_TEST_REQUEST_REBOOT}>:TFM_FWU_TEST_REQUEST_REBOOT>
            $<$<BOOL:${TFM_FWU_TEST_QUERY_WITH_NULL}>:TFM_FWU_TEST_QUERY_WITH_NULL>
            $<$<BOOL:${MCUBOOT_IMAGE_NUMBER}>:MCUBOOT_IMAGE_NUMBER=${MCUBOOT_IMAGE_NUMBER}>
        INTERFACE
            TEST_S_FWU
    )

    target_link_libraries(tfm_s_tests
        INTERFACE
            tfm_test_suite_fwu_s
    )

endif()
