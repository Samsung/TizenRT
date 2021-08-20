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
 * os/board/imxrt1020-evk/src/imxrt_boot.c
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

#include <stdio.h>

#include <tinyara/board.h>
#include "imxrt_gpio.h"
//#include <tinyara/gpio.h>
#include <arch/board/board.h>

#include "imxrt1020-evk.h"
#include "imxrt_start.h"
#include "imxrt_flash.h"
#ifdef CONFIG_IMXRT_GPT
#include "imxrt_gpt.h"
#endif
#ifdef CONFIG_WATCHDOG
#include <tinyara/watchdog.h>
#include "imxrt_wdog.h"
#endif
#ifdef CONFIG_ANALOG
#include <tinyara/analog/adc.h>
#include "imxrt_adc.h"
#endif

#ifdef CONFIG_PRODCONFIG
#include <tinyara/prodconfig.h>
#include <sys/types.h>
#endif

#ifdef CONFIG_IMXRT_SEMC_SDRAM
#include "imxrt_semc_sdram.h"
#endif

#ifdef CONFIG_IMXRT_PIT
#include "imxrt_pit.h"
#define PIT_DEVPATH	"/dev/pit"
#endif

#ifdef CONFIG_IMXRT_QTMR
#include "imxrt_qtmr.h"
#endif

#include CONFIG_PROD_HEADER

/****************************************************************************
 * Name: imxrt_board_adc_initialize
 *
 * Description:
 *   Initialize the ADC. As the pins of each ADC channel are exported through
 *   configurable GPIO and it is board-specific, information on available
 *   ADC channels should be passed to imxrt_adc_initialize().
 *
 * Input Parameters:
 *   chanlist  - The list of channels
 *   cchannels - Number of channels
 *
 * Returned Value:
 *   Valid ADC device structure reference on succcess; a NULL on failure
 *
 ****************************************************************************/
#define ADC_ASSIGN(a, b) (a << 5 | b)

/****************************************************************************
 * Name: board_gpio_initialize
 *
 * Description:
 *   GPIO intialization for imxrt
 *
 ****************************************************************************/
#define PORT_ASSIGN(a, b) (a >> (GPIO_PORT_SHIFT - 5) | b >> GPIO_PIN_SHIFT)

#ifdef CONFIG_PRODCONFIG
int up_check_prodswd(void)
{
	return OK;
}
int up_check_proddownload(void)
{
	return OK;
}
#endif

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

#ifdef CONFIG_GPIO
static int IS_GPIO_PIN(PORT_PROPERTY pin)
{
	switch (pin.u32Alternative) {
	case PORT_TYPE_GPIO_OUTPUT_HIGH:
	case PORT_TYPE_GPIO_OUTPUT_LOW:
	case PORT_TYPE_GPIO_INPUT:
		return 1;
	default:
		return 0;
	}
}
#endif

#ifdef CONFIG_ANALOG
static int IS_ADC_PIN(PORT_PROPERTY pin)
{
	switch (pin.u32Alternative) {
	case PORT_TYPE_ADC:
		return 1;
	default:
		return 0;
	}
}
#endif

/****************************************************************************
 * Name: imxrt_spi_initialize
 *
 * Description:
 *   SPI intialization for imxrt
 *
 ****************************************************************************/
static void imxrt_spi_initialize(void)
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

void imxrt_iotbus_initialize(void)
{
	for (int port_idx = 0; port_idx < PORT_INIT_TABLE; port_idx++) {
		uint32_t pinset = (sPort_InitTable[port_idx].u32Port << GPIO_PIN_SHIFT) | sPort_InitTable[port_idx].u32Alternative;

#ifdef CONFIG_GPIO
		if (IS_GPIO_PIN(sPort_InitTable[port_idx])) {
			struct gpio_lowerhalf_s *lower = imxrt_gpio_lowerhalf(pinset);
			if (!lower) {
				lldbg("ERROR: adc_register[%x] port(%d) failed\n", pinset, port_idx);
				continue;
			}
			gpio_register(sPort_InitTable[port_idx].u32Port, lower);
		} else
#endif
#ifdef CONFIG_ANALOG
		if (IS_ADC_PIN(sPort_InitTable[port_idx])) {
			struct adc_dev_s *lower = imxrt_adc_lowerhalf(pinset);
			if (!lower) {
				lldbg("ERROR: adc_register[%x] port(%d) failed\n", pinset, port_idx);
				continue;
			}
			char adc_path[64];
			snprintf(adc_path, 64, "adc-%d", sPort_InitTable[port_idx].u32Port);
			adc_register(adc_path, lower);
		} else
#endif
		{
		}
	}

	imxrt_spi_initialize();
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

	imxrt_iotbus_initialize();

#ifdef CONFIG_WATCHDOG
	imxrt_wdog_initialize(CONFIG_WATCHDOG_DEVPATH, IMXRT_WDOG1);
#endif

#ifdef CONFIG_IMXRT_TIMER_INTERFACE
	{
		int timer_idx;
		char timer_path[CONFIG_PATH_MAX];

#ifdef CONFIG_IMXRT_GPT
		for (timer_idx = 0; timer_idx < IMXRT_GPT_CH_MAX; timer_idx++) {
			snprintf(timer_path, sizeof(timer_path), "/dev/timer%d", timer_idx);
			imxrt_timer_initialize(timer_path, timer_idx);
		}
#endif
#ifdef CONFIG_IMXRT_PIT
		imxrt_pit_initialize(PIT_DEVPATH);
#endif

#ifdef CONFIG_IMXRT_QTMR
		for (timer_idx = 0; timer_idx < kQTMR_MAX; timer_idx++) {
			snprintf(timer_path, sizeof(timer_path), "/dev/qtimer%d", timer_idx);
			imxrt_qtmr_initialize(timer_path, timer_idx);
		}

#endif
	}
#endif
}
#endif							/* CONFIG_BOARD_INITIALIZE */
