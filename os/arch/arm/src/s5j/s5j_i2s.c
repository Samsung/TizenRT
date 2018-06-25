/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/s5j/s5j_i2s.c
 *
 *   Copyright (C) 2013-2014, 2016 Gregory Nutt. All rights reserved.
 *   Authors: Gregory Nutt <gnutt@nuttx.org>
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <errno.h>
#include <assert.h>
#include <queue.h>
#include <debug.h>

#include <arch/board/board.h>
#include <arch/chip/i2s.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/kmalloc.h>
#include <tinyara/wdog.h>
#include <tinyara/wqueue.h>
#include <tinyara/audio/audio.h>
#include <tinyara/audio/i2s.h>

#include "up_internal.h"
#include "up_arch.h"
#include "cache.h"

#include "chip.h"
#include "chip/s5jt200_i2s.h"
#include "s5j_dma.h"
#include "s5j_i2s.h"
#include "s5j_clock.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/

#ifndef CONFIG_SCHED_WORKQUEUE
#error Work queue support is required (CONFIG_SCHED_WORKQUEUE)
#endif

#ifndef CONFIG_AUDIO
#error CONFIG_AUDIO required by this driver
#endif

#define S5J_I2S_MAXPORTS 1

#ifndef CONFIG_S5J_I2S_MAXINFLIGHT
#define CONFIG_S5J_I2S_MAXINFLIGHT 16
#endif

/* Assume no RX/TX support until we learn better */

#undef I2S_HAVE_RX
#undef I2S_HAVE_TX_P
#undef I2S_HAVE_TX_S

#if defined(CONFIG_S5J_I2S)

/* The I2S can handle most any bit width from 2 to 32.  However, the DMA
 * logic here is constrained to byte, half-word, and word sizes.
 */

/* Check for I2S RX support */

#if defined(CONFIG_S5J_I2S_RX)
#define I2S_HAVE_RX 1
#endif

/* Check for I2S TX support */

#if defined(CONFIG_S5J_I2S_TX_P)
#define I2S_HAVE_TX_P 1
#endif

#if defined(CONFIG_S5J_I2S_TX_S)
#define I2S_HAVE_TX_S 1
#endif

#ifndef CONFIG_S5J_I2S_DATALEN
#define CONFIG_S5J_I2S_DATALEN 16
#endif

#ifndef CONFIG_S5J_I2S_SAMPLERATE
#define CONFIG_S5J_I2S_SAMPLERATE 1
#endif

#endif

#ifdef CONFIG_DEBUG
#define CONFIG_DEBUG_FEATURES
#endif

/* DMA configuration */

#if !defined(CONFIG_I2S_TXP_DMACH) || !defined(CONFIG_I2S_TXS_DMACH) || !defined(CONFIG_I2S_RX_DMACH)
#undef CONFIG_I2S_TXP_DMACH
#undef CONFIG_I2S_TXS_DMACH
#undef CONFIG_I2S_RX_DMACH

#define CONFIG_I2S_TXP_DMACH	0
#define CONFIG_I2S_TXS_DMACH	1
#define CONFIG_I2S_RX_DMACH	2

#endif

#if defined(I2S_HAVE_RX) || defined(I2S_HAVE_TX_P) || defined(I2S_HAVE_TX_S)

/****************************************************************************
 * Private Types
 ****************************************************************************/
/* I2S buffer container */

struct s5j_buffer_s {
	struct s5j_buffer_s *flink;	/* Supports a singly linked list */
	i2s_callback_t callback;	/* Function to call when the transfer completes */
	dma_task *dmatask;		/* DMA transfer task structure */
	uint32_t timeout;		/* The timeout value to use with DMA transfers */
	void *arg;			/* The argument to be returned with the callback */
	struct ap_buffer_s *apb;	/* The audio buffer */
	int result;			/* The result of the transfer */
};

/* This structure describes the state of one receiver or transmitter transport */

struct s5j_transport_s {
	DMA_HANDLE dma;				/* I2S DMA handle */
	WDOG_ID dog;				/* Watchdog that handles DMA timeouts */
	sq_queue_t pend;			/* A queue of pending transfers */
	sq_queue_t act;				/* A queue of active transfers */
	sq_queue_t done;			/* A queue of completed transfers */
	struct work_s work;			/* Supports worker thread operations */
	int error_state;			/* Channel error state, 0 - OK*/
};

/* The state of the one I2S peripheral */

struct s5j_i2s_s {
	struct i2s_dev_s dev;			/* Externally visible I2S interface */
	uintptr_t base;				/* I2S controller register base address */
	int isr_num;				/* isr number */
	xcpt_t isr_handler;			/* irs handler */

	i2s_err_cb_t err_cb;			/* registered error callback function */
	void *err_cb_arg;			/* argiment to return with err cb call */

	sem_t exclsem;				/* Assures mutually exclusive acess to I2S */
	uint8_t datalen;			/* Data width (8, 16, or 32) */
	uint8_t rx_datalen;			/* Data width (8, 16, or 32) */
	uint8_t txp_datalen;			/* Data width (8, 16, or 32) */
	uint8_t txs_datalen;			/* Data width (8, 16, or 32) */

	uint8_t rxenab:1;			/* True: RX transfers enabled */
	uint8_t txpenab:1;			/* True: TX primary transfers enabled */
	uint8_t txsenab:1;			/* True: TX secondary transfers enabled */

	uint32_t samplerate;			/* Not actually needed in slave mode */

#ifdef I2S_HAVE_RX
	struct s5j_transport_s rx;		/* RX transport state */
#endif
#ifdef I2S_HAVE_TX_P
	struct s5j_transport_s txp;		/* TX primary transport state */
#endif
#ifdef I2S_HAVE_TX_S
	struct s5j_transport_s txs;		/* TX secodary transport state */
#endif

	/* Pre-allocated pool of buffer containers */

	sem_t bufsem_tx;			/* Buffer wait semaphore */
	struct s5j_buffer_s *freelist_tx;	/* A list a free buffer containers */
	struct s5j_buffer_s containers_tx[CONFIG_S5J_I2S_MAXINFLIGHT];

