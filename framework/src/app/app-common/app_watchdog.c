/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd All Rights Reserved
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlog.h>
#include <app/app_common_extension.h>

#include "appcore_watchdog.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "CAPI_APPFW_APPLICATION"

int app_watchdog_timer_enable(void)
{
	int r;

	r = appcore_watchdog_enable();
	if (r != 0) {
		LOGE("Failed to enable watchdog timer");
		return APP_ERROR_INVALID_CONTEXT;
	}

	return APP_ERROR_NONE;
}

int app_watchdog_timer_disable(void)
{
	int r;

	r = appcore_watchdog_disable();
	if (r != 0) {
		LOGE("Failed to disable watchdog timer");
		return APP_ERROR_INVALID_CONTEXT;
	}

	return APP_ERROR_NONE;
}

int app_watchdog_timer_kick(void)
{
	int r;

	r = appcore_watchdog_kick();
	if (r != 0) {
		LOGE("Failed to kick watchdog timer");
		return APP_ERROR_INVALID_CONTEXT;
	}

	return APP_ERROR_NONE;
}
