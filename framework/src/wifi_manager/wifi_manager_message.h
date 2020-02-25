/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
#ifndef _WIFI_MANAGER_MESSAGE_H__
#define _WIFI_MANAGER_MESSAGE_H__

typedef struct {
	int fd;
} handler_queue;

typedef struct {
	int fd;
	sem_t *signal;
	void *msg;
} handler_msg;

int wifimgr_message_in(handler_msg *msg, handler_queue *queue);
int wifimgr_messsage_out(handler_msg *msg, handler_queue *queue);

#endif //_WIFI_MANAGER_MESSAGE_H__



