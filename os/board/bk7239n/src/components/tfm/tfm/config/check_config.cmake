#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
# Copyright (c) 2021, Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

function(tfm_invalid_config)
    if (${ARGV})
        string (REPLACE ";" " " ARGV_STRING "${ARGV}")
        string (REPLACE "STREQUAL"     "=" ARGV_STRING "${ARGV_STRING}")
        string (REPLACE "GREATER"      ">" ARGV_STRING "${ARGV_STRING}")
        string (REPLACE "LESS"         "<" ARGV_STRING "${ARGV_STRING}")
        string (REPLACE "VERSION_LESS" "<" ARGV_STRING "${ARGV_STRING}")
        string (REPLACE "EQUAL"        "=" ARGV_STRING "${ARGV_STRING}")
        string (REPLACE "IN_LIST"      "in" ARGV_STRING "${ARGV_STRING}")

        message(FATAL_ERROR "INVALID CONFIG: ${ARGV_STRING}")
    endif()
endfunction()

set (TFM_L3_PLATFORM_LISTS arm/mps2/an521 arm/musca_b1/sse_200 stm/stm32l562e_dk)
set (VALID_ISOLATION_LEVELS 1 2 3)

tfm_invalid_config(TFM_LIB_MODEL AND TFM_PSA_API)
tfm_invalid_config(NOT TFM_LIB_MODEL AND NOT TFM_PSA_API)
tfm_invalid_config(NOT TFM_ISOLATION_LEVEL IN_LIST VALID_ISOLATION_LEVELS)
tfm_invalid_config(TFM_ISOLATION_LEVEL EQUAL 3 AND NOT TFM_PLATFORM IN_LIST TFM_L3_PLATFORM_LISTS)
tfm_invalid_config(TFM_ISOLATION_LEVEL GREATER 1 AND TFM_LIB_MODEL)
tfm_invalid_config(TFM_ISOLATION_LEVEL GREATER 1 AND PSA_FRAMEWORK_HAS_MM_IOVEC)
tfm_invalid_config(TFM_LIB_MODEL AND PSA_FRAMEWORK_HAS_MM_IOVEC)

tfm_invalid_config(TFM_MULTI_CORE_TOPOLOGY AND TFM_LIB_MODEL)
tfm_invalid_config(TFM_MULTI_CORE_TOPOLOGY AND TFM_NS_MANAGE_NSID)
tfm_invalid_config(TFM_PLAT_SPECIFIC_MULTI_CORE_COMM AND NOT TFM_MULTI_CORE_TOPOLOGY)

tfm_invalid_config((TFM_S_REG_TEST OR TFM_NS_REG_TEST) AND TEST_PSA_API)

tfm_invalid_config(TFM_PARTITION_PROTECTED_STORAGE AND NOT TFM_PARTITION_INTERNAL_TRUSTED_STORAGE)
tfm_invalid_config((TFM_PARTITION_PROTECTED_STORAGE AND PS_ROLLBACK_PROTECTION) AND NOT TFM_PARTITION_PLATFORM)
tfm_invalid_config(PS_ROLLBACK_PROTECTION AND NOT PS_ENCRYPTION)

tfm_invalid_config(SUITE STREQUAL "IPC" AND NOT TEST_PSA_API STREQUAL "IPC")

tfm_invalid_config(TEST_PSA_API STREQUAL "IPC" AND TFM_LIB_MODEL)
tfm_invalid_config(TEST_PSA_API STREQUAL "CRYPTO" AND NOT TFM_PARTITION_CRYPTO)
tfm_invalid_config(TEST_PSA_API STREQUAL "INITIAL_ATTESTATION" AND NOT TFM_PARTITION_INITIAL_ATTESTATION)
tfm_invalid_config(TEST_PSA_API STREQUAL "INTERNAL_TRUSTED_STORAGE" AND NOT TFM_PARTITION_INTERNAL_TRUSTED_STORAGE)
tfm_invalid_config(TEST_PSA_API STREQUAL "PROTECTED_STORAGE" AND NOT TFM_PARTITION_PROTECTED_STORAGE)
tfm_invalid_config(TEST_PSA_API STREQUAL "STORAGE" AND NOT TFM_PARTITION_INTERNAL_TRUSTED_STORAGE)
tfm_invalid_config(TEST_PSA_API STREQUAL "STORAGE" AND NOT TFM_PARTITION_PROTECTED_STORAGE)