	sem_t bufsem_rx;			/* Buffer wait semaphore */
	struct s5j_buffer_s *freelist_rx;	/* A list a free buffer containers */
	struct s5j_buffer_s containers_rx[CONFIG_S5J_I2S_MAXINFLIGHT];
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* Register helpers */
//#define CONFIG_S5J_I2S_DUMPBUFFERS

#ifdef CONFIG_S5J_I2S_DUMPBUFFERS
#define       i2s_init_buffer(b, s)   memset(b, 0x55, s);
#define       i2s_dump_buffer(m, b, s) lib_dumpbuffer(m, b, s)
#else
#define       i2s_init_buffer(b, s)
#define       i2s_dump_buffer(m, b, s)
#endif

/* Semaphore helpers */

static void i2s_exclsem_take(struct s5j_i2s_s *priv);
#define         i2s_exclsem_give(priv) sem_post(&priv->exclsem)

static void i2s_bufsem_rx_take(struct s5j_i2s_s *priv);
#define         i2s_bufsem_rx_give(priv) sem_post(&priv->bufsem_rx)

static void i2s_bufsem_tx_take(struct s5j_i2s_s *priv);
#define         i2s_bufsem_tx_give(priv) sem_post(&priv->bufsem_tx)

/* Buffer container helpers */

static struct s5j_buffer_s *i2s_buf_rx_allocate(struct s5j_i2s_s *priv);
static void i2s_buf_rx_free(struct s5j_i2s_s *priv, struct s5j_buffer_s *bfcontainer);
static void i2s_buf_rx_initialize(struct s5j_i2s_s *priv);

static struct s5j_buffer_s *i2s_buf_tx_allocate(struct s5j_i2s_s *priv);
static void i2s_buf_tx_free(struct s5j_i2s_s *priv, struct s5j_buffer_s *bfcontainer);
static void i2s_buf_tx_initialize(struct s5j_i2s_s *priv);

#ifdef I2S_HAVE_RX
static void i2s_rxdma_timeout(int argc, uint32_t arg);
static int i2s_rx_start(struct s5j_i2s_s *priv);
static void i2s_rx_worker(void *arg);
static void i2s_rx_schedule(struct s5j_i2s_s *priv, int result);
static void i2s_rxdma_callback(DMA_HANDLE handle, void *arg, int result);
#endif
#ifdef I2S_HAVE_TX_P
static void i2s_txpdma_timeout(int argc, uint32_t arg);
static int i2s_txp_start(struct s5j_i2s_s *priv);
static void i2s_txp_worker(void *arg);
static void i2s_txp_schedule(struct s5j_i2s_s *priv, int result);
static void i2s_txpdma_callback(DMA_HANDLE handle, void *arg, int result);
#endif

/* I2S methods (and close friends) */

static int i2s_checkwidth(struct s5j_i2s_s *priv, int bits);

static uint32_t i2s_rxdatawidth(struct i2s_dev_s *dev, int bits);
static int i2s_receive(struct i2s_dev_s *dev, struct ap_buffer_s *apb, i2s_callback_t callback, void *arg, uint32_t timeout);

static uint32_t i2s_samplerate(struct i2s_dev_s *dev, uint32_t rate);

static uint32_t i2s_txdatawidth(struct i2s_dev_s *dev, int bits);
static int i2s_send(struct i2s_dev_s *dev, struct ap_buffer_s *apb, i2s_callback_t callback, void *arg, uint32_t timeout);
static int i2s_stop(struct i2s_dev_s *dev, i2s_ch_dir_t dir);
static int i2s_pause(struct i2s_dev_s *dev, i2s_ch_dir_t dir);
static int i2s_resume(struct i2s_dev_s *dev, i2s_ch_dir_t dir);


static int i2s_err_cb_register(struct i2s_dev_s *dev, i2s_err_cb_t cb, void *arg);	


/* Initialization */

#ifdef I2S_HAVE_RX
static int i2s_rx_configure(struct s5j_i2s_s *priv);
#endif
#ifdef I2S_HAVE_TX_P
static int i2s_txp_configure(struct s5j_i2s_s *priv);
#endif
static uint32_t i2s_bitrate(struct s5j_i2s_s *priv);
static int i2s_dma_allocate(struct s5j_i2s_s *priv);
static void i2s_dma_free(struct s5j_i2s_s *priv);

static int i2s_configure(struct s5j_i2s_s *priv);

/****************************************************************************
 * Private Data
 ****************************************************************************/
/* I2S device operations */

static const struct i2s_ops_s g_i2sops = {
	/* Receiver methods */

	.i2s_rxsamplerate = i2s_samplerate,
	.i2s_rxdatawidth = i2s_rxdatawidth,
	.i2s_receive = i2s_receive,

	/* Transmitter methods */

	.i2s_txsamplerate = i2s_samplerate,
	.i2s_txdatawidth = i2s_txdatawidth,
	.i2s_send = i2s_send,

	.i2s_stop = i2s_stop,
	.i2s_pause = i2s_pause,
	.i2s_resume = i2s_resume,
	.i2s_err_cb_register = i2s_err_cb_register,
};

static struct s5j_i2s_s *g_i2sdevice[S5J_I2S_MAXPORTS];

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: i2s_exclsem_take
 *
 * Description:
 *   Take the exclusive access semaphore handling any exceptional conditions
 *
 * Input Parameters:
 *   priv - A reference to the I2S peripheral state
 *
 * Returned Value:
 *  None
 *
 ****************************************************************************/

static void i2s_exclsem_take(struct s5j_i2s_s *priv)
{
	int ret;

	/* Wait until we successfully get the semaphore.  EINTR is the only
	 * expected 'failure' (meaning that the wait for the semaphore was
	 * interrupted by a signal.
	 */

	do {
		ret = sem_wait(&priv->exclsem);
		DEBUGASSERT(ret == 0 || errno == EINTR);
	} while (ret < 0);
}

/****************************************************************************
 * Name: i2s_bufsem_take
 *
 * Description:
 *   Take the buffer semaphore handling any exceptional conditions
 *
 * Input Parameters:
 *   priv - A reference to the I2S peripheral state
 *
 * Returned Value:
 *  None
 *
 ****************************************************************************/

static void i2s_bufsem_rx_take(struct s5j_i2s_s *priv)
{
	int ret;

	/* Wait until we successfully get the semaphore.  EINTR is the only
	 * expected 'failure' (meaning that the wait for the semaphore was
	 * interrupted by a signal.
	 */

	do {
		ret = sem_wait(&priv->bufsem_rx);
		DEBUGASSERT(ret == 0 || errno == EINTR);
	} while (ret < 0);
}

static void i2s_bufsem_tx_take(struct s5j_i2s_s *priv)
{
	int ret;

	/* Wait until we successfully get the semaphore.  EINTR is the only
	 * expected 'failure' (meaning that the wait for the semaphore was
	 * interrupted by a signal.
	 */

	do {
		ret = sem_wait(&priv->bufsem_tx);
		DEBUGASSERT(ret == 0 || errno == EINTR);
	} while (ret < 0);
}

/****************************************************************************
 * Name: i2s_buf_allocate
 *
 * Description:
 *   Allocate a buffer container by removing the one at the head of the
 *   free list
 *
 * Input Parameters:
 *   priv - i2s state instance
 *
 * Returned Value:
 *   A non-NULL pointer to the allocate buffer container on success; NULL if
 *   there are no available buffer containers.
 *
 * Assumptions:
 *   The caller does NOT have exclusive access to the I2S state structure.
 *   That would result in a deadlock!
 *
 ****************************************************************************/

static struct s5j_buffer_s *i2s_buf_rx_allocate(struct s5j_i2s_s *priv)
{
	struct s5j_buffer_s *bfcontainer;
	irqstate_t flags;

	/* Set aside a buffer container.  By doing this, we guarantee that we will
	 * have at least one free buffer container.
	 */

	i2s_bufsem_rx_take(priv);

	/* Get the buffer from the head of the free list */

	flags = irqsave();
	bfcontainer = priv->freelist_rx;
	ASSERT(bfcontainer);

	/* Unlink the buffer from the freelist */

	priv->freelist_rx = bfcontainer->flink;
	irqrestore(flags);

	return bfcontainer;
}

static struct s5j_buffer_s *i2s_buf_tx_allocate(struct s5j_i2s_s *priv)
{
	struct s5j_buffer_s *bfcontainer;
	irqstate_t flags;

	/* Set aside a buffer container.  By doing this, we guarantee that we will
	 * have at least one free buffer container.
	 */

	i2s_bufsem_tx_take(priv);

	/* Get the buffer from the head of the free list */

	flags = irqsave();
	bfcontainer = priv->freelist_tx;
	ASSERT(bfcontainer);

	/* Unlink the buffer from the freelist */

	priv->freelist_tx = bfcontainer->flink;
	irqrestore(flags);

