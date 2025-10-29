#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

target_include_directories(${CC312_PSA_DRIVER_API_TARGET}
    PUBLIC
        .
        include
)

target_sources(${CC312_PSA_DRIVER_API_TARGET}
    PRIVATE
        # PSA Cryptoprocessor Unified Driver interface entry point sources
        src/cc3xx_psa_hash.c
        src/cc3xx_psa_cipher.c
        src/cc3xx_psa_mac.c
        src/cc3xx_psa_entropy.c
        src/cc3xx_psa_key_agreement.c
        src/cc3xx_psa_asymmetric_signature.c
        src/cc3xx_psa_asymmetric_encryption.c
        src/cc3xx_psa_key_generation.c
        src/cc3xx_psa_aead.c
        # Internal sources
        src/cc3xx_internal_asn1_util.c
        src/cc3xx_internal_ecc_util.c
        src/cc3xx_internal_rsa_util.c
        src/cc3xx_internal_drbg_util.c
        src/cc3xx_internal_hash_util.c
        src/cc3xx_internal_aes.c
        src/cc3xx_internal_chacha20.c
        src/cc3xx_internal_ecdh.c
        src/cc3xx_internal_ccm.c
        src/cc3xx_internal_gcm.c
        src/cc3xx_internal_chacha20_poly1305.c
)
