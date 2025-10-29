#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

########################## S test framework ####################################

set(TEST_FRAMEWORK_S        ON        CACHE BOOL      "Whether to build S regression tests framework")

########################## S test suites #######################################

set(TEST_S_ATTESTATION      OFF        CACHE BOOL      "Whether to build S regression Attestation tests")
set(TEST_S_AUDIT            OFF        CACHE BOOL      "Whether to build S regression Audit log tests")
set(TEST_S_CRYPTO           OFF        CACHE BOOL      "Whether to build S regression Crypto tests")
set(TEST_S_ITS              OFF        CACHE BOOL      "Whether to build S regression ITS tests")
set(TEST_S_PS               OFF        CACHE BOOL      "Whether to build S regression PS tests")
set(TEST_S_PLATFORM         OFF        CACHE BOOL      "Whether to build S regression Platform tests")

set(TEST_S_FWU              OFF       CACHE BOOL      "Whether to build S regression FWU tests")
set(TEST_S_IPC              OFF       CACHE BOOL      "Whether to build S regression IPC tests")
set(TEST_S_SFN_BACKEND      OFF       CACHE BOOL      "Whether to build S regression SFN tests")
set(TEST_S_FPU              OFF       CACHE BOOL      "Whether to build S regression FPU tests")
set(TEST_S_VT              ON        CACHE BOOL      "Whether to build S regression VT tests")
