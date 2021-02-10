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
#ifndef __INCLUDE_REBOOT_REASON_H
#define __INCLUDE_REBOOT_REASON_H

/********************************************************************************************
 * Included Files
 ********************************************************************************************/

typedef enum {
	REBOOT_REASON_INITIALIZED	= 0,
	REBOOT_SYSTEM_DATAABORT		= 1,	/* Data abort */
	REBOOT_SYSTEM_PREFETCHABORT	= 2,	/* Prefetch abort */
	REBOOT_SYSTEM_MEMORYALLOCFAIL	= 3,	/* Memory allocation failure */
	REBOOT_SYSTEM_WATCHDOG		= 4,	/* Watchdog timeout */
	REBOOT_SYSTEM_HW_RESET		= 5,	/* HW power reset */
	REBOOT_SYSTEM_USER_INTENDED	= 6,	/* Reboot from user intention */
	REBOOT_SYSTEM_WIFICORE_WATCHDOG = 11,	/* Wi-Fi Core Watchdog Reset */
	REBOOT_SYSTEM_WIFICORE_PANIC    = 12,	/* Wi-Fi Core Panic */
	REBOOT_SYSTEM_BINARY_UPDATE	= 34,	/* Reboot for Binary Update */
	REBOOT_UNKNOWN 	 		= 99,
} reboot_reason_code_t;

#endif					/* __INCLUDEREBOOT_REASON_H */
