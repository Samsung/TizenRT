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

/// @file tc_i2c.c
/// @brief Test Case Example for i2c driver
#include <tinyara/config.h>
#include <stdio.h>
#include <fcntl.h>
#include "tc_internal.h"
#include <tinyara/i2c.h>

/**
* @fn                   :tc_driver_i2c_transfer
* @brief                :Test the i2c driver transfer api's
* @scenario             :Test the i2c driver
* API's covered         :i2c_read, i2c_write, i2c_writeread
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/

static void tc_driver_i2c_transfer(void)
{
	int ret = 0;
	FAR struct i2c_config_s config;
	uint8_t buf[2];
	uint8_t addr[2];
	int port = 1;

	config.frequency = 100000;
	config.addrlen = 7;

#ifndef CONFIG_DISABLE_MANUAL_TESTCASE
	/* Manual test case is enabled. I2C compatible device must be present
	   in the board. Update the device address and register address  below */
	config.address = 0x1A;
	addr[0] = 0x00;
	addr[1] = 0x00;
	/* Change the I2C port based on hardware configuration below */
	/* port = xx */
#define ASSERT_I2C_TRANSFER TC_ASSERT_GEQ_CLEANUP
#else
	/* I2C hardware not present or configured.
	   So, we will test for negative results */
#define ASSERT_I2C_TRANSFER TC_ASSERT_LT_CLEANUP
#endif

	FAR struct i2c_dev_s *dev = up_i2cinitialize(port);
	TC_ASSERT_GT("up_i2cinitialize", dev, 0);

	/* Positive test cases */
	ret = i2c_write(dev, &config, addr, 1);
	ASSERT_I2C_TRANSFER("i2c_write", ret, 0, up_i2cuninitialize(dev));

	ret = i2c_read(dev, &config, buf, 1);
	ASSERT_I2C_TRANSFER("i2c_read", ret, 0, up_i2cuninitialize(dev));

	ret = i2c_writeread(dev, &config, addr, 2, buf, 2);
	ASSERT_I2C_TRANSFER("i2c_writeread", ret, 0, up_i2cuninitialize(dev));

	ret = i2c_writeread(dev, &config, addr, 2, buf, -2);
	ASSERT_I2C_TRANSFER("i2c_writeread", ret, 0, up_i2cuninitialize(dev));

	/* Negative test cases */
	ret = i2c_read(0, &config, buf, 2);
	TC_ASSERT_LT_CLEANUP("i2c_read", ret, 0, up_i2cuninitialize(dev));

	ret = i2c_write(0, &config, buf, 2);
	TC_ASSERT_LT_CLEANUP("i2c_write", ret, 0, up_i2cuninitialize(dev));

	ret = i2c_writeread(0, &config, buf, 2, buf, 2);
	TC_ASSERT_LT_CLEANUP("i2c_writeread", ret, 0, up_i2cuninitialize(dev));

	up_i2cuninitialize(dev);

	TC_SUCCESS_RESULT();

}

/****************************************************************************
 * Name: i2c driver test
 ****************************************************************************/
void i2c_main(void)
{
	tc_driver_i2c_transfer();
	return;
}
