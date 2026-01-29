#-------------------------------------------------------------------------------
# Copyright (c) 2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------


########  WARNING: This is an auto-generated file. Do not edit!  ########

if(TFM_PSA_API)
    set(CONFIG_TFM_PSA_API_SFN_CALL        0        PARENT_SCOPE)
    set(CONFIG_TFM_PSA_API_CROSS_CALL      0      PARENT_SCOPE)
    set(CONFIG_TFM_PSA_API_SUPERVISOR_CALL 1 PARENT_SCOPE)

    if((CONFIG_TFM_FP GREATER 0) AND CONFIG_TFM_SPM_BACKEND_SFN)
        message(FATAL_ERROR "FP is not supported for SFN model.")
    endif()
endif()