	return bfcontainer;
}

/****************************************************************************
 * Name: i2s_buf_free
 *
 * Description:
 *   Free buffer container by adding it to the head of the free list
 *
 * Input Parameters:
 *   priv - I2S state instance
 *   bfcontainer - The buffer container to be freed
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *   The caller has exclusive access to the I2S state structure
 *
 ****************************************************************************/

static void i2s_buf_rx_free(struct s5j_i2s_s *priv, struct s5j_buffer_s *bfcontainer)
{
	irqstate_t flags;

	/* Put the buffer container back on the free list */

	flags = irqsave();
	bfcontainer->flink = priv->freelist_rx;
	priv->freelist_rx = bfcontainer;
	irqrestore(flags);

	/* Wake up any threads waiting for a buffer container */

	i2s_bufsem_rx_give(priv);
}

static void i2s_buf_tx_free(struct s5j_i2s_s *priv, struct s5j_buffer_s *bfcontainer)
{
	irqstate_t flags;

	/* Put the buffer container back on the free list */

	flags = irqsave();
	bfcontainer->flink = priv->freelist_tx;
	priv->freelist_tx = bfcontainer;
	irqrestore(flags);

	/* Wake up any threads waiting for a buffer container */

	i2s_bufsem_tx_give(priv);
}

/****************************************************************************
 * Name: i2s_buf_initialize
 *
 * Description:
 *   Initialize the buffer container allocator by adding all of the
 *   pre-allocated buffer containers to the free list
 *
 * Input Parameters:
 *   priv - I2S state instance
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *   Called early in I2S initialization so that there are no issues with
 *   concurrency.
 *
 ****************************************************************************/

static void i2s_buf_rx_initialize(struct s5j_i2s_s *priv)
{
	int i;

	priv->freelist_rx = NULL;
	sem_init(&priv->bufsem_rx, 0, 0);

	for (i = 0; i < CONFIG_S5J_I2S_MAXINFLIGHT; i++) {
		i2s_buf_rx_free(priv, &priv->containers_rx[i]);
	}
}

static void i2s_buf_tx_initialize(struct s5j_i2s_s *priv)
{
	int i;

	priv->freelist_tx = NULL;
	sem_init(&priv->bufsem_tx, 0, 0);

	for (i = 0; i < CONFIG_S5J_I2S_MAXINFLIGHT; i++) {
		i2s_buf_tx_free(priv, &priv->containers_tx[i]);
	}
}

/****************************************************************************
 * Name: i2s_rxdma_timeout
 *
 * Description:
 *   The RX watchdog timeout without completion of the RX DMA.
 *
 * Input Parameters:
 *   argc   - The number of arguments (should be 1)
 *   arg    - The argument (state structure reference cast to uint32_t)
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *   Always called from the interrupt level with interrupts disabled.
 *
 ****************************************************************************/

#ifdef I2S_HAVE_RX
static void i2s_rxdma_timeout(int argc, uint32_t arg)
{
	struct s5j_i2s_s *priv = (struct s5j_i2s_s *)arg;
	DEBUGASSERT(priv != NULL);

	/* Cancel the DMA */

	s5j_dmastop(priv->rx.dma);

	/* Then schedule completion of the transfer to occur on the worker thread.
	 * NOTE: s5j_dmastop() will call the DMA complete callback with an error
	 * of -EINTR.  So the following is just insurance and should have no
	 * effect if the worker is already schedule.
	 */

	i2s_rx_schedule(priv, -ETIMEDOUT);
}
#endif

/****************************************************************************
 * Name: i2s_rx_start
 *
 * Description:
 *   Setup and initiate first RX DMA transfer
 *
 * Input Parameters:
 *   priv - I2S state instance
 *
 * Returned Value:
 *   OK on success; a negated errno value on failure
 *
 * Assumptions:
 *   Interrupts are disabled
 *
 ****************************************************************************/

#ifdef I2S_HAVE_RX

static int i2s_rxdma_prep(struct s5j_i2s_s *priv, struct s5j_buffer_s *bfcontainer)
{
	struct ap_buffer_s *apb;
	dma_task *dmatask;

	apb = bfcontainer->apb;
	dmatask = bfcontainer->dmatask;
	/* No data received yet */

	apb->nbytes = 0;
	apb->curbyte = 0;

	dmatask->dst = (void *)apb->samp;
	dmatask->src = (void *)(priv->base + S5J_I2S_RXD);
	dmatask->size = apb->nmaxbytes;
	dmatask->callback = i2s_rxdma_callback;
	dmatask->arg = priv;

	/* Configure the RX DMA task */
	s5j_dmasetup(priv->rx.dma, dmatask);

	return 0;
}



static int i2s_rx_start(struct s5j_i2s_s *priv)
{
	struct s5j_buffer_s *bfcontainer;
	uint32_t timeout;
	bool notimeout;
	int ret;
	irqstate_t flags;
	volatile u32 reg;

	/* Check if the DMA is IDLE */
	if (!sq_empty(&priv->rx.act)) {
		return OK;
	}

	/* If there are no pending transfer, then bail returning success */
	if (sq_empty(&priv->rx.pend)) {
		return OK;
	}

	/* If I2S RX DMA is active, pend buffers will be fetched and processed */
	reg = getreg32(priv->base + S5J_I2S_CON);	
	if (reg & I2S_CR_RXDMACTIVE) {
		return OK;
	}
		
	/* Here is no DMA activity, We do not care about IRQ, just initate first transfer */
	i2sinfo("RX Initiate first RX\n");

	timeout = 0;
	notimeout = false;

	/* Remove the pending RX transfer at the head of the RX pending queue. */
	bfcontainer = (struct s5j_buffer_s *)sq_remfirst(&priv->rx.pend);

	DEBUGASSERT(bfcontainer && bfcontainer->apb);


	/* Increment the DMA timeout */

	if (bfcontainer->timeout > 0) {
		timeout += bfcontainer->timeout;
	} else {
		notimeout = true;
	}

	/* Add the container to the list of active DMAs */
	/* Container will be removed on the interrupt level */
	sq_addlast((sq_entry_t *) bfcontainer, &priv->rx.act);

	flags = irqsave();
	/* FLUSH RX FIFO */
	putreg32(I2S_FIC_RFLUSH, priv->base + S5J_I2S_FIC);
	putreg32(0, priv->base + S5J_I2S_FIC);

	/* Start the DMA, saving the container as the current active transfer */
	s5j_dmastart(priv->rx.dma, bfcontainer->dmatask);

	/* Enable Receiver */
	modifyreg32(priv->base + S5J_I2S_CON, 0, I2S_CR_RXDMACTIVE);
	modifyreg32(priv->base + S5J_I2S_CON, 0, I2S_CR_I2SACTIVE);
	modifyreg32(priv->base + S5J_I2S_CON, 0, I2S_CR_FRXOFINTEN);
	irqrestore(flags);


	/* Start a watchdog to catch DMA timeouts */
	if (!notimeout) {
		ret = wd_start(priv->rx.dog, timeout, (wdentry_t) i2s_rxdma_timeout, 1, (uint32_t) priv);

		/* Check if we have successfully started the watchdog timer.  Note
		 * that we do nothing in the case of failure to start the timer.  We
		 * are already committed to the DMA anyway.  Let's just hope that the
		 * DMA does not hang.
		 */

		if (ret < 0) {
			i2serr("ERROR: wd_start failed: %d\n", errno);
		}
	}

	return OK;
}
#endif

/****************************************************************************
 * Name: i2s_rx_worker
 *
 * Description:
 *   RX transfer done worker
 *
 * Input Parameters:
 *   arg - the I2S device instance cast to void*
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#ifdef I2S_HAVE_RX
static void i2s_rx_worker(void *arg)
{
	struct s5j_i2s_s *priv = (struct s5j_i2s_s *)arg;
	struct ap_buffer_s *apb;
	struct s5j_buffer_s *bfcontainer;
	irqstate_t flags;

	DEBUGASSERT(priv);

	/* When the transfer was started, the active buffer containers were removed
	 * from the rx.pend queue and saved in the rx.act queue.  We get here when the
	 * DMA is finished... either successfully, with a DMA error, or with a DMA
	 * timeout.
	 *
	 * In any case, the buffer containers in rx.act will be moved to the end
	 * of the rx.done queue and rx.act queue will be emptied before this worker
	 * is started.
	 */

	i2sinfo("rx.act.head=%p rx.done.head=%p\n", priv->rx.act.head, priv->rx.done.head);


	/* Process each buffer in the rx.done queue */
	while (sq_peek(&priv->rx.done) != NULL) {
		/* Remove the buffer container from the rx.done queue.  NOTE that
		 * interrupts must be disabled to do this because the rx.done queue is
		 * also modified from the interrupt level.
		 */

		flags = irqsave();
		bfcontainer = (struct s5j_buffer_s *)sq_remfirst(&priv->rx.done);
		irqrestore(flags);

		/* Perform the RX transfer done callback */

		DEBUGASSERT(bfcontainer && bfcontainer->apb && bfcontainer->callback);
		apb = bfcontainer->apb;

		/* If the DMA was successful, then update the number of valid bytes in
		 * the audio buffer.
		 */

		if (bfcontainer->result == OK) {
			apb->nbytes = apb->nmaxbytes;
		}

		i2s_dump_buffer("Received", apb->samp, apb->nbytes);

		/* Perform the RX transfer done callback */
		bfcontainer->callback(&priv->dev, apb, bfcontainer->arg, bfcontainer->result);

		/* Release our reference on the audio buffer.  This may very likely
		 * cause the audio buffer to be freed.
		 */
		apb_free(apb);

		/* And release the buffer container */
		i2s_buf_rx_free(priv, bfcontainer);
	}

}
#endif

/****************************************************************************
 * Name: i2s_rx_schedule
 *
 * Description:
 *   An RX DMA completion or timeout has occurred.  Schedule processing on
 *   the working thread.
 *
 * Input Parameters:
 *   handle - The DMA handler
 *   arg - A pointer to the chip select struction
 *   result - The result of the DMA transfer
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *   Interrupts are disabled
 *
 ****************************************************************************/

#ifdef I2S_HAVE_RX
static void i2s_rx_schedule(struct s5j_i2s_s *priv, int result)
{
	struct s5j_buffer_s *bfcontainer;
	int ret;

	/* Upon entry, the transfer(s) that just completed are the ones in the
	 * priv->rx.act queue.  NOTE: In certain conditions, this function may
	 * be called an additional time, hence, we can't assert this to be true.
	 * For example, in the case of a timeout, this function will be called by
	 * both indirectly via the s5j_dmastop() logic and directly via the
	 * i2s_rxdma_timeout() logic.
	 */

	/* Move first entry from the rx.act queue to the rx.done queue */

	if (!sq_empty(&priv->rx.act)) {
		/* Remove the next buffer container from the rx.act list */

		bfcontainer = (struct s5j_buffer_s *)sq_remfirst(&priv->rx.act);

		/* Report the result of the transfer */

		bfcontainer->result = result;

		/* Add the completed buffer container to the tail of the rx.done queue */

		sq_addlast((sq_entry_t *) bfcontainer, &priv->rx.done);
	}

	if (!sq_empty(&priv->rx.pend)) {
		/* Remove the next buffer container from the tx.pend list */
		bfcontainer = (struct s5j_buffer_s *)sq_remfirst(&priv->rx.pend);

		/* Add the completed buffer container to the tail of the tx.act queue */
		sq_addlast((sq_entry_t *) bfcontainer, &priv->rx.act);

		/* Start next DMA transfer */
		s5j_dmastart(priv->rx.dma, bfcontainer->dmatask);
	}



	/* If the worker has completed running, then reschedule the working thread.
	 * REVISIT:  There may be a race condition here.  So we do nothing is the
	 * worker is not available.
	 */
	if (work_available(&priv->rx.work)) {
		/* Schedule the TX DMA done processing to occur on the worker thread. */

		ret = work_queue(HPWORK, &priv->rx.work, i2s_rx_worker, priv, 0);
		if (ret != 0) {
			i2serr("ERROR: Failed to queue RX work: %d\n", ret);
		}
	}
}
#endif

