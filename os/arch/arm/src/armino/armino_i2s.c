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

#include "irq/irq.h"

#include <arch/board/board.h>
#include "armino_i2s.h"

#include <driver/i2s.h>
#include <driver/i2s_types.h>


/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
//#ifndef CONFIG_SCHED_WORKQUEUE
//#error Work queue support is required (CONFIG_SCHED_WORKQUEUE)
//#endif

#ifdef CONFIG_AUDIO
//#ifndef CONFIG_AUDIO
//#error CONFIG_AUDIO required by this driver
//#endif

#ifndef CONFIG_ARMINO_I2S_MAXINFLIGHT
#define CONFIG_ARMINO_I2S_MAXINFLIGHT 10
#endif

/* Assume no RX/TX support until we learn better */
#undef I2S_HAVE_RX
#undef I2S_HAVE_TX

#if defined(CONFIG_ARMINO_I2S)

/* Check for I2S RX support */
#if defined(CONFIG_ARMINO_I2S_RX)
#define I2S_HAVE_RX     1
#endif

/* Check for I2S TX support */
#if defined(CONFIG_ARMINO_I2S_TX)
#define I2S_HAVE_TX     1
#endif

#ifndef CONFIG_ARMINO_I2S_DATALEN
#define CONFIG_ARMINO_I2S_DATALEN        (I2S_BITS_PER_SAMPLE_16BIT)
#endif

#ifndef CONFIG_ARMINO_I2S_SAMPLERATE
#define CONFIG_ARMINO_I2S_SAMPLERATE     (I2S_SAMP_RATE_44100)
#endif

#endif

#ifdef CONFIG_DEBUG
#define CONFIG_DEBUG_FEATURES
#endif

#define DEBUG_I2S_DRIVER    0

#undef i2serr
#undef i2sinfo
#if defined(DEBUG_I2S_DRIVER) && (0 < DEBUG_I2S_DRIVER)
//#define i2sinfo(format, ...)   bk_dump_printf(format, ##__VA_ARGS__)
//#define i2serr(format, ...)   bk_dump_printf(format, ##__VA_ARGS__)
#define i2sinfo(format, ...)   printf(format, ##__VA_ARGS__)
#define i2serr(format, ...)   printf(format, ##__VA_ARGS__)
#else
#define i2sinfo(format, ...)
#endif
#define i2serr(format, ...)   printf(format, ##__VA_ARGS__)

#if defined(I2S_HAVE_RX) || defined(I2S_HAVE_TX)

#ifndef MIN
#define MIN(a,b) a < b ? a : b
#endif

#define OVER_SAMPLE_RATE (384U)

#define I2S_DMA_FRAME_SIZE	320		/* 4 ~ 16384, set to a factor of APB size */

#define I2S_MAX_BUFFER_SIZE 3840

#if defined(CONFIG_ARMINO_I2S_ROLE_MASTER) && (0 < CONFIG_ARMINO_I2S_ROLE_MASTER)
#define I2S_WORK_ROLE I2S_ROLE_MASTER
#else
#define I2S_WORK_ROLE I2S_ROLE_SLAVE
#endif

#ifndef CONFIG_ARMINO_I2S_GPIO_GROUP_ID
#define CONFIG_ARMINO_I2S_GPIO_GROUP_ID (I2S_GPIO_GROUP_0)
#endif

struct armino_buffer_s {
	struct armino_buffer_s *flink;	/* Supports a singly linked list */
	i2s_callback_t callback;		/* Function to call when the transfer completes */
	uint32_t timeout;				/* The timeout value to use with DMA transfers */
	void *arg;						/* The argument to be returned with the callback */
	struct ap_buffer_s *apb;		/* The audio buffer */
	int result;						/* The result of the transfer */
};

/* This structure describes the state of one receiver or transmitter transport */

struct armino_transport_s {
	WDOG_ID dog;				/* Watchdog that handles DMA timeouts */
	sq_queue_t pend;			/* A queue of pending transfers */
	sq_queue_t act;				/* A queue of active transfers */
	sq_queue_t done;			/* A queue of completed transfers */
	struct work_s work;			/* Supports worker thread operations */
	int error_state;			/* Channel error state, 0 - OK*/
};

/* I2S configuration parameters for i2s_param_config function */
typedef struct {
	i2s_gpio_group_id_t gpio_group_id;
	i2s_config_t i2s_config;

	/* tx config */
	uint8_t tx_en:1;
	i2s_channel_id_t tx_chl_id;
	uint32_t tx_page_size;
	uint8_t tx_page_num;

	/* rx config */
	uint8_t rx_en:1;
	i2s_channel_id_t rx_chl_id;
	uint32_t rx_page_size;
	uint8_t rx_page_num;
} armino_i2s_config_t;

/* The state of the one I2S peripheral */

struct armino_i2s_s {
	struct i2s_dev_s dev;           /* Externally visible I2S interface, must the first element!! */

	armino_i2s_config_t config;

	i2s_err_cb_t err_cb;            /* registered error callback function */
	void *err_cb_arg;               /* argiment to return with err cb call */

	sem_t exclsem;                  /* Assures mutually exclusive acess to I2S */

	uint8_t rxenab:1;               /* True: RX transfers enabled */
	uint8_t txenab:1;               /* True: TX transfers enabled */

	struct ap_buffer_s *apb_tx;		/* Pointer to application TX audio buffer to track current TX byte count */
	RingBufferContext *ch1_tx_rb;	/* Ring buffer context for channel 1 TX */

	struct ap_buffer_s *apb_rx;		/* Pointer to application RX audio buffer to track current RX byte count */
	RingBufferContext *ch1_rx_rb;	/* Ring buffer context for channel 1 RX */

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	struct armino_transport_s rx;		/* RX transport state */
#endif

