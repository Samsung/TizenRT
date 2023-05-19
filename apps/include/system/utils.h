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
#ifndef __APPS_INCLUDE_SYSTEM_UTILS_H
#define __APPS_INCLUDE_SYSTEM_UTILS_H
#include <tinyara/config.h>

#ifdef CONFIG_SYSTEM_CMDS
void system_register_utilcmds(void);
#endif
#ifdef CONFIG_SECURITY_LEVEL_CMDS
void security_level_register_utilcmds(void);
#endif
#ifdef CONFIG_FS_CMDS
void fs_register_utilcmds(void);
#endif
#ifdef CONFIG_NET_CMDS
void net_register_utilcmds(void);
void net_register_appcmds(void);
#endif

#ifdef CONFIG_ENABLE_STACKMONITOR_CMD
void stkmon_logging(struct tcb_s *);
#endif

#if defined(CONFIG_ENABLE_CPULOAD)
int utils_cpuload(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_DATE)
int utils_date(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_DMESG)
int utils_dmesg(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_ENV_GET)
int utils_env_get(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_ENV_SET)
int utils_env_set(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_ENV_UNSET)
int utils_env_unset(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_FREE)
int utils_free(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_HEAPINFO)
int utils_heapinfo(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_PRODCONFIG)
int utils_prodconfig(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_IRQINFO)
int utils_irqinfo(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_KILL)
int utils_kill(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_KILLALL)
int utils_killall(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_PS)
int utils_ps(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_STACKMONITOR)
int utils_stackmonitor(int argc, char **args);
#endif

#if defined(CONFIG_TTRACE)
int utils_ttrace(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_TZSELECT)
int utils_tzselect(int argc, char **args);
#endif

#if defined(CONFIG_ENABLE_UPTIME)
int utils_uptime(int argc, char **args);
#endif

#endif							/* __APPS_INCLUDE_SYSTEM_UTILS_H */
