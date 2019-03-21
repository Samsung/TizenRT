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
/************************************************************************************
 * os/arch/arm/src/imxrt/imxrt_lpspi.h
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
 *   Authors: Ivan Ucherdzhiev <ivanucherdjiev@gmail.com>
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
 ************************************************************************************/

#ifndef __ARCH_ARM_SRC_IMXRT_IMXRT_LPSPI_H
#define __ARCH_ARM_SRC_IMXRT_IMXRT_LPSPI_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>

#include <stdbool.h>

#include <tinyara/spi/spi.h>

#include "chip.h"
#include "chip/imxrt_lpspi.h"

/********************************************************************************************
 * Pre-processor Definitions
 ********************************************************************************************/

#define IOMUX_LPSPI1_CS (IOMUX_SLEW_FAST | IOMUX_DRIVE_50OHM | \
                         IOMUX_SPEED_MEDIUM | IOMUX_PULL_UP_100K | \
                         _IOMUX_PULL_ENABLE)
#define GPIO_LPSPI1_CS  (GPIO_OUTPUT | GPIO_OUTPUT_ONE | \
                         GPIO_PORT3 | GPIO_PIN13 | IOMUX_LPSPI1_CS)

#define LPSPI_CFGR1_OUTCFG_RETAIN  (0 << 26) /* Output data retains last value when chip select is negated */
#define LPSPI_CFGR1_OUTCFG_TRISTATE (1 << 26) /* Output data is tristated when chip select is negated */

#define LPSPI_CFGR1_PINCFG_SIN_SOUT  (0 << LPSPI_CFGR1_PINCFG_SHIFT)  /* SIN is used for input data and SOUT is used for output data */
#define LPSPI_CFGR1_PINCFG_SIN_SIN   (1 << LPSPI_CFGR1_PINCFG_SHIFT)  /* SIN is used for both input and output data */
#define LPSPI_CFGR1_PINCFG_SOUT_SOUT (2 << LPSPI_CFGR1_PINCFG_SHIFT)  /* SOUT is used for both input and output data */
#define LPSPI_CFGR1_PINCFG_SOUT_SIN  (3 << LPSPI_CFGR1_PINCFG_SHIFT)  /* SOUT is used for input data and SIN is used for output data */

/************************************************************************************
 * Public Functions
 ************************************************************************************/

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

struct spi_dev_s; /* Forward reference */

/************************************************************************************
 * Name: imxrt_lpspibus_initialize
 *
 * Description:
 *   Initialize the selected SPI bus
 *
 * Input Parameters:
 *   bus number (for hardware that has mutiple SPI interfaces)
 *
 * Returned Value:
 *   Valid SPI device structure reference on succcess; a NULL on failure
 *
 ************************************************************************************/

FAR struct spi_dev_s *imxrt_lpspibus_initialize(int bus);

/************************************************************************************
 * Name:  imxrt_lpspi1/2/...select and imxrt_lpspi1/2/...status
 *
 * Description:
 *   The external functions, imxrt_lpspi1/2/...select, imxrt_lpspi1/2/...status, and
 *   imxrt_lpspi1/2/...cmddata must be provided by board-specific logic.  These are
 *   implementations of the select, status, and cmddata methods of the SPI interface
 *   defined by struct spi_ops_s (see include/nuttx/spi/spi.h). All other methods
 *   (including imxrt_lpspibus_initialize()) are provided by common IMXRT logic.  To use this
 *   common SPI logic on your board:
 *
 *   1. Provide logic in imxrt_boardinitialize() to configure SPI chip select
 *      pins.
 *   2. Provide imxrt_lpspi1/2/...select() and imxrt_lpspi1/2/...status() functions in your
 *      board-specific logic.  These functions will perform chip selection and
 *      status operations using GPIOs in the way your board is configured.
 *   3. If CONFIG_SPI_CMDDATA is defined in your NuttX configuration file, then
 *      provide imxrt_lpspi1/2/...cmddata() functions in your board-specific logic.
 *      These functions will perform cmd/data selection operations using GPIOs in the
 *      way your board is configured.
 *   4. Add a calls to imxrt_lpspibus_initialize() in your low level application
 *      initialization logic
 *   5. The handle returned by imxrt_lpspibus_initialize() may then be used to bind the
 *      SPI driver to higher level logic (e.g., calling
 *      mmcsd_spislotinitialize(), for example, will bind the SPI driver to
 *      the SPI MMC/SD driver).
 *
 ************************************************************************************/

