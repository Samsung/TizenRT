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
/**
 * @defgroup Task_Manager Task_Manager
 * @ingroup Task_Manager
 * @brief Provides APIs for Task Manager
 * @{
 */

/**
 * @file task_manager/task_manager.h
 */
#ifndef __TASK_MANAGER_H__
#define __TASK_MANAGER_H__

#include <tinyara/config.h>
#include <signal.h>
#include <pthread.h>

#include <tinyara/task_manager_internal.h>

/**
 * @brief Task State which managed by Task Manager
 */
#define TM_APP_STATE_RUNNING      (1)
#define TM_APP_STATE_PAUSE        (2)
#define TM_APP_STATE_STOP         (3)
#define TM_APP_STATE_UNREGISTERED (4)

/**
 * @brief Task Permission
 * @details
 * TM_APP_PERMISSION_ALL : Any Task can request the control to Task Manager
 * TM_APP_PERMISSION_GROUP : Only same group task can request the control to Task Manager
 *                     -> group : Tasks set which registered from same parent task
 * TM_APP_PERMISSION_DEDICATE : Only a task which requested to register that task can request the control to Task Manager
 */
#define TM_APP_PERMISSION_ALL      (0)
#define TM_APP_PERMISSION_GROUP    (1)
#define TM_APP_PERMISSION_DEDICATE (2)

/**
 * @brief Returnable Flag from Task Manager
 * @details These defined can be used in timeout argument for most of the task manager APIs.\n
 * If timeout argument is greater than 0, its API will wait to receive during timeout milliseconds.
 */
#define TM_NO_RESPONSE       (0)
#define TM_RESPONSE_WAIT_INF (-1)

/**
 * @brief Error Type of Result Value returned from Task Manager
 * @details If operation is failed, these defined values will be returned.
 */
enum tm_result_error_e {
	TM_ALREADY_STARTED_APP = -1,
	TM_ALREADY_PAUSED_APP = -2,
	TM_ALREADY_STOPPED_APP = -3,
	TM_UNREGISTERED_APP = -4,
	TM_OPERATION_FAIL = -5,
	TM_COMMUCATION_FAIL = -6,
	TM_BUSY = -7,
	TM_INVALID_PARAM = -8,
	TM_INVALID_DRVFD = -9,
	TM_OUT_OF_MEMORY = -10,
	TM_NO_PERMISSION = -11,
	TM_NOT_SUPPORTED = -12,
	TM_UNREGISTERED_MSG = -13,
	TM_ALREADY_REGISTERED_CB = -14,
	TM_REPLY_TIMEOUT = -15,
};

/**
 * @brief Broadcast message list
 * @details These values can be used for broadcast messges.\n
 * If user wants to add some other types of broadcast message,\n
 * user can add their own broadcast messages at <task_manager/task_manager_broadcast_list.h>.
 */

enum tm_defined_broadcast_msg {
	TM_BROADCAST_WIFI_ON = 1,
	TM_BROADCAST_WIFI_OFF = 2,
	TM_BROADCAST_SYSTEM_MSG_MAX,
#ifndef CONFIG_TASK_MANAGER_USER_SPECIFIC_BROADCAST
	TM_BROADCAST_MSG_MAX = TM_BROADCAST_SYSTEM_MSG_MAX,
#endif
};
/**
 * @brief Application Info Structure
 */
struct tm_appinfo_s {
	char *name;
	int tm_gid;
	int handle;
	int status;
	int permission;
};
typedef struct tm_appinfo_s tm_appinfo_t;

struct tm_appinfo_list_s {
	tm_appinfo_t task;
	struct tm_appinfo_list_s *next;
};
typedef struct tm_appinfo_list_s tm_appinfo_list_t;

/**
 * @brief Unicast message Structure
 */
struct tm_msg_s {
	int msg_size;
	void *msg;
};
typedef struct tm_msg_s tm_msg_t;

/**
 * @brief Unicast callback function type
 */
typedef void (*_tm_unicast_t)(tm_msg_t *);

/**
 * @brief Broadcast callback function type
 */
typedef void (*_tm_broadcast_t)(void *, void *);

/**
 * @brief Termination callback function type
 */
