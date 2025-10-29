#-------------------------------------------------------------------------------
# Copyright (c) 2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if (TEST_NS_SLIH_IRQ)
    add_subdirectory(tfm_slih_test_service)
endif()

if (TEST_NS_FLIH_IRQ)
    add_subdirectory(tfm_flih_test_service)
endif()
