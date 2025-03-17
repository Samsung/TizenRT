/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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

#ifndef __INCLUDE_TINYARA_SILENT_REBOOT_H
#define __INCLUDE_TINYARA_SILENT_REBOOT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define SILENT_REBOOT_DRVPATH     "/dev/silent_reboot"

typedef struct silent_reboot_status_s {
    int lock_count;
    int reboot_timezone_left;
    int reboot_delay_left;
} silent_reboot_status_t;

void silent_reboot_driver_register(void);

#ifdef CONFIG_SILENT_REBOOT
int silent_reboot_lock(void);
int silent_reboot_unlock(void);
int silent_reboot_delay(int timeout);
int silent_reboot_force_perform_after_timeout(int timeout);
bool silent_reboot_is_silent_mode(void);
int silent_reboot_get_status(silent_reboot_status_t *status);
void silent_reboot_initialize(void);
#else
#define silent_reboot_lock()   (0)
#define silent_reboot_unlock() (0)
#define silent_reboot_delay(timeout) (0)
#define silent_reboot_force_perform_after_timeout(timeout) (0)
#define silent_reboot_is_silent_mode() (false)
#define silent_reboot_get_status(status) (0)
#define silent_reboot_initialize()
#endif

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __INCLUDE_TINYARA_SILENT_REBOOT_H */
