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
#ifdef __LINUX__
#include "netlog.h"
#else
#include <tinyara/net/netlog.h>
#endif

int netlogger_print(netlog_module_e mod, netlog_log_level_e level, const char *format, ...)
{
// To-do
	return 0;
}

int netlog_set_level(netlog_module_e module, netlog_log_level_e level)
{
// To-do
	return 0;
}

int netlog_reset(void)
{
// To-do
	return 0;
}
