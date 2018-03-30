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

#include <pthread.h>

#include "things_api.h"
#include "things_def.h"
#include "logging/things_logger.h"
#include "framework/things_req_handler.h"
#include "octypes.h"
#include "ocpayload.h"
#include "utils/things_malloc.h"
#include "oic_string.h"
#include "framework/things_server_builder.h"

#ifdef __ST_THINGS_RTOS__

#define TAG "[thingsstacknew]"

extern things_request_handler_s *g_req_handler;
extern things_server_builder_s *g_server_builder;

#endif
