/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <semaphore.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/semaphore.h>
#include <tinyara/spi/spi.h>

#include <arch/board/board.h>

#include "up_internal.h"
#include "up_arch.h"

#include "chip.h"

#include "amebalite_spi.h"
#include "PinNames.h"
#include "spi_api.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* Configuration ********************************************************************/

/* SPI interrupts */

#ifdef CONFIG_AMEBALITE_SPI_INTERRUPTS
#error "Interrupt driven SPI not yet supported"
#endif

#if defined(CONFIG_AMEBALITE_SPI_DMA)
#error "DMA mode is not yet supported"
#endif

/* Can't have both interrupt driven SPI and SPI DMA */

#if defined(CONFIG_AMEBALITE_SPI_INTERRUPTS) && defined(CONFIG_AMEBALITE_SPI_DMA)
#error "Cannot enable both interrupt mode and DMA mode for SPI"
#endif

#undef spierr
#undef spiinfo
#if defined(DEBUG_IMX_I2C)
#define spiinfo(format, ...)   printf(format, ##__VA_ARGS__)
#define spierr(format, ...)    printf(format, ##__VA_ARGS__)
#else
#define spiinfo(format, ...)
//#define spierr(format, ...)
#endif
#define spierr(format, ...)   printf(format, ##__VA_ARGS__)

#define AMEBALITE_SPI_MASTER	0
#define AMEBALITE_SPI_SLAVE	1

/************************************************************************************
 * Private Types
 ************************************************************************************/

struct amebalite_spidev_s {
	struct spi_dev_s spidev;    /* Externally visible part of the SPI interface */
//	uint32_t spibase;           /* SPIn base address */
//#ifdef CONFIG_AMEBALITE_SPI_INTERRUPTS
//	uint8_t spiirq;             /* SPI IRQ number */
//#endif
	sem_t exclsem;              /* Held while chip is selected for mutual exclusion */
	uint32_t frequency;         /* Requested clock frequency */
	uint32_t actual;            /* Actual clock frequency */

	spi_t spi_object;
	uint32_t spi_idx;
	PinName spi_mosi;
	PinName spi_miso;
	PinName spi_sclk;
	PinName spi_cs;
	int8_t nbits;               /* Width of word in bits */
	uint8_t mode;               /* Mode 0,1,2,3 */
	int role;
};

enum amebalite_delay_e {
	SPI_PCS_TO_SCK = 1,       /* PCS-to-SCK delay. */
	SPI_LAST_SCK_TO_PCS,      /* Last SCK edge to PCS delay. */
	SPI_BETWEEN_TRANSFER      /* Delay between transfers. */
};

/************************************************************************************
 * Private Function Prototypes
 ************************************************************************************/

/* Helpers */

static inline uint32_t amebalite_spi_getreg32(FAR struct amebalite_spidev_s *priv,
					uint8_t offset);
static inline void amebalite_spi_putreg32(FAR struct amebalite_spidev_s *priv,
					uint8_t offset, uint32_t value);
static inline uint16_t amebalite_spi_readword(FAR struct amebalite_spidev_s *priv);
static inline void amebalite_spi_writeword(FAR struct amebalite_spidev_s *priv,
					uint16_t byte);
static inline bool amebalite_spi_9to16bitmode(FAR struct amebalite_spidev_s *priv);
static inline void amebalite_spi_master_set_delays(FAR struct amebalite_spidev_s
					*priv, uint32_t delay_ns,
					enum amebalite_delay_e type);
static inline void amebalite_spi_master_set_delay_scaler(FAR struct
					amebalite_spidev_s *priv,
					uint32_t scaler,
					enum amebalite_delay_e type);

/* SPI methods */

static int amebalite_spi_lock(FAR struct spi_dev_s *dev, bool lock);
static uint32_t amebalite_spi_setfrequency(FAR struct spi_dev_s *dev,
					uint32_t frequency);
static void amebalite_spi_setmode(FAR struct spi_dev_s *dev,
				enum spi_mode_e mode);
static void amebalite_spi_setbits(FAR struct spi_dev_s *dev, int nbits);
#ifdef CONFIG_SPI_HWFEATURES
static int amebalite_spi_hwfeatures(FAR struct spi_dev_s *dev,
				amebalite_spi_hwfeatures_t features);
