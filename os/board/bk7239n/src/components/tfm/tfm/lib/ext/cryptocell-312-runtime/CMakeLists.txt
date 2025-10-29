#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

project("CC312 Runtime Library" LANGUAGES C ASM)

cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0079 NEW)

set(CC312_LIB_TARGET            ${CC312_LIB_PREFIX}cc312)
set(CC312_PAL_TARGET            ${CC312_LIB_PREFIX}cc312_pal)
set(CC312_CDMPU_TARGET          ${CC312_LIB_PREFIX}cc312_cdmpu)
set(CC312_MBEDTLS_API_TARGET    ${CC312_LIB_PREFIX}cc312_mbedtls_api)
set(CC312_PSA_DRIVER_API_TARGET ${CC312_LIB_PREFIX}cc312_psa_driver_api)

# Add library targets for this project
add_library(${CC312_LIB_TARGET} STATIC EXCLUDE_FROM_ALL)
add_library(${CC312_PAL_TARGET} STATIC EXCLUDE_FROM_ALL)
add_library(${CC312_CDMPU_TARGET} INTERFACE)

# Library interface components
add_library(${CC312_MBEDTLS_API_TARGET} STATIC EXCLUDE_FROM_ALL)
add_library(${CC312_PSA_DRIVER_API_TARGET} STATIC EXCLUDE_FROM_ALL)

add_subdirectory(codesafe)
add_subdirectory(host)
add_subdirectory(shared)

#  Platform depedency must be defined at this point scope
if (NOT TARGET ${CC312_PLATFORM_DEPENDENCY})
  message(ERROR "CC312_PLATFORM_DEPENDENCY target is not available for CC-312")
endif()

########################## LIB #################################################

target_compile_definitions(${CC312_LIB_TARGET}
    PRIVATE
        DLLI_MAX_BUFF_SIZE=0x10000
        CC_MNG_MIN_BACKUP_SIZE_IN_BYTES=512
        CC_SUPPORT_PKA_64_16
    INTERFACE
        USE_MBEDTLS_CRYPTOCELL
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:relwithdebinfo>>:CC_PAL_MAX_LOG_LEVEL=3>
    PUBLIC
        CC_IOT
)
target_link_libraries(${CC312_LIB_TARGET}
    PRIVATE
        ${CC312_PAL_TARGET}
        ${CC312_LIB_PREFIX}mbedcrypto
        ${CC312_PLATFORM_DEPENDENCY}
    INTERFACE
        ${CC312_MBEDTLS_API_TARGET}
)

########################## PAL #################################################

target_compile_definitions(${CC312_PAL_TARGET}
    PUBLIC
        CC_IOT
    INTERFACE
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:relwithdebinfo>>:CC_PAL_MAX_LOG_LEVEL=3>
)

target_link_libraries(${CC312_PAL_TARGET}
    PRIVATE
        ${CC312_PLATFORM_DEPENDENCY}
)

########################## CDMPU ###############################################

target_link_libraries(${CC312_CDMPU_TARGET}
    INTERFACE
        ${CC312_PAL_TARGET}
)

target_compile_definitions(${CC312_CDMPU_TARGET}
    INTERFACE
        DLLI_MAX_BUFF_SIZE=0x10000
        USE_MBEDTLS_CRYPTOCELL
        CC_MNG_MIN_BACKUP_SIZE_IN_BYTES=512
        CC_IOT
        CC_SUPPORT_PKA_64_16
)

########################## DRIVER INTERFACE ####################################

target_compile_definitions(${CC312_PSA_DRIVER_API_TARGET}
    PUBLIC
        MBEDTLS_PSA_CRYPTO_DRIVERS
        PSA_CRYPTO_DRIVER_CC3XX
)

target_link_libraries(${CC312_PSA_DRIVER_API_TARGET}
    PUBLIC
        ${CC312_LIB_PREFIX}mbedcrypto
    INTERFACE
        ${CC312_LIB_TARGET}
    PRIVATE
        ${CC312_PLATFORM_DEPENDENCY}
)

target_link_libraries(${CC312_MBEDTLS_API_TARGET}
    PUBLIC
        ${CC312_LIB_PREFIX}mbedcrypto
    INTERFACE
        ${CC312_LIB_TARGET}
    PRIVATE
        ${CC312_PLATFORM_DEPENDENCY}
)
