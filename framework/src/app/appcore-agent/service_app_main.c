/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
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

#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <eventloop/eventloop.h>
#include <app/bundle.h>
#include <aul.h>
#include <dlog.h>
// #include <vconf-internal-keys.h>
#include <app/app_common.h>
// #include <Ecore.h>
#include <appcore_base.h>
#include <app/service_app.h>
#include <app/service_app_extension.h>
#include <aul_job_scheduler.h>
// #include <bundle_internal.h>
#include <glib.h>
#include "service_app_internal.h"


#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "CAPI_APPFW_APPLICATION"

extern int app_control_create_event(bundle *data, struct app_control_s **app_control);

typedef enum {
	APP_STATE_NOT_RUNNING,
	APP_STATE_CREATING,
	APP_STATE_RUNNING,
} app_state_e;

struct app_event_handler {
	app_event_type_e type;
	app_event_cb cb;
	void *data;
	void* raw;
};

struct app_event_info {
	app_event_type_e type;
	void *value;
};

struct service_app_context {
	service_app_lifecycle_callback_s callback;
	void *data;
	GList *running_jobs;
	GList *pending_jobs;
	GList *job_handlers;
};

struct service_app_job_s {
	char *job_id;
	service_app_job_cb callback;
	void *user_data;
};

struct job_s {
	int job_status;
	char *job_id;
	bundle *job_data;
	el_timer_t *timer;
	// guint idler; // TODO: is there idler in eventloop or libuv?
};

static struct service_app_context __context;
static app_state_e __app_state = APP_STATE_NOT_RUNNING;

static int __app_event_converter[APPCORE_BASE_EVENT_MAX] = {
	[APP_EVENT_LOW_MEMORY] = APPCORE_BASE_EVENT_LOW_MEMORY,
	[APP_EVENT_LOW_BATTERY] = APPCORE_BASE_EVENT_LOW_BATTERY,
	[APP_EVENT_LANGUAGE_CHANGED] = APPCORE_BASE_EVENT_LANG_CHANGE,
	[APP_EVENT_DEVICE_ORIENTATION_CHANGED] = APPCORE_BASE_EVENT_DEVICE_ORIENTATION_CHANGED,
	[APP_EVENT_REGION_FORMAT_CHANGED] = APPCORE_BASE_EVENT_REGION_CHANGE,
	[APP_EVENT_SUSPENDED_STATE_CHANGED] = APPCORE_BASE_EVENT_SUSPENDED_STATE_CHANGE,
};

static int __on_error(app_error_e error, const char *function, const char *description);
static void __destroy_job_handler(void *data);
static bool __exist_job_handler(const char *job_id);

static struct job_s *__create_job(int job_status, const char *job_id, bundle *job_data)
{
	struct job_s *job;

	job = calloc(1, sizeof(struct job_s));
	if (job == NULL) {
		/* LCOV_EXCL_START */
		LOGE("Out of memory");
		return NULL;
		/* LCOV_EXCL_STOP */
	}

	job->job_id = strdup(job_id);
	if (job->job_id == NULL) {
		/* LCOV_EXCL_START */
		LOGE("Out of memory");
		free(job);
		return NULL;
		/* LCOV_EXCL_STOP */
	}

	job->job_data = bundle_dup(job_data);
	if (job->job_data == NULL) {
		/* LCOV_EXCL_START */
		LOGE("Out of memory");
		free(job->job_id);
		free(job);
		return NULL;
		/* LCOV_EXCL_STOP */
	}

	job->job_status = job_status;

	return job;
}

static void __destroy_job(void *data)
{
	struct job_s *job = (struct job_s *)data;

	if (job == NULL)
		return;

	// TODO:
	// if (job->idler)
	// 	g_source_remove(job->idler); /* LCOV_EXCL_LINE */
	if (job->timer)
		eventloop_delete_timer(job->timer); /* LCOV_EXCL_LINE */
	if (job->job_data)
		bundle_free(job->job_data);
	if (job->job_id)
		free(job->job_id);
	free(job);
}

/* LCOV_EXCL_START */
static bool __pending_job_timeout_handler(void *data)
{
	struct job_s *job = (struct job_s *)data;

	LOGE("[__TIMEOUT__] Job(%s) Status(%d)", job->job_id, job->job_status);
	__context.pending_jobs = g_list_remove(__context.pending_jobs, job);
	// eventloop_delete_timer(job->timer); // TODO: necessary?
	job->timer = NULL;
	__destroy_job(job);

	return EVENTLOOP_CALLBACK_STOP;//G_SOURCE_REMOVE;
}
/* LCOV_EXCL_STOP */