#endif
static uint16_t amebalite_spi_send(FAR struct spi_dev_s *dev, uint16_t wd);
static void amebalite_spi_exchange(FAR struct spi_dev_s *dev,
				FAR const void *txbuffer, FAR void *rxbuffer,
				size_t nwords);
#ifndef CONFIG_SPI_EXCHANGE
static void amebalite_spi_sndblock(FAR struct spi_dev_s *dev,
				FAR const void *txbuffer, size_t nwords);
static void amebalite_spi_recvblock(FAR struct spi_dev_s *dev, FAR void *rxbuffer,
				size_t nwords);
#endif

/* Initialization */

static void amebalite_spi_bus_initialize(FAR struct amebalite_spidev_s *priv);

/************************************************************************************
 * Private Data
 ************************************************************************************/

static const struct spi_ops_s g_spi0ops = {
	.lock         = amebalite_spi_lock,
	.select       = amebalite_spi0select,
	.setfrequency = amebalite_spi_setfrequency,
	.setmode      = amebalite_spi_setmode,
	.setbits      = amebalite_spi_setbits,
#ifdef CONFIG_SPI_HWFEATURES
	.hwfeatures   = amebalite_spi_hwfeatures,
#endif
	.status       = amebalite_spi0status,
#ifdef CONFIG_SPI_CMDDATA
	.cmddata      = amebalite_spi0cmddata,
#endif
	.send         = amebalite_spi_send,
#ifdef CONFIG_SPI_EXCHANGE
	.exchange     = amebalite_spi_exchange,
#else
	.sndblock     = amebalite_spi_sndblock,
	.recvblock    = amebalite_spi_recvblock,
#endif
#ifdef CONFIG_SPI_CALLBACK
	.registercallback = amebalite_spi0register,  /* Provided externally */
#else
	.registercallback = 0,                     /* Not implemented */
#endif
};

static struct amebalite_spidev_s g_spi0dev = {
	.spidev       = {
		&g_spi0ops
	},
	/*
	.spibase      = AMEBALITE_SPI1_BASE,
#ifdef CONFIG_AMEBALITE_SPI_INTERRUPTS
	.spiirq       = AMEBALITE_IRQ_SPI1,
#endif
#ifdef CONFIG_AMEBALITE_SPI_DMA
	.rxch         = DMAMAP_SPI1_RX,
	.txch         = DMAMAP_SPI1_TX,
#endif
*/

	.spi_object = {0},

	.spi_idx = MBED_SPI0,
	.spi_mosi = PA_29,
	.spi_miso = PA_30,
	.spi_sclk = PA_28,
	.spi_cs = PA_31,
	.nbits = 8,
	.mode = SPIDEV_MODE0,
	.role = AMEBALITE_SPI_SLAVE,
};

static const struct spi_ops_s g_spi1ops = {
	.lock         = amebalite_spi_lock,
	.select       = amebalite_spi1select,
	.setfrequency = amebalite_spi_setfrequency,
	.setmode      = amebalite_spi_setmode,
	.setbits      = amebalite_spi_setbits,
#ifdef CONFIG_SPI_HWFEATURES
	.hwfeatures   = amebalite_spi_hwfeatures,
#endif
	.status       = amebalite_spi1status,
#ifdef CONFIG_SPI_CMDDATA
	.cmddata      = amebalite_spi1cmddata,
#endif
	.send         = amebalite_spi_send,
#ifdef CONFIG_SPI_EXCHANGE
	.exchange     = amebalite_spi_exchange,
#else
	.sndblock     = amebalite_spi_sndblock,
	.recvblock    = amebalite_spi_recvblock,
#endif
#ifdef CONFIG_SPI_CALLBACK
	.registercallback = amebalite_spi1register,  /* Provided externally */
#else
	.registercallback = 0,                     /* Not implemented */
#endif
};

static struct amebalite_spidev_s g_spi1dev = {
	.spidev       = {
		&g_spi1ops
	},
	/*
	.spibase      = AMEBALITE_SPI1_BASE,
#ifdef CONFIG_AMEBALITE_SPI_INTERRUPTS
	.spiirq       = AMEBALITE_IRQ_SPI1,
#endif
#ifdef CONFIG_AMEBALITE_SPI_DMA
	.rxch         = DMAMAP_SPI1_RX,
	.txch         = DMAMAP_SPI1_TX,
#endif
*/

