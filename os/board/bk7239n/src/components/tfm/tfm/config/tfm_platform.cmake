#-------------------------------------------------------------------------------
# Copyright (c) 2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(TARGET_PATH "${CMAKE_SOURCE_DIR}/platform/ext/target")

if (NOT IS_ABSOLUTE "${TFM_PLATFORM}" AND NOT IS_DIRECTORY "${TARGET_PATH}/${TFM_PLATFORM}")
    # If TFM_PLATFORM is not a relative patch to ${TARGET_PATH}, then it could
    # be a platform name, for example an521. Search directories which contain
    # the "preload.cmake" and find the the match one.

    # Get the list of directories which have preload.cmake
    file(GLOB_RECURSE PLATFORM_PATHS ${TARGET_PATH} "preload.cmake")

    # Search the list with platform name and store the result in PLATFORM_PATHS
    list(FILTER PLATFORM_PATHS INCLUDE REGEX "${TFM_PLATFORM}")

    # Get the length of list PLATFORM_PATHS
    list(LENGTH PLATFORM_PATHS _PLATFORM_NUM)

    if (${_PLATFORM_NUM} STREQUAL 1)
        # Get the absolute path of the platform
        get_filename_component(PLATFORM_ABS_PATH ${PLATFORM_PATHS} DIRECTORY)
        set(TFM_PLATFORM ${PLATFORM_ABS_PATH} CACHE STRING "Target platform set as an absolute path." FORCE)
    elseif (${_PLATFORM_NUM} STREQUAL 0)
        Message(FATAL_ERROR "Platform ${TFM_PLATFORM} is not found in TF-M")
    elseif (${_PLATFORM_NUM} GREATER 1)
        Message(FATAL_ERROR "Two or more platforms ${TFM_PLATFORM} are found in TF-M")
    endif()
endif()

# If TFM_PLATFORM is an absolute path which maybe inputed by developer or
# transformed from platform name by the process above, it will be converted to
# relative path here.
if (IS_ABSOLUTE "${TFM_PLATFORM}")
    file(RELATIVE_PATH TFM_PLATFORM_RELATIVE_PATH ${TARGET_PATH} ${TFM_PLATFORM})
    set(TFM_PLATFORM "${TFM_PLATFORM_RELATIVE_PATH}" CACHE STRING "Target platform set as an relative path." FORCE)
endif()

# Some compiler flags depend on the CPU / platform config. This include should
# be run before the toolchain file so the compiler can be configured properly.
if (NOT EXISTS "${TARGET_PATH}/${TFM_PLATFORM}/preload.cmake" OR NOT EXISTS "${TARGET_PATH}/${TFM_PLATFORM}/CMakeLists.txt")
    Message(FATAL_ERROR "Unsupported TFM_PLATFORM ${TFM_PLATFORM}")
else()
    include(platform/ext/target/${TFM_PLATFORM}/preload.cmake)
endif()
