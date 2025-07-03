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


#include "chip.h"

#include "amebasmart_spi.h"
#include "PinNames.h"
#include "spi_api.h"
#include "spi_ex_api.h"

#ifdef CONFIG_PM
#include <tinyara/pm/pm.h>
#endif

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* Configuration ********************************************************************/

/* SPI interrupts */

#ifdef CONFIG_AMEBASMART_SPI_INTERRUPTS
#error "Interrupt driven SPI not yet supported"
#endif



/* Can't have both interrupt driven SPI and SPI DMA */

#if defined(CONFIG_AMEBASMART_SPI_INTERRUPTS) && defined(CONFIG_AMEBASMART_SPI_DMA)
#error "Cannot enable both interrupt mode and DMA mode for SPI"
#endif

#undef spierr
#undef spiinfo
#if defined(CONFIG_DEBUG_SPI)
#define spiinfo(format, ...)   printf(format, ##__VA_ARGS__)
#define spierr(format, ...)    printf(format, ##__VA_ARGS__)
#else
#define spiinfo(format, ...)
//#define spierr(format, ...)
#endif
#define spierr(format, ...)   printf(format, ##__VA_ARGS__)

#define AMEBASMART_SPI_MASTER	0
#define AMEBASMART_SPI_SLAVE	1
#define SPI_DMA_MAX_BUFFER_SIZE 65535
/************************************************************************************
 * Private Types
 ************************************************************************************/

struct amebasmart_spidev_s {
	struct spi_dev_s spidev;    /* Externally visible part of the SPI interface */
//	uint32_t spibase;           /* SPIn base address */
//#ifdef CONFIG_AMEBASMART_SPI_INTERRUPTS
//	uint8_t spiirq;             /* SPI IRQ number */
//#endif
	sem_t exclsem;              /* Held while chip is selected for mutual exclusion */
	uint32_t frequency;         /* Requested clock frequency */
	uint32_t actual;            /* Actual clock frequency */
	int refs;                    /* Reference count */

#ifdef CONFIG_AMEBASMART_SPI_DMA
	sem_t rxsem;				/* Wait for RX DMA to complete */
	sem_t txsem;				/* Wait for TX DMA to complete */
#endif
	spi_t spi_object;
	uint32_t spi_idx;
	PinName spi_mosi;
	PinName spi_miso;
	PinName spi_sclk;
	PinName spi_cs0;
	int8_t nbits;               /* Width of word in bits */
	uint8_t mode;               /* Mode 0,1,2,3 */
	int role;
	int8_t multi_cs:1;
	gpio_t gpio_cs0;
#ifdef CONFIG_SPI_CS
	gpio_t gpio_cs1;
	PinName spi_cs1;
#endif
};

enum amebasmart_delay_e {
	SPI_PCS_TO_SCK = 1,       /* PCS-to-SCK delay. */
	SPI_LAST_SCK_TO_PCS,      /* Last SCK edge to PCS delay. */
	SPI_BETWEEN_TRANSFER      /* Delay between transfers. */
};

/************************************************************************************
 * Private Function Prototypes
 ************************************************************************************/

/* Helpers */

static inline uint32_t amebasmart_spi_getreg32(FAR struct amebasmart_spidev_s *priv,
					uint8_t offset);
static inline void amebasmart_spi_putreg32(FAR struct amebasmart_spidev_s *priv,
					uint8_t offset, uint32_t value);
static inline uint16_t amebasmart_spi_readword(FAR struct amebasmart_spidev_s *priv);
static inline void amebasmart_spi_writeword(FAR struct amebasmart_spidev_s *priv,
					uint16_t byte);
static inline bool amebasmart_spi_9to16bitmode(FAR struct amebasmart_spidev_s *priv);
static inline void amebasmart_spi_master_set_delays(FAR struct amebasmart_spidev_s
					*priv, uint32_t delay_ns,
					enum amebasmart_delay_e type);
static inline void amebasmart_spi_master_set_delay_scaler(FAR struct
					amebasmart_spidev_s *priv,
					uint32_t scaler,
					enum amebasmart_delay_e type);

/* DMA support*/
#ifdef CONFIG_AMEBASMART_SPI_DMA
static void spi_dmarxwait(FAR struct amebasmart_spidev_s *priv);
static void spi_dmatxwait(FAR struct amebasmart_spidev_s *priv);
static inline void spi_dmarxwakeup(FAR struct amebasmart_spidev_s *priv);
static inline void spi_dmatxwakeup(FAR struct amebasmart_spidev_s *priv);
static void spi_dmamastercallback(FAR struct amebasmart_spidev_s *priv, SpiIrq event);
static void spi_hookmastercallback(FAR struct amebasmart_spidev_s *priv);
#endif