	.spi_object = {0},

	.spi_idx = MBED_SPI1,
	.spi_mosi = PB_2,
	.spi_miso = PB_3,
	.spi_sclk = PB_4,
	.spi_cs = PB_5,
	.nbits = 8,
	.mode = SPIDEV_MODE0,
	.role = AMEBALITE_SPI_MASTER
};
/************************************************************************************
 * Private Functions
 ************************************************************************************/

/************************************************************************************
 * Name: amebalite_spi_getreg8
 *
 * Description:
 *   Get the contents of the SPI register at offset
 *
 * Input Parameters:
 *   priv   - private SPI device structure
 *   offset - offset to the register of interest
 *
 * Returned Value:
 *   The contents of the 8-bit register
 *
 ************************************************************************************/

static inline uint8_t amebalite_spi_getreg8(FAR struct amebalite_spidev_s *priv,
					uint8_t offset)
{
	return 0;
}

/************************************************************************************
 * Name: amebalite_spi_putreg8
 *
 * Description:
 *   Write a 8-bit value to the SPI register at offset
 *
 * Input Parameters:
 *   priv   - private SPI device structure
 *   offset - offset to the register of interest
 *   value  - the 8-bit value to be written
 *
 ************************************************************************************/

static inline void amebalite_spi_putreg8(FAR struct amebalite_spidev_s *priv,
				uint8_t offset, uint8_t value)
{
	return;
}

/************************************************************************************
 * Name: amebalite_spi_getreg
 *
 * Description:
 *   Get the contents of the SPI register at offset
 *
 * Input Parameters:
 *   priv   - private SPI device structure
 *   offset - offset to the register of interest
 *
 * Returned Value:
 *   The contents of the 32-bit register
 *
 ************************************************************************************/

static inline uint32_t amebalite_spi_getreg32(FAR struct amebalite_spidev_s *priv,
					uint8_t offset)
{
	return 0;
}

/************************************************************************************
 * Name: amebalite_spi_putreg
 *
 * Description:
 *   Write a 16-bit value to the SPI register at offset
 *
 * Input Parameters:
 *   priv   - private SPI device structure
 *   offset - offset to the register of interest
 *   value  - the 32-bit value to be written
 *
 * Returned Value:
 *   The contents of the 32-bit register
 *
 ************************************************************************************/

static inline void amebalite_spi_putreg32(FAR struct amebalite_spidev_s *priv,
					uint8_t offset, uint32_t value)
{
	return;
}

/************************************************************************************
 * Name: amebalite_spi_readword
 *
 * Description:
 *   Read one word from SPI
 *
 * Input Parameters:
 *   priv - Device-specific state data
 *
 * Returned Value:
 *   word as read
 *
 ************************************************************************************/

static inline uint16_t amebalite_spi_readword(FAR struct amebalite_spidev_s *priv)
{
	return 0;
}

/************************************************************************************
 * Name: amebalite_spi_writeword
 *
 * Description:
 *   Write one word to SPI
 *
 * Input Parameters:
 *   priv - Device-specific state data
 *   word - word to send
 *
 * Returned Value:
 *   None
 *
 ************************************************************************************/

static inline void amebalite_spi_writeword(FAR struct amebalite_spidev_s *priv, uint16_t word)
{
	return;
}

/************************************************************************************
 * Name: amebalite_spi_readbyte
 *
 * Description:
 *   Read one byte from SPI
 *
 * Input Parameters:
 *   priv - Device-specific state data
 *
 * Returned Value:
 *   Byte as read
 *
 ************************************************************************************/

static inline uint8_t amebalite_spi_readbyte(FAR struct amebalite_spidev_s *priv)
{
	return 0;
}

/************************************************************************************
 * Name: amebalite_spi_writebyte
 *
 * Description:
 *   Write one 8-bit frame to the SPI FIFO
 *
 * Input Parameters:
 *   priv - Device-specific state data
 *   byte - Byte to send
 *
 * Returned Value:
 *   None
 *
 ************************************************************************************/

