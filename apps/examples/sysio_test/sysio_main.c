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
/// @file sysio_main.c
/// @brief test code for System I/O (GPIO, PWM, I2C, SPI, UART)

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#ifdef CONFIG_API_DIR
#include <iotbus_error.h>
#include <iotbus_gpio.h>
#include <iotbus_pwm.h>
#include <iotbus_i2c.h>
#include <iotbus_spi.h>
#include <iotbus_uart.h>
#else
#include <iotbus/iotbus_error.h>
#include <iotbus/iotbus_gpio.h>
#include <iotbus/iotbus_pwm.h>
#include <iotbus/iotbus_i2c.h>
#include <iotbus/iotbus_spi.h>
#include <iotbus/iotbus_uart.h>
#endif

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define SYSIO_DEBUG             if (g_debug) printf
#define SYSIO_PRINT             printf

#define SYSIO_RESULT_SUCCESS    0
#define SYSIO_RESULT_FAIL       1

#define FAIL_REASON_STRING_SIZE 32
#define REGISTER_FAIL_REASON(reason_string, p_fail_flag) \
do { \
	if (*p_fail_flag == 0) { \
		snprintf(failstr, FAIL_REASON_STRING_SIZE, reason_string); \
		*p_fail_flag = 1; \
	} \
} while (0) \


/****************************************************************************
 * Enumeration
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/
static int g_debug = 1;

/****************************************************************************
 * function prototype
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * static function
 ****************************************************************************/
static void show_usage(FAR const char *progname)
{
	SYSIO_PRINT("\nUsage: %s <command> [debug]\n", progname);
	SYSIO_PRINT("\nWhere:\n");
	SYSIO_PRINT("    <command>    command string (gpio | pwm | i2c | spi | uart) \n");
	SYSIO_PRINT("                  - gpio  : test gpio api \n");
	SYSIO_PRINT("                  - pwm   : test pwm api \n");
	SYSIO_PRINT("                  - i2c    : test i2c api \n");
	SYSIO_PRINT("                  - spi    : test spi api \n");
	SYSIO_PRINT("                  - uart  : test uart api \n");
	SYSIO_PRINT("	[debug]            debug option (0 | 1) (default: 0) \n");
	SYSIO_PRINT("\n");
}

/****************************************************************************
 * public function
 ****************************************************************************/

/******************************
     GPIO TEST
 ******************************/
