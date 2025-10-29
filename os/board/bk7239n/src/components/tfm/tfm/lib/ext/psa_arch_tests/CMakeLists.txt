#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

#Set to not download submodules if that option is available
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.16.0")
    cmake_policy(SET CMP0097 NEW)
endif()

if ("${PSA_ARCH_TESTS_PATH}" STREQUAL "DOWNLOAD")
    find_package(Git)
    file(GLOB PATCH_FILES *.patch)

    if (PATCH_FILES)
        FetchContent_Declare(psa_arch_tests
            GIT_REPOSITORY https://github.com/ARM-software/psa-arch-tests.git
            GIT_TAG ${PSA_ARCH_TESTS_VERSION}
            GIT_SHALLOW FALSE
            GIT_PROGRESS TRUE
            GIT_SUBMODULES ""
            PATCH_COMMAND ${GIT_EXECUTABLE} apply ${PATCH_FILES}
        )
    else()
        FetchContent_Declare(psa_arch_tests
            GIT_REPOSITORY https://github.com/ARM-software/psa-arch-tests.git
            GIT_TAG ${PSA_ARCH_TESTS_VERSION}
            GIT_SHALLOW FALSE
            GIT_PROGRESS TRUE
            GIT_SUBMODULES ""
        )
    endif()

    FetchContent_GetProperties(psa_arch_tests)
    if(NOT psa_arch_tests_POPULATED)
        FetchContent_Populate(psa_arch_tests)
        set(PSA_ARCH_TESTS_PATH ${psa_arch_tests_SOURCE_DIR} CACHE PATH "Path to PSA_ARCH_TESTS (or DOWNLOAD to get automatically" FORCE)
    endif()
endif()