static inline void amebalite_spi_writebyte(FAR struct amebalite_spidev_s *priv, uint8_t byte)
{
	return;
}

/************************************************************************************
 * Name: amebalite_spi_9to16bitmode
 *
 * Description:
 *   Check if the SPI is operating in more then 8 bit mode
 *
 * Input Parameters:
 *   priv     - Device-specific state data
 *
 * Returned Value:
 *   true: >8 bit mode-bit mode, false: <= 8-bit mode
 *
 ************************************************************************************/

static inline bool amebalite_spi_9to16bitmode(FAR struct amebalite_spidev_s *priv)
{
	if (priv->nbits < 9) return false;
	else return true;
}

/************************************************************************************
 * Name: amebalite_spi_modifyreg
 *
 * Description:
 *   Clear and set bits in register
 *
 * Input Parameters:
 *   priv    - Device-specific state data
 *   offset  - Register offset
 *   clrbits - The bits to clear
 *   setbits - The bits to set
 *
 * Returned Value:
 *   None
 *
 ************************************************************************************/

static void amebalite_spi_modifyreg32(FAR struct amebalite_spidev_s *priv,
				uint8_t offset, uint32_t clrbits,
				uint32_t setbits)
{
}

/************************************************************************************
 * Name: amebalite_spi_master_set_delays
 *
 * Description:
 *   SET SPI Delay times
 *
 * Input Parameters:
 *   priv   - Device-specific state data
 *   scaler - scaler value
 *   type   - delay time type
 *
 * Returned Value:
 *   None
 *
 ************************************************************************************/

static inline void amebalite_spi_master_set_delay_scaler(FAR struct amebalite_spidev_s *priv,
						uint32_t scaler,
						enum amebalite_delay_e type)
{
	/*
	switch (type) {
	case SPI_PCS_TO_SCK:
		amebalite_spi_modifyreg32(priv, AMEBALITE_SPI_CCR_OFFSET,
					SPI_CCR_PCSSCK_MASK, 0);
		amebalite_spi_modifyreg32(priv, AMEBALITE_SPI_CCR_OFFSET, 0,
					SPI_CCR_PCSSCK(scaler));
	break;

	case SPI_LAST_SCK_TO_PCS:
		amebalite_spi_modifyreg32(priv, AMEBALITE_SPI_CCR_OFFSET,
					SPI_CCR_SCKPCS_MASK, 0);
		amebalite_spi_modifyreg32(priv, AMEBALITE_SPI_CCR_OFFSET, 0,
					SPI_CCR_SCKPCS(scaler));
	break;

	case SPI_BETWEEN_TRANSFER:
		amebalite_spi_modifyreg32(priv, AMEBALITE_SPI_CCR_OFFSET, SPI_CCR_DBT_MASK,
					0);
		amebalite_spi_modifyreg32(priv, AMEBALITE_SPI_CCR_OFFSET, 0,
					SPI_CCR_DBT(scaler));
	break;
	}
	*/
}

/************************************************************************************
 * Name: amebalite_spi_master_set_delays
 *
 * Description:
 *   SET SPI Delay times
 *
 * Input Parameters:
 *   priv     - Device-specific state data
 *   delay_ns - delay time in nano seconds
 *   type     - delay time type
 *
 * Returned Value:
 *   None
 *
 ************************************************************************************/
static inline void amebalite_spi_master_set_delays(FAR struct amebalite_spidev_s *priv,
						uint32_t delay_ns,
						enum amebalite_delay_e type)
{
}

/************************************************************************************
 * Name: amebalite_spi_lock
 *
 * Description:
 *   On SPI busses where there are multiple devices, it will be necessary to
 *   lock SPI to have exclusive access to the busses for a sequence of
 *   transfers.  The bus should be locked before the chip is selected. After
 *   locking the SPI bus, the caller should then also call the setfrequency,
 *   setbits, and setmode methods to make sure that the SPI is properly
 *   configured for the device.  If the SPI buss is being shared, then it
 *   may have been left in an incompatible state.
 *
 * Input Parameters:
 *   dev  - Device-specific state data
 *   lock - true: Lock spi bus, false: unlock SPI bus
 *
 * Returned Value:
 *   None
 *
 ************************************************************************************/

