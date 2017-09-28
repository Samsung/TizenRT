/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

#ifndef __APPS_SYSTEM_UTILS_KDBG_COMMANDS_H
#define __APPS_SYSTEM_UTILS_KDBG_COMMANDS_H

#include <tinyara/config.h>

#if defined(CONFIG_ENABLE_DATE)
int kdbg_date(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_DMESG)
int kdbg_dmesg(int argc, char **args);
#endif

#ifndef CONFIG_DISABLE_ENVIRON
#if defined(CONFIG_ENABLE_ENV_GET)
int kdbg_env_get(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_ENV_SET)
int kdbg_env_set(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_ENV_UNSET)
int kdbg_env_unset(int argc, char **args);
#endif
#endif							/* !CONFIG_DISABLE_ENVIRON */

#if defined(CONFIG_ENABLE_FREE)
int kdbg_free(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_HEAPINFO)
int kdbg_heapinfo(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_IRQINFO)
int kdbg_irqinfo(int argc, char **args);
#endif

#ifndef CONFIG_DISABLE_SIGNALS
#if defined(CONFIG_ENABLE_KILL)
int kdbg_kill(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_KILLALL)
int kdbg_killall(int argc, char **args);
#endif
#endif							/* !CONFIG_DISABLE_SIGNALS */

#if defined(CONFIG_ENABLE_PS)
int kdbg_ps(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_STACKMONITOR)
int kdbg_stackmonitor(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_STACKOPT)
int kdbg_stackopt(int argc, char **args);
#endif

#if defined(CONFIG_TTRACE)
int kdbg_ttrace(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_UPTIME)
int kdbg_uptime(int argc, char **args);
#endif

#endif							/* __APPS_SYSTEM_UTILS_KDBG_COMMANDS_H */
