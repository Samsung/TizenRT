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
#ifndef __SERVICE_APP_MAIN_H__
#define __SERVICE_APP_MAIN_H__

#include "app_control.h"
#include "app_common.h"
#include <stdbool.h>

typedef bool (*service_app_create_cb)(void *user_data);
typedef void (*service_app_terminate_cb)(void *user_data);
typedef void (*service_app_control_cb)(app_control_h app_control, void *user_data);

typedef struct {
	service_app_create_cb create; /**< This callback function is called at the start of the application. */
	service_app_terminate_cb terminate; /**< This callback function is called once after the main loop of the application exits. */
	service_app_control_cb app_control; /**< This callback function is called when another application sends the launch request to the application. */
} service_app_lifecycle_callback_s;

int service_app_main(int argc, char **argv, service_app_lifecycle_callback_s *callback, void *user_data);
int service_app_add_event_handler(app_event_handler_h *handler, app_event_type_e event_type, app_event_cb callback, void *user_data);
void service_app_exit(void);

#endif /* __SERVICE_APP_MAIN_H__ */
