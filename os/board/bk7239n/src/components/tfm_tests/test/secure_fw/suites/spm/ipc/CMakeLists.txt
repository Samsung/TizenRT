#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if (NOT TEST_NS_IPC AND NOT TEST_S_IPC)
    return()
endif()

####################### Non Secure #############################################

if (TEST_NS_IPC)
    add_library(tfm_test_suite_ipc_ns STATIC EXCLUDE_FROM_ALL)

    target_sources(tfm_test_suite_ipc_ns
        PRIVATE
            non_secure/ipc_ns_interface_testsuite.c
            ${CMAKE_CURRENT_SOURCE_DIR}/../common/suites/client_api_tests.c
            $<$<BOOL:${PSA_FRAMEWORK_HAS_MM_IOVEC}>:
                ${CMAKE_CURRENT_SOURCE_DIR}/../common/suites/mmiovec_test.c>
    )

    target_include_directories(tfm_test_suite_ipc_ns
        PUBLIC
            non_secure
    )

    target_compile_definitions(tfm_test_suite_ipc_ns
        PRIVATE
            DOMAIN_NS=1
        INTERFACE
            TEST_NS_IPC
    )

    target_link_libraries(tfm_test_suite_ipc_ns
        PRIVATE
            tfm_test_framework_ns
            spm_test_common
    )

    target_link_libraries(tfm_ns_tests
        INTERFACE
            tfm_test_suite_ipc_ns
    )
endif()

####################### Secure #################################################

if (TEST_S_IPC)
    target_sources(tfm_test_suite_ipc_s
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/secure/ipc_s_interface_testsuite.c
            ${CMAKE_CURRENT_SOURCE_DIR}/../common/suites/client_api_tests.c
    )

    target_include_directories(tfm_test_suite_ipc_s
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/secure
    )

    target_compile_definitions(tfm_test_suite_ipc_s
        PRIVATE
            $<$<AND:$<VERSION_GREATER:${TFM_ISOLATION_LEVEL},1>,$<BOOL:${TFM_ISOLATION_TEST_APP_2_PSA}>>:TFM_IPC_ISOLATION_2_APP_ACCESS_PSA_MEM>
            $<$<AND:$<VERSION_GREATER:${TFM_ISOLATION_LEVEL},1>,$<BOOL:${TFM_ISOLATION_TEST_MEM_CHECK}>>:TFM_IPC_ISOLATION_2_MEM_CHECK>
        INTERFACE
            TEST_S_IPC
    )

    target_link_libraries(tfm_test_suite_ipc_s
        PRIVATE
            tfm_test_framework_s
            spm_test_common
    )

    target_link_libraries(tfm_s_tests
        INTERFACE
            tfm_test_suite_ipc_s
    )
endif()