static void __job_finish(void)
{
	if (__context.running_jobs) {
		/* LCOV_EXCL_START */
		g_list_free_full(__context.running_jobs, __destroy_job);
		__context.running_jobs = NULL;
		/* LCOV_EXCL_STOP */
	}

	if (__context.pending_jobs) {
		/* LCOV_EXCL_START */
		g_list_free_full(__context.pending_jobs, __destroy_job);
		__context.pending_jobs = NULL;
		/* LCOV_EXCL_STOP */
	}

	if (__context.job_handlers) {
		g_list_free_full(__context.job_handlers, __destroy_job_handler);
		__context.job_handlers = NULL;
	}
}

static int __service_app_create(void *data)
{
	appcore_base_on_create();

	if (__context.callback.create == NULL ||
			__context.callback.create(__context.data) == false)
		return __on_error(APP_ERROR_INVALID_CONTEXT, __FUNCTION__, "service_app_create_cb() returns false");

	return APP_ERROR_NONE;
}

static int __service_app_terminate(void *data)
{
	if (__context.callback.terminate)
		__context.callback.terminate(__context.data);

	appcore_base_on_terminate();

	return APP_ERROR_NONE;
}

static int __service_app_control(bundle *b, void *data)
{
	app_control_h app_control = NULL;
	const char *job_id;

	LOGD("[SERVICE_APP] app_control callback");
	appcore_base_on_control(b);

	job_id = bundle_get_val(b, AUL_K_JOB_ID);
	if (job_id) {
		LOGD("[__JOB__] Job(%s)", job_id);
		return 0;
	}

	if (app_control_create_event(b, &app_control) != 0)
		return -1;

	if (__context.callback.app_control)
		__context.callback.app_control(app_control, __context.data);

	app_control_destroy(app_control);

	return 0;
}

static int __service_app_receive(aul_type type, bundle *b, void *data)
{
	appcore_base_on_receive(type, b);

	if (type == AUL_TERMINATE_BGAPP) {
		appcore_base_exit();
		return 0;
	}

	return 0;
}

static void __loop_init(int argc, char **argv, void *data)
{
	// event loop init
}

static void __loop_fini(void)
{
	// event loop shutdown
}

static void __loop_run(void *data)
{
	eventloop_loop_run();
}

static void __exit_main_loop(void *data)
{
	eventloop_loop_stop();
}

static void __loop_exit(void *data)
{
	eventloop_thread_safe_function_call(__exit_main_loop, data);
}

static const char *__error_to_string(app_error_e error)
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

static int __on_error(app_error_e error, const char *function, const char *description)
{
	if (description)
		LOGE("[%s] %s(0x%08x) : %s", function, __error_to_string(error), error, description);
	else
		LOGE("[%s] %s(0x%08x)", function, __error_to_string(error), error);

	return error;
}

EXPORT_API int service_app_main_ext(int argc, char **argv, service_app_lifecycle_callback_s *callback,
		service_app_loop_method_s *method, void *user_data)
{
	int ret;
	appcore_base_ops ops = appcore_base_get_default_ops();

	if (argc < 1 || argv == NULL || callback == NULL || method == NULL)
		return __on_error(APP_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);

	if (callback->create == NULL)
		return __on_error(APP_ERROR_INVALID_PARAMETER, __FUNCTION__, "service_app_create_cb() callback must be registered");

	if (__app_state != APP_STATE_NOT_RUNNING)
		return __on_error(APP_ERROR_ALREADY_RUNNING, __FUNCTION__, NULL); /* LCOV_EXCL_LINE */

	/* override methods */
	ops.create = __service_app_create;
	ops.terminate = __service_app_terminate;
	ops.control = __service_app_control;
	ops.receive = __service_app_receive;
	ops.run = method->run;
	ops.exit = method->exit;
	ops.init = method->init;
	ops.finish = method->fini;

	__context.callback = *callback;
	__context.data = user_data;

	__app_state = APP_STATE_CREATING;
	ret = appcore_base_init(ops, argc, argv, NULL);
	if (ret < 0) {
		/* LCOV_EXCL_START */
		__app_state = APP_STATE_NOT_RUNNING;
		return __on_error(APP_ERROR_INVALID_CONTEXT, __FUNCTION__, NULL);
		/* LCOV_EXCL_STOP */
	}

	appcore_base_fini();
	__job_finish();
	__app_state = APP_STATE_NOT_RUNNING;
	return APP_ERROR_NONE;
}

EXPORT_API int service_app_main(int argc, char **argv, service_app_lifecycle_callback_s *callback, void *user_data)
{
	service_app_loop_method_s method = {
		.init = __loop_init,
		.fini = __loop_fini,
		.run = __loop_run,
		.exit = __loop_exit,
	};

	return service_app_main_ext(argc, argv, callback, &method, user_data);
}

EXPORT_API void service_app_exit(void)
{
	appcore_base_exit();
}