	sem_t bufsem_tx;			/* Buffer wait semaphore */
	struct armino_buffer_s *freelist_tx;	/* A list a free buffer containers */
	struct armino_buffer_s containers_tx[CONFIG_ARMINO_I2S_MAXINFLIGHT];

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	struct armino_transport_s tx;		/* TX primary transport state */
#endif

	sem_t bufsem_rx;			/* Buffer wait semaphore */
	struct armino_buffer_s *freelist_rx;	/* A list a free buffer containers */
	struct armino_buffer_s containers_rx[CONFIG_ARMINO_I2S_MAXINFLIGHT];

};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* Register helpers */
#define CONFIG_ARMINO_I2S_BUFFERS_DEBUG   0

#if defined(CONFIG_ARMINO_I2S_BUFFERS_DEBUG) && (0 < CONFIG_ARMINO_I2S_BUFFERS_DEBUG)
#define       armino_buffer_debug_printf(b)   armino_buffer_info_printf(b)
#else
#define       armino_buffer_debug_printf(b)
#endif

#define CONFIG_ARMINO_I2S_DUMPBUFFERS   0

#if defined(CONFIG_ARMINO_I2S_DUMPBUFFERS) && (0 < CONFIG_ARMINO_I2S_DUMPBUFFERS)
#define       i2s_init_buffer(b, s)   memset(b, 0x55, s);
#define       i2s_dump_buffer(m, b, s) lib_dumpbuffer(m, b, s)
#else
#define       i2s_init_buffer(b, s)
#define       i2s_dump_buffer(m, b, s)
#endif

//#define I2S_RX_DEBUG   //GPIO debug

#ifdef I2S_RX_DEBUG

#define I2S_RX_DMA_ISR_START()                 	do { GPIO_DOWN(14); GPIO_UP(14);} while (0)
#define I2S_RX_DMA_ISR_END()                    do { GPIO_DOWN(14); } while (0)

#define I2S_RX_WD_START()         				do { GPIO_DOWN(15); GPIO_UP(15);} while (0)
#define I2S_RX_WD_END()           				do { GPIO_DOWN(15); } while (0)

#define I2S_RX_DOWN_START()        				do { GPIO_DOWN(16); GPIO_UP(16);} while (0)
#define I2S_RX_DOWN_END()          				do { GPIO_DOWN(16); } while (0)

#define I2S_RX_CALLBACK_START()           		do { GPIO_DOWN(17); GPIO_UP(17);} while (0)
#define I2S_RX_CALLBACK_END()             		do { GPIO_DOWN(17); } while (0)

#define AUD_ONBOARD_MIC_OUTPUT_START()          do { GPIO_DOWN(36); GPIO_UP(36);} while (0)
#define AUD_ONBOARD_MIC_OUTPUT_END()            do { GPIO_DOWN(36); } while (0)

#else

#define I2S_RX_DMA_ISR_START()
#define I2S_RX_DMA_ISR_END()

#define I2S_RX_WD_START()
#define I2S_RX_WD_END()

#define I2S_RX_DOWN_START()
#define I2S_RX_DOWN_END()

#define I2S_RX_CALLBACK_START()
#define I2S_RX_CALLBACK_END()

#define AUD_ONBOARD_MIC_OUTPUT_START()
#define AUD_ONBOARD_MIC_OUTPUT_END()
#endif


/* Semaphore helpers */
static void i2s_exclsem_take(struct armino_i2s_s *priv);
#define i2s_exclsem_give(priv) sem_post(&priv->exclsem)

/* Buffer container helpers */
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
static void i2s_rxdma_callback(void);
static void i2s_rxdma_timeout(int argc, uint32_t arg);
static int i2s_rx_start(struct armino_i2s_s *priv);
static void i2s_rx_worker(void *arg);
static void i2s_rx_schedule(struct armino_i2s_s *priv, int result);

static void i2s_bufsem_rx_take(struct armino_i2s_s *priv);
#define i2s_bufsem_rx_give(priv) sem_post(&priv->bufsem_rx)

static struct armino_buffer_s *i2s_buf_rx_allocate(struct armino_i2s_s *priv);
static void i2s_buf_rx_free(struct armino_i2s_s *priv, struct armino_buffer_s *bfcontainer);
static void i2s_buf_rx_initialize(struct armino_i2s_s *priv);

#endif

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
static void i2s_txdma_callback(void);
static void i2s_txdma_timeout(int argc, uint32_t arg);
static int i2s_tx_start(struct armino_i2s_s *priv);
static void i2s_tx_worker(void *arg);
static void i2s_tx_schedule(struct armino_i2s_s *priv, int result);

static void i2s_bufsem_tx_take(struct armino_i2s_s *priv);
#define i2s_bufsem_tx_give(priv) sem_post(&priv->bufsem_tx)

static struct armino_buffer_s *i2s_buf_tx_allocate(struct armino_i2s_s *priv);
static void i2s_buf_tx_free(struct armino_i2s_s *priv, struct armino_buffer_s *bfcontainer);
static void i2s_buf_tx_initialize(struct armino_i2s_s *priv);

#endif

/* I2S methods (and close friends) */

static uint32_t i2s_rxdatawidth(struct i2s_dev_s *dev, int bits);

static uint32_t i2s_samplerate(struct i2s_dev_s *dev, uint32_t rate);

static uint32_t i2s_txdatawidth(struct i2s_dev_s *dev, int bits);

static int i2s_stop(struct i2s_dev_s *dev, i2s_ch_dir_t dir);
static int i2s_pause(struct i2s_dev_s *dev, i2s_ch_dir_t dir);
static int i2s_resume(struct i2s_dev_s *dev, i2s_ch_dir_t dir);

