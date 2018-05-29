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

#include <signal.h>

/**
 * @brief Task State which managed by Task Manager
 */
#define TM_TASK_STATE_RUNNING      (1)
#define TM_TASK_STATE_PAUSE        (2)
#define TM_TASK_STATE_STOP         (3)
#define TM_TASK_STATE_UNREGISTERED (4)

/**
 * @brief Task Permission
 * @details
 * TM_TASK_PERMISSION_ALL : Any Task can request the control to Task Manager
 * TM_TASK_PERMISSION_GROUP : Only same group task can request the control to Task Manager
 *                     -> group : Tasks set which registered from same parent task
 * TM_TASK_PERMISSION_DEDICATE : Only a task which requested to register that task can request the control to Task Manager
 */
#define TM_TASK_PERMISSION_ALL      (0)
#define TM_TASK_PERMISSION_GROUP    (1)
#define TM_TASK_PERMISSION_DEDICATE (2)

/**
 * @brief Returnable Flag from Task Manager
 * @details These defined can be used in timeout argument for most of the task manager APIs.\n
 * If timeout argument is greater than 0, its API will wait to receive during timeout milliseconds.
 */
#define TM_NO_RESPONSE       (0)
#define TM_RESPONSE_WAIT_INF (-1)

/**
 * @brief Error Type of Request
 * @details When fail to request to task manager, these defined values will be returned.
 */
#define TM_FAIL_ALREADY_STARTED_TASK  (-1)    // same as negative RUNNING state
#define TM_FAIL_ALREADY_PAUSED_TASK   (-2)    // same as negative PAUSE state
#define TM_FAIL_ALREADY_STOPPED_TASK  (-3)    // same as negative STOP state
#define TM_FAIL_UNREGISTERED_TASK     (-4)    // same as negative UNREGISTERED state
#define TM_FAIL_NOT_PERMITTED         (-5)
#define TM_FAIL_NOT_REGISTERED        (-6)
#define TM_FAIL_START_NOT_CREATED     (-7)
#define TM_FAIL_SEND_MSG              (-8)
#define TM_FAIL_SET_HANDLER           (-9)
#define TM_FAIL_NO_HANDLER            (-10)
#define TM_FAIL_PAUSE                 (-11)
#define TM_FAIL_RESUME                (-12)
#define TM_FAIL_REQ_TO_MGR            (-13)
#define TM_FAIL_RESPONSE              (-14)
#define TM_FAIL_UNREGISTER            (-15)
#define TM_FAIL_SET_TERMINATION_CB    (-16)
#define TM_INVALID_PARAM              (-17)
#define TM_OUT_OF_MEMORY              (-18)

/**
 * @brief Broadcast message list
 * @details These values can be used for broadcast messges
 * If user wants to add some other types of broadcast message,
 * user can add their own broadcast messages at here.
 */
#define TM_BROADCAST_WIFI_ON       (1 << 0)
#define TM_BROADCAST_WIFI_OFF      (1 << 1)

/**
 * @brief Task Info Structure
 */
struct task_info_s {
	char *name;
	int gid;
	int handle;
	int status;
	int permission;
};
typedef struct task_info_s task_info_t;

struct task_info_list_s {
	task_info_t task;
	struct task_info_list_s *next;
};
typedef struct task_info_list_s task_info_list_t;

struct tm_response_s {
	int status;
	task_info_list_t *data;
};
typedef struct tm_response_s tm_response_t;

typedef siginfo_t tm_msg_t;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
/**
 * @brief Request to register a task
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] name the name of task which request to register
 * @param[in] permission the permission of task which request to register\n
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds
 * @return On success, handle id is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_register(char *name, int permission, int timeout);
/**
 * @brief Request to unregister a task
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] handle the handle id of task which request to unregister
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
 * @param[in] handle the handle id of task which request to start
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds 
 * @return On success, OK is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_start(int handle, int timeout);
/**
 * @brief Request to terminate the task
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] handle the handle id of task which request to terminate
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds 
 * @return On success, OK is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_terminate(int handle, int timeout);
/**
 * @brief Request to pause the task
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] handle the handle id of task which request to pause
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
 * @param[in] handle the handle id of task which request to resume
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds
 * @return On success, OK is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_resume(int handle, int timeout);
/**
 * @brief Request to send messages to the task
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] handle the handle id of task which request to send
 * @param[in] msg message which request to send
 * @param[in] msg_size the size of message
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds 
 * @return On success, OK is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_unicast(int handle, void *msg, int msg_size, int timeout);

/**
 * @brief Request to send messages to the tasks
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] msg message which request to broadcast.\n
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds
 * @return On success, OK is returned. On failure, defined negative value is returned.
 *         (This return value only checks whether a broadcast message has been requested 
*           to the task manager to broadcast.)
 * @since TizenRT v2.0 PRE
 */
int task_manager_broadcast(int msg, int timeout);

/**
 * @brief Set handler function API
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] func the handler function which handle the msg\n
 * In handler function, tm_msg_t is for containing data. It is originally siginfo_t type.\n
 * @return On success, OK is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_set_handler(void (*func)(int signo, tm_msg_t *data));

/**
 * @brief Set handler function API which will be used for receiving broadcast messages
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] msg_mask the message mask for selecting specific broadcast msgs\n
 *            User can change the msg_mask by using task_manager_set_broadcast_handler
 *            with the changed msg_mask value
 * @param[in] func the handler function which handle broadcast msgs\n
 * In handler function, tm_msg_t is for containing data. It is originally siginfo_t type.\n
 * @return On success, OK is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_set_broadcast_handler(int msg_mask, void (*func)(int signo, tm_msg_t *data));

/**
 * @brief Set callback function for resource deallocation API. If you set the callback, it will works when task is terminated.
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] func the callback function which deallocate resources\n
 * @return On success, OK is returned. On failure, defined negative value is returned.
 * @since TizenRT v2.0 PRE
 */
int task_manager_set_termination_cb(void (*func)(void));
/**
 * @brief Get task information list through task name
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] name the task name which want to get information
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds 
 * @return On success, the list of task information is returned(at the end of the list, NULL will be returned). On failure, NULL is returned.
 * @since TizenRT v2.0 PRE
 */
task_info_list_t *task_manager_getinfo_with_name(char *name, int timeout);
/**
 * @brief Get task information through handle
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] handle the handle id of task
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds 
 * @return On success, the task information is returned. On failure, NULL is returned.
 * @since TizenRT v2.0 PRE
 */
task_info_t *task_manager_getinfo_with_handle(int handle, int timeout);
/**
 * @brief Get task information list through group
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] group the group id which want to get information
 * @param[in] timeout returnable flag. It can be one of the below.\n
 *			TM_NO_RESPONSE : Ignore the response of request from task manager\n
 *			TM_RESPONSE_WAIT_INF : Blocked until get the response from task manager\n
 *			integer value : Specifies an upper limit on the time for which will block in milliseconds 
 * @return On success, the task information is returned. On failure, NULL is returned.
 * @since TizenRT v2.0 PRE
 */
task_info_list_t *task_manager_getinfo_with_group(int group, int timeout);
/**
 * @brief Clean task information
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] info the task information
 * @return none
 * @since TizenRT v2.0 PRE
 */
void task_manager_clean_info(task_info_t **info);
/**
 * @brief Clean task information list
 * @details @b #include <task_manager/task_manager.h>
 * @param[in] info_list the task information list
 * @return none
 * @since TizenRT v2.0 PRE
 */
void task_manager_clean_infolist(task_info_list_t **info_list);

#endif
/**
 * @}
 */
