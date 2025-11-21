#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
# Copyright (c) 2021, Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (TFM_MULTI_CORE_TOPOLOGY)
    return()
endif()

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

if(NOT TFM_PSA_API)
    return()
endif()

target_sources(tfm_spm
    PRIVATE
        $<$<NOT:$<BOOL:${TFM_MULTI_CORE_TOPOLOGY}>>:${CMAKE_CURRENT_SOURCE_DIR}/ns_agent_tz.c>
)

target_sources(tfm_partitions
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/load_info_ns_agent_tz.c
)

# If this is added to the spm, it is discarded as it is not used. Since the
# spm is a static library it can't generate veneers under all compilers so
# instead this single file is added to the tfm_s target.
target_sources(tfm_s
    PRIVATE
        $<$<NOT:$<BOOL:${TFM_MULTI_CORE_TOPOLOGY}>>:${CMAKE_CURRENT_SOURCE_DIR}/tfm_psa_api_veneers.c>
)
