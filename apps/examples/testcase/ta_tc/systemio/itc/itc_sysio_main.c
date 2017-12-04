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

extern sem_t tc_sem;

int itc_sysio_launcher(int argc, FAR char *argv[])
{
	total_pass = 0;
	total_fail = 0;

	sem_wait(&tc_sem);

	printf("\n########## SystemIO ITC Start ##########\n");

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

	printf("\n########## SystemIO ITC End [PASS : %d, FAIL : %d] ##########\n", total_pass, total_fail);

	sem_post(&tc_sem);

	return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int itc_sysio_main(int argc, char *argv[])
#endif
{
#ifdef CONFIG_TASH
	tash_cmd_install("sysio_itc", itc_sysio_launcher, 0);
#else
	itc_sysio_launcher(argc, argv);
#endif
	return 0;
}
