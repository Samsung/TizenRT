#-------------------------------------------------------------------------------
# Copyright (c) 2021-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

############################ Partitions ########################################

set(TFM_PSA_API                 OFF          CACHE BOOL      "Use PSA API instead of secure library model")
set(TFM_PARTITION_AUDIT_LOG     ON           CACHE BOOL      "Enable Audit Log partition")
set(CONFIG_TFM_PARTITION_META   OFF)
