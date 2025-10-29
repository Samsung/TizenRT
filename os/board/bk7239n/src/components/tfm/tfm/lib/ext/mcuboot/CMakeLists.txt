#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# Set to not download submodules if that option is available.
# Because of https://gitlab.kitware.com/cmake/cmake/-/issues/20579 CMP0097 is
# non-functional until cmake 3.18.0.
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.18.0")
    cmake_policy(SET CMP0097 NEW)
    set(MCUBOOT_SUBMODULES "")
else()
    # Workaround for not having CMP0097. Fetch the smallest submodule.
    set(MCUBOOT_SUBMODULES "boot/cypress/libs/cy-mbedtls-acceleration/")
endif()

if ("${MCUBOOT_PATH}" STREQUAL "DOWNLOAD")
    FetchContent_Declare(mcuboot
        GIT_REPOSITORY https://github.com/mcu-tools/mcuboot.git
        GIT_TAG ${MCUBOOT_VERSION}
        GIT_SHALLOW FALSE
        GIT_PROGRESS TRUE
        GIT_SUBMODULES "${MCUBOOT_SUBMODULES}"
    )
    # ToDo: set GIT_SHALLOW to 'TRUE' when MCUBOOT_VERSION will be set to a tag
    # (instead of a commit hash) with the new release.

    FetchContent_GetProperties(mcuboot)
    if(NOT mcuboot_POPULATED)
        FetchContent_Populate(mcuboot)
        set(MCUBOOT_PATH ${mcuboot_SOURCE_DIR} CACHE PATH "Path to MCUBOOT (or DOWNLOAD to get automatically" FORCE)
    endif()
endif()