/* SPI methods */

static int amebasmart_spi_lock(FAR struct spi_dev_s *dev, bool lock);
static uint32_t amebasmart_spi_setfrequency(FAR struct spi_dev_s *dev,
					uint32_t frequency);
static void amebasmart_spi_setmode(FAR struct spi_dev_s *dev,
				enum spi_mode_e mode);
static void amebasmart_spi_setbits(FAR struct spi_dev_s *dev, int nbits);
#ifdef CONFIG_SPI_HWFEATURES
static int amebasmart_spi_hwfeatures(FAR struct spi_dev_s *dev,
				amebasmart_spi_hwfeatures_t features);
#endif
static uint16_t amebasmart_spi_send(FAR struct spi_dev_s *dev, uint16_t wd);
static void amebasmart_spi_exchange(FAR struct spi_dev_s *dev,
				FAR const void *txbuffer, FAR void *rxbuffer,
				size_t nwords);
#ifndef CONFIG_SPI_EXCHANGE
static void amebasmart_spi_sndblock(FAR struct spi_dev_s *dev,
				FAR const void *txbuffer, size_t nwords);
static void amebasmart_spi_recvblock(FAR struct spi_dev_s *dev, FAR void *rxbuffer,
				size_t nwords);
#endif

/* Initialization */

static void amebasmart_spi_bus_initialize(FAR struct amebasmart_spidev_s *priv, uint8_t multi_cs);

/************************************************************************************
 * Private Data
 ************************************************************************************/
#ifdef CONFIG_AMEBASMART_SPI0
static const struct spi_ops_s g_spi0ops = {
	.lock         = amebasmart_spi_lock,
	.select       = amebasmart_spi_select,
	.setfrequency = amebasmart_spi_setfrequency,
	.setmode      = amebasmart_spi_setmode,
	.setbits      = amebasmart_spi_setbits,
#ifdef CONFIG_SPI_HWFEATURES
	.hwfeatures   = amebasmart_spi_hwfeatures,
#endif
	.status       = amebasmart_spi0status,
#ifdef CONFIG_SPI_CMDDATA
	.cmddata      = amebasmart_spi0cmddata,
#endif
	.send         = amebasmart_spi_send,
#ifdef CONFIG_SPI_EXCHANGE
	.exchange     = amebasmart_spi_exchange,
#else
	.sndblock     = amebasmart_spi_sndblock,
	.recvblock    = amebasmart_spi_recvblock,
#endif
#ifdef CONFIG_SPI_CALLBACK
	.registercallback = amebasmart_spi0register,  /* Provided externally */
#else
	.registercallback = 0,                     /* Not implemented */
#endif
};

static struct amebasmart_spidev_s g_spi0dev = {
	.spidev       = {
		&g_spi0ops
	},
	/*
	.spibase      = AMEBASMART_SPI1_BASE,
#ifdef CONFIG_AMEBASMART_SPI_INTERRUPTS
	.spiirq       = AMEBASMART_IRQ_SPI1,
#endif
#ifdef CONFIG_AMEBASMART_SPI_DMA
	.rxch         = DMAMAP_SPI1_RX,
	.txch         = DMAMAP_SPI1_TX,
#endif
*/

	.spi_object = {0},
	.refs = 0,
	.spi_idx = MBED_SPI0,
	.spi_mosi = PB_4,
	.spi_miso = PB_3,
	.spi_sclk = PB_6,
	.spi_cs0 = PB_5,
#if defined(CONFIG_SPI_CS) && defined(CONFIG_AMEBASMART_SPI0_CS)
	.spi_cs1 = PB_31,
#endif
	.nbits = 8,
	.mode = SPIDEV_MODE0,
	.role = AMEBASMART_SPI_MASTER,
};
#endif

#ifdef CONFIG_AMEBASMART_SPI1
static const struct spi_ops_s g_spi1ops = {
	.lock         = amebasmart_spi_lock,
	.select       = amebasmart_spi_select,
	.setfrequency = amebasmart_spi_setfrequency,
	.setmode      = amebasmart_spi_setmode,
	.setbits      = amebasmart_spi_setbits,
#ifdef CONFIG_SPI_HWFEATURES
	.hwfeatures   = amebasmart_spi_hwfeatures,
#endif
	.status       = amebasmart_spi1status,
#ifdef CONFIG_SPI_CMDDATA
	.cmddata      = amebasmart_spi1cmddata,
#endif
	.send         = amebasmart_spi_send,
#ifdef CONFIG_SPI_EXCHANGE
	.exchange     = amebasmart_spi_exchange,
#else
	.sndblock     = amebasmart_spi_sndblock,
	.recvblock    = amebasmart_spi_recvblock,
#endif
#ifdef CONFIG_SPI_CALLBACK
	.registercallback = amebasmart_spi1register,  /* Provided externally */
#else
	.registercallback = 0,                     /* Not implemented */
#endif
};

