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
#ifdef CONFIG_KERNEL_CMDS
#include "kdbg_commands.h"
#endif

#ifdef CONFIG_KERNEL_CMDS
const static tash_cmdlist_t kdbg_cmds[] = {
#if defined(CONFIG_ENABLE_CPULOAD_CMD)
	{"cpuload",  kdbg_cpuload,      TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_DATE_CMD)
	{"date",     kdbg_date,         TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_DMESG_CMD)
	{"dmesg",    kdbg_dmesg,        TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_ENV_GET_CMD) && !defined(CONFIG_DISABLE_ENVIRON)
	{"getenv",   kdbg_env_get,      TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_FREE_CMD)
	{"free",     kdbg_free,         TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_HEAPINFO_CMD)
	{"heapinfo", kdbg_heapinfo,     TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_IRQINFO_CMD)
	{"irqinfo",   kdbg_irqinfo,      TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_KILL_CMD)
	{"kill",     kdbg_kill,         TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_KILLALL_CMD)
	{"killall",  kdbg_killall,      TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_PS_CMD)
	{"ps",       kdbg_ps,           TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_ENV_SET_CMD) && !defined(CONFIG_DISABLE_ENVIRON)
	{"setenv",   kdbg_env_set,      TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_STACKMONITOR_CMD)
	{"stkmon",   kdbg_stackmonitor, TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_TTRACE)
	{"ttrace",   kdbg_ttrace,       TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_ENV_UNSET_CMD) && !defined(CONFIG_DISABLE_ENVIRON)
	{"unsetenv", kdbg_env_unset,    TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_UPTIME_CMD)
	{"uptime",   kdbg_uptime,       TASH_EXECMD_SYNC},
#endif
	{NULL,       NULL,              0}
};

void kernel_register_utilcmds(void)
{
	tash_cmdlist_install(kdbg_cmds);
}
#endif

