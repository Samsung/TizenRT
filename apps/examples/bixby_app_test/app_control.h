#ifndef __APP_CONTROL_H__
#define __APP_CONTROL_H__

#include "bundle.h"

#define BUNDLE_KEY_PREFIX_AUL "__AUL_"
#define BUNDLE_KEY_PREFIX_SERVICE "__APP_SVC_"

#define BUNDLE_KEY_OPERATION	"__APP_SVC_OP_TYPE__"
#define BUNDLE_KEY_URI		"__APP_SVC_URI__"
#define BUNDLE_KEY_MIME		"__APP_SVC_MIME_TYPE__"
#define BUNDLE_KEY_DATA		"__APP_SVC_DATA__"
#define BUNDLE_KEY_PACKAGE	"__APP_SVC_PKG_NAME__"
#define BUNDLE_KEY_WINDOW	"__APP_SVC_K_WIN_ID__"

/**
 * @brief Definition for the app_control operation: An explicit launch for an application that excludes the homescreen application.
 * @details Input: Nothing\n
 *          Output: Nothing\n
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define APP_CONTROL_OPERATION_DEFAULT "http://tizen.org/appcontrol/operation/default"
#define APP_CONTROL_OPERATION_LAUNCH_ON_EVENT "http://tizen.org/appcontrol/operation/launch_on_event"

typedef enum {
	APP_CONTROL_RESULT_APP_STARTED,
	APP_CONTROL_RESULT_SUCCEEDED,
	APP_CONTROL_RESULT_FAILED,
	APP_CONTROL_RESULT_CANCELED
} app_control_result_e;

typedef enum {
	APP_CONTROL_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	APP_CONTROL_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	APP_CONTROL_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	APP_CONTROL_ERROR_APP_NOT_FOUND = TIZEN_ERROR_APPLICATION | 0x21, /**< The application is not found */
	APP_CONTROL_ERROR_KEY_NOT_FOUND = TIZEN_ERROR_KEY_NOT_AVAILABLE, /**< Specified key is not found */
	APP_CONTROL_ERROR_KEY_REJECTED = TIZEN_ERROR_KEY_REJECTED, /**< Key is not available */
	APP_CONTROL_ERROR_INVALID_DATA_TYPE = TIZEN_ERROR_APPLICATION | 0x22, /**< Invalid data type */
	APP_CONTROL_ERROR_LAUNCH_REJECTED = TIZEN_ERROR_APPLICATION | 0x23, /**< The application cannot be launched now*/
	APP_CONTROL_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED,    /**< Permission denied */
	APP_CONTROL_ERROR_LAUNCH_FAILED = TIZEN_ERROR_APPLICATION | 0x24, /**< Internal launch error */
	APP_CONTROL_ERROR_TIMED_OUT = TIZEN_ERROR_TIMED_OUT, /**< Time out */
	APP_CONTROL_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR       /**< IO error */
} app_control_error_e;

typedef enum {
	APP_CONTROL_TYPE_REQUEST,
	APP_CONTROL_TYPE_EVENT,
	APP_CONTROL_TYPE_REPLY,
} app_control_type_e;

struct app_control_s {
	int id;
	app_control_type_e type;
	bundle *data;
	int launch_pid;
};
typedef struct app_control_s *app_control_h;

typedef void (*app_control_reply_cb)(app_control_h request, app_control_h reply, app_control_result_e result, void *user_data);

typedef struct app_control_request_context_s {
	app_control_h app_control;
	app_control_reply_cb reply_cb;
	void *user_data;
} *app_control_request_context_h;

int app_control_create(app_control_h *app_control);
int app_control_destroy(app_control_h app_control);
int app_control_set_app_id(app_control_h app_control, const char *app_id);
int app_control_add_extra_data(app_control_h app_control, const char *key, const char *value);
int app_control_send_launch_request(app_control_h app_control, app_control_reply_cb callback, void *user_data);
int app_control_clone(app_control_h *clone, app_control_h app_control);
int app_control_set_operation(app_control_h app_control, const char *operation);
int app_control_add_extra_data_array(app_control_h app_control, const char *key, const char* value[], int length);

#endif
