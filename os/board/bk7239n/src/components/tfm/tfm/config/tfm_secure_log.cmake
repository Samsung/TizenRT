#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# Secure regression tests also require SP log function
# Enable SP log raw dump when SP log level is higher than silence or TF-M
# regression test is enabled.
if ((NOT ${TFM_PARTITION_LOG_LEVEL} STREQUAL TFM_PARTITION_LOG_LEVEL_SILENCE)
    OR TFM_S_REG_TEST OR TFM_NS_REG_TEST)
    set(TFM_SP_LOG_RAW_ENABLED ON)
endif()

# SP log relies on SPM log.
# Enable SPM log when SPM log level is higher than silence or SP log is active.
if ((NOT ${TFM_SPM_LOG_LEVEL} STREQUAL TFM_SPM_LOG_LEVEL_SILENCE)
    OR TFM_SP_LOG_RAW_ENABLED)
    set(TFM_SPM_LOG_RAW_ENABLED ON)
endif()
