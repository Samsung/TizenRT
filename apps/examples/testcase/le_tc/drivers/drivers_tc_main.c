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

/// @file drivers_tc_main.c

/// @brief Main Function for Drivers TestCase Example

#include <tinyara/config.h>
#include "tc_internal.h"
#include "tc_common.h"
#include <sys/types.h>

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int tc_drivers_main(int argc, char *argv[])
#endif
{
	if (tc_handler(TC_START, "Drivers TC") == ERROR) {
		return ERROR;
	}
#ifdef CONFIG_TC_DRIVERS_NULL
	dev_null_main();
#endif

#ifdef CONFIG_TC_DRIVERS_ZERO
	dev_zero_main();
#endif

#ifdef CONFIG_TC_DRIVERS_RTC
	rtc_main();
#endif

#ifdef CONFIG_TC_DRIVERS_PWM
	pwm_main();
#endif

#ifdef CONFIG_TC_DRIVERS_WATCHDOG
	watchdog_main();
#endif

#ifdef CONFIG_TC_DRIVERS_LOOP
	loop_main();
#endif

#ifdef CONFIG_TC_DRIVERS_ADC
	adc_main();
#endif

#ifdef CONFIG_TC_DRIVERS_BCH
	bch_main();
#endif

#ifdef CONFIG_TC_DRIVERS_I2C
	i2c_main();
#endif

	(void)tc_handler(TC_END, "Drivers TC");

	return 0;
}
