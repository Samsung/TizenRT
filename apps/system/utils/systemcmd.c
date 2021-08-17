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

#include <tinyara/config.h>
#include <sys/types.h>
#include <apps/shell/tash.h>
#ifdef CONFIG_SYSTEM_CMDS
#include <apps/system/utils.h>
#endif

#ifdef CONFIG_SYSTEM_CMDS
const static tash_cmdlist_t system_cmds[] = {
#if defined(CONFIG_ENABLE_CPULOAD)
	{"cpuload",  utils_cpuload,      TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_DATE)
	{"date",     utils_date,         TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_DMESG)
	{"dmesg",    utils_dmesg,        TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_ENV_GET)
	{"getenv",   utils_env_get,      TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_FREE)
	{"free",     utils_free,         TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_HEAPINFO)
	{"heapinfo", utils_heapinfo,     TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_IRQINFO)
	{"irqinfo",   utils_irqinfo,      TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_KILL)
	{"kill",     utils_kill,         TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_KILLALL)
	{"killall",  utils_killall,      TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_PRODCONFIG)
	{"prodconfig", utils_prodconfig, TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_PS)
	{"ps",       utils_ps,           TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_ENV_SET)
	{"setenv",   utils_env_set,      TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_STACKMONITOR)
	{"stkmon",   utils_stackmonitor, TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_TTRACE)
	{"ttrace",   utils_ttrace,       TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_TZSELECT)
	{"tzselect", utils_tzselect,	TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_ENV_UNSET)
	{"unsetenv", utils_env_unset,    TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_UPTIME)
	{"uptime",   utils_uptime,       TASH_EXECMD_SYNC},
#endif
	{NULL,       NULL,              0}
};

void system_register_utilcmds(void)
{
	tash_cmdlist_install(system_cmds);
}
#endif