static int amebalite_spi_lock(FAR struct spi_dev_s *dev, bool lock)
{
	FAR struct amebalite_spidev_s *priv = (FAR struct amebalite_spidev_s *)dev;
	int ret;

	if (lock) {
		/* Take the semaphore (perhaps waiting) */

		do {
			ret = sem_wait(&priv->exclsem);

			/* The only case that an error should occur here is if the wait was
			 * awakened by a signal.
			 */
			DEBUGASSERT(errno != EINTR);
		} while (ret < OK);
	} else {
		(void)sem_post(&priv->exclsem);
		ret = OK;
	}

	return ret;
}

/************************************************************************************
 * Name: amebalite_spi0select
 *
 * Description:
 *   Enable/disable the SPI slave select. The implementation of this method
 *   must include handshaking:  If a device is selected, it must hold off
 *   all other attempts to select the device until the device is deselecte.
 *
 * Input Parameters:
 *   dev -       Device-specific state data
 *   devid -    Device Id
 *   selected - whether it is selected or not
 *
 * Returned Value: None
 *
 *
 ************************************************************************************/
void amebalite_spi0select(FAR struct spi_dev_s *dev, enum spi_dev_e devid, bool selected)
{
	return;
}

/************************************************************************************
 * Name: amebalite_spi0status
 *
 * Description:
 *   Get the spi status
 *
 * Input Parameters:
 *   dev -       Device-specific state data
 *   devid  - Device ID
 *
 * Returned Value:
 *   Returns the SPI status
 *
 ************************************************************************************/
uint8_t amebalite_spi0status(FAR struct spi_dev_s *dev, enum spi_dev_e devid)
{
	return 0;
}

/************************************************************************************
 * Name: amebalite_spi0cmddata
 *
 * Description:
 *   Send cmd to device according to devid.
 *
 * Input Parameters:
 *   dev -       Device-specific state data
 *   devid -   Device ID
 *   cmd - the command that need to be sent
 *
 * Returned Value:
 *   TBD
 *
 ************************************************************************************/
int amebalite_spi0cmddata(FAR struct spi_dev_s *dev, uint32_t devid, bool cmd)
{
	return 0;
}

/************************************************************************************
 * Name: amebalite_spi1select
 *
 * Description:
 *   Enable/disable the SPI slave select. The implementation of this method
 *   must include handshaking:  If a device is selected, it must hold off
 *   all other attempts to select the device until the device is deselecte.
 *
 * Input Parameters:
 *   dev -       Device-specific state data
 *   devid -    Device Id
 *   selected - whether it is selected or not
 *
 * Returned Value: None
 *
 *
 ************************************************************************************/
void amebalite_spi1select(FAR struct spi_dev_s *dev, enum spi_dev_e devid, bool selected)
{
	return;
}

/************************************************************************************
 * Name: amebalite_spi1status
 *
 * Description:
 *   Get the spi status
 *
 * Input Parameters:
 *   dev -       Device-specific state data
 *   devid  - Device ID
 *
 * Returned Value:
 *   Returns the SPI status
 *
 ************************************************************************************/
uint8_t amebalite_spi1status(FAR struct spi_dev_s *dev, enum spi_dev_e devid)
{
	return 0;
}

/************************************************************************************
 * Name: amebalite_spi1cmddata
 *
 * Description:
 *   Send cmd to device according to devid.
 *
 * Input Parameters:
 *   dev -       Device-specific state data
 *   devid -   Device ID
 *   cmd - the command that need to be sent
 *
 * Returned Value:
 *   TBD
 *
 ************************************************************************************/
int amebalite_spi1cmddata(FAR struct spi_dev_s *dev, uint32_t devid, bool cmd)
{
	return 0;
}

/************************************************************************************
 * Name: amebalite_spi_setfrequency
 *
 * Description:
 *   Set the SPI frequency.
 *
 * Input Parameters:
 *   dev -       Device-specific state data
 *   frequency - The SPI frequency requested
 *
 * Returned Value:
 *   Returns the actual frequency selected
 *
 ************************************************************************************/

