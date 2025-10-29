#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

add_library(bl1_1_lib INTERFACE)

target_sources(bl1_1_lib
    INTERFACE
        $<$<BOOL:${TFM_BL1_DEFAULT_PROVISIONING}>:${CMAKE_CURRENT_SOURCE_DIR}/provisioning.c>
        $<$<BOOL:${TFM_BL1_2_IN_OTP}>:${CMAKE_CURRENT_SOURCE_DIR}/image_otp.c>
        $<$<NOT:$<BOOL:${TFM_BL1_2_IN_OTP}>>:${CMAKE_CURRENT_SOURCE_DIR}/image_flash.c>
)

target_include_directories(bl1_1_lib
    INTERFACE
        interface
)

target_link_libraries(bl1_1_lib
    INTERFACE
        bl1_1_shared_lib
        platform_bl1
)
