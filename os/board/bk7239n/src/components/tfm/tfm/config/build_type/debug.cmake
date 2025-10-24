#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(MBEDCRYPTO_BUILD_TYPE               relwithdebinfo CACHE STRING "Build type of Mbed Crypto library")
set(TFM_SPM_LOG_LEVEL                   TFM_SPM_LOG_LEVEL_DEBUG CACHE STRING "Set debug SPM log level as Debug level")
set(TFM_PARTITION_LOG_LEVEL             TFM_PARTITION_LOG_LEVEL_DEBUG CACHE STRING "Set debug SP log level as Debug level")

set(CONFIG_TFM_HALT_ON_CORE_PANIC       ON         CACHE BOOL      "On fatal errors in the secure firmware, halt instead of rebooting.")
