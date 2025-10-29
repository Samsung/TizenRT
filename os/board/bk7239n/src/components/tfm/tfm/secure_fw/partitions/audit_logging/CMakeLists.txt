#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (NOT TFM_PARTITION_AUDIT_LOG)
    return()
endif()

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

add_library(tfm_psa_rot_partition_audit STATIC)

target_sources(tfm_psa_rot_partition_audit
    PRIVATE
        audit_core.c
)

target_include_directories(tfm_psa_rot_partition_audit
    PUBLIC
        .
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/audit_logging
)

target_link_libraries(tfm_psa_rot_partition_audit
    PRIVATE
        platform_s
        tfm_secure_api
        psa_interface
)

########################### Audit defs #########################################

add_library(tfm_audit_logging_defs INTERFACE)

target_include_directories(tfm_audit_logging_defs
    INTERFACE
        .
)

############################ Secure API ########################################

target_sources(tfm_sprt
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tfm_audit_secure_api.c
)

# The veneers give warnings about not being properly declared so they get hidden
# to not overshadow _real_ warnings.
set_source_files_properties(tfm_audit_secure_api.c
    PROPERTIES
        COMPILE_FLAGS
            $<$<C_COMPILER_ID:ARMClang>:-Wno-implicit-function-declaration>
            $<$<C_COMPILER_ID:GNU>:-Wno-implicit-function-declaration>
            $<$<C_COMPILER_ID:IAR>:>
)

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_psa_rot_partition_audit
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
        TFM_PARTITION_AUDIT_LOG
)
