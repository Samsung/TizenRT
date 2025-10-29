#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

########################## TEST SYNC ###########################################

if ((NOT TFM_PARTITION_PROTECTED_STORAGE AND NOT FORWARD_PROT_MSG))
    set(TEST_NS_PS              OFF        CACHE BOOL      "Whether to build NS regression PS tests")
    set(TEST_S_PS               OFF        CACHE BOOL      "Whether to build S regression PS tests")
endif()

if (NOT TFM_PARTITION_INTERNAL_TRUSTED_STORAGE AND NOT FORWARD_PROT_MSG)
    set(TEST_NS_ITS             OFF        CACHE BOOL      "Whether to build NS regression ITS tests")
    set(TEST_S_ITS              OFF        CACHE BOOL      "Whether to build S regression ITS tests")

    # TEST_NS_PS relies on TEST_NS_ITS
    set(TEST_NS_PS              OFF        CACHE BOOL      "Whether to build NS regression PS tests")
endif()

if (NOT TFM_PARTITION_CRYPTO AND NOT FORWARD_PROT_MSG)
    set(TEST_NS_CRYPTO          OFF        CACHE BOOL      "Whether to build NS regression Crypto tests")
    set(TEST_S_CRYPTO           OFF        CACHE BOOL      "Whether to build S regression Crypto tests")
endif()

if (NOT TFM_PARTITION_INITIAL_ATTESTATION AND NOT FORWARD_PROT_MSG)
    set(TEST_NS_ATTESTATION     OFF        CACHE BOOL      "Whether to build NS regression Attestation tests")
    set(TEST_S_ATTESTATION      OFF        CACHE BOOL      "Whether to build S regression Attestation tests")
    set(TEST_NS_QCBOR           OFF        CACHE BOOL      "Whether to build NS regression QCBOR tests")
    set(TEST_NS_T_COSE          OFF        CACHE BOOL      "Whether to build NS regression t_cose tests")
endif()

if (SYMMETRIC_INITIAL_ATTESTATION)
    set(TEST_NS_T_COSE          OFF        CACHE BOOL      "Whether to build NS regression t_cose tests")
endif()

if (NOT TFM_PARTITION_PLATFORM AND NOT FORWARD_PROT_MSG)
    set(TEST_NS_PLATFORM        OFF        CACHE BOOL      "Whether to build NS regression Platform tests")
    set(TEST_S_PLATFORM         OFF        CACHE BOOL      "Whether to build S regression Platform tests")
endif()

if (NOT TFM_PARTITION_FIRMWARE_UPDATE)
    set(TEST_NS_FWU             OFF        CACHE BOOL      "Whether to build NS regression FWU tests")
    set(TEST_S_FWU              OFF        CACHE BOOL      "Whether to build S regression FWU tests")
endif()

if (NOT TFM_PARTITION_AUDIT_LOG)
    set(TEST_NS_AUDIT           OFF        CACHE BOOL      "Whether to build NS regression Audit log tests")
    set(TEST_S_AUDIT            OFF        CACHE BOOL      "Whether to build S regression Audit log tests")
endif()

if (NOT TFM_MULTI_CORE_TOPOLOGY)
    set(TEST_NS_MULTI_CORE      OFF        CACHE BOOL      "Whether to build NS regression multi-core tests")
endif()

if (NOT TFM_NS_MANAGE_NSID)
    set(TEST_NS_MANAGE_NSID     OFF        CACHE BOOL      "Whether to build NS regression NSID management tests")
endif()

if (CONFIG_TFM_FP STREQUAL "soft")
    set(TEST_S_FPU                       OFF        CACHE BOOL      "Whether to build S regression FPU tests")
    set(TEST_NS_FPU                      OFF        CACHE BOOL      "Whether to build NS regression FPU tests")
endif()

########################## Test framework sync #################################

get_cmake_property(CACHE_VARS CACHE_VARIABLES)

if (NS)
    # Force TEST_FRAMEWORK_NS ON if single NS test ON
    foreach(CACHE_VAR ${CACHE_VARS})
        string(REGEX MATCH "^TEST_NS_.*" _NS_TEST_FOUND "${CACHE_VAR}")
        if (_NS_TEST_FOUND AND "${${CACHE_VAR}}")
            set(TEST_FRAMEWORK_NS       ON        CACHE BOOL      "Whether to build NS regression tests framework")
            break()
        endif()
    endforeach()
endif()

