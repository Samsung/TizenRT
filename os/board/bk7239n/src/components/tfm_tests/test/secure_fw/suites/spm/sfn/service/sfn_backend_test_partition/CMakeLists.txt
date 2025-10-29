#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

add_library(tfm_app_rot_partition_sfn_backend_test STATIC)

target_sources(tfm_app_rot_partition_sfn_backend_test
    PRIVATE
        sfn_backend_test_partition.c
        ../../../common/service/client_api_test_service.c
        $<$<BOOL:${PSA_FRAMEWORK_HAS_MM_IOVEC}>:
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../common/service/tfm_mmiovec_test_service.c>
)

target_include_directories(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/sfn_backend_test_partition
)

# The generated sources
target_sources(tfm_app_rot_partition_sfn_backend_test
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/sfn_backend_test_partition/auto_generated/intermedia_sfn_backend_test_partition.c
)
target_sources(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/sfn_backend_test_partition/auto_generated/load_info_sfn_backend_test_partition.c
)

target_include_directories(tfm_app_rot_partition_sfn_backend_test
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/sfn_backend_test_partition
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../common/service
        ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(tfm_app_rot_partition_sfn_backend_test
    PRIVATE
        tfm_secure_api
        psa_interface
        platform_s
        tfm_sprt
        spm_test_service_common
)

############################ Partition Defs ####################################
target_link_libraries(tfm_partitions
    INTERFACE
        tfm_app_rot_partition_sfn_backend_test
)
