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
 /****************************************************************************
 * Included Files
 ****************************************************************************/
#include <task_manager/task_manager.h>

/**
 * @ingroup Task_Manager
 * @brief User specific broadcast messages are declared at this file.
 * @{
 */

/**
 * @file task_manager/task_manager_broadcast_list.h
 */
#ifndef __TASK_MANAGER_BROADCAST_LIST_H__
#define __TASK_MANAGER_BROADCAST_LIST_H__

/**
 * @brief User specific broadcast message list
 * @details These values can be used for broadcast messages.\n
 * If user wants to add some other types of broadcast message,\n
 * user can add their own broadcast messages at here.\n
 * \n
 * User must avoid setting an arbitrary value to their broadcast message.\n
 * First user specific broadcast message would be set as (TM_BROADCAST_USER_SPECIFIC_MAX + 1).\n
 * Other user specific broadcast messages are automatically set as ascending order.\n
 * An example of a user specific broadcast message is shown below\n
 * \n
 * enum tm_user_specific_broadcast_msg {\n
 *		TM_BROADCAST_USER_SPECIFIC_MIN = TM_BROADCAST_SYSTEM_MSG_MAX,\n
 *		TM_USER_SPECIFIC_BROADCAST_MSG_1,\n
 *		TM_USER_SPECIFIC_BROADCAST_MSG_2,\n
 *		TM_BROADCAST_MSG_MAX,\n
 * };
 */

enum tm_user_specific_broadcast_msg {
	TM_BROADCAST_USER_SPECIFIC_MIN = TM_BROADCAST_SYSTEM_MSG_MAX,

/* Please Add User Specific Broadcast Message at HERE */

	TM_BROADCAST_MSG_MAX,
};
#endif
/**
 * @}
 */