static struct amebasmart_spidev_s g_spi1dev = {
	.spidev       = {
		&g_spi1ops
	},
	/*
	.spibase      = AMEBASMART_SPI1_BASE,
#ifdef CONFIG_AMEBASMART_SPI_INTERRUPTS
	.spiirq       = AMEBASMART_IRQ_SPI1,
#endif
#ifdef CONFIG_AMEBASMART_SPI_DMA
	.rxch         = DMAMAP_SPI1_RX,
	.txch         = DMAMAP_SPI1_TX,
#endif
*/

	.spi_object = {0},
	.refs = 0,
	.spi_idx = MBED_SPI1,
	.spi_mosi = PB_28,
	.spi_miso = PB_27,
	.spi_sclk = PB_25,
	.spi_cs0 = PB_26,
#if defined(CONFIG_SPI_CS) && defined(CONFIG_AMEBASMART_SPI1_CS)
	.spi_cs1 = PB_29,
#endif
	.nbits = 8,
	.mode = SPIDEV_MODE0,
	.role = AMEBASMART_SPI_MASTER
};
#endif

/************************************************************************************
 * Private Functions
 ************************************************************************************/

/************************************************************************************
 * Name: amebasmart_spi_getreg8
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

static inline uint8_t amebasmart_spi_getreg8(FAR struct amebasmart_spidev_s *priv,
					uint8_t offset)
{
	return 0;
}

/************************************************************************************
 * Name: amebasmart_spi_putreg8
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

static inline void amebasmart_spi_putreg8(FAR struct amebasmart_spidev_s *priv,
				uint8_t offset, uint8_t value)
{
	return;
}

/************************************************************************************
 * Name: amebasmart_spi_getreg
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

static inline uint32_t amebasmart_spi_getreg32(FAR struct amebasmart_spidev_s *priv,
					uint8_t offset)
{
	return 0;
}

/************************************************************************************
 * Name: amebasmart_spi_putreg
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

static inline void amebasmart_spi_putreg32(FAR struct amebasmart_spidev_s *priv,
					uint8_t offset, uint32_t value)
{
	return;
}

/************************************************************************************
 * Name: amebasmart_spi_readword
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

static inline uint16_t amebasmart_spi_readword(FAR struct amebasmart_spidev_s *priv)
{
	return 0;
}

/************************************************************************************
 * Name: amebasmart_spi_writeword
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

static inline void amebasmart_spi_writeword(FAR struct amebasmart_spidev_s *priv, uint16_t word)
{
	return;
}

/************************************************************************************
 * Name: amebasmart_spi_readbyte
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

static inline uint8_t amebasmart_spi_readbyte(FAR struct amebasmart_spidev_s *priv)
{
	return 0;
}

/************************************************************************************
 * Name: amebasmart_spi_writebyte
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

static inline void amebasmart_spi_writebyte(FAR struct amebasmart_spidev_s *priv, uint8_t byte)
{
	return;
}

/************************************************************************************
 * Name: amebasmart_spi_9to16bitmode
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

static inline bool amebasmart_spi_9to16bitmode(FAR struct amebasmart_spidev_s *priv)
{
	if (priv->nbits < 9) return false;
	else return true;
}

/************************************************************************************
 * Name: spi_dmarxwait
 *
 * Description:
 *   Wait for DMA to complete.
 *
 ************************************************************************************/

