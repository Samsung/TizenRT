/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <errno.h>
#include <assert.h>
#include <queue.h>
#include <debug.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/kmalloc.h>
#include <tinyara/wdog.h>
#include <tinyara/wqueue.h>
#include <tinyara/audio/audio.h>
#include <tinyara/audio/i2s.h>
#ifdef CONFIG_AUDIO_ALC1019
#include <tinyara/audio/alc1019.h>
#endif

#include "irq/irq.h"

#include <arch/board/board.h>
#include "amebasmart_i2s.h"

#include "PinNames.h"
#include "i2s_api.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
// #ifndef CONFIG_SCHED_WORKQUEUE
// #error Work queue support is required (CONFIG_SCHED_WORKQUEUE)
// #endif

#ifdef CONFIG_AUDIO
// #ifndef CONFIG_AUDIO
// #error CONFIG_AUDIO required by this driver
// #endif

#ifndef CONFIG_AMEBASMART_I2S_MAXINFLIGHT
#define CONFIG_AMEBASMART_I2S_MAXINFLIGHT 32
#endif

/* Assume no RX/TX support until we learn better */
#undef I2S_HAVE_RX
#undef I2S_HAVE_TX

#if defined(CONFIG_AMEBASMART_I2S)

/* Check for I2S RX support */
#if defined(CONFIG_AMEBASMART_I2S_RX)
#define I2S_HAVE_RX 1
#endif

/* Check for I2S TX support */
#if defined(CONFIG_AMEBASMART_I2S_TX)
#define I2S_HAVE_TX 1
#endif

#ifndef CONFIG_AMEBASMART_I2S_DATALEN
#define CONFIG_AMEBASMART_I2S_DATALEN (I2S_BITS_PER_SAMPLE_16BIT)
#endif

#ifndef CONFIG_AMEBASMART_I2S_SAMPLERATE
#define CONFIG_AMEBASMART_I2S_SAMPLERATE (SR_44p1KHZ)
#endif

#endif

#ifdef CONFIG_DEBUG
#define CONFIG_DEBUG_FEATURES
#endif

#define DEBUG_I2S_DRIVER 0

#undef i2serr
#undef i2sinfo
#if defined(DEBUG_I2S_DRIVER) && (0 < DEBUG_I2S_DRIVER)
#define i2sinfo(format, ...) printf(format, ##__VA_ARGS__)
#define i2serr(format, ...) printf(format, ##__VA_ARGS__)
#else
#define i2sinfo(format, ...)
#endif
#define i2serr(format, ...) printf(format, ##__VA_ARGS__)

#if defined(I2S_HAVE_RX) || defined(I2S_HAVE_TX)

#ifndef MIN
#define MIN(a, b) a < b ? a : b
#endif

#define OVER_SAMPLE_RATE (384U)

#define I2S_DMA_PAGE_SIZE 2048	/* 4 ~ 16384, set to a factor of APB size */
#define I2S_DMA_PAGE_NUM 4	/* Vaild number is 2~4 */

struct amebasmart_buffer_s {
	struct amebasmart_buffer_s *flink; /* Supports a singly linked list */
	i2s_callback_t callback;		   /* Function to call when the transfer completes */
	uint32_t timeout;				   /* The timeout value to use with DMA transfers */
	void *arg;						   /* The argument to be returned with the callback */
	struct ap_buffer_s *apb;		   /* The audio buffer */
	int result;						   /* The result of the transfer */
};

/* This structure describes the state of one receiver or transmitter transport */

struct amebasmart_transport_s {
	WDOG_ID dog;		/* Watchdog that handles DMA timeouts */
	sq_queue_t pend;	/* A queue of pending transfers */
	sq_queue_t act;		/* A queue of active transfers */
	sq_queue_t done;	/* A queue of completed transfers */
	struct work_s work; /* Supports worker thread operations */
	int error_state;	/* Channel error state, 0 - OK*/
};

/* I2S configuration parameters for i2s_param_config function */
typedef struct {

	int sample_rate;					   /*!< I2S sample rate */
	i2s_bits_per_sample_t bits_per_sample; /*!< I2S bits per sample */

	uint8_t channel_num;
} i2s_config_t;

/* I2S Device hardware configuration */
struct amebasmart_i2s_config_s {
	uint32_t i2s_mclk_pin;
	uint32_t i2s_sclk_pin;
	uint32_t i2s_ws_pin;
	uint32_t i2s_sd_tx_pin;
	uint32_t i2s_sd_rx_pin;

	uint8_t i2s_idx;	/* I2S index*/
	uint8_t rxenab : 1; /* True: RX transfers enabled */
	uint8_t txenab : 1; /* True: TX transfers enabled */
};

/* The state of the one I2S peripheral */
struct amebasmart_i2s_s {
	struct i2s_dev_s dev; /* Externally visible I2S interface, must the first element!! */

	i2s_t *i2s_object;
	const struct amebasmart_i2s_config_s *config;	/* Port configuration */

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	uint8_t i2s_tx_buf[(I2S_DMA_PAGE_NUM + 1) * I2S_DMA_PAGE_SIZE]__attribute__((aligned(64))); /* Allocate buffer for use in TX, must I2S_DMA_PAGE_NUM+1 for zero buffer */
	i2s_irq_handler tx_isr_handler;
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	//uint8_t i2s_rx_buf[(I2S_DMA_PAGE_NUM + 1) * I2S_DMA_PAGE_SIZE]__attribute__((aligned(64))); /* Allocate buffer for use in RX, must I2S_DMA_PAGE_NUM+1 for zero buffer */
	uint8_t* i2s_rx_buf;
	i2s_irq_handler rx_isr_handler;
#endif

	i2s_err_cb_t err_cb; /* registered error callback function */
	void *err_cb_arg;	 /* argiment to return with err cb call */

	sem_t exclsem; /* Assures mutually exclusive acess to I2S */

	uint8_t rxenab : 1; /* True: RX transfers enabled */
	uint8_t txenab : 1; /* True: TX transfers enabled */

	int sample_rate;			/*!< I2S sample rate */
	int channel_num;			/*!< Number of channels */
	int bits_per_sample;		/*!< Bits per sample */
	struct ap_buffer_s *apb_tx; /* Pointer to application TX audio buffer to track current TX byte count */

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	struct amebasmart_transport_s rx; /* RX transport state */
	sem_t bufsem_rx;						 /* Buffer wait semaphore */
	struct amebasmart_buffer_s *freelist_rx; /* A list a free buffer containers */
	struct amebasmart_buffer_s containers_rx[I2S_DMA_PAGE_NUM];
#endif

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	struct amebasmart_transport_s tx; /* TX primary transport state */
	sem_t bufsem_tx;						 /* Buffer wait semaphore */
	struct amebasmart_buffer_s *freelist_tx; /* A list a free buffer containers */
	struct amebasmart_buffer_s containers_tx[I2S_DMA_PAGE_NUM];
#endif
};

