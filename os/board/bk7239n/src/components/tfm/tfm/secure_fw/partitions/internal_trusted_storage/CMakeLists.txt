#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (NOT TFM_PARTITION_INTERNAL_TRUSTED_STORAGE)
    return()
endif()

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

add_library(tfm_psa_rot_partition_its STATIC)

target_include_directories(tfm_psa_rot_partition_its
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/internal_trusted_storage
    PUBLIC
        # Required for ps_object_defs.h
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../protected_storage
)
target_include_directories(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/internal_trusted_storage
)

target_sources(tfm_psa_rot_partition_its
    PRIVATE
        tfm_its_req_mngr.c
        tfm_internal_trusted_storage.c
        its_utils.c
        flash/its_flash.c
        flash/its_flash_nand.c
        flash/its_flash_nor.c
        flash/its_flash_ram.c
        flash_fs/its_flash_fs.c
        flash_fs/its_flash_fs_dblock.c
        flash_fs/its_flash_fs_mblock.c
)

# The generated sources
target_sources(tfm_psa_rot_partition_its
    PRIVATE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/internal_trusted_storage/auto_generated/intermedia_tfm_internal_trusted_storage.c>
)
target_sources(tfm_partitions
    INTERFACE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/internal_trusted_storage/auto_generated/load_info_tfm_internal_trusted_storage.c>
)

target_link_libraries(tfm_psa_rot_partition_its
    PRIVATE
        tfm_secure_api
        platform_s
        psa_interface
        tfm_sprt
)

target_compile_definitions(tfm_psa_rot_partition_its
    PUBLIC
        $<$<BOOL:${PS_CREATE_FLASH_LAYOUT}>:PS_CREATE_FLASH_LAYOUT>
        $<$<BOOL:${PS_ENCRYPTION}>:PS_ENCRYPTION>
        $<$<BOOL:${PS_RAM_FS}>:PS_RAM_FS>
        $<$<BOOL:${PS_ROLLBACK_PROTECTION}>:PS_ROLLBACK_PROTECTION>
        $<$<BOOL:${PS_VALIDATE_METADATA_FROM_FLASH}>:PS_VALIDATE_METADATA_FROM_FLASH>
        PS_MAX_ASSET_SIZE=${PS_MAX_ASSET_SIZE}
        PS_NUM_ASSETS=${PS_NUM_ASSETS}
        PS_CRYPTO_AEAD_ALG=${PS_CRYPTO_AEAD_ALG}
    PRIVATE
        $<$<BOOL:${ITS_CREATE_FLASH_LAYOUT}>:ITS_CREATE_FLASH_LAYOUT>
        $<$<BOOL:${ITS_RAM_FS}>:ITS_RAM_FS>
        $<$<OR:$<BOOL:${ITS_VALIDATE_METADATA_FROM_FLASH}>,$<BOOL:${PS_VALIDATE_METADATA_FROM_FLASH}>>:ITS_VALIDATE_METADATA_FROM_FLASH>
        ITS_MAX_ASSET_SIZE=${ITS_MAX_ASSET_SIZE}
        ITS_NUM_ASSETS=${ITS_NUM_ASSETS}
        $<$<BOOL:${ITS_BUF_SIZE}>:ITS_BUF_SIZE=${ITS_BUF_SIZE}>
)

################ Display the configuration being applied #######################

if (NOT CONFIG_TFM_PARTITION_QUIET)
    message(STATUS "----------- Display storage configuration - start ------------")

    message(STATUS "PS_CREATE_FLASH_LAYOUT is set to ${PS_CREATE_FLASH_LAYOUT}")
    message(STATUS "PS_ENCRYPTION is set to ${PS_ENCRYPTION}")
    message(STATUS "PS_RAM_FS is set to ${PS_RAM_FS}")
    message(STATUS "PS_ROLLBACK_PROTECTION is set to ${PS_ROLLBACK_PROTECTION}")
    message(STATUS "PS_VALIDATE_METADATA_FROM_FLASH is set to ${PS_VALIDATE_METADATA_FROM_FLASH}")
    message(STATUS "PS_MAX_ASSET_SIZE is set to ${PS_MAX_ASSET_SIZE}")
    message(STATUS "PS_NUM_ASSETS is set to ${PS_NUM_ASSETS}")
    message(STATUS "PS_CRYPTO_AEAD_ALG is set to ${PS_CRYPTO_AEAD_ALG}")

    message(STATUS "ITS_CREATE_FLASH_LAYOUT is set to ${ITS_CREATE_FLASH_LAYOUT}")
    message(STATUS "ITS_RAM_FS is set to ${ITS_RAM_FS}")
    message(STATUS "ITS_VALIDATE_METADATA_FROM_FLASH is set to ${ITS_VALIDATE_METADATA_FROM_FLASH}")
    message(STATUS "ITS_MAX_ASSET_SIZE is set to ${ITS_MAX_ASSET_SIZE}")
    message(STATUS "ITS_NUM_ASSETS is set to ${ITS_NUM_ASSETS}")
    if (${ITS_BUF_SIZE})
        message(STATUS "ITS_BUF_SIZE is set to ${ITS_BUF_SIZE}")
    else()
        message(STATUS "ITS_BUF_SIZE is not set (defaults to ITS_MAX_ASSET_SIZE)")
    endif()

    message(STATUS "----------- Display storage configuration - stop -------------")
endif()

############################ Secure API ########################################

target_sources(tfm_sprt
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tfm_its_secure_api.c
)

# The veneers give warnings about not being properly declared so they get hidden
# to not overshadow _real_ warnings.
set_source_files_properties(tfm_its_secure_api.c
    PROPERTIES
        COMPILE_FLAGS
            $<$<C_COMPILER_ID:ARMClang>:-Wno-implicit-function-declaration>
            $<$<C_COMPILER_ID:GNU>:-Wno-implicit-function-declaration>
            $<$<C_COMPILER_ID:IAR>:>
)

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_psa_rot_partition_its
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
        TFM_PARTITION_INTERNAL_TRUSTED_STORAGE
)
