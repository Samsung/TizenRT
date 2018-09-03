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
#ifndef __APP_MANAGER_H__
#define __APP_MANAGER_H__ 

#include <errno.h>
#include "app_info.h"
#include "tizen_error.h"

// pkbuild #define app_info_h char
#define TIZEN_ERROR_NONE 0
#define TIZEN_ERROR_PERMISSION_DENIED -EACCES
#define TIZEN_ERROR_APPLICATION_MANAGER -0x01110000

typedef enum {
	APP_MANAGER_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	APP_MANAGER_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	APP_MANAGER_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	APP_MANAGER_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< Internal I/O error */
	APP_MANAGER_ERROR_NO_SUCH_APP = TIZEN_ERROR_APPLICATION_MANAGER | 0x01, /**< No such application */
	APP_MANAGER_ERROR_DB_FAILED = TIZEN_ERROR_APPLICATION_MANAGER | 0x03, /**< Database error  */
	APP_MANAGER_ERROR_INVALID_PACKAGE = TIZEN_ERROR_APPLICATION_MANAGER | 0x04, /**< Invalid package name */
	APP_MANAGER_ERROR_APP_NO_RUNNING = TIZEN_ERROR_APPLICATION_MANAGER | 0x05, /**< App is not running */
	APP_MANAGER_ERROR_REQUEST_FAILED = TIZEN_ERROR_APPLICATION_MANAGER | 0x06, /**< Internal aul request error */
	APP_MANAGER_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
} app_manager_error_e;

/**
 * @brief Enumeration for the application state
 * @since_tizen 3.0
 */
typedef enum {
    APP_STATE_UNDEFINED, /**< The undefined state */
    APP_STATE_FOREGROUND, /**< The UI application is running in the foreground. */
    APP_STATE_BACKGROUND, /**< The UI application is running in the background. */
    APP_STATE_SERVICE, /**< The Service application is running. */
    APP_STATE_TERMINATED, /**< The application is terminated. */
} app_state_e;


struct app_context_s {
	char *app_id;
	pid_t pid;
	char *pkg_id;
	app_state_e app_state;
	bool is_sub_app;
	char *instance_id;
};
typedef struct app_context_s *app_context_h;

int app_manager_error(app_manager_error_e error, const char *function, const char *description);
int app_manager_get_app_id(pid_t pid, char **app_id);
int app_manager_get_app_context(const char *app_id, app_context_h *app_context);
int app_manager_resume_app(app_context_h app_context);
int app_manager_get_app_info(const char *app_id, app_info_h *app_info);
int app_manager_is_running(const char *app_id, bool *running);
#endif //__APP_MANAGER_H__