/* I2S device structures */
static const struct amebasmart_i2s_config_s amebasmart_i2s2_config = {
	.i2s_mclk_pin = PB_22,
	.i2s_sclk_pin = PB_21,
	.i2s_ws_pin = PA_16,
	.i2s_sd_tx_pin = PB_10,
	.i2s_sd_rx_pin = PB_19,

	.i2s_idx = I2S_NUM_2,
	.rxenab = 0,
	.txenab = 1,
};

static const struct amebasmart_i2s_config_s amebasmart_i2s3_config = {
	.i2s_mclk_pin = PA_15,
	.i2s_sclk_pin = PA_14,
	.i2s_ws_pin = PA_13,
	.i2s_sd_tx_pin = PB_11,
	.i2s_sd_rx_pin = PB_20,

	.i2s_idx = I2S_NUM_3,
	.rxenab = 1,
	.txenab = 0,
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* Register helpers */
#define CONFIG_AMEBASMART_I2S_DUMPBUFFERS 0

#if defined(CONFIG_AMEBASMART_I2S_DUMPBUFFERS) && (0 < CONFIG_AMEBASMART_I2S_DUMPBUFFERS)
#define i2s_init_buffer(b, s) memset(b, 0x55, s);
#define i2s_dump_buffer(m, b, s) lib_dumpbuffer(m, b, s)
#else
#define i2s_init_buffer(b, s)
#define i2s_dump_buffer(m, b, s)
#endif

/* Semaphore helpers */
static void i2s_exclsem_take(struct amebasmart_i2s_s *priv);
#define i2s_exclsem_give(priv) sem_post(&priv->exclsem)

/* Buffer container helpers */
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
static void i2s_rxdma_callback(struct amebasmart_i2s_s *priv, int result);
static void i2s_rxdma_timeout(int argc, uint32_t arg);
static int i2s_rxdma_prep(struct amebasmart_i2s_s *priv, struct amebasmart_buffer_s *bfcontainer);
static int i2s_rx_start(struct amebasmart_i2s_s *priv);
static void i2s_rx_worker(void *arg);
static void i2s_rx_schedule(struct amebasmart_i2s_s *priv, int result);

static void i2s_bufsem_rx_take(struct amebasmart_i2s_s *priv);
#define i2s_bufsem_rx_give(priv) sem_post(&priv->bufsem_rx)

static struct amebasmart_buffer_s *i2s_buf_rx_allocate(struct amebasmart_i2s_s *priv);
static void i2s_buf_rx_free(struct amebasmart_i2s_s *priv, struct amebasmart_buffer_s *bfcontainer);
static void i2s_buf_rx_initialize(struct amebasmart_i2s_s *priv);

#endif

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
static void i2s_txdma_callback(struct amebasmart_i2s_s *priv, int result);
static void i2s_txdma_timeout(int argc, uint32_t arg);
static int i2s_tx_start(struct amebasmart_i2s_s *priv);
static void i2s_tx_worker(void *arg);
static void i2s_tx_schedule(struct amebasmart_i2s_s *priv, int result);

static void i2s_bufsem_tx_take(struct amebasmart_i2s_s *priv);
#define i2s_bufsem_tx_give(priv) sem_post(&priv->bufsem_tx)

static struct amebasmart_buffer_s *i2s_buf_tx_allocate(struct amebasmart_i2s_s *priv);
static void i2s_buf_tx_free(struct amebasmart_i2s_s *priv, struct amebasmart_buffer_s *bfcontainer);
static void i2s_buf_tx_initialize(struct amebasmart_i2s_s *priv);

#endif

/* I2S methods (and close friends) */

static uint32_t i2s_rxdatawidth(struct i2s_dev_s *dev, int bits);

static uint32_t i2s_samplerate(struct i2s_dev_s *dev, uint32_t rate);
static uint32_t i2s_txdatawidth(struct i2s_dev_s *dev, int bits);
static int i2s_stop_transfer(struct i2s_dev_s *dev, i2s_ch_dir_t dir);
static int i2s_stop(struct i2s_dev_s *dev, i2s_ch_dir_t dir);
static int i2s_pause(struct i2s_dev_s *dev, i2s_ch_dir_t dir);
static int i2s_resume(struct i2s_dev_s *dev, i2s_ch_dir_t dir);
static int i2s_receive(struct i2s_dev_s *dev, struct ap_buffer_s *apb, i2s_callback_t callback, void *arg, uint32_t timeout);
static int i2s_send(struct i2s_dev_s *dev, struct ap_buffer_s *apb, i2s_callback_t callback, void *arg, uint32_t timeout);
static int i2s_err_cb_register(struct i2s_dev_s *dev, i2s_err_cb_t cb, void *arg);

/****************************************************************************
 * External functions
 ****************************************************************************/

static const i2s_config_t i2s_default_config = {

	.sample_rate = SP_48K,
	.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,

	.channel_num = 2,
};

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

static struct amebasmart_i2s_s *g_i2sdevice[I2S_NUM_MAX] = {NULL};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)

/****************************************************************************
 * Name: i2s_txdma_callback
 *
 * Description:
 *   This callback function is invoked at the completion of the I2S TX DMA.
 *
 * Input Parameters:
 *   priv - amebasmart private i2s data
 *   result - The result of the DMA transfer
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void i2s_txdma_callback(struct amebasmart_i2s_s *priv, int result)
{
	DEBUGASSERT(priv != NULL);

	/* Cancel the watchdog timeout */

	(void)wd_cancel(priv->tx.dog);

	/* Then schedule completion of the transfer to occur on the worker thread */

	i2s_tx_schedule(priv, result);
}

static void i2s_txdma_timeout(int argc, uint32_t arg)
{
	struct amebasmart_i2s_s *priv = (struct amebasmart_i2s_s *)arg;
	DEBUGASSERT(priv != NULL);

	/* Then schedule completion of the transfer to occur on the worker thread.
	 * Set the result with -ETIMEDOUT.
	 */
	i2s_tx_schedule(priv, -ETIMEDOUT);
}

