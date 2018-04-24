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

/// @file tc_internal.h

/// @brief Header file for System IO TestCase Example
#ifndef __EXAMPLES_TESTCASE_SYSTEMIO_UTC_INTERNAL_H
#define __EXAMPLES_TESTCASE_SYSTEMIO_UTC_INTERNAL_H

//#define IS_PASS(value) value==1?"PASS":"FAIL"

#include "tc_common.h"

#define SYSIO_UTC_PRINT   printf
#define SYSIO_UTC_UART_PRINT   printf
#define SYSIO_UTC_GPIO_PRINT   printf
#define SYSIO_UTC_PWM_PRINT   printf
#define SYSIO_UTC_I2C_PRINT   printf

int utc_gpio_main(void);
int utc_i2c_main(void);
int utc_pwm_main(void);
int utc_spi_main(void);
int utc_uart_main(void);

#endif							/* __EXAMPLES_TESTCASE_SYSTEMIO_UTC_INTERNAL_H */
