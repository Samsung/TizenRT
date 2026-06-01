/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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
/*******************************************************************************
* Modifications:
* Copyright 2026 ABOV Semiconductor Co., Ltd.
*
* This file has been modified by ABOV Semiconductor.
*
* Modifications include:
*   - Implement ADAM110 lower half interface
*
* Modifications by ABOV Semiconductor are licensed under the BSD-3-Clause license.
* The original portions of this file remain licensed under the Apache License, Version 2.0.
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation and/or
* other materials provided with the distribution.
*
 * 3. Neither the name of the copyright holder nor the names of its contributors
* may be used to endorse or promote products derived from this software without
* specific prior written permission.
*
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

#ifndef __INCLUDE_TINYARA_AUDIO_ADAM110_H
#define __INCLUDE_TINYARA_AUDIO_ADAM110_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdint.h>
#include <stdbool.h>

#include <tinyara/spi/spi.h>

#ifdef CONFIG_AUDIO_ADAM110

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Default configuration values */

#ifndef CONFIG_ADAM110_MSG_PRIO
#define CONFIG_ADAM110_MSG_PRIO          1
#endif

#ifndef CONFIG_ADAM110_NUM_BUFFERS
#define CONFIG_ADAM110_NUM_BUFFERS       4
#endif

#ifndef CONFIG_ADAM110_WORKER_STACKSIZE
#define CONFIG_ADAM110_WORKER_STACKSIZE  1024
#endif

#define BT_MIC_SUPPORT 0

/* Helper macros ************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/
/* This is the type of the ADAM110 interrupt handler.  The lower level code
 * will intercept the interrupt and provide the upper level with the private
 * data that was provided when the interrupt was attached.
 */

struct adam110_lower_s;			/* Forward reference.  Defined below */

typedef CODE void (*adam110_handler_t)(FAR void *arg);

/* A reference to a structure of this type must be passed to the ADAM110
 * driver.  This structure provides information about the configuration
 * of the ADAM110 and provides some board-specific hooks.
 *
 * Memory for this structure is provided by the caller.  It is not copied
 * by the driver and is presumed to persist while the driver is active.
 */

struct adam110_lower_s {
	/* SPI related configs */
	struct spi_io_config spi_config;

	/* callback function: attach the mi48 interrupt handler to the GPIO interrupt */
	CODE int (*attach)(adam110_handler_t handler, FAR char *arg);

	/* eanble/disable adam110 interrupts hadling on Host Device */
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
 * Name: adam110_initialize
 *
 * Description:
 *   Initialize the ADAM110 device.
 *
 * Input Parameters:
 *   spi     - An SPI driver instance
 *   lower   - Persistent board configuration data
 *
 * Returned Value:
 *   A new lower half audio interface for the ADAM110 device is returned on
 *   success; NULL is returned on failure.
 *
 ****************************************************************************/

struct audio_lowerhalf_s;		/* Forward reference. Defined in tinyara/audio/audio.h */

FAR struct audio_lowerhalf_s *adam110_initialize(struct spi_dev_s *spi, FAR struct adam110_lower_s *lower);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* CONFIG_AUDIO_ADAM110 */
#endif							/* __INCLUDE_TINYARA_AUDIO_ADAM110_H */
