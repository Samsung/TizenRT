/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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

#ifndef __INCLUDE_TINYARA_HEALTH_MONITOR_H
#define __INCLUDE_TINYARA_HEALTH_MONITOR_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <sys/types.h>
#include <stdint.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define HEALTH_MONITOR_DEVPATH        "/dev/health_monitor"
#define HEALTH_MONITOR_MIN_TIMEOUT_MS 1U

/****************************************************************************
 * Public Types
 ****************************************************************************/

#ifdef CONFIG_HEALTH_MONITOR
struct health_monitor_s {
	clock_t deadline;		/* Absolute system tick */
	uint32_t timeout;		/* Timeout in ticks; zero means unregistered */
};
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef CONFIG_HEALTH_MONITOR
void health_monitor_register(void);
void health_monitor_initialize(void);
int health_monitor_start(uint32_t timeout_ms);
int health_monitor_stop(void);
void health_monitor_kick(void);
#endif /* CONFIG_HEALTH_MONITOR */

#endif /* __INCLUDE_TINYARA_HEALTH_MONITOR_H */