/****************************************************************************
 * Name: i2s_rxdma_callback
 *
 * Description:
 *   This callback function is invoked at the completion of the I2S RX DMA.
 *
 * Input Parameters:
 *   handle - The DMA handler
 *   arg - A pointer to the chip select struction
 *   result - The result of the DMA transfer
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#ifdef I2S_HAVE_RX
static void i2s_rxdma_callback(DMA_HANDLE handle, void *arg, int result)
{
	struct s5j_i2s_s *priv = (struct s5j_i2s_s *)arg;
	DEBUGASSERT(priv != NULL);

	/* Cancel the watchdog timeout */

	(void)wd_cancel(priv->rx.dog);


	/* Then schedule completion of the transfer to occur on the worker thread */

	i2s_rx_schedule(priv, result);
}
#endif

/****************************************************************************
 * Name: i2s_txpdma_timeout
 *
 * Description:
 *   The Primary TX watchdog timeout without completion of the TX DMA.
 *
 * Input Parameters:
 *   argc   - The number of arguments (should be 1)
 *   arg    - The argument (state structure reference cast to uint32_t)
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *   Always called from the interrupt level with interrupts disabled.
 *
 ****************************************************************************/

#ifdef I2S_HAVE_TX_P
static void i2s_txpdma_timeout(int argc, uint32_t arg)
{
	struct s5j_i2s_s *priv = (struct s5j_i2s_s *)arg;
	DEBUGASSERT(priv != NULL);

	/* Cancel the DMA */

	s5j_dmastop(priv->txp.dma);

	/* Then schedule completion of the transfer to occur on the worker thread.
	 * NOTE: s5j_dmastop() will call the DMA complete callback with an error
	 * of -EINTR.  So the following is just insurance and should have no
	 * effect if the worker is already schedule.
	 */

	i2s_txp_schedule(priv, -ETIMEDOUT);
}
#endif

/****************************************************************************
 * Name: i2s_txp_start
 *
 * Description:
 *   Setup and initiate the first TX DMA transfer
 *
 * Input Parameters:
 *   priv - I2S state instance
 *
 * Returned Value:
 *   OK on success; a negated errno value on failure
 *
 ****************************************************************************/

#ifdef I2S_HAVE_TX_P


static int i2s_txpdma_prep(struct s5j_i2s_s *priv, struct s5j_buffer_s *bfcontainer)
{
struct ap_buffer_s *apb;
dma_task *dmatask;


	apb = bfcontainer->apb;
	dmatask = bfcontainer->dmatask;
	/* Get the transfer information, accounting for any data offset */

	dmatask->dst = (void *)(priv->base + S5J_I2S_TXD);
	dmatask->src = (void *)&apb->samp[apb->curbyte];
	dmatask->size = apb->nbytes - apb->curbyte;
	dmatask->callback = i2s_txpdma_callback;
	dmatask->arg = priv;

	/* Configure the TX DMA task */
	s5j_dmasetup(priv->txp.dma, dmatask);

	return 0;
}


static int i2s_txp_start(struct s5j_i2s_s *priv)
{
	struct s5j_buffer_s *bfcontainer;
	uint32_t timeout;
	bool notimeout;
	int ret;
	irqstate_t flags;
	volatile u32 reg;

	/* Check if the DMA is IDLE */
	if (!sq_empty(&priv->txp.act)) { 
		return OK;
	}

	/* If there are no pending transfer, then bail returning success */
	if (sq_empty(&priv->txp.pend)) {
		return OK;
	}

	/* If I2S TX DMA is active, pend buffers will be fetched and processed */
	reg = getreg32(priv->base + S5J_I2S_CON);	
	if (reg & I2S_CR_TXDMACTIVE) {
		return OK;
	}

	/* Here is no DMA activity, We do not care about IRQ, just initate first transfer */

	timeout = 0;
	notimeout = false;

	/* Remove the pending TX transfer at the head of the TX pending queue. */
	bfcontainer = (struct s5j_buffer_s *)sq_remfirst(&priv->txp.pend);

	DEBUGASSERT(bfcontainer && bfcontainer->apb);

	/* Increment the DMA timeout */

	if (bfcontainer->timeout > 0) {
		timeout += bfcontainer->timeout;
	} else {
		notimeout = true;
	}

	/* Add the container to the list of active DMAs */
	sq_addlast((sq_entry_t *) bfcontainer, &priv->txp.act);

	flags = irqsave();
	/* Start the DMA, saving the container as the current active transfer */
	s5j_dmastart(priv->txp.dma, bfcontainer->dmatask);

	/* Enable transmitter */
	modifyreg32(priv->base + S5J_I2S_CON, 0, I2S_CR_TXDMACTIVE);
	modifyreg32(priv->base + S5J_I2S_CON, 0, I2S_CR_I2SACTIVE);

	/* Enable TX interrupt to catch the end of TX */
	modifyreg32(priv->base + S5J_I2S_CON, 0, I2S_CR_FTXURINTEN);
	irqrestore(flags);

	/* Start a watchdog to catch DMA timeouts */

	if (!notimeout) {
		ret = wd_start(priv->txp.dog, timeout, (wdentry_t) i2s_txpdma_timeout, 1, (uint32_t) priv);

		/* Check if we have successfully started the watchdog timer.  Note
		 * that we do nothing in the case of failure to start the timer.  We
		 * are already committed to the DMA anyway.  Let's just hope that the
		 * DMA does not hang.
		 */

		if (ret < 0) {
			i2serr("ERROR: wd_start failed: %d\n", errno);
		}
	}

	return OK;
}
#endif

/****************************************************************************
 * Name: i2s_txp_worker
 *
 * Description:
 *   Primary TX transfer done worker
 *
 * Input Parameters:
 *   arg - the I2S device instance cast to void*
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#ifdef I2S_HAVE_TX_P
static void i2s_txp_worker(void *arg)
{
	struct s5j_i2s_s *priv = (struct s5j_i2s_s *)arg;
	struct s5j_buffer_s *bfcontainer;
	irqstate_t flags;

	DEBUGASSERT(priv);

	/* When the transfer was started, the active buffer containers were removed
	 * from the txp.pend queue and saved in the txp.act queue.  We get here when the
	 * DMA is finished... either successfully, with a DMA error, or with a DMA
	 * timeout.
	 *
	 * In any case, the buffer containers in txp.act will be moved to the end
	 * of the txp.done queue and txp.act will be emptied before this worker is
	 * started.
	 */

	i2sinfo("txp.act.head=%p txp.done.head=%p\n", priv->txp.act.head, priv->txp.done.head);


	/* Process each buffer in the tx.done queue */
	while (sq_peek(&priv->txp.done) != NULL) {
		/* Remove the buffer container from the tx.done queue.  NOTE that
		 * interrupts must be enabled to do this because the tx.done queue is
		 * also modified from the interrupt level.
		 */

		flags = irqsave();
		bfcontainer = (struct s5j_buffer_s *)sq_remfirst(&priv->txp.done);
		irqrestore(flags);

		/* Perform the TX transfer done callback */

		DEBUGASSERT(bfcontainer && bfcontainer->callback);
		bfcontainer->callback(&priv->dev, bfcontainer->apb, bfcontainer->arg, bfcontainer->result);

		/* Release our reference on the audio buffer.  This may very likely
		 * cause the audio buffer to be freed.
		 */

		apb_free(bfcontainer->apb);

		/* And release the buffer container */

		i2s_buf_tx_free(priv, bfcontainer);
	}

}
#endif

/****************************************************************************
 * Name: i2s_txp_schedule
 *
 * Description:
 *   A TX DMA completion or timeout has occurred.  Schedule processing on
 *   the working thread.
 *
 * Input Parameters:
 *   handle - The DMA handler
 *   arg - A pointer to the chip select struction
 *   result - The result of the DMA transfer
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *   - Interrupts are disabled
 *   - The TX timeout has been canceled.
 *
 ****************************************************************************/

#ifdef I2S_HAVE_TX_P
static void i2s_txp_schedule(struct s5j_i2s_s *priv, int result)
{
	struct s5j_buffer_s *bfcontainer;
	int ret;

	/* Upon entry, the transfer(s) that just completed are the ones in the
	 * priv->tx.act queue.  NOTE: In certain conditions, this function may
	 * be called an additional time, hence, we can't assert this to be true.
	 * For example, in the case of a timeout, this function will be called by
	 * both indirectly via the s5j_dmastop() logic and directly via the
	 * i2s_txdma_timeout() logic.
	 */

	/* Move all entries from the tx.act queue to the tx.done queue */

	if (!sq_empty(&priv->txp.act)) {
		/* Remove the next buffer container from the tx.act list */

		bfcontainer = (struct s5j_buffer_s *)sq_remfirst(&priv->txp.act);

		/* Report the result of the transfer */

		bfcontainer->result = result;

		/* Add the completed buffer container to the tail of the tx.done queue */

		sq_addlast((sq_entry_t *) bfcontainer, &priv->txp.done);
	}


	if (!sq_empty(&priv->txp.pend)) {
		/* Remove the next buffer container from the tx.pend list */
		bfcontainer = (struct s5j_buffer_s *)sq_remfirst(&priv->txp.pend);

		/* Add the completed buffer container to the tail of the tx.act queue */
		sq_addlast((sq_entry_t *) bfcontainer, &priv->txp.act);

		/* Start next DMA transfer */
		s5j_dmastart(priv->txp.dma, bfcontainer->dmatask);

	}

	/* If the worker has completed running, then reschedule the working thread.
	 * REVISIT:  There may be a race condition here.  So we do nothing is the
	 * worker is not available.
	 */

	if (work_available(&priv->txp.work)) {
		/* Schedule the TX DMA done processing to occur on the worker thread. */

		ret = work_queue(HPWORK, &priv->txp.work, i2s_txp_worker, priv, 0);
		if (ret != 0) {
			i2serr("ERROR: Failed to queue TX primary work: %d\n", ret);
		}
	}
}
#endif

