#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (NOT TFM_PARTITION_SYSTEM_CLOCK)
    return()
endif()

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

add_library(tfm_psa_rot_partition_system_clock STATIC)

target_include_directories(tfm_psa_rot_partition_system_clock
    INTERFACE
        ${CMAKE_SOURCE_DIR}/secure_fw/include
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/system_clock
)
target_include_directories(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/system_clock
)

target_sources(tfm_psa_rot_partition_system_clock
    PRIVATE
        system_clock_partition.c
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/system_clock/auto_generated/intermedia_tfm_system_clock.c>
)
target_sources(tfm_partitions
    INTERFACE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/system_clock/auto_generated/load_info_tfm_system_clock.c>
)

target_link_libraries(tfm_psa_rot_partition_system_clock
    PRIVATE
        platform_s
        psa_interface
        tfm_secure_api
        tfm_spm
        tfm_sprt
)

target_compile_definitions(tfm_psa_rot_partition_system_clock
    PRIVATE
        $<$<BOOL:${TFM_PSA_API}>:TFM_PSA_API>
)

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_psa_rot_partition_system_clock
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
        TFM_PARTITION_SYSTEM_CLOCK
)