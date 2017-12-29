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

#ifndef _THINGS_WAIT_HANDLER_H_
#define _THINGS_WAIT_HANDLER_H_

#include <octypes.h>

#define DEFAULT_TIMEOUT     60	// default 20 second wait.

typedef struct timeout_s {
	int cur_counter;			// current time-out counter
	int cur_num;				// current time-out number
	int ori_num;				// initial time-out number
} timeout_s;

typedef void *(*check_time_out_call_func)(timeout_s *param);

OCDoHandle get_request_handle(void);

int cas_request_handle(OCDoHandle EqualVal, OCDoHandle writeHandleVal);

bool is_empty_request_handle(void);

OCDoHandle add_request_handle(OCDoHandle HandleVal);

void del_all_request_handle(void);

long long next_time_out(const long long ori_num, const long long cur_num);	// PDF Gaussian Distribution Time-Out Calculator.

unsigned long int create_time_out_process(OCDoHandle hadler, check_time_out_call_func call_func, timeout_s *time_out);

#endif							/* _THINGS_WAIT_HANDLER_H_ */
