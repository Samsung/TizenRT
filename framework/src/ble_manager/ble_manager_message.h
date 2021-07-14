/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#pragma once

#include <sys/select.h>
#include <semaphore.h>

typedef struct {
	int fd;
	int nd;
	int max;
	fd_set rfds;
} ble_handler_queue;

typedef struct {
	sem_t *signal;
	void *msg;
} ble_handler_msg;

int blemgr_message_in(ble_handler_msg *msg, ble_handler_queue *queue);
int blemgr_message_out(ble_handler_msg *msg, ble_handler_queue *queue);
