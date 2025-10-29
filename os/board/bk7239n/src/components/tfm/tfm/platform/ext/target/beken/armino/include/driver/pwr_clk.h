// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <modules/pm.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   get the 32k clock source configured by the customer
 *
 * This API get the 32k clock source configured by the customer
 *
 * @return
 *    - PM_LPO_SRC_X32K: support the external 32k clock
 *    - PM_LPO_SRC_ROSC: default support the ROSC
 */
// pm_lpo_src_e bk_clk_32k_customer_config_get(void);


#ifdef __cplusplus
}
#endif


