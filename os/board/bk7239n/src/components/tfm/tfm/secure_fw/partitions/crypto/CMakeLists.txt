#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (NOT TFM_PARTITION_CRYPTO)
    return()
endif()

cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0079 NEW)

add_library(tfm_psa_rot_partition_crypto STATIC)

target_sources(tfm_psa_rot_partition_crypto
    PRIVATE
        crypto_init.c
        crypto_alloc.c
        crypto_cipher.c
        crypto_hash.c
        crypto_mac.c
        crypto_key.c
        crypto_aead.c
        crypto_asymmetric.c
        crypto_key_derivation.c
        crypto_key_management.c
        crypto_rng.c
)

# The generated sources
target_sources(tfm_psa_rot_partition_crypto
    PRIVATE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/crypto/auto_generated/intermedia_tfm_crypto.c>
)
target_sources(tfm_partitions
    INTERFACE
        $<$<BOOL:${TFM_PSA_API}>:
            ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/crypto/auto_generated/load_info_tfm_crypto.c>
)

# Set include directory
target_include_directories(tfm_psa_rot_partition_crypto
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/crypto
)
target_include_directories(tfm_partitions
    INTERFACE
        ${CMAKE_BINARY_DIR}/generated/secure_fw/partitions/crypto
)

# Linking to external interfaces
target_link_libraries(tfm_psa_rot_partition_crypto
    PRIVATE
        tfm_secure_api
        platform_s
        crypto_service_mbedcrypto
        psa_interface
        tfm_sprt
)
target_compile_definitions(tfm_psa_rot_partition_crypto
    PUBLIC
        $<$<BOOL:${CRYPTO_RNG_MODULE_DISABLED}>:TFM_CRYPTO_RNG_MODULE_DISABLED>
        $<$<BOOL:${CRYPTO_KEY_MODULE_DISABLED}>:TFM_CRYPTO_KEY_MODULE_DISABLED>
        $<$<BOOL:${CRYPTO_AEAD_MODULE_DISABLED}>:TFM_CRYPTO_AEAD_MODULE_DISABLED>
        $<$<BOOL:${CRYPTO_MAC_MODULE_DISABLED}>:TFM_CRYPTO_MAC_MODULE_DISABLED>
        $<$<BOOL:${CRYPTO_CIPHER_MODULE_DISABLED}>:TFM_CRYPTO_CIPHER_MODULE_DISABLED>
        $<$<BOOL:${CRYPTO_HASH_MODULE_DISABLED}>:TFM_CRYPTO_HASH_MODULE_DISABLED>
        $<$<BOOL:${CRYPTO_ASYM_SIGN_MODULE_DISABLED}>:TFM_CRYPTO_ASYM_SIGN_MODULE_DISABLED>
        $<$<BOOL:${CRYPTO_ASYM_ENCRYPT_MODULE_DISABLED}>:TFM_CRYPTO_ASYM_ENCRYPT_MODULE_DISABLED>
        $<$<BOOL:${CRYPTO_KEY_DERIVATION_MODULE_DISABLED}>:TFM_CRYPTO_KEY_DERIVATION_MODULE_DISABLED>
    PRIVATE
        $<$<BOOL:${CRYPTO_ENGINE_BUF_SIZE}>:TFM_CRYPTO_ENGINE_BUF_SIZE=${CRYPTO_ENGINE_BUF_SIZE}>
        $<$<BOOL:${CRYPTO_CONC_OPER_NUM}>:TFM_CRYPTO_CONC_OPER_NUM=${CRYPTO_CONC_OPER_NUM}>
        $<$<AND:$<BOOL:${TFM_PSA_API}>,$<BOOL:${CONFIG_TFM_CRYPTO_IOVEC_BUFFER_SIZE}>>:TFM_CRYPTO_IOVEC_BUFFER_SIZE=${CONFIG_TFM_CRYPTO_IOVEC_BUFFER_SIZE}>
)

################ Display the configuration being applied #######################
if (NOT CONFIG_TFM_PARTITION_QUIET)
    message(STATUS "---------- Display crypto configuration - start --------------")

    message(STATUS "CRYPTO_RNG_MODULE_DISABLED is set to ${CRYPTO_RNG_MODULE_DISABLED}")
    message(STATUS "CRYPTO_KEY_MODULE_DISABLED is set to ${CRYPTO_KEY_MODULE_DISABLED}")
    message(STATUS "CRYPTO_AEAD_MODULE_DISABLED is set to ${CRYPTO_AEAD_MODULE_DISABLED}")
    message(STATUS "CRYPTO_MAC_MODULE_DISABLED is set to ${CRYPTO_MAC_MODULE_DISABLED}")
    message(STATUS "CRYPTO_CIPHER_MODULE_DISABLED is set to ${CRYPTO_CIPHER_MODULE_DISABLED}")
    message(STATUS "CRYPTO_HASH_MODULE_DISABLED is set to ${CRYPTO_HASH_MODULE_DISABLED}")
    message(STATUS "CRYPTO_KEY_DERIVATION_MODULE_DISABLED is set to ${CRYPTO_KEY_DERIVATION_MODULE_DISABLED}")
    message(STATUS "CRYPTO_ASYM_SIGN_MODULE_DISABLED is set to ${CRYPTO_ASYM_SIGN_MODULE_DISABLED}")
    message(STATUS "CRYPTO_ASYM_ENCRYPT_MODULE_DISABLED is set to ${CRYPTO_ASYM_ENCRYPT_MODULE_DISABLED}")
    message(STATUS "CRYPTO_ENGINE_BUF_SIZE is set to ${CRYPTO_ENGINE_BUF_SIZE}")
    message(STATUS "CRYPTO_CONC_OPER_NUM is set to ${CRYPTO_CONC_OPER_NUM}")
    if (${TFM_PSA_API})
        message(STATUS "CONFIG_TFM_CRYPTO_IOVEC_BUFFER_SIZE is set to ${CONFIG_TFM_CRYPTO_IOVEC_BUFFER_SIZE}")
    endif()
    message(STATUS "---------- Display crypto configuration - stop ---------------")
