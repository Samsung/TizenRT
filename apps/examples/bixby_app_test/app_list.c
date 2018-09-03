/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#include <sys/types.h>
#include <bixby_apps/app_utils.h>


#if defined(CONFIG_DEVICE_TYPE1)
#include "device_type1_proto.h"
#elif defined(CONFIG_DEVICE_TYPE2)
#include "device_type2_proto.h"
#else //default
#include "bixby_apps_proto.h"
#endif

app_t bixby_app_list[] = {
#if defined(CONFIG_DEVICE_TYPE1)
#include "device_type1_list.h"
#elif defined(CONFIG_DEVICE_TYPE2)
#include "device_type2_list.h"
#else //default
#include "bixby_apps_list.h"
#endif
	{NULL, NULL, 0, 0, false, 0}
};