static int amebasmart_i2s_tx(struct amebasmart_i2s_s *priv, struct amebasmart_buffer_s *bfcontainer)
{
	int ret = OK;
	int *ptx_buf;
	int tx_size;

#ifdef CONFIG_PM
	bsp_pm_domain_control(BSP_I2S_DRV, 1);
#endif
	tx_size = I2S_DMA_PAGE_SIZE; /* Track current byte size to increment by */
	struct ap_buffer_s *apb;
	if (NULL != bfcontainer && NULL != bfcontainer->apb) {

		apb = bfcontainer->apb;
		/* Add the container to the list of active DMAs */
		sq_addlast((sq_entry_t *)bfcontainer, &priv->tx.act);

		priv->apb_tx = apb; /* Reference current TX apb in our I2S struct */
							/* Start sending first page, after that the txdma callback will be called in the tx irq handler */
		ptx_buf = i2s_get_tx_page(priv->i2s_object);

		if ((apb->nbytes - apb->curbyte) <= tx_size) {
			tx_size = apb->nbytes - apb->curbyte;
			memset(ptx_buf, 0, I2S_DMA_PAGE_SIZE); /* Clear ptx_buf to prevent sending old data since we are sending less than I2S_DMA_PAGE_SIZE */
			memcpy((void *)ptx_buf, (void *)&apb->samp[apb->curbyte], tx_size);
		} else {
			memcpy((void *)ptx_buf, (void *)&apb->samp[apb->curbyte], I2S_DMA_PAGE_SIZE);
		}
		apb->curbyte += tx_size; /* No padding, ptx_buf is big enough to fill the whole tx_size */

		i2s_enable(priv->i2s_object);
		i2s_send_page(priv->i2s_object, (uint32_t *)ptx_buf);
	} else {

		ret = -1;
		i2serr("ERROR: bfcontainer or bfcontainer->apb is NULL\n");
	}
#ifdef CONFIG_PM
	bsp_pm_domain_control(BSP_I2S_DRV, 0);
#endif
	return ret;
}

