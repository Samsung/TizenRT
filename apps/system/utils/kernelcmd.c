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
#if defined(CONFIG_EXAMPLES_FOTA_SAMPLE)
extern void fota_sample_app_install(void);
#endif

#ifdef CONFIG_EXAMPLES_KERNEL_SAMPLE
extern int kernel_sample_main(int argc, char **args);
#endif
#ifdef CONFIG_EXAMPLES_TESTCASE
extern int tc_main(int argc, char **args);
#endif

#ifdef CONFIG_KERNEL_CMDS
const static tash_cmdlist_t kdbg_cmds[] = {
#if defined(CONFIG_ENABLE_DATE)
	{"date",     kdbg_date,         TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_DMESG)
	{"dmesg",    kdbg_dmesg,        TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_ENV_GET) && !defined(CONFIG_DISABLE_ENVIRON)
	{"getenv",   kdbg_env_get,      TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_FREE)
	{"free",     kdbg_free,         TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_HEAPINFO)
	{"heapinfo", kdbg_heapinfo,     TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_KILL)
	{"kill",     kdbg_kill,         TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_KILLALL)
	{"killall",  kdbg_killall,      TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_PS)
	{"ps",       kdbg_ps,           TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_ENV_SET) && !defined(CONFIG_DISABLE_ENVIRON)
	{"setenv",   kdbg_env_set,      TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_STACKMONITOR)
	{"stkmon",   kdbg_stackmonitor, TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_ENABLE_ENV_UNSET) && !defined(CONFIG_DISABLE_ENVIRON)
	{"unsetenv", kdbg_env_unset,    TASH_EXECMD_SYNC},
#endif
	{NULL,       NULL,              0}
};

void kernel_register_utilcmds(void)
{
	tash_cmdlist_install(kdbg_cmds);
}
#endif

const static tash_cmdlist_t kernel_appcmds[] = {
#if defined(CONFIG_EXAMPLES_KERNEL_SAMPLE)
	{"kernel_sample", kernel_sample_main, TASH_EXECMD_SYNC},
#endif
#if defined(CONFIG_EXAMPLES_TESTCASE)
	{"tc",            tc_main,            TASH_EXECMD_SYNC},
#endif
	{NULL,            NULL,               0}
};

void kernel_register_appcmds(void)
{
#if defined(CONFIG_EXAMPLES_FOTA_SAMPLE)
	fota_sample_app_install();
#endif
	tash_cmdlist_install(kernel_appcmds);
}
