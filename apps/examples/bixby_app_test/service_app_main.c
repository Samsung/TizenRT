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
#include <tinyara/config.h>
#include <mqueue.h>
#include <fcntl.h>
#include <unistd.h>
#include <task_manager/task_manager.h>
#include "dlog.h"
#include "app_common.h"
#include "app_control.h"
#include "service_app_main.h"

static const char* service_app_error_to_string(app_error_e error)
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

int service_app_error(app_error_e error, const char* function, const char *description)
{
	if (description) {
		LOGE("[%s] %s(0x%08x) : %s", function, service_app_error_to_string(error), error, description);
	} else {
		LOGE("[%s] %s(0x%08x)", function, service_app_error_to_string(error), error);
	}

	return error;
}

int service_app_main(int argc, char **argv, service_app_lifecycle_callback_s *callback, void *user_data)
{
	int nbytes;
	struct app_control_s app_control;
	mqd_t ac_recv_mqfd;
	tm_appinfo_t *info;
	char *q_name;

	struct mq_attr attr;
	attr.mq_maxmsg = CONFIG_TASK_MANAGER_MAX_MSG;
	attr.mq_msgsize = sizeof(struct app_control_s);
	attr.mq_flags = 0;

	(*callback->create)(user_data);

	info = task_manager_getinfo_with_pid(getpid(), TM_RESPONSE_WAIT_INF);
	if (info == NULL) {
		return service_app_error(APP_ERROR_INVALID_CONTEXT, __FUNCTION__, NULL);
	}
	asprintf(&q_name, "%s%d", "appcontrol_", info->handle);

	ac_recv_mqfd = mq_open(q_name, O_RDONLY | O_CREAT, 0666, 100);
	if (ac_recv_mqfd < 0) {
		free(q_name);
		task_manager_clean_info(&info);
		return service_app_error(APP_ERROR_NOT_SUPPORTED, __FUNCTION__, NULL);
	}

	nbytes = mq_receive(ac_recv_mqfd, (char *)&app_control, sizeof(struct app_control_s), NULL);
	if (nbytes <= 0) {
		free(q_name);
		task_manager_clean_info(&info);
		return service_app_error(APP_ERROR_NOT_SUPPORTED, __FUNCTION__, NULL);
	}
	(*callback->app_control)(&app_control, user_data);

	task_manager_clean_info(&info);
	mq_close(ac_recv_mqfd);
	mq_unlink(q_name);
	free(q_name);

	//wait event
	/* TIZENRT - FIX ME */
	while (1) {
		sleep(1);
	}

	(*callback->terminate)(user_data);
	return APP_ERROR_NONE;
}

int service_app_add_event_handler(app_event_handler_h *handler, app_event_type_e event_type, app_event_cb callback, void *user_data)
{
	return APP_ERROR_NONE;
}

void service_app_exit(void)
{
	exit(1);
}
