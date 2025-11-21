#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15)

project("Bootloader" VERSION 0.1.0 LANGUAGES C ASM)

add_executable(bl2
    src/flash_map.c
    $<$<BOOL:${DEFAULT_MCUBOOT_SECURITY_COUNTERS}>:src/security_cnt.c>
    $<$<BOOL:${DEFAULT_MCUBOOT_FLASH_MAP}>:src/default_flash_map.c>
    $<$<BOOL:${MCUBOOT_DATA_SHARING}>:src/shared_data.c>
    $<$<BOOL:${PLATFORM_DEFAULT_PROVISIONING}>:src/provisioning.c>
)

add_executable(bl2_B
    src/flash_map.c
    $<$<BOOL:${DEFAULT_MCUBOOT_SECURITY_COUNTERS}>:src/security_cnt.c>
    $<$<BOOL:${DEFAULT_MCUBOOT_FLASH_MAP}>:src/default_flash_map.c>
    $<$<BOOL:${MCUBOOT_DATA_SHARING}>:src/shared_data.c>
    $<$<BOOL:${PLATFORM_DEFAULT_PROVISIONING}>:src/provisioning.c>
)

add_subdirectory(ext/mcuboot)

set_target_properties(bl2
    PROPERTIES
        SUFFIX ".axf"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

set_target_properties(bl2_B
    PROPERTIES
        SUFFIX ".axf"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_include_directories(bl2
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${MCUBOOT_PATH}/boot/bootutil/src>
)

target_include_directories(bl2_B
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${MCUBOOT_PATH}/boot/bootutil/src>
)

target_link_libraries(bl2
    PRIVATE
        tfm_boot_status
        $<$<BOOL:${TEST_BL2}>:mcuboot_tests>
)

target_link_libraries(bl2_B
    PRIVATE
        tfm_boot_status
        $<$<BOOL:${TEST_BL2}>:mcuboot_tests>
)

target_compile_options(bl2
    PRIVATE
        ${BL2_COMPILER_CP_FLAG}
)

target_compile_options(bl2_B
    PRIVATE
        ${BL2_COMPILER_CP_FLAG}
)

target_link_options(bl2
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-Wl,-Map=${CMAKE_BINARY_DIR}/bin/bl2.map>
        $<$<C_COMPILER_ID:ARMClang>:--map>
        $<$<C_COMPILER_ID:IAR>:--map\;${CMAKE_BINARY_DIR}/bin/bl2.map>
)

target_link_options(bl2_B
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-Wl,-Map=${CMAKE_BINARY_DIR}/bin/bl2_B.map>
        $<$<C_COMPILER_ID:ARMClang>:--map>
        $<$<C_COMPILER_ID:IAR>:--map\;${CMAKE_BINARY_DIR}/bin/bl2_B.map>
)

target_compile_definitions(bl2
    PRIVATE
        $<$<BOOL:${DEFAULT_MCUBOOT_FLASH_MAP}>:DEFAULT_MCUBOOT_FLASH_MAP>
        $<$<BOOL:${PLATFORM_PSA_ADAC_SECURE_DEBUG}>:PLATFORM_PSA_ADAC_SECURE_DEBUG>
        $<$<BOOL:${TEST_BL2}>:TEST_BL2>
        -DCONFIG_ENABLE_MCUBOOT_BL2=1
        CONFIG_BINARY_SIGNING=${CONFIG_BINARY_SIGNING}
)

target_compile_definitions(bl2_B
    PRIVATE
        $<$<BOOL:${DEFAULT_MCUBOOT_FLASH_MAP}>:DEFAULT_MCUBOOT_FLASH_MAP>
        $<$<BOOL:${PLATFORM_PSA_ADAC_SECURE_DEBUG}>:PLATFORM_PSA_ADAC_SECURE_DEBUG>
        $<$<BOOL:${TEST_BL2}>:TEST_BL2>
        -DCONFIG_ENABLE_MCUBOOT_BL2=1
        CONFIG_BINARY_SIGNING=${CONFIG_BINARY_SIGNING}
)

add_convert_to_bin_target(bl2)
add_convert_to_bin_target(bl2_B)
############################### BOOT HAL # #####################################

add_library(bl2_hal INTERFACE)

target_include_directories(bl2_hal
    INTERFACE
        include
)

############################### MBEDCRYPTO #####################################
add_library(bl2_mbedcrypto_config INTERFACE)

target_compile_definitions(bl2_mbedcrypto_config
    INTERFACE
        $<$<STREQUAL:${MCUBOOT_SIGNATURE_TYPE},RSA>:MCUBOOT_SIGN_RSA>
        $<$<STREQUAL:${MCUBOOT_SIGNATURE_TYPE},RSA>:MCUBOOT_SIGN_RSA_LEN=${MCUBOOT_SIGNATURE_KEY_LEN}>
        MBEDTLS_CONFIG_FILE="${MCUBOOT_MBEDCRYPTO_CONFIG_FILEPATH}"
        # Workaround for https://github.com/ARMmbed/mbedtls/issues/1077
        $<$<OR:$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv8-m.base>,$<STREQUAL:${TFM_SYSTEM_ARCHITECTURE},armv6-m>>:MULADDC_CANNOT_USE_R7>
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
set(MBEDTLS_TARGET_PREFIX bl2_)

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

target_link_libraries(${MBEDTLS_TARGET_PREFIX}mbedcrypto
    PUBLIC
        bl2_mbedcrypto_config
)

target_include_directories(${MBEDTLS_TARGET_PREFIX}mbedcrypto
    PUBLIC
        ${MBEDCRYPTO_PATH}/library
)

target_compile_options(bl2_mbedcrypto
    PRIVATE
        ${BL2_COMPILER_CP_FLAG}
)

target_compile_options(bl2_mbedtls
    PRIVATE
        ${BL2_COMPILER_CP_FLAG}
)

target_compile_options(bl2_mbedx509
    PRIVATE
        ${BL2_COMPILER_CP_FLAG}
)

############################### CODE SHARING ###################################

if (TFM_CODE_SHARING)
    target_share_symbols(bl2 ${CMAKE_CURRENT_SOURCE_DIR}/bl2_shared_symbols.txt)
    target_share_symbols(bl2_B ${CMAKE_CURRENT_SOURCE_DIR}/bl2_B_shared_symbols.txt)

    if (NOT EXISTS ${MBEDCRYPTO_PATH}/library/code_share.c)
        message(FATAL_ERROR "File ${MBEDCRYPTO_PATH}/library/code_share.c does not exist.
        Have the patch ${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/0005-Enable-crypto-code-sharing-between-independent-binar.patch
        been applied to the mbedcrypto repo at ${MBEDCRYPTO_PATH}?
        Hint: The command might be `cd ${MBEDCRYPTO_PATH} && git apply ${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/*.patch`")
    endif()
endif()
