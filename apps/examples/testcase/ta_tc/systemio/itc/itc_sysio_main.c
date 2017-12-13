/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

/// @file sysio_tc_main.c

/// @brief Main Function for Test Case Example
#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include "tc_common.h"
#include "itc_internal.h"

/***************************************************************************
 * Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sysio_tc_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int itc_sysio_main(int argc, char *argv[])
#endif
{
	if (tc_handler(TC_START, "SystemIO ITC") == ERROR) {
		return ERROR;
	}

#ifdef CONFIG_SYSIO_ITC_PWM
	itc_pwm_main();
#endif

#ifdef CONFIG_SYSIO_ITC_UART
	itc_uart_main();
#endif

#ifdef CONFIG_SYSIO_ITC_SPI
	itc_spi_main();
#endif

#ifdef CONFIG_SYSIO_ITC_I2C
	itc_i2c_main();
#endif

#ifdef CONFIG_SYSIO_ITC_GPIO
	itc_gpio_main();
#endif

	(void)tc_handler(TC_END, "SystemIO ITC");

	return 0;
}