static uint32_t amebalite_spi_setfrequency(FAR struct spi_dev_s *dev,
					uint32_t frequency)
{
	FAR struct amebalite_spidev_s *priv = (FAR struct amebalite_spidev_s *)dev;

	/* Has the SPI bus frequency changed? */

	if (frequency != priv->frequency) {
		/* Disable SPI if it is enabled */

		priv->frequency = frequency;

		if (priv->role == AMEBALITE_SPI_MASTER)
			spi_frequency(&priv->spi_object, priv->frequency);
	}

	return priv->frequency;
}

/************************************************************************************
 * Name: amebalite_spi_setmode
 *
 * Description:
 *   Set the SPI mode.  see enum spi_mode_e mode for mode definitions
 *
 * Input Parameters:
 *   dev  - Device-specific state data
 *   mode - The SPI mode requested
 *
 * Returned Value:
 *   Returns the actual frequency selected
 *
 ************************************************************************************/

static void amebalite_spi_setmode(FAR struct spi_dev_s *dev, enum spi_mode_e mode)
{
	FAR struct amebalite_spidev_s *priv = (FAR struct amebalite_spidev_s *)dev;

	spiinfo("mode=%d\n", mode);

	/* Has the mode changed? */

	if (mode != priv->mode) {
		/* Disable SPI if it is enabled */

		priv->mode = mode;
		spi_format(&priv->spi_object, priv->nbits, priv->mode, priv->role);

	}
}

/************************************************************************************
 * Name: amebalite_spi_setbits
 *
 * Description:
 *   Set the number of bits per word.
 *
 * Input Parameters:
 *   dev   - Device-specific state data
 *   nbits - The number of bits requested
 *
 * Returned Value:
 *   None
 *
 ************************************************************************************/

static void amebalite_spi_setbits(FAR struct spi_dev_s *dev, int nbits)
{
	FAR struct amebalite_spidev_s *priv = (FAR struct amebalite_spidev_s *)dev;

	spiinfo("nbits=%d\n", nbits);

	/* Has the number of bits changed? */

	if (nbits != priv->nbits) {

		if (nbits < 4 || nbits > 16) {
			return;
		}

		/* Save the selection so the subsequence re-configurations will be faster */

		priv->nbits = nbits;
		spi_format(&priv->spi_object, priv->nbits, priv->mode, priv->role);

	}
}

/****************************************************************************
 * Name: amebalite_spi_hwfeatures
 *
 * Description:
 *   Set hardware-specific feature flags.
 *
 * Input Parameters:
 *   dev      - Device-specific state data
 *   features - H/W feature flags
 *
 * Returned Value:
 *   Zero (OK) if the selected H/W features are enabled; A negated errno
 *   value if any H/W feature is not supportable.
 *
 ****************************************************************************/

#ifdef CONFIG_SPI_HWFEATURES
static int amebalite_spi_hwfeatures(FAR struct spi_dev_s *dev,
				amebalite_spi_hwfeatures_t features)
{
#ifdef CONFIG_SPI_BITORDER
	FAR struct amebalite_spidev_s *priv = (FAR struct amebalite_spidev_s *)dev;

	spiinfo("features=%08x\n", features);

	/* Other H/W features are not supported */
#endif
	return -ENOSYS;
}
#endif

/************************************************************************************
 * Name: amebalite_spi_send
 *
 * Description:
 *   Exchange one word on SPI
 *
 * Input Parameters:
 *   dev - Device-specific state data
 *   wd  - The word to send.  the size of the data is determined by the
 *         number of bits selected for the SPI interface.
 *
 * Returned Value:
 *   response
 *
 ************************************************************************************/

static uint16_t amebalite_spi_send(FAR struct spi_dev_s *dev, uint16_t wd)
{
	FAR struct amebalite_spidev_s *priv = (FAR struct amebalite_spidev_s *)dev;

	uint16_t ret = 0;
	DEBUGASSERT(priv);

	if (priv->role == AMEBALITE_SPI_MASTER) {

		ret = spi_master_write(&priv->spi_object, wd);
	} else if (priv->role == AMEBALITE_SPI_SLAVE) {

		spi_slave_write(&priv->spi_object, wd);
		ret = wd;
	}

	/* Check and clear any error flags (Reading from the SR clears the error
	 * flags).
	 */

	spiinfo("Sent: %04x Return: %04x\n", wd, ret);

	return ret;
}

