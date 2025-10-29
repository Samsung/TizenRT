#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

include(cmake/version.cmake)

############################ CONFIGURATION #####################################

# Configure TFM_PLATFORM
include(${CMAKE_SOURCE_DIR}/config/tfm_platform.cmake)

set(TFM_ALLOWED_LOG_LEVELS OFF ERROR WARNING INFO DEBUG)
list(FIND TFM_ALLOWED_LOG_LEVELS ${CONFIG_TFM_LOG_LEVEL} TFM_LOG_LEVEL_ID)

if(TFM_SYSTEM_MVE)
    message(FATAL_ERROR "Hardware MVE is currently not supported in TF-M")
endif()
if(TFM_SYSTEM_DSP)
    message(FATAL_ERROR "Hardware DSP is currently not supported in TF-M")
endif()

# The default build type is release. If debug symbols are needed then
# -DCMAKE_BUILD_TYPE=debug should be used (likewise with other build types)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type: [Debug, Release, RelWithDebInfo, MinSizeRel]" FORCE)
endif()

include(config/set_config.cmake)

if(NOT ${CMAKE_GENERATOR} STREQUAL "Unix Makefiles" AND
   NOT ${CMAKE_GENERATOR} STREQUAL "Ninja")
    Message(FATAL_ERROR "Unsupported generator ${CMAKE_GENERATOR}. Hint: Try -G\"Unix Makefiles\"")
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

############################### Compiler configuration #########################

#Legacy compat option - load CMAKE_TOOLCHAIN_FILE as a TFM_TOOLCHAIN_FILE
if (CMAKE_TOOLCHAIN_FILE)
    message(DEPRECATION "SETTING CMAKE_TOOLCHAIN_FILE is deprecated. It has been replaced with TFM_TOOLCHAIN_FILE.")
    message(DEPRECATION "INTERPRETING -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} as -DTFM_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
    set(TFM_TOOLCHAIN_FILE ${CMAKE_TOOLCHAIN_FILE})
    unset(CMAKE_TOOLCHAIN_FILE)
endif()

if (NOT IS_ABSOLUTE ${TFM_TOOLCHAIN_FILE})
    message(FATAL_ERROR "SETTING CMAKE_TOOLCHAIN_FILE no longer accepts relative paths. Please supply an absolute path or instead use TFM_TOOLCHAIN_FILE (which does accept relative paths)")
endif()

include(${TFM_TOOLCHAIN_FILE})
set(CMAKE_PROJECT_INCLUDE_BEFORE ${CMAKE_SOURCE_DIR}/cmake/disable_compiler_detection.cmake)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config/armino_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config/armino_config.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/config)

set(TFM_VERSION "0.0.0")
project("Trusted Firmware M" VERSION ${TFM_VERSION} LANGUAGES C ASM)
tfm_toolchain_reload_compiler()

# Synchronise the install path variables. If CMAKE_INSTALL_PREFIX is manually
# set then set both to the value of that, else set both to the value of
# TFM_INSTALL_PATH. This has to be done after the call to `project()`.
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX  ${TFM_INSTALL_PATH} CACHE PATH  "" FORCE)
else()
    set(TFM_INSTALL_PATH  ${CMAKE_INSTALL_PREFIX} CACHE PATH  "Path to which to install TF-M files" FORCE)
endif()

############################ Config Check ######################################

include(${CMAKE_SOURCE_DIR}/config/check_config.cmake)

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/platform/ext/target/${TFM_PLATFORM}/check_config.cmake)
    include(platform/ext/target/${TFM_PLATFORM}/check_config.cmake)
endif()

################################################################################

add_subdirectory(lib/ext)
add_subdirectory(lib/fih)
add_subdirectory(tools)
add_subdirectory(secure_fw)

if(NS OR TFM_S_REG_TEST OR TFM_NS_REG_TEST OR TEST_BL2 OR TEST_BL1_1 OR TEST_BL1_2)
    add_subdirectory(${TFM_TEST_REPO_PATH}/tfm_tests ${CMAKE_CURRENT_BINARY_DIR}/tf-m-tests)
endif()

add_subdirectory(interface)
if(BL2)
    add_subdirectory(bl2)
endif()

if(BL1 AND PLATFORM_DEFAULT_BL1)
    add_subdirectory(bl1/bl1_2)
    add_subdirectory(bl1/bl1_1)
endif()

add_subdirectory(platform)

include(cmake/install.cmake)

if(CRYPTO_HW_ACCELERATOR)
    add_subdirectory(platform/ext/accelerator)
endif()