typedef void (*_tm_termination_t)(void *);

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
/**
 * @brief Request to register a builtin task
 * @details @b #include <task_manager/task_manager.h>\n
 * This API can request to register a builtin task.\n
 * Find apps/builtin/README.md to know how to use builtin task.
 * @param[in] name the name of task to be registered
 * @param[in] permission the permission of task to be registered
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds
 * @return On success, handle id is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_register_builtin(char *name, int permission, int timeout);
/**
 * @brief Request to register a task which is not in builtin list
 * @details @b #include <task_manager/task_manager.h>\n
 * Find apps/builtin/README.md to know how to use builtin task.
 * @param[in] name the name of task to be registered
 * @param[in] priority the priority of task to be registered
 * @param[in] stack_size the stack_size of task to be registered
 * @param[in] entry the entry function pointer
 * @param[in] argv the argument to pass when task creation
 * @param[in] permission the permission of task to be registered
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds
 * @return On success, handle id is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_register_task(char *name, int priority, int stack_size, main_t entry, char * argv[], int permission, int timeout);
/**
 * @brief Request to register a pthread which is not in builtin list
 * @details @b #include <task_manager/task_manager.h>\n
 * @param[in] name the name of pthread to be registered
 * @param[in] attr the attribute of pthread to be registered
 * @param[in] start_routine the entry function pointer
 * @param[in] arg the argument to pass when task creation
 * @param[in] permission the permission of task to be registered
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds
 * @return On success, handle id is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_register_pthread(char *name, pthread_attr_t *attr, pthread_startroutine_t start_routine, pthread_addr_t arg, int permission, int timeout);
/**
 * @brief Request to unregister a task
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] handle the handle id of task to be unregistered
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds
 * @return On success, OK is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_unregister(int handle, int timeout);
/**
 * @brief Request to start the task
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] handle the handle id of task to be started
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds
 * @return On success, OK is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_start(int handle, int timeout);
/**
 * @brief Request to stop the task
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] handle the handle id of task to be stopped.
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds
 * @return On success, OK is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_stop(int handle, int timeout);
/**
 * @brief Request to pause the task
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] handle the handle id of task to be paused
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds
 * @return On success, OK is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_pause(int handle, int timeout);
/**
 * @brief Request to resume the task
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] handle the handle id of task to be resumed
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds
 * @return On success, OK is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_resume(int handle, int timeout);
/**
 * @brief Request to restart the task
 * @details @b #include <task_manager/task_manager.h>
 * It cannot guarantee the resource deallocation.
 * @param[in] handle the handle id of task to be restarted
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds
 * @return On success, OK is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_restart(int handle, int timeout);
/**
 * @brief Request to send messages to the task
 * @details @b #include <task_manager/task_manager.h>\n
 * task_manager_unicast can be used with SYNC and ASYNC mode.\n
 * With SYNC mode, reply msg can be sent through task_manager_reply_unicast API.
 * @param[in] handle the handle id of task to be sent
 * @param[in] send_msg message structure to be unicasted
 * @param[out] reply_msg the reply msg structure for sync. If this is NULL, task_manager_unicast works asynchronously.
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds\n
 *            If reply is not NULL(=SYNC), timeout should not be set to TM_NO_RESPONSE.\n
 *            When you enable CONFIG_TASK_MANAGER_UNICAST_REPLY_TIMEOUT with non-zero value, task manager waits the reply during\n
 *            CONFIG_TASK_MANAGER_UNICAST_REPLY_TIMEOUT even if user calls this API with TM_RESPONSE_WAIT_INF.\n
 *            When set to zero, task manager waits the reply forever, it can cause task manager hang.
 * @return On success, OK is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_unicast(int handle, tm_msg_t *send_msg, tm_msg_t *reply_msg, int timeout);
/**
 * @brief Request to send messages to the tasks
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] msg message structure to be unicasted
 * @param[in] data data and its size to be broadcasted
 * @return On success, OK is returned. On failure, defined negative value is returned.\n
 *         (This return value only checks whether a broadcast message has been requested\n
 *          to the task manager to broadcast.)
 * @since TizenRT v2.0 PRE
 */
