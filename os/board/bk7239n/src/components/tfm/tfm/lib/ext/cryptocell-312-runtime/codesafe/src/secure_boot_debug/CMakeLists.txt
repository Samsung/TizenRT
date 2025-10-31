#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

target_sources(${CC312_LIB_TARGET}
    PRIVATE
        cc3x_verifier/bootimagesverifier_base_single.c
        cc3x_verifier/bootimagesverifier_parser.c
        cc3x_verifier/bootimagesverifier_swcomp.c
        common/common_cert_parser.c
        common/common_cert_verify.c
        crypto_driver/rsa_exp.c
        crypto_driver/rsa_pki_pka.c
        crypto_driver/rsa_pki_pka.c
        crypto_driver/rsa_verify.c
        platform/common/cc3x/secureboot_base_swimgverify.c
        platform/nvm/cc3x_nvm_rt/nvm_otp.c
        secure_boot_gen/secureboot_base_func.c
)
