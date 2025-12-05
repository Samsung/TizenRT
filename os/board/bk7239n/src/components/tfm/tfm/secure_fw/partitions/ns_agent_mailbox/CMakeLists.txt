#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
# Copyright (c) 2021, Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (NOT TFM_MULTI_CORE_TOPOLOGY)
    return()
endif()

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

add_library(tfm_psa_rot_partition_ns_agent_mailbox STATIC)

target_sources(tfm_psa_rot_partition_ns_agent_mailbox
    PRIVATE
        ${CMAKE_SOURCE_DIR}/secure_fw/spm/cmsis_psa/tfm_rpc.c
        $<$<NOT:$<BOOL:${TFM_PLAT_SPECIFIC_MULTI_CORE_COMM}>>:${CMAKE_SOURCE_DIR}/secure_fw/spm/cmsis_psa/tfm_spe_mailbox.c>
        ns_agent_mailbox.c
)

target_link_libraries(tfm_psa_rot_partition_ns_agent_mailbox
    PRIVATE
        platform_common_interface
        psa_interface
        tfm_spm
        tfm_sprt
        platform_s
        tfm_secure_api
)

# The generated sources
target_sources(tfm_psa_rot_partition_ns_agent_mailbox
    PRIVATE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/ns_agent_mailbox/auto_generated/intermedia_ns_agent_mailbox.c
)
target_sources(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/ns_agent_mailbox/auto_generated/load_info_ns_agent_mailbox.c
)

# Set include directory
target_include_directories(tfm_psa_rot_partition_ns_agent_mailbox
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PUBLIC
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/ns_agent_mailbox
)

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_psa_rot_partition_ns_agent_mailbox
)
