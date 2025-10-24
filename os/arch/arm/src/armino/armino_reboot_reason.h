/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

#ifndef __ARMINO_REBOOT_REASON_H__
#define __ARMINO_REBOOT_REASON_H__

#include <tinyara/reboot_reason.h>
#include "system.h"

enum {
	REBOOT_SYSTEM_SOFT_RESET     = REBOOT_BOARD_SPECIFIC1, /* System soft reset */
	REBOOT_SYSTEM_SUPER_DEEP     = REBOOT_BOARD_SPECIFIC2, /* System reset by super deep sleep */
 
};

#endif /* __ARMINO_REBOOT_REASON_H__ */