/****************************************************************************
 * Name: i2s_txpdma_callback
 *
 * Description:
 *   This callback function is invoked at the completion of the I2S TX DMA.
 *
 * Input Parameters:
 *   handle - The DMA handler
 *   arg - A pointer to the chip select struction
 *   result - The result of the DMA transfer
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#ifdef I2S_HAVE_TX_P
static void i2s_txpdma_callback(DMA_HANDLE handle, void *arg, int result)
{
	struct s5j_i2s_s *priv = (struct s5j_i2s_s *)arg;
	DEBUGASSERT(priv != NULL);

	/* Cancel the watchdog timeout */

	(void)wd_cancel(priv->txp.dog);


	/* Then schedule completion of the transfer to occur on the worker thread */

	i2s_txp_schedule(priv, result);
}
#endif

/****************************************************************************
 * Name: i2s_checkwidth
 *
 * Description:
 *   Check for a valid bit width.  The I2S is capable of handling most any
 *   bit width from 2 to 32, but the DMA logic in this driver is constrained
 *   to 8-, 16-, and 32-bit data widths
 *
 * Input Parameters:
 *   dev  - Device-specific state data
 *   rate - The I2S sample rate in samples (not bits) per second
 *
 * Returned Value:
 *   Returns the resulting bitrate
 *
 ****************************************************************************/

static int i2s_checkwidth(struct s5j_i2s_s *priv, int bits)
{
	/* The I2S can handle most any bit width from 2 to 32.  However, the DMA
	 * logic here is constrained to byte, half-word, and word sizes.
	 */

	switch (bits) {
	case 8:
		break;

	case 16:
		break;

	case 32:
		break;

	default:
		i2serr("ERROR: Unsupported or invalid data width: %d\n", bits);
		return (bits < 2 || bits > 32) ? -EINVAL : -ENOSYS;
	}

	/* Save the new data width */
	priv->datalen = bits;
	return OK;
}

/****************************************************************************
 * Name: i2s_bitrate
 *
 * Description:
 *   In master mode supposed to set bit rate. But now, in slave mode,
 *   we just calculate it. Still we set all bit related regs.
 *
 * Input Parameter:
 *   priv - I2S device structure (only the sample rate and data length is
 *          needed at this point).
 *
 * Returned Value:
 *   The current bitrate
 *
 ****************************************************************************/

static uint32_t i2s_bitrate(struct s5j_i2s_s *priv)
{
	uint32_t bitrate;
	uint32_t regval;
	DEBUGASSERT(priv && priv->samplerate > 0 && priv->datalen > 0);

	switch (priv->datalen) {
	case 8:
		regval = BLC_8BIT;
		break;

	case 16:
		regval = BLC_16BIT;
		break;

	case 32:
		regval = BLC_24BIT;
		break;

	default:
		i2serr("ERROR: Unsupported or invalid datalen: %d\n", priv->datalen);
		return -EINVAL;
	}

	modifyreg32(priv->base + S5J_I2S_MOD, I2S_MOD_BLC_S_MASK, I2S_MOD_BLC_S(regval));
	modifyreg32(priv->base + S5J_I2S_MOD, I2S_MOD_BLC_P_MASK, I2S_MOD_BLC_P(regval));
	modifyreg32(priv->base + S5J_I2S_MOD, I2S_MOD_BLC_MASK, I2S_MOD_BLC(regval));

	/* Even in slave mode it recommended to set. Set common values */
	modifyreg32(priv->base + S5J_I2S_MOD, I2S_MOD_RFS_MASK, I2S_MOD_RFS(RFS_192));
	modifyreg32(priv->base + S5J_I2S_MOD, I2S_MOD_BFS_MASK, I2S_MOD_BFS(BFS_64));

	bitrate = priv->samplerate * priv->datalen;
	return bitrate;
}

/****************************************************************************
 * Name: i2s_rxdatawidth
 *
 * Description:
 *   Set the I2S RX data width.  The RX bitrate is determined by
 *   sample_rate * data_width.
 *
 * Input Parameters:
 *   dev   - Device-specific state data
 *   width - The I2S data with in bits.
 *
 * Returned Value:
 *   Returns the resulting bitrate
 *
 ****************************************************************************/

static uint32_t i2s_rxdatawidth(struct i2s_dev_s *dev, int bits)
{

#ifdef I2S_HAVE_RX
	struct s5j_i2s_s *priv = (struct s5j_i2s_s *)dev;
	int ret;

	DEBUGASSERT(priv && bits > 1);

	/* Check if this is a bit width that we are configured to handle */

	ret = i2s_checkwidth(priv, bits);
	if (ret < 0) {
		i2serr("ERROR: i2s_checkwidth failed: %d\n", ret);
		return 0;
	}

	/* Reconfigure the RX DMA (and TX DMA if applicable) */
	return i2s_bitrate(priv);
#endif

	return 0;
}

/****************************************************************************
 * Name: i2s_receive
 *
 * Description:
 *   Receive a block of data from I2S.
 *
 * Input Parameters:
 *   dev      - Device-specific state data
 *   apb      - A pointer to the audio buffer in which to recieve data
 *   callback - A user provided callback function that will be called at
 *              the completion of the transfer.  The callback will be
 *              performed in the context of the worker thread.
 *   arg      - An opaque argument that will be provided to the callback
 *              when the transfer complete
 *   timeout  - The timeout value to use.  The transfer will be canceled
 *              and an ETIMEDOUT error will be reported if this timeout
 *              elapsed without completion of the DMA transfer.  Units
 *              are system clock ticks.  Zero means no timeout.
 *
 * Returned Value:
 *   OK on success; a negated errno value on failure.  NOTE:  This function
 *   only enqueues the transfer and returns immediately.  Success here only
 *   means that the transfer was enqueued correctly.
 *
 *   When the transfer is complete, a 'result' value will be provided as
 *   an argument to the callback function that will indicate if the transfer
 *   failed.
 *
 ****************************************************************************/

static int i2s_receive(struct i2s_dev_s *dev, struct ap_buffer_s *apb, i2s_callback_t callback, void *arg, uint32_t timeout)
{

	struct s5j_i2s_s *priv = (struct s5j_i2s_s *)dev;
#ifdef I2S_HAVE_RX
	struct s5j_buffer_s *bfcontainer;
	irqstate_t flags;
	int ret;
#endif

	i2sinfo("apb=%p nmaxbytes=%d arg=%p timeout=%d\n", apb, apb->nmaxbytes, arg, timeout);

	i2s_init_buffer(apb->samp, apb->nmaxbytes);

#ifdef I2S_HAVE_RX

	/* Invalidate the data cache so that nothing gets flush into the
	 * DMA buffer after starting the DMA transfer.
	 */

	arch_invalidate_dcache((uintptr_t) apb->samp, (uintptr_t)(apb->samp + apb->nmaxbytes));

	/* Allocate a buffer container in advance */

	bfcontainer = i2s_buf_rx_allocate(priv);
	DEBUGASSERT(bfcontainer);

	/* Get exclusive access to the I2S driver data */

	i2s_exclsem_take(priv);
	i2sinfo("RX Exclusive Enter\n");

	/* Has the RX channel been enabled? */

	if (!priv->rxenab) {
		i2serr("ERROR: I2S has no receiver\n");
		ret = -EAGAIN;
		goto errout_with_exclsem;
	}

	/* Add a reference to the audio buffer */

	apb_reference(apb);

	/* Initialize the buffer container structure */

	bfcontainer->callback = (void *)callback;
	bfcontainer->timeout = timeout;
	bfcontainer->arg = arg;
	bfcontainer->apb = apb;
	bfcontainer->result = -EBUSY;

	/* Prepare DMA microcode */
	i2s_rxdma_prep(priv, bfcontainer);

	/* Add the buffer container to the end of the RX pending queue */
	flags = irqsave();
	sq_addlast((sq_entry_t *) bfcontainer, &priv->rx.pend);
	irqrestore(flags);

	/* Then start the next transfer through.
	 * If ther is no active transfer, then one except us can start the process.
	 * Perform it smooth, without scheduling.	
	 */
	i2s_rx_start(priv);

	i2s_exclsem_give(priv);
	i2sinfo("RX Exclusive Exit\n");

	return OK;

errout_with_exclsem:
	i2s_exclsem_give(priv);
	i2s_buf_rx_free(priv, bfcontainer);
	return ret;

#else
	i2serr("ERROR: I2S has no receiver\n");
	UNUSED(priv);
	return -ENOSYS;
#endif

}

