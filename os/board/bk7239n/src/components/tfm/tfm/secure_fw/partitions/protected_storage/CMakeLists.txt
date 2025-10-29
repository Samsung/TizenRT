#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (NOT TFM_PARTITION_PROTECTED_STORAGE)
    if(TFM_PARTITION_PSA_PROXY)
        add_library(tfm_app_rot_partition_ps INTERFACE)

        target_include_directories(tfm_app_rot_partition_ps
            INTERFACE
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        )
    endif()
    return()
endif()

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

add_library(tfm_app_rot_partition_ps STATIC)

target_include_directories(tfm_app_rot_partition_ps
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/protected_storage
)
target_include_directories(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/protected_storage
)

target_sources(tfm_app_rot_partition_ps
    PRIVATE
        tfm_ps_req_mngr.c
        tfm_protected_storage.c
        ps_object_system.c
        ps_object_table.c
        ps_utils.c
        $<$<BOOL:${PS_ENCRYPTION}>:crypto/ps_crypto_interface.c>
        $<$<BOOL:${PS_ENCRYPTION}>:ps_encrypted_object.c>
        # The test_ps_nv_counters.c will be used instead, when PS secure test is
        # ON and PS_TEST_NV_COUNTERS is ON
        $<$<NOT:$<AND:$<BOOL:${TEST_S_PS}>,$<BOOL:${PS_TEST_NV_COUNTERS}>>>:nv_counters/ps_nv_counters.c>
)

# The generated sources
target_sources(tfm_app_rot_partition_ps
    PRIVATE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/protected_storage/auto_generated/intermedia_tfm_protected_storage.c>
)
target_sources(tfm_partitions
    INTERFACE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/protected_storage/auto_generated/load_info_tfm_protected_storage.c>
)

target_link_libraries(tfm_app_rot_partition_ps
    PRIVATE
        tfm_secure_api
        psa_interface
        secure_fw
        platform_s
        tfm_psa_rot_partition_its
)

############################ Secure API ########################################

target_sources(tfm_sprt
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tfm_ps_secure_api.c
)

# The veneers give warnings about not being properly declared so they get hidden
# to not overshadow _real_ warnings.
set_source_files_properties(tfm_ps_secure_api.c
    PROPERTIES
        COMPILE_FLAGS
            $<$<C_COMPILER_ID:ARMClang>:-Wno-implicit-function-declaration>
            $<$<C_COMPILER_ID:GNU>:-Wno-implicit-function-declaration>
            $<$<C_COMPILER_ID:IAR>:>
)

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_app_rot_partition_ps
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
        TFM_PARTITION_PROTECTED_STORAGE
)