static int __event_cb(void *event, void *data)
{
	app_event_handler_h handler = data;

	struct app_event_info app_event;

	app_event.type = handler->type;
	app_event.value = event;

	if (handler->cb)
		handler->cb(&app_event, handler->data);
	return 0;
}

EXPORT_API int service_app_add_event_handler(app_event_handler_h *event_handler, app_event_type_e event_type, app_event_cb callback, void *user_data)
{
	app_event_handler_h handler;

	if (event_handler == NULL || callback == NULL)
		return __on_error(APP_ERROR_INVALID_PARAMETER, __FUNCTION__, "null parameter");

	if (event_type < APP_EVENT_LOW_MEMORY || event_type > APP_EVENT_SUSPENDED_STATE_CHANGED ||
			event_type == APP_EVENT_DEVICE_ORIENTATION_CHANGED)
		return __on_error(APP_ERROR_INVALID_PARAMETER, __FUNCTION__, "invalid event type");

	handler = calloc(1, sizeof(struct app_event_handler));
	if (!handler)
		return __on_error(APP_ERROR_OUT_OF_MEMORY, __FUNCTION__, "insufficient memory"); /* LCOV_EXCL_LINE */

	handler->type = event_type;
	handler->cb = callback;
	handler->data = user_data;
	handler->raw = appcore_base_add_event(__app_event_converter[event_type], __event_cb, handler);

	*event_handler = handler;

	return APP_ERROR_NONE;
}

EXPORT_API int service_app_remove_event_handler(app_event_handler_h event_handler)
{
	int ret;
	app_event_type_e type;

	if (event_handler == NULL)
		return __on_error(APP_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL);

	type = event_handler->type;
	if (type < APP_EVENT_LOW_MEMORY ||
			type > APP_EVENT_SUSPENDED_STATE_CHANGED ||
			type == APP_EVENT_DEVICE_ORIENTATION_CHANGED)
		return __on_error(APP_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL); /* LCOV_EXCL_LINE */

	ret = appcore_base_remove_event(event_handler->raw);
	free(event_handler);

	if (ret < 0)
		return __on_error(APP_ERROR_INVALID_PARAMETER, __FUNCTION__, NULL); /* LCOV_EXCL_LINE */

	return APP_ERROR_NONE;
}

EXPORT_API void service_app_exit_without_restart(void)
{
	// aul_status_update(STATUS_NORESTART);
	appcore_base_exit();
}

static void __invoke_job_callback(int status, const char *job_id,
		bundle *job_data)
{
	struct service_app_job_s *handle;
	GList *iter;

	iter = __context.job_handlers;
	while (iter) {
		handle = (struct service_app_job_s *)iter->data;
		iter = iter->next;
		if (strcmp(handle->job_id, job_id) == 0) {
			handle->callback(status, job_id, job_data,
					handle->user_data);
		}
	}
}

static void __handle_job(void *data)
{
	struct job_s *job = (struct job_s *)data;

	__context.running_jobs = g_list_remove(__context.running_jobs, job);

	LOGD("[__JOB___] Job(%s) Status(%d) START", job->job_id, job->job_status);
	if (job->job_status == SERVICE_APP_JOB_STATUS_START) {
		// aul_job_scheduler_update_job_status(job->job_id, JOB_STATUS_START);
		__invoke_job_callback(job->job_status, job->job_id, job->job_data);
	} else {
		__invoke_job_callback(job->job_status, job->job_id, job->job_data);
		// aul_job_scheduler_update_job_status(job->job_id, JOB_STATUS_STOPPED);
	}
	LOGD("[__JOB__] Job(%s) Status(%d) END", job->job_id, job->job_status);

	// job->idler = 0;
	__destroy_job(job);

	// return EVENTLOOP_CALLBACK_STOP; // G_SOURCE_REMOVE;
}

static void __flush_pending_job(const char *job_id)
{
	struct job_s *job;
	GList *iter;

	iter = __context.pending_jobs;
	while (iter) {
		/* LCOV_EXCL_START */
		job = (struct job_s *)iter->data;
		iter = iter->next;
		if (strcmp(job->job_id, job_id) == 0) {
			__context.pending_jobs = g_list_remove(__context.pending_jobs, job);

			if (job->timer) {
				eventloop_delete_timer(job->timer);
				job->timer = NULL;
			}

			// job->idler = g_idle_add(__handle_job, job);
			eventloop_thread_safe_function_call(__handle_job, job); // TODO:
			__context.running_jobs = g_list_append(__context.running_jobs, job);
		}
		/* LCOV_EXCL_STOP */
	}
}

static bool __exist_job_handler(const char *job_id)
{
	struct service_app_job_s *handle;
	GList *iter;

	iter = __context.job_handlers;
	while (iter) {
		handle = (struct service_app_job_s *)iter->data;
		if (strcmp(handle->job_id, job_id) == 0)
			return true;

		iter = iter->next;
	}

	return false;
}

