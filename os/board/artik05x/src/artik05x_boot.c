/*****************************************************************************
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
/****************************************************************************
 * board/artik05x/src/artik05x_boot.c
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

#include <tinyara/gpio.h>
#include <tinyara/board.h>

#include "up_arch.h"
#include "s5j_gpio.h"
#include "s5j_i2c.h"
#include "s5j_watchdog.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/***************************************************************************
 * Name: board_gpio_initialize
 *
 * Description:
 *  Expose board dependent GPIOs
 ****************************************************************************/
static void board_gpio_initialize(void)
{
#ifdef CONFIG_GPIO
	int i;
	struct gpio_lowerhalf_s *lower;

	struct {
		uint8_t minor;
		uint16_t pincfg;
	} pins[] = {
		{ 29, GPIO_XGPIO0 }, /* ARTIK_A053_XGPIO0 */
		{ 30, GPIO_XGPIO1 }, /* ARTIK_A053_XGPIO1 */
		{ 31, GPIO_XGPIO2 }, /* ARTIK_A053_XGPIO2 */
		{ 32, GPIO_XGPIO3 }, /* ARTIK_A053_XGPIO3 */
		{ 33, GPIO_XGPIO4 }, /* ARTIK_A053_XGPIO3 */
		{ 34, GPIO_XGPIO5 }, /* ARTIK_A053_XGPIO3 */
		{ 35, GPIO_XGPIO6 }, /* ARTIK_A053_XGPIO3 */
		{ 36, GPIO_XGPIO7 }, /* ARTIK_A053_XGPIO3 */
		{ 37, GPIO_XGPIO8 }, /* ARTIK_A053_XGPIO8 */
		{ 38, GPIO_XGPIO9 }, /* ARTIK_A053_XGPIO9 */
		{ 39, GPIO_XGPIO10 }, /* ARTIK_A053_XGPIO10 */
		{ 40, GPIO_XGPIO11 }, /* ARTIK_A053_XGPIO11 */
		{ 41, GPIO_XGPIO12 }, /* ARTIK_A053_XGPIO12 */
		{ 42, GPIO_XGPIO13 }, /* ARTIK_A053_XGPIO13 */
		{ 43, GPIO_XGPIO14 }, /* ARTIK_A053_XGPIO14 */
		{ 44, GPIO_XGPIO15 }, /* ARTIK_A053_XGPIO15 */
		{ 45, GPIO_XGPIO16 }, /* ARTIK_A053_XGPIO16 */
		{ 46, GPIO_XGPIO17 }, /* ARTIK_A053_XGPIO17 */
		{ 47, GPIO_XGPIO18 }, /* ARTIK_A053_XGPIO18 */
		{ 48, GPIO_XGPIO19 }, /* ARTIK_A053_XGPIO19 */
		{ 49, GPIO_XGPIO20 }, /* ARTIK_A053_XGPIO20 */
		{ 50, GPIO_XGPIO21 }, /* ARTIK_A053_XGPIO21 */
		{ 51, GPIO_XGPIO22 }, /* ARTIK_A053_XGPIO22 */
		{ 52, GPIO_XGPIO23 }, /* ARTIK_A053_XGPIO23 */
		{ 53, GPIO_XGPIO24 }, /* ARTIK_A053_XGPIO24 */
		{ 54, GPIO_XGPIO25 }, /* ARTIK_A053_XGPIO25 */
		{ 55, GPIO_XGPIO26 }, /* ARTIK_A053_XGPIO26 */
		{ 56, GPIO_XGPIO27 }, /* ARTIK_A053_XGPIO27 */
		{ 20, GPIO_XGPIO28 }, /* ARTIK_A053_XGPIO28 */
		{ 57, GPIO_XEINT0 }, /* ARTIK_A053_XEINT0 */
		{ 58, GPIO_XEINT1 }, /* ARTIK_A053_XEINT1 */
		{ 59, GPIO_XEINT2 }, /* ARTIK_A053_XEINT2 */
	};

	for (i = 0; i < sizeof(pins) / sizeof(*pins); i++) {
		lower = s5j_gpio_lowerhalf(pins[i].pincfg);
		gpio_register(pins[i].minor, lower);
	}
#endif							/* CONFIG_GPIO */
}

/****************************************************************************
 * Name: board_i2c_initialize
 *
 * Description:
 *  Expose board dependent I2Cs
 ****************************************************************************/
static void board_i2c_initialize(void)
{
#if defined(CONFIG_I2C) && defined(CONFIG_S5J_I2C)
	s5j_i2c_register(0);
	s5j_i2c_register(1);
#endif
}

