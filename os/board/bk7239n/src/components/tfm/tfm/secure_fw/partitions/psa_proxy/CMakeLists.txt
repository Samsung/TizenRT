#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if(NOT TFM_PARTITION_PSA_PROXY)
    return()
endif()

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

add_library(tfm_psa_rot_partition_psa_proxy STATIC)

# Set include directory
target_include_directories(tfm_psa_rot_partition_psa_proxy
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/psa_proxy
    PUBLIC
        # Required for spm_ipc.h
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../../spm/cmsis_psa
)
target_include_directories(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/psa_proxy
)

target_sources(tfm_psa_rot_partition_psa_proxy
    PRIVATE
        psa_proxy.c
        psa_proxy_shared_mem_mngr.c
        ../../../interface/src/multi_core/tfm_ns_mailbox.c
)

# The generated sources
target_sources(tfm_psa_rot_partition_psa_proxy
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/psa_proxy/auto_generated/intermedia_tfm_psa_proxy.c
)
target_sources(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/psa_proxy/auto_generated/load_info_tfm_psa_proxy.c
)

target_link_libraries(tfm_psa_rot_partition_psa_proxy
    PRIVATE
        tfm_secure_api
        psa_interface
        secure_fw
        platform_s
)

############################ Secure API ########################################

target_sources(tfm_sprt
    PRIVATE
        ../crypto/tfm_crypto_secure_api.c
        ../initial_attestation/tfm_attest_secure_api.c
        ../internal_trusted_storage/tfm_its_secure_api.c
        ../platform/tfm_platform_secure_api.c
        ../protected_storage/tfm_ps_secure_api.c
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
        tfm_psa_rot_partition_psa_proxy
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
        TFM_PARTITION_PSA_PROXY
)