static struct service_app_job_s *__create_job_handler(const char *job_id,
		service_app_job_cb callback, void *user_data)
{
	struct service_app_job_s *handle;

	handle = calloc(1, sizeof(struct service_app_job_s));
	if (handle == NULL) {
		/* LCOV_EXCL_START */
		LOGE("Out of memory");
		return NULL;
		/* LCOV_EXCL_STOP */
	}

	handle->job_id = strdup(job_id);
	if (handle->job_id == NULL) {
		/* LCOV_EXCL_START */
		LOGE("Out of memory");
		free(handle);
		return NULL;
		/* LCOV_EXCL_STOP */
	}

	handle->callback = callback;
	handle->user_data = user_data;

	return handle;
}

static void __destroy_job_handler(void *data)
{
	struct service_app_job_s *handle = (struct service_app_job_s *)data;

	if (handle == NULL)
		return;

	if (handle->job_id)
		free(handle->job_id);
	free(handle);
}

EXPORT_API service_app_job_h service_app_add_job_handler(const char *job_id,
		service_app_job_cb callback, void *user_data)
{
	struct service_app_job_s *handle;

	if (job_id == NULL || callback == NULL) {
		/* LCOV_EXCL_START */
		LOGE("Invalid parameter");
		return NULL;
		/* LCOV_EXCL_STOP */
	}

	handle = __create_job_handler(job_id, callback, user_data);
	if (handle == NULL)
		return NULL;

	__context.job_handlers = g_list_append(__context.job_handlers, handle);

	__flush_pending_job(job_id);

	return handle;
}

EXPORT_API int service_app_remove_job_handler(service_app_job_h handle)
{
	if (handle == NULL ||
			g_list_index(__context.job_handlers, handle) < 0) {
		/* LCOV_EXCL_START */
		return __on_error(APP_ERROR_INVALID_PARAMETER, __FUNCTION__,
				NULL);
		/* LCOV_EXCL_STOP */
	}

	__context.job_handlers = g_list_remove(__context.job_handlers, handle);

	__destroy_job_handler(handle);

	return APP_ERROR_NONE;
}

EXPORT_API int service_app_job_raise(int job_status, const char *job_id,
		bundle *job_data)
{
	struct job_s *job;

	if (job_status < SERVICE_APP_JOB_STATUS_START ||
			job_status > SERVICE_APP_JOB_STATUS_STOP ||
			job_id == NULL || job_data == NULL) {
		/* LCOV_EXCL_START */
		return __on_error(APP_ERROR_INVALID_PARAMETER, __FUNCTION__,
				NULL);
		/* LCOV_EXCL_STOP */
	}

	job = __create_job(job_status, job_id, job_data);
	if (job == NULL)
		return __on_error(APP_ERROR_OUT_OF_MEMORY, __FUNCTION__, NULL); /* LCOV_EXCL_LINE */

	if (!__exist_job_handler(job_id)) {
		/* LCOV_EXCL_START */
		job->timer = eventloop_add_timer(5000, false, __pending_job_timeout_handler, job);
		__context.pending_jobs = g_list_append(__context.pending_jobs, job);
		/* LCOV_EXCL_STOP */
	} else {
		// job->idler = g_idle_add(__handle_job, job);
		eventloop_thread_safe_function_call(__handle_job, job); // TODO:
		__context.running_jobs = g_list_append(__context.running_jobs, job);
	}

	return APP_ERROR_NONE;
}

static void __remove_running_job(const char *job_id)
{
	struct job_s *job;
	GList *iter;

	iter = __context.running_jobs;
	while (iter) {
		/* LCOV_EXCL_START */
		job = (struct job_s *)iter->data;
		iter = iter->next;
		if (strcmp(job->job_id, job_id) == 0) {
			__context.running_jobs = g_list_remove(
					__context.running_jobs, job);
			__destroy_job(job);
		}
		/* LCOV_EXCL_STOP */
	}
}

EXPORT_API int service_app_job_finished(const char *job_id)
{
	int r;

	if (job_id == NULL) {
		/* LCOV_EXCL_START */
		return __on_error(APP_ERROR_INVALID_PARAMETER, __FUNCTION__,
				NULL);
		/* LCOV_EXCL_STOP */
	}

	__remove_running_job(job_id);

	// r = aul_job_scheduler_update_job_status(job_id, JOB_STATUS_FINISHED);
	// if (r != AUL_R_OK) {
	// 	/* LCOV_EXCL_START */
	// 	return __on_error(APP_ERROR_INVALID_CONTEXT, __FUNCTION__,
	// 			NULL);
	// 	/* LCOV_EXCL_STOP */
	// }

	return APP_ERROR_NONE;
}
