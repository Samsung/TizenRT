/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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

#ifndef __DRIVERS_SENSOR_AIS25BA_H
#define __DRIVERS_SENSOR_AIS25BA_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/i2c.h>
#include <sys/types.h>
#include <stdint.h>

struct ais25ba_dev_s
{
        /* I2C bus and address for device. */
	
        struct i2c_dev_s *i2c;
        struct i2c_config_s i2c_config;
	struct i2s_dev_s *i2s;
        /* Configuration for device. */
	struct sensor_upperhalf_s *upper;
	void *priv;
};

#endif	/* __DRIVERS_SENSOR_AIS25BA_H */ 