#ifdef CONFIG_AMEBASMART_SPI_DMA
static void spi_dmarxwait(FAR struct amebasmart_spidev_s *priv)
{
	/* Take the semaphore (perhaps waiting).  If the result is zero, then the DMA
	 * must not really have completed???
	 */
	while (sem_wait(&priv->rxsem) != OK) {
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

#ifdef CONFIG_AMEBASMART_SPI_DMA
static void spi_dmatxwait(FAR struct amebasmart_spidev_s *priv)
{
	/* Take the semaphore (perhaps waiting).  If the result is zero, then the DMA
	 * must not really have completed???
	 */
	while (sem_wait(&priv->txsem) != OK) {
		/* The only case that an error should occur here is if the wait was awakened
		 * by a signal.
		 */
		ASSERT(errno == EINTR);
	}
	/*DMA triggers irq when all data has transferred from memory to SPI fifo, it is around 0.5ms earlier than SPI transfer all data in FIFO out
	 *compensate by adding 0.5ms delay to ensure that all data in fifo are transferred out before manually pull up CS pin by GPIO*/

}
#endif

/************************************************************************************
 * Name: spi_dmarxwakeup
 *
 * Description:
 *   Signal that DMA is complete
 *
 ************************************************************************************/

#ifdef CONFIG_AMEBASMART_SPI_DMA
static inline void spi_dmarxwakeup(FAR struct amebasmart_spidev_s *priv)
{
	(void)sem_post(&priv->rxsem);
}
#endif

/************************************************************************************
 * Name: spi_dmatxwakeup
 *
 * Description:
 *   Signal that DMA is complete
 *
 ************************************************************************************/

#ifdef CONFIG_AMEBASMART_SPI_DMA
static inline void spi_dmatxwakeup(FAR struct amebasmart_spidev_s *priv)
{
	(void)sem_post(&priv->txsem);
}
#endif

/************************************************************************************
 * Name: spi_dmamastercallback
 *
 * Description:
 *   Called when the RX DMA completes
 * 	 Called when the TX DMA completes
 *
 ************************************************************************************/

#ifdef CONFIG_AMEBASMART_SPI_DMA
static void spi_dmamastercallback(FAR struct amebasmart_spidev_s *priv, SpiIrq event)
{

	switch(event){
		case SpiRxIrq:
			spi_dmarxwakeup(priv);
			break;
		case SpiTxIrq:
			spi_dmatxwakeup(priv);
			break;
		default:
			DBG_8195A("unknown interrput event!\n");
	}
}
#endif



/************************************************************************************
 * Name: spi_hookmastercallback
 *
 * Description:
 *   Register the master DMA callback
 *
 ************************************************************************************/

#ifdef CONFIG_AMEBASMART_SPI_DMA
static void spi_hookmastercallback(FAR struct amebasmart_spidev_s *priv)
{
	spi_irq_hook(&priv->spi_object, (spi_irq_handler) spi_dmamastercallback, (uint32_t)priv);
}
#endif



/************************************************************************************
 * Name: amebasmart_spi_modifyreg
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

static void amebasmart_spi_modifyreg32(FAR struct amebasmart_spidev_s *priv,
				uint8_t offset, uint32_t clrbits,
				uint32_t setbits)
{
}

/************************************************************************************
 * Name: amebasmart_spi_master_set_delays
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

static inline void amebasmart_spi_master_set_delay_scaler(FAR struct amebasmart_spidev_s *priv,
						uint32_t scaler,
						enum amebasmart_delay_e type)
{
	/*
	switch (type) {
	case SPI_PCS_TO_SCK:
		amebasmart_spi_modifyreg32(priv, AMEBASMART_SPI_CCR_OFFSET,
					SPI_CCR_PCSSCK_MASK, 0);
		amebasmart_spi_modifyreg32(priv, AMEBASMART_SPI_CCR_OFFSET, 0,
					SPI_CCR_PCSSCK(scaler));
	break;

	case SPI_LAST_SCK_TO_PCS:
		amebasmart_spi_modifyreg32(priv, AMEBASMART_SPI_CCR_OFFSET,
					SPI_CCR_SCKPCS_MASK, 0);
		amebasmart_spi_modifyreg32(priv, AMEBASMART_SPI_CCR_OFFSET, 0,
					SPI_CCR_SCKPCS(scaler));
	break;

	case SPI_BETWEEN_TRANSFER:
		amebasmart_spi_modifyreg32(priv, AMEBASMART_SPI_CCR_OFFSET, SPI_CCR_DBT_MASK,
					0);
		amebasmart_spi_modifyreg32(priv, AMEBASMART_SPI_CCR_OFFSET, 0,
					SPI_CCR_DBT(scaler));
	break;
	}
	*/
}

/************************************************************************************
 * Name: amebasmart_spi_master_set_delays
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
static inline void amebasmart_spi_master_set_delays(FAR struct amebasmart_spidev_s *priv,
						uint32_t delay_ns,
						enum amebasmart_delay_e type)
{
}

/************************************************************************************
 * Name: amebasmart_spi_lock
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

static int amebasmart_spi_lock(FAR struct spi_dev_s *dev, bool lock)
{
	FAR struct amebasmart_spidev_s *priv = (FAR struct amebasmart_spidev_s *)dev;
	/* When a lock is invoked, means that we are going to do some SPI operation,
	   thus we should decide to suspend/resume SPI domain here
	*/
	if (lock) {
#ifdef CONFIG_PM
		bsp_pm_domain_control(BSP_SPI_DRV, 1);
#endif
		/* Take the semaphore (perhaps waiting) */

		/* The only case that an error should occur here is if the wait was
		 * awakened by a signal.
		 */

		while (sem_wait(&priv->exclsem) != OK) {
			DEBUGASSERT(errno == EINTR);
		}

	} else {
		(void)sem_post(&priv->exclsem);
#ifdef CONFIG_PM
		bsp_pm_domain_control(BSP_SPI_DRV, 0);
#endif
	}

	return OK;
}

