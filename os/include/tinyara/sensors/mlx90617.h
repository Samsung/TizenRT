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

#ifndef __DRIVERS_SENSOR_MLX90617_H
#define __DRIVERS_SENSOR_MLX90617_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/i2c.h>
#include <tinyara/spi/spi.h>
#include <sys/types.h>
#include <stdint.h>

struct sensor_config {
        struct i2c_config_s i2c_config;
	struct spi_io_config spi_config;
};

#endif  /* __DRIVERS_SENSOR_MLX90617_H */

