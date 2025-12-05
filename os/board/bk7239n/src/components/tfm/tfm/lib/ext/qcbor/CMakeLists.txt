#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

add_library(tfm_qcbor_s STATIC EXCLUDE_FROM_ALL)

target_sources(tfm_qcbor_s
    PRIVATE
        src/ieee754.c
        src/qcbor_encode.c
        src/qcbor_decode.c
        src/UsefulBuf.c
)

target_include_directories(tfm_qcbor_s
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
)

target_link_libraries(tfm_qcbor_s
    PRIVATE
        tfm_attestation_defs
        tfm_t_cose_defs
)

target_compile_options(tfm_qcbor_s
    PRIVATE
        ${COMPILER_CP_FLAG}
)