########################## FPU ################################################

tfm_invalid_config(NOT (CONFIG_TFM_FP STREQUAL "soft" OR CONFIG_TFM_FP STREQUAL "hard"))
tfm_invalid_config(NOT CMAKE_C_COMPILER_ID STREQUAL "GNU" AND CONFIG_TFM_FP STREQUAL "hard")
tfm_invalid_config((NOT CONFIG_TFM_FP_ARCH) AND CONFIG_TFM_FP STREQUAL "hard")
tfm_invalid_config((NOT TFM_PSA_API) AND CONFIG_TFM_FP STREQUAL "hard")
tfm_invalid_config(CONFIG_TFM_FP STREQUAL "soft" AND CONFIG_TFM_LAZY_STACKING)

########################## BL2 #################################################

get_property(MCUBOOT_STRATEGY_LIST CACHE MCUBOOT_UPGRADE_STRATEGY PROPERTY STRINGS)
tfm_invalid_config(BL2 AND (NOT MCUBOOT_UPGRADE_STRATEGY IN_LIST MCUBOOT_STRATEGY_LIST))

tfm_invalid_config(MCUBOOT_IMAGE_NUMBER GREATER 3)

get_property(MCUBOOT_ALIGN_VAL_LIST CACHE MCUBOOT_ALIGN_VAL PROPERTY STRINGS)
tfm_invalid_config(BL2 AND (NOT MCUBOOT_ALIGN_VAL IN_LIST MCUBOOT_ALIGN_VAL_LIST))

####################### Code sharing ###########################################

set(TFM_CODE_SHARING_PLATFORM_LISTS arm/mps2/an521 arm/musca_b1/sse_200) # Without crypto hw acceleration
tfm_invalid_config(NOT TFM_CODE_SHARING STREQUAL "OFF" AND NOT TFM_PLATFORM IN_LIST TFM_CODE_SHARING_PLATFORM_LISTS)
tfm_invalid_config(NOT TFM_CODE_SHARING STREQUAL "OFF" AND CRYPTO_HW_ACCELERATOR)

########################## Platform ############################################

tfm_invalid_config(OTP_NV_COUNTERS_RAM_EMULATION AND NOT (PLATFORM_DEFAULT_OTP OR PLATFORM_DEFAULT_NV_COUNTERS))
tfm_invalid_config(PLATFORM_DEFAULT_NV_COUNTERS AND  NOT PLATFORM_DEFAULT_OTP_WRITEABLE)
tfm_invalid_config(TFM_DUMMY_PROVISIONING AND  NOT PLATFORM_DEFAULT_OTP_WRITEABLE)

####################### Firmware Update Parttion ###############################

# tfm_invalid_config(TFM_PARTITION_FIRMWARE_UPDATE AND NOT TFM_PARTITION_PLATFORM)
# tfm_invalid_config((MCUBOOT_UPGRADE_STRATEGY STREQUAL "DIRECT_XIP" OR MCUBOOT_UPGRADE_STRATEGY STREQUAL "RAM_LOAD") AND TFM_PARTITION_FIRMWARE_UPDATE)
# tfm_invalid_config(TFM_PARTITION_FIRMWARE_UPDATE AND NOT MCUBOOT_DATA_SHARING)

########################## FIH #################################################

get_property(TFM_FIH_PROFILE_LIST CACHE TFM_FIH_PROFILE PROPERTY STRINGS)
tfm_invalid_config(NOT TFM_FIH_PROFILE IN_LIST TFM_FIH_PROFILE_LIST)

########################### TF-M audit log #####################################

tfm_invalid_config(TFM_PARTITION_AUDIT_LOG AND NOT TFM_LIB_MODEL)

########################### TF-M crypto ########################################

tfm_invalid_config(CRYPTO_NV_SEED AND CRYPTO_HW_ACCELERATOR)
tfm_invalid_config(NOT CRYPTO_NV_SEED AND NOT CRYPTO_HW_ACCELERATOR)

######################## TF-M Porfile config check #############################

tfm_invalid_config(TFM_PROFILE STREQUAL "profile_small" AND CONFIG_TFM_SPM_BACKEND_IPC)

########################### Test check config ##################################

if(TFM_S_REG_TEST OR TFM_NS_REG_TEST)
    include(${TFM_TEST_PATH}/config/check_config.cmake)
endif()

###################### Compiler check for FP support ###########################

include(config/cp_check.cmake)
