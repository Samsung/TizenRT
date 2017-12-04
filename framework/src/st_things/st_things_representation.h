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

#ifndef __ST_THINGS_REPRESENTATION_H__
#define __ST_THINGS_REPRESENTATION_H__

#ifdef __ST_THINGS_RTOS__
#include <st_things/st_things_types.h>
#else
#include "st_things_types.h"
#endif							// __ST_THINGS_RTOS__
#include "octypes.h"

st_things_representation_s *create_representation_inst(void);

st_things_representation_s *create_representation_inst_internal(OCRepPayload *payload);

void destroy_representation_inst(st_things_representation_s *rep);

void destroy_representation_inst_internal(st_things_representation_s *rep, bool destroy_payload);

#endif							// __ST_THINGS_REPRESENTATION_H__