/****************************************************************************
 * Name: i2s_samplerate
 *
 * Description:
 *   Set the I2S sample rate.
 *
 * Input Parameters:
 *   dev  - Device-specific state data
 *   rate - The I2S sample rate in samples (not bits) per second
 *
 * Returned Value:
 *   Returns the resulting bitrate
 *
 ****************************************************************************/

static uint32_t i2s_samplerate(struct i2s_dev_s *dev, uint32_t rate)
{
	struct s5j_i2s_s *priv = (struct s5j_i2s_s *)dev;
	DEBUGASSERT(priv && priv->samplerate > 0 && rate > 0);

	/* Check if the receiver is driven by the MCK/2 */

	priv->samplerate = rate;
	return i2s_bitrate(priv);
}

/****************************************************************************
 * Name: i2s_txdatawidth
 *
 * Description:
 *   Set the I2S TX data width.  The TX bitrate is determined by
 *   sample_rate * data_width.
 *
 * Input Parameters:
 *   dev   - Device-specific state data
 *   width - The I2S data with in bits.
 *
 * Returned Value:
 *   Returns the resulting bitrate
 *
 ****************************************************************************/

static uint32_t i2s_txdatawidth(struct i2s_dev_s *dev, int bits)
{
#ifdef I2S_HAVE_TX_P
	struct s5j_i2s_s *priv = (struct s5j_i2s_s *)dev;
	int ret;

	DEBUGASSERT(priv && bits > 1);

	/* Check if this is a bit width that we are configured to handle */

	ret = i2s_checkwidth(priv, bits);
	if (ret < 0) {
		i2serr("ERROR: i2s_checkwidth failed: %d\n", ret);
		return 0;
	}

	/* Reconfigure the RX DMA (and TX DMA if applicable) */

	return i2s_bitrate(priv);
#endif

	return 0;
}

/***************************************************************************
****************************************************************************/
static int i2s_pause(struct i2s_dev_s *dev, i2s_ch_dir_t dir)
{
	struct s5j_i2s_s *priv = (struct s5j_i2s_s *)dev;
	volatile u32 reg = 0;
	DEBUGASSERT(priv);

	reg = getreg32(priv->base + S5J_I2S_CON);
#ifdef I2S_HAVE_TX_P
	if (dir == I2S_TX && priv->txpenab && (reg & I2S_CR_TXDMACTIVE)) {
		modifyreg32(priv->base + S5J_I2S_CON, 0, I2S_CR_TXCHPAUSE);
	}
#endif

#ifdef I2S_HAVE_RX
	if (dir == I2S_RX && priv->rxenab && (reg & I2S_CR_RXDMACTIVE)) {
		modifyreg32(priv->base + S5J_I2S_CON, 0, I2S_CR_RXCHPAUSE);
	}
#endif
	return 0;
}

static int i2s_resume(struct i2s_dev_s *dev, i2s_ch_dir_t dir)
{
	struct s5j_i2s_s *priv = (struct s5j_i2s_s *)dev;
	volatile u32 reg;
	DEBUGASSERT(priv);

	reg = getreg32(priv->base + S5J_I2S_CON);
#ifdef I2S_HAVE_TX_P
	if (dir == I2S_TX && priv->txpenab  && (reg & I2S_CR_TXDMACTIVE)) {
		modifyreg32(priv->base + S5J_I2S_CON, I2S_CR_TXCHPAUSE, 0);
	}
#endif

#ifdef I2S_HAVE_RX
	if (dir == I2S_RX && priv->rxenab  && (reg & I2S_CR_RXDMACTIVE)) {
		modifyreg32(priv->base + S5J_I2S_CON, I2S_CR_RXCHPAUSE, 0);
	}
#endif

	return 0;
}

static int i2s_stop(struct i2s_dev_s *dev, i2s_ch_dir_t dir)
{
	struct s5j_i2s_s *priv = (struct s5j_i2s_s *)dev;
	irqstate_t flags;
	struct s5j_buffer_s *bfcontainer;
	volatile u32 reg;
	DEBUGASSERT(priv);

#ifdef I2S_HAVE_TX_P
	if (dir == I2S_TX) {
		modifyreg32(priv->base + S5J_I2S_CON, I2S_CR_FTXURINTEN, 0);
		modifyreg32(priv->base + S5J_I2S_CON, I2S_CR_TXDMACTIVE, 0);
		modifyreg32(priv->base + S5J_I2S_CON, I2S_CR_TXCHPAUSE, 0);
		s5j_dmastop(priv->txp.dma);

		while (sq_peek(&priv->txp.pend) != NULL) {
			flags = irqsave();
			bfcontainer = (struct s5j_buffer_s *)sq_remfirst(&priv->txp.pend);
			irqrestore(flags);
			apb_free(bfcontainer->apb);
			i2s_buf_tx_free(priv, bfcontainer);
		}

		while (sq_peek(&priv->txp.act) != NULL) {
			flags = irqsave();
			bfcontainer = (struct s5j_buffer_s *)sq_remfirst(&priv->txp.act);
			irqrestore(flags);
			apb_free(bfcontainer->apb);
			i2s_buf_tx_free(priv, bfcontainer);
		}

		while (sq_peek(&priv->txp.done) != NULL) {
			flags = irqsave();
			bfcontainer = (struct s5j_buffer_s *)sq_remfirst(&priv->txp.done);
			irqrestore(flags);
			i2s_buf_tx_free(priv, bfcontainer);
		}
	}
#endif

#ifdef I2S_HAVE_RX
	if (dir == I2S_RX) {
		modifyreg32(priv->base + S5J_I2S_CON, I2S_CR_RXDMACTIVE, 0);
		modifyreg32(priv->base + S5J_I2S_CON, I2S_CR_RXDMACTIVE, 0);
		modifyreg32(priv->base + S5J_I2S_CON, I2S_CR_RXCHPAUSE, 0);
		s5j_dmastop(priv->rx.dma);

		while (sq_peek(&priv->rx.pend) != NULL) {
			flags = irqsave();
			bfcontainer = (struct s5j_buffer_s *)sq_remfirst(&priv->rx.pend);
			irqrestore(flags);
			apb_free(bfcontainer->apb);
			i2s_buf_rx_free(priv, bfcontainer);
		}

		while (sq_peek(&priv->rx.act) != NULL) {
			flags = irqsave();
			bfcontainer = (struct s5j_buffer_s *)sq_remfirst(&priv->rx.act);
			irqrestore(flags);
			apb_free(bfcontainer->apb);
			i2s_buf_rx_free(priv, bfcontainer);
		}

		while (sq_peek(&priv->rx.done) != NULL) {
			flags = irqsave();
			bfcontainer = (struct s5j_buffer_s *)sq_remfirst(&priv->rx.done);
			irqrestore(flags);
			i2s_buf_rx_free(priv, bfcontainer);
		}
	}
#endif


	reg = getreg32(priv->base + S5J_I2S_CON);

	if ((reg & (I2S_CR_TXDMACTIVE | I2S_CR_RXDMACTIVE)) == 0) {
		modifyreg32(priv->base + S5J_I2S_CON, I2S_CR_I2SACTIVE, 0);
	}

	return 0;
}

/****************************************************************************
 * Name: i2s_send
 *
 * Description:
 *   Send a block of data on I2S.
 *
 * Input Parameters:
 *   dev      - Device-specific state data
 *   apb      - A pointer to the audio buffer from which to send data
 *   callback - A user provided callback function that will be called at
 *              the completion of the transfer.  The callback will be
 *              performed in the context of the worker thread.
 *   arg      - An opaque argument that will be provided to the callback
 *              when the transfer complete
 *   timeout  - The timeout value to use.  The transfer will be canceled
 *              and an ETIMEDOUT error will be reported if this timeout
 *              elapsed without completion of the DMA transfer.  Units
 *              are system clock ticks.  Zero means no timeout.
 *
 * Returned Value:
 *   OK on success; a negated errno value on failure.  NOTE:  This function
 *   only enqueues the transfer and returns immediately.  Success here only
 *   means that the transfer was enqueued correctly.
 *
 *   When the transfer is complete, a 'result' value will be provided as
 *   an argument to the callback function that will indicate if the transfer
 *   failed.
 *
 ****************************************************************************/

