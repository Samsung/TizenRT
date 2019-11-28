/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __APPCORE_WATCHDOG_H__
#define __APPCORE_WATCHDOG_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enables watchdog timer.
 *
 * @return      @c 0 on success,
 *              otherwise a negative error value
 */
int appcore_watchdog_enable(void);

/**
 * @brief Disables watchdog timer.
 *
 * @return      @c 0 on success,
 *              otherwise a negative error value
 */
int appcore_watchdog_disable(void);

/**
 * @brief Kicks watchdog timer.
 *
 * @return      @c 0 on success,
 *              otherwise a negative error value
 */
int appcore_watchdog_kick(void);

#ifdef __cplusplus
}
#endif

#endif /* __APPCORE_WATCHDOG_H__ */
