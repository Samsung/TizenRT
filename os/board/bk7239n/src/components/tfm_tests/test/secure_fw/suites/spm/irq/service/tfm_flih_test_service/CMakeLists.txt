#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
cmake_policy(SET CMP0079 NEW)

add_library(tfm_app_rot_partition_flih_test STATIC)

target_sources(tfm_app_rot_partition_flih_test
    PRIVATE
        ./tfm_flih_test_service.c
        $<$<BOOL:${TEST_NS_FLIH_IRQ}>:
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../common/service/tfm_irq_test_service.c>
)

# The generated sources
target_sources(tfm_app_rot_partition_flih_test
    PRIVATE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_flih_test_service/auto_generated/intermedia_tfm_flih_test_service.c>
    INTERFACE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_flih_test_service/auto_generated/load_info_tfm_flih_test_service.c>
)

target_include_directories(tfm_app_rot_partition_flih_test
    PUBLIC
        ./
        ${CMAKE_BINARY_DIR}/generated/secure_fw/test_services/tfm_flih_test_service
)

target_link_libraries(tfm_app_rot_partition_flih_test
    PRIVATE
        psa_interface
        tfm_sprt
        spm_test_service_common
        platform_s
        tfm_secure_api
)

target_link_libraries(tfm_spm
    PRIVATE
        tfm_app_rot_partition_flih_test
)

# Interfaces for other targets
add_library(tfm_flih_test_service_interface INTERFACE)

target_include_directories(tfm_flih_test_service_interface
    INTERFACE
        ./
)

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_app_rot_partition_flih_test
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
        $<$<BOOL:${TEST_NS_FLIH_IRQ}>:TEST_NS_FLIH_IRQ>
)
