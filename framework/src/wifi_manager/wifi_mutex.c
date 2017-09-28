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

#include "wifi_mutex.h"
#include "wifi_semaphore.h"

wifi_utils_result_e wifi_mutex_create(wifi_mutex *mutex)
{
	return wifi_semaphore_create((wifi_semaphore *)mutex, 1);
}

wifi_utils_result_e wifi_mutex_acquire(wifi_mutex *mutex, int time_out)
{
	return wifi_semaphore_acquire((wifi_semaphore *)mutex, time_out);
}

wifi_utils_result_e wifi_mutex_release(wifi_mutex *mutex)
{
	return wifi_semaphore_release((wifi_semaphore *)mutex);
}

wifi_utils_result_e wifi_mutex_destroy(wifi_mutex *mutex)
{
	return wifi_semaphore_destroy((wifi_semaphore *)mutex);
}