int systemio_test_gpio(char *failstr)
{
	int result = SYSIO_RESULT_FAIL;
	int fail_flag = 0;
	int ret;
	int i, j;
	int value;
	iotbus_gpio_edge_e edgemode;
	int signals[] = { 1, 0, 1 };
	int total_signals = sizeof(signals) / sizeof(int);
	int edges[] = { 2 /*GPIO_EDGE_RISING */ , 3 /*GPIO_EDGE_FALLING */ };
	int total_edges = sizeof(edges) / sizeof(int);
	struct gpio_s {
		iotbus_gpio_context_h cxt;
		int num;
	};
	static struct gpio_s gpio[] = { {NULL, 12}, {NULL, 14} };
	int totalGpios = sizeof(gpio) / sizeof(struct gpio_s);

	/* open */
	for (i = 0; i < totalGpios; i++) {
		/* open */
		gpio[i].cxt = iotbus_gpio_open(gpio[i].num);
		if (gpio[i].cxt == NULL) {
			SYSIO_DEBUG("[IOTAPI] iotbus_gpio_open(gpio%d) fail \n", gpio[i].num);
			REGISTER_FAIL_REASON("iotbus_gpio_open() fail", &fail_flag);
			continue;
		} else {
			SYSIO_DEBUG("[IOTAPI] iotbus_gpio_open(gpio%d) success \n", gpio[i].num);
		}

		/* write/read signal test */
		for (j = 0; j < total_signals; j++) {
			/* write */
			ret = iotbus_gpio_write(gpio[i].cxt, signals[j]);
			if (ret != IOTBUS_ERROR_NONE) {
				SYSIO_DEBUG("[IOTAPI] iotbus_gpio_write(gpio%d, signal=%d) fail \n",
							gpio[i].num,
							signals[j]);
				REGISTER_FAIL_REASON("iotbus_gpio_write() fail", &fail_flag);

			} else {
				SYSIO_DEBUG("[IOTAPI] iotbus_gpio_write(gpio%d, signal=%d) success \n",
							gpio[i].num,
							signals[j]);
			}

			/* read */
			value = iotbus_gpio_read(gpio[i].cxt);
			if (value < 0) {
				SYSIO_DEBUG("[IOTAPI] iotbus_gpio_read(gpio%d) fail \n", gpio[i].num);
				REGISTER_FAIL_REASON("iotbus_gpio_read() fail", &fail_flag);

			} else {
				SYSIO_DEBUG("[IOTAPI] iotbus_gpio_read(gpio%d) success \n", gpio[i].num);
				SYSIO_DEBUG("[IOTAPI] gpio%d signal verification : %s (expected val: %d, read val: %d) \n",
							gpio[i].num,
							signals[j] == value ? "SUCCESS" : "FAIL",
							signals[j],
							value);
			}
		}

		/* set/get edge test */
		for (j = 0; j < total_edges; j++) {
			/* set edge */
			ret = iotbus_gpio_set_edge_mode(gpio[i].cxt, edges[j]);
			if (ret != IOTBUS_ERROR_NONE) {
				SYSIO_DEBUG("[IOTAPI] iotbus_gpio_set_edge_mode(gpio%d, edge=%d) fail \n",
							gpio[i].num,
							edges[j]);
				REGISTER_FAIL_REASON("iotbus_gpio_set_edge_mode() fail", &fail_flag);

			} else {
				SYSIO_DEBUG("[IOTAPI] iotbus_gpio_set_edge_mode(gpio%d, edge=%d) success \n",
							gpio[i].num,
							edges[j]);
			}

			/* get edge */
			ret = iotbus_gpio_get_edge_mode(gpio[i].cxt, &edgemode);
			if (ret < 0) {
				SYSIO_DEBUG("[IOTAPI] iotbus_gpio_get_edge_mode(gpio%d) fail \n", gpio[i].num);
				REGISTER_FAIL_REASON("iotbus_gpio_get_edge_mode() fail", &fail_flag);

			} else {
				SYSIO_DEBUG("[IOTAPI] iotbus_gpio_get_edge_mode(gpio%d) success \n", gpio[i].num);
				SYSIO_DEBUG("[IOTAPI] gpio%d edge verification : %s (expected val: %d, read val: %d) \n",
							gpio[i].num,
							edges[j] == edgemode ? "SUCCESS" : "FAIL",
							edges[j],
							edgemode);
			}
		}

		/* get gpio pin number */
		value = iotbus_gpio_get_pin(gpio[i].cxt);
		SYSIO_DEBUG("[IOTAPI] iotbus_gpio_get_pin(gpio%d) success \n", gpio[i].num);
		SYSIO_DEBUG("[IOTAPI] gpio%d pin_number verification : %s (expected val: %d, read val: %d) \n",
					gpio[i].num,
					gpio[i].num == value ? "SUCCESS" : "FAIL",
					gpio[i].num,
					value);
		if (gpio[i].num != value) {
			REGISTER_FAIL_REASON("iotbus_gpio_get_pin() fail", &fail_flag);
		}

		/* close */
		ret = iotbus_gpio_close(gpio[i].cxt);
		if (ret != IOTBUS_ERROR_NONE) {
			SYSIO_DEBUG("[IOTAPI] iotbus_gpio_close(gpio%d) fail \n", gpio[i].num);
			REGISTER_FAIL_REASON("iotbus_gpio_close() fail", &fail_flag);

		} else {
			SYSIO_DEBUG("[IOTAPI] iotbus_gpio_close(gpio%d) success \n", gpio[i].num);
		}

		SYSIO_DEBUG("\n");
	}

	/* check if succeed */
	if (fail_flag == 0) {
		result = SYSIO_RESULT_SUCCESS;
	}

	return result;
}

/******************************
     PWM TEST
 ******************************/
