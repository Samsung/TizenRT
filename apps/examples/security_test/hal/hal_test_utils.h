/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#ifndef __HAL_TEST_UTILS_H__
#define __HAL_TEST_UTILS_H__

#include <tinyara/config.h>
#include <tinyara/security_hal.h>

void hal_test_free_buffer(hal_data *data);
int hal_test_malloc_buffer(hal_data *data, int buf_len);
void hal_test_init_buffer(hal_data *data);

#endif // __HAL_TEST_UTILS_H__
