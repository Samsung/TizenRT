/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __STM32H745_SYSTEMRESET_H__
#define __STM32H745_SYSTEMRESET_H__

#include <tinyara/reboot_reason.h>

enum {
	REBOOT_SYSTEM_SFTR1ST_RESET = REBOOT_BOARD_SPECIFIC1,
	REBOOT_SYSTEM_SFTR2ST_RESET = REBOOT_BOARD_SPECIFIC2,
	REBOOT_SYSTEM_BOR_RESET     = REBOOT_BOARD_SPECIFIC3,
};

#endif /* __STM32H745_SYSTEMRESET_H__ */