static int i2s_send(struct i2s_dev_s *dev, struct ap_buffer_s *apb, i2s_callback_t callback, void *arg, uint32_t timeout)
{

	struct s5j_i2s_s *priv = (struct s5j_i2s_s *)dev;
#ifdef I2S_HAVE_TX_P
	struct s5j_buffer_s *bfcontainer;
	irqstate_t flags;
	int ret;
#endif

	DEBUGASSERT(priv && apb);

	i2sinfo("apb=%p nbytes=%d arg=%p timeout=%d\n", apb, apb->nbytes - apb->curbyte, arg, timeout);

	i2s_dump_buffer("Sending", &apb->samp[apb->curbyte], apb->nbytes - apb->curbyte);

#ifdef I2S_HAVE_TX_P
	/* Flush the data cache so that everything is in the physical memory
	 * before starting the DMA.
	 */
	arch_clean_dcache((uintptr_t)&apb->samp[apb->curbyte], 
			(uintptr_t)((void *)&apb->samp[apb->curbyte] + (apb->nbytes - apb->curbyte)));


	/* Allocate a buffer container in advance */

	bfcontainer = i2s_buf_tx_allocate(priv);
	DEBUGASSERT(bfcontainer);

	/* Get exclusive access to the I2S driver data */

	i2s_exclsem_take(priv);

	i2sinfo("TX Exclusive Enter\n");

	/* Has the TX channel been enabled? */

	if (!priv->txpenab) {
		i2serr("ERROR: I2S has no transmitter\n");
		ret = -EAGAIN;
		goto errout_with_exclsem;
	}

	/* Add a reference to the audio buffer */

	apb_reference(apb);

	/* Initialize the buffer container structure */

	bfcontainer->callback = (void *)callback;
	bfcontainer->timeout = timeout;
	bfcontainer->arg = arg;
	bfcontainer->apb = apb;
	bfcontainer->result = -EBUSY;

	/* Prepare DMA microcode */
	i2s_txpdma_prep(priv, bfcontainer);


	/* Add the buffer container to the end of the TX pending queue */

	flags = irqsave();
	sq_addlast((sq_entry_t *) bfcontainer, &priv->txp.pend);
	irqrestore(flags);

	/* Then start the next transfer.  If there is already a transfer in progess,
	 * then this will do nothing.
	 */
	ret = i2s_txp_start(priv);

	DEBUGASSERT(ret == OK);
	i2s_exclsem_give(priv);
	i2sinfo("TX Exclusive Exit\n");

	return OK;

errout_with_exclsem:
	i2s_exclsem_give(priv);
	i2s_buf_tx_free(priv, bfcontainer);
	return ret;

#else
	i2serr("ERROR: I2S has no transmitter\n");
	UNUSED(priv);
	return -ENOSYS;
#endif
}


static int i2s_err_cb_register(struct i2s_dev_s *dev, i2s_err_cb_t cb, void *arg)
{
	struct s5j_i2s_s *priv = (struct s5j_i2s_s *)dev;
	DEBUGASSERT(priv);

	priv->err_cb = cb;
	priv->err_cb_arg = arg;

	/* Set initial state to error, it will be cleaned when operation starts */
#ifdef I2S_HAVE_RX
	priv->rx.error_state = -1;
#endif
#ifdef I2S_HAVE_TX_P
	priv->txp.error_state = -1;
#endif
#ifdef I2S_HAVE_TX_S
	priv->txs.error_state = -1;
#endif

return 0;
}

/****************************************************************************
 * Name: i2s_rx/tx_configure
 *
 * Description:
 *   Configure the i2s receiver and transmitter.
 *
 * Input Parameters:
 *   priv - Fully initialized i2s device structure.
 *
 * Returned Value:
 *   OK is returned on failure.  A negated errno value is returned on failure.
 *
 ****************************************************************************/

static int i2s_rx_cfg_clr(struct s5j_i2s_s *priv)
{
	int i;

	for (i = 0; i < CONFIG_S5J_I2S_MAXINFLIGHT; i++) {
		if (priv->containers_rx[i].dmatask != NULL) {
			s5j_dmatask_free(priv->containers_rx[i].dmatask);
		}
		priv->containers_rx[i].dmatask = NULL;
	}

	priv->rxenab = 0;
	return 0;
}

static int i2s_rx_configure(struct s5j_i2s_s *priv)
{
	int i;

	/* Here I initialie dma task structures for RX channel */
	for (i = 0; i < CONFIG_S5J_I2S_MAXINFLIGHT; i++) {
		priv->containers_rx[i].dmatask = dma_task_p2m_sb_4B_x256_alloc(DMA_I2S_RX);
		if (priv->containers_rx[i].dmatask == NULL) {
			goto err;
		}
	}

	priv->rxenab = 1;
	i2sinfo("i2s_rx_configure success with i2s dev addr 0x%x\n");
	return OK;

err:
	i2s_rx_cfg_clr(priv);

	return -ENOMEM;
}

static int i2s_txp_cfg_clr(struct s5j_i2s_s *priv)
{
	int i;

	for (i = 0; i < CONFIG_S5J_I2S_MAXINFLIGHT; i++) {
		if (priv->containers_tx[i].dmatask != NULL) {
			s5j_dmatask_free(priv->containers_tx[i].dmatask);
		}
		priv->containers_tx[i].dmatask = NULL;
	}

	priv->txpenab = 0;
	return 0;
}

static int i2s_txp_configure(struct s5j_i2s_s *priv)
{
	int i;

	/* Here I initialize dma task structures for TXP channel */
	for (i = 0; i < CONFIG_S5J_I2S_MAXINFLIGHT; i++) {
		priv->containers_tx[i].dmatask = dma_task_m2p_sb_4B_x256_alloc(DMA_I2S_TX);

		if (priv->containers_tx[i].dmatask == NULL) {
			goto err;
		}
	}

	priv->txpenab = 1;
	return OK;

err:
	i2s_txp_cfg_clr(priv);
	return -ENOMEM;
}

static int i2s_txs_configure(struct s5j_i2s_s *priv)
{
	priv->txsenab = 0;
	return OK;
}

/****************************************************************************
 * Name: i2s_dma_allocate
 *
 * Description:
 *   Allocate I2S DMA channels
 *
 * Input Parameters:
 *   priv - Partially initialized I2S device structure.  This function
 *          will complete the DMA specific portions of the initialization
 *
 * Returned Value:
 *   OK on success; A negated errno value on failure.
 *
 ****************************************************************************/

static int i2s_dma_allocate(struct s5j_i2s_s *priv)
{
#ifdef I2S_HAVE_RX
	if (priv->rxenab) {
		/* Allocate an RX DMA channel */

		priv->rx.dma = s5j_dmachannel(CONFIG_I2S_RX_DMACH, "pdma");
		if (!priv->rx.dma) {
			i2serr("ERROR: Failed to allocate the RX DMA channel\n");
			goto errout;
		}

		/* Create a watchdog time to catch RX DMA timeouts */

		priv->rx.dog = wd_create();
		if (!priv->rx.dog) {
			i2serr("ERROR: Failed to create the RX DMA watchdog\n");
			goto errout;
		}
	}
#endif

#ifdef I2S_HAVE_TX_P
	if (priv->txpenab) {
		/* Allocate a TX DMA channel */

		priv->txp.dma = s5j_dmachannel(CONFIG_I2S_TXP_DMACH, "pdma");
		if (!priv->txp.dma) {
			i2serr("ERROR: Failed to allocate the TXP DMA channel\n");
			goto errout;
		}

		/* Create a watchdog time to catch TX DMA timeouts */

		priv->txp.dog = wd_create();
		if (!priv->txp.dog) {
			i2serr("ERROR: Failed to create the TXP DMA watchdog\n");
			goto errout;
		}
	}
#endif

#ifdef I2S_HAVE_TX_S
	if (priv->txsenab) {
		/* Allocate a TX DMA channel */

		priv->txs.dma = s5j_dmachannel(CONFIG_I2S_TXS_DMACH, "pdma");
		if (!priv->txs.dma) {
			i2serr("ERROR: Failed to allocate the TX DMA channel\n");
			goto errout;
		}

		/* Create a watchdog time to catch TX DMA timeouts */

		priv->txs.dog = wd_create();
		if (!priv->txs.dog) {
			i2serr("ERROR: Failed to create the TX DMA watchdog\n");
			goto errout;
		}
	}
#endif

	/* Success exit */

	return OK;

	/* Error exit */

errout:
	i2s_dma_free(priv);
	return -ENOMEM;
}

/****************************************************************************
 * Name: i2s_dma_free
 *
 * Description:
 *   Release DMA-related resources allocated by i2s_dma_allocate()
 *
 * Input Parameters:
 *   priv - Partially initialized I2S device structure.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void i2s_dma_free(struct s5j_i2s_s *priv)
{
#ifdef I2S_HAVE_TX_P
	if (priv->txp.dog) {
		wd_delete(priv->txp.dog);
		priv->txp.dog = NULL;
	}

	if (priv->txp.dma) {
		s5j_dmafree(priv->txp.dma);
		priv->txp.dma = NULL;
	}
#endif

#ifdef I2S_HAVE_TX_S
	if (priv->txs.dog) {
		wd_delete(priv->txs.dog);
		priv->txs.dog = NULL;
	}

	if (priv->txs.dma) {
		s5j_dmafree(priv->txs.dma);
		priv->txs.dma = NULL;
	}
#endif

#ifdef i2s_HAVE_RX
	if (priv->rx.dog) {
		wd_delete(priv->rx.dog);
		priv->rx.dog = NULL;
	}

	if (priv->rx.dma) {
		s5j_dmafree(priv->rx.dma);
		priv->rx.dma = NULL;
	}
#endif
}

/****************************************************************************
 * Name: i2s_configure
 *
 * Description:
 *   Configure i2s
 *
 * Input Parameters:
 *   priv - Partially initialized I2S device structure.  These functions
 *          will complete the I2S specific portions of the initialization
 *
 * Returned Value:
 *   configure status
 *
 ****************************************************************************/

