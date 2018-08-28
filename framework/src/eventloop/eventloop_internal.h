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

#ifndef __EVENTLOOP_INTERNAL_H__
#define __EVENTLOOP_INTERNAL_H__

#include <stdlib.h>

/* Wrapper of allocation APIs */
#define EL_ALLOC(a)  malloc(a)
#define EL_FREE(a)   free(a)

/* A value for the state of async loop */
#define ASYNCLOOP_RUNNING 0

#define LOOP_IS_STOPPED(loop) (loop->stop_flag == 1)

el_loop_t *get_app_loop(void);
int get_async_task(void);

#endif
