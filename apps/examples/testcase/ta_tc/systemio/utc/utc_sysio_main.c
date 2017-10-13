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
int total_pass;
int total_fail;

extern sem_t tc_sem;
extern int working_tc;

char *Expect_Value(int expect, int value)
{
	if (expect == value) {
		total_pass++;
		return "PASS";
	}

	total_fail++;
	return "FAIL";
}

static void show_usage(FAR const char *progname)
{
	SYSIO_UTC_PRINT("\nUsage: %s <command> \n", progname);
	SYSIO_UTC_PRINT("\nWhere:\n");
	SYSIO_UTC_PRINT("	 <command>	  command string (gpio | pwm | i2c | spi | uart) \n");
	SYSIO_UTC_PRINT("				   - gpio  : tc gpio api \n");
	SYSIO_UTC_PRINT("				   - pwm   : tc pwm api \n");
	SYSIO_UTC_PRINT("				   - i2c	: tc i2c api \n");
	SYSIO_UTC_PRINT("				   - spi	: tc spi api \n");
	SYSIO_UTC_PRINT("				   - uart  : tc uart api \n");
	SYSIO_UTC_PRINT("\n");
}

int sysio_utc(int argc, FAR char *argv[])
{
	int cmdtype = 0;

	sem_wait(&tc_sem);
	working_tc++;

	SYSIO_UTC_PRINT("=== TINYARA SYSIO UTC START! ===\n");

#if 0

	switch (argc) {
	case 2:
		if (strcmp(argv[1], "gpio") == 0) {
			cmdtype = 1;
		} else if (strcmp(argv[1], "pwm") == 0) {
			cmdtype = 2;
		} else if (strcmp(argv[1], "i2c") == 0) {
			cmdtype = 3;
		} else if (strcmp(argv[1], "spi") == 0) {
			cmdtype = 4;
		} else if (strcmp(argv[1], "uart") == 0) {
			cmdtype = 5;
		} else if (strcmp(argv[1], "i2c_test") == 0) {
			cmdtype = 6;
		} else if (strcmp(argv[1], "i2c_acc") == 0) {
			cmdtype = 7;
		} else if (strcmp(argv[1], "i2c_light") == 0) {
			cmdtype = 8;
		} else {
			show_usage(argv[0]);
		}
		break;

	default:
		show_usage(argv[0]);
		break;
	}
#endif

	int i;
	for (i = 1; i <= 5; i++) {
		total_pass = 0;
		total_fail = 0;
		cmdtype = i;

		switch (cmdtype) {
		case 1:
#ifdef CONFIG_SYSIO_UTC_GPIO
			SYSIO_UTC_PRINT("################################################################\n");
			SYSIO_UTC_PRINT("######################## G P I O ###############################\n");
			SYSIO_UTC_PRINT("################################################################\n");
			utc_gpio_main();
#else
			SYSIO_UTC_PRINT("plz set CONFIG_UTC_SYSIO_GPIO");
			continue;
#endif
			break;
		case 2:
#ifdef CONFIG_SYSIO_UTC_PWM
			SYSIO_UTC_PRINT("################################################################\n");
			SYSIO_UTC_PRINT("######################## P  W	M ###############################\n");
			SYSIO_UTC_PRINT("################################################################\n");
			utc_pwm_main();
#else
			SYSIO_UTC_PRINT("plz set CONFIG_UTC_SYSIO_PWM");
			continue;

#endif
			break;
		case 3:
#ifdef CONFIG_SYSIO_UTC_I2C
			SYSIO_UTC_PRINT("################################################################\n");
			SYSIO_UTC_PRINT("######################## I  2	C ###############################\n");
			SYSIO_UTC_PRINT("################################################################\n");
			utc_i2c_main();
#else
			SYSIO_UTC_PRINT("plz set CONFIG_UTC_SYSIO_I2C");
			continue;
#endif
			break;
		case 4:
#ifdef CONFIG_SYSIO_UTC_SPI
			SYSIO_UTC_PRINT("################################################################\n");
			SYSIO_UTC_PRINT("######################## S  P	I ###############################\n");
			SYSIO_UTC_PRINT("################################################################\n");
			utc_spi_main();
#else
			SYSIO_UTC_PRINT("plz set CONFIG_UTC_SYSIO_SPI");
			continue;

#endif
			break;
		case 5:
#ifdef CONFIG_SYSIO_UTC_UART
			SYSIO_UTC_PRINT("################################################################\n");
			SYSIO_UTC_PRINT("######################## U A R T ###############################\n");
			SYSIO_UTC_PRINT("################################################################\n");
			utc_uart_main();
#else
			SYSIO_UTC_PRINT("plz set CONFIG_UTC_SYSIO_UART");
			continue;

#endif
			break;
		}

		if (cmdtype != 0) {
			SYSIO_UTC_PRINT("\n=== TINYARA SYSIO TC COMPLETE ===\n");
			SYSIO_UTC_PRINT("\t\tTotal pass : %d\n\t\tTotal fail : %d\n", total_pass, total_fail);
		}

		sleep(1);
	}

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
