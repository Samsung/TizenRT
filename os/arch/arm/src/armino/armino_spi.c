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

#include "armino_spi.h"
#include "driver/spi.h"
#include "hal_spi_types.h"
/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* Configuration ********************************************************************/

/* SPI interrupts */
#ifdef CONFIG_ARMINO_SPI_INTERRUPTS
#error "Interrupt driven SPI not yet supported"
#endif

/* Can't have both interrupt driven SPI and SPI DMA */

#if defined(CONFIG_ARMINO_SPI_INTERRUPTS) && defined(CONFIG_ARMINO_SPI_DMA)
#error "Cannot enable both interrupt mode and DMA mode for SPI"
#endif

#define DEBUG_ARMINO_SPI
#undef spierr
#undef spiinfo
#if defined(DEBUG_ARMINO_SPI)
#define spiinfo(format, ...)   printf(format, ##__VA_ARGS__)
#define spierr(format, ...)    printf(format, ##__VA_ARGS__)
#else
#define spiinfo(format, ...)   
#define spierr(format, ...)    printf(format, ##__VA_ARGS__)
#endif

#define ARMINO_SPI_MASTER	0
#define ARMINO_SPI_SLAVE	1

#define DMA_BUFFERSIZE		4096
#define SPI_BIT_WIDTH_8			8
#define SPI_BIT_WIDTH_16		16

/************************************************************************************
 * Private Types
 ************************************************************************************/

struct armino_spidev_s {
	struct spi_dev_s spidev;    /* Externally visible part of the SPI interface */
	sem_t exclsem;              /* Held while chip is selected for mutual exclusion */
	uint32_t spi_idx;
	//armino spi config
	spi_role_t role;           /**< SPI as master or slave */
	spi_bit_width_t bit_width; /**< SPI data bit witdth */
	spi_polarity_t polarity;   /**< SPI clock polarity */
	spi_phase_t phase;         /**< SPI clock phase */
	spi_wire_mode_t wire_mode; /**< SPI wire mode */
	uint32_t baud_rate;        /**< SPI transmit and receive SCK clock */
	spi_bit_order_t bit_order; /**< SPI bit order, MSB/LSB */
#if (CONFIG_SPI_BYTE_INTERVAL)
	uint32_t byte_interval;    /**< SPI byte interval, only for master */
#endif
#if (CONFIG_SPI_DMA)
	spi_dma_mode_t dma_mode;   /**< SPI whether use dma */
	dma_id_t spi_tx_dma_chan;  /**< SPI tx dma channel */
	dma_id_t spi_rx_dma_chan;  /**< SPI rx dma channel */
	dma_data_width_t spi_tx_dma_width;  /**< SPI tx dma destination width */
	dma_data_width_t spi_rx_dma_width;  /**< SPI rx dma source width */
#endif
};

enum armino_delay_e {
	SPI_PCS_TO_SCK = 1,       /* PCS-to-SCK delay. */
	SPI_LAST_SCK_TO_PCS,      /* Last SCK edge to PCS delay. */
	SPI_BETWEEN_TRANSFER      /* Delay between transfers. */
};

/************************************************************************************
 * Private Function Prototypes
 ************************************************************************************/

/* Helpers */

static inline uint32_t armino_spi_getreg32(FAR struct armino_spidev_s *priv,
					uint8_t offset);
static inline void armino_spi_putreg32(FAR struct armino_spidev_s *priv,
					uint8_t offset, uint32_t value);
static inline uint16_t armino_spi_readword(FAR struct armino_spidev_s *priv);
static inline void armino_spi_writeword(FAR struct armino_spidev_s *priv,
					uint16_t byte);
static inline bool armino_spi_9to16bitmode(FAR struct armino_spidev_s *priv);
static inline void armino_spi_master_set_delays(FAR struct armino_spidev_s
					*priv, uint32_t delay_ns,
					enum armino_delay_e type);
static inline void armino_spi_master_set_delay_scaler(FAR struct
					armino_spidev_s *priv,
					uint32_t scaler,
					enum armino_delay_e type);

/* SPI methods */

static int armino_spi_lock(FAR struct spi_dev_s *dev, bool lock);
static uint32_t armino_spi_setfrequency(FAR struct spi_dev_s *dev,
					uint32_t frequency);
static void armino_spi_setmode(FAR struct spi_dev_s *dev,
				enum spi_mode_e mode);
static void armino_spi_setbits(FAR struct spi_dev_s *dev, int nbits);
#ifdef CONFIG_SPI_HWFEATURES
static int armino_spi_hwfeatures(FAR struct spi_dev_s *dev,
				armino_spi_hwfeatures_t features);