# Force TEST_FRAMEWORK_S ON if single S test ON
foreach(CACHE_VAR ${CACHE_VARS})
    string(REGEX MATCH "^TEST_S_.*" _S_TEST_FOUND "${CACHE_VAR}")
    if (_S_TEST_FOUND AND "${${CACHE_VAR}}")
        set(TEST_FRAMEWORK_S        ON        CACHE BOOL      "Whether to build S regression tests framework")
        break()
    endif()
endforeach()

########################## Extra test suites ###################################

if (EXTRA_NS_TEST_SUITES_PATHS)
    set(TEST_FRAMEWORK_NS       ON        CACHE BOOL      "Whether to build NS regression tests framework")
endif()

if (EXTRA_S_TEST_SUITES_PATHS)
    set(TEST_FRAMEWORK_S        ON        CACHE BOOL      "Whether to build S regression tests framework")
endif()

########################## Test profile ########################################

if (TFM_PROFILE)
    include(${TFM_TEST_PATH}/config/profile/${TFM_PROFILE}_test.cmake)
endif()

########################## SLIH/FLIH IRQ Test ##################################

# Make FLIH IRQ test as the default IRQ test
if (NOT TFM_LIB_MODEL AND PLATFORM_FLIH_IRQ_TEST_SUPPORT
    AND TEST_NS AND NOT TEST_NS_SLIH_IRQ)
    set(TEST_NS_FLIH_IRQ        ON        CACHE BOOL      "Whether to build NS regression First-Level Interrupt Handling tests")
endif()

if (NOT TFM_LIB_MODEL AND PLATFORM_SLIH_IRQ_TEST_SUPPORT
    AND TEST_NS AND NOT TEST_NS_FLIH_IRQ)
    set(TEST_NS_SLIH_IRQ        ON        CACHE BOOL      "Whether to build NS regression Second-Level Interrupt Handling tests")
endif()

############################ IPC backend test ##################################
if (CONFIG_TFM_SPM_BACKEND_IPC AND TEST_NS)
    set(TEST_NS_IPC             ON        CACHE BOOL      "Whether to build NS regression SFN backend tests")
endif()

if (CONFIG_TFM_SPM_BACKEND_IPC AND TEST_S)
    set(TEST_S_IPC              ON        CACHE BOOL      "Whether to build NS regression SFN backend tests")
endif()

############################ SFN backend test ##################################

if (CONFIG_TFM_SPM_BACKEND_SFN AND TEST_NS)
    set(TEST_NS_SFN_BACKEND     ON        CACHE BOOL      "Whether to build NS regression SFN backend tests")
endif()

if (CONFIG_TFM_SPM_BACKEND_SFN AND TEST_S)
    set(TEST_S_SFN_BACKEND      ON        CACHE BOOL      "Whether to build S regression SFN backend tests")
endif()

######################### Library Model Specific Tests #########################
if (TFM_LIB_MODEL AND TEST_NS)
    set(TEST_NS_CORE            ON        CACHE BOOL      "Whether to build NS regression Core tests")
endif()

########################## Load default config #################################

if (TEST_S)
    include(${TFM_TEST_PATH}/config/default_s_test_config.cmake)
endif()
if (TEST_NS)
    if (NOT NS)
        # In this case, TEST_NS is used to configure corresponding test secure
        # services during SPE build alone.
        # Disable TEST_FRAMEWORK_NS if NS is OFF as NS test framework shall
        # run inside NS environment
        set(TEST_FRAMEWORK_NS       OFF        CACHE BOOL      "Whether to build NS regression tests framework")
    endif()
    include(${TFM_TEST_PATH}/config/default_ns_test_config.cmake)
endif()

###################### Test Partition configurations ###########################
if (TEST_NS_IPC OR TEST_S_IPC)
    set(TFM_PARTITION_IPC_TEST  ON)
else()
    set(TFM_PARTITION_IPC_TEST  OFF)
endif()

if (TEST_NS_SFN_BACKEND OR TEST_S_SFN_BACKEND)
    set(TFM_PARTITION_SFN_BACKEND_TEST  ON)
else()
    set(TFM_PARTITION_SFN_BACKEND_TEST  OFF)
endif()

# Enable FPU test partition if S or NS FP test enabled
if (TEST_S_FPU OR TEST_NS_FPU)
    set(TEST_PARTITION_FPU_TEST        ON)
else()
    set(TEST_PARTITION_FPU_TEST        OFF)
endif()

include(${TFM_TEST_PATH}/config/default_test_config.cmake)