static int i2s_tx_start(struct amebasmart_i2s_s *priv)
{
	struct amebasmart_buffer_s *bfcontainer = NULL;
	int ret;
	irqstate_t flags;

	//struct ap_buffer_s *apb;

	/* Check if the DMA is IDLE */
	if (!sq_empty(&priv->tx.act)) {
		i2sinfo("[TX] actived!\n");
		return OK;
	}

	/* If there are no pending transfer, then bail returning success */
	if (sq_empty(&priv->tx.pend)) {
		i2sinfo("[TX] empty!\n");
		return OK;
	}

	flags = enter_critical_section();

	/* Remove the pending TX transfer at the head of the TX pending queue. */
	bfcontainer = (struct amebasmart_buffer_s *)sq_remfirst(&priv->tx.pend);

	/* Start first transfer */
	amebasmart_i2s_tx(priv, bfcontainer);
	leave_critical_section(flags);

	/* Start a watchdog to catch DMA timeouts */
	if (bfcontainer->timeout > 0) {
		ret = wd_start(priv->tx.dog, bfcontainer->timeout, (wdentry_t)i2s_txdma_timeout, 1, (uint32_t)priv);

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

static void i2s_tx_worker(void *arg)
{
	struct amebasmart_i2s_s *priv = (struct amebasmart_i2s_s *)arg;
	struct amebasmart_buffer_s *bfcontainer;
	irqstate_t flags;

	DEBUGASSERT(priv);

	/* When the transfer was started, the active buffer containers were removed
	 * from the tx.pend queue and saved in the tx.act queue.  We get here when the
	 * DMA is finished... either successfully, with a DMA error, or with a DMA
	 * timeout.
	 *
	 * In any case, the buffer containers in tx.act will be moved to the end
	 * of the tx.done queue and tx.act will be emptied before this worker is
	 * started.
	 */

	i2sinfo("tx.act.head=%p tx.done.head=%p\n", priv->tx.act.head, priv->tx.done.head);

	/* Process each buffer in the tx.done queue */
	while (sq_peek(&priv->tx.done) != NULL) {
		/* Remove the buffer container from the tx.done queue.  NOTE that
		 * interrupts must be enabled to do this because the tx.done queue is
		 * also modified from the interrupt level.
		 */

		flags = enter_critical_section();
		bfcontainer = (struct amebasmart_buffer_s *)sq_remfirst(&priv->tx.done);
		leave_critical_section(flags);
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

static void i2s_tx_schedule(struct amebasmart_i2s_s *priv, int result)
{
	struct amebasmart_buffer_s *bfcontainer;
	int ret;

	/* Upon entry, the transfer(s) that just completed are the ones in the
	 * priv->tx.act queue.  NOTE: In certain conditions, this function may
	 * be called an additional time, hence, we can't assert this to be true.
	 * For example, in the case of a timeout, this function will be called by
	 * both indirectly via the amebasmart_dmastop() logic and directly via the
	 * i2s_txdma_timeout() logic.
	 */

	/* Move all entries from the tx.act queue to the tx.done queue */

	if (!sq_empty(&priv->tx.act)) {
		/* Remove the next buffer container from the tx.act list */

		bfcontainer = (struct amebasmart_buffer_s *)sq_remfirst(&priv->tx.act);

		/* Report the result of the transfer */

		bfcontainer->result = result;

		/* Add the completed buffer container to the tail of the tx.done queue */

		sq_addlast((sq_entry_t *)bfcontainer, &priv->tx.done);
	}

	if (!sq_empty(&priv->tx.pend)) {
		/* Remove the next buffer container from the tx.pend list */
		bfcontainer = (struct amebasmart_buffer_s *)sq_remfirst(&priv->tx.pend);

		/* Start next transfer */
		amebasmart_i2s_tx(priv, bfcontainer);
	}

	/* If the worker has completed running, then reschedule the working thread.
	 * REVISIT:  There may be a race condition here.  So we do nothing if the
	 * worker is not available.
	 */

	if (work_available(&priv->tx.work)) {
		/* Schedule the TX DMA done processing to occur on the worker thread. */

		ret = work_queue(HPWORK, &priv->tx.work, i2s_tx_worker, priv, 0);
		if (ret != 0) {
			i2serr("ERROR: Failed to queue TX primary work: %d\n", ret);
		}
	}
}
#endif

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
#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	struct amebasmart_i2s_s *priv = (struct amebasmart_i2s_s *)dev;

	/* Support 16, 24, 32 bits */
	DEBUGASSERT(priv && (bits == I2S_BITS_PER_SAMPLE_16BIT || bits == I2S_BITS_PER_SAMPLE_32BIT || bits == I2S_BITS_PER_SAMPLE_24BIT));

	priv->bits_per_sample = bits;

	/* amebasmart 16, 24, 32, bits setting */
	if (bits == I2S_BITS_PER_SAMPLE_16BIT)
		priv->i2s_object->word_length = WL_16b;
	else if (bits == I2S_BITS_PER_SAMPLE_24BIT)
		priv->i2s_object->word_length = WL_24b;
	else if (bits == I2S_BITS_PER_SAMPLE_32BIT)
		priv->i2s_object->word_length = WL_32b;

	i2s_set_param(priv->i2s_object, priv->i2s_object->channel_num, priv->i2s_object->sampling_rate, priv->i2s_object->word_length);

	return priv->bits_per_sample * priv->sample_rate;
#endif

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
	struct amebasmart_i2s_s *priv = (struct amebasmart_i2s_s *)dev;

	DEBUGASSERT(priv && apb);

	// i2sinfo("[I2S TX] apb=%p nbytes=%d samp=%p arg=%p timeout=%d\n", apb, apb->nbytes - apb->curbyte, apb->samp, arg, timeout);
	i2s_dump_buffer("Sending", &apb->samp[apb->curbyte], apb->nbytes - apb->curbyte);
#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	
	struct amebasmart_buffer_s *bfcontainer;
	irqstate_t flags;
	int ret;

	/* Has the TX channel been enabled? */
	if (!priv->txenab) {
		i2serr("ERROR: I2S has no transmitter\n");
		return -EAGAIN;
	}

	/* Allocate a buffer container in advance */

	bfcontainer = i2s_buf_tx_allocate(priv);

	i2s_exclsem_take(priv);
	i2sinfo("TX Exclusive Enter\n");

	apb_reference(apb);

	/* Initialize the buffer container structure */

	bfcontainer->callback = (void *)callback;
	bfcontainer->timeout = timeout;
	bfcontainer->arg = arg;
	bfcontainer->apb = apb;
	bfcontainer->result = -EBUSY;

	flags = enter_critical_section();
	sq_addlast((sq_entry_t *)bfcontainer, &priv->tx.pend);
	leave_critical_section(flags);

	ret = i2s_tx_start(priv);

	i2s_exclsem_give(priv);
	i2sinfo("TX Exclusive Exit\n");

	return OK;

#else
	i2serr("ERROR: I2S has no transmitter\n");
	UNUSED(priv);
	return -ENOSYS;
#endif
}

/*!
 * @brief Tx interrupt handler.
 *
 * @param base pointer.
 * @param handle Pointer to the sai_handle_t structure.
 */

void i2s_transfer_tx_handleirq(void *data, char *pbuf)
{
	/* This handler is called after every completion of i2s_send_page() */
	struct amebasmart_i2s_s *priv = (struct amebasmart_i2s_s *)data;
	int tx_size;

#ifdef CONFIG_PM
	bsp_pm_domain_control(BSP_I2S_DRV, 1);
#endif
	tx_size = I2S_DMA_PAGE_SIZE;							   /* Track current byte size to increment by */
	if ((priv->apb_tx->nbytes - priv->apb_tx->curbyte) <= 0) { /* Condition to stop sending if all data in the buffer has been sent */
		int result = OK;
		i2s_txdma_callback(priv, result);
	} else {
		int *ptx_buf;
		ptx_buf = i2s_get_tx_page(priv->i2s_object);

		if ((priv->apb_tx->nbytes - priv->apb_tx->curbyte) <= tx_size) {
			tx_size = priv->apb_tx->nbytes - priv->apb_tx->curbyte;
			memset(ptx_buf, 0, I2S_DMA_PAGE_SIZE); /* Clear ptx_buf to prevent sending old data since we are sending less than I2S_DMA_PAGE_SIZE */
			memcpy((void *)ptx_buf, (void *)&priv->apb_tx->samp[priv->apb_tx->curbyte], tx_size);
		} else {
			memcpy((void *)ptx_buf, (void *)&priv->apb_tx->samp[priv->apb_tx->curbyte], I2S_DMA_PAGE_SIZE);
		}
		priv->apb_tx->curbyte += tx_size; /* No padding, ptx_buf is big enough to fill the whole tx_size */
		i2s_send_page(priv->i2s_object, (uint32_t *)ptx_buf);
	}
#ifdef CONFIG_PM
	bsp_pm_domain_control(BSP_I2S_DRV, 0);
#endif
}

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
/****************************************************************************
 * Name: i2s_rxdma_callback
 *
 * Description:
 *   This callback function is invoked at the completion of the I2S RX DMA.
 *
 * Input Parameters:
 *   priv - amebasmart i2s private data
 *   result - The result of the DMA transfer
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void i2s_rxdma_callback(struct amebasmart_i2s_s *priv, int result)
{
	DEBUGASSERT(priv != NULL);

	/* Cancel the watchdog timeout */
	(void)wd_cancel(priv->rx.dog);

	/* Then schedule completion of the transfer to occur on the worker thread */
	i2s_rx_schedule(priv, result);
}

static void i2s_rxdma_timeout(int argc, uint32_t arg)
{
	struct amebasmart_i2s_s *priv = (struct amebasmart_i2s_s *)arg;
	DEBUGASSERT(priv != NULL);

	/* Timeout: set the result to -ETIMEDOUT. */
	i2s_rx_schedule(priv, -ETIMEDOUT);
}

static int i2s_rxdma_prep(struct amebasmart_i2s_s *priv, struct amebasmart_buffer_s *bfcontainer)
{
	struct ap_buffer_s *apb;

	apb = bfcontainer->apb;

	/* No data received yet */
	apb->nbytes = 0;
	apb->curbyte = 0;

	priv->i2s_rx_buf = apb->samp;
	i2s_set_dma_buffer(priv->i2s_object, NULL, (char *)priv->i2s_rx_buf, I2S_DMA_PAGE_NUM, I2S_DMA_PAGE_SIZE);

	return 0;
}

static int i2s_rx_start(struct amebasmart_i2s_s *priv)
{
	struct amebasmart_buffer_s *bfcontainer = NULL;
	int ret;
	irqstate_t flags;

	/* Check if the DMA is IDLE */
	if (!sq_empty(&priv->rx.act)) {
		i2sinfo("[RX start] RX active!\n");
		return OK;
	}

	/* If there are no pending transfer, then bail returning success */
	if (sq_empty(&priv->rx.pend)) {
		i2sinfo("[RX start] RX pend empty!\n");
		return OK;
	}

	flags = enter_critical_section();
	/* Remove the pending RX transfer at the head of the RX pending queue. */
	bfcontainer = (struct amebasmart_buffer_s *)sq_remfirst(&priv->rx.pend);

	if (bfcontainer != NULL && NULL != bfcontainer->apb) {

		/* Add the container to the list of active DMAs */
		sq_addlast((sq_entry_t *)bfcontainer, &priv->rx.act);

		i2s_recv_page(priv->i2s_object);
	}
	leave_critical_section(flags);

	/* Start a watchdog to catch DMA timeouts */
	if (bfcontainer->timeout > 0) {
		ret = wd_start(priv->rx.dog, bfcontainer->timeout, (wdentry_t)i2s_rxdma_timeout, 1, (uint32_t)priv);

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

static void i2s_rx_worker(void *arg)
{
	struct amebasmart_i2s_s *priv = (struct amebasmart_i2s_s *)arg;
	struct ap_buffer_s *apb;
	struct amebasmart_buffer_s *bfcontainer;
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
		flags = enter_critical_section();
		bfcontainer = (struct amebasmart_buffer_s *)sq_remfirst(&priv->rx.done);
		leave_critical_section(flags);

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

static void i2s_rx_schedule(struct amebasmart_i2s_s *priv, int result)
{
	struct amebasmart_buffer_s *bfcontainer;
	int ret;

	/* Upon entry, the transfer(s) that just completed are the ones in the
	 * priv->rx.act queue.  NOTE: In certain conditions, this function may
	 * be called an additional time, hence, we can't assert this to be true.
	 * For example, in the case of a timeout, this function will be called by
	 * both indirectly via the amebasmart_dmastop() logic and directly via the
	 * i2s_rxdma_timeout() logic.
	 */
	/* Move first entry from the rx.act queue to the rx.done queue */
	if (!sq_empty(&priv->rx.act)) {
		/* Remove the next buffer container from the rx.act list */
		bfcontainer = (struct amebasmart_buffer_s *)sq_remfirst(&priv->rx.act);

		/* Report the result of the transfer */
		bfcontainer->result = result;

		/* Add the completed buffer container to the tail of the rx.done queue */
		sq_addlast((sq_entry_t *)bfcontainer, &priv->rx.done);
	}

	if (!sq_empty(&priv->rx.pend)) {
		/* Remove the next buffer container from the tx.pend list */
		bfcontainer = (struct amebasmart_buffer_s *)sq_remfirst(&priv->rx.pend);

		/* Add the completed buffer container to the tail of the tx.act queue */
		sq_addlast((sq_entry_t *)bfcontainer, &priv->rx.act);
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
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	struct amebasmart_i2s_s *priv = (struct amebasmart_i2s_s *)dev;

	/* Support 16, 24, 32 bits */
	DEBUGASSERT(priv && (bits == I2S_BITS_PER_SAMPLE_16BIT || bits == I2S_BITS_PER_SAMPLE_32BIT || bits == I2S_BITS_PER_SAMPLE_24BIT));

	priv->bits_per_sample = bits;

	/* amebasmart 16, 24, 32, bits setting */
	if (bits == I2S_BITS_PER_SAMPLE_16BIT)
		priv->i2s_object->word_length = WL_16b;
	else if (bits == I2S_BITS_PER_SAMPLE_24BIT)
		priv->i2s_object->word_length = WL_24b;
	else if (bits == I2S_BITS_PER_SAMPLE_32BIT)
		priv->i2s_object->word_length = WL_32b;

	i2s_set_param(priv->i2s_object, priv->i2s_object->channel_num, priv->i2s_object->sampling_rate, priv->i2s_object->word_length);

	return priv->bits_per_sample * priv->sample_rate;
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
 *   apb      - A pointer to the audio buffer in which to receive data
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
	struct amebasmart_i2s_s *priv = (struct amebasmart_i2s_s *)dev;

	i2sinfo("[I2S RX] apb=%p nmaxbytes=%d samp=%p arg=%p timeout=%d\n", apb, apb->nmaxbytes, apb->samp, arg, timeout);

	/* apb samp is the buffer user use to receive, nmaxbyte should be less than I2S_DMA_PAGE_NUM*I2S_DMA_PAGE_SIZE */
	i2s_init_buffer(apb->samp, apb->nmaxbytes);

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	struct amebasmart_buffer_s *bfcontainer;
	irqstate_t flags;
	int ret;
	printf("i2s_receive\n");
	/* Has the RX channel been enabled? */
	if (!priv->rxenab) {
		i2serr("ERROR: I2S has no receiver\n");
		return -EAGAIN;
	}

	bfcontainer = i2s_buf_rx_allocate(priv);
	DEBUGASSERT(bfcontainer);

	i2s_exclsem_take(priv);
	i2sinfo("RX Exclusive Enter\n");

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

	flags = enter_critical_section();
	sq_addlast((sq_entry_t *)bfcontainer, &priv->rx.pend);
	leave_critical_section(flags);
	i2sinfo("i2s_rx_start\n");
	/* Start transfer */
	ret = i2s_rx_start(priv);

	i2s_exclsem_give(priv);
	i2sinfo("RX Exclusive Exit\n");

	return OK;

#else
	i2serr("ERROR: I2S has no receiver\n");
	UNUSED(priv);
	return -ENOSYS;
#endif
}

/*!
 * @brief Tx interrupt handler.
 *
 * @param base pointer.
 * @param handle Pointer to the sai_handle_t structure.
 */
void i2s_transfer_rx_handleirq(void *data, char *pbuf)
{
	struct amebasmart_i2s_s *priv = (struct amebasmart_i2s_s *)data;
	i2s_t *obj = priv->i2s_object;

	/* submit a new page for receive */
	i2s_recv_page(obj);

	i2s_rxdma_callback(priv, OK);
}

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
/****************************************************************************
 * Name: i2s_bufsem_rx_take
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
static void i2s_bufsem_rx_take(struct amebasmart_i2s_s *priv)
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

/****************************************************************************
 * Name: i2s_buf_rx_allocate
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
static struct amebasmart_buffer_s *i2s_buf_rx_allocate(struct amebasmart_i2s_s *priv)
{
	struct amebasmart_buffer_s *bfcontainer;
	irqstate_t flags;

	/* Set aside a buffer container.  By doing this, we guarantee that we will
	 * have at least one free buffer container.
	 */
	i2s_bufsem_rx_take(priv);

	/* Get the buffer from the head of the free list */
	flags = enter_critical_section();
	bfcontainer = priv->freelist_rx;
	ASSERT(bfcontainer);

	/* Unlink the buffer from the freelist */
	priv->freelist_rx = bfcontainer->flink;
	leave_critical_section(flags);

	return bfcontainer;
}

/****************************************************************************
 * Name: i2s_buf_rx_free
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
static void i2s_buf_rx_free(struct amebasmart_i2s_s *priv, struct amebasmart_buffer_s *bfcontainer)
{
	irqstate_t flags;

	/* Put the buffer container back on the free list */
	flags = enter_critical_section();
	bfcontainer->flink = priv->freelist_rx;
	priv->freelist_rx = bfcontainer;
	leave_critical_section(flags);

	/* Wake up any threads waiting for a buffer container */
	i2s_bufsem_rx_give(priv);
}

/****************************************************************************
 * Name: i2s_buf_rx_initialize
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
static void i2s_buf_rx_initialize(struct amebasmart_i2s_s *priv)
{
	int i;

	priv->freelist_rx = NULL;
	sem_init(&priv->bufsem_rx, 0, 1);

	for (i = 0; i < I2S_DMA_PAGE_NUM; i++) {
		i2s_buf_rx_free(priv, &priv->containers_rx[i]);
	}
}
#endif

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
/****************************************************************************
 * Name: i2s_bufsem_tx_take
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
static void i2s_bufsem_tx_take(struct amebasmart_i2s_s *priv)
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
 * Name: i2s_buf_tx_allocate
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
static struct amebasmart_buffer_s *i2s_buf_tx_allocate(struct amebasmart_i2s_s *priv)
{
	struct amebasmart_buffer_s *bfcontainer;
	irqstate_t flags;

	/* Set aside a buffer container.  By doing this, we guarantee that we will
	 * have at least one free buffer container.
	 */
	i2s_bufsem_tx_take(priv);

	/* Get the buffer from the head of the free list */
	flags = enter_critical_section();
	bfcontainer = priv->freelist_tx;
	ASSERT(bfcontainer);

	/* Unlink the buffer from the freelist */
	priv->freelist_tx = bfcontainer->flink;
	leave_critical_section(flags);

	return bfcontainer;
}

/****************************************************************************
 * Name: i2s_buf_tx_free
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
static void i2s_buf_tx_free(struct amebasmart_i2s_s *priv, struct amebasmart_buffer_s *bfcontainer)
{
	irqstate_t flags;

	/* Put the buffer container back on the free list */
	flags = enter_critical_section();
	bfcontainer->flink = priv->freelist_tx;
	priv->freelist_tx = bfcontainer;
	leave_critical_section(flags);

	/* Wake up any threads waiting for a buffer container */
	i2s_bufsem_tx_give(priv);
}

/****************************************************************************
 * Name: i2s_buf_tx_initialize
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
static void i2s_buf_tx_initialize(struct amebasmart_i2s_s *priv)
{
	int i;

	priv->freelist_tx = NULL;
	sem_init(&priv->bufsem_tx, 0, 1);

	for (i = 0; i < I2S_DMA_PAGE_NUM; i++) {
		i2s_buf_tx_free(priv, &priv->containers_tx[i]);
	}
}
#endif

/*! @} */

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
static void i2s_exclsem_take(struct amebasmart_i2s_s *priv)
{
	int ret;

	/* Wait until we successfully get the semaphore.  EINTR is the only
	 * expected 'failure' (meaning that the wait for the semaphore was
	 * interrupted by a signal.
	 */
	do {
		ret = sem_wait(&priv->exclsem);
		DEBUGASSERT(errno != EINTR);
	} while (ret < 0);
}

/****************************************************************************
 * Name: i2s_pause
 *
 * Description:
 *   Pause data transfer on I2S.
 *
 * Input Parameters:
 *   dev      - Device-specific state data
 *   dir      - the data flow direction of I2S
 *
 * Returned Value:
 *   OK on success;
 *
 ****************************************************************************/
static int i2s_pause(struct i2s_dev_s *dev, i2s_ch_dir_t dir)
{
	struct amebasmart_i2s_s *priv = (struct amebasmart_i2s_s *)dev;
	DEBUGASSERT(priv);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (dir == I2S_TX && priv->txenab) {
		ameba_i2s_pause(priv->i2s_object);
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX && priv->rxenab) {
		ameba_i2s_pause(priv->i2s_object);
	}
#endif

	return OK;
}

/****************************************************************************
 * Name: i2s_resume
 *
 * Description:
 *   Resume data transfer on I2S if it is paused.
 *
 * Input Parameters:
 *   dev      - Device-specific state data
 *   dir      - the data flow direction of I2S
 *
 * Returned Value:
 *   OK on success;
 *
 ****************************************************************************/
static int i2s_resume(struct i2s_dev_s *dev, i2s_ch_dir_t dir)
{
	struct amebasmart_i2s_s *priv = (struct amebasmart_i2s_s *)dev;
	DEBUGASSERT(priv);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (dir == I2S_TX && priv->txenab) {
		ameba_i2s_resume(priv->i2s_object);
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX && priv->rxenab) {
		ameba_i2s_resume(priv->i2s_object);
	}
#endif

	return OK;
}

/****************************************************************************
 * Name: i2s_stop
 *
 * Description:
 *   stop i2s transferring
 *
 * Input Parameters:
 *   dev     - device handler
 *   dir     - transferring direction
 *
 * Returned Value:
 *  OK
 *
 ****************************************************************************/
static int i2s_stop_transfer(struct i2s_dev_s *dev, i2s_ch_dir_t dir)
{
	struct amebasmart_i2s_s *priv = (struct amebasmart_i2s_s *)dev;
	DEBUGASSERT(priv);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (dir == I2S_TX) {
		ameba_i2s_pause(priv->i2s_object);
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX) {
		ameba_i2s_pause(priv->i2s_object);
	}
#endif

	return OK;
}

static int i2s_stop(struct i2s_dev_s *dev, i2s_ch_dir_t dir)
{
	struct amebasmart_i2s_s *priv = (struct amebasmart_i2s_s *)dev;
	irqstate_t flags;
	struct amebasmart_buffer_s *bfcontainer;
	DEBUGASSERT(priv);

	i2s_exclsem_take(priv);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (dir == I2S_TX) {
		i2s_disable(priv->i2s_object, 0);
		while (sq_peek(&priv->tx.pend) != NULL) {
			flags = enter_critical_section();
			bfcontainer = (struct amebasmart_buffer_s *)sq_remfirst(&priv->tx.pend);
			leave_critical_section(flags);
			apb_free(bfcontainer->apb);
			i2s_buf_tx_free(priv, bfcontainer);
		}

		while (sq_peek(&priv->tx.act) != NULL) {
			flags = enter_critical_section();
			bfcontainer = (struct amebasmart_buffer_s *)sq_remfirst(&priv->tx.act);
			leave_critical_section(flags);
			apb_free(bfcontainer->apb);
			i2s_buf_tx_free(priv, bfcontainer);
		}

		while (sq_peek(&priv->tx.done) != NULL) {
			flags = enter_critical_section();
			bfcontainer = (struct amebasmart_buffer_s *)sq_remfirst(&priv->tx.done);
			leave_critical_section(flags);
			i2s_buf_tx_free(priv, bfcontainer);
		}
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX) {
		i2s_disable(priv->i2s_object, 0);
		while (sq_peek(&priv->rx.pend) != NULL) {
			flags = enter_critical_section();
			bfcontainer = (struct amebasmart_buffer_s *)sq_remfirst(&priv->rx.pend);
			leave_critical_section(flags);
			apb_free(bfcontainer->apb);
			i2s_buf_rx_free(priv, bfcontainer);
		}

		while (sq_peek(&priv->rx.act) != NULL) {
			flags = enter_critical_section();
			bfcontainer = (struct amebasmart_buffer_s *)sq_remfirst(&priv->rx.act);
			leave_critical_section(flags);
			apb_free(bfcontainer->apb);
			i2s_buf_rx_free(priv, bfcontainer);
		}

		while (sq_peek(&priv->rx.done) != NULL) {
			flags = enter_critical_section();
			bfcontainer = (struct amebasmart_buffer_s *)sq_remfirst(&priv->rx.done);
			leave_critical_section(flags);
			i2s_buf_rx_free(priv, bfcontainer);
		}
	}
#endif

	i2s_exclsem_give(priv);

	return OK;
}

/****************************************************************************
 * Name: i2s_err_cb_register
 *
 * Description:
 *   Register the error callback
 *
 * Input Parameters:
 *   dev      - Device-specific state data
 *   cb       - the function pointer to callback
 *   arg      - argument to be transferred to the callback function
 *
 * Returned Value:
 *  OK
 *
 ****************************************************************************/
static int i2s_err_cb_register(struct i2s_dev_s *dev, i2s_err_cb_t cb, void *arg)
{
	struct amebasmart_i2s_s *priv = (struct amebasmart_i2s_s *)dev;
	DEBUGASSERT(priv);

	priv->err_cb = cb;
	priv->err_cb_arg = arg;

	return OK;
}

/****************************************************************************
 * Name: amebasmart_i2s_isr_initialize
 *
 * Description:
 *   allocate isr for i2s modules
 *
 * Input Parameters:
 *   priv - Partially initialized I2S device structure.  These functions
 *          will complete the I2S specific portions of the initialization
 *
 * Returned Value:
 *   configure status
 *
 *
 ****************************************************************************/
int amebasmart_i2s_isr_initialize(struct amebasmart_i2s_s *priv)
{
	/* Attach the GPIO peripheral to the allocated CPU interrupt */
#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	i2s_tx_irq_handler(priv->i2s_object, (i2s_irq_handler)priv->tx_isr_handler, (uint32_t)priv);
#endif
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	i2s_rx_irq_handler(priv->i2s_object, (i2s_irq_handler)priv->rx_isr_handler, (uint32_t)priv);
#endif
	return 0;
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
	struct amebasmart_i2s_s *priv = (struct amebasmart_i2s_s *)dev;
	DEBUGASSERT(priv && rate > 0);

	priv->i2s_object->sampling_rate = rate;
	priv->sample_rate = rate;

	i2s_set_param(priv->i2s_object, priv->i2s_object->channel_num, priv->i2s_object->sampling_rate, priv->i2s_object->word_length);

	return priv->i2s_object->sampling_rate;
}

/****************************************************************************
 * Name: i2s_getdefaultconfig
 *
 * Description:
 *   Get default configuration for I2S bus.
 *
 * Input Parameters:
 *   priv  - Device-specific private data
 *
 ****************************************************************************/
static void i2s_getdefaultconfig(struct amebasmart_i2s_s *priv)
{
	priv->i2s_object->i2s_idx = priv->config->i2s_idx;
	priv->i2s_object->sampling_rate = i2s_default_config.sample_rate;
	priv->sample_rate = priv->i2s_object->sampling_rate;
	/* priv->i2s_object->clock = ; */
	priv->i2s_object->channel_num = i2s_default_config.channel_num;
	priv->channel_num = priv->i2s_object->channel_num;

	priv->bits_per_sample = i2s_default_config.bits_per_sample;
	if (priv->bits_per_sample == I2S_BITS_PER_SAMPLE_16BIT)
		priv->i2s_object->word_length = WL_16b;
	else if (priv->bits_per_sample == I2S_BITS_PER_SAMPLE_24BIT)
		priv->i2s_object->word_length = WL_24b;
	else if (priv->bits_per_sample == I2S_BITS_PER_SAMPLE_32BIT)
		priv->i2s_object->word_length = WL_32b;
	else
		priv->i2s_object->word_length = WL_32b;

	priv->rxenab = priv->config->rxenab;
	if (priv->config->rxenab) {
		priv->i2s_object->channel_length = SP_RXCL_32;
		priv->i2s_object->fifo_num = SP_RX_FIFO2;
		priv->i2s_object->direction = SP_DIR_RX;
		priv->i2s_object->role = MASTER;
	}
	priv->txenab = priv->config->txenab;
	if (priv->config->txenab) {
		priv->i2s_object->channel_length = SP_TXCL_32;
		priv->i2s_object->fifo_num = SP_TX_FIFO2;
		priv->i2s_object->direction = SP_DIR_TX;
		priv->i2s_object->role = MASTER;
	}

	priv->i2s_object->mode = MULTIIO;
}

/****************************************************************************
 * Name: i2s_allocate_wd
 *
 * Description:
 *   Get default configuration for I2S bus.
 *
 * Input Parameters:
 *   priv  - Device-specific private data
 *
 ****************************************************************************/
static int i2s_allocate_wd(struct amebasmart_i2s_s *priv)
{
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (priv->rxenab) {
		priv->rx.dog = wd_create();
		if (!priv->rx.dog) {
			i2serr("ERROR: Failed to create the RX DMA watchdog\n");
			goto errout;
		}
	}
#endif
#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (priv->txenab) {
		priv->tx.dog = wd_create();
		if (!priv->tx.dog) {
			i2serr("ERROR: Failed to create the TX DMA watchdog\n");
			goto errout;
		}
	}
#endif
	return OK;

errout:
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (priv->rx.dog) {
		wd_delete(priv->rx.dog);
		priv->rx.dog = NULL;
	}
#endif
#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (priv->tx.dog) {
		wd_delete(priv->tx.dog);
		priv->tx.dog = NULL;
	}
#endif
	return -ENOMEM;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: amebasmart_i2s_initialize
 *
 * Description:
 *   Initialize the selected i2s port
 *
 * Input Parameter:
 *   port      - Decide which I2S port to be initialized
 *   is_reinit - Decide whether the system wakeup from sleep and require reinit
 *
 * Returned Value:
 *   Valid i2s device structure reference on succcess; a NULL on failure
 *
 ****************************************************************************/
struct i2s_dev_s *amebasmart_i2s_initialize(uint16_t port, bool is_reinit)
{
	struct amebasmart_i2s_config_s *hw_config_s = NULL;
	struct amebasmart_i2s_s *priv;
	int ret;

	/* Assign HW configuration */
	if (port == I2S_NUM_2) {
		hw_config_s = (struct amebasmart_i2s_config_s *)&amebasmart_i2s2_config;
	} else if (port == I2S_NUM_3) {
		hw_config_s = (struct amebasmart_i2s_config_s *)&amebasmart_i2s3_config;
	} else {
		i2serr("Please select I2S2 or I2S3 bus\n");
		return NULL;
	}

	if (!is_reinit) {
		if (g_i2sdevice[port] != NULL) {
			return &g_i2sdevice[port]->dev;
		}

		/* Allocate a new state structure for this chip select.  NOTE that there
		* is no protection if the same chip select is used in two different
		* chip select structures.
		*/
		priv = (struct amebasmart_i2s_s *)kmm_zalloc(sizeof(struct amebasmart_i2s_s));
		if (!priv) {
			i2serr("ERROR: Failed to allocate a chip select structure\n");
			return NULL;
		}
	} else {
		/* The I2S structure should exist after system wakeup */
		priv = g_i2sdevice[port];
		DEBUGASSERT(priv);
	}

	priv->i2s_object = (i2s_t *)kmm_zalloc(sizeof(i2s_t));
	DEBUGASSERT(priv->i2s_object);
	/* Config values initialization */
	priv->config = hw_config_s;	/* Get HW configuation */

	/* Get default configuration */
	i2s_getdefaultconfig(priv);

	/* Initialize buffering */
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	i2s_buf_rx_initialize(priv);
#endif

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	i2s_buf_tx_initialize(priv);
#endif

	/* I2s object initialization */
	i2s_init(priv->i2s_object, hw_config_s->i2s_sclk_pin, hw_config_s->i2s_ws_pin, hw_config_s->i2s_sd_tx_pin, hw_config_s->i2s_sd_rx_pin, hw_config_s->i2s_mclk_pin);

	/* Initialize buffering */
#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	i2s_set_dma_buffer(priv->i2s_object, (char *)priv->i2s_tx_buf, NULL, I2S_DMA_PAGE_NUM, I2S_DMA_PAGE_SIZE); /* Allocate DMA Buffer for TX */
#endif

	/* configures IRQ */
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	priv->rx_isr_handler = (i2s_irq_handler)&i2s_transfer_rx_handleirq;
#endif

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	priv->tx_isr_handler = (i2s_irq_handler)&i2s_transfer_tx_handleirq;
#endif

	ret = amebasmart_i2s_isr_initialize(priv);
	if (ret != OK) {
		i2serr("I2S initialize: isr fails\n");
		goto errout_with_alloc;
	}
	/* Initialize the I2S priv device structure  */
	sem_init(&priv->exclsem, 0, 1);
	priv->dev.ops = &g_i2sops;

	ret = i2s_allocate_wd(priv);
	if (ret != OK) {
		goto errout_with_alloc;
	}
	/* Basic settings */
	//priv->i2s_num = priv->i2s_object->i2s_idx;
	if (!is_reinit) {
		g_i2sdevice[port] = priv;
	}

	/* Success exit */
	return &priv->dev;

	/* Failure exits */
errout_with_alloc:
	sem_destroy(&priv->exclsem);
	kmm_free(priv);
	return NULL;
}
#endif /* I2S_HAVE_RX || I2S_HAVE_TX */
#endif /* CONFIG_AUDIO */

#ifdef CONFIG_PM
static void amebasmart_i2s_suspend(uint16_t port)
{
	struct amebasmart_i2s_s *priv = g_i2sdevice[port];

	i2s_disable(priv->i2s_object, 1);
	kmm_free(priv->i2s_object);
	priv->i2s_object = NULL;
	sem_destroy(&priv->exclsem);
	sem_destroy(&priv->bufsem_tx);
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (priv->rx.dog) {
		wd_delete(priv->rx.dog);
		priv->rx.dog = NULL;
	}
#endif
#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (priv->tx.dog) {
		wd_delete(priv->tx.dog);
		priv->tx.dog = NULL;
	}
#endif
}

