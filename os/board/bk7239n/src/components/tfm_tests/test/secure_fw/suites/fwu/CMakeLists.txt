#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0079 NEW)

if (NOT TFM_PARTITION_FIRMWARE_UPDATE)
    return()
endif()

if("${TFM_FWU_BOOTLOADER_LIB}" STREQUAL "mcuboot")
    add_subdirectory(mcuboot)
endif()