/************************************************************************************
 * Name: amebasmart_spi_select
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
void amebasmart_spi_select(FAR struct spi_dev_s *dev, enum spi_dev_e devid, bool selected)
{
	FAR struct amebasmart_spidev_s *priv = (FAR struct amebasmart_spidev_s *)dev;
#ifdef CONFIG_SPI_CS
	if (priv->multi_cs)
	{
		if (selected == 1) {
			if (devid == 0){ 
				//Select cs0, unselect cs1
				gpio_write(&priv->gpio_cs0, 0);
				gpio_write(&priv->gpio_cs1, 1);
			} else {
				//Select cs1, unselect cs0
				gpio_write(&priv->gpio_cs0, 1);
				gpio_write(&priv->gpio_cs1, 0);

			}
		} else {
			if (devid == 0) {
				//Unselect cs0
				gpio_write(&priv->gpio_cs0, 1);
			} else {
				//Unselect cs1
				gpio_write(&priv->gpio_cs1, 1);
			}
		}
		return;
	}
#endif
	if (selected == 1) {
		gpio_write(&priv->gpio_cs0, 0);
	} else {
		gpio_write(&priv->gpio_cs0, 1);
	}
	return;
}

/************************************************************************************
 * Name: amebasmart_spi0status
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
uint8_t amebasmart_spi0status(FAR struct spi_dev_s *dev, enum spi_dev_e devid)
{
	return 0;
}

/************************************************************************************
 * Name: amebasmart_spi0cmddata
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
int amebasmart_spi0cmddata(FAR struct spi_dev_s *dev, uint32_t devid, bool cmd)
{
	return 0;
}

/************************************************************************************
 * Name: amebasmart_spi1status
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
uint8_t amebasmart_spi1status(FAR struct spi_dev_s *dev, enum spi_dev_e devid)
{
	return 0;
}

/************************************************************************************
 * Name: amebasmart_spi1cmddata
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
int amebasmart_spi1cmddata(FAR struct spi_dev_s *dev, uint32_t devid, bool cmd)
{
	return 0;
}

/************************************************************************************
 * Name: amebasmart_spi_setfrequency
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

static uint32_t amebasmart_spi_setfrequency(FAR struct spi_dev_s *dev,
					uint32_t frequency)
{
	FAR struct amebasmart_spidev_s *priv = (FAR struct amebasmart_spidev_s *)dev;

	/* Has the SPI bus frequency changed? */

	if (frequency != priv->frequency) {
		/* Disable SPI if it is enabled */

		priv->frequency = frequency;

		if (priv->role == AMEBASMART_SPI_MASTER)
			spi_frequency(&priv->spi_object, priv->frequency);
	}

	return priv->frequency;
}

/************************************************************************************
 * Name: amebasmart_spi_setmode
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

static void amebasmart_spi_setmode(FAR struct spi_dev_s *dev, enum spi_mode_e mode)
{
	FAR struct amebasmart_spidev_s *priv = (FAR struct amebasmart_spidev_s *)dev;

	spiinfo("mode=%d\n", mode);

	/* Has the mode changed? */

	if (mode != priv->mode) {
		/* Disable SPI if it is enabled */

		priv->mode = mode;
		spi_format(&priv->spi_object, priv->nbits, priv->mode, priv->role);

	}
}

/************************************************************************************
 * Name: amebasmart_spi_setbits
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

static void amebasmart_spi_setbits(FAR struct spi_dev_s *dev, int nbits)
{
	FAR struct amebasmart_spidev_s *priv = (FAR struct amebasmart_spidev_s *)dev;

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
 * Name: amebasmart_spi_hwfeatures
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
static int amebasmart_spi_hwfeatures(FAR struct spi_dev_s *dev,
				amebasmart_spi_hwfeatures_t features)
{
#ifdef CONFIG_SPI_BITORDER
	FAR struct amebasmart_spidev_s *priv = (FAR struct amebasmart_spidev_s *)dev;

	spiinfo("features=%08x\n", features);

	/* Other H/W features are not supported */
