#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

add_library(tfm_fih_headers INTERFACE)
add_library(tfm_fih_implementation STATIC)

target_sources(tfm_fih_implementation
    PRIVATE
        src/fih.c
)

target_include_directories(tfm_fih_headers
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
)

target_link_libraries(tfm_fih_implementation
    PUBLIC
        tfm_fih_headers
        platform_common_interface
)

target_compile_definitions(tfm_fih_headers
    INTERFACE
        TFM_FIH_PROFILE_${TFM_FIH_PROFILE}
        $<$<NOT:$<STREQUAL:${TFM_FIH_PROFILE},OFF>>:TFM_FIH_PROFILE_ON>
)

target_compile_options(tfm_fih_headers
    INTERFACE
        $<$<C_COMPILER_ID:GNU>:-Wno-unused-value>
)
