/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef _ST_THINGS_REQUEST_HANDLER_H_
#define _ST_THINGS_REQUEST_HANDLER_H_

#include <st_things/st_things.h>
#include "things_resource.h"

int handle_get_request_cb(struct things_resource_s *resource);

int handle_set_request_cb(struct things_resource_s *resource);

int register_request_handler_cb(st_things_get_request_cb get_cb, st_things_set_request_cb set_cb);

#endif							// _ST_THINGS_REQUEST_HANDLER_H_