/************************************************************************************
 * Name: amebalite_spi_exchange (no DMA).  aka amebalite_spi_exchange_nodma
 *
 * Description:
 *   Exchange a block of data on SPI without using DMA
 *
 * Input Parameters:
 *   dev      - Device-specific state data
 *   txbuffer - A pointer to the buffer of data to be sent
 *   rxbuffer - A pointer to a buffer in which to receive data
 *   nwords   - the length of data to be exchaned in units of words.
 *              The wordsize is determined by the number of bits-per-word
 *              selected for the SPI interface.  If nbits <= 8, the data is
 *              packed into uint8_t's; if nbits >8, the data is packed into uint16_t's
 *
 * Returned Value:
 *   None
 *
 ************************************************************************************/

#if !defined(CONFIG_AMEBALITE_SPI_DMA) || defined(CONFIG_AMEBALITE_DMACAPABLE)
#if !defined(CONFIG_AMEBALITE_SPI_DMA)
static void amebalite_spi_exchange(FAR struct spi_dev_s *dev,
				FAR const void *txbuffer, FAR void *rxbuffer,
				size_t nwords)
#else
static void amebalite_spi_exchange_nodma(FAR struct spi_dev_s *dev,
				FAR const void *txbuffer,
				FAR void *rxbuffer, size_t nwords)
#endif
{
	FAR struct amebalite_spidev_s *priv = (FAR struct amebalite_spidev_s *)dev;
	DEBUGASSERT(priv);

	spiinfo("txbuffer=%p rxbuffer=%p nwords=%d\n", txbuffer, rxbuffer, nwords);

	/* 8- or 16-bit mode? */

	if (amebalite_spi_9to16bitmode(priv)) {
		/* 16-bit mode */

		const uint16_t *src = (const uint16_t *)txbuffer;
		uint16_t *dest = (uint16_t *)rxbuffer;
		uint16_t word;

		while (nwords-- > 0) {
			/* Get the next word to write.  Is there a source buffer? */

			if (src) {
				word = *src++;
			} else {
				word = 0xffff;
			}

			/* Exchange one word */

			word = amebalite_spi_send(dev, word);

			/* Is there a buffer to receive the return value? */

			if (dest) {
				*dest++ = word;
			}
		}
	} else {
		/* 8-bit mode */

		const uint8_t *src = (const uint8_t *)txbuffer;
		uint8_t *dest = (uint8_t *)rxbuffer;
		uint8_t word;

		while (nwords-- > 0) {
			/* Get the next word to write.  Is there a source buffer? */

			if (src) {
				word = *src++;
			} else {
				word = 0xff;
			}

			/* Exchange one word */

			word = (uint8_t)amebalite_spi_send(dev, (uint16_t) word);

			/* Is there a buffer to receive the return value? */

			if (dest) {
				*dest++ = word;
			}
		}
	}
}
#endif /* !CONFIG_AMEBALITE_SPI_DMA || CONFIG_AMEBALITE_DMACAPABLE */

/****************************************************************************
 * Name: amebalite_spi_sndblock
 *
 * Description:
 *   Send a block of data on SPI
 *
 * Input Parameters:
 *   dev      - Device-specific state data
 *   txbuffer - A pointer to the buffer of data to be sent
 *   nwords   - the length of data to send from the buffer in number of words.
 *              The wordsize is determined by the number of bits-per-word
 *              selected for the SPI interface.  If nbits <= 8, the data is
 *              packed into uint8_t's; if nbits >8, the data is packed into uint16_t's
 *
 * Returned Value:
 *   None
 *
 ************************************************************************************/

#ifndef CONFIG_SPI_EXCHANGE
static void amebalite_spi_sndblock(FAR struct spi_dev_s *dev,
				FAR const void *txbuffer, size_t nwords)
{
	spiinfo("txbuffer=%p nwords=%d\n", txbuffer, nwords);
	return amebalite_spi_exchange(dev, txbuffer, NULL, nwords);
}
#endif

/************************************************************************************
 * Name: amebalite_spi_recvblock
 *
 * Description:
 *   Receive a block of data from SPI
 *
 * Input Parameters:
 *   dev      - Device-specific state data
 *   rxbuffer - A pointer to the buffer in which to receive data
 *   nwords   - the length of data that can be received in the buffer in number
 *              of words.  The wordsize is determined by the number of bits-per-word
 *              selected for the SPI interface.  If nbits <= 8, the data is
 *              packed into uint8_t's; if nbits >8, the data is packed into uint16_t's
 *
 * Returned Value:
 *   None
 *
 ************************************************************************************/