#endif
	return -ENOSYS;
}
#endif

/************************************************************************************
 * Name: amebasmart_spi_send
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

static uint16_t amebasmart_spi_send(FAR struct spi_dev_s *dev, uint16_t wd)
{
	FAR struct amebasmart_spidev_s *priv = (FAR struct amebasmart_spidev_s *)dev;

	uint16_t ret = 0;
	DEBUGASSERT(priv);

	if (priv->role == AMEBASMART_SPI_MASTER) {

		ret = spi_master_write(&priv->spi_object, wd);
	} else if (priv->role == AMEBASMART_SPI_SLAVE) {

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
 * Name: amebasmart_spi_exchange (no DMA).  aka amebasmart_spi_exchange_nodma
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




static void amebasmart_spi_exchange(FAR struct spi_dev_s *dev,
				FAR const void *txbuffer, FAR void *rxbuffer,
				size_t nwords)
{
	FAR struct amebasmart_spidev_s *priv = (FAR struct amebasmart_spidev_s *)dev;
	DEBUGASSERT(priv);

	spiinfo("txbuffer=%p rxbuffer=%p nwords=%d\n", txbuffer, rxbuffer, nwords);

	/* 8- or 16-bit mode? */
#if !defined(CONFIG_AMEBASMART_SPI_DMA)
	if (amebasmart_spi_9to16bitmode(priv)) {
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

			word = amebasmart_spi_send(dev, word);

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

			word = (uint8_t)amebasmart_spi_send(dev, (uint16_t) word);

			/* Is there a buffer to receive the return value? */

			if (dest) {
				*dest++ = word;
			}
		}
	}
#else
/*************************************************************************
 * Name: spi_exchange (with DMA)
 *
 * Description:
 *   Exchange a block of data on SPI using DMA
 *
 * Input Parameters:
 *   dev      - Device-specific state data
 *   txbuffer - A pointer to the buffer of data to be sent
 *   rxbuffer - A pointer to a buffer in which to receive data
 *   nwords   - the length of data to be exchanged in units of words.
 *              The wordsize is determined by the number of bits-per-word
 *              selected for the SPI interface.  If nbits <= 8, the data is
 *              packed into uint8_t's; if nbits >8, the data is packed into uint16_t's
 *
 * Returned Value:
 *   None
 *
 ************************************************************************************/
	/*if SPI DMA is called in idle thread, semaphore will cause issues, use polling method instead*/
	if(getpid() == 0) {
		if (amebasmart_spi_9to16bitmode(priv)) {
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

				word = amebasmart_spi_send(dev, word);

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

				word = (uint8_t)amebasmart_spi_send(dev, (uint16_t) word);

				/* Is there a buffer to receive the return value? */

				if (dest) {
					*dest++ = word;
				}
			}
		}

		return;
	}

	int mode_16bit = 1;
	uint32_t remain_data_len = 0;
	uint32_t send_data_len = 0;
	spi_hookmastercallback(priv);
	if (amebasmart_spi_9to16bitmode(priv)) {
		/* 16-bit mode */
		mode_16bit++;
	}
	remain_data_len = nwords * mode_16bit;
	uint8_t *rxbuff_aligned = NULL;
	uint8_t *txbuff_aligned =  NULL;
	rxbuff_aligned = (uint8_t *)rtos_mem_zmalloc(SPI_DMA_MAX_BUFFER_SIZE);
	if(rxbuff_aligned == NULL) {
		lldbg("rxbuff_aligned malloc failed\n");
		return;
	}
	if (txbuffer) {
		txbuff_aligned = (uint8_t *)rtos_mem_zmalloc(SPI_DMA_MAX_BUFFER_SIZE);
		if(txbuff_aligned == NULL) {
			rtos_mem_free(rxbuff_aligned);
			lldbg("txbuff_aligned malloc failed\n");
			return;
		}
	}
	while (remain_data_len > 0) {
		if (remain_data_len >= SPI_DMA_MAX_BUFFER_SIZE) {
			send_data_len = SPI_DMA_MAX_BUFFER_SIZE;
		} else {
			send_data_len = remain_data_len;
		}
		if (txbuffer && rxbuffer) {
			memcpy(txbuff_aligned, txbuffer, send_data_len);
			spi_master_write_read_stream_dma(&priv->spi_object, (char *)txbuff_aligned, (char *)rxbuff_aligned, (uint32_t)send_data_len);
			spi_dmarxwait(priv);
			spi_dmatxwait(priv);
			memcpy(rxbuffer, rxbuff_aligned, send_data_len);
		} else if (txbuffer) {
			memcpy(txbuff_aligned, txbuffer, send_data_len);
			spi_master_write_read_stream_dma(&priv->spi_object, (char *)txbuff_aligned, (char *)rxbuff_aligned, (uint32_t)send_data_len);
			spi_dmatxwait(priv);
			spi_dmarxwait(priv);
		} else if (rxbuffer) {
			spi_flush_rx_fifo(&priv->spi_object);
			spi_master_read_stream_dma(&priv->spi_object, (char *) rxbuff_aligned, (uint32_t)send_data_len);
			spi_dmarxwait(priv);
			memcpy(rxbuffer, rxbuff_aligned, send_data_len);
		}
		remain_data_len -= send_data_len;
		if (txbuffer) {
			txbuffer += send_data_len;
		}
		if (rxbuffer) {
			rxbuffer += send_data_len;
		}
	}
	if (txbuff_aligned) {
		rtos_mem_free(txbuff_aligned);
	}
	rtos_mem_free(rxbuff_aligned);
