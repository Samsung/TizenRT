/* ****************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#ifndef FMWUP_UTIL_INTERNAL_H_
#define FMWUP_UTIL_INTERNAL_H_

#include "fmwup_api.h"

void fmwup_internal_propagate_timed_wait();
void fmwup_internal_propagate_cond_signal();

void fmwup_internal_propagate_resource(fmwup_state_e state, fmwup_result_e result, bool wait_flag);
int fmwup_internal_update_command(int64_t update_type);

#endif							/* FMWUP_UTIL_INTERNAL_H_ */
