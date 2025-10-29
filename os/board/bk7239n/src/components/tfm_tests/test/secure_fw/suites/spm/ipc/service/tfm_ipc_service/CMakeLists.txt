#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

add_library(tfm_psa_rot_partition_ipc_service STATIC)

target_sources(tfm_psa_rot_partition_ipc_service
    PRIVATE
        ../../../common/service/client_api_test_service.c
        ./tfm_ipc_service_test.c
)

# The generated sources
target_sources(tfm_psa_rot_partition_ipc_service
    PRIVATE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_ipc_service/auto_generated/intermedia_tfm_ipc_service_test.c>
        $<$<BOOL:${PSA_FRAMEWORK_HAS_MM_IOVEC}>:
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../common/service/tfm_mmiovec_test_service.c>
)

target_sources(tfm_partitions
    INTERFACE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_ipc_service/auto_generated/load_info_tfm_ipc_service_test.c>
)

# Set include directory
target_include_directories(tfm_psa_rot_partition_ipc_service
    PUBLIC
        .
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_ipc_service
)

target_include_directories(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_ipc_service
)

target_link_libraries(tfm_psa_rot_partition_ipc_service
    PRIVATE
        tfm_secure_api
        psa_interface
        platform_s
        tfm_sprt
        spm_test_service_common
)

target_link_libraries(tfm_spm
    PRIVATE
        tfm_psa_rot_partition_ipc_service
)

target_compile_definitions(tfm_psa_rot_partition_ipc_service
    PRIVATE
        $<$<AND:$<VERSION_GREATER:${TFM_ISOLATION_LEVEL},1>,$<BOOL:${TFM_ISOLATION_TEST_APP_2_PSA}>>:TFM_IPC_ISOLATION_2_APP_ACCESS_PSA_MEM>
        $<$<AND:$<VERSION_GREATER:${TFM_ISOLATION_LEVEL},1>,$<BOOL:${TFM_ISOLATION_TEST_MEM_CHECK}>>:TFM_IPC_ISOLATION_2_MEM_CHECK>
)

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_psa_rot_partition_ipc_service
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
        TFM_PARTITION_TEST_CORE_IPC
        $<$<VERSION_GREATER:${TFM_ISOLATION_LEVEL},1>:TFM_IPC_ISOLATION_2_TEST_READ_ONLY_MEM>
)
