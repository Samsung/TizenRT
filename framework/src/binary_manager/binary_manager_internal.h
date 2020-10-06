/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#ifndef __BINARY_MANAGER_INTERNAL_H
#define __BINARY_MANAGER_INTERNAL_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/binary_manager.h>

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
binmgr_result_type_e binary_manager_set_request(binmgr_request_t *request_msg, int cmd, void *arg);
binmgr_result_type_e binary_manager_send_request(binmgr_request_t *request_msg);
binmgr_result_type_e binary_manager_receive_response(void *response_msg, int msg_size);

#endif							/* __BINARY_MANAGER_INTERNAL_H */
