/*
 * Copyright (c) 2011 - 2016 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlog.h>
#include <app_common_internal.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "CAPI_APPFW_APPLICATION"

static const char *app_error_to_string(app_error_e error)
{
	switch (error) {
	case APP_ERROR_NONE:
		return "NONE";
	case APP_ERROR_INVALID_PARAMETER:
		return "INVALID_PARAMETER";
	case APP_ERROR_OUT_OF_MEMORY:
		return "OUT_OF_MEMORY";
	case APP_ERROR_INVALID_CONTEXT:
		return "INVALID_CONTEXT";
	case APP_ERROR_NO_SUCH_FILE:
		return "NO_SUCH_FILE";
	case APP_ERROR_ALREADY_RUNNING:
		return "ALREADY_RUNNING";
	default:
		return "UNKNOWN";
	}
}

int app_error(app_error_e error, const char *function, const char *description)
{
	if (description)
		LOGE("[%s] %s(0x%08x) : %s", function, app_error_to_string(error), error, description);
	else
		LOGE("[%s] %s(0x%08x)", function, app_error_to_string(error), error);

	return error;
}

