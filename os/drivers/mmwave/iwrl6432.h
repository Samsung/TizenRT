/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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

#ifndef __DRIVERS_IWRL6432_SENSOR_H
#define __DRIVERS_IWRL6432_SENSOR_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/spi/spi.h>
#include <semaphore.h>
#include <tinyara/wdog.h>

struct iwrl6432_s {
	/* Publicly visible device structure */

	struct mmwave_dev_s *upper;
	struct mmwave_config_s *config;
	uint8_t **data_buffer;
	/* Private LCD-specific information follows */
	//WDOG_ID guard_wdog_id;
	FAR struct spi_dev_s *spi; /* SPI device */
};

#endif /* __DRIVERS_IWRL6432_SENSOR_H */
