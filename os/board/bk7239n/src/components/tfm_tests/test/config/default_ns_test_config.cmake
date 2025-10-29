#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

########################## NS test framework ###################################

set(TEST_FRAMEWORK_NS       ON        CACHE BOOL      "Whether to build NS regression tests framework")

########################## NS test suites ######################################

set(TEST_NS_ATTESTATION     OFF        CACHE BOOL      "Whether to build NS regression Attestation tests")
set(TEST_NS_T_COSE          OFF        CACHE BOOL      "Whether to build NS regression t_cose tests")
set(TEST_NS_QCBOR           OFF        CACHE BOOL      "Whether to build NS regression QCBOR tests")
set(TEST_NS_AUDIT           OFF        CACHE BOOL      "Whether to build NS regression Audit log tests")
set(TEST_NS_CRYPTO          OFF        CACHE BOOL      "Whether to build NS regression Crypto tests")
set(TEST_NS_ITS             OFF        CACHE BOOL      "Whether to build NS regression ITS tests")
set(TEST_NS_PS              OFF        CACHE BOOL      "Whether to build NS regression PS tests")
set(TEST_NS_PLATFORM        OFF        CACHE BOOL      "Whether to build NS regression Platform tests")
set(TEST_NS_FWU             OFF        CACHE BOOL      "Whether to build NS regression FWU tests")
set(TEST_NS_MULTI_CORE      OFF        CACHE BOOL      "Whether to build NS regression multi-core tests")
set(TEST_NS_MANAGE_NSID     OFF        CACHE BOOL      "Whether to build NS regression NSID management tests")
set(TEST_NS_VT              ON        CACHE BOOL      "Whether to build NS regression VT tests")

set(TEST_NS_SLIH_IRQ        OFF       CACHE BOOL      "Whether to build NS regression Second-Level Interrupt Handling tests")
set(TEST_NS_FLIH_IRQ        OFF       CACHE BOOL      "Whether to build NS regression First-Level Interrupt Handling tests")
set(TEST_NS_FPU             OFF       CACHE BOOL      "Whether to build NS regression FPU tests")
set(TEST_NS_IPC             OFF       CACHE BOOL      "Whether to build NS regression IPC tests")
set(TEST_NS_SFN_BACKEND     OFF       CACHE BOOL      "Whether to build NS regression SFN backend tests")
set(TEST_NS_CORE            OFF       CACHE BOOL      "Whether to build NS regression Core tests")
