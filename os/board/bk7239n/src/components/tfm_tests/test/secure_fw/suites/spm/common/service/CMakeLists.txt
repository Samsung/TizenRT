#-------------------------------------------------------------------------------
# Copyright (c) 2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

add_library(spm_test_service_common INTERFACE)

target_include_directories(spm_test_service_common
    INTERFACE
        .
        ./..
)