static int i2s_receive(struct i2s_dev_s *dev, struct ap_buffer_s *apb, i2s_callback_t callback, void *arg, uint32_t timeout);
static int i2s_send(struct i2s_dev_s *dev, struct ap_buffer_s *apb, i2s_callback_t callback, void *arg, uint32_t timeout);

static int i2s_err_cb_register(struct i2s_dev_s *dev, i2s_err_cb_t cb, void *arg);

/****************************************************************************
* External functions
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

static struct armino_i2s_s *g_i2sdevice[I2S_NUM_MAX] = {NULL};
static bool i2s_tx_is_start = false;
static uint32_t tx_free_page_size = I2S_DMA_FRAME_SIZE * 2;
static uint32_t rx_filled_page_size = 0;

static void armino_buffer_info_printf(struct armino_buffer_s *armino_buffer)
{
	bk_dump_printf("armino_buffer_s: %p, callback: %p, timeout: %d\n", armino_buffer, armino_buffer->callback, armino_buffer->timeout);
	bk_dump_printf("apb: %p, nbytes: %d, curbyte: %d, maxbytes: %d\n", armino_buffer->apb, armino_buffer->apb->nbytes, armino_buffer->apb->curbyte, armino_buffer->apb->nmaxbytes);
}


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
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void i2s_txdma_callback(void)
{
	DEBUGASSERT(g_i2sdevice[0] != NULL);

	/* Cancel the watchdog timeout */

	(void)wd_cancel(g_i2sdevice[0]->tx.dog);


	/* Then schedule completion of the transfer to occur on the worker thread */

	i2s_tx_schedule(g_i2sdevice[0], 0);
}

static void i2s_txdma_timeout(int argc, uint32_t arg)
{
	struct armino_i2s_s *priv = (struct armino_i2s_s *)arg;
	DEBUGASSERT(priv != NULL);

	/* Then schedule completion of the transfer to occur on the worker thread.
	 * Set the result with -ETIMEDOUT.
	 */
	i2s_tx_schedule(priv, -ETIMEDOUT);
}

static int armino_i2s_tx(struct armino_i2s_s *priv, struct armino_buffer_s *bfcontainer)
{
	int tx_size;

	i2sinfo("[%s][%d] Enter\n", __func__, __LINE__);

	tx_size = tx_free_page_size; /* Track current byte size to increment by */
	struct ap_buffer_s *apb;
	if (NULL != bfcontainer && NULL != bfcontainer->apb) {

		apb = bfcontainer->apb;
		/* Start sending first page, after that the txdma callback will be called in the tx irq handler */
		if ((apb->nbytes - apb->curbyte) <= tx_size) {
			tx_size = apb->nbytes - apb->curbyte;
			ring_buffer_write(priv->ch1_tx_rb, (uint8_t*)&apb->samp[apb->curbyte], tx_size);
		}
		else {
			ring_buffer_write(priv->ch1_tx_rb, (uint8_t*)&apb->samp[apb->curbyte], tx_size);

			/* Add the container to the list of active DMAs */
			sq_addlast((sq_entry_t *)bfcontainer, &priv->tx.act);
			priv->apb_tx = apb; /* Reference current TX apb in our I2S struct */
		}
		apb->curbyte += tx_size; /* No padding, ptx_buf is big enough to fill the whole tx_size */
		tx_free_page_size -= tx_size;
		i2sinfo("%s tx_free_page_size=%d\n", __func__, tx_free_page_size);
	}

	/* Check tx_free_page_size and start i2s when i2s not start */
	if (!i2s_tx_is_start && tx_free_page_size == 0)
	{
		bk_i2s_start();
		i2sinfo("%s bk_i2s_start start!\n", __func__);
		i2s_tx_is_start = true;
	}

	i2sinfo("[%s][%d] Exit\n", __func__, __LINE__);

	return 0;
}

