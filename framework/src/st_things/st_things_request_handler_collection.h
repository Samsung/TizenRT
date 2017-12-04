/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#ifndef __ST_THINGS_REQUEST_HANDLER_COLLECTION_H__
#define __ST_THINGS_REQUEST_HANDLER_COLLECTION_H__

#include "things_resource.h"
#include "octypes.h"

bool form_collection_links(things_resource_s *collection_rsrc, OCRepPayload ***links, size_t *count);

int handle_get_req_on_collection_rsrc(things_resource_s *collection_rsrc);

int handle_post_req_on_collection_rsrc(things_resource_s *collection_rsrc);

#endif							// __ST_THINGS_REQUEST_HANDLER_COLLECTION_H__
