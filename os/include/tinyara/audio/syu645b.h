/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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

#ifndef __INCLUDE_TINYARA_AUDIO_SYU645B_H
#define __INCLUDE_TINYARA_AUDIO_SYU645B_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdint.h>
#include <stdbool.h>

#include <tinyara/irq.h>
#include <tinyara/i2c.h>

#ifdef CONFIG_AUDIO_SYU645B

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************
 *
 * CONFIG_AUDIO_SYU645B - Enables SYU645B support
 * CONFIG_SYU645B_INITVOLUME - The initial volume level in the range {0..1000}
 * CONFIG_SYU645B_INFLIGHT - Maximum number of buffers that the SYU645B driver
 *   will send to the I2S driver before any have completed.
 * CONFIG_SYU645B_MSG_PRIO - Priority of messages sent to the SYU645B worker
 *   thread.
 * CONFIG_SYU645B_WORKER_STACKSIZE - Stack size to use when creating the
 *   SYU645B worker thread.
 */

/* Pre-requisites */

#ifndef CONFIG_AUDIO
#error CONFIG_AUDIO is required for audio subsystem support
#endif

#ifndef CONFIG_I2S
#error CONFIG_I2S is required by the SYU645B driver
#endif

#ifndef CONFIG_I2C
#error CONFIG_I2C is required by the SYU645B driver
#endif

#ifndef CONFIG_SCHED_WORKQUEUE
#error CONFIG_SCHED_WORKQUEUE is required by the SYU645B driver
#endif

/* Default configuration values */

#ifndef CONFIG_SYU645B_INITVOLUME
#define CONFIG_SYU645B_INITVOLUME       250
#endif

#ifndef CONFIG_SYU645B_INFLIGHT
#define CONFIG_SYU645B_INFLIGHT          2
#endif

#if CONFIG_SYU645B_INFLIGHT > 255
#error CONFIG_SYU645B_INFLIGHT must fit in a uint8_t
#endif

#ifndef CONFIG_SYU645B_MSG_PRIO
#define CONFIG_SYU645B_MSG_PRIO          1
#endif

#ifndef CONFIG_SYU645B_WORKER_STACKSIZE
#define CONFIG_SYU645B_WORKER_STACKSIZE  768
#endif

/* Helper macros ************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/
/* This is the type of the SYU645B interrupt handler.  The lower level code
 * will intercept the interrupt and provide the upper level with the private
 * data that was provided when the interrupt was attached.
 */

struct syu645b_lower_s;			/* Forward reference.  Defined below */

typedef CODE int (*syu645b_handler_t)(FAR const struct syu645b_lower_s *lower, FAR void *arg);

/* A reference to a structure of this type must be passed to the SYU645B
 * driver.  This structure provides information about the configuration
 * of the SYU645B and provides some board-specific hooks.
 *
 * Memory for this structure is provided by the caller.  It is not copied
 * by the driver and is presumed to persist while the driver is active.
 */

struct syu645b_lower_s {
	/* I2C characterization */
	struct i2c_config_s i2c_config;	/* I2C config should be bere */

	/* Clocking is provided via MCLK.  The SYU645B driver will need to know
	 * the frequency of MCLK in order to generate the correct bitrates.
	 */
	uint32_t mclk;				/* Master clock frequency. In slave mode never used */

	CODE void (*control_hw_reset)(bool active);
	CODE void (*control_powerdown)(bool enter_powerdown);
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
 * Name: syu645b_initialize
 *
 * Description:
 *   Initialize the SYU645B device.
 *
 * Input Parameters:
 *   i2c     - An I2C driver instance
 *   i2s     - An I2S driver instance
 *   lower   - Persistent board configuration data
 *
 * Returned Value:
 *   A new lower half audio interface for the SYU645B device is returned on
 *   success; NULL is returned on failure.
 *
 ****************************************************************************/

struct i2c_dev_s;				/* Forward reference. Defined in include/tinyara ... */
struct i2s_dev_s;				/* Forward reference. Defined in include/tinyara ... */
struct audio_lowerhalf_s;		/* Forward reference. Defined in tinyara/audio/audio.h */

FAR struct audio_lowerhalf_s *syu645b_initialize(struct i2c_dev_s *i2c, FAR struct i2s_dev_s *i2s, FAR struct syu645b_lower_s *lower);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* CONFIG_AUDIO_SYU645B */
#endif							/* __INCLUDE_TINYARA_AUDIO_SYU645B_H */
