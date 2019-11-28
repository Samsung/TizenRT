/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// #include <glib.h>
// #include <aul.h>
// #include <aul_watchdog.h>

#include "appcore_watchdog.h"
#include "appcore_watchdog_private.h"

EXPORT_API int appcore_watchdog_enable(void)
{
	// _W("[__APPCORE_WATCHDOG__] enable");
	// return aul_watchdog_enable();
}

EXPORT_API int appcore_watchdog_disable(void)
{
	// _W("[__APPCORE_WATCHDOG__] kick");
	// return aul_watchdog_disable();
}

EXPORT_API int appcore_watchdog_kick(void)
{
	// _W("[__APPCORE_WATCHDOG__] kick");
	// return aul_watchdog_kick();
}
