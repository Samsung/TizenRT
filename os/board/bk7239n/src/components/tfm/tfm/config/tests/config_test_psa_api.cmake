#------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#------------------------------------------------------------------------------

############ Override defaults for PSA API tests ##############################

# CRYPTO_ENGINE_BUF_SIZE needs to be much larger for PSA API tests.
if(NOT (CRYPTO_ENGINE_BUF_SIZE GREATER 0x5000))
    set(CRYPTO_ENGINE_BUF_SIZE              0x5000      CACHE STRING    "Heap size for the crypto backend")
endif()

# When building for the PSA Crypto API tests, ensure the ITS max asset size is
# set to at least the size of the largest asset created by the tests
if (("${TEST_PSA_API}" STREQUAL "CRYPTO") AND NOT (ITS_MAX_ASSET_SIZE GREATER 1229))
    set(ITS_MAX_ASSET_SIZE                  "1229"      CACHE STRING    "The maximum asset size to be stored in the Internal Trusted Storage area")
endif()

if ("${TEST_PSA_API}" STREQUAL "IPC")
    set(TFM_PARTITION_FF_TEST   ON)
else()
    set(TFM_PARTITION_FF_TEST   OFF)
endif()