#endif
static uint16_t armino_spi_send(FAR struct spi_dev_s *dev, uint16_t wd);
static void armino_spi_exchange(FAR struct spi_dev_s *dev,
				FAR const void *txbuffer, FAR void *rxbuffer,
				size_t nwords);
#ifndef CONFIG_SPI_EXCHANGE
static void armino_spi_sndblock(FAR struct spi_dev_s *dev,
				FAR const void *txbuffer, size_t nwords);
static void armino_spi_recvblock(FAR struct spi_dev_s *dev, FAR void *rxbuffer,
				size_t nwords);
#endif

/* Initialization */

static void armino_spi_bus_initialize(FAR struct armino_spidev_s *priv);

/************************************************************************************
 * Private Data
 ************************************************************************************/

static const struct spi_ops_s g_spi0ops = {
	.lock         = armino_spi_lock,
	.select       = armino_spi0select,
	.setfrequency = armino_spi_setfrequency,
	.setmode      = armino_spi_setmode,
	.setbits      = armino_spi_setbits,
#ifdef CONFIG_SPI_HWFEATURES
	.hwfeatures   = armino_spi_hwfeatures,
#endif
	.status       = armino_spi0status,
#ifdef CONFIG_SPI_CMDDATA
	.cmddata      = armino_spi0cmddata,
#endif
	.send         = armino_spi_send,
#ifdef CONFIG_SPI_EXCHANGE
	.exchange     = armino_spi_exchange,
#else
	.sndblock     = armino_spi_sndblock,
	.recvblock    = armino_spi_recvblock,
#endif
#ifdef CONFIG_SPI_CALLBACK
	.registercallback = armino_spi0register,  /* Provided externally */
#else
	.registercallback = 0,                     /* Not implemented */
#endif
};

static struct armino_spidev_s g_spi0dev = {
	.spidev       = {
		&g_spi0ops
	},
	.spi_idx = 0,
	.role = SPI_ROLE_SLAVE,
	.bit_width = SPI_BIT_WIDTH_8BITS,
	.polarity = SPI_POLARITY_LOW,
	.phase = SPI_PHASE_1ST_EDGE,
	.wire_mode = SPI_4WIRE_MODE,
	.baud_rate = 6000000,
	.bit_order = SPI_MSB_FIRST,
	.byte_interval = 1,
	.dma_mode = SPI_DMA_MODE_ENABLE,
	.spi_tx_dma_width = DMA_DATA_WIDTH_8BITS,
	.spi_rx_dma_width = DMA_DATA_WIDTH_8BITS,
};

static const struct spi_ops_s g_spi1ops = {
	.lock         = armino_spi_lock,
	.select       = armino_spi1select,
	.setfrequency = armino_spi_setfrequency,
	.setmode      = armino_spi_setmode,
	.setbits      = armino_spi_setbits,
#ifdef CONFIG_SPI_HWFEATURES
	.hwfeatures   = armino_spi_hwfeatures,
#endif
	.status       = armino_spi1status,
#ifdef CONFIG_SPI_CMDDATA
	.cmddata      = armino_spi1cmddata,
#endif
	.send         = armino_spi_send,
#ifdef CONFIG_SPI_EXCHANGE
	.exchange     = armino_spi_exchange,
#else
	.sndblock     = armino_spi_sndblock,
	.recvblock    = armino_spi_recvblock,
#endif
#ifdef CONFIG_SPI_CALLBACK
	.registercallback = armino_spi1register,  /* Provided externally */
#else
	.registercallback = 0,                     /* Not implemented */
#endif
};

static struct armino_spidev_s g_spi1dev = {
	.spidev       = {
		&g_spi1ops
	},
	.spi_idx = 1,
	.role = SPI_ROLE_MASTER,
	.bit_width = SPI_BIT_WIDTH_8BITS,
	.polarity = SPI_POLARITY_LOW,
	.phase = SPI_PHASE_1ST_EDGE,
	.wire_mode = SPI_4WIRE_MODE,
	.baud_rate = 6000000,
	.bit_order = SPI_MSB_FIRST,
	.byte_interval = 1,
	.dma_mode = SPI_DMA_MODE_ENABLE,
	.spi_tx_dma_width = DMA_DATA_WIDTH_8BITS,
	.spi_rx_dma_width = DMA_DATA_WIDTH_8BITS,
};
/************************************************************************************
 * Private Functions
 ************************************************************************************/

