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
#ifndef _IOTBUS_DEBUG_H__
#define _IOTBUS_DEBUG_H__

#include <tinyara/config.h>
#include <debug.h>

#ifdef CONFIG_CPP_HAVE_VARARGS
#ifdef CONFIG_DEBUG_IOTBUS
#define ibdbg(format, ...)			dbg(format, ##__VA_ARGS__)
#define ibvdbg(format, ...)			vdbg(format, ##__VA_ARGS__)
#else
#define ibdbg(...)
#define ibvdbg(...)
#endif
#else /* !CONFIG_CPP_HAVE_VARARGS */
#ifdef CONFIG_DEBUG_IOTBUS
#define ibdbg		dbg
#define ibvdbg		vdbg
#else
#define ibdbg		(void)
#define ibvdbg		(void)
#endif
#endif /* CONFIG_CPP_HAVE_VARARGS */

#endif