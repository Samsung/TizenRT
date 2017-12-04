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
#include "utc_internal.h"
#include <apps/shell/tash.h>

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
extern int working_tc;

int sysio_utc(int argc, FAR char *argv[])
{
	sem_wait(&tc_sem);
	working_tc++;

	printf("\n########## SystemIO UTC Start ##########\n");

	total_pass = 0;
	total_fail = 0;

#ifdef CONFIG_SYSIO_UTC_GPIO
	utc_gpio_main();
#endif

#ifdef CONFIG_SYSIO_UTC_PWM
	utc_pwm_main();
#endif

#ifdef CONFIG_SYSIO_UTC_I2C
	utc_i2c_main();
#endif

#ifdef CONFIG_SYSIO_UTC_SPI
	utc_spi_main();
#endif

#ifdef CONFIG_SYSIO_UTC_UART
	utc_uart_main();
#endif

	printf("\n########## SystemIO UTC End [PASS : %d, FAIL : %d] ##########\n", total_pass, total_fail);

	working_tc--;
	sem_post(&tc_sem);

	return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int utc_sysio_main(int argc, char *argv[])
#endif
{

#ifdef CONFIG_TASH
	tash_cmd_install("sysio_utc", sysio_utc, TASH_EXECMD_SYNC);
#else
	sysio_utc(argc, argv);
#endif
	return 0;
}
