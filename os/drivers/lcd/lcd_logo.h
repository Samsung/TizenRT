/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/

#ifndef __DRIVER_LCD_LOGO_H
#define __DRIVER_LCD_LOGO_H

#include <tinyara/config.h>

#define LCD_BLACK_VAL 0
#ifdef CONFIG_LCD_LOGO_52_340
#define LOGO_XRES 52
#define LOGO_YRES 340
#else
#define LOGO_XRES 0
#define LOGO_YRES 0
#endif      /* CONFIG_LCD_LOGO_52_340 */

#endif      /* __DRIVER_LCD_LOGO_H */