int systemio_test_pwm(char *failstr)
{
	int result = SYSIO_RESULT_FAIL;
	int fail_flag = 0;

	int device = 0, channel = 1;

	iotbus_pwm_context_h hnd;

	hnd = iotbus_pwm_open(device, channel);
	if (!hnd) {
		SYSIO_DEBUG("[IOTAPI] iotbus_pwm_open() fail \n");
		REGISTER_FAIL_REASON("iotbus_pwm_open() fail", &fail_flag);

		goto done;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_pwm_open() success \n");
	}

	int dc = 20;				// 20 %
	int freq = 1000;			// 1000 Hz
	int ret = iotbus_pwm_set_duty_cycle(hnd, dc);
	if (ret < 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_pwm_set_duty_cycle() fail \n");
		REGISTER_FAIL_REASON("iotbus_pwm_set_duty_cycle()", &fail_flag);
		// goto done ;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_pwm_set_duty_cycle() success \n");
	}

	ret = iotbus_pwm_set_period(hnd, freq);
	if (ret < 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_pwm_set_period() fail\n");
		REGISTER_FAIL_REASON("iotbus_pwm_set_period() fail", &fail_flag);
		// goto done ;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_pwm_set_period() success \n");
	}

	ret = iotbus_pwm_is_enabled(hnd);
	if (ret < 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_pwm_is_enabled() fail \n");
		REGISTER_FAIL_REASON("iotbus_pwm_is_enabled() fail", &fail_flag);
		// goto done ;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_pwm_is_enabled() success \n");
		SYSIO_DEBUG("[IOTAPI] pwm enabled=%d \n", ret);
	}

	ret = iotbus_pwm_set_enabled(hnd, IOTBUS_PWM_ENABLE);
	if (ret < 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_pwm_set_enabled(IOTBUS_PWM_ENABLE) fail \n");
		REGISTER_FAIL_REASON("iotbus_pwm_set_enabled(1) fail", &fail_flag);
		// goto done ;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_pwm_set_enabled(IOTBUS_PWM_ENABLE) success \n");
	}

	int cnt = 0;
	dc = 20;
	while (cnt < 10) {
		ret = iotbus_pwm_set_duty_cycle(hnd, dc);
		if (ret < 0) {
			SYSIO_DEBUG("[IOTAPI] iotbus_pwm_set_duty_cycle(dc = %d) fail \n", dc);
			REGISTER_FAIL_REASON("iotbus_pwm_set_duty_cycle()", &fail_flag);
			// goto done ;
		} else {
			SYSIO_DEBUG("[IOTAPI] iotbus_pwm_set_duty_cycle(dc = %d) success \n", dc);
		}
		dc += 5;
		cnt++;
	}

	ret = iotbus_pwm_get_duty_cycle(hnd);
	if (ret < 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_pwm_get_duty_cycle() fail \n");
		REGISTER_FAIL_REASON("iotbus_pwm_get_duty_cycle()", &fail_flag);
		// goto done ;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_pwm_get_duty_cycle() success \n");
		SYSIO_DEBUG("[IOTAPI] pwm duty cycle=%d \n", ret);
	}

	ret = iotbus_pwm_get_period(hnd);
	if (ret < 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_pwm_get_period() fail \n");
		REGISTER_FAIL_REASON("iotbus_pwm_get_period()", &fail_flag);
		// goto done ;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_pwm_get_period() success \n");
		SYSIO_DEBUG("[IOTAPI] pwm period=%d \n", ret);
	}

	ret = iotbus_pwm_set_enabled(hnd, IOTBUS_PWM_DISABLE);
	if (ret < 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_pwm_set_enabled(IOTBUS_PWM_DISABLE) fail \n");
		REGISTER_FAIL_REASON("iotbus_pwm_set_enabled(0)", &fail_flag);
		// goto done ;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_pwm_set_enabled(IOTBUS_PWM_DISABLE) success \n");
	}

	ret = iotbus_pwm_is_enabled(hnd);
	if (ret < 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_pwm_is_enabled() fail \n");
		REGISTER_FAIL_REASON("iotbus_pwm_is_enabled()", &fail_flag);
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_pwm_is_enabled() success \n");
		SYSIO_DEBUG("[IOTAPI] pwm enabled=%d \n", ret);
	}

	ret = iotbus_pwm_close(hnd);
	if (ret < 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_pwm_close() fail \n");
		REGISTER_FAIL_REASON("iotbus_pwm_close()", &fail_flag);
		// goto done ;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_pwm_close() success \n");
	}

done:
	/* check if succeed */
	if (fail_flag == 0) {
		result = SYSIO_RESULT_SUCCESS;
	}

	return result;
}

/******************************
     I2C TEST
 ******************************/
