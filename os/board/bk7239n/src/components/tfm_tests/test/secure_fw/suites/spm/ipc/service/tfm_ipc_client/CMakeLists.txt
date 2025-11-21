#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

add_library(tfm_app_rot_partition_ipc_client STATIC)

target_sources(tfm_app_rot_partition_ipc_client
    PRIVATE
        ./tfm_ipc_client_test.c
)

# The generated sources
target_sources(tfm_app_rot_partition_ipc_client
    PRIVATE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_ipc_client/auto_generated/intermedia_tfm_ipc_client_test.c>
)

target_sources(tfm_partitions
    INTERFACE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_ipc_client/auto_generated/load_info_tfm_ipc_client_test.c>
)

# Set include directory
target_include_directories(tfm_app_rot_partition_ipc_client
    PUBLIC
        .
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_ipc_client
        ${TFM_TEST_PATH}/secure_fw/common_test_services/tfm_secure_client_2
)

target_include_directories(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_ipc_client
)

target_link_libraries(tfm_app_rot_partition_ipc_client
    PRIVATE
        tfm_secure_api
        psa_interface
        platform_s
)

target_link_libraries(tfm_spm
    PRIVATE
        tfm_app_rot_partition_ipc_client
)

target_compile_definitions(tfm_app_rot_partition_ipc_client
    PRIVATE
        $<$<BOOL:${TFM_ISOLATION_TEST_APP_2_PSA}>:TFM_IPC_ISOLATION_2_APP_ACCESS_PSA_MEM>
        $<$<BOOL:${TFM_ISOLATION_TEST_MEM_CHECK}>:TFM_IPC_ISOLATION_2_MEM_CHECK>
)

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_app_rot_partition_ipc_client
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
        TFM_PARTITION_TEST_CORE_IPC
        $<$<AND:$<BOOL:${TEST_S_IPC}>,$<VERSION_EQUAL:${TFM_ISOLATION_LEVEL},3>>:TFM_IPC_ISOLATION_3_RETRIEVE_APP_MEM>
)
