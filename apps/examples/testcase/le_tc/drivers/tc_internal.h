/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

/// @brief Header file for Drivers TestCase Example
#ifndef __EXAMPLES_TESTCASE_DRIVERS_TC_INTERNAL_H
#define __EXAMPLES_TESTCASE_DRIVERS_TC_INTERNAL_H

#include "tc_common.h"
#include <tinyara/config.h>

/**********************************************************
* TC Function Declarations
**********************************************************/
#ifdef CONFIG_TC_DRIVERS_NULL
void dev_null_main(void);
#endif

#ifdef CONFIG_TC_DRIVERS_ZERO
void dev_zero_main(void);
#endif

#ifdef CONFIG_TC_DRIVERS_RTC
void rtc_main(void);
#endif

#ifdef CONFIG_TC_DRIVERS_PWM
void pwm_main(void);
#endif

#ifdef CONFIG_TC_DRIVERS_WATCHDOG
void watchdog_main(void);
#endif

#ifdef CONFIG_TC_DRIVERS_LOOP
void loop_main(void);
#endif

#ifdef CONFIG_TC_DRIVERS_ADC
void adc_main(void);
#endif

#ifdef CONFIG_TC_DRIVERS_BCH
void bch_main(void);
#endif

#ifdef CONFIG_TC_DRIVERS_I2C
void i2c_main(void);
#endif

#endif							/* __EXAMPLES_TESTCASE_DRIVERS_TC_INTERNAL_H */
