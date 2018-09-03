/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#include <stdbool.h>
#include "system_settings.h"

int system_settings_get_value_bool(system_settings_key_e key, bool *value)
{
	if (key == SYSTEM_SETTINGS_KEY_LOCALE_TIMEFORMAT_24HOUR) {
		*value = true;
		return SYSTEM_SETTINGS_ERROR_NONE;
	}

	*value = false;
	return SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED;
}