/************************************************************************************
 * Name: armino_spi_getreg8
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

static inline uint8_t armino_spi_getreg8(FAR struct armino_spidev_s *priv,
					uint8_t offset)
{
	return 0;
}

/************************************************************************************
 * Name: armino_spi_putreg8
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

static inline void armino_spi_putreg8(FAR struct armino_spidev_s *priv,
				uint8_t offset, uint8_t value)
{
	return;
}

/************************************************************************************
 * Name: armino_spi_getreg
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

static inline uint32_t armino_spi_getreg32(FAR struct armino_spidev_s *priv,
					uint8_t offset)
{
	return 0;
}

/************************************************************************************
 * Name: armino_spi_putreg
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

static inline void armino_spi_putreg32(FAR struct armino_spidev_s *priv,
					uint8_t offset, uint32_t value)
{
	return;
}

/************************************************************************************
 * Name: armino_spi_readword
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

static inline uint16_t armino_spi_readword(FAR struct armino_spidev_s *priv)
{
	return 0;
}

/************************************************************************************
 * Name: armino_spi_writeword
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

static inline void armino_spi_writeword(FAR struct armino_spidev_s *priv, uint16_t word)
{
	return;
}

/************************************************************************************
 * Name: armino_spi_readbyte
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

static inline uint8_t armino_spi_readbyte(FAR struct armino_spidev_s *priv)
{
	return 0;
}

/************************************************************************************
 * Name: armino_spi_writebyte
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

static inline void armino_spi_writebyte(FAR struct armino_spidev_s *priv, uint8_t byte)
{
	return;
}

/************************************************************************************
 * Name: armino_spi_9to16bitmode
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

static inline bool armino_spi_9to16bitmode(FAR struct armino_spidev_s *priv)
{
	if (priv->bit_width < 9) return false;
	else return true;
}

/************************************************************************************
 * Name: armino_spi_modifyreg
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

static void armino_spi_modifyreg32(FAR struct armino_spidev_s *priv,
				uint8_t offset, uint32_t clrbits,
				uint32_t setbits)
{
}

/************************************************************************************
 * Name: armino_spi_master_set_delays
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

static inline void armino_spi_master_set_delay_scaler(FAR struct armino_spidev_s *priv,
						uint32_t scaler,
						enum armino_delay_e type)
{

}

/************************************************************************************
 * Name: armino_spi_master_set_delays
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
static inline void armino_spi_master_set_delays(FAR struct armino_spidev_s *priv,
						uint32_t delay_ns,
						enum armino_delay_e type)
{
}

/************************************************************************************
 * Name: armino_spi_lock
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

static int armino_spi_lock(FAR struct spi_dev_s *dev, bool lock)
{
	FAR struct armino_spidev_s *priv = (FAR struct armino_spidev_s *)dev;

	if (lock) {
		/* Take the semaphore (perhaps waiting) */

		/* The only case that an error should occur here is if the wait was
		 * awakened by a signal.
		 */

		while (sem_wait(&priv->exclsem) != 0) {
			DEBUGASSERT(errno == EINTR);
                }

	} else {
		(void)sem_post(&priv->exclsem);
	}

	return OK;
}

/************************************************************************************
 * Name: armino_spi0select
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
void armino_spi0select(FAR struct spi_dev_s *dev, enum spi_dev_e devid, bool selected)
{
	return;
}

/************************************************************************************
 * Name: spi_dmatrxcallback
 *
 * Description:
 *   Callback function for SPI DMA TxRx which enters when SPI DMA complete
 *
 * Returned Value: None
 *
 *
 ************************************************************************************/
#ifdef CONFIG_ARMINO_SPI_DMA
static void spi_dmatrxcallback(void *pdata, SpiIrq event)
{
	FAR struct armino_spidev_s *priv = (FAR struct armino_spidev_s *)pdata;
	switch(event){
		case SpiRxIrq:
			(void)sem_post(&priv->rxsem);
			break;
		case SpiTxIrq:
			(void)sem_post(&priv->txsem);
			break;
		default:
			spierr("ERROR: unknown interrput evnent!\n");
	}
}
#endif

#ifdef CONFIG_ARMINO_SPI_DMA
static void spi_dmarxwait(FAR struct armino_spidev_s *priv)
{
	/* Take the semaphore (perhaps waiting).  If the result is zero, then the DMA
	 * must not really have completed???
	 */

	while (sem_wait(&priv->rxsem) != 0) {
		/* The only case that an error should occur here is if the wait was awakened
		 * by a signal.
		 */

		ASSERT(errno == EINTR);
	}
}
#endif