int task_manager_broadcast(int msg, tm_msg_t *data);
/**
 * @brief Set unicast callback function API
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] func the callback function which handle the msg\n
 * @return On success, OK is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_set_unicast_cb(void (*func)(tm_msg_t *data));
/**
 * @brief Register callback function which will be used for processing a certain received broadcast message
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] msg a certain broadcast message.\n
 *            When this message is received, the callback function func is called.\n
 *            Pre defined broadcast messages are presented at the top of <task_manager/task_manager.h>.\n
 *            User can add pre defined broadcast messages at the <task_manager/task_manager_broadcast_list.h>\n
 *            If this message is not pre defined at the <task_manager/task_manager.h> and <task_manager/task_manager_broadcast_list.h>,\n
 *            user should use task_manager_alloc_broadcast_msg() API to get a new broadacast message.
 * @param[in] func the callback function which will be called when a msg is received.
 * @param[in] cb_data a data pointer to pass to the callback function func.
 * @return On success, OK is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_set_broadcast_cb(int msg, void (*func)(void *user_data, void *data), void *cb_data);
/**
 * @brief Set callback function for resource deallocation API. If you set the callback, it will works when task terminates.
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] func the callback function that is called when the task or thread terminates normally.
 * @param[in] cb_data a data pointer to pass to the callback function func.
 * @return On success, OK is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_set_exit_cb(void (*func)(void *data), void *cb_data);
/**
 * @brief Set callback function for resource deallocation API. If you set the callback, it will works when task is cancelled.
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] func the callback function that is called when the task or thread is stopped by task manager.
 * @param[in] cb_data a data pointer to pass to the callback function func.
 * @return On success, OK is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_set_stop_cb(void (*func)(void *data), void *cb_data);
/**
 * @brief Get task information list through task name
 * @details @b #include <task_manager/task_manager.h>
 * After using the list of task information, use task_manager_clean_infolist() API to free the allocated memory.
 * @param[in] name the task name to get information
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds
 * @return On success, the list of task information is returned(at the end of the list, NULL will be returned). On failure, NULL is returned.
 * @since TizenRT v2.0 PRE
 */
tm_appinfo_list_t *task_manager_getinfo_with_name(char *name, int timeout);
/**
 * @brief Get task information through handle
 * @details @b #include <task_manager/task_manager.h>
 * After using the task information, use task_manager_clean_info() API to free the allocated memory.
 * @param[in] handle the handle id to get information
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds
 * @return On success, the task information is returned. On failure, NULL is returned.
 * @since TizenRT v2.0 PRE
 */
tm_appinfo_t *task_manager_getinfo_with_handle(int handle, int timeout);
/**
 * @brief Get task information list through group
 * @details @b #include <task_manager/task_manager.h>
 * After using the list of task information, use task_manager_clean_infolist() API to free the allocated memory.
 * @param[in] group the group id to get information
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds
 * @return On success, the task information is returned. On failure, NULL is returned.
 * @since TizenRT v2.0 PRE
 */
tm_appinfo_list_t *task_manager_getinfo_with_group(int group, int timeout);
/**
 * @brief Get the handle through pid
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] pid the pid which to get its task information\n
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds\n
 *            For this API, timeout cannot be set to TM_NO_RESPONSE.
 * @return On success, the task information is returned. On failure, NULL is returned.
 * @since TizenRT v2.0 PRE
 */
tm_appinfo_t *task_manager_getinfo_with_pid(int pid, int timeout);
/**
 * @brief Clean task information
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] info the task information
 * @return none
 * @since TizenRT v2.0 PRE
 */
void task_manager_clean_info(tm_appinfo_t **info);
/**
 * @brief Clean task information list
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] info_list the task information list
 * @return none
 * @since TizenRT v2.0 PRE
 */
void task_manager_clean_infolist(tm_appinfo_list_t **info_list);
/**
 * @brief Send unicast reply message
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] reply_msg the pointer of msg for reply
 * @return On success, OK is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_reply_unicast(tm_msg_t *reply_msg);
/**
 * @brief Allocate a new broadcast message which is not defined in the <task_manager/task_manager.h>\n
 *        and <task_manager/task_manager_broadcast_list.h>
 * @details @b #include <task_manager/task_manager.h>
 * @return On success, a newly allocated broadcast message value is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_alloc_broadcast_msg(void);
/**
 * @brief Unregister callback function which was used for a certain broadcast message.
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] msg the broadcast message corresponding to the callback function to be unregistered\n
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds
 * @return On success, OK is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_unset_broadcast_cb(int msg, int timeout);
/**
 * @brief Remove the broadcast message which was allocated by using task_manager_alloc_broadcast_msg() API.
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] msg the message which to be removed.\n
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds
 * @return On success, OK is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_dealloc_broadcast_msg(int msg, int timeout);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/**
 * @}
 */
