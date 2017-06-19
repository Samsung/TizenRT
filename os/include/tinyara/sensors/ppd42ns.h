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

#ifndef __PPD42NS_H__
#define __PPD42NS_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/sensors/sensor.h>

#define SENSOR_NAME_PPD42NS				ppd42ns
SENSOR_EXTERNAL_FUNCTION_PROTOTYPE(SENSOR_NAME_PPD42NS);

/********************
   definitions
 ********************/
#define PPD42NS_DEBUG_ON			0

/********************
   custom ioctl id
 ********************/
#define PPD42NS_IOCTL_ID_GET_GPIO_DEVPATH			(SENSOR_IOCTL_ID_CUSTOM	+ 0)

#endif							/* __PPD42NS_H__ */