#endif							/* CONFIG_AMEBASMART_SPI_DMA */
}

/****************************************************************************
 * Name: amebasmart_spi_sndblock
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
static void amebasmart_spi_sndblock(FAR struct spi_dev_s *dev,
				FAR const void *txbuffer, size_t nwords)
{
	spiinfo("txbuffer=%p nwords=%d\n", txbuffer, nwords);
	return amebasmart_spi_exchange(dev, txbuffer, NULL, nwords);
}
#endif

/************************************************************************************
 * Name: amebasmart_spi_recvblock
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
static void amebasmart_spi_recvblock(FAR struct spi_dev_s *dev, FAR void *rxbuffer,
				size_t nwords)
{
	spiinfo("rxbuffer=%p nwords=%d\n", rxbuffer, nwords);
	return amebasmart_spi_exchange(dev, NULL, rxbuffer, nwords);
}
#endif

/************************************************************************************
 * Name: amebasmart_spi_clock_enable
 *
 * Description:
 *   Ungate SPI clock
 *
 ************************************************************************************/

void amebasmart_spi_clock_enable(uint32_t base)
{
	return;
}

/************************************************************************************
 * Name: amebasmart_spi_clock_disable
 *
 * Description:
 *   Gate SPI clock
 *
 ************************************************************************************/

void amebasmart_spi_clock_disable(uint32_t base)
{
	return;
}

/************************************************************************************
 * Name: amebasmart_spi_bus_initialize
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

static void amebasmart_spi_bus_initialize(struct amebasmart_spidev_s *priv, uint8_t multi_cs)
{

	DEBUGASSERT(priv);
	DEBUGASSERT(&priv->spi_object);

	priv->spi_object.spi_idx = priv->spi_idx;
	priv->multi_cs = multi_cs;

	spi_init(&priv->spi_object, priv->spi_mosi, priv->spi_miso, priv->spi_sclk, priv->spi_cs0);
	spi_format(&priv->spi_object, priv->nbits, priv->mode, priv->role);
	sem_init(&priv->exclsem, 0, 1);
#ifdef CONFIG_AMEBASMART_SPI_DMA
	sem_init(&priv->rxsem, 0, 0);
	sem_init(&priv->txsem, 0, 0);
#endif
	gpio_init(&priv->gpio_cs0, priv->spi_cs0);
	gpio_dir(&priv->gpio_cs0, PIN_OUTPUT);
	gpio_mode(&priv->gpio_cs0, PullNone);
	gpio_write(&priv->gpio_cs0, 1);
#ifdef CONFIG_SPI_CS
	if (priv->multi_cs)
	{
		gpio_init(&priv->gpio_cs1, priv->spi_cs1);
		gpio_dir(&priv->gpio_cs1, PIN_OUTPUT);
		gpio_mode(&priv->gpio_cs1, PullNone);
		gpio_write(&priv->gpio_cs1, 1);
	}
#endif
}

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: amebasmart_spibus_initialize
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

FAR struct spi_dev_s *amebasmart_spibus_initialize(int bus)
{
	FAR struct amebasmart_spidev_s *priv = NULL;

	irqstate_t flags = enter_critical_section();
#ifdef CONFIG_AMEBASMART_SPI0	
	if (bus == 0) {
		/* Select SPI0 */

		priv = &g_spi0dev;

		/* Only configure if the bus is not already configured */
		/* Checks for Multislaves feature */
#if defined(CONFIG_SPI_CS) && defined(CONFIG_AMEBASMART_SPI0_CS)
		amebasmart_spi_bus_initialize(priv, 1);