#ifdef CONFIG_IMXRT_LPSPI1
void imxrt_lpspi1select(FAR struct spi_dev_s *dev, enum spi_dev_e devid, bool selected);
uint8_t imxrt_lpspi1status(FAR struct spi_dev_s *dev, enum spi_dev_e devid);
int imxrt_lpspi1cmddata(FAR struct spi_dev_s *dev, uint32_t devid, bool cmd);
#endif

#ifdef CONFIG_IMXRT_LPSPI2
void imxrt_lpspi2select(FAR struct spi_dev_s *dev, enum spi_dev_e devid, bool selected);
uint8_t imxrt_lpspi2status(FAR struct spi_dev_s *dev, enum spi_dev_e devid);
int imxrt_lpspi2cmddata(FAR struct spi_dev_s *dev, uint32_t devid, bool cmd);
#endif

#ifdef CONFIG_IMXRT_LPSPI3
void imxrt_lpspi3select(FAR struct spi_dev_s *dev, enum spi_dev_e devid, bool selected);
uint8_t imxrt_lpspi3status(FAR struct spi_dev_s *dev, enum spi_dev_e devid);
int imxrt_lpspi3cmddata(FAR struct spi_dev_s *dev, uint32_t devid, bool cmd);
#endif

#ifdef CONFIG_IMXRT_LPSPI4
void imxrt_lpspi4select(FAR struct spi_dev_s *dev, enum spi_dev_e devid, bool selected);
uint8_t imxrt_lpspi4status(FAR struct spi_dev_s *dev, enum spi_dev_e devid);
int imxrt_lpspi4cmddata(FAR struct spi_dev_s *dev, uint32_t devid, bool cmd);
#endif

/************************************************************************************
 * Name: imxrt_lpspi1/2/...register
 *
 * Description:
 *   If the board supports a card detect callback to inform the SPI-based MMC/SD
 *   driver when an SD card is inserted or removed, then CONFIG_SPI_CALLBACK should
 *   be defined and the following function(s) must be implemented.  These functions
 *   implements the registercallback method of the SPI interface (see
 *   include/nuttx/spi/spi.h for details)
 *
 * Input Parameters:
 *   dev -      Device-specific state data
 *   callback - The function to call on the media change
 *   arg -      A caller provided value to return with the callback
 *
 * Returned Value:
 *   0 on success; negated errno on failure.
 *
 ****************************************************************************/

#ifdef CONFIG_SPI_CALLBACK
#ifdef CONFIG_IMXRT_LPSPI1
int imxrt_lpspi1register(FAR struct spi_dev_s *dev, spi_mediachange_t callback,
                       FAR void *arg);
#endif

#ifdef CONFIG_IMXRT_LPSPI2
int imxrt_lpspi2register(FAR struct spi_dev_s *dev, spi_mediachange_t callback,
                       FAR void *arg);
#endif

#ifdef CONFIG_IMXRT_LPSPI3
int imxrt_lpspi3register(FAR struct spi_dev_s *dev, spi_mediachange_t callback,
                       FAR void *arg);
#endif

#ifdef CONFIG_IMXRT_LPSPI4
int imxrt_lpspi4register(FAR struct spi_dev_s *dev, spi_mediachange_t callback,
                       FAR void *arg);
#endif
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_IMXRT_IMXRT_LPSPI_H */