static int i2s_configure(struct s5j_i2s_s *priv)
{
	unsigned int reg;
	int ret;

	/* GPIO initialisation */

	priv->base = S5J_I2S_BASE;

	/* Reset I2S */
	putreg32(0, priv->base + S5J_I2S_CON);
	modifyreg32(priv->base + S5J_I2S_CON, I2S_CR_SW_RST_MASK, I2S_CR_SW_RST_RELEASE);

	/* Set Mode */
	reg = I2S_MOD_OP_CLK_PCLK | I2S_MOD_CDCLKCON_IN | I2S_MOD_MSS_SLAVE | I2S_MOD_RCLKSRC | I2S_MOD_TXR(TXR_TXRX) | I2S_MOD_RFS(RFS_256) | I2S_MOD_BFS(BFS_64);
	putreg32(reg, priv->base + S5J_I2S_MOD);

	ret = s5j_configgpio(GPIO_I2S0_BCLK);
	if (ret < 0) {
		return ret;
	}
	ret = s5j_configgpio(GPIO_I2S0_LRCK);
	if (ret < 0) {
		return ret;
	}
	ret = s5j_configgpio(GPIO_I2S0_SDO);
	if (ret < 0) {
		return ret;
	}
	ret = s5j_configgpio(GPIO_I2S0_SDI);
	if (ret < 0) {
		return ret;
	}

	s5j_clk_mux_select(CLK_MUX_I2SB, CLK_MUX_SELECT_BCLK);

	/* We are always slave here ... Below code should set priv structure */
	/* Set priv structure here */
	priv->datalen = CONFIG_S5J_I2S_DATALEN;
	priv->samplerate = CONFIG_S5J_I2S_SAMPLERATE;
	return 0;
}

static int i2s_irq_handler(int irq, FAR void *context, FAR void *arg)
{
	volatile u32 reg;
	struct s5j_i2s_s *priv = (struct s5j_i2s_s *)arg;
	struct s5j_buffer_s *bfcontainer;

	/* Check faults here */
	reg = getreg32(priv->base + S5J_I2S_CON);

#ifdef I2S_HAVE_RX
	/* This is the case, when there is a new transfer series 
	 * is initiated right before RX FIFO OVF 
	 * It can be a new series or delay/lag in users application receiver.
	*/	

	if (!sq_empty(&priv->rx.pend) && sq_empty(&priv->rx.act)) {
		/* Remove the next buffer container from the tx.pend list */
		bfcontainer = (struct s5j_buffer_s *)sq_remfirst(&priv->rx.pend);

		/* Add the completed buffer container to the tail of the tx.act queue */
		sq_addlast((sq_entry_t *) bfcontainer, &priv->rx.act);

		/* Start next DMA transfer */
		s5j_dmastart(priv->rx.dma, bfcontainer->dmatask);
		
		/* We don't stop I2S operation, report anyways about OVF */
		if (reg & I2S_CR_FRXOFSTATUS) {
			if (priv->err_cb) {
				priv->err_cb((struct i2s_dev_s *)priv, priv->err_cb_arg, I2S_ERR_CB_RX);
			}
		}
	} else {
		/* If OVF we report */
		if (reg & I2S_CR_FRXOFSTATUS) {
			modifyreg32(priv->base + S5J_I2S_CON, I2S_CR_FRXOFINTEN, 0);
			modifyreg32(priv->base + S5J_I2S_CON, 0, I2S_CR_FRXOFSTATUS);
			modifyreg32(priv->base + S5J_I2S_CON, I2S_CR_RXDMACTIVE, 0);
			if (priv->err_cb) {
				priv->err_cb((struct i2s_dev_s *)priv, priv->err_cb_arg, I2S_ERR_CB_RX);
			}
		}
	}
#endif

#ifdef I2S_HAVE_TX_P
	/* This is the case, when there is a new transfer series 
	 * is initiated right before TX FIFO UNF 
	 * It can be a new series or delay/lag in users application receiver.
	*/	
	if (!sq_empty(&priv->txp.pend) && sq_empty(&priv->txp.act)) {
		/* Remove the next buffer container from the tx.pend list */
		bfcontainer = (struct s5j_buffer_s *)sq_remfirst(&priv->txp.pend);

		/* Add the completed buffer container to the tail of the tx.act queue */
		sq_addlast((sq_entry_t *) bfcontainer, &priv->txp.act);

		/* Start next DMA transfer */
		s5j_dmastart(priv->txp.dma, bfcontainer->dmatask);

	
		/* We don't stop I2S operation, report anyways about UNF */
		if (reg & I2S_CR_FTXURSTATUS) {
			if (priv->err_cb) {
				priv->err_cb((struct i2s_dev_s *)priv, priv->err_cb_arg, I2S_ERR_CB_TX);
			}
		}
	} else {
		/* If UNF we report */
		if (reg & I2S_CR_FTXURSTATUS) {
			modifyreg32(priv->base + S5J_I2S_CON, I2S_CR_FTXURINTEN, 0);
			modifyreg32(priv->base + S5J_I2S_CON, 0, I2S_CR_FTXURSTATUS);
			modifyreg32(priv->base + S5J_I2S_CON, I2S_CR_TXDMACTIVE, 0);
			if (priv->err_cb) {
				priv->err_cb((struct i2s_dev_s *)priv, priv->err_cb_arg, I2S_ERR_CB_TX);
			}
		}
	}
#endif


	reg = getreg32(priv->base + S5J_I2S_CON);

	if ((reg & (I2S_CR_TXDMACTIVE | I2S_CR_RXDMACTIVE)) == 0) {
		modifyreg32(priv->base + S5J_I2S_CON, I2S_CR_I2SACTIVE, 0);
	}


	return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name:
 *
 * Description:
 *   Initialize the selected i2s port
 *
 * Input Parameter:
 *   none
 *
 * Returned Value:
 *   Valid i2s device structure reference on succcess; a NULL on failure
 *
 ****************************************************************************/

struct i2s_dev_s *s5j_i2s_initialize(uint16_t port)
{
	if (port >= S5J_I2S_MAXPORTS) {
		i2serr("ERROR: Port number outside the allowed port number range\n");
		return NULL;
	}
	if (g_i2sdevice[port] != NULL) {
		return &g_i2sdevice[port]->dev;
	}

	struct s5j_i2s_s *priv;
	int ret;

	/* Allocate a new state structure for this chip select.  NOTE that there
	 * is no protection if the same chip select is used in two different
	 * chip select structures.
	 */
	priv = (struct s5j_i2s_s *)zalloc(sizeof(struct s5j_i2s_s));
	if (!priv) {
		i2serr("ERROR: Failed to allocate a chip select structure\n");
		return NULL;
	}
	priv->isr_num = IRQ_I2S;
	priv->isr_handler = i2s_irq_handler;

	/* Initialize the I2S priv device structure  */
	sem_init(&priv->exclsem, 0, 1);
	priv->dev.ops = &g_i2sops;

	/* Initialize buffering */
	i2s_buf_rx_initialize(priv);
	i2s_buf_tx_initialize(priv);

	/* configure i2s port */
	ret = i2s_configure(priv);

#ifdef I2S_HAVE_RX
	/* Configure the receiver */

	ret = i2s_rx_configure(priv);
	if (ret < 0) {
		i2serr("ERROR: Failed to configure the receiver: %d\n", ret);
		goto errout_with_clocking;
	}
#endif

#ifdef I2S_HAVE_TX_P
	/* Configure the transmitter */

	ret = i2s_txp_configure(priv);
	if (ret < 0) {
		i2serr("ERROR: Failed to configure the primary transmitter: %d\n", ret);
		goto errout_with_clocking;
	}
#endif

#ifdef I2S_HAVE_TX_S
	ret = i2s_txs_configure(priv);
	if (ret < 0) {
		i2serr("ERROR: Failed to configure the secondary transmitter: %d\n", ret);
		goto errout_with_clocking;
	}
#endif

	/* Allocate DMA channels */
	ret = i2s_dma_allocate(priv);
	if (ret < 0) {
		goto errout_with_alloc;
	}

	irq_attach(priv->isr_num, priv->isr_handler, priv);
	up_enable_irq(priv->isr_num);

	g_i2sdevice[port] = priv;

	/* Success exit */
	return &priv->dev;

	/* Failure exits */

errout_with_alloc:
	i2s_rx_cfg_clr(priv);
	i2s_txp_cfg_clr(priv);

errout_with_clocking:

	sem_destroy(&priv->exclsem);
	kmm_free(priv);
	return NULL;
}

#endif							/* I2S_HAVE_RX || I2S_HAVE_TX_P I2S_HAVE_TX_S */
