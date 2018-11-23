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

#ifdef CONFIG_TASH
#ifdef CONFIG_KERNEL_CMDS
void kernel_register_utilcmds(void);
#endif
#ifdef CONFIG_FS_CMDS
void fs_register_utilcmds(void);
#endif
#ifdef CONFIG_NET_CMDS
void net_register_utilcmds(void);
void net_register_appcmds(void);
#endif
#endif							/* CONFIG_TASH */

#ifdef CONFIG_ENABLE_STACKMONITOR_CMD
void stkmon_logging(struct tcb_s *);
#endif
#endif							/* __APPS_INCLUDE_SYSTEM_UTILS_H */
