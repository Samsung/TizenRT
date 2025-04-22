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