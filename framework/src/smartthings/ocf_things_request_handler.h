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

#ifndef _OCF_THINGS_REQUEST_HANDLER_H_
#define _OCF_THINGS_REQUEST_HANDLER_H_

#include "ocf_things.h"
#include "oicresource.h"

int handle_get_request_cb(struct OicResource* resource);

int handle_set_request_cb(struct OicResource* resource);

int register_request_handler_cb(ocf_things_get_request_cb get_cb,
                                ocf_things_set_request_cb set_cb);

ocf_things_representation_s* create_representation_inst(void);

void destroy_representation_inst(ocf_things_representation_s* rep);

#endif // _OCF_THINGS_REQUEST_HANDLER_H_
