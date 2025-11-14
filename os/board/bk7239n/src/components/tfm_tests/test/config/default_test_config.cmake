#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

########################## NS test framework ###################################

set(TEST_FRAMEWORK_NS       OFF        CACHE BOOL      "Whether to build NS regression tests framework")

########################## NS test suites ######################################

set(TEST_NS_ATTESTATION     OFF        CACHE BOOL      "Whether to build NS regression Attestation tests")
set(TEST_NS_T_COSE          OFF        CACHE BOOL      "Whether to build NS regression t_cose tests")
set(TEST_NS_QCBOR           OFF        CACHE BOOL      "Whether to build NS regression QCBOR tests")
set(TEST_NS_AUDIT           OFF        CACHE BOOL      "Whether to build NS regression Audit log tests")
set(TEST_NS_CORE            OFF        CACHE BOOL      "Whether to build NS regression Core tests")
set(TEST_NS_CRYPTO          OFF        CACHE BOOL      "Whether to build NS regression Crypto tests")
set(TEST_NS_ITS             OFF        CACHE BOOL      "Whether to build NS regression ITS tests")
set(TEST_NS_PS              OFF        CACHE BOOL      "Whether to build NS regression PS tests")
set(TEST_NS_PLATFORM        OFF        CACHE BOOL      "Whether to build NS regression Platform tests")
set(TEST_NS_FWU             OFF        CACHE BOOL      "Whether to build NS regression FWU tests")
set(TEST_NS_IPC             OFF        CACHE BOOL      "Whether to build NS regression IPC tests")
set(TEST_NS_SLIH_IRQ        OFF        CACHE BOOL      "Whether to build NS regression Second-Level Interrupt Handling tests")
set(TEST_NS_FLIH_IRQ        OFF        CACHE BOOL      "Whether to build NS regression First-Level Interrupt Handling tests")
set(TEST_NS_MULTI_CORE      OFF        CACHE BOOL      "Whether to build NS regression multi-core tests")
set(TEST_NS_MANAGE_NSID     OFF        CACHE BOOL      "Whether to build NS regression NSID management tests")
set(TEST_NS_SFN_BACKEND     OFF        CACHE BOOL      "Whether to build NS regression SFN backend tests")
set(TEST_NS_FPU             OFF        CACHE BOOL      "Whether to build NS regression FPU tests")

########################## S test framework ####################################

set(TEST_FRAMEWORK_S        OFF        CACHE BOOL      "Whether to build S regression tests framework")

########################## S test suites #######################################

set(TEST_S_ATTESTATION      OFF        CACHE BOOL      "Whether to build S regression Attestation tests")
set(TEST_S_AUDIT            OFF        CACHE BOOL      "Whether to build S regression Audit log tests")
set(TEST_S_CRYPTO           OFF        CACHE BOOL      "Whether to build S regression Crypto tests")
set(TEST_S_ITS              OFF        CACHE BOOL      "Whether to build S regression ITS tests")
set(TEST_S_PS               OFF        CACHE BOOL      "Whether to build S regression PS tests")
set(TEST_S_PLATFORM         OFF        CACHE BOOL      "Whether to build S regression Platform tests")
set(TEST_S_FWU              OFF        CACHE BOOL      "Whether to build S regression FWU tests")
set(TEST_S_IPC              OFF        CACHE BOOL      "Whether to build S regression IPC tests")
set(TEST_S_SFN_BACKEND      OFF        CACHE BOOL      "Whether to build S regression SFN tests")

################################## PS Tests ####################################

set(PS_TEST_NV_COUNTERS                 ON          CACHE BOOL      "Use the test NV counters to test Protected Storage rollback scenarios")

################################## Default CRYPTO Tests ########################

set(TFM_CRYPTO_TEST_ALG_CBC                 ON          CACHE BOOL      "Test CBC cryptography mode")
set(TFM_CRYPTO_TEST_ALG_CCM                 ON          CACHE BOOL      "Test CCM cryptography mode")
set(TFM_CRYPTO_TEST_ALG_CFB                 ON          CACHE BOOL      "Test CFB cryptography mode")
set(TFM_CRYPTO_TEST_ALG_ECB                 ON          CACHE BOOL      "Test ECB cryptography mode")
set(TFM_CRYPTO_TEST_ALG_CTR                 ON          CACHE BOOL      "Test CTR cryptography mode")
set(TFM_CRYPTO_TEST_ALG_OFB                 ON          CACHE BOOL      "Test OFB cryptography mode")
set(TFM_CRYPTO_TEST_ALG_GCM                 ON          CACHE BOOL      "Test GCM cryptography mode")
set(TFM_CRYPTO_TEST_ALG_SHA_512             ON          CACHE BOOL      "Test SHA-512 cryptography algorithm")
set(TFM_CRYPTO_TEST_HKDF                    ON          CACHE BOOL      "Test the HKDF key derivation algorithm")
set(TFM_CRYPTO_TEST_ECDH                    ON          CACHE BOOL      "Test the ECDH key agreement algorithm")
set(TFM_CRYPTO_TEST_CHACHA20                OFF         CACHE BOOL      "Test the ChaCha20 stream cipher")
set(TFM_CRYPTO_TEST_ALG_CHACHA20_POLY1305   OFF         CACHE BOOL      "Test ChaCha20-Poly1305 AEAD algorithm")

################################## FWU Tests ###################################

set(TFM_FWU_TEST_REQUEST_REBOOT         OFF         CACHE BOOL      "Test psa_fwu_request_reboot")
set(TFM_FWU_TEST_WRITE_WITH_NULL        OFF         CACHE BOOL      "Test psa_fwu_write with data block NULL")
set(TFM_FWU_TEST_QUERY_WITH_NULL        OFF         CACHE BOOL      "Test psa_fwu_query with info NULL")

############################################ FPU Tests #########################

set(TEST_S_FPU                          OFF         CACHE BOOL      "Whether to build S regression FPU tests")

################################## Extra test suites ###########################

set(EXTRA_NS_TEST_SUITES_PATHS          ""          CACHE PATH      "List of extra non-secure test suites directories. An extra test suite folder contains source code, CMakeLists.txt and cmake configuration file")
set(EXTRA_S_TEST_SUITES_PATHS           ""          CACHE PATH      "List of extra secure test suites directories. An extra test suite folder contains source code, CMakeLists.txt and cmake configuration file")

######################### Platform specific setting ############################

set(PLATFORM_SLIH_IRQ_TEST_SUPPORT      OFF         CACHE BOOL    "Platform supports SLIH IRQ tests")
set(PLATFORM_FLIH_IRQ_TEST_SUPPORT      OFF         CACHE BOOL    "Platform supports FLIH IRQ tests")

