#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# Set to not download submodules if that option is available
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.16.0")
    cmake_policy(SET CMP0097 NEW)
endif()

if ("${PLATFORM_PSA_ADAC_SOURCE_PATH}" STREQUAL "DOWNLOAD")
    FetchContent_Declare(libpsaadac
        GIT_REPOSITORY https://git.trustedfirmware.org/shared/psa-adac.git
        GIT_TAG ${PLATFORM_PSA_ADAC_VERSION}
    )

    FetchContent_GetProperties(libpsaadac)
    if (NOT libpsaadac_POPULATED)
        FetchContent_Populate(libpsaadac)
        set(PLATFORM_PSA_ADAC_SOURCE_PATH ${libpsaadac_SOURCE_DIR} CACHE PATH "Path to source directory of psa-adac." FORCE)
        set(PLATFORM_PSA_ADAC_BUILD_PATH ${libpsaadac_BINARY_DIR} CACHE PATH "Path to build directory of psa-adac.")
    endif ()
endif ()


set(PSA_ADAC_TARGET "trusted-firmware-m")
set(PSA_ADAC_MBEDTLS_INCLUDE "${MBEDCRYPTO_PATH}/include")
set(PSA_ADAC_TOOLCHAIN    FALSE    CACHE BOOL "Whether to use psa-adac toolchain." FORCE)

add_subdirectory(${PLATFORM_PSA_ADAC_SOURCE_PATH} ${PLATFORM_PSA_ADAC_BUILD_PATH})

target_link_libraries(trusted-firmware-m-psa-adac
    PRIVATE
        platform_bl2
)
