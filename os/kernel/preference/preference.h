/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#ifndef __KERNEL_PREFERENCE_PREFERENCE_H
#define __KERNEL_PREFERENCE_PREFERENCE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdbool.h>
#include <tinyara/preference.h>

int preference_write_key(preference_data_t *data);
int preference_read_key(preference_data_t *data);
int preference_remove_key(int type, const char *key);
int preference_remove_all_key(int type, const char *path);
int preference_check_key(int type, const char *key, bool *result);
#if !defined(CONFIG_DISABLE_MQUEUE) && !defined(CONFIG_DISABLE_SIGNAL)
void preference_send_cb_msg(int type, const char *key);
#endif
int preference_register_callback(preference_callback_t *data);
int preference_unregister_callback(const char *key, int type);
int preference_get_private_keypath(const char *key, char **path);
void preference_clear_callbacks(pid_t pid);
#endif							/* __KERNEL_PREFERENCE_PREFERENCE_H */
