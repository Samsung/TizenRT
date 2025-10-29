#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if (NOT TEST_S_FPU)
    return()
endif()

add_library(tfm_app_rot_partition_fpu_service STATIC)

target_sources(tfm_app_rot_partition_fpu_service
    PRIVATE
        tfm_fpu_service_test.c
)

# The generated sources
target_sources(tfm_app_rot_partition_fpu_service
    PRIVATE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_fpu_service/auto_generated/intermedia_tfm_fpu_service_test.c>
)

target_sources(tfm_partitions
    INTERFACE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_fpu_service/auto_generated/load_info_tfm_fpu_service_test.c>
)

target_include_directories(tfm_app_rot_partition_fpu_service
    PUBLIC
        .
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_fpu_service
        ${TFM_TEST_PATH}/secure_fw/suites/fpu
)

target_include_directories(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_fpu_service
)

target_link_libraries(tfm_app_rot_partition_fpu_service
    PRIVATE
        tfm_secure_api
        platform_s
        tfm_test_framework_s
)

target_link_libraries(tfm_spm
    PRIVATE
        tfm_app_rot_partition_fpu_service
)

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_app_rot_partition_fpu_service
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
        TEST_PARTITION_FPU_TEST
)

