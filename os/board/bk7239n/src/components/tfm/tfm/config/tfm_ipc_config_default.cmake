#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

############################ Partitions ########################################

set(TFM_PSA_API                 ON          CACHE BOOL      "Use PSA API instead of secure library model")
set(CONFIG_TFM_SPM_BACKEND_IPC  ON)
set(CONFIG_TFM_SPM_BACKEND_SFN  OFF)
set(CONFIG_TFM_PARTITION_META   ON)