int systemio_test_i2c(char *failstr)
{
	// i2c read write test
	int result = SYSIO_RESULT_FAIL;
	int fail_flag = 0;
	int ret = 0;
	iotbus_i2c_context_h hnd;
	int bus_num = 1;

	// changing addr and cmd depending on board
	uint8_t addr = 0x08;
	uint8_t cmd[2] = { 0x01, 0x02 };

	hnd = iotbus_i2c_init(bus_num);
	if (hnd == NULL) {
		SYSIO_DEBUG("[IOTAPI] iotbus_i2c_init() fail \n");
		REGISTER_FAIL_REASON("iotbus_i2c_init() fail", &fail_flag);
		goto done;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_i2c_init() success \n");
	}

	ret = iotbus_i2c_set_frequency(hnd, IOTBUS_I2C_STD);
	if (ret < 0) {
		REGISTER_FAIL_REASON("iotbus_i2c_set_frequency() fail", &fail_flag);
		SYSIO_DEBUG("[IOTAPI] iotbus_i2c_set_frequency() fail \n");
		//return ;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_i2c_set_frequency() success \n");
	}

	ret = iotbus_i2c_set_address(hnd, addr);
	if (ret < 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_i2c_set_address() fail \n");
		REGISTER_FAIL_REASON("iotbus_i2c_set_address() fail", &fail_flag);
		//return ;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_i2c_set_address() success \n");
	}

	ret = iotbus_i2c_write(hnd, cmd, 2);
	if (ret < 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_i2c_write() fail \n");
		//      REGISTER_FAIL_REASON("iotbus_i2c_write() fail", &fail_flag);
		//return ;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_i2c_write() success \n");
	}

	uint8_t buf[10];
	ret = iotbus_i2c_read(hnd, buf, 10);
	if (ret < 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_i2c_read() fail \n");
		//      REGISTER_FAIL_REASON("iotbus_i2c_read() fail", &fail_flag);
		//return ;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_i2c_read() success \n");
	}

	ret = iotbus_i2c_stop(hnd);
	if (ret < 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_i2c_stop() fail \n");
		REGISTER_FAIL_REASON("iotbus_i2c_stop() fail", &fail_flag);
		//return ;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_i2c_stop() success \n");
	}

done:
	/* check if succeed */
	if (fail_flag == 0) {
		result = SYSIO_RESULT_SUCCESS;
	}

	return result;

}

/******************************
     SPI TEST
 ******************************/
int systemio_test_spi(char *failstr)
{
	int result = SYSIO_RESULT_FAIL;
	int fail_flag = 0;
	int ret;
	iotbus_spi_context_h hSpi;
	unsigned int bus = 0;
	unsigned char txbuf[64] = { 0, };
	unsigned char rxbuf[64] = { 0, };;
	struct iotbus_spi_config_s config = {
		(char)8,
		0,
		12000000,
		0,
	};

	hSpi = iotbus_spi_open(bus, &config);
	if (!hSpi) {
		SYSIO_DEBUG("[IOTAPI] iotbus_spi_open() fail \n");
		REGISTER_FAIL_REASON("iotbus_spi_open() fail", &fail_flag);
		goto done;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_spi_open() success \n");
	}

	ret = iotbus_spi_write(hSpi, txbuf, 8);
	if (ret != 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_spi_write() fail \n");
		REGISTER_FAIL_REASON("iotbus_spi_write() fail", &fail_flag);
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_spi_write() success \n");
	}

	ret = iotbus_spi_recv(hSpi, rxbuf, 8);
	if (ret != 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_spi_recv() fail \n");
		REGISTER_FAIL_REASON("iotbus_spi_recv() fail", &fail_flag);
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_spi_recv() success \n");
	}

#if 0
	ret = iotbus_spi_transfer_buf(hSpi, txbuf, rxbuf, 16);
	if (ret != 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_spi_transfer_buf() fail \n");
		REGISTER_FAIL_REASON("iotbus_spi_transfer_buf() fail");
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_spi_transfer_buf() success \n");
	}
#endif

	ret = iotbus_spi_close(hSpi);
	if (ret != 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_spi_close() fail \n");
		REGISTER_FAIL_REASON("iotbus_spi_close() fail", &fail_flag);
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_spi_close() success \n");
	}

done:
	/* check if succeed */
	if (fail_flag == 0) {
		result = SYSIO_RESULT_SUCCESS;
	}

	return result;
}

/******************************
     UART TEST
 ******************************/
