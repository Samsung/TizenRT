#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

add_subdirectory(log)

if(NS)
    add_subdirectory(app)
endif()

add_subdirectory(test)
