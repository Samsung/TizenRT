#-------------------------------------------------------------------------------
# Copyright (c) 2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (NOT TEST_NS_CORE)
    return()
endif()

add_subdirectory(tfm_core_test)
add_subdirectory(tfm_core_test_2)
