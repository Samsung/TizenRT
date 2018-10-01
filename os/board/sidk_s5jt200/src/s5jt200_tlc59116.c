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
/****************************************************************************
 * board/sidk_s5jt200/src/s5jt200_tlc59116.c
 *
 *   Copyright (C) 2009, 2011, 2013, 2015 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *           Laurent Latil <laurent@latil.nom.fr>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <debug.h>
#include <assert.h>
#include <sys/types.h>

#include <tinyara/board.h>
#include <arch/board/board.h>
#include <tinyara/i2c.h>
#include <iotbus/iotbus_i2c.h>
#include <sidk_s5jt200.h>

/****************************************************************************
 * Private Data
 ****************************************************************************/
static struct i2c_dev_s *i2c_dev;
static struct i2c_config_s configs;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static uint16_t tlc59xxx_write(uint8_t addr, uint8_t data)
{
	int32_t ret;
	uint8_t reg[2];

	reg[0] = addr;
	reg[1] = data;

	ret = i2c_write(i2c_dev, &configs, reg, 2);

	return ret;
}

static uint32_t tlc59xxx_read(uint8_t addr, uint8_t *data, uint8_t bytecnt)
{
	int32_t ret;
	uint8_t reg;

	reg = addr;
	ret = i2c_write(i2c_dev, &configs, &reg, 1);

	ret = i2c_read(i2c_dev, &configs, data, bytecnt);

	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
void tlc59116_ledon(int led, uint8_t duty)
{
	switch (led) {
	default:
	case TLC59116_LED_0:
		tlc59xxx_write(TLC59116_REG_PWM(0), duty);
		tlc59xxx_write(TLC59116_REG_PWM(1), duty);
		tlc59xxx_write(TLC59116_REG_PWM(2), duty);
		break;

	case TLC59116_LED_1:
		tlc59xxx_write(TLC59116_REG_PWM(4), duty);
		tlc59xxx_write(TLC59116_REG_PWM(5), duty);
		tlc59xxx_write(TLC59116_REG_PWM(6), duty);
		break;

	case TLC59116_LED_2:
		tlc59xxx_write(TLC59116_REG_PWM(9), duty);
		tlc59xxx_write(TLC59116_REG_PWM(10), duty);
		tlc59xxx_write(TLC59116_REG_PWM(11), duty);
		break;

	case TLC59116_LED_3:
		tlc59xxx_write(TLC59116_REG_PWM(13), duty);
		tlc59xxx_write(TLC59116_REG_PWM(14), duty);
		tlc59xxx_write(TLC59116_REG_PWM(15), duty);
		break;
	}
}

void tlc59116_ledoff(int led, int rgb)
{
	switch (led) {
	default:
	case TLC59116_LED_0:
		tlc59xxx_write(TLC59116_REG_PWM(4), 0x0);
		tlc59xxx_write(TLC59116_REG_PWM(5), 0x0);
		tlc59xxx_write(TLC59116_REG_PWM(6), 0x0);
		break;

	case TLC59116_LED_1:
		tlc59xxx_write(TLC59116_REG_PWM(4), 0x0);
		tlc59xxx_write(TLC59116_REG_PWM(5), 0x0);
		tlc59xxx_write(TLC59116_REG_PWM(6), 0x0);
		break;

	case TLC59116_LED_2:
		tlc59xxx_write(TLC59116_REG_PWM(4), 0x0);
		tlc59xxx_write(TLC59116_REG_PWM(5), 0x0);
		tlc59xxx_write(TLC59116_REG_PWM(6), 0x0);
		break;

	case TLC59116_LED_3:
		tlc59xxx_write(TLC59116_REG_PWM(4), 0x0);
		tlc59xxx_write(TLC59116_REG_PWM(5), 0x0);
		tlc59xxx_write(TLC59116_REG_PWM(6), 0x0);
		break;
	}
}

int tlc59116_initialize(void)
{
	uint8_t port = 0;

	i2c_dev = up_i2cinitialize(port);
	if (i2c_dev == NULL) {
		slldbg("i2ctest_main: up_i2cinitialize(i2c:%d) failed\n", port);
		return ERROR;
	}

	configs.frequency = 100000;
	configs.address = TLC59116_ADDR;
	configs.addrlen = 7;

	tlc59xxx_write(TLC59116_REG_MODE1, 0x00);
	tlc59xxx_write(TLC59116_REG_MODE2, 0x00);

	/*
	 * LED driver x is individual brightness and group dimming/blinking
	 * can be controlled through its PWMx register.
	 */
	tlc59xxx_write(TLC59116_REG_LED(0), 0xFF);
	tlc59xxx_write(TLC59116_REG_LED(1), 0xFF);
	tlc59xxx_write(TLC59116_REG_LED(2), 0xFF);
	tlc59xxx_write(TLC59116_REG_LED(3), 0xFF);

	/*
	 * 97-kHz fixed frequency signal is used for each output.
	 * Set 8 steps to LED0~3 (duty 3.1%) as default.
	 */
	tlc59116_ledon(TLC59116_LED_0, 0x8);
	tlc59116_ledon(TLC59116_LED_1, 0x8);
	tlc59116_ledon(TLC59116_LED_2, 0x8);
	tlc59116_ledon(TLC59116_LED_3, 0x8);

	return OK;
}
