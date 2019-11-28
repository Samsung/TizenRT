/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <app/service_app.h>
#include <app/bundle.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Called before create-callback
 * @since_tizen 4.0
 * @param[in] argc The argument count
 * @param[in] argv The argument vector
 * @param[in] user_data The user data passed from the callback registration function
 * @see service_app_main_ext()
 */
typedef void (*service_app_loop_method_init_cb)(int argc, char **argv, void *user_data);

/**
 * @brief Called after terminate-callback
 * @since_tizen 4.0
 * @param[in] user_data The user data passed from the callback registration function
 * @see service_app_main_ext()
 */
typedef void (*service_app_loop_method_fini_cb)(void);


/**
 * @brief Called to run main loop
 * @since_tizen 4.0
 * @param[in] user_data The user data passed from the callback registration function
 * @see service_app_main_ext()
 */
typedef void (*service_app_loop_method_run_cb)(void *user_data);

/**
 * @brief Called to exit main loop
 * @since_tizen 4.0
 * @param[in] user_data The user data passed from the callback registration function
 * @see	service_app_maini_ext()
 */
typedef void (*service_app_loop_method_exit_cb)(void *user_data);

/**
 * @brief The structure type containing the set of callback functions to provide methods
 * @since_tizen tizen 4.0
 */
typedef struct {
	service_app_loop_method_init_cb init;
	service_app_loop_method_fini_cb fini;
	service_app_loop_method_run_cb run;
	service_app_loop_method_exit_cb exit;
} service_app_loop_method_s;

/**
 * @brief Runs the main loop of the application until service_app_exit() is called.
 * @details This function is the main entry point of the Tizen service application.
 * @since_tizen 4.0
 * @param[in] argc The argument count
 * @param[in] argv The argument vector
 * @param[in] callback The set of callback functions to handle application events
 * @param[in] method The set of callback functions to be used for entering and exiting main loop
 * @param[in] user_data The user data to be passed to the callback functions
 * @return @c 0 on success,
 *         otherwise a negative error value.
 * @retval #APP_ERROR_NONE Successful
 * @retval #APP_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_ERROR_INVALID_CONTEXT The application is launched illegally, not launched by the launch system.
 * @retval #APP_ERROR_ALREADY_RUNNING The main loop has already started
 * @see service_app_create_cb()
 * @see service_app_terminate_cb()
 * @see service_app_control_cb()
 * @see service_app_exit()
 * @see #service_app_lifecycle_callback_s
 */
int service_app_main_ext(int argc, char **argv, service_app_lifecycle_callback_s *callback,
	       service_app_loop_method_s *method, void *user_data);


/**
 * @brief Enumeration for the job handler
 * @since_tizen 4.0
 * @remarks This is for App Framework internally.
 */
typedef enum {
	SERVICE_APP_JOB_STATUS_START,
	SERVICE_APP_JOB_STATUS_STOP,
} service_app_job_status_e;

/**
 * @brief Called when the job is triggered.
 * @since_tizen 4.0
 * @param[in]   job_status      The status of the job
 * @param[in]   job_id          The ID of the job
 * @param[in]   job_data        The data of the job
 * @param[in]   user_data       The user data passed from the callback registration function
 * @remarks This is for App Framework internally.
 */
typedef int (*service_app_job_cb)(int job_status, const char *job_id,
		bundle *job_data, void *user_data);

/**
 * @brief The job handle.
 * @since_tizen 4.0
 * @remarks This is for App Framework internally.
 */
typedef struct service_app_job_s *service_app_job_h;

/**
 * @brief Adds the job handler.
 * @since_tizen 4.0
 * @param[in]    job_id         The ID of the job
 * @param[in]    callback       The job callback function
 * @param[in]    user_data      The user data to be passed to the callback function
 * @return @c the job handler on success,
 *         otherwise NULL.
 * @remarks This is for App Framework internally.
 */
service_app_job_h service_app_add_job_handler(const char *job_id,
		service_app_job_cb callback, void *user_data);

/**
 * @brief Removes the registered job handler.
 * @since_tizen 4.0
 * @param[in]    handle         The job handle
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @remarks This is for App Framework internally.
 */
int service_app_remove_job_handler(service_app_job_h handle);

/**
 * @brief Raises the job.
 * @since_tizen 4.0
 * @param[in]   job_status      The status of the job
 * @param[in]   job_id          The ID of the job
 * @param[in]   job_data        The data of the job
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @remarks This is for App Framework internally.
 */
int service_app_job_raise(int job_status, const char *job_id, bundle *job_data);

/**
 * @brief Notifies that the job is finished.
 * @since_tizen 4.0
 * @param[in]    job_id         The ID of the job
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @remarks This is for App Framework internally.
 */
int service_app_job_finished(const char *job_id);

#ifdef __cplusplus
}
#endif