/************************************************************************************
 * Name: spi_dmatxwait
 *
 * Description:
 *   Wait for DMA to complete.
 *
 ************************************************************************************/

#ifdef CONFIG_ARMINO_SPI_DMA
static void spi_dmatxwait(FAR struct armino_spidev_s *priv)
{
	/* Take the semaphore (perhaps waiting).  If the result is zero, then the DMA
	 * must not really have completed???
	 */

	while (sem_wait(&priv->txsem) != 0) {
		/* The only case that an error should occur here is if the wait was awakened
		 * by a signal.
		 */

		ASSERT(errno == EINTR);
	}
}
#endif
/************************************************************************************
 * Name: armino_spi0status
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
uint8_t armino_spi0status(FAR struct spi_dev_s *dev, enum spi_dev_e devid)
{
	return 0;
}

/************************************************************************************
 * Name: armino_spi0cmddata
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
int armino_spi0cmddata(FAR struct spi_dev_s *dev, enum spi_dev_e devid, bool cmd)
{
	return 0;
}

/************************************************************************************
 * Name: armino_spi1select
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
void armino_spi1select(FAR struct spi_dev_s *dev, enum spi_dev_e devid, bool selected)
{
	//SPI CS PIN auto select
	return;
}

/************************************************************************************
 * Name: armino_spi1status
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
uint8_t armino_spi1status(FAR struct spi_dev_s *dev, enum spi_dev_e devid)
{
	return 0;
}

/************************************************************************************
 * Name: armino_spi1cmddata
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
int armino_spi1cmddata(FAR struct spi_dev_s *dev, enum spi_dev_e devid, bool cmd)
{
	return 0;
}

/************************************************************************************
 * Name: armino_spi_setfrequency
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

static uint32_t armino_spi_setfrequency(FAR struct spi_dev_s *dev,
					uint32_t frequency)
{
	FAR struct armino_spidev_s *priv = (FAR struct armino_spidev_s *)dev;

	DEBUGASSERT(priv);

	bk_spi_set_baud_rate(priv->spi_idx, frequency);
    return 0;
}

/************************************************************************************
 * Name: armino_spi_setmode
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

static void armino_spi_setmode(FAR struct spi_dev_s *dev, enum spi_mode_e mode)
{
	FAR struct armino_spidev_s *priv = (FAR struct armino_spidev_s *)dev;

	DEBUGASSERT(priv);

	bk_spi_set_mode(priv->spi_idx, (spi_mode_t) mode);
}

/************************************************************************************
 * Name: armino_spi_setbits
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


static void armino_spi_setbits(FAR struct spi_dev_s *dev, int nbits)
{
	FAR struct armino_spidev_s *priv = (FAR struct armino_spidev_s *)dev;

	DEBUGASSERT(priv);
	if(SPI_BIT_WIDTH_8 == nbits) {
		bk_spi_set_bit_width(priv->spi_idx, SPI_BIT_WIDTH_8BITS);
	} else if(SPI_BIT_WIDTH_16 == nbits) {
		bk_spi_set_bit_width(priv->spi_idx, SPI_BIT_WIDTH_16BITS);
	} else {
		spiinfo("nbits=%d not support\n", nbits);
	}
}

/****************************************************************************
 * Name: armino_spi_hwfeatures
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
static int armino_spi_hwfeatures(FAR struct spi_dev_s *dev,
				armino_spi_hwfeatures_t features)
{
#ifdef CONFIG_SPI_BITORDER
	FAR struct armino_spidev_s *priv = (FAR struct armino_spidev_s *)dev;

	spiinfo("features=%08x\n", features);

	/* Other H/W features are not supported */
#endif
	return -ENOSYS;
}
#endif

/************************************************************************************
 * Name: armino_spi_send
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

static uint16_t armino_spi_send(FAR struct spi_dev_s *dev, uint16_t wd)
{
	FAR struct armino_spidev_s *priv = (FAR struct armino_spidev_s *)dev;

	uint16_t ret = 0;
	DEBUGASSERT(priv);

	ret = bk_spi_dma_write_bytes(priv->spi_idx, (void *)&wd, 1);
	if (priv->role == SPI_ROLE_MASTER) {
	} else if (priv->role == SPI_ROLE_SLAVE) {
		ret = wd;
	}

	/* Check and clear any error flags (Reading from the SR clears the error
	 * flags).
	 */

	spiinfo("Sent: %04x Return: %04x\n", wd, ret);

	return ret;
}