/****************************************************************************
 * Name: board_audio_initialize
 *
 * Description:
 *  Initialize all audio related
 ****************************************************************************/
static void board_audio_initialize(void)
{
#if defined(CONFIG_AUDIO_ALC5658)
	s5j_alc5658_initialize(0);
#elif defined(CONFIG_AUDIO_ALC5658CHAR)
	s5j_alc5658char_initialize(0);
#elif defined(CONFIG_AUDIO_I2SCHAR)
	alc5658_i2c_initialize();
	i2schar_devinit();
#endif
}

static void board_wdt_initialize(void)
{
#ifdef CONFIG_S5J_WATCHDOG
#ifdef CONFIG_WATCHDOG
	s5j_wdg_initialize(CONFIG_WATCHDOG_DEVPATH);
	putreg32(0x40C, 0x80090000);
#endif
#endif
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
static void artik05x_clear_bootcount(void)
{
#ifdef CONFIG_ARTIK05X_BOOT_COUNTS_ADDR
	/*
	 * As BL2 sets up a watchdog before it jumps to secondary OS,
	 * we should disable the watchdog to prevent it from barking.
	 *
	 * FIXME: use watchdog driver rather than accessing SFR directly
	 */
	putreg32(0, 0x80030000);

	/* then, clear the boot count */
	putreg32(0, CONFIG_ARTIK05X_BOOT_COUNTS_ADDR);
#endif
}

/****************************************************************************
 * Name: up_flashinitialize
 *
 * Description:
 *   Create an initialized MTD device instance for internal flash.
 *   Initialize MTD master & each of partitions here.
 *   Each of partition classfied according to TAG number
 *
 ****************************************************************************/
FAR struct mtd_dev_s *up_flashinitialize(void)
{
	return NULL;
}

/****************************************************************************
 * Name: s5j_board_initialize
 *
 * Description:
 *   All S5J architectures must provide the following entry point. This entry
 *   point is called early in the initialization -- after all memory has been
 *   configured and mapped but before any devices have been initialized.
 *
 *   This function must perform low level initialization including:
 *
 *   - Initialization of board-specific memory resources (e.g., SDRAM)
 *   - Configuration of board-specific resources (GPIOs, LEDs, etc).
 *   - Setup of the console UART. This UART done early so that the serial
 *     console is available for debugging very early in the boot sequence.
 *
 *   Special precautions must be taken if .data/.bss lie in SRAM. In that
 *   case, the boot logic cannot initialize .data or .bss. The function
 *   must then:
 *
 *   - Take precautions to assume that logic does not access any global
 *     data that might lie in SDRAM.
 *   - Call the function arm_data_initialize() as soon as SDRAM has been
 *     properly configured for use.
 *
 ****************************************************************************/
void s5j_board_initialize(void)
{
#ifdef CONFIG_S5J_SFLASH
	s5j_sflash_init();
#endif
}

/****************************************************************************
 * Name: board_sensor_initialize
 *
 * Description:
 *  Expose board dependent Sensors
 ****************************************************************************/
static void board_sensor_initialize(void)
{
#if defined(CONFIG_SENSOR_PPD42NS) && defined(CONFIG_S5J_SENSOR_PPD42NS)
	s5j_ppd42ns_initialize();
#endif
}

#ifdef CONFIG_BOARD_INITIALIZE
/****************************************************************************
 * Name: board_initialize
 *
 * Description:
 *   If CONFIG_BOARD_INITIALIZE is selected, then an additional
 *   initialization call will be performed in the boot-up sequence to a
 *   function called board_initialize().  board_initialize() will be
 *   called immediately after up_initialize() is called and just before the
 *   initial application is started.  This additional initialization phase
 *   may be used, for example, to initialize board-specific device drivers.
 *
 ****************************************************************************/
void board_initialize(void)
{
	artik05x_clear_bootcount();

	/* Perform app-specific initialization here instaed of from the TASH. */
	board_app_initialize();

#ifdef CONFIG_SCSC_WLAN
	slsi_driver_initialize();
#endif

#ifdef CONFIG_S5J_PWM
	board_pwm_setup();
#endif

	board_gpio_initialize();
	board_i2c_initialize();
	board_audio_initialize();
	board_sensor_initialize();
	board_wdt_initialize();

#ifdef CONFIG_S5J_SSS
	/* verify ARTIK Key */
	sssro_verify();
#endif
}
#endif							/* CONFIG_BOARD_INITIALIZE */
