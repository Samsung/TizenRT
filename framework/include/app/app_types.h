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


#ifndef __TIZEN_APPFW_APP_TYPES_H__
#define __TIZEN_APPFW_APP_TYPES_H__

#include <app/tizen.h>

/**
 * @file app_types.h
 */

/**
 * @addtogroup CAPI_APPLICATION_MODULE
 * @{
 */

/**
 * @brief Enumeration for application error.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum {
	APP_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	APP_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	APP_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	APP_ERROR_INVALID_CONTEXT = TIZEN_ERROR_APPLICATION | 0x01, /**< Invalid application context */
	APP_ERROR_NO_SUCH_FILE = TIZEN_ERROR_NO_SUCH_FILE, /**< No such file or directory */
	APP_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED, /**< Not supported (Since 4.0) */
	APP_ERROR_ALREADY_RUNNING = TIZEN_ERROR_ALREADY_IN_PROGRESS, /**< Application is already running */
	APP_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied @if MOBILE (Since 2.3.1) @endif */
} app_error_e;


/**
 * @}
 */

#endif /* __TIZEN_APPFW_APP_TYPES_H__ */