/************************************************************************************
 * Name: armino_spi_exchange (no DMA).  aka armino_spi_exchange_nodma
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

static void armino_spi_exchange(FAR struct spi_dev_s *dev,
				FAR const void *txbuffer, FAR void *rxbuffer,
				size_t nwords)
{
	FAR struct armino_spidev_s *priv = (FAR struct armino_spidev_s *)dev;
	DEBUGASSERT(priv);

	bk_spi_dma_duplex_xfer(priv->spi_idx, txbuffer, nwords, rxbuffer, nwords);
}

/****************************************************************************
 * Name: armino_spi_sndblock
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
static void armino_spi_sndblock(FAR struct spi_dev_s *dev,
				FAR const void *txbuffer, size_t nwords)
{
	spiinfo("txbuffer=%p nwords=%d\n", txbuffer, nwords);
	return armino_spi_exchange(dev, txbuffer, NULL, nwords);
}
#endif

/************************************************************************************
 * Name: armino_spi_recvblock
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
static void armino_spi_recvblock(FAR struct spi_dev_s *dev, FAR void *rxbuffer,
				size_t nwords)
{
	spiinfo("rxbuffer=%p nwords=%d\n", rxbuffer, nwords);
	return armino_spi_exchange(dev, NULL, rxbuffer, nwords);
}
#endif

/************************************************************************************
 * Name: armino_spi_clock_enable
 *
 * Description:
 *   Ungate SPI clock
 *
 ************************************************************************************/

void armino_spi_clock_enable(uint32_t base)
{
	return;
}

/************************************************************************************
 * Name: armino_spi_clock_disable
 *
 * Description:
 *   Gate SPI clock
 *
 ************************************************************************************/

void armino_spi_clock_disable(uint32_t base)
{
	return;
}

/************************************************************************************
 * Name: armino_spi_bus_initialize
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

static void armino_spi_bus_initialize(struct armino_spidev_s *priv)
{
	DEBUGASSERT(priv);

	bk_spi_driver_init();
	spi_config_t config = {0};
	config.role = priv->role;
	config.bit_width = priv->bit_width;
	config.polarity = priv->polarity;
	config.phase = priv->phase;
	config.wire_mode = priv->wire_mode;
	config.baud_rate = priv->baud_rate;
	config.bit_order = priv->bit_order;
#if (CONFIG_SPI_BYTE_INTERVAL)
	config.byte_interval = priv->byte_interval;
#endif
#if CONFIG_SPI_DMA
	config.dma_mode = priv->dma_mode;
	config.spi_tx_dma_chan = bk_dma_alloc(DMA_DEV_GSPI0);
	config.spi_rx_dma_chan = bk_dma_alloc(DMA_DEV_GSPI0_RX);
	config.spi_tx_dma_width = priv->spi_tx_dma_width;
	config.spi_rx_dma_width = priv->spi_rx_dma_width;
#endif
	BK_LOG_ON_ERR(bk_spi_init(priv->spi_idx, &config));

	/* Initialize the SPI semaphore that enforces mutually exclusive access */
    sem_init(&priv->exclsem, 0, 1);

	//beken can auto select cs pin; beken tx/ rx sem is handled in transmit api
}

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: armino_spibus_initialize
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

FAR struct spi_dev_s *armino_spibus_initialize(int bus)
{
	FAR struct armino_spidev_s *priv = NULL;

	irqstate_t flags = irqsave();

	if (bus == 1) {
		/* Select SPI1 */

		priv = &g_spi0dev;

		/* Only configure if the bus is not already configured */

		armino_spi_bus_initialize(priv);

	} else if (bus == 2) {
		/* Select SPI2 */

		priv = &g_spi1dev;

		/* Only configure if the bus is not already configured */

		armino_spi_bus_initialize(priv);

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
	FAR struct armino_spidev_s *priv = NULL;

	irqstate_t flags = irqsave();

	if (port == 0) {
		/* Select SPI1 */

		priv = &g_spi0dev;

		/* Only configure if the bus is not already configured */

		armino_spi_bus_initialize(priv);
	}
	else if (port == 1) {
		/* Select SPI1 */
		priv = &g_spi1dev;

		/* Only configure if the bus is not already configured */
		armino_spi_bus_initialize(priv);
	} else
	{
		spierr("ERROR: Unsupported SPI bus: %d\n", port);
		irqrestore(flags);
		return NULL;
	}

	irqrestore(flags);

	return (FAR struct spi_dev_s *)priv;
}