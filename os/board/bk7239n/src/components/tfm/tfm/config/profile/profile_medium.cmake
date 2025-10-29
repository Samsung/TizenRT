#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(TFM_PROFILE                         profile_medium CACHE STRING    "Profile to use")
set(TFM_ISOLATION_LEVEL                 2           CACHE STRING    "Isolation level")

############################ Partitions ########################################

set(ITS_BUF_SIZE                        32          CACHE STRING    "Size of the ITS internal data transfer buffer (defaults to ITS_MAX_ASSET_SIZE if not set)")
set(CRYPTO_ENGINE_BUF_SIZE              0x4000      CACHE STRING    "Heap size for the crypto backend")
set(PS_CRYPTO_AEAD_ALG                  PSA_ALG_CCM CACHE STRING    "The AEAD algorithm to use for authenticated encryption in protected storage")

set(CRYPTO_ASYM_ENCRYPT_MODULE_DISABLED OFF          CACHE BOOL      "Disable PSA Crypto asymmetric key encryption module")

set(TFM_PARTITION_AUDIT_LOG             OFF         CACHE BOOL      "Enable Audit Log partition")


################################## Dependencies ################################

set(TFM_MBEDCRYPTO_CONFIG_PATH          "${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/mbedcrypto_config/tfm_mbedcrypto_config_profile_medium.h" CACHE PATH "Config to use for Mbed Crypto")
