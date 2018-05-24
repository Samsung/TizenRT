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

#ifndef __APPS_SYSTEM_UTILS_NETCMD_H
#define __APPS_SYSTEM_UTILS_NETCMD_H

#ifndef MAX
#define MAX(a, b) (a > b ? a : b)
#endif

/* Common, message formats */

#define syntax "%s: syntax error\n"
#define fmtargrequired "%s: missing required argument(s)\n"
#define fmtnomatching "%s: no matching %s\n"
#define fmtarginvalid "%s: argument invalid\n"
#define fmtargrange "%s: value out of range\n"
#define fmtcmdnotfound "%s: command not found\n"
#define fmtnosuch "%s: no such %s: %s\n"
#define fmttoomanyargs "%s: too many arguments\n"
#define fmtdeepnesting "%s: nesting too deep\n"
#define fmtcontext "%s: not valid in this context\n"
#define fmtcmdfailed "%s: %s failed: %s\n"
#define fmtcmdoutofmemory "%s: out of memory\n"
#define fmtinternalerror "%s: Internal error\n"
#ifndef CONFIG_DISABLE_SIGNALS
#define fmtsignalrecvd "%s: Interrupted by signal\n"
#endif

#endif //__APPS_SYSTEM_UTILS_NETCMD_H
