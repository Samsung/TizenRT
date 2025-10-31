#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

add_library(bl1_2_lib STATIC)

target_include_directories(bl1_2_lib
    PUBLIC
        ./interface
    PRIVATE
        .
)

target_sources(bl1_2_lib
    PRIVATE
        ./image.c
)

target_link_libraries(bl1_2_lib
    PUBLIC
        platform_common_interface
    PRIVATE
        platform_bl1_interface
        bl1_1_shared_lib_interface
)
