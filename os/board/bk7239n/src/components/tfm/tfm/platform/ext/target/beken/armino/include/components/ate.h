// Copyright 2020-2021 Beken
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

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ATE API
 * @defgroup bk_ate API group
 * @{
 */
#if CONFIG_ATE
/**
 * @brief     Init the ATE driver
 *
 * This API inits the resoure for ATE mode.
 *
 * @attention 1. This API re-use UART TX PIN.(UART Port defined as CONFIG_UART_ATE_PORT in bkxxxx.defconfig)
 *               it should be called before UART APIs and after GPIO inited.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
int bk_ate_init(void);

/**
 * @brief     Deinit the ATE driver:Do nothing
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
int bk_ate_deinit(void);

/**
 * @brief     Get the ATE mode is whether enable
 *
 * @attention 1. This API depends on HW(UART TX PIN is whether inputed low level when DUT power on)
 *				 After the bk_ate_init, it will not change the ATE enable status.
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bool ate_is_enabled(void);
/**
 * @brief ate enable or disable
 *
 * enable or disable ate
 *
 * @attention
 * - This API is used to ctrl ate(enable or disable)
 *
 * @param
 * -0x0:close ate feature;0x1:open ate feature
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
int bk_ate_ctrl(bool enable);
#else
bool ate_is_enabled(void);
#endif
/**
 * @}
 */
 

#ifdef __cplusplus
}
#endif
