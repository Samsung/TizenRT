#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(MCUBOOT_LOG_LEVEL                   "OFF"      CACHE STRING    "Level of logging to use for MCUboot [OFF, ERROR, WARNING, INFO, DEBUG]")
set(TFM_SPM_LOG_LEVEL                   TFM_SPM_LOG_LEVEL_SILENCE CACHE STRING "Set minsizerel SPM log level as Silence level")
set(TFM_PARTITION_LOG_LEVEL             TFM_PARTITION_LOG_LEVEL_SILENCE CACHE STRING "Set minsizerel SP log level as Silence level")
set(TFM_BL1_LOGGING                     OFF          CACHE BOOL      "Whether BL1 will log to uart")
