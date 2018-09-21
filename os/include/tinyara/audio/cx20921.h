/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
 * include/tinyara/audio/cx20921.h
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
 *   Author:  Gregory Nutt <gnutt@nuttx.org>
 *
 * Reference:
 *   "CX20921 Ultra Low Power CODEC for Portable Audio Applications, Pre-
 *    Production"
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

#ifndef __INCLUDE_TINYARA_AUDIO_CX20921_H
#define __INCLUDE_TINYARA_AUDIO_CX20921_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdint.h>
#include <stdbool.h>

#include <tinyara/irq.h>
#include <tinyara/i2c.h>

#ifdef CONFIG_AUDIO_CX20921

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************
 *
 * CONFIG_AUDIO_CX20921 - Enables CX20921 support
 * CONFIG_CX20921_BUFFER_SIZE - Preferred buffer size
 * CONFIG_CX20921_NUM_BUFFERS - Preferred number of buffers
 */

/* Pre-requisites */

#ifndef CONFIG_AUDIO
#error CONFIG_AUDIO is required for audio subsystem support
#endif

#ifndef CONFIG_I2S
#error CONFIG_I2S is required by the CX20921 driver
#endif

#ifndef CONFIG_SCHED_WORKQUEUE
#error CONFIG_SCHED_WORKQUEUE is required by the CX20921 driver
#endif

/* Default configuration values */
#ifndef CONFIG_CX20921_BUFFER_SIZE
#define CONFIG_CX20921_BUFFER_SIZE       4096
#endif

#ifndef CONFIG_CX20921_NUM_BUFFERS
#define CONFIG_CX20921_NUM_BUFFERS       4
#endif

#ifndef CONFIG_CX20921_WORKER_STACKSIZE
#define CONFIG_CX20921_WORKER_STACKSIZE  768
#endif

/* Helper macros ************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/
/* This is the type of the CX20921 interrupt handler.  The lower level code
 * will intercept the interrupt and provide the upper level with the private
 * data that was provided when the interrupt was attached.
 */

struct cx20921_lower_s;                 /* Forward reference.  Defined below */


typedef CODE int (*cx20921_handler_t)(FAR const struct cx20921_lower_s *lower, FAR void *arg);

/* A reference to a structure of this type must be passed to the CX20921
 * driver.  This structure provides information about the configuration
 * of the CX20921 and provides some board-specific hooks.
 *
 * Memory for this structure is provided by the caller.  It is not copied
 * by the driver and is presumed to persist while the driver is active.
 */

struct cx20921_lower_s {
	/* I2C characterization */
	struct i2c_config_s i2c_config;	/* I2C config should be bere */

	/* Clocking is provided via MCLK.  The CX20921 driver will need to know
	 * the frequency of MCLK in order to generate the correct bitrates.
	 */
	uint32_t mclk;				/* Master clock frequency. In slave mode never used */

	CODE void (*control_hw_reset)(bool active);
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
 * Name: cx20921_initialize
 *
 * Description:
 *   Initialize the CX20921 device.
 *
 * Input Parameters:
 *   i2c     - An I2C driver instance
 *   i2s     - An I2S driver instance
 *   lower   - Persistent board configuration data
 *
 * Returned Value:
 *   A new lower half audio interface for the CX20921 device is returned on
 *   success; NULL is returned on failure.
 *
 ****************************************************************************/

struct i2c_dev_s;               /* Forward reference. Defined in include/tinyara ... */
struct i2s_dev_s;               /* Forward reference. Defined in include/tinyara ... */
struct audio_lowerhalf_s;       /* Forward reference. Defined in tinyara/audio/audio.h */

FAR struct audio_lowerhalf_s *cx20921_initialize(struct i2c_dev_s *i2c, FAR struct i2s_dev_s *i2s, FAR struct cx20921_lower_s *lower);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* CONFIG_AUDIO_CX20921 */
#endif							/* __INCLUDE_TINYARA_AUDIO_CX20921_H */
