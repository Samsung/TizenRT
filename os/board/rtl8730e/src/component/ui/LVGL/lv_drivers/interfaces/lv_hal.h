/*
 * Copyright (c) 2022 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless reqUired by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef UI_LVGL_LV_DRIVERS_LV_HAL_H
#define UI_LVGL_LV_DRIVERS_LV_HAL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void lv_hal_init();

#ifdef CONFIG_IMAGE_SD_CARD
void sd_card_mount()
#endif

#ifdef __cplusplus
}
#endif

#endif // UI_LVGL_LV_DRIVERS_LV_HAL_H
/** @} */