static int i2s_tx_start(struct armino_i2s_s *priv)
{
	struct armino_buffer_s *bfcontainer = NULL;
	int ret;
	irqstate_t flags;
	bool tx_work_flag = false;
	struct ap_buffer_s *apb;

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

	i2sinfo("[%s][%d] -->>>>>>>Enter\n", __func__, __LINE__);

	/* Start sending data until tx_free_page_size is 0 */
	while (tx_free_page_size)
	{
		i2sinfo("%s tx_free_page_size=%d\n", __func__, tx_free_page_size);
		flags = irqsave();

		/* Remove the pending TX transfer at the head of the TX pending queue. */
		bfcontainer = (struct armino_buffer_s *)sq_remfirst(&priv->tx.pend);

		/* printf armino_buffer info */
		armino_buffer_debug_printf(bfcontainer);

		/* Start first transfer */
		armino_i2s_tx(priv, bfcontainer);
		irqrestore(flags);

		apb = bfcontainer->apb;
		/* Check if all bfcontainer data has been sent */
		if (apb->curbyte >= apb->nbytes) {
			/* Report the result of the transfer */
			bfcontainer->result = 0;
			/* Add the completed buffer container to the tail of the tx.done queue */
			i2sinfo("add bfcontainer=%p to tx.done\n", bfcontainer);
			armino_buffer_debug_printf(bfcontainer);
			flags = irqsave();
			sq_addlast((sq_entry_t *) bfcontainer, &priv->tx.done);
			irqrestore(flags);
			i2sinfo("[%s][%d]tx.act.head=%p tx.pend.head=%p tx.done.head=%p\n", __func__, __LINE__, priv->tx.act.head, priv->tx.pend.head, priv->tx.done.head);
			tx_work_flag = true;
		}

		if (tx_free_page_size) {
			if (sq_empty(&priv->tx.pend)) {
				i2sinfo("[TX] empty!\n");
				break;
			}
		}
	}

	/* Start a watchdog to catch DMA timeouts */
	if (bfcontainer->timeout > 0 && i2s_tx_is_start && tx_free_page_size == 0) {
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

	if (tx_work_flag) {
		if (work_available(&priv->tx.work)) {
			ret = work_queue(HPWORK, &priv->tx.work, i2s_tx_worker, priv, 0);
			if (ret != 0) {
				i2serr("ERROR: Failed to queue TX primary work: %d\n", ret);
			}
		}
	}

	i2sinfo("[%s][%d] -->>>>>Exit\n", __func__, __LINE__);

	return OK;
}

static void i2s_tx_worker(void *arg)
{
	struct armino_i2s_s *priv = (struct armino_i2s_s *)arg;
	struct armino_buffer_s *bfcontainer;
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

	i2sinfo("[%s][%d]tx.act.head=%p tx.pend.head=%p tx.done.head=%p\n", __func__, __LINE__, priv->tx.act.head, priv->tx.pend.head, priv->tx.done.head);


	/* Process each buffer in the tx.done queue */
	while (sq_peek(&priv->tx.done) != NULL) {
		/* Remove the buffer container from the tx.done queue.  NOTE that
		 * interrupts must be enabled to do this because the tx.done queue is
		 * also modified from the interrupt level.
		 */

		flags = irqsave();
		bfcontainer = (struct armino_buffer_s *)sq_remfirst(&priv->tx.done);
		irqrestore(flags);
		/* Perform the TX transfer done callback */

		DEBUGASSERT(bfcontainer && bfcontainer->callback);
		i2sinfo("bfcontainer=%p tx.done.callback\n", bfcontainer);
		armino_buffer_debug_printf(bfcontainer);
		bfcontainer->callback(&priv->dev, bfcontainer->apb, bfcontainer->arg, bfcontainer->result);

		/* Release our reference on the audio buffer.  This may very likely
		 * cause the audio buffer to be freed.
		 */

		apb_free(bfcontainer->apb);

		/* And release the buffer container */

		i2s_buf_tx_free(priv, bfcontainer);
	}

}

static void i2s_tx_schedule(struct armino_i2s_s *priv, int result)
{
	struct armino_buffer_s *bfcontainer;
	int ret;

	/* Upon entry, the transfer(s) that just completed are the ones in the
	 * priv->tx.act queue.  NOTE: In certain conditions, this function may
	 * be called an additional time, hence, we can't assert this to be true.
	 * For example, in the case of a timeout, this function will be called by
	 * both indirectly via the armino_dmastop() logic and directly via the
	 * i2s_txdma_timeout() logic.
	 */

	/* Move all entries from the tx.act queue to the tx.done queue */

	if (!sq_empty(&priv->tx.act)) {
		/* Remove the next buffer container from the tx.act list */

		bfcontainer = (struct armino_buffer_s *)sq_remfirst(&priv->tx.act);

		/* Report the result of the transfer */

		bfcontainer->result = result;

		/* Add the completed buffer container to the tail of the tx.done queue */
		i2sinfo("[%s][%d]tx.act.head=%p tx.pend.head=%p tx.done.head=%p\n", __func__, __LINE__, priv->tx.act.head, priv->tx.pend.head, priv->tx.done.head);
		sq_addlast((sq_entry_t *) bfcontainer, &priv->tx.done);
	}


	if (!sq_empty(&priv->tx.pend)) {
		/* Remove the next buffer container from the tx.pend list */
		bfcontainer = (struct armino_buffer_s *)sq_remfirst(&priv->tx.pend);

		/* Start next transfer */
		armino_i2s_tx(priv, bfcontainer);
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
	struct armino_i2s_s *priv = (struct armino_i2s_s *)dev;

	/* Support 16, 24, 32 bits */
	DEBUGASSERT(priv && (bits == I2S_BITS_PER_SAMPLE_16BIT \
				|| bits == I2S_BITS_PER_SAMPLE_32BIT \
				|| bits == I2S_BITS_PER_SAMPLE_24BIT));

	priv->config.i2s_config.data_length = bits;

	/* armino 16, 24, 32, bits setting */
	//if (bits == I2S_BITS_PER_SAMPLE_16BIT) priv->i2s_object.word_length = WL_16b;
	//else if (bits == I2S_BITS_PER_SAMPLE_24BIT) priv->i2s_object.word_length = WL_24b;
	//else if (bits == I2S_BITS_PER_SAMPLE_32BIT) priv->i2s_object.word_length = WL_32b;

	return priv->config.i2s_config.data_length * priv->config.i2s_config.samp_rate;
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
	struct armino_i2s_s *priv = (struct armino_i2s_s *)dev;
	DEBUGASSERT(priv && apb);

	// i2sinfo("[I2S TX] apb=%p nbytes=%d samp=%p arg=%p timeout=%d\n", apb, apb->nbytes - apb->curbyte, apb->samp, arg, timeout);
	i2s_dump_buffer("Sending", &apb->samp[apb->curbyte], apb->nbytes - apb->curbyte);
#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	struct armino_buffer_s *bfcontainer;
	irqstate_t flags;
	int ret;

	/* Has the TX channel been enabled? */
	if (!priv->txenab) {
		i2sinfo("I2S has no transmitter\n");
		return -EAGAIN;
	}

	/* Allocate a buffer container in advance */

	bfcontainer = i2s_buf_tx_allocate(priv);

	i2s_exclsem_take(priv);
	i2sinfo("[%s][%d] ----- TX Exclusive Enter -----\n", __func__, __LINE__);

	apb_reference(apb);

	/* Initialize the buffer container structure */

	bfcontainer->callback = (void *)callback;
	bfcontainer->timeout = timeout;
	bfcontainer->arg = arg;
	bfcontainer->apb = apb;
	bfcontainer->result = -EBUSY;

	armino_buffer_debug_printf(bfcontainer);

	//i2sinfo("[%s][%d]tx.act.head=%p tx.pend.head=%p tx.done.head=%p\n", __func__, __LINE__, priv->tx.act.head, priv->tx.pend.head, priv->tx.done.head);
	flags = irqsave();
	sq_addlast((sq_entry_t *) bfcontainer, &priv->tx.pend);
	irqrestore(flags);

	//i2sinfo("[%s][%d]tx.act.head=%p tx.pend.head=%p tx.done.head=%p\n", __func__, __LINE__, priv->tx.act.head, priv->tx.pend.head, priv->tx.done.head);

	ret = i2s_tx_start(priv);

	i2s_exclsem_give(priv);
	i2sinfo("[%s][%d] ----- TX Exclusive Exit -----\n", __func__, __LINE__);

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

int i2s_transfer_tx_handleirq(uint32_t size)
{
	/* This handler is called after every completion of i2s_send_page() */
	struct armino_i2s_s *priv = g_i2sdevice[0];
	int tx_size;

	/* update free page size */
	tx_free_page_size += I2S_DMA_FRAME_SIZE;

	i2sinfo("[%s][%d] tx_free_page_size=%d\n", __func__, __LINE__, tx_free_page_size);

	tx_size = tx_free_page_size; /* Track current byte size to increment by */
	if ((priv->apb_tx->nbytes - priv->apb_tx->curbyte) <= tx_free_page_size) { /* Condition to stop sending if all data in the buffer has been sent */
		tx_size = priv->apb_tx->nbytes - priv->apb_tx->curbyte;
		ring_buffer_write(priv->ch1_tx_rb, (uint8_t*)&priv->apb_tx->samp[priv->apb_tx->curbyte], tx_size);
		tx_free_page_size -= tx_size;
		priv->apb_tx->curbyte += tx_size;
		i2s_txdma_callback();
	}
	else {
		ring_buffer_write(priv->ch1_tx_rb, (uint8_t*)&priv->apb_tx->samp[priv->apb_tx->curbyte], tx_size);
		priv->apb_tx->curbyte += tx_size; /* No padding, ptx_buf is big enough to fill the whole tx_size */
		//i2s_send_page(&priv->i2s_object, (uint32_t*)ptx_buf);
		tx_free_page_size -= tx_size;
	}

	return size;
}

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)

/****************************************************************************
 * Name: i2s_rxdma_callback
 *
 * Description:
 *   This callback function is invoked at the completion of the I2S RX DMA.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void i2s_rxdma_callback(void)
{
	DEBUGASSERT(g_i2sdevice[0] != NULL);

	/* Cancel the watchdog timeout */

	(void)wd_cancel(g_i2sdevice[0]->rx.dog);
	I2S_RX_WD_END();

	/* Then schedule completion of the transfer to occur on the worker thread */

	i2s_rx_schedule(g_i2sdevice[0], OK);
}

static void i2s_rxdma_timeout(int argc, uint32_t arg)
{
	struct armino_i2s_s *priv = (struct armino_i2s_s *)arg;
	I2S_RX_WD_END();

	DEBUGASSERT(priv != NULL);

	/* Timeout: set the result to -ETIMEDOUT. */
	i2s_rx_schedule(priv, -ETIMEDOUT);
}

static int armino_i2s_rx(struct armino_i2s_s *priv, struct armino_buffer_s *bfcontainer)
{
	int rx_size;

	i2sinfo("[%s][%d] Enter\n", __func__, __LINE__);

	rx_size = rx_filled_page_size; /* Track current byte size to increment by */
	struct ap_buffer_s *apb;
	if (NULL != bfcontainer && NULL != bfcontainer->apb) {

		apb = bfcontainer->apb;
		if ((apb->nmaxbytes - apb->nbytes) <= rx_size) {
			rx_size = apb->nmaxbytes - apb->nbytes;
			ring_buffer_read(priv->ch1_rx_rb, (uint8_t*)&apb->samp[apb->curbyte], rx_size);
		}
		else {
			if (rx_size) {
				ring_buffer_read(priv->ch1_rx_rb, (uint8_t*)&apb->samp[apb->curbyte], rx_size);
			}
			/* Add the container to the list of active DMAs */
			sq_addlast((sq_entry_t *)bfcontainer, &priv->rx.act);
			priv->apb_rx = apb; /* Reference current RX apb in our I2S struct */
		}
		apb->curbyte += rx_size; /* No padding, ptx_buf is big enough to fill the whole rx_size */
		apb->nbytes += rx_size; /* No padding, ptx_buf is big enough to fill the whole rx_size */
		rx_filled_page_size -= rx_size;
		i2sinfo("[%s][%d] rx_filled_page_size=%d\n", __func__, __LINE__, rx_filled_page_size);
	}

	i2sinfo("[%s][%d] Exit\n", __func__, __LINE__);

	return 0;
}

static int i2s_rx_start(struct armino_i2s_s *priv)
{
	struct armino_buffer_s *bfcontainer = NULL;
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

	i2sinfo("[%s][%d] -->>>>>>>Enter\n", __func__, __LINE__);

	flags = irqsave();
	/* Remove the pending RX transfer at the head of the RX pending queue. */
	bfcontainer = (struct armino_buffer_s *)sq_remfirst(&priv->rx.pend);

	/* Start first transfer */
	armino_i2s_rx(priv, bfcontainer);
	irqrestore(flags);

	/* Start a watchdog to catch DMA timeouts */
	if (bfcontainer->timeout > 0) {
		I2S_RX_WD_START();
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

	i2sinfo("[%s][%d] -->>>>>Exit\n", __func__, __LINE__);

	return OK;
}

static void i2s_rx_worker(void *arg)
{
	struct armino_i2s_s *priv = (struct armino_i2s_s *)arg;
	struct ap_buffer_s *apb;
	struct armino_buffer_s *bfcontainer;
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

	i2sinfo("[%s][%d]rx.act.head=%p rx.pend.head=%p rx.done.head=%p\n", __func__, __LINE__, priv->rx.act.head, priv->rx.pend.head, priv->rx.done.head);

	/* Process each buffer in the rx.done queue */
	while (sq_peek(&priv->rx.done) != NULL) {
		/* Remove the buffer container from the rx.done queue.  NOTE that
		 * interrupts must be disabled to do this because the rx.done queue is
		 * also modified from the interrupt level.
		 */

		flags = irqsave();
		bfcontainer = (struct armino_buffer_s *)sq_remfirst(&priv->rx.done);
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

		I2S_RX_CALLBACK_START();
		/* Perform the RX transfer done callback */
		bfcontainer->callback(&priv->dev, apb, bfcontainer->arg, bfcontainer->result);
		I2S_RX_CALLBACK_END();

		/* Release our reference on the audio buffer.  This may very likely
		 * cause the audio buffer to be freed.
		 */
		apb_free(apb);

		/* And release the buffer container */
		i2s_buf_rx_free(priv, bfcontainer);
	}

}

static void i2s_rx_schedule(struct armino_i2s_s *priv, int result)
{
	struct armino_buffer_s *bfcontainer;
	int ret;
	struct ap_buffer_s *apb;

	/* Upon entry, the transfer(s) that just completed are the ones in the
	 * priv->rx.act queue.  NOTE: In certain conditions, this function may
	 * be called an additional time, hence, we can't assert this to be true.
	 * For example, in the case of a timeout, this function will be called by
	 * both indirectly via the armino_dmastop() logic and directly via the
	 * i2s_rxdma_timeout() logic.
	 */

	/* Move first entry from the rx.act queue to the rx.done queue */

	if (!sq_empty(&priv->rx.act)) {
		/* Remove the next buffer container from the rx.act list */

		bfcontainer = (struct armino_buffer_s *)sq_remfirst(&priv->rx.act);

		/* Report the result of the transfer */

		bfcontainer->result = result;

		/* Add the completed buffer container to the tail of the rx.done queue */
		I2S_RX_DOWN_START();
		I2S_RX_DOWN_END();
		sq_addlast((sq_entry_t *) bfcontainer, &priv->rx.done);
		priv->apb_rx = NULL;
	}

	/* Start sending data until rx_free_page_size is 0 */
	do {
		if (!sq_empty(&priv->rx.pend)) {
			/* Remove the next buffer container from the tx.pend list */
			bfcontainer = (struct armino_buffer_s *)sq_remfirst(&priv->rx.pend);

			/* Start next transfer */
			armino_i2s_rx(priv, bfcontainer);

			apb = bfcontainer->apb;
			/* Check if all bfcontainer data has been received */
			if (apb->nbytes >= apb->nmaxbytes) {
				/* Report the result of the transfer */
				bfcontainer->result = 0;
				/* Add the completed buffer container to the tail of the rx.done queue */
				i2sinfo("add bfcontainer=%p to rx.done\n", bfcontainer);
				armino_buffer_debug_printf(bfcontainer);
				/* called in dma isr, not need to save irq flags */
				//flags = irqsave();
				sq_addlast((sq_entry_t *) bfcontainer, &priv->rx.done);
				I2S_RX_DOWN_START();
				I2S_RX_DOWN_END();
				//irqrestore(flags);
				i2sinfo("[%s][%d]rx.act.head=%p rx.pend.head=%p rx.done.head=%p\n", __func__, __LINE__, priv->rx.act.head, priv->rx.pend.head, priv->rx.done.head);
			}
		}
		else {
			if (rx_filled_page_size) {
				ring_buffer_read_abort(priv->ch1_rx_rb, rx_filled_page_size);
				rx_filled_page_size = 0;
				break;
			}
		}
	} while (rx_filled_page_size);

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
	struct armino_i2s_s *priv = (struct armino_i2s_s *)dev;

	/* Support 16, 24, 32 bits */
	DEBUGASSERT(priv && (bits == I2S_BITS_PER_SAMPLE_16BIT \
				|| bits == I2S_BITS_PER_SAMPLE_32BIT \
				|| bits == I2S_BITS_PER_SAMPLE_24BIT));

	priv->config.i2s_config.data_length = bits;

	/* armino 16, 24, 32, bits setting */
	//if (bits == I2S_BITS_PER_SAMPLE_16BIT) priv->i2s_object.word_length = WL_16b;
	//else if (bits == I2S_BITS_PER_SAMPLE_24BIT) priv->i2s_object.word_length = WL_24b;
	//else if (bits == I2S_BITS_PER_SAMPLE_32BIT) priv->i2s_object.word_length = WL_32b;

	return priv->config.i2s_config.data_length * priv->config.i2s_config.samp_rate;
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
	struct armino_i2s_s *priv = (struct armino_i2s_s *)dev;

	i2sinfo("[I2S RX] apb=%p nmaxbytes=%d samp=%p arg=%p timeout=%d\n", apb, apb->nmaxbytes, apb->samp, arg, timeout);

	/* apb samp is the buffer user use to receive, nmaxbyte should be less than CONFIG_ARMINO_I2S_MAXINFLIGHT*I2S_DMA_FRAME_SIZE */
	i2s_init_buffer(apb->samp, apb->nmaxbytes);

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	struct armino_buffer_s *bfcontainer;
	irqstate_t flags;
	int ret;

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

	flags = irqsave();
	sq_addlast((sq_entry_t *) bfcontainer, &priv->rx.pend);
	irqrestore(flags);

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
int i2s_transfer_rx_handleirq(uint32_t size)
{
	struct armino_i2s_s *priv = g_i2sdevice[0];
	int rx_size;

	I2S_RX_DMA_ISR_START();

	/* update rx filled page size */
	rx_filled_page_size += I2S_DMA_FRAME_SIZE;

	/* Check whether user read I2S data */
	if (!priv->apb_rx) {
		/* abort i2s received data */
		ring_buffer_read_abort(priv->ch1_rx_rb, I2S_DMA_FRAME_SIZE);
		rx_filled_page_size -= I2S_DMA_FRAME_SIZE;

		I2S_RX_DMA_ISR_END();

		return size;
	}

	//i2sinfo("[%s][%d] rx_filled_page_size=%d\n", __func__, __LINE__, rx_filled_page_size);

	rx_size = rx_filled_page_size; /* Track current byte size to increment by */
	if ((priv->apb_rx->nmaxbytes - priv->apb_rx->nbytes) <= rx_size) { /* Condition to stop sending if all data in the buffer has been sent */
		rx_size = priv->apb_rx->nmaxbytes - priv->apb_rx->nbytes;
		ring_buffer_read(priv->ch1_rx_rb, (uint8_t*)&priv->apb_rx->samp[priv->apb_rx->curbyte], rx_size);
		priv->apb_rx->nbytes += rx_size;
		priv->apb_rx->curbyte += rx_size;
		rx_filled_page_size -= rx_size;
		i2s_rxdma_callback();
	}
	else {
		ring_buffer_read(priv->ch1_rx_rb, (uint8_t*)&priv->apb_rx->samp[priv->apb_rx->curbyte], rx_size);
		priv->apb_rx->nbytes += rx_size;
		priv->apb_rx->curbyte += rx_size;
		rx_filled_page_size -= rx_size;
	}

	I2S_RX_DMA_ISR_END();

	return size;
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

static void i2s_bufsem_rx_take(struct armino_i2s_s *priv)
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

static struct armino_buffer_s *i2s_buf_rx_allocate(struct armino_i2s_s *priv)
{
	struct armino_buffer_s *bfcontainer;
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

static void i2s_buf_rx_free(struct armino_i2s_s *priv, struct armino_buffer_s *bfcontainer)
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

static void i2s_buf_rx_initialize(struct armino_i2s_s *priv)
{
	int i;

	priv->freelist_rx = NULL;
	sem_init(&priv->bufsem_rx, 0, 1);

	for (i = 0; i < CONFIG_ARMINO_I2S_MAXINFLIGHT; i++) {
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

static void i2s_bufsem_tx_take(struct armino_i2s_s *priv)
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

static struct armino_buffer_s *i2s_buf_tx_allocate(struct armino_i2s_s *priv)
{
	struct armino_buffer_s *bfcontainer;
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

static void i2s_buf_tx_free(struct armino_i2s_s *priv, struct armino_buffer_s *bfcontainer)
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

static void i2s_buf_tx_initialize(struct armino_i2s_s *priv)
{
	int i;

	priv->freelist_tx = NULL;
	sem_init(&priv->bufsem_tx, 0, 1);

	for (i = 0; i < CONFIG_ARMINO_I2S_MAXINFLIGHT; i++) {
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

static void i2s_exclsem_take(struct armino_i2s_s *priv)
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
	struct armino_i2s_s *priv = (struct armino_i2s_s *)dev;
	DEBUGASSERT(priv);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (dir == I2S_TX && priv->txenab) {
		bk_i2s_pause(I2S_TXRX_TYPE_TX);
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX && priv->rxenab) {
		bk_i2s_pause(I2S_TXRX_TYPE_RX);
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
	struct armino_i2s_s *priv = (struct armino_i2s_s *)dev;
	DEBUGASSERT(priv);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (dir == I2S_TX && priv->txenab) {
		bk_i2s_resume(I2S_TXRX_TYPE_TX);
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX && priv->rxenab) {
		bk_i2s_resume(I2S_TXRX_TYPE_RX);
	}
#endif

	return OK;
}

static int i2s_stop(struct i2s_dev_s *dev, i2s_ch_dir_t dir)
{
	struct armino_i2s_s *priv = (struct armino_i2s_s *)dev;
	irqstate_t flags;
	struct armino_buffer_s *bfcontainer;
	DEBUGASSERT(priv);

	i2s_exclsem_take(priv);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (dir == I2S_TX) {
		//i2s_disable(&priv->i2s_object);
		bk_i2s_stop();
		bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX);
		while (sq_peek(&priv->tx.pend) != NULL) {
			flags = irqsave();
			bfcontainer = (struct armino_buffer_s *)sq_remfirst(&priv->tx.pend);
			irqrestore(flags);
			apb_free(bfcontainer->apb);
			i2s_buf_tx_free(priv, bfcontainer);
		}

		while (sq_peek(&priv->tx.act) != NULL) {
			flags = irqsave();
			bfcontainer = (struct armino_buffer_s *)sq_remfirst(&priv->tx.act);
			irqrestore(flags);
			apb_free(bfcontainer->apb);
			i2s_buf_tx_free(priv, bfcontainer);
		}

		while (sq_peek(&priv->tx.done) != NULL) {
			i2sinfo("[%s][%d]tx.act.head=%p tx.pend.head=%p tx.done.head=%p\n", __func__, __LINE__, priv->tx.act.head, priv->tx.pend.head, priv->tx.done.head);
			flags = irqsave();
			bfcontainer = (struct armino_buffer_s *)sq_remfirst(&priv->tx.done);
			irqrestore(flags);
			i2sinfo("[%s][%d]tx.act.head=%p tx.pend.head=%p tx.done.head=%p\n", __func__, __LINE__, priv->tx.act.head, priv->tx.pend.head, priv->tx.done.head);
			i2s_buf_tx_free(priv, bfcontainer);
		}
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX) {
		bk_i2s_stop();
		bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX);
		while (sq_peek(&priv->rx.pend) != NULL) {
			flags = irqsave();
			bfcontainer = (struct armino_buffer_s *)sq_remfirst(&priv->rx.pend);
			irqrestore(flags);
			apb_free(bfcontainer->apb);
			i2s_buf_rx_free(priv, bfcontainer);
		}

		while (sq_peek(&priv->rx.act) != NULL) {
			flags = irqsave();
			bfcontainer = (struct armino_buffer_s *)sq_remfirst(&priv->rx.act);
			irqrestore(flags);
			apb_free(bfcontainer->apb);
			i2s_buf_rx_free(priv, bfcontainer);
		}

		while (sq_peek(&priv->rx.done) != NULL) {
			flags = irqsave();
			bfcontainer = (struct armino_buffer_s *)sq_remfirst(&priv->rx.done);
			irqrestore(flags);
			i2s_buf_rx_free(priv, bfcontainer);
		}
	}
#endif

#if (defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)) || (defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX))
	bk_i2s_deinit();
	bk_i2s_driver_deinit();
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
	struct armino_i2s_s *priv = (struct armino_i2s_s *)dev;
	DEBUGASSERT(priv);

	priv->err_cb = cb;
	priv->err_cb_arg = arg;

	return OK;
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
	struct armino_i2s_s *priv = (struct armino_i2s_s *)dev;
	DEBUGASSERT(priv && rate > 0);

	priv->config.i2s_config.samp_rate = rate;

	return priv->config.i2s_config.samp_rate;
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

static int i2s_allocate_wd(struct armino_i2s_s *priv)
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
	if (priv->rx.dog){
		wd_delete(priv->rx.dog);
		priv->rx.dog = NULL;
	}
#endif
#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (priv->tx.dog){
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
struct i2s_dev_s *armino_i2s_initialize(uint16_t port)
{
	if (port >= I2S_NUM_MAX) {
		i2serr("ERROR: Port number outside the allowed port number range\n");
		return NULL;
	}

	if (g_i2sdevice[port] != NULL) {
		i2sinfo("g_i2sdevice[%d] already initialized\n", port);
		return &g_i2sdevice[port]->dev;
	}

	struct armino_i2s_s *priv;
	int ret;

	/* Allocate a new state structure for this chip select.  NOTE that there
	 * is no protection if the same chip select is used in two different
	 * chip select structures.
	 */
	priv = (struct armino_i2s_s *)kmm_zalloc(sizeof(struct armino_i2s_s));
	if (!priv) {
		i2serr("ERROR: Failed to allocate a chip select structure\n");
		return NULL;
	}
	os_memset(priv, 0, sizeof(struct armino_i2s_s));

	/* init i2s driver */
	i2sinfo("init i2s driver \n");
	bk_i2s_driver_init();

	//init i2s configure
	i2sinfo("init i2s configure \n");
	i2s_config_t i2s_config = DEFAULT_I2S_CONFIG();
	i2s_config.role = I2S_WORK_ROLE;
	i2s_config.store_mode = I2S_LRCOM_STORE_16R16L;
	//i2s_config.samp_rate = I2S_SAMP_RATE_44100;
	bk_i2s_init(CONFIG_ARMINO_I2S_GPIO_GROUP_ID, &i2s_config);
	i2sinfo("init i2s driver, group_id: %d and config successful \n", CONFIG_ARMINO_I2S_GPIO_GROUP_ID);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	i2sinfo("init i2s channel 1 tx \n");
	ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX, 640, i2s_transfer_tx_handleirq, &priv->ch1_tx_rb);
	if (ret != BK_OK) {
		i2sinfo("bk_i2s_chl_init fail \n");
		goto errout_with_alloc;
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	i2sinfo("init i2s channel 1 rx \n");
	ret = bk_i2s_chl_init(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX, 640, i2s_transfer_rx_handleirq, &priv->ch1_rx_rb);
	if (ret != BK_OK) {
		i2sinfo("bk_i2s_chl_init fail \n");
		goto errout_with_alloc;
	}
#endif

	/* Initialize buffering */
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	i2sinfo("init i2s channel 1 rx buffering \n");
	i2s_buf_rx_initialize(priv);
	priv->rxenab = 1;
#endif
#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	i2sinfo("init i2s channel 1 tx buffering \n");
	i2s_buf_tx_initialize(priv);
	priv->txenab = 1;
#endif

	/* Initialize the I2S priv device structure  */
	sem_init(&priv->exclsem, 0, 1);
	priv->dev.ops = &g_i2sops;

	ret = i2s_allocate_wd(priv);
	if (ret != OK) {
		goto errout_with_alloc;
	}

	/* Basic settings */
	g_i2sdevice[port] = priv;

#if (defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)) || (defined(I2S_HAVE_TX) && (0 == I2S_HAVE_TX))
	i2sinfo("i2s channel 1 start \n");
	bk_i2s_start();
#endif

	/* Success exit */
	i2sinfo("[%s][%d] tx=%p tx.act.head=%p tx.pend.head=%p tx.done.head=%p\n", __func__, __LINE__, &priv->tx, priv->tx.act.head, priv->tx.pend.head, priv->tx.done.head);
	i2sinfo("[%s][%d] rx=%p rx.act.head=%p rx.pend.head=%p rx.done.head=%p\n", __func__, __LINE__, &priv->rx, priv->rx.act.head, priv->rx.pend.head, priv->rx.done.head);
	return &priv->dev;

	/* Failure exits */
errout_with_alloc:
	sem_destroy(&priv->exclsem);
	kmm_free(priv);
	return NULL;
}
#endif							/* I2S_HAVE_RX || I2S_HAVE_TX */
#endif							/* CONFIG_AUDIO */
