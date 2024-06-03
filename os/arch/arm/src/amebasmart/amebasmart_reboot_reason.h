/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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

#ifndef __AMEBASMART_REBOOT_REASON_H__
#define __AMEBASMART_REBOOT_REASON_H__

#include <tinyara/reboot_reason.h>

enum {
	REBOOT_SYSTEM_DSLP_RESET     = REBOOT_BOARD_SPECIFIC1, /* System wake up from deep sleep */
	REBOOT_SYSTEM_SYS_RESET_CORE = REBOOT_BOARD_SPECIFIC2, /* System reset by Core */
	REBOOT_SYSTEM_BOD_RESET      = REBOOT_BOARD_SPECIFIC3, /* Brownout reset */
	REBOOT_SYSTEM_TZWD_RESET     = REBOOT_BOARD_SPECIFIC4, /* TrustZone Watch dog */
	REBOOT_SYSTEM_NP_LP_FAULT    = REBOOT_BOARD_SPECIFIC5, /* Reset due to fault in KM4/KM0 */
};

#endif /* __AMEBASMART_REBOOT_REASON_H__ */
