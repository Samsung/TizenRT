#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (NOT TFM_PARTITION_PLATFORM)
    return()
endif()

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

add_library(tfm_psa_rot_partition_platform STATIC
    platform_sp.c
)

# The generated sources
target_sources(tfm_psa_rot_partition_platform
    PRIVATE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/platform/auto_generated/intermedia_tfm_platform.c>
)
target_sources(tfm_partitions
    INTERFACE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/platform/auto_generated/load_info_tfm_platform.c>
)

# Set include directory
target_include_directories(tfm_psa_rot_partition_platform
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/platform
)
target_include_directories(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/platform
)

target_link_libraries(tfm_psa_rot_partition_platform
    PRIVATE
        platform_s
        psa_interface
        tfm_secure_api
        tfm_spm
)

############################ Secure API ########################################

target_sources(tfm_sprt
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tfm_platform_secure_api.c
)

# The veneers give warnings about not being properly declared so they get hidden
# to not overshadow _real_ warnings.
set_source_files_properties(tfm_platform_secure_api.c
    PROPERTIES
        COMPILE_FLAGS
            $<$<C_COMPILER_ID:ARMClang>:-Wno-implicit-function-declaration>
            $<$<C_COMPILER_ID:GNU>:-Wno-implicit-function-declaration>
            $<$<C_COMPILER_ID:IAR>:>
)

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_psa_rot_partition_platform
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
        TFM_PARTITION_PLATFORM
)
