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
/****************************************************************************
 * include/tinyara/audio/alc5658.h
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
 *   Author:  Gregory Nutt <gnutt@nuttx.org>
 *
 * Reference:
 *   "ALC5658 Ultra Low Power CODEC for Portable Audio Applications, Pre-
 *    Production", September 2012, Rev 3.3, Wolfson Microelectronics
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

#ifndef __INCLUDE_TINYARA_AUDIO_ALC5658_H
#define __INCLUDE_TINYARA_AUDIO_ALC5658_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include <tinyara/irq.h>
#include <tinyara/i2c.h>

#ifdef CONFIG_AUDIO_ALC5658

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************
 *
 * CONFIG_AUDIO_ALC5658 - Enables ALC5658 support
 * CONFIG_ALC5658_INITVOLUME - The initial volume level in the range {0..1000}
 * CONFIG_ALC5658_INFLIGHT - Maximum number of buffers that the ALC5658 driver
 *   will send to the I2S driver before any have completed.
 * CONFIG_ALC5658_BUFFER_SIZE - Preferred buffer size
 * CONFIG_ALC5658_NUM_BUFFERS - Preferred number of buffers
 * CONFIG_ALC5658_REGDUMP - Enable logic to dump all ALC5658 registers to
 *   the SYSLOG device.
 */

/* Pre-requisites */

#ifndef CONFIG_AUDIO
#error CONFIG_AUDIO is required for audio subsystem support
#endif

#ifndef CONFIG_I2S
#error CONFIG_I2S is required by the ALC5658 driver
#endif

#ifndef CONFIG_I2C
#error CONFIG_I2C is required by the ALC5658 driver
#endif

#ifndef CONFIG_SCHED_WORKQUEUE
#error CONFIG_SCHED_WORKQUEUE is required by the ALC5658 driver
#endif

/* Default configuration values */

#ifndef CONFIG_ALC5658_INITVOLUME
#define CONFIG_ALC5658_INITVOLUME       250
#endif

#ifndef CONFIG_ALC5658_INFLIGHT
#define CONFIG_ALC5658_INFLIGHT          2
#endif

#if CONFIG_ALC5658_INFLIGHT > 255
#error CONFIG_ALC5658_INFLIGHT must fit in a uint8_t
#endif

#ifndef CONFIG_ALC5658_BUFFER_SIZE
#define CONFIG_ALC5658_BUFFER_SIZE       2048
#endif

#ifndef CONFIG_ALC5658_NUM_BUFFERS
#define CONFIG_ALC5658_NUM_BUFFERS       4
#endif

/* Helper macros ************************************************************/

#define ALC5658_ATTACH(s, isr, arg) ((s)->attach(s, isr, arg))
#define ALC5658_DETACH(s)         ((s)->attach(s, NULL, NULL))
#define ALC5658_ENABLE(s)         ((s)->enable(s, true))
#define ALC5658_DISABLE(s)        ((s)->enable(s, false))
#define ALC5658_RESTORE(s, e)      ((s)->enable(s, e))

/****************************************************************************
 * Public Types
 ****************************************************************************/
/* This is the type of the ALC5658 interrupt handler.  The lower level code
 * will intercept the interrupt and provide the upper level with the private
 * data that was provided when the interrupt was attached.
 */

struct alc5658_lower_s;			/* Forward reference.  Defined below */

typedef CODE int (*alc5658_handler_t)(FAR const struct alc5658_lower_s *lower, FAR void *arg);

/* A reference to a structure of this type must be passed to the ALC5658
 * driver.  This structure provides information about the configuration
 * of the ALC5658 and provides some board-specific hooks.
 *
 * Memory for this structure is provided by the caller.  It is not copied
 * by the driver and is presumed to persist while the driver is active.
 */

struct alc5658_lower_s {
	/* I2C characterization */
	struct i2c_config_s i2c_config;	/* I2C config should be bere */

	/* Clocking is provided via MCLK.  The ALC5658 driver will need to know
	 * the frequency of MCLK in order to generate the correct bitrates.
	 */
	uint32_t mclk;				/* Master clock frequency. In slave mode never used */

	/* IRQ/GPIO access callbacks.  These operations all hidden behind
	 * callbacks to isolate the ALC5658 driver from differences in GPIO
	 * interrupt handling by varying boards and MCUs.  If possible,
	 * interrupts should be configured on both rising and falling edges
	 * so that contact and loss-of-contact events can be detected.
	 *
	 * attach  - Attach or detach the ALC5658 interrupt handler to the GPIO
	 *           interrupt
	 * enable  - Enable or disable the GPIO interrupt.  Returns the
	 *           previous interrupt state.
	 */

	CODE int (*attach)(FAR const struct alc5658_lower_s *lower, alc5658_handler_t isr, FAR void *arg);
	CODE bool(*enable)(FAR const struct alc5658_lower_s *lower, bool enable);
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: alc5658_initialize
 *
 * Description:
 *   Initialize the ALC5658 device.
 *
 * Input Parameters:
 *   i2c     - An I2C driver instance
 *   i2s     - An I2S driver instance
 *   lower   - Persistent board configuration data
 *
 * Returned Value:
 *   A new lower half audio interface for the ALC5658 device is returned on
 *   success; NULL is returned on failure.
 *
 ****************************************************************************/

struct i2c_dev_s;				/* Forward reference. Defined in include/tinyara ... */
struct i2s_dev_s;				/* Forward reference. Defined in include/tinyara ... */
struct audio_lowerhalf_s;		/* Forward reference. Defined in tinyara/audio/audio.h */

FAR struct audio_lowerhalf_s *alc5658_initialize(struct i2c_dev_s *i2c, FAR struct i2s_dev_s *i2s, FAR struct alc5658_lower_s *lower);

/****************************************************************************
 * Name: alc5658_dump_registers
 *
 * Description:
 *   Dump the contents of all ALC5658 registers to the syslog device
 *
 * Input Parameters:
 *   dev - The device instance returned by alc5658_initialize
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

#ifdef CONFIG_ALC5658_REGDUMP
void alc5658_dump_registers(FAR struct audio_lowerhalf_s *dev, FAR const char *msg);
#else
/* This eliminates the need for any conditional compilation in the
 * including file.
 */

#define alc5658_dump_registers(d, m)
#endif

/****************************************************************************
 * Name: alc5658_clock_analysis
 *
 * Description:
 *   Analyze the settings in the clock chain and dump to syslog.
 *
 * Input Parameters:
 *   dev - The device instance returned by alc5658_initialize
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

#ifdef CONFIG_ALC5658_CLKDEBUG
void alc5658_clock_analysis(FAR struct audio_lowerhalf_s *dev, FAR const char *msg);
#else
/* This eliminates the need for any conditional compilation in the
 * including file.
 */

#define alc5658_clock_analysis(d, m)
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* CONFIG_AUDIO_ALC5658 */
#endif							/* __INCLUDE_TINYARA_AUDIO_ALC5658_H */