static uint32_t rtk_i2s_suspend(uint32_t expected_idle_time, void *param)
{
	(void)expected_idle_time;
	(void)param;
	/* Nothing to do here */
#ifdef CONFIG_AMEBASMART_I2S2
	amebasmart_i2s_suspend(I2S_NUM_2);
#endif
#ifdef CONFIG_AMEBASMART_I2S3
	amebasmart_i2s_suspend(I2S_NUM_3);
#endif

	return 1;
}

static uint32_t rtk_i2s_resume(uint32_t expected_idle_time, void *param)
{
	(void)expected_idle_time;
	(void)param;

	/* For PG Sleep, I2S HW will be lost power, thus a reinitialization is required here */
#ifdef CONFIG_AMEBASMART_I2S2
	(void)amebasmart_i2s_initialize(I2S_NUM_2, I2S_REINIT);
#endif
#ifdef CONFIG_AMEBASMART_I2S3
	(void)amebasmart_i2s_initialize(I2S_NUM_3, I2S_REINIT);
#endif

	return 1;
}
#endif

#ifdef CONFIG_PM
void i2s_pminitialize(void)
{
	bsp_pm_domain_register("I2S", BSP_I2S_DRV);
	pmu_register_sleep_callback(PMU_I2S_DEVICE, (PSM_HOOK_FUN)rtk_i2s_suspend, NULL, (PSM_HOOK_FUN)rtk_i2s_resume, NULL);
}
#endif
