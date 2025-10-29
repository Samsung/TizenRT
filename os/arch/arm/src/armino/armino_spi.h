/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_ARMINO_SPI_H
#define __ARCH_ARM_SRC_ARMINO_SPI_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>

#include <stdbool.h>

#include <tinyara/spi/spi.h>

/********************************************************************************************
 * Pre-processor Definitions
 ********************************************************************************************/

/************************************************************************************
 * Public Functions
 ************************************************************************************/

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

struct spi_dev_s; /* Forward reference */

/************************************************************************************
 * Name: armino_spibus_initialize
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

FAR struct spi_dev_s *armino_spibus_initialize(int bus);

/************************************************************************************
 * Name:  armino_spi1/2/...select and armino_spi1/2/...status
 *
 * Description:
 *   The external functions, armino_spi1/2/...select, armino_spi1/2/...status, and
 *   armino_spi1/2/...cmddata must be provided by board-specific logic.  These are
 *   implementations of the select, status, and cmddata methods of the SPI interface
 *   defined by struct spi_ops_s (see include/nuttx/spi/spi.h). All other methods
 *   (including armino_spibus_initialize()) are provided by common ARMINO logic.  To use this
 *   common SPI logic on your board:
 *
 *   1. Provide logic in armino_boardinitialize() to configure SPI chip select
 *      pins.
 *   2. Provide armino_spi1/2/...select() and armino_spi1/2/...status() functions in your
 *      board-specific logic.  These functions will perform chip selection and
 *      status operations using GPIOs in the way your board is configured.
 *   3. If CONFIG_SPI_CMDDATA is defined in your NuttX configuration file, then
 *      provide armino_spi1/2/...cmddata() functions in your board-specific logic.
 *      These functions will perform cmd/data selection operations using GPIOs in the
 *      way your board is configured.
 *   4. Add a calls to armino_spibus_initialize() in your low level application
 *      initialization logic
 *   5. The handle returned by armino_spibus_initialize() may then be used to bind the
 *      SPI driver to higher level logic (e.g., calling
 *      mmcsd_spislotinitialize(), for example, will bind the SPI driver to
 *      the SPI MMC/SD driver).
 *
 ************************************************************************************/

void armino_spi0select(FAR struct spi_dev_s *dev, enum spi_dev_e devid, bool selected);
uint8_t armino_spi0status(FAR struct spi_dev_s *dev, enum spi_dev_e devid);
int armino_spi0cmddata(FAR struct spi_dev_s *dev, enum spi_dev_e devid, bool cmd);

void armino_spi1select(FAR struct spi_dev_s *dev, enum spi_dev_e devid, bool selected);
uint8_t armino_spi1status(FAR struct spi_dev_s *dev, enum spi_dev_e devid);
int armino_spi1cmddata(FAR struct spi_dev_s *dev, enum spi_dev_e devid, bool cmd);

/************************************************************************************
 * Name: armino_spi1/2/...register
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
int armino_spi0register(FAR struct spi_dev_s *dev, spi_mediachange_t callback,
			FAR void *arg);
int armino_spi1register(FAR struct spi_dev_s *dev, spi_mediachange_t callback,
			FAR void *arg);

#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_ARMINO_SPI_H */