endif()

############################ Secure API ########################################

target_sources(tfm_sprt
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tfm_crypto_secure_api.c
)

# The veneers give warnings about not being properly declared so they get hidden
# to not overshadow _real_ warnings.
set_source_files_properties(tfm_crypto_secure_api.c
    PROPERTIES
        COMPILE_FLAGS
            $<$<C_COMPILER_ID:ARMClang>:-Wno-implicit-function-declaration>
            $<$<C_COMPILER_ID:GNU>:-Wno-implicit-function-declaration>
            $<$<C_COMPILER_ID:IAR>:>
)

############################ Partition Defs ####################################

target_link_libraries(tfm_partitions
    INTERFACE
        tfm_psa_rot_partition_crypto
)

target_compile_definitions(tfm_partition_defs
    INTERFACE
        TFM_PARTITION_CRYPTO
)

############################### MBEDCRYPTO #####################################

add_library(crypto_service_mbedcrypto_config INTERFACE)

target_compile_definitions(crypto_service_mbedcrypto_config
    INTERFACE
        MBEDTLS_CONFIG_FILE="${TFM_MBEDCRYPTO_CONFIG_PATH}"
        $<$<BOOL:${TFM_MBEDCRYPTO_PLATFORM_EXTRA_CONFIG_PATH}>:MBEDTLS_USER_CONFIG_FILE="${TFM_MBEDCRYPTO_PLATFORM_EXTRA_CONFIG_PATH}">
        PSA_CRYPTO_SECURE
        # Workaround for https://github.com/ARMmbed/mbedtls/issues/1077
        $<$<OR:$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv8-m.base>,$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv6-m>>:MULADDC_CANNOT_USE_R7>
        $<$<BOOL:${CRYPTO_NV_SEED}>:CRYPTO_NV_SEED>
        $<$<BOOL:${PLATFORM_DEFAULT_NV_SEED}>:PLATFORM_DEFAULT_NV_SEED>
)
cmake_policy(SET CMP0079 NEW)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)
set(ENABLE_TESTING OFF)
set(ENABLE_PROGRAMS OFF)
set(MBEDTLS_FATAL_WARNINGS OFF)
set(ENABLE_DOCS OFF)
set(INSTALL_MBEDTLS_HEADERS OFF)
set(LIB_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/mbedcrypto/install)

# Set the prefix to be used by mbedTLS targets
set(MBEDTLS_TARGET_PREFIX crypto_service_)

# Mbedcrypto is quite a large lib, and it uses too much memory for it to be
# reasonable to build it in debug info. As a compromise, if `debug` build type
# is selected mbedcrypto will build under `relwithdebinfo` which preserved debug
# symbols whild optimizing space.
set(SAVED_BUILD_TYPE ${CMAKE_BUILD_TYPE})
set(CMAKE_BUILD_TYPE ${MBEDCRYPTO_BUILD_TYPE})
add_subdirectory(${MBEDCRYPTO_PATH} ${CMAKE_CURRENT_BINARY_DIR}/mbedcrypto EXCLUDE_FROM_ALL)
set(CMAKE_BUILD_TYPE ${SAVED_BUILD_TYPE} CACHE STRING "Build type: [Debug, Release, RelWithDebInfo, MinSizeRel]" FORCE)

if(NOT TARGET ${MBEDTLS_TARGET_PREFIX}mbedcrypto)
    message(FATAL_ERROR "Target ${MBEDTLS_TARGET_PREFIX}mbedcrypto does not exist. Have the patches in ${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto been applied to the mbedcrypto repo at ${MBEDCRYPTO_PATH} ?
    Hint: The command might be `cd ${MBEDCRYPTO_PATH} && git apply ${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/*.patch`")
endif()

target_include_directories(${MBEDTLS_TARGET_PREFIX}mbedcrypto
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Fix platform_s and crypto_service_mbedcrypto libraries cyclic linking
set_target_properties(${MBEDTLS_TARGET_PREFIX}mbedcrypto PROPERTIES LINK_INTERFACE_MULTIPLICITY 3)

target_sources(${MBEDTLS_TARGET_PREFIX}mbedcrypto
    PRIVATE
        $<$<NOT:$<BOOL:${CRYPTO_HW_ACCELERATOR}>>:${CMAKE_CURRENT_SOURCE_DIR}/tfm_mbedcrypto_alt.c>
)

target_compile_options(${MBEDTLS_TARGET_PREFIX}mbedcrypto
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-Wno-unused-parameter>
        $<$<C_COMPILER_ID:ARMClang>:-Wno-unused-parameter>
)

target_link_libraries(${MBEDTLS_TARGET_PREFIX}mbedcrypto
    PRIVATE
        psa_interface
        platform_s
    PUBLIC
        crypto_service_mbedcrypto_config
    INTERFACE
        platform_common_interface
)
