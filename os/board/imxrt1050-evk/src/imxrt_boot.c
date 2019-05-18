/****************************************************************************
 *
 * Copyright 2019 NXP Semiconductors All Rights Reserved.
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
 * os/board/imxrt1050-evk/src/imxrt_boot.c
 *
 *   Copyright (C) 2015 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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
 * 3. Neither the name TinyARA nor the names of its contributors may be
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

#include <tinyara/board.h>
#include <tinyara/pwm.h>
#include <tinyara/gpio.h>
#include <tinyara/spi/spi.h>

#include <arch/board/board.h>
#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT102x)
#include "chip/imxrt102x_config.h"
#include "imxrt1020-evk.h"
#elif defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
#include "chip/imxrt105x_config.h"
#include "imxrt1050-evk.h"
#else
#error Unrecognized i.MX RT architecture
#endif

#include "imxrt_start.h"
#include "imxrt_pwm.h"
#include "imxrt_flash.h"
 #include "imxrt_gpio.h"
#include <tinyara/gpio.h>
#include "imxrt_lpspi.h"

#ifdef CONFIG_IMXRT_SEMC_SDRAM
#include "imxrt_semc_sdram.h"
#endif

/****************************************************************************
 * Name: board_gpio_initialize
 *
 * Description:
 *   GPIO intialization for imxrt
 *
 ****************************************************************************/
static void imxrt_gpio_initialize(void)
{
#ifdef CONFIG_GPIO
	int i;
	struct gpio_lowerhalf_s *lower;

	struct {
		uint8_t minor;
		gpio_pinset_t pinset;
	} pins[] = {
		{
			9, GPIO_LED
		},
		{
			11, GPIO_LED2
		},
	};

	for (i = 0; i < sizeof(pins) / sizeof(*pins); i++) {
		lower = imxrt_gpio_lowerhalf(pins[i].pinset);
		gpio_register(pins[i].minor, lower);
	}
#endif
}

/****************************************************************************
 * Name: board_pwmm_initialize
 *
 * Description:
 *   PWM intialization for imxrt
 *
 ****************************************************************************/
static void imxrt_pwm_initialize(void)
{
#ifdef CONFIG_PWM
	struct pwm_lowerhalf_s *pwm;
	char path[10];
	int ret;
	int i;

	for (i = 0; i < PWM_CNT_COUNT; i++) {
		pwm = imxrt_pwminitialize(i);
		if (!pwm) {
			lldbg("Failed to get imxrt PWM lower half\n");
			return;
		}

		/* Register the PWM driver at "/dev/pwmx" */
		snprintf(path, sizeof(path), "/dev/pwm%d", i);
		ret = pwm_register(path, pwm);
		if (ret < 0) {
			lldbg("Imxrt PWM registeration failure: %d\n", ret);
		}
	}
#endif
	return;
}

/****************************************************************************
 * Name: imxrt_spi_initialize
 *
 * Description:
 *   SPI intialization for imxrt
 *
 ****************************************************************************/
void imxrt_spi_initialize(void)
{
#ifdef CONFIG_SPI
	struct spi_dev_s *spi;
	spi = up_spiinitialize(1);

#ifdef CONFIG_SPI_USERIO
	if (spi_uioregister(1, spi) < 0) {
		lldbg("Failed to register SPI%d\n", 1);
	}
#endif
#endif
}

/****************************************************************************
 * Name: imxrt_boardinitialize
 *
 * Description:
 *   All i.MX RT architectures must provide the following entry point.  This
 *   entry point is called early in the initialization -- after clocking and
 *   memory have been configured but before caches have been enabled and
 *   before any devices have been initialized.
 *
 ****************************************************************************/

void imxrt_boardinitialize(void)
{
	/* Configure on-board LEDs if LED support has been selected. */

#ifdef CONFIG_ARCH_LEDS
	imxrt_autoled_initialize();
#endif
#ifdef CONFIG_IMXRT_SEMC_SDRAM
	imxrt_semc_sdram_init();
#endif
	imxrt_flash_init();
}

/****************************************************************************
 * Name: board_initialize
 *
 * Description:
 *   If CONFIG_BOARD_INITIALIZE is selected, then an additional
 *   initialization call will be performed in the boot-up sequence to a
 *   function called board_initialize().  board_initialize() will be
 *   called immediately after up_intitialize() is called and just before the
 *   initial application is started.  This additional initialization phase
 *   may be used, for example, to initialize board-specific device drivers.
 *
 ****************************************************************************/

#ifdef CONFIG_BOARD_INITIALIZE
void board_initialize(void)
{
	/* Perform board initialization */

	(void)imxrt_bringup();

	imxrt_gpio_initialize();

	imxrt_pwm_initialize();

	imxrt_spi_initialize();
}
#endif							/* CONFIG_BOARD_INITIALIZE */
