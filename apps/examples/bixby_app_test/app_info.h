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
#ifndef __APP_INFO_H__
#define __APP_INFO_H__

#include "tizen_error.h"

#define TIZEN_ERROR_APPLICATION_MANAGER -0x01110000

typedef void* pkgmgrinfo_appinfo_h;

struct app_info_s {
	char *app_id;
	void *pkg_app_info;
};

typedef struct app_info_s *app_info_h;
typedef bool (*app_info_metadata_cb) (const char *metadata_key, const char *metadata_value, void *user_data);

/**
 * @brief Enumerations for Application Info Error.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum {
	APP_INFO_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	APP_INFO_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	APP_INFO_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	APP_INFO_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< Internal I/O error */
	APP_INFO_ERROR_NO_SUCH_APP = TIZEN_ERROR_APPLICATION_MANAGER | 0x01, /**< No such application */
	APP_INFO_ERROR_DB_FAILED = TIZEN_ERROR_APPLICATION_MANAGER | 0x03, /**< Database error  */
	APP_INFO_ERROR_INVALID_PACKAGE = TIZEN_ERROR_APPLICATION_MANAGER | 0x04, /**< Invalid package name */
	APP_INFO_ERROR_APP_NO_RUNNING = TIZEN_ERROR_APPLICATION_MANAGER | 0x05, /**< App is not running */
	APP_INFO_ERROR_REQUEST_FAILED = TIZEN_ERROR_APPLICATION_MANAGER | 0x06, /**< Internal aul request error */
	APP_INFO_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
	APP_INFO_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED /**< Not supported (Since 3.0) */
} app_info_error_e;

int app_info_get_localed_label(const char *app_id, const char *locale, char **label);
int app_info_create(const char *app_id, app_info_h *app_info);
int app_info_destroy(app_info_h app_info);
int app_info_foreach_metadata(app_info_h app_info, app_info_metadata_cb callback, void *user_data);

#endif