#ifndef CONFIG_SPI_EXCHANGE
static void amebalite_spi_recvblock(FAR struct spi_dev_s *dev, FAR void *rxbuffer,
				size_t nwords)
{
	spiinfo("rxbuffer=%p nwords=%d\n", rxbuffer, nwords);
	return amebalite_spi_exchange(dev, NULL, rxbuffer, nwords);
}
#endif

/************************************************************************************
 * Name: amebalite_spi_clock_enable
 *
 * Description:
 *   Ungate SPI clock
 *
 ************************************************************************************/

void amebalite_spi_clock_enable(uint32_t base)
{
	return;
}

/************************************************************************************
 * Name: amebalite_spi_clock_disable
 *
 * Description:
 *   Gate SPI clock
 *
 ************************************************************************************/

void amebalite_spi_clock_disable(uint32_t base)
{
	return;
}

/************************************************************************************
 * Name: amebalite_spi_bus_initialize
 *
 * Description:
 *   Initialize the selected SPI bus in its default state (Master, 8-bit, mode 0, etc.)
 *
 * Input Parameters:
 *   priv   - private SPI device structure
 *
 * Returned Value:
 *   None
 *
 ************************************************************************************/

static void amebalite_spi_bus_initialize(struct amebalite_spidev_s *priv)
{

	DEBUGASSERT(priv);
	DEBUGASSERT(&priv->spi_object);

	priv->spi_object.spi_idx = priv->spi_idx;
	spi_init(&priv->spi_object, priv->spi_mosi, priv->spi_miso, priv->spi_sclk, priv->spi_cs);
	spi_format(&priv->spi_object, priv->nbits, priv->mode, priv->role);
	sem_init(&priv->exclsem, 0, 1);

}

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: amebalite_spibus_initialize
 *
 * Description:
 *   Initialize the selected SPI bus
 *
 * Input Parameters:
 *   Port number (for hardware that has mutiple SPI interfaces)
 *
 * Returned Value:
 *   Valid SPI device structure reference on success; a NULL on failure
 *
 ************************************************************************************/

FAR struct spi_dev_s *amebalite_spibus_initialize(int bus)
{
	FAR struct amebalite_spidev_s *priv = NULL;

	irqstate_t flags = irqsave();

	if (bus == 0) {
		/* Select SPI0 */

		priv = &g_spi0dev;

		/* Only configure if the bus is not already configured */

		amebalite_spi_bus_initialize(priv);

	} else if (bus == 1) {
		/* Select SPI1 */

		priv = &g_spi1dev;

		/* Only configure if the bus is not already configured */

		amebalite_spi_bus_initialize(priv);

	} else {
		spierr("ERROR: Unsupported SPI bus: %d\n", bus);
		irqrestore(flags);
		return NULL;
	}

	irqrestore(flags);

	return (FAR struct spi_dev_s *)priv;
}

/************************************************************************************
 * Name: up_spiinitialize
 *
 * Description:
 *   Initialize the selected SPI bus
 *
 * Input Parameters:
 *   Port number (for hardware that has mutiple SPI interfaces)
 *
 * Returned Value:
 *   Valid SPI device structure reference on success; a NULL on failure
 *
 ************************************************************************************/

FAR struct spi_dev_s *up_spiinitialize(int port)
{
	FAR struct amebalite_spidev_s *priv = NULL;

	irqstate_t flags = irqsave();

	if (port == 0) {
		/* Select SPI0 */

		priv = &g_spi0dev;

		/* Only configure if the bus is not already configured */

		amebalite_spi_bus_initialize(priv);
	}
	else if (port == 1) {
		/* Select SPI1 */

		priv = &g_spi1dev;

		/* Only configure if the bus is not already configured */

		amebalite_spi_bus_initialize(priv);
	} else
	{
		spierr("ERROR: Unsupported SPI bus: %d\n", port);
		irqrestore(flags);
		return NULL;
	}

	irqrestore(flags);

	return (FAR struct spi_dev_s *)priv;
}