int systemio_test_uart(char *failstr)
{
	int result = SYSIO_RESULT_FAIL;
	int fail_flag = 0;
	iotbus_uart_context_h uart;
	int ret = -1;
	char *dev_path = "/dev/ttyS1";

	uart = iotbus_uart_init(dev_path);
	if (!uart) {
		SYSIO_DEBUG("[IOTAPI] iotbus_uart_init() fail \n");
		REGISTER_FAIL_REASON("iotbus_uart_init() fail", &fail_flag);
		goto errout_without_handle;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_uart_init() success \n");
	}

	unsigned int baudrate = 115200;
	ret = iotbus_uart_set_baudrate(uart, baudrate);
	if (ret < 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_uart_set_baudrate(baudrate=%d) fail \n", baudrate);
		REGISTER_FAIL_REASON("iotbus_uart_set_baudrate() fail", &fail_flag);
		goto errout_with_handle;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_uart_set_baudrate(baudrate=%d) success \n", baudrate);
	}

	ret = iotbus_uart_set_mode(uart, 8, IOTBUS_UART_PARITY_NONE, 1);
	if (ret < 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_uart_set_mode(mode=8N1) fail \n");
		REGISTER_FAIL_REASON("iotbus_uart_set_mode() fail", &fail_flag);
		goto errout_with_handle;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_uart_set_mode(mode=8N1) success \n");
	}

	ret = iotbus_uart_set_flowcontrol(uart, 1, 1);
	if (ret < 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_uart_set_flowcontrol(1,1) fail \n");
		REGISTER_FAIL_REASON("iotbus_uart_set_flowcontrol() fail", &fail_flag);
		goto errout_with_handle;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_uart_set_flowcontrol(1,1) success \n");
	}

#if 0
	char rbuf[20];
	ret = iotbus_uart_read(uart, rbuf, sizeof(rbuf));
	if (ret < 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_uart_read() fail \n");
		REGISTER_FAIL_REASON("iotbus_uart_read() fail", &fail_flag);
		//goto errout_with_handle;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_uart_read() success \n");
	}
#endif

	char tbuf[32] = "Go Bears!UART WRITE TEST OK!\n";
	ret = iotbus_uart_write(uart, tbuf, sizeof(tbuf));
	if (ret < 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_uart_write() fail \n");
		//      REGISTER_FAIL_REASON("iotbus_uart_write() fail", &fail_flag);
		//goto errout_with_handle;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_uart_write() success \n");
	}

	ret = iotbus_uart_flush(uart);
	if (ret < 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_uart_flush() fail \n");
		//      REGISTER_FAIL_REASON("iotbus_uart_flush() fail", &fail_flag);
		//goto errout_with_handle;
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_uart_flush() success \n");
	}

errout_with_handle:
	ret = iotbus_uart_stop(uart);
	if (ret < 0) {
		SYSIO_DEBUG("[IOTAPI] iotbus_uart_stop() fail \n");
		REGISTER_FAIL_REASON("iotbus_uart_stop() fail", &fail_flag);
	} else {
		SYSIO_DEBUG("[IOTAPI] iotbus_uart_stop() success \n");
	}

errout_without_handle:
	/* check if succeed */
	if (fail_flag == 0) {
		result = SYSIO_RESULT_SUCCESS;
	}
	return result;
}

/****************************************************************************
 * mdns_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int sysio_main(int argc, char *argv[])
#endif
{
	int result = SYSIO_RESULT_FAIL;
	int cmdtype = 0;			/* 1: gpio, 2: pwm, 3: i2c, 4: spi, 5: uart */
	int ret = 0;
	char fail_reason[32] = { 0, };
	char *test_title;

	g_debug = 0;
	switch (argc) {
	case 3:
		g_debug = atoi(argv[2]);
	/* Fall Through */

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
		} else {
			show_usage(argv[0]);
		}
		break;

	default:
		show_usage(argv[0]);
		break;
	}

	switch (cmdtype) {
	case 1:					/* gpio */
		test_title = "GPIO";
		ret = systemio_test_gpio(fail_reason);
		break;

	case 2:					/* pwm */
		test_title = "PWM";
		ret = systemio_test_pwm(fail_reason);
		break;

	case 3:					/* i2c */
		test_title = "I2C";
		ret = systemio_test_i2c(fail_reason);
		break;

	case 4:					/* spi */
		test_title = "SPI";
		ret = systemio_test_spi(fail_reason);
		break;

	case 5:					/* uart */
		test_title = "UART";
		ret = systemio_test_uart(fail_reason);
		break;

	default:
		test_title = "N/A";
		goto done;

	}

	/* result is success */
	if (ret == SYSIO_RESULT_SUCCESS) {
		result = SYSIO_RESULT_SUCCESS;
	}

done:
	SYSIO_PRINT("- %s Test Result : %s ",
				test_title,
				result == SYSIO_RESULT_SUCCESS ? "SUCCESS" : "FAIL");
	if (result == SYSIO_RESULT_FAIL) {
		SYSIO_PRINT("(reason : %s) \n", fail_reason);
	} else {
		SYSIO_PRINT("\n");
	}

	return result;
}
