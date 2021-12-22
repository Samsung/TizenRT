/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License\n");
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

#define SET_START_TIME(operation) \
	do {\
		gettimeofday(&start, NULL);\
		CT_LOG(TAG, #operation ", start time: %ld sec %ld usec", start.tv_sec, start.tv_usec);\
	} while (0)

#define CALCULATE_ELAPSED_TIME(operation, total_time) \
	do {\
		long elapsed = 0;\
		gettimeofday(&end, NULL);\
		CT_LOG(TAG, #operation ", end time: %ld sec %ld usec", end.tv_sec, end.tv_usec);\
		elapsed = (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);\
		total_time += elapsed;\
	} while (0)