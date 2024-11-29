/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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

#ifndef __INCLUDE_TINYARA_AUDIO_NDP120_H
#define __INCLUDE_TINYARA_AUDIO_NDP120_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdint.h>
#include <stdbool.h>

#include <tinyara/spi/spi.h>

#ifdef CONFIG_AUDIO_NDP120

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Default configuration values */

#ifndef CONFIG_NDP120_MSG_PRIO
#define CONFIG_NDP120_MSG_PRIO          1
#endif

#ifndef CONFIG_NDP120_NUM_BUFFERS
#define CONFIG_NDP120_NUM_BUFFERS       4
#endif

#ifndef CONFIG_NDP120_WORKER_STACKSIZE
#define CONFIG_NDP120_WORKER_STACKSIZE  1024
#endif

/* Helper macros ************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/
/* This is the type of the NDP120 interrupt handler.  The lower level code
 * will intercept the interrupt and provide the upper level with the private
 * data that was provided when the interrupt was attached.
 */

struct ndp120_lower_s;			/* Forward reference.  Defined below */

typedef CODE void (*ndp120_handler_t)(FAR void *arg);

/* A reference to a structure of this type must be passed to the NDP120
 * driver.  This structure provides information about the configuration
 * of the NDP120 and provides some board-specific hooks.
 *
 * Memory for this structure is provided by the caller.  It is not copied
 * by the driver and is presumed to persist while the driver is active.
 */

struct ndp120_lower_s {
	/* SPI related configs */
	struct spi_io_config spi_config;

	/* callback function: attach the mi48 interrupt handler to the GPIO interrupt */
	CODE int (*attach)(ndp120_handler_t handler, FAR char *arg);

	/* eanble/disable ndp120 interrupts hadling on Host Device */
	CODE void (*irq_enable)(bool enable);

	/* Enable/Disable Internal MIC on Host Device */
	CODE void (*set_dmic)(bool enable);
#ifdef CONFIG_PM
	/* PM related h/w operations */
	CODE void (*set_pm_state)(bool sleep);
#endif
	CODE void (*reset)(void);
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
 * Name: ndp120_initialize
 *
 * Description:
 *   Initialize the NDP120 device.
 *
 * Input Parameters:
 *   spi     - An SPI driver instance
 *   lower   - Persistent board configuration data
 *
 * Returned Value:
 *   A new lower half audio interface for the NDP120 device is returned on
 *   success; NULL is returned on failure.
 *
 ****************************************************************************/

struct audio_lowerhalf_s;		/* Forward reference. Defined in tinyara/audio/audio.h */

FAR struct audio_lowerhalf_s *ndp120_initialize(struct spi_dev_s *spi, FAR struct ndp120_lower_s *lower);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* CONFIG_AUDIO_NDP120 */
#endif							/* __INCLUDE_TINYARA_AUDIO_NDP120_H */