#else
		amebasmart_spi_bus_initialize(priv, 0);
#endif

	} else
#endif
#ifdef CONFIG_AMEBASMART_SPI1
	if (bus == 1) {
		/* Select SPI1 */

		priv = &g_spi1dev;

		/* Only configure if the bus is not already configured */

#if defined(CONFIG_SPI_CS) && defined(CONFIG_AMEBASMART_SPI1_CS)
		amebasmart_spi_bus_initialize(priv, 1);
#else
		amebasmart_spi_bus_initialize(priv, 0);
#endif

	} else 
#endif
	{
		spierr("ERROR: Unsupported SPI bus: %d\n", bus);
		leave_critical_section(flags);
		return NULL;
	}

	leave_critical_section(flags);

	return (FAR struct spi_dev_s *)priv;
}

/****************************************************************************
 * Name: rtk_spi_suspend/resume
 *
 * Description:
 *   Suspend or resume spi peripherals for/from sleep modes.
 *
 ****************************************************************************/

#ifdef CONFIG_PM
static uint32_t rtk_spi_suspend(uint32_t expected_idle_time, void *param)
{
	(void)expected_idle_time;
	(void)param;
	/* Is there anything need to be done here? Unregister something...? */
#ifdef CONFIG_AMEBASMART_SPI0
	sem_destroy(&g_spi0dev.exclsem);
	g_spi0dev.refs--;
#ifdef CONFIG_AMEBASMART_SPI_DMA
	sem_destroy(&g_spi0dev.txsem);
	sem_destroy(&g_spi0dev.rxsem);
#endif
#endif
#ifdef CONFIG_AMEBASMART_SPI1
	sem_destroy(&g_spi1dev.exclsem);
	g_spi1dev.refs--;
#ifdef CONFIG_AMEBASMART_SPI_DMA
	sem_destroy(&g_spi1dev.txsem);
	sem_destroy(&g_spi1dev.rxsem);
#endif
#endif
	return 1;
}

static uint32_t rtk_spi_resume(uint32_t expected_idle_time, void *param)
{
	(void)expected_idle_time;
	(void)param;
	/* For PG Sleep, SPI HW will be lost power, thus a reinitialization is required here */
#ifdef CONFIG_AMEBASMART_SPI0
	(void) up_spiinitialize(0);
#endif
#ifdef CONFIG_AMEBASMART_SPI1
	(void) up_spiinitialize(1);
#endif
	return 1;
}
#endif

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
	FAR struct amebasmart_spidev_s *priv = NULL;

	irqstate_t flags = enter_critical_section();
#ifdef CONFIG_AMEBASMART_SPI0
	if (port == 0) {
		/* Select SPI0 */

		priv = &g_spi0dev;

		if (priv->refs > 0) {
			dbg("SPI port%d has been initialized before!\n", port);
			leave_critical_section(flags);
			return (FAR struct spi_dev_s *)priv;
		}
		/* Only configure if the bus is not already configured */
		/* Checks for Multislaves feature */
#if defined(CONFIG_SPI_CS) && defined(CONFIG_AMEBASMART_SPI0_CS)
		amebasmart_spi_bus_initialize(priv, 1);
#else
		amebasmart_spi_bus_initialize(priv, 0);
#endif
	}
	else 
#endif
#ifdef CONFIG_AMEBASMART_SPI1
	if (port == 1) {
		/* Select SPI1 */

		priv = &g_spi1dev;

		if (priv->refs > 0) {
			dbg("SPI port%d has been initialized before!\n", port);
			leave_critical_section(flags);
			return (FAR struct spi_dev_s *)priv;
		}
		/* Only configure if the bus is not already configured */
#if defined(CONFIG_SPI_CS) && defined(CONFIG_AMEBASMART_SPI1_CS)
		amebasmart_spi_bus_initialize(priv, 1);
#else
		amebasmart_spi_bus_initialize(priv, 0);
#endif
	} else
#endif
	{
		spierr("ERROR: Unsupported SPI bus: %d\n", port);
		leave_critical_section(flags);
		return NULL;
	}

	priv->refs++;
	leave_critical_section(flags);

	return (FAR struct spi_dev_s *)priv;
}

#ifdef CONFIG_PM
void spi_pminitialize(void)
{
	/* Domain registration for operation control */
	bsp_pm_domain_register("SPI", BSP_SPI_DRV);
	pmu_register_sleep_callback(PMU_SPI_DEVICE, (PSM_HOOK_FUN)rtk_spi_suspend, NULL, (PSM_HOOK_FUN)rtk_spi_resume, NULL);
}
#endif
