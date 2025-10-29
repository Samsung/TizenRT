#-------------------------------------------------------------------------------
# Copyright (c) 2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

################################### FP ########################################

if (NOT DEFINED CONFIG_TFM_FP_ARCH)
    set(CONFIG_TFM_FP_ARCH "")
    return()
endif()

if (CONFIG_TFM_FP STREQUAL "hard")
    set(CONFIG_TFM_LAZY_STACKING         ON          CACHE BOOL      "Enable lazy stacking")
endif()
