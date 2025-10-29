#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

target_include_directories(${CC312_CDMPU_TARGET}
    INTERFACE
        .
)

target_sources(${CC312_CDMPU_TARGET}
    INTERFACE
        ccm_alt.c
        mbedtls_common.c
)

target_include_directories(${CC312_MBEDTLS_API_TARGET}
    INTERFACE
        .
)

target_sources(${CC312_MBEDTLS_API_TARGET}
    PRIVATE
        aes_alt.c
        cc_ecp_internal.c
        ccm_alt.c
        chacha20_alt.c
        chachapoly_alt.c
        cmac_alt.c
        dhm_alt.c
        ecdh_alt.c
        ecdsa_alt.c
        ecdsa_edwards.c
        ecp_common.c
        gcm_alt.c
        mbedtls_common.c
        mbedtls_hash_common.c
        poly1305_alt.c
        rsa_alt.c
        sha1_alt.c
        sha256_alt.c
        trng_api.c
)
