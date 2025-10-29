#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

target_include_directories(${CC312_LIB_TARGET}
    PRIVATE
        include/cc_mng
    PUBLIC
        hw/include
        include
        include/cc_util
        include/crypto_api
        include/crypto_api/cc3x
        include/mbedtls
        include/pal
        include/pal/no_os
        include/proj/cc3x
)

target_sources(${CC312_LIB_TARGET}
    PRIVATE
        src/proj/cc3x/cc_ecpki_info.c
        src/proj/cc3x/cc_hash_info.c
        src/proj/cc3x/cc_rsa_info.c
)

target_include_directories(${CC312_PAL_TARGET}
    PRIVATE
        include
        include/proj/cc3x
        hw/include
        include/pal
        include/pal/no_os
)

target_include_directories(${CC312_CDMPU_TARGET}
    INTERFACE
        include/crypto_api
        include/crypto_api/cc3x
        include/cc_mng
)
