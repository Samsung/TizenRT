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
#ifndef __SYSTEM_SETTINGS_H__
#define __SYSTEM_SETTINGS_H__

#include "tizen_error.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Enumeration for system settings error.
 * @since_tizen 2.3
 */
typedef enum {
	SYSTEM_SETTINGS_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	SYSTEM_SETTINGS_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	SYSTEM_SETTINGS_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	SYSTEM_SETTINGS_ERROR_IO_ERROR =  TIZEN_ERROR_IO_ERROR, /**< Internal I/O error */
	SYSTEM_SETTINGS_ERROR_PERMISSION_DENIED =  TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
	SYSTEM_SETTINGS_ERROR_NOT_SUPPORTED =  TIZEN_ERROR_NOT_SUPPORTED, /**< Not supported @if MOBILE (Since 2.3.1) @endif */
}
system_settings_error_e;


/**
 * @brief Enumeration for System Settings Key.
 * @since_tizen 2.3
 */
typedef enum {
	SYSTEM_SETTINGS_KEY_LOCALE_TIMEFORMAT_24HOUR,	/**< (bool) Indicates whether the 24-hour clock is used. If the value is @c false, the 12-hour clock is used. */
	SYSTEM_SETTINGS_KEY_SOUND_SILENT_MODE,			/**< GET (bool) Indicates whether the device is in the silent mode. */
	SYSTEM_SETTINGS_KEY_MAX,
} system_settings_key_e;


int system_settings_get_value_bool(system_settings_key_e key, bool *value);

#ifdef __cplusplus
}
#endif

#endif
