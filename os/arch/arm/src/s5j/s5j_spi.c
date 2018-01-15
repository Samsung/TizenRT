/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/s5j/s5j_spi.c
 *
 *   Copyright (C) 2013 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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

/*****************************************************************************
 * Included Files
 *****************************************************************************/
#include <tinyara/config.h>

#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <errno.h>

#include <tinyara/arch.h>
#include <tinyara/spi/spi.h>

#include "up_arch.h"
#include <s5j_spi.h>
#include <string.h>
#include <assert.h>
#include <debug.h>
#include <tinyara/irq.h>
#include <arch/irq.h>
#include <arch/board/board.h>
#include <tinyara/kmalloc.h>
#include <poll.h>
#include <tinyara/fs/fs.h>
#include <stddef.h>
#include <chip.h>

#include "chip/s5jt200_spi.h"
#include "s5j_clock.h"

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct s5j_spidev_s {
	struct spi_dev_s spidev;
	uint32_t base;
	uint8_t port;
	unsigned int freqid;
	s32 gpio_clk;
	s32 gpio_nss;
	s32 gpio_miso;
	s32 gpio_mosi;

#ifndef CONFIG_SPI_POLLWAIT
	sem_t xfrsem;				/* Wait for transfer to complete */
#endif

#ifndef CONFIG_SPI_OWNBUS
	sem_t exclsem;
#endif
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static uint32_t spi_getreg32(struct s5j_spidev_s *priv, int offset)
{
	return getreg32(priv->base + offset);
}

static void spi_putreg32(struct s5j_spidev_s *priv, int offset, uint32_t value)
{
	putreg32(value, priv->base + offset);
}

static void spi_modifyreg32(struct s5j_spidev_s *priv, int offset,
				uint32_t clearbits, uint32_t setbits)
{
	modifyreg32(priv->base + offset, clearbits, setbits);
}

static uint32_t spi_getTXFIFO_lv(struct s5j_spidev_s *priv)
{
	return ((spi_getreg32(priv, S5J_SPI_STATUS_OFFSET) >>
			SPI_STATUS_TX_FIFO_LVL_SHIFT) & SPI_STATUS_TX_FIFO_LVL_MASK);
}

static uint32_t spi_getRXFIFO_lv(struct s5j_spidev_s *priv)
{
	return ((spi_getreg32(priv, S5J_SPI_STATUS_OFFSET) >>
			SPI_STATUS_RX_FIFO_LVL_SHIFT) & SPI_STATUS_RX_FIFO_LVL_MASK);
}

/****************************************************************************
 * Name: spi_lock
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
 ****************************************************************************/
static int spi_lock(struct spi_dev_s *dev, bool lock)
{
	FAR struct s5j_spidev_s *priv = (FAR struct s5j_spidev_s *)dev;

	if (lock) {
		while (sem_wait(&priv->exclsem) != 0) {
			DEBUGASSERT(errno == EINTR);
		}
	} else {
		(void)sem_post(&priv->exclsem);
	}

	return OK;
}

/****************************************************************************
 * Name: spi_select
 *
 * Description:
 *   Enable/disable the SPI slave select.   The implementation of this method
 *   must include handshaking:  If a device is selected, it must hold off
 *   all other attempts to select the device until the device is deselecte.
 *
 ****************************************************************************/
static void spi_select(struct spi_dev_s *dev, enum spi_dev_e devid, bool selected)
{
	FAR struct s5j_spidev_s *priv = (FAR struct s5j_spidev_s *)dev;
	unsigned int cs_reg;

	cs_reg = spi_getreg32(priv, S5J_SPI_CS_REG_OFFSET);
	cs_reg |= SPI_CS_REG_NSSOUT_OFF;

	if (selected == TRUE) {
		cs_reg &= ~SPI_CS_REG_NSSOUT_OFF;
	}

	spi_putreg32(priv, S5J_SPI_CS_REG_OFFSET, cs_reg);
}

/****************************************************************************
 * Name: spi_setfrequency
 *
 * Description:
 *   Set the SPI frequency.
 *
 ****************************************************************************/
static uint32_t spi_setfrequency(struct spi_dev_s *dev, uint32_t frequency)
{
	FAR struct s5j_spidev_s *priv = (FAR struct s5j_spidev_s *)dev;

	s5j_clk_set_rate(priv->freqid, (unsigned long)frequency);

	return OK;
}

/****************************************************************************
 * Name: spi_setmode
 *
 * Description:
 *   Set the SPI mode.  see enum spi_mode_e for mode definitions
 *
 ****************************************************************************/
static void spi_setmode(struct spi_dev_s *dev, enum spi_mode_e mode)
{
	FAR struct s5j_spidev_s *priv = (FAR struct s5j_spidev_s *)dev;

	switch (mode) {
	default:
	case SPIDEV_MODE0:
		spi_modifyreg32(priv, S5J_SPI_CH_CFG_OFFSET,
						SPI_CH_CFG_CPHA_MASK | SPI_CH_CFG_CPOL_MASK,
						SPI_CH_CFG_CPHA_FORMATA | SPI_CH_CFG_CPOL_HIGH);
		break;
	case SPIDEV_MODE1:
		spi_modifyreg32(priv, S5J_SPI_CH_CFG_OFFSET,
						SPI_CH_CFG_CPHA_MASK | SPI_CH_CFG_CPOL_MASK,
						SPI_CH_CFG_CPHA_FORMATA | SPI_CH_CFG_CPOL_LOW);
		break;
	case SPIDEV_MODE2:
		spi_modifyreg32(priv, S5J_SPI_CH_CFG_OFFSET,
						SPI_CH_CFG_CPHA_MASK | SPI_CH_CFG_CPOL_MASK,
						SPI_CH_CFG_CPHA_FORMATB | SPI_CH_CFG_CPOL_HIGH);
		break;
	case SPIDEV_MODE3:
		spi_modifyreg32(priv, S5J_SPI_CH_CFG_OFFSET,
						SPI_CH_CFG_CPHA_MASK | SPI_CH_CFG_CPOL_MASK,
						SPI_CH_CFG_CPHA_FORMATB | SPI_CH_CFG_CPOL_LOW);
		break;
	}
}

/****************************************************************************
 * Name: spi_setbits
 *
 * Description:
 *   Set the number of bits per word.
 *
 ****************************************************************************/
static void spi_setbits(struct spi_dev_s *dev, int nbits)
{
	FAR struct s5j_spidev_s *priv = (FAR struct s5j_spidev_s *)dev;

	switch (nbits) {
	case 8:
		spi_modifyreg32(priv, S5J_SPI_MODE_CFG_OFFSET,
						SPI_MODE_CFG_BUS_WIDTH_MASK |
						SPI_MODE_CFG_CH_WIDTH_MASK,
						SPI_MODE_CFG_BUS_WIDTH_8 |
						SPI_MODE_CFG_CH_WIDTH_8);
		break;
	case 16:
		spi_modifyreg32(priv, S5J_SPI_MODE_CFG_OFFSET,
						SPI_MODE_CFG_BUS_WIDTH_MASK |
						SPI_MODE_CFG_CH_WIDTH_MASK,
						SPI_MODE_CFG_BUS_WIDTH_16 |
						SPI_MODE_CFG_CH_WIDTH_16);
		break;
	case 32:
		spi_modifyreg32(priv, S5J_SPI_MODE_CFG_OFFSET,
						SPI_MODE_CFG_BUS_WIDTH_MASK |
						SPI_MODE_CFG_CH_WIDTH_MASK,
						SPI_MODE_CFG_BUS_WIDTH_32 |
						SPI_MODE_CFG_CH_WIDTH_32);
		break;
	default:
		DEBUGASSERT(0 == 1);
		break;
	}
}

/****************************************************************************
 * Name: spi_exchange
 *
 * Description:
 *   Exchange a block data with the SPI device. Support only byte transfers.
 *
 ****************************************************************************/
static void spi_exchange(struct spi_dev_s *dev, const void *txbuffer, void *rxbuffer, size_t nwords)
{
	FAR struct s5j_spidev_s *priv = (FAR struct s5j_spidev_s *)dev;

	size_t sent = 0;
	size_t received = 0;
	unsigned int dummy_rx;

	/* SPI FIFO FLUSH */
	int ch_cfg = spi_getreg32(priv, S5J_SPI_CH_CFG_OFFSET);

	spi_putreg32(priv, S5J_SPI_CH_CFG_OFFSET, SPI_CH_CFG_FIFO_FLUSH_ON);
	spi_putreg32(priv, S5J_SPI_CH_CFG_OFFSET, ch_cfg);

	/* TX/RX */
	if ((rxbuffer == NULL) && (txbuffer == NULL)) {
		while (received < nwords) {
			if (sent < nwords) {
				if (spi_getTXFIFO_lv(priv) < 64) {
					spi_putreg32(priv, S5J_SPI_TX_DATA_OFFSET, 0);
					sent++;
				}
			}

			if (spi_getRXFIFO_lv(priv) > 0) {
				dummy_rx = spi_getreg32(priv, S5J_SPI_RX_DATA_OFFSET);
				received++;
			}
		}
		return;
	}

	if (rxbuffer == NULL) {
		while (received < nwords) {
			if (sent < nwords) {
				if (spi_getTXFIFO_lv(priv) < 64) {
					spi_putreg32(priv, S5J_SPI_TX_DATA_OFFSET, ((unsigned char *)txbuffer)[sent++]);
				}
			}

			if (spi_getRXFIFO_lv(priv) > 0) {
				dummy_rx = spi_getreg32(priv, S5J_SPI_RX_DATA_OFFSET);
				received++;
			}
		}
		return;
	}

	if (txbuffer == NULL) {
		while (received < nwords) {
			if (sent < nwords) {
				if (spi_getTXFIFO_lv(priv) < 64) {
					spi_putreg32(priv, S5J_SPI_TX_DATA_OFFSET, ((unsigned char *)rxbuffer)[sent++]);
				}
			}

			if (spi_getRXFIFO_lv(priv) > 0) {
				((unsigned char *)rxbuffer)[received++] = spi_getreg32(priv, S5J_SPI_RX_DATA_OFFSET);
			}

		}
		return;
	}

	while (received < nwords) {
		if (sent < nwords) {
			if (spi_getTXFIFO_lv(priv) < 64) {
				spi_putreg32(priv, S5J_SPI_TX_DATA_OFFSET, ((unsigned char *)txbuffer)[sent++]);
			}
		}

		if (spi_getRXFIFO_lv(priv) > 0) {
			((unsigned char *)rxbuffer)[received++] = spi_getreg32(priv, S5J_SPI_RX_DATA_OFFSET);
		}

	}
}

/****************************************************************************
 * Name: spi_send
 *
 * Description:
 *   Exchange one word on SPI. Currently support only byte transfers.
 *
 ****************************************************************************/
static uint16_t spi_send(struct spi_dev_s *dev, uint16_t wd)
{
	uint8_t txbyte;
	uint8_t rxbyte;

	txbyte = (uint8_t)wd;
	rxbyte = (uint8_t)0;
	spi_exchange(dev, &txbyte, &rxbyte, 1);

	return (uint16_t)rxbyte;
}

#ifndef CONFIG_SPI_EXCHANGE
/****************************************************************************
 * Name: spi_sndblock
 *
 * Description:
 *   Send a block of data on SPI. Support only byte transfers
 *
 ****************************************************************************/
static void spi_sndblock(struct spi_dev_s *dev, const void *txbuffer, size_t nwords)
{
	spi_exchange(dev, txbuffer, NULL, nwords);
}

/****************************************************************************
 * Name: spi_recvblock
 *
 * Description:
 *   Revice a block of data from SPI. Support only byte transfers.
 *
 ****************************************************************************/
static void spi_recvblock(struct spi_dev_s *dev, void *rxbuffer, size_t nwords)
{
	spi_exchange(dev, NULL, rxbuffer, nwords);
}
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct spi_ops_s g_spiops = {
#ifndef CONFIG_SPI_OWNBUS
	.lock				= spi_lock,
#endif
	.select				= spi_select,
	.setfrequency		= spi_setfrequency,
	.setmode			= (void *)spi_setmode,
	.setbits			= (void *)spi_setbits,
	.status				= 0,
#ifdef CONFIG_SPI_CMDDATA
	.cmddata			= 0,
#endif
	.send				= spi_send,
#ifdef CONFIG_SPI_EXCHANGE
	.exchange			= spi_exchange,
#else
	.sndblock			= spi_sndblock,
	.recvblock			= spi_recvblock,
#endif
	.registercallback	= 0,
};

static struct s5j_spidev_s g_spi0dev = {
	.spidev		= { .ops = &g_spiops },
	.base		= S5J_SPI0_BASE,
	.port		= SPI_PORT0,
	.freqid		= CLK_SPL_SPI0,
	.gpio_clk	= GPIO_SPI0_CLK,
	.gpio_nss	= GPIO_SPI0_CS,
	.gpio_miso	= GPIO_SPI0_MISO,
	.gpio_mosi	= GPIO_SPI0_MOSI,
};
static struct s5j_spidev_s g_spi1dev = {
	.spidev		= { .ops = &g_spiops },
	.base		= S5J_SPI1_BASE,
	.port		= SPI_PORT1,
	.freqid		= CLK_SPL_SPI1,
	.gpio_clk	= GPIO_SPI1_CLK,
	.gpio_nss	= GPIO_SPI1_CS,
	.gpio_miso	= GPIO_SPI1_MISO,
	.gpio_mosi	= GPIO_SPI1_MOSI,
};
static struct s5j_spidev_s g_spi2dev = {
	.spidev		= { .ops = &g_spiops },
	.base		= S5J_SPI2_BASE,
	.port		= SPI_PORT2,
	.freqid		= CLK_SPL_SPI2,
	.gpio_clk	= GPIO_SPI2_CLK,
	.gpio_nss	= GPIO_SPI2_CS,
	.gpio_miso	= GPIO_SPI2_MISO,
	.gpio_mosi	= GPIO_SPI2_MOSI,
};
static struct s5j_spidev_s g_spi3dev = {
	.spidev		= { .ops = &g_spiops },
	.base		= S5J_SPI3_BASE,
	.port		= SPI_PORT3,
	.freqid		= CLK_SPL_SPI3,
	.gpio_clk	= GPIO_SPI3_CLK,
	.gpio_nss	= GPIO_SPI3_CS,
	.gpio_miso	= GPIO_SPI3_MISO,
	.gpio_mosi	= GPIO_SPI3_MOSI,
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_spiinitialize
 *
 * Description:
 *   Initialize the selected SPI port
 *
 ****************************************************************************/
struct spi_dev_s *up_spiinitialize(int port)
{
	FAR struct s5j_spidev_s *priv = NULL;

	if (port >= SPI_PORT_MAX) {
		return NULL;
	}

	switch (port) {
	case SPI_PORT0:
		priv = &g_spi0dev;
		break;
	case SPI_PORT1:
		priv = &g_spi1dev;
		break;
	case SPI_PORT2:
		priv = &g_spi2dev;
		break;
	case SPI_PORT3:
		priv = &g_spi3dev;
		break;
	}
	lldbg("Prepare SPI%d for Master operation\n", priv->port);

	/* SET GPIO for the port */
	VERIFY(s5j_configgpio(priv->gpio_clk) == OK);
	VERIFY(s5j_configgpio(priv->gpio_nss) == OK);
	VERIFY(s5j_configgpio(priv->gpio_miso) == OK);
	VERIFY(s5j_configgpio(priv->gpio_mosi) == OK);

#ifndef CONFIG_SPI_POLLWAIT
	sem_init(&priv->xfrsem, 0, 0);
#endif

#ifndef CONFIG_SPI_OWNBUS
	sem_init(&priv->exclsem, 0, 1);
#endif

	/* SET SPI INITIAL */

	/* TX/RX enable. Master.CPHA 00. */
	spi_setmode((struct spi_dev_s *)priv, SPIDEV_MODE0);
	spi_modifyreg32(priv, S5J_SPI_CH_CFG_OFFSET,
					SPI_CH_CFG_TX_CH_MASK |
					SPI_CH_CFG_RX_CH_MASK |
					SPI_CH_CFG_SLAVE_MASK |
					SPI_CH_CFG_FIFO_FLUSH_MASK |
					SPI_CH_CFG_HIGH_SPEED_MASK,
					SPI_CH_CFG_TX_CH_ON |
					SPI_CH_CFG_RX_CH_ON |
					SPI_CH_CFG_MASTER |
					SPI_CH_CFG_FIFO_FLUSH_OFF |
					SPI_CH_CFG_HIGH_SPEED_DIS);

	/*  No FIFO. N0 DMA. 8 bits */
	spi_modifyreg32(priv, S5J_SPI_MODE_CFG_OFFSET,
					SPI_MODE_CFG_DMA_TYPE_MASK |
					SPI_MODE_CFG_DMA_TX_MASK |
					SPI_MODE_CFG_DMA_RX_MASK |
					SPI_MODE_CFG_TX_RDY_MASK |
					SPI_MODE_CFG_RX_RDY_MASK |
					SPI_MODE_CFG_BUS_WIDTH_MASK |
					SPI_MODE_CFG_TRAILING_MASK |
					SPI_MODE_CFG_CH_WIDTH_MASK,
					SPI_MODE_CFG_DMA_SINGLE |
					SPI_MODE_CFG_DMA_TX_OFF |
					SPI_MODE_CFG_DMA_RX_OFF |
					SPI_MODE_CFG_TX_RDY_LVL(0) |
					SPI_MODE_CFG_RX_RDY_LVL(0) |
					SPI_MODE_CFG_BUS_WIDTH_8 |
					SPI_MODE_CFG_TRAILING_CNT(0) |
					SPI_MODE_CFG_CH_WIDTH_8);

	/* CS Manual Passive */
	spi_modifyreg32(priv, S5J_SPI_CS_REG_OFFSET,
					SPI_CS_REG_NSSOUT_MASK |
					SPI_CS_REG_MODE_MASK |
					SPI_CS_REG_NCS_TIME_MASK,
					SPI_CS_REG_NSSOUT_OFF |
					SPI_CS_REG_MODE_MANUAL |
					SPI_CS_REG_NCS_TIME_COUNT(0));

	/* Disable Interrupts */
	spi_putreg32(priv, S5J_SPI_INT_EN_OFFSET, 0);

	return (struct spi_dev_s *)priv;
}
