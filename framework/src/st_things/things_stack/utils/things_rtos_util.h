/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#ifndef THINGS_RTOS_UTIL_H_
#define THINGS_RTOS_UTIL_H_
#include "pthread.h"

int pthread_create_rtos(FAR pthread_t *thread, FAR const pthread_attr_t *attr, pthread_startroutine_t start_routine, pthread_addr_t arg, long stack_size, char *thread_name);

#endif							/* THINGS_RTOS_UTIL_H_ */
