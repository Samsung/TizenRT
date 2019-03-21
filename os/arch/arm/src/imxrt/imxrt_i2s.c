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
#include "imxrt_i2s.h"
#include "imxrt_periphclks.h"
#include "imxrt_clock.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/
#define I2S_MAX_BUFFER_SIZE     (4096 - 8)	//the maximum RAM limited by lldesc
#define PLL_CLK                 (320000000)
#define I2S_BASE_CLK            (PLL_CLK / 2)
#define I2S_FULL_DUPLEX_SLAVE_MODE_MASK   (I2S_MODE_TX | I2S_MODE_RX | I2S_MODE_SLAVE)
#define I2S_FULL_DUPLEX_MASTER_MODE_MASK  (I2S_MODE_TX | I2S_MODE_RX | I2S_MODE_MASTER)

#define APLL_MIN_FREQ (250000000)
#define APLL_MAX_FREQ (500000000)
#define APLL_I2S_MIN_RATE (10675)	//in Hz, I2S Clock rate limited by hardware

#ifndef CONFIG_SCHED_WORKQUEUE
#error Work queue support is required (CONFIG_SCHED_WORKQUEUE)
#endif

#ifndef CONFIG_AUDIO
#error CONFIG_AUDIO required by this driver
#endif

#ifndef CONFIG_IMXRT_I2S_MAXINFLIGHT
#define CONFIG_IMXRT_I2S_MAXINFLIGHT 32
#endif

/* Assume no RX/TX support until we learn better */
#undef I2S_HAVE_RX
#undef I2S_HAVE_TX

#if defined(CONFIG_IMXRT_I2S)

/* Check for I2S RX support */
#if defined(CONFIG_IMXRT_I2S_RX)
#define I2S_HAVE_RX     1
#endif

/* Check for I2S TX support */
#if defined(CONFIG_IMXRT_I2S_TX)
#define I2S_HAVE_TX     1
#endif

#ifndef CONFIG_IMXRT_I2S_DATALEN
#define CONFIG_IMXRT_I2S_DATALEN        (I2S_BITS_PER_SAMPLE_16BIT)
#endif

#ifndef CONFIG_IMXRT_I2S_SAMPLERATE
#define CONFIG_IMXRT_I2S_SAMPLERATE     (44100)
#endif

#ifndef CONFIG_IMXRT_I2S_CHANNEL_FMT
#define CONFIG_IMXRT_I2S_CHANNEL_FMT    (I2S_CHANNEL_FMT_RIGHT_LEFT)
#endif

#ifndef CONFIG_IMXRT_I2S_COMM_FMT
#define CONFIG_IMXRT_I2S_COMM_FMT       I2S_COMM_FORMAT_I2S
#endif

#ifndef CONFIG_IMXRT_I2S_INTR_LEVEL
#define CONFIG_IMXRT_I2S_INTR_LEVEL    (1)
#endif

#endif

#ifdef CONFIG_DEBUG
#define CONFIG_DEBUG_FEATURES
#endif

#define DEBUG_I2S_DRIVER    0

#undef i2serr
#undef i2sinfo
#if defined(DEBUG_I2S_DRIVER) && (0 < DEBUG_I2S_DRIVER)
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

/****************************************************************************
 * Private Types
 ****************************************************************************/

enum _sai_transfer_state {
    kSAI_Busy = 0x0U, /*!< SAI is busy */
    kSAI_Idle,        /*!< Transfer is done. */
    kSAI_Error        /*!< Transfer error occured. */
};

/* I2S configuration parameters for i2s_param_config function */
typedef struct {
	i2s_mode_t mode;			/*!< I2S work mode */
	int sample_rate;			/*!< I2S sample rate */
	i2s_bits_per_sample_t bits_per_sample;	/*!< I2S bits per sample */
	i2s_channel_fmt_t channel_format;	/*!< I2S channel format */
	i2s_comm_format_t communication_format;	/*!< I2S communication format */

	int int_alloc_level;		/*INT level */

	bool use_apll;				/*!< I2S using APLL as main I2S clock, enable it to get accurate clock */
	int fixed_mclk;				/*!< I2S using fixed MCLK output. If use_apll = true and fixed_mclk > 0, then the clock output for i2s is fixed and equal to the fixed_mclk value. */
} i2s_config_t;

/* The state of the one I2S peripheral */

struct imxrt_i2s_s {
	struct i2s_dev_s dev;		/* Externally visible I2S interface, must the first element!! */

	i2s_port_t i2s_num;			/* I2S controller register base address */
	I2S_Type * i2s_base;

	int intrupt_level;
	int tx_isr_num;				/* isr tx number */
	int rx_isr_num;             /* isr rx number */
	xcpt_t isr_handler;			/* irs handler */

	i2s_err_cb_t err_cb;		/* registered error callback function */
	void *err_cb_arg;			/* argiment to return with err cb call */

	sem_t exclsem;				/* Assures mutually exclusive acess to I2S */

	uint8_t rxenab:1;			/* True: RX transfers enabled */
	uint8_t txenab:1;			/* True: TX transfers enabled */

	i2s_mode_t mode;			/*!< I2S work mode */
	int sample_rate;			/*!< I2S sample rate */
	int channel_num;			/*!< Number of channels */
	int bytes_per_sample;		/*!< Bytes per sample */
	int bits_per_sample;		/*!< Bits per sample */

	bool use_apll;				/*!< I2S use APLL clock */
	int fixed_mclk;				/*!< I2S fixed MLCK clock */

	sai_handle_t   si_tx_handle;      /*!< Bits per sample */
    sai_handle_t   si_rx_handle;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* Register helpers */
#define CONFIG_IMXRT_I2S_DUMPBUFFERS   0

#if defined(CONFIG_IMXRT_I2S_DUMPBUFFERS) && (0 < CONFIG_IMXRT_I2S_DUMPBUFFERS)
#define       i2s_init_buffer(b, s)   memset(b, 0x55, s);
#define       i2s_dump_buffer(m, b, s) lib_dumpbuffer(m, b, s)
#else
#define       i2s_init_buffer(b, s)
#define       i2s_dump_buffer(m, b, s)
#endif

/* Semaphore helpers */
static void i2s_exclsem_take(struct imxrt_i2s_s *priv);
#define i2s_exclsem_give(priv) sem_post(&priv->exclsem)

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

int i2s_set_clk(struct imxrt_i2s_s *priv, uint32_t rate, i2s_bits_per_sample_t bits, i2s_channel_t ch);

/****************************************************************************
* External functions
****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/
/* Base pointer array */
static I2S_Type *const s_saiBases[] = I2S_BASE_PTRS;
/*!@brief SAI handle pointer */
sai_handle_t *s_saiHandle[ARRAY_SIZE(s_saiBases)][2];

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/* Clock name array */
static const clock_ip_name_t s_saiClock[] = SAI_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

static const i2s_config_t i2s0_default_config = {
#if defined(CONFIG_IMXRT_I2S0_MODE_MASTER) && (0 < CONFIG_IMXRT_I2S0_MODE_MASTER)
	.mode = I2S_MODE_MASTER,
#else
	.mode = I2S_MODE_SLAVE,
#endif

	.sample_rate = CONFIG_IMXRT_I2S_SAMPLERATE,
	.bits_per_sample = CONFIG_IMXRT_I2S_DATALEN,
	.channel_format = CONFIG_IMXRT_I2S_CHANNEL_FMT,
	.communication_format = CONFIG_IMXRT_I2S_COMM_FMT,
	.int_alloc_level = CONFIG_IMXRT_I2S_INTR_LEVEL,
};

static const i2s_config_t i2s1_default_config = {
#if defined(CONFIG_IMXRT_I2S1_MODE_MASTER) && (0 < CONFIG_IMXRT_I2S1_MODE_MASTER)
	.mode = I2S_MODE_MASTER,
#else
	.mode = I2S_MODE_SLAVE,
#endif

	.sample_rate = CONFIG_IMXRT_I2S_SAMPLERATE,
	.bits_per_sample = CONFIG_IMXRT_I2S_DATALEN,
	.channel_format = CONFIG_IMXRT_I2S_CHANNEL_FMT,
	.communication_format = CONFIG_IMXRT_I2S_COMM_FMT,
	.int_alloc_level = CONFIG_IMXRT_I2S_INTR_LEVEL,
};

static const i2s_config_t i2s2_default_config = {
#if defined(CONFIG_IMXRT_I2S1_MODE_MASTER) && (0 < CONFIG_IMXRT_I2S1_MODE_MASTER)
	.mode = I2S_MODE_MASTER,
#else
	.mode = I2S_MODE_SLAVE,
#endif

	.sample_rate = CONFIG_IMXRT_I2S_SAMPLERATE,
	.bits_per_sample = CONFIG_IMXRT_I2S_DATALEN,
	.channel_format = CONFIG_IMXRT_I2S_CHANNEL_FMT,
	.communication_format = CONFIG_IMXRT_I2S_COMM_FMT,
	.int_alloc_level = CONFIG_IMXRT_I2S_INTR_LEVEL,
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

static struct imxrt_i2s_s *g_i2sdevice[I2S_NUM_MAX] = {NULL};

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/*!
 * @brief Resets the SAI Tx.
 *
 * This function enables the software reset and FIFO reset of SAI Tx. After reset, clear the reset bit.
 *
 * @param base SAI base pointer
 */
void i2s_tx_reset(I2S_Type *base)
{
    /* Set the software reset and FIFO reset to clear internal state */
    base->TCSR = I2S_TCSR_SR_MASK | I2S_TCSR_FR_MASK;

    /* Clear software reset bit, this should be done by software */
    base->TCSR &= ~I2S_TCSR_SR_MASK;

    /* Reset all Tx register values */
    base->TCR2 = 0;
    base->TCR3 = 0;
    base->TCR4 = 0;
    base->TCR5 = 0;
    base->TMR = 0;
}

/*!
 * @brief Resets the SAI Rx.
 *
 * This function enables the software reset and FIFO reset of SAI Rx. After reset, clear the reset bit.
 *
 * @param base SAI base pointer
 */
void i2s_rx_reset(I2S_Type *base)
{
    /* Set the software reset and FIFO reset to clear internal state */
    base->RCSR = I2S_RCSR_SR_MASK | I2S_RCSR_FR_MASK;

    /* Clear software reset bit, this should be done by software */
    base->RCSR &= ~I2S_RCSR_SR_MASK;

    /* Reset all Rx register values */
    base->RCR2 = 0;
    base->RCR3 = 0;
    base->RCR4 = 0;
    base->RCR5 = 0;
    base->RMR = 0;
}

/*! @} */

/*!
 * @name Status
 * @{
 */

/*!
 * @brief Gets the SAI Tx status flag state.
 *
 * @param base SAI base pointer
 * @return SAI Tx status flag value. Use the Status Mask to get the status value needed.
 */
static inline uint32_t i2s_tx_getstatusflag(I2S_Type *base)
{
    return base->TCSR;
}

/*!
 * @brief Clears the SAI Tx status flag state.
 *
 * @param base SAI base pointer
 * @param mask State mask. It can be a combination of the following source if defined:
 *        @arg kSAI_WordStartFlag
 *        @arg kSAI_SyncErrorFlag
 *        @arg kSAI_FIFOErrorFlag
 */
static inline void i2s_tx_clearstatusflag(I2S_Type *base, uint32_t mask)
{
    base->TCSR = ((base->TCSR & 0xFFE3FFFFU) | mask);
}

/*!
 * @brief Gets the SAI Tx status flag state.
 *
 * @param base SAI base pointer
 * @return SAI Rx status flag value. Use the Status Mask to get the status value needed.
 */
static inline uint32_t i2s_rx_getstatusflag(I2S_Type *base)
{
    return base->RCSR;
}

/*!
 * @brief Clears the SAI Rx status flag state.
 *
 * @param base SAI base pointer
 * @param mask State mask. It can be a combination of the following sources if defined.
 *        @arg kSAI_WordStartFlag
 *        @arg kSAI_SyncErrorFlag
 *        @arg kSAI_FIFOErrorFlag
 */
static inline void i2s_rx_clearstatusflags(I2S_Type *base, uint32_t mask)
{
    base->RCSR = ((base->RCSR & 0xFFE3FFFFU) | mask);
}

/*!
 * @brief Do software reset or FIFO reset .
 *
 * FIFO reset means clear all the data in the FIFO, and make the FIFO pointer both to 0.
 * Software reset means claer the Tx internal logic, including the bit clock, frame count etc. But software
 * reset will not clear any configuration registers like TCR1~TCR5.
 * This function will also clear all the error flags such as FIFO error, sync error etc.
 *
 * @param base SAI base pointer
 * @param type Reset type, FIFO reset or software reset
 */
void i2s_tx_softwarereset(I2S_Type *base, sai_reset_type_t type)
{
    base->TCSR |= (uint32_t)type;

    /* Clear the software reset */
    base->TCSR &= ~I2S_TCSR_SR_MASK;
}

/*!
 * @brief Do software reset or FIFO reset .
 *
 * FIFO reset means clear all the data in the FIFO, and make the FIFO pointer both to 0.
 * Software reset means claer the Rx internal logic, including the bit clock, frame count etc. But software
 * reset will not clear any configuration registers like RCR1~RCR5.
 * This function will also clear all the error flags such as FIFO error, sync error etc.
 *
 * @param base SAI base pointer
 * @param type Reset type, FIFO reset or software reset
 */
void i2s_rx_softwarereset(I2S_Type *base, sai_reset_type_t type)
{
    base->RCSR |= (uint32_t)type;

    /* Clear the software reset */
    base->RCSR &= ~I2S_RCSR_SR_MASK;
}

/*!
 * @brief Set the Tx channel FIFO enable mask.
 *
 * @param base SAI base pointer
 * @param mask Channel enable mask, 0 means all channel FIFO disabled, 1 means channel 0 enabled,
 * 3 means both channel 0 and channel 1 enabled.
 */
void i2s_tx_setchannel_fifomask(I2S_Type *base, uint8_t mask)
{
    base->TCR3 &= ~I2S_TCR3_TCE_MASK;
    base->TCR3 |= I2S_TCR3_TCE(mask);
}

/*!
 * @brief Set the Rx channel FIFO enable mask.
 *
 * @param base SAI base pointer
 * @param mask Channel enable mask, 0 means all channel FIFO disabled, 1 means channel 0 enabled,
 * 3 means both channel 0 and channel 1 enabled.
 */
void i2s_rx_setchannel_fifomask(I2S_Type *base, uint8_t mask)
{
    base->RCR3 &= ~I2S_RCR3_RCE_MASK;
    base->RCR3 |= I2S_RCR3_RCE(mask);
}

/*! @} */

/*!
 * @name Interrupts
 * @{
 */

/*!
 * @brief Enables the SAI Tx interrupt requests.
 *
 * @param base SAI base pointer
 * @param mask interrupt source
 *     The parameter can be a combination of the following sources if defined.
 *     @arg kSAI_WordStartInterruptEnable
 *     @arg kSAI_SyncErrorInterruptEnable
 *     @arg kSAI_FIFOWarningInterruptEnable
 *     @arg kSAI_FIFORequestInterruptEnable
 *     @arg kSAI_FIFOErrorInterruptEnable
 */
static inline void i2s_tx_enableinterrupts(I2S_Type *base, uint32_t mask)
{
    base->TCSR = ((base->TCSR & 0xFFE3FFFFU) | mask);
}

/*!
 * @brief Enables the SAI Rx interrupt requests.
 *
 * @param base SAI base pointer
 * @param mask interrupt source
 *     The parameter can be a combination of the following sources if defined.
 *     @arg kSAI_WordStartInterruptEnable
 *     @arg kSAI_SyncErrorInterruptEnable
 *     @arg kSAI_FIFOWarningInterruptEnable
 *     @arg kSAI_FIFORequestInterruptEnable
 *     @arg kSAI_FIFOErrorInterruptEnable
 */
static inline void i2s_rx_enableinterrupts(I2S_Type *base, uint32_t mask)
{
    base->RCSR = ((base->RCSR & 0xFFE3FFFFU) | mask);
}

/*!
 * @brief Disables the SAI Tx interrupt requests.
 *
 * @param base SAI base pointer
 * @param mask interrupt source
 *     The parameter can be a combination of the following sources if defined.
 *     @arg kSAI_WordStartInterruptEnable
 *     @arg kSAI_SyncErrorInterruptEnable
 *     @arg kSAI_FIFOWarningInterruptEnable
 *     @arg kSAI_FIFORequestInterruptEnable
 *     @arg kSAI_FIFOErrorInterruptEnable
 */
static inline void i2s_tx_disableinterrupts(I2S_Type *base, uint32_t mask)
{
    base->TCSR = ((base->TCSR & 0xFFE3FFFFU) & (~mask));
}

/*!
 * @brief Disables the SAI Rx interrupt requests.
 *
 * @param base SAI base pointer
 * @param mask interrupt source
 *     The parameter can be a combination of the following sources if defined.
 *     @arg kSAI_WordStartInterruptEnable
 *     @arg kSAI_SyncErrorInterruptEnable
 *     @arg kSAI_FIFOWarningInterruptEnable
 *     @arg kSAI_FIFORequestInterruptEnable
 *     @arg kSAI_FIFOErrorInterruptEnable
 */
static inline void i2s_rx_disableinterrupts(I2S_Type *base, uint32_t mask)
{
    base->RCSR = ((base->RCSR & 0xFFE3FFFFU) & (~mask));
}

/*! @} */

/*!
 * @name DMA Control
 * @{
 */

/*!
 * @brief Enables/disables the SAI Tx DMA requests.
 * @param base SAI base pointer
 * @param mask DMA source
 *     The parameter can be combination of the following sources if defined.
 *     @arg kSAI_FIFOWarningDMAEnable
 *     @arg kSAI_FIFORequestDMAEnable
 * @param enable True means enable DMA, false means disable DMA.
 */
static inline void i2s_tx_enabledma(I2S_Type *base, uint32_t mask, bool enable)
{
    if (enable) {
        base->TCSR = ((base->TCSR & 0xFFE3FFFFU) | mask);
    } else {
        base->TCSR = ((base->TCSR & 0xFFE3FFFFU) & (~mask));
    }
}

/*!
 * @brief Enables/disables the SAI Rx DMA requests.
 * @param base SAI base pointer
 * @param mask DMA source
 *     The parameter can be a combination of the following sources if defined.
 *     @arg kSAI_FIFOWarningDMAEnable
 *     @arg kSAI_FIFORequestDMAEnable
 * @param enable True means enable DMA, false means disable DMA.
 */
static inline void i2s_rx_enabledma(I2S_Type *base, uint32_t mask, bool enable)
{
    if (enable) {
        base->RCSR = ((base->RCSR & 0xFFE3FFFFU) | mask);
    } else {
        base->RCSR = ((base->RCSR & 0xFFE3FFFFU) & (~mask));
    }
}

/*!
 * @brief  Gets the SAI Tx data register address.
 *
 * This API is used to provide a transfer address for the SAI DMA transfer configuration.
 *
 * @param base SAI base pointer.
 * @param channel Which data channel used.
 * @return data register address.
 */
static inline uint32_t i2s_tx_getdataregisteraddress(I2S_Type *base, uint32_t channel)
{
    return (uint32_t)(&(base->TDR)[channel]);
}

/*!
 * @brief  Gets the SAI Rx data register address.
 *
 * This API is used to provide a transfer address for the SAI DMA transfer configuration.
 *
 * @param base SAI base pointer.
 * @param channel Which data channel used.
 * @return data register address.
 */
static inline uint32_t i2s_rx_getdataregisteraddress(I2S_Type *base, uint32_t channel)
{
    return (uint32_t)(&(base->RDR)[channel]);
}

#if defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER)
/*!
 * @brief Set the master clock divider.
 *
 * This API will compute the master clock divider according to master clock frequency and master
 * clock source clock source frequency.
 *
 * @param base SAI base pointer.
 * @param mclk_Hz Mater clock frequency in Hz.
 * @param mclkSrcClock_Hz Master clock source frequency in Hz.
 */
static void i2s_setmasterclockdivider(I2S_Type *base, uint32_t mclk_Hz, uint32_t mclkSrcClock_Hz)
{
    uint32_t freq = mclkSrcClock_Hz;
    uint16_t fract, divide;
    uint32_t remaind = 0;
    uint32_t current_remainder = 0xFFFFFFFFU;
    uint16_t current_fract = 0;
    uint16_t current_divide = 0;
    uint32_t mul_freq = 0;
    uint32_t max_fract = 256;

    /*In order to prevent overflow */
    freq /= 100;
    mclk_Hz /= 100;

    /* Compute the max fract number */
    max_fract = mclk_Hz * 4096 / freq + 1;
    if (max_fract > 256) {
        max_fract = 256;
    }

    /* Looking for the closet frequency */
    for (fract = 1; fract < max_fract; fract++) {
        mul_freq = freq * fract;
        remaind = mul_freq % mclk_Hz;
        divide = mul_freq / mclk_Hz;

        /* Find the exactly frequency */
        if (remaind == 0) {
            current_fract = fract;
            current_divide = mul_freq / mclk_Hz;
            break;
        }

        /* Closer to next one, set the closest to next data */
        if (remaind > mclk_Hz / 2) {
            remaind = mclk_Hz - remaind;
            divide += 1;
        }

        /* Update the closest div and fract */
        if (remaind < current_remainder) {
            current_fract = fract;
            current_divide = divide;
            current_remainder = remaind;
        }
    }

    /* Fill the computed fract and divider to registers */
    base->MDR = I2S_MDR_DIVIDE(current_divide - 1) | I2S_MDR_FRACT(current_fract - 1);

    /* Waiting for the divider updated */
    while (base->MCR & I2S_MCR_DUF_MASK) {
    }
}
#endif /* FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER */

/*!
 * @brief Get the instance number for SAI.
 *
 * @param base SAI base pointer.
 */
uint32_t i2s_getinstance(I2S_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_saiBases); instance++) {
        if (s_saiBases[instance] == base) {
            break;
        }
    }

    DEBUGASSERT(instance < ARRAY_SIZE(s_saiBases));

    return instance;
}

/*!
 * @brief sends a piece of data in non-blocking way.
 *
 * @param base SAI base pointer
 * @param channel Data channel used.
 * @param bitWidth How many bits in a audio word, usually 8/16/24/32 bits.
 * @param buffer Pointer to the data to be written.
 * @param size Bytes to be written.
 */
static void i2s_write_nonblocking(I2S_Type *base, uint32_t channel, uint32_t bitWidth, uint8_t *buffer, uint32_t size)
{
    uint32_t i = 0;
    uint8_t j = 0;
    uint8_t bytesPerWord = bitWidth / 8U;
    uint32_t data = 0;
    uint32_t temp = 0;

    for (i = 0; i < size / bytesPerWord; i++) {
        for (j = 0; j < bytesPerWord; j++) {
            temp = (uint32_t)(*buffer);
            data |= (temp << (8U * j));
            buffer++;
        }
        base->TDR[channel] = data;
        data = 0;
    }
}

/*!
 * @brief Receive a piece of data in non-blocking way.
 *
 * @param base SAI base pointer
 * @param channel Data channel used.
 * @param bitWidth How many bits in a audio word, usually 8/16/24/32 bits.
 * @param buffer Pointer to the data to be read.
 * @param size Bytes to be read.
 */
static void i2s_read_nonblocking(I2S_Type *base, uint32_t channel, uint32_t bitWidth, uint8_t *buffer, uint32_t size)
{
    uint32_t i = 0;
    uint8_t j = 0;
    uint8_t bytesPerWord = bitWidth / 8U;
    uint32_t data = 0;

    for (i = 0; i < size / bytesPerWord; i++) {
        data = base->RDR[channel];
        for (j = 0; j < bytesPerWord; j++) {
            *buffer = (data >> (8U * j)) & 0xFF;
            buffer++;
        }
    }
}

/*!
 * @brief Enables/disables the SAI Tx.
 *
 * @param base SAI base pointer
 * @param enable True means enable SAI Tx, false means disable.
 */
void i2s_tx_enable(I2S_Type *base, bool enable)
{
    if (enable) {
        /* If clock is sync with Rx, should enable RE bit. */
        if (((base->TCR2 & I2S_TCR2_SYNC_MASK) >> I2S_TCR2_SYNC_SHIFT) == 0x1U) {
            base->RCSR = ((base->RCSR & 0xFFE3FFFFU) | I2S_RCSR_RE_MASK);
        }
        base->TCSR = ((base->TCSR & 0xFFE3FFFFU) | I2S_TCSR_TE_MASK);
        /* Also need to clear the FIFO error flag before start */
        i2s_tx_clearstatusflag(base, kSAI_FIFOErrorFlag);
    } else {
        /* If RE not sync with TE, than disable TE, otherwise, shall not disable TE */
        if (((base->RCR2 & I2S_RCR2_SYNC_MASK) >> I2S_RCR2_SYNC_SHIFT) != 0x1U) {
            /* Should not close RE even sync with Rx */
            base->TCSR = ((base->TCSR & 0xFFE3FFFFU) & (~I2S_TCSR_TE_MASK));
        }
    }
}

/*!
 * @brief Enables/disables the SAI Rx.
 *
 * @param base SAI base pointer
 * @param enable True means enable SAI Rx, false means disable.
 */
void i2s_rx_enable(I2S_Type *base, bool enable)
{
    if (enable) {
        /* If clock is sync with Tx, should enable TE bit. */
        if (((base->RCR2 & I2S_RCR2_SYNC_MASK) >> I2S_RCR2_SYNC_SHIFT) == 0x1U) {
            base->TCSR = ((base->TCSR & 0xFFE3FFFFU) | I2S_TCSR_TE_MASK);
        }
        base->RCSR = ((base->RCSR & 0xFFE3FFFFU) | I2S_RCSR_RE_MASK);
        /* Also need to clear the FIFO error flag before start */
        i2s_rx_clearstatusflags(base, kSAI_FIFOErrorFlag);
    } else {
        /* While TX is not sync with RX, close RX */
        if (((base->TCR2 & I2S_TCR2_SYNC_MASK) >> I2S_TCR2_SYNC_SHIFT) != 0x1U) {
            base->RCSR = ((base->RCSR & 0xFFE3FFFFU) & (~I2S_RCSR_RE_MASK));
        }
    }
}

/*!
 * @name Initialization and deinitialization
 * @{
 */

/*!
 * @brief Initializes the SAI Tx peripheral.
 *
 * Ungates the SAI clock, resets the module, and configures SAI Tx with a configuration structure.
 * The configuration structure can be custom filled or set with default values by
 * SAI_TxGetDefaultConfig().
 *
 * @note  This API should be called at the beginning of the application to use
 * the SAI driver. Otherwise, accessing the SAIM module can cause a hard fault
 * because the clock is not enabled.
 *
 * @param base SAI base pointer
 * @param config SAI configuration structure.
*/
void i2s_tx_init(I2S_Type *base, const sai_config_t *config)
{
    uint32_t val = 0;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable the SAI clock */
    imxrt_clock_enableclock(s_saiClock[i2s_getinstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

#if defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)
    /* Master clock source setting */
    val = (base->MCR & ~I2S_MCR_MICS_MASK);
    base->MCR = (val | I2S_MCR_MICS(config->mclkSource));

    /* Configure Master clock output enable */
    val = (base->MCR & ~I2S_MCR_MOE_MASK);
    base->MCR = (val | I2S_MCR_MOE(config->mclkOutputEnable));
#endif /* FSL_FEATURE_SAI_HAS_MCR */

    /* Configure audio protocol */
    switch (config->protocol) {
    case kSAI_BusLeftJustified:
        base->TCR2 |= I2S_TCR2_BCP_MASK;
        base->TCR3 &= ~I2S_TCR3_WDFL_MASK;
        base->TCR4 = I2S_TCR4_MF(1U) | I2S_TCR4_SYWD(31U) | I2S_TCR4_FSE(0U) | I2S_TCR4_FSP(0U) | I2S_TCR4_FRSZ(1U);
        break;
    case kSAI_BusRightJustified:
        base->TCR2 |= I2S_TCR2_BCP_MASK;
        base->TCR3 &= ~I2S_TCR3_WDFL_MASK;
        base->TCR4 = I2S_TCR4_MF(1U) | I2S_TCR4_SYWD(31U) | I2S_TCR4_FSE(0U) | I2S_TCR4_FSP(0U) | I2S_TCR4_FRSZ(1U);
        break;
    case kSAI_BusI2S:
        base->TCR2 |= I2S_TCR2_BCP_MASK;
        base->TCR3 &= ~I2S_TCR3_WDFL_MASK;
        base->TCR4 = I2S_TCR4_MF(1U) | I2S_TCR4_SYWD(31U) | I2S_TCR4_FSE(1U) | I2S_TCR4_FSP(1U) | I2S_TCR4_FRSZ(1U);
        break;
    case kSAI_BusPCMA:
        base->TCR2 &= ~I2S_TCR2_BCP_MASK;
        base->TCR3 &= ~I2S_TCR3_WDFL_MASK;
        base->TCR4 = I2S_TCR4_MF(1U) | I2S_TCR4_SYWD(0U) | I2S_TCR4_FSE(1U) | I2S_TCR4_FSP(0U) | I2S_TCR4_FRSZ(1U);
        break;
    case kSAI_BusPCMB:
        base->TCR2 &= ~I2S_TCR2_BCP_MASK;
        base->TCR3 &= ~I2S_TCR3_WDFL_MASK;
        base->TCR4 = I2S_TCR4_MF(1U) | I2S_TCR4_SYWD(0U) | I2S_TCR4_FSE(0U) | I2S_TCR4_FSP(0U) | I2S_TCR4_FRSZ(1U);
        break;
    default:
        break;
    }

    /* Set master or slave */
    if (config->masterSlave == kSAI_Master) {
        base->TCR2 |= I2S_TCR2_BCD_MASK;
        base->TCR4 |= I2S_TCR4_FSD_MASK;

        /* Bit clock source setting */
        val = base->TCR2 & (~I2S_TCR2_MSEL_MASK);
        base->TCR2 = (val | I2S_TCR2_MSEL(config->bclkSource));
    } else {
        base->TCR2 &= ~I2S_TCR2_BCD_MASK;
        base->TCR4 &= ~I2S_TCR4_FSD_MASK;
    }

    /* Set Sync mode */
    switch (config->syncMode) {
    case kSAI_ModeAsync:
        val = base->TCR2;
        val &= ~I2S_TCR2_SYNC_MASK;
        base->TCR2 = (val | I2S_TCR2_SYNC(0U));
        break;
    case kSAI_ModeSync:
        val = base->TCR2;
        val &= ~I2S_TCR2_SYNC_MASK;
        base->TCR2 = (val | I2S_TCR2_SYNC(1U));
        /* If sync with Rx, should set Rx to async mode */
        val = base->RCR2;
        val &= ~I2S_RCR2_SYNC_MASK;
        base->RCR2 = (val | I2S_RCR2_SYNC(0U));
        break;
    case kSAI_ModeSyncWithOtherTx:
        val = base->TCR2;
        val &= ~I2S_TCR2_SYNC_MASK;
        base->TCR2 = (val | I2S_TCR2_SYNC(2U));
        break;
    case kSAI_ModeSyncWithOtherRx:
        val = base->TCR2;
        val &= ~I2S_TCR2_SYNC_MASK;
        base->TCR2 = (val | I2S_TCR2_SYNC(3U));
        break;
    default:
        break;
    }
}

/*!
 * @brief Initializes the the SAI Rx peripheral.
 *
 * Ungates the SAI clock, resets the module, and configures the SAI Rx with a configuration structure.
 * The configuration structure can be custom filled or set with default values by
 * SAI_RxGetDefaultConfig().
 *
 * @note  This API should be called at the beginning of the application to use
 * the SAI driver. Otherwise, accessing the SAI module can cause a hard fault
 * because the clock is not enabled.
 *
 * @param base SAI base pointer
 * @param config SAI configuration structure.
 */
void i2s_rx_init(I2S_Type *base, const sai_config_t *config)
{
    uint32_t val = 0;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable SAI clock first. */
	imxrt_clock_enableclock(s_saiClock[i2s_getinstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

#if defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)
    /* Master clock source setting */
    val = (base->MCR & ~I2S_MCR_MICS_MASK);
    base->MCR = (val | I2S_MCR_MICS(config->mclkSource));

    /* Configure Master clock output enable */
    val = (base->MCR & ~I2S_MCR_MOE_MASK);
    base->MCR = (val | I2S_MCR_MOE(config->mclkOutputEnable));
#endif /* FSL_FEATURE_SAI_HAS_MCR */

    /* Configure audio protocol */
    switch (config->protocol) {
    case kSAI_BusLeftJustified:
        base->RCR2 |= I2S_RCR2_BCP_MASK;
        base->RCR3 &= ~I2S_RCR3_WDFL_MASK;
        base->RCR4 = I2S_RCR4_MF(1U) | I2S_RCR4_SYWD(31U) | I2S_RCR4_FSE(0U) | I2S_RCR4_FSP(0U) | I2S_RCR4_FRSZ(1U);
        break;
    case kSAI_BusRightJustified:
        base->RCR2 |= I2S_RCR2_BCP_MASK;
        base->RCR3 &= ~I2S_RCR3_WDFL_MASK;
        base->RCR4 = I2S_RCR4_MF(1U) | I2S_RCR4_SYWD(31U) | I2S_RCR4_FSE(0U) | I2S_RCR4_FSP(0U) | I2S_RCR4_FRSZ(1U);
        break;
    case kSAI_BusI2S:
        base->RCR2 |= I2S_RCR2_BCP_MASK;
        base->RCR3 &= ~I2S_RCR3_WDFL_MASK;
        base->RCR4 = I2S_RCR4_MF(1U) | I2S_RCR4_SYWD(31U) | I2S_RCR4_FSE(1U) | I2S_RCR4_FSP(1U) | I2S_RCR4_FRSZ(1U);
        break;
    case kSAI_BusPCMA:
        base->RCR2 &= ~I2S_RCR2_BCP_MASK;
        base->RCR3 &= ~I2S_RCR3_WDFL_MASK;
        base->RCR4 = I2S_RCR4_MF(1U) | I2S_RCR4_SYWD(0U) | I2S_RCR4_FSE(1U) | I2S_RCR4_FSP(0U) | I2S_RCR4_FRSZ(1U);
        break;
    case kSAI_BusPCMB:
        base->RCR2 &= ~I2S_RCR2_BCP_MASK;
        base->RCR3 &= ~I2S_RCR3_WDFL_MASK;
        base->RCR4 = I2S_RCR4_MF(1U) | I2S_RCR4_SYWD(0U) | I2S_RCR4_FSE(0U) | I2S_RCR4_FSP(0U) | I2S_RCR4_FRSZ(1U);
        break;
    default:
        break;
    }

    /* Set master or slave */
    if (config->masterSlave == kSAI_Master) {
        base->RCR2 |= I2S_RCR2_BCD_MASK;
        base->RCR4 |= I2S_RCR4_FSD_MASK;

        /* Bit clock source setting */
        val = base->RCR2 & (~I2S_RCR2_MSEL_MASK);
        base->RCR2 = (val | I2S_RCR2_MSEL(config->bclkSource));
    } else {
        base->RCR2 &= ~I2S_RCR2_BCD_MASK;
        base->RCR4 &= ~I2S_RCR4_FSD_MASK;
    }

    /* Set Sync mode */
    switch (config->syncMode) {
    case kSAI_ModeAsync:
        val = base->RCR2;
        val &= ~I2S_RCR2_SYNC_MASK;
        base->RCR2 = (val | I2S_RCR2_SYNC(0U));
        break;
    case kSAI_ModeSync:
        val = base->RCR2;
        val &= ~I2S_RCR2_SYNC_MASK;
        base->RCR2 = (val | I2S_RCR2_SYNC(1U));
        /* If sync with Tx, should set Tx to async mode */
        val = base->TCR2;
        val &= ~I2S_TCR2_SYNC_MASK;
        base->TCR2 = (val | I2S_TCR2_SYNC(0U));
        break;
    case kSAI_ModeSyncWithOtherTx:
        val = base->RCR2;
        val &= ~I2S_RCR2_SYNC_MASK;
        base->RCR2 = (val | I2S_RCR2_SYNC(2U));
        break;
    case kSAI_ModeSyncWithOtherRx:
        val = base->RCR2;
        val &= ~I2S_RCR2_SYNC_MASK;
        base->RCR2 = (val | I2S_RCR2_SYNC(3U));
        break;
    default:
        break;
    }
}

/*!
 * @brief  Sets the SAI Tx configuration structure to default values.
 *
 * This API initializes the configuration structure for use in SAI_TxConfig().
 * The initialized structure can remain unchanged in SAI_TxConfig(), or it can be modified
 *  before calling SAI_TxConfig().
 * This is an example.
   @code
   sai_config_t config;
   SAI_TxGetDefaultConfig(&config);
   @endcode
 *
 * @param config pointer to master configuration structure
 */
void i2s_tx_getdefaultconfig(sai_config_t *config)
{
    config->bclkSource = kSAI_BclkSourceMclkDiv;
    config->masterSlave = kSAI_Master;
    config->mclkSource = kSAI_MclkSourceSysclk;
    config->protocol = kSAI_BusLeftJustified;
    config->syncMode = kSAI_ModeAsync;
#if defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)
    config->mclkOutputEnable = true;
#endif /* FSL_FEATURE_SAI_HAS_MCR */
}

/*!
 * @brief  Sets the SAI Rx configuration structure to default values.
 *
 * This API initializes the configuration structure for use in SAI_RxConfig().
 * The initialized structure can remain unchanged in SAI_RxConfig() or it can be modified
 *  before calling SAI_RxConfig().
 * This is an example.
   @code
   sai_config_t config;
   SAI_RxGetDefaultConfig(&config);
   @endcode
 *
 * @param config pointer to master configuration structure
 */
void i2s_rx_getdefaultconfig(sai_config_t *config)
{
    config->bclkSource = kSAI_BclkSourceMclkDiv;
    config->masterSlave = kSAI_Master;
    config->mclkSource = kSAI_MclkSourceSysclk;
    config->protocol = kSAI_BusLeftJustified;
    config->syncMode = kSAI_ModeSync;
#if defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)
    config->mclkOutputEnable = true;
#endif /* FSL_FEATURE_SAI_HAS_MCR */
}

/*!
 * @brief De-initializes the SAI peripheral.
 *
 * This API gates the SAI clock. The SAI module can't operate unless SAI_TxInit
 * or SAI_RxInit is called to enable the clock.
 *
 * @param base SAI base pointer
*/
void i2s_deinit(I2S_Type *base)
{
    i2s_tx_enable(base, false);
    i2s_rx_enable(base, false);
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
	imxrt_clock_disableclock(s_saiClock[i2s_getinstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

/*! @} */

/*!
 * @name Bus Operations
 * @{
 */

/*!
 * @brief Configures the SAI Tx audio format.
 *
 * The audio format can be changed at run-time. This function configures the sample rate and audio data
 * format to be transferred.
 *
 * @param base SAI base pointer.
 * @param format Pointer to the SAI audio data format structure.
 * @param mclkSourceClockHz SAI master clock source frequency in Hz.
 * @param bclkSourceClockHz SAI bit clock source frequency in Hz. If the bit clock source is a master
 * clock, this value should equal the masterClockHz.
*/
void i2s_tx_setformat(I2S_Type *base,
                     sai_transfer_format_t *format,
                     uint32_t mclkSourceClockHz,
                     uint32_t bclkSourceClockHz)
{
    uint32_t bclk = 0;
    uint32_t val = 0;
    uint32_t channels = 2U;

    if (format->stereo != kSAI_Stereo) {
        channels = 1U;
    }

    if (format->isFrameSyncCompact) {
        bclk = format->sampleRate_Hz * format->bitWidth * channels;
        val = (base->TCR4 & (~I2S_TCR4_SYWD_MASK));
        val |= I2S_TCR4_SYWD(format->bitWidth - 1U);
        base->TCR4 = val;
    } else {
        bclk = format->sampleRate_Hz * 32U * 2U;
    }

/* Compute the mclk */
#if defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER)
    /* Check if master clock divider enabled, then set master clock divider */
    if (base->MCR & I2S_MCR_MOE_MASK) {
        i2s_setmasterclockdivider(base, format->masterClockHz, mclkSourceClockHz);
    }
#endif /* FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER */

    /* Set bclk if needed */
    if (base->TCR2 & I2S_TCR2_BCD_MASK) {
        base->TCR2 &= ~I2S_TCR2_DIV_MASK;
        base->TCR2 |= I2S_TCR2_DIV((bclkSourceClockHz / bclk) / 2U - 1U);
    }

    /* Set bitWidth */
    val = (format->isFrameSyncCompact) ? (format->bitWidth - 1) : 31U;
    if (format->protocol == kSAI_BusRightJustified) {
        base->TCR5 = I2S_TCR5_WNW(val) | I2S_TCR5_W0W(val) | I2S_TCR5_FBT(val);
    } else {
        base->TCR5 = I2S_TCR5_WNW(val) | I2S_TCR5_W0W(val) | I2S_TCR5_FBT(format->bitWidth - 1);
    }

    /* Set mono or stereo */
    base->TMR = (uint32_t)format->stereo;

    /* Set data channel */
    base->TCR3 &= ~I2S_TCR3_TCE_MASK;
    base->TCR3 |= I2S_TCR3_TCE(1U << format->channel);

#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    /* Set watermark */
    base->TCR1 = format->watermark;
#endif /* FSL_FEATURE_SAI_FIFO_COUNT  */
}

/*!
 * @brief Configures the SAI Rx audio format.
 *
 * The audio format can be changed at run-time. This function configures the sample rate and audio data
 * format to be transferred.
 *
 * @param base SAI base pointer.
 * @param format Pointer to the SAI audio data format structure.
 * @param mclkSourceClockHz SAI master clock source frequency in Hz.
 * @param bclkSourceClockHz SAI bit clock source frequency in Hz. If the bit clock source is a master
 * clock, this value should equal the masterClockHz.
*/
void i2s_rx_setformat(I2S_Type *base,
                     sai_transfer_format_t *format,
                     uint32_t mclkSourceClockHz,
                     uint32_t bclkSourceClockHz)
{
    uint32_t bclk = 0;
    uint32_t val = 0;
    uint32_t channels = 2U;

    if (format->stereo != kSAI_Stereo) {
        channels = 1U;
    }

    if (format->isFrameSyncCompact) {
        bclk = format->sampleRate_Hz * format->bitWidth * channels;
        val = (base->RCR4 & (~I2S_RCR4_SYWD_MASK));
        val |= I2S_RCR4_SYWD(format->bitWidth - 1U);
        base->RCR4 = val;
    } else {
        bclk = format->sampleRate_Hz * 32U * 2U;
    }

/* Compute the mclk */
#if defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER)
    /* Check if master clock divider enabled */
    if (base->MCR & I2S_MCR_MOE_MASK) {
        i2s_setmasterclockdivider(base, format->masterClockHz, mclkSourceClockHz);
    }
#endif /* FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER */

    /* Set bclk if needed */
    if (base->RCR2 & I2S_RCR2_BCD_MASK) {
        base->RCR2 &= ~I2S_RCR2_DIV_MASK;
        base->RCR2 |= I2S_RCR2_DIV((bclkSourceClockHz / bclk) / 2U - 1U);
    }

    /* Set bitWidth */
    val = (format->isFrameSyncCompact) ? (format->bitWidth - 1) : 31U;
    if (format->protocol == kSAI_BusRightJustified) {
        base->RCR5 = I2S_RCR5_WNW(val) | I2S_RCR5_W0W(val) | I2S_RCR5_FBT(val);
    } else {
        base->RCR5 = I2S_RCR5_WNW(val) | I2S_RCR5_W0W(val) | I2S_RCR5_FBT(format->bitWidth - 1);
    }

    /* Set mono or stereo */
    base->RMR = (uint32_t)format->stereo;

    /* Set data channel */
    base->RCR3 &= ~I2S_RCR3_RCE_MASK;
    base->RCR3 |= I2S_RCR3_RCE(1U << format->channel);

#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    /* Set watermark */
    base->RCR1 = format->watermark;
#endif /* FSL_FEATURE_SAI_FIFO_COUNT  */
}

/*!
 * @brief Sends data using a blocking method.
 *
 * @note This function blocks by polling until data is ready to be sent.
 *
 * @param base SAI base pointer.
 * @param channel Data channel used.
 * @param bitWidth How many bits in an audio word; usually 8/16/24/32 bits.
 * @param buffer Pointer to the data to be written.
 * @param size Bytes to be written.
 */
void i2s_write_blocking(I2S_Type *base, uint32_t channel, uint32_t bitWidth, uint8_t *buffer, uint32_t size)
{
    uint32_t i = 0;
    uint8_t bytesPerWord = bitWidth / 8U;

    while (i < size) {
        /* Wait until it can write data */
        while (!(base->TCSR & I2S_TCSR_FWF_MASK)) {
        }

        i2s_write_nonblocking(base, channel, bitWidth, buffer, bytesPerWord);
        buffer += bytesPerWord;
        i += bytesPerWord;
    }

    /* Wait until the last data is sent */
    while (!(base->TCSR & I2S_TCSR_FWF_MASK)) {
    }
}

/*!
 * @brief Writes data into SAI FIFO.
 *
 * @param base SAI base pointer.
 * @param channel Data channel used.
 * @param data Data needs to be written.
 */
static inline void i2s_writedata(I2S_Type *base, uint32_t channel, uint32_t data)
{
    base->TDR[channel] = data;
}

/*!
 * @brief Receives data using a blocking method.
 *
 * @note This function blocks by polling until data is ready to be sent.
 *
 * @param base SAI base pointer.
 * @param channel Data channel used.
 * @param bitWidth How many bits in an audio word; usually 8/16/24/32 bits.
 * @param buffer Pointer to the data to be read.
 * @param size Bytes to be read.
 */
void i2s_read_blocking(I2S_Type *base, uint32_t channel, uint32_t bitWidth, uint8_t *buffer, uint32_t size)
{
    uint32_t i = 0;
    uint8_t bytesPerWord = bitWidth / 8U;

    while (i < size) {
        /* Wait until data is received */
        while (!(base->RCSR & I2S_RCSR_FWF_MASK)) {
        }

        i2s_read_nonblocking(base, channel, bitWidth, buffer, bytesPerWord);
        buffer += bytesPerWord;
        i += bytesPerWord;
    }
}

/*!
 * @brief Reads data from the SAI FIFO.
 *
 * @param base SAI base pointer.
 * @param channel Data channel used.
 * @return Data in SAI FIFO.
 */
static inline uint32_t i2s_readdata(I2S_Type *base, uint32_t channel)
{
    return base->RDR[channel];
}

/*! @} */

/*!
 * @name Transactional
 * @{
 */
/*!
 * @brief Configures the SAI Tx audio format.
 *
 * The audio format can be changed at run-time. This function configures the sample rate and audio data
 * format to be transferred.
 *
 * @param base SAI base pointer.
 * @param handle SAI handle pointer.
 * @param format Pointer to the SAI audio data format structure.
 * @param mclkSourceClockHz SAI master clock source frequency in Hz.
 * @param bclkSourceClockHz SAI bit clock source frequency in Hz. If a bit clock source is a master
 * clock, this value should equal the masterClockHz in format.
 * @return Status of this function. Return value is the status_t.
*/
status_t i2s_transfer_tx_setformat(I2S_Type *base,
                                 sai_handle_t *handle,
                                 sai_transfer_format_t *format,
                                 uint32_t mclkSourceClockHz,
                                 uint32_t bclkSourceClockHz)
{
    DEBUGASSERT(handle);

    if ((mclkSourceClockHz < format->sampleRate_Hz) || (bclkSourceClockHz < format->sampleRate_Hz)) {
        return kStatus_InvalidArgument;
    }

    /* Copy format to handle */
    handle->bitWidth = format->bitWidth;
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    handle->watermark = format->watermark;
#endif
    handle->channel = format->channel;

    i2s_tx_setformat(base, format, mclkSourceClockHz, bclkSourceClockHz);

    return kStatus_Success;
}

/*!
 * @brief Configures the SAI Rx audio format.
 *
 * The audio format can be changed at run-time. This function configures the sample rate and audio data
 * format to be transferred.
 *
 * @param base SAI base pointer.
 * @param handle SAI handle pointer.
 * @param format Pointer to the SAI audio data format structure.
 * @param mclkSourceClockHz SAI master clock source frequency in Hz.
 * @param bclkSourceClockHz SAI bit clock source frequency in Hz. If a bit clock source is a master
 * clock, this value should equal the masterClockHz in format.
 * @return Status of this function. Return value is one of status_t.
*/
status_t i2s_transfer_rx_setformat(I2S_Type *base,
                                 sai_handle_t *handle,
                                 sai_transfer_format_t *format,
                                 uint32_t mclkSourceClockHz,
                                 uint32_t bclkSourceClockHz)
{
    DEBUGASSERT(handle);

    if ((mclkSourceClockHz < format->sampleRate_Hz) || (bclkSourceClockHz < format->sampleRate_Hz)) {
        return kStatus_InvalidArgument;
    }

    /* Copy format to handle */
    handle->bitWidth = format->bitWidth;
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    handle->watermark = format->watermark;
#endif
    handle->channel = format->channel;

    i2s_rx_setformat(base, format, mclkSourceClockHz, bclkSourceClockHz);

    return kStatus_Success;
}

/*!
 * @brief Performs an interrupt non-blocking send transfer on SAI.
 *
 * @note This API returns immediately after the transfer initiates.
 * Call the SAI_TxGetTransferStatusIRQ to poll the transfer status and check whether
 * the transfer is finished. If the return status is not kStatus_SAI_Busy, the transfer
 * is finished.
 *
 * @param base SAI base pointer.
 * @param handle Pointer to the sai_handle_t structure which stores the transfer state.
 * @param xfer Pointer to the sai_transfer_t structure.
 * @retval kStatus_Success Successfully started the data receive.
 * @retval kStatus_SAI_TxBusy Previous receive still not finished.
 * @retval kStatus_InvalidArgument The input parameter is invalid.
 */
status_t i2s_transfer_send_nonblocking(I2S_Type *base, sai_handle_t *handle, sai_transfer_t *xfer)
{
    DEBUGASSERT(handle);

    /* Check if the queue is full */
    if (handle->saiQueue[handle->queueUser].data) {
        return kStatus_SAI_QueueFull;
    }

    /* Add into queue */
    handle->transferSize[handle->queueUser] = xfer->dataSize;
    handle->saiQueue[handle->queueUser].data = xfer->data;
    handle->saiQueue[handle->queueUser].dataSize = xfer->dataSize;
    handle->queueUser = (handle->queueUser + 1) % SAI_XFER_QUEUE_SIZE;

    /* Set the state to busy */
    handle->state = kSAI_Busy;

/* Enable interrupt */
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    /* Use FIFO request interrupt and fifo error*/
    i2s_tx_enableinterrupts(base, kSAI_FIFOErrorInterruptEnable | kSAI_FIFORequestInterruptEnable);
#else
    i2s_tx_enableinterrupts(base, kSAI_FIFOErrorInterruptEnable | kSAI_FIFOWarningInterruptEnable);
#endif /* FSL_FEATURE_SAI_FIFO_COUNT */

    /* Enable Tx transfer */
    i2s_tx_enable(base, true);

    return kStatus_Success;
}

/*!
 * @brief Performs an interrupt non-blocking receive transfer on SAI.
 *
 * @note This API returns immediately after the transfer initiates.
 * Call the SAI_RxGetTransferStatusIRQ to poll the transfer status and check whether
 * the transfer is finished. If the return status is not kStatus_SAI_Busy, the transfer
 * is finished.
 *
 * @param base SAI base pointer
 * @param handle Pointer to the sai_handle_t structure which stores the transfer state.
 * @param xfer Pointer to the sai_transfer_t structure.
 * @retval kStatus_Success Successfully started the data receive.
 * @retval kStatus_SAI_RxBusy Previous receive still not finished.
 * @retval kStatus_InvalidArgument The input parameter is invalid.
 */
status_t i2s_transfer_receive_nonblocking(I2S_Type *base, sai_handle_t *handle, sai_transfer_t *xfer)
{
    DEBUGASSERT(handle);

    /* Check if the queue is full */
    if (handle->saiQueue[handle->queueUser].data) {
        return kStatus_SAI_QueueFull;
    }

    /* Add into queue */
    handle->transferSize[handle->queueUser] = xfer->dataSize;
    handle->saiQueue[handle->queueUser].data = xfer->data;
    handle->saiQueue[handle->queueUser].dataSize = xfer->dataSize;
    handle->queueUser = (handle->queueUser + 1) % SAI_XFER_QUEUE_SIZE;

    /* Set state to busy */
    handle->state = kSAI_Busy;

/* Enable interrupt */
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    /* Use FIFO request interrupt and fifo error*/
    i2s_rx_enableinterrupts(base, kSAI_FIFOErrorInterruptEnable | kSAI_FIFORequestInterruptEnable);
#else
    i2s_rx_enableinterrupts(base, kSAI_FIFOErrorInterruptEnable | kSAI_FIFOWarningInterruptEnable);
#endif /* FSL_FEATURE_SAI_FIFO_COUNT */

    /* Enable Rx transfer */
    i2s_rx_enable(base, true);

    return kStatus_Success;
}

/*!
 * @brief Gets a set byte count.
 *
 * @param base SAI base pointer.
 * @param handle Pointer to the sai_handle_t structure which stores the transfer state.
 * @param count Bytes count sent.
 * @retval kStatus_Success Succeed get the transfer count.
 * @retval kStatus_NoTransferInProgress There is not a non-blocking transaction currently in progress.
 */
status_t i2s_transfer_getsendcount(I2S_Type *base, sai_handle_t *handle, size_t *count)
{
    DEBUGASSERT(handle);

    status_t status = kStatus_Success;

    if (handle->state != kSAI_Busy) {
        status = kStatus_NoTransferInProgress;
    } else {
        *count = (handle->transferSize[handle->queueDriver] - handle->saiQueue[handle->queueDriver].dataSize);
    }

    return status;
}

/*!
 * @brief Gets a received byte count.
 *
 * @param base SAI base pointer.
 * @param handle Pointer to the sai_handle_t structure which stores the transfer state.
 * @param count Bytes count received.
 * @retval kStatus_Success Succeed get the transfer count.
 * @retval kStatus_NoTransferInProgress There is not a non-blocking transaction currently in progress.
 */
status_t i2s_transfer_getreceivecount(I2S_Type *base, sai_handle_t *handle, size_t *count)
{
    DEBUGASSERT(handle);

    status_t status = kStatus_Success;

    if (handle->state != kSAI_Busy) {
        status = kStatus_NoTransferInProgress;
    } else {
        *count = (handle->transferSize[handle->queueDriver] - handle->saiQueue[handle->queueDriver].dataSize);
    }

    return status;
}

/*!
 * @brief Aborts the current send.
 *
 * @note This API can be called any time when an interrupt non-blocking transfer initiates
 * to abort the transfer early.
 *
 * @param base SAI base pointer.
 * @param handle Pointer to the sai_handle_t structure which stores the transfer state.
 */
void i2s_transfer_abortsend(I2S_Type *base, sai_handle_t *handle)
{
    DEBUGASSERT(handle);

    /* Stop Tx transfer and disable interrupt */
    i2s_tx_enable(base, false);
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    /* Use FIFO request interrupt and fifo error */
    i2s_tx_disableinterrupts(base, kSAI_FIFOErrorInterruptEnable | kSAI_FIFORequestInterruptEnable);
#else
    i2s_tx_disableinterrupts(base, kSAI_FIFOErrorInterruptEnable | kSAI_FIFOWarningInterruptEnable);
#endif /* FSL_FEATURE_SAI_FIFO_COUNT */

    handle->state = kSAI_Idle;

    /* Clear the queue */
    memset(handle->saiQueue, 0, sizeof(sai_transfer_t) * SAI_XFER_QUEUE_SIZE);
    handle->queueDriver = 0;
    handle->queueUser = 0;
}

/*!
 * @brief Aborts the the current IRQ receive.
 *
 * @note This API can be called when an interrupt non-blocking transfer initiates
 * to abort the transfer early.
 *
 * @param base SAI base pointer
 * @param handle Pointer to the sai_handle_t structure which stores the transfer state.
 */
void i2s_transfer_abortreceive(I2S_Type *base, sai_handle_t *handle)
{
    DEBUGASSERT(handle);

    /* Stop Tx transfer and disable interrupt */
    i2s_rx_enable(base, false);
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    /* Use FIFO request interrupt and fifo error */
    i2s_rx_disableinterrupts(base, kSAI_FIFOErrorInterruptEnable | kSAI_FIFORequestInterruptEnable);
#else
    i2s_rx_disableinterrupts(base, kSAI_FIFOErrorInterruptEnable | kSAI_FIFOWarningInterruptEnable);
#endif /* FSL_FEATURE_SAI_FIFO_COUNT */

    handle->state = kSAI_Idle;

    /* Clear the queue */
    memset(handle->saiQueue, 0, sizeof(sai_transfer_t) * SAI_XFER_QUEUE_SIZE);
    handle->queueDriver = 0;
    handle->queueUser = 0;
}

/*!
 * @brief Terminate all SAI send.
 *
 * This function will clear all transfer slots buffered in the sai queue. If users only want to abort the
 * current transfer slot, please call SAI_TransferAbortSend.
 *
 * @param base SAI base pointer.
 * @param handle SAI eDMA handle pointer.
 */
void i2s_transferterminatesend(I2S_Type *base, sai_handle_t *handle)
{
    DEBUGASSERT(handle);

    /* Abort the current transfer */
    i2s_transfer_abortsend(base, handle);

    /* Clear all the internal information */
    memset(handle->saiQueue, 0U, sizeof(handle->saiQueue));
    memset(handle->transferSize, 0U, sizeof(handle->transferSize));
    handle->queueUser = 0U;
    handle->queueDriver = 0U;
}

/*!
 * @brief Terminate all SAI receive.
 *
 * This function will clear all transfer slots buffered in the sai queue. If users only want to abort the
 * current transfer slot, please call SAI_TransferAbortReceive.
 *
 * @param base SAI base pointer.
 * @param handle SAI eDMA handle pointer.
 */
void i2s_transferterminatereceive(I2S_Type *base, sai_handle_t *handle)
{
    DEBUGASSERT(handle);

    /* Abort the current transfer */
    i2s_transfer_abortreceive(base, handle);

    /* Clear all the internal information */
    memset(handle->saiQueue, 0U, sizeof(handle->saiQueue));
    memset(handle->transferSize, 0U, sizeof(handle->transferSize));
    handle->queueUser = 0U;
    handle->queueDriver = 0U;
}

/*!
 * @brief Tx interrupt handler.
 *
 * @param base SAI base pointer.
 * @param handle Pointer to the sai_handle_t structure.
 */
void i2s_transfer_tx_handleirq(I2S_Type *base, sai_handle_t *handle)
{
    DEBUGASSERT(handle);

    uint8_t *buffer = handle->saiQueue[handle->queueDriver].data;
    uint8_t dataSize = handle->bitWidth / 8U;

    /* Handle Error */
    if (base->TCSR & I2S_TCSR_FEF_MASK) {
        /* Clear FIFO error flag to continue transfer */
        i2s_tx_clearstatusflag(base, kSAI_FIFOErrorFlag);

        /* Reset FIFO for safety */
        i2s_tx_softwarereset(base, kSAI_ResetTypeFIFO);

        /* Call the callback */
        if (handle->callbackParam[handle->queueDriver].callback) {
            (handle->callbackParam[handle->queueDriver].callback)((FAR struct i2s_dev_s *)handle->callbackParam[handle->queueDriver].dev_ptr, handle->callbackParam[handle->queueDriver].apb, handle->callbackParam[handle->queueDriver].arg, -1);
        }
    }

/* Handle transfer */
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    if (base->TCSR & I2S_TCSR_FRF_MASK) {
        /* Judge if the data need to transmit is less than space */
        uint8_t size = MIN((handle->saiQueue[handle->queueDriver].dataSize),
                           (size_t)((FSL_FEATURE_SAI_FIFO_COUNT - handle->watermark) * dataSize));

        /* Copy the data from sai buffer to FIFO */
        i2s_write_nonblocking(base, handle->channel, handle->bitWidth, buffer, size);

        /* Update the internal counter */
        handle->saiQueue[handle->queueDriver].dataSize -= size;
        handle->saiQueue[handle->queueDriver].data += size;
    }
#else
    if (base->TCSR & I2S_TCSR_FWF_MASK) {
        uint8_t size = MIN((handle->saiQueue[handle->queueDriver].dataSize), dataSize);

        i2s_write_nonblocking(base, handle->channel, handle->bitWidth, buffer, size);

        /* Update internal counter */
        handle->saiQueue[handle->queueDriver].dataSize -= size;
        handle->saiQueue[handle->queueDriver].data += size;
    }
#endif /* FSL_FEATURE_SAI_FIFO_COUNT */

    /* If finished a blcok, call the callback function */
    if (handle->saiQueue[handle->queueDriver].dataSize == 0U) {
        memset(&handle->saiQueue[handle->queueDriver], 0, sizeof(sai_transfer_t));
        handle->queueDriver = (handle->queueDriver + 1) % SAI_XFER_QUEUE_SIZE;

		/* Call the callback */
        if (handle->callbackParam[handle->queueDriver].callback) {
            (handle->callbackParam[handle->queueDriver].callback)((FAR struct i2s_dev_s *)handle->callbackParam[handle->queueDriver].dev_ptr, handle->callbackParam[handle->queueDriver].apb, handle->callbackParam[handle->queueDriver].arg, OK);
        }
    }

    /* If all data finished, just stop the transfer */
    if (handle->saiQueue[handle->queueDriver].data == NULL) {
        i2s_transfer_abortsend(base, handle);
    }
}

/*!
 * @brief Tx interrupt handler.
 *
 * @param base SAI base pointer.
 * @param handle Pointer to the sai_handle_t structure.
 */
void i2s_transfer_rx_handleirq(I2S_Type *base, sai_handle_t *handle)
{
    DEBUGASSERT(handle);

    uint8_t *buffer = handle->saiQueue[handle->queueDriver].data;
    uint8_t dataSize = handle->bitWidth / 8U;

    /* Handle Error */
    if (base->RCSR & I2S_RCSR_FEF_MASK) {
        /* Clear FIFO error flag to continue transfer */
        i2s_rx_clearstatusflags(base, kSAI_FIFOErrorFlag);

        /* Reset FIFO for safety */
        i2s_rx_softwarereset(base, kSAI_ResetTypeFIFO);

        /* Call the callback */
        if (handle->callbackParam[handle->queueDriver].callback) {
            (handle->callbackParam[handle->queueDriver].callback)((FAR struct i2s_dev_s *)handle->callbackParam[handle->queueDriver].dev_ptr, handle->callbackParam[handle->queueDriver].apb, handle->callbackParam[handle->queueDriver].arg, -1);
        }
    }

/* Handle transfer */
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    if (base->RCSR & I2S_RCSR_FRF_MASK) {
        /* Judge if the data need to transmit is less than space */
        uint8_t size = MIN((handle->saiQueue[handle->queueDriver].dataSize), (handle->watermark * dataSize));

        /* Copy the data from sai buffer to FIFO */
        i2s_read_nonblocking(base, handle->channel, handle->bitWidth, buffer, size);

        /* Update the internal counter */
        handle->saiQueue[handle->queueDriver].dataSize -= size;
        handle->saiQueue[handle->queueDriver].data += size;
    }
#else
    if (base->RCSR & I2S_RCSR_FWF_MASK) {
        uint8_t size = MIN((handle->saiQueue[handle->queueDriver].dataSize), dataSize);

        i2s_read_nonblocking(base, handle->channel, handle->bitWidth, buffer, size);

        /* Update internal state */
        handle->saiQueue[handle->queueDriver].dataSize -= size;
        handle->saiQueue[handle->queueDriver].data += size;
    }
#endif /* FSL_FEATURE_SAI_FIFO_COUNT */

    /* If finished a blcok, call the callback function */
    if (handle->saiQueue[handle->queueDriver].dataSize == 0U) {
        memset(&handle->saiQueue[handle->queueDriver], 0, sizeof(sai_transfer_t));
        handle->queueDriver = (handle->queueDriver + 1) % SAI_XFER_QUEUE_SIZE;

		/* Call the callback */
        if (handle->callbackParam[handle->queueDriver].callback) {
            (handle->callbackParam[handle->queueDriver].callback)((FAR struct i2s_dev_s *)handle->callbackParam[handle->queueDriver].dev_ptr, handle->callbackParam[handle->queueDriver].apb, handle->callbackParam[handle->queueDriver].arg, OK);
        }
    }

    /* If all data finished, just stop the transfer */
    if (handle->saiQueue[handle->queueDriver].data == NULL) {
        i2s_transfer_abortreceive(base, handle);
    }
}

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

static void i2s_exclsem_take(struct imxrt_i2s_s *priv)
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
	struct imxrt_i2s_s *priv = (struct imxrt_i2s_s *)dev;
	int ret;

	/* Support 16/32bits only */
	DEBUGASSERT(priv && (bits == I2S_BITS_PER_SAMPLE_16BIT || bits == I2S_BITS_PER_SAMPLE_32BIT));

	if (bits == I2S_BITS_PER_SAMPLE_16BIT || bits == I2S_BITS_PER_SAMPLE_32BIT) {
		priv->bits_per_sample = bits;
		ret = i2s_set_clk(priv, priv->sample_rate, priv->bits_per_sample, priv->channel_num);
		if (ret == OK) {
			return priv->bits_per_sample * priv->sample_rate;
		}
	}
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
	struct imxrt_i2s_s *priv = (struct imxrt_i2s_s *)dev;

	i2sinfo("[I2S RX] apb=%p nmaxbytes=%d samp=%p arg=%p timeout=%d\n", apb, apb->nmaxbytes, apb->samp, arg, timeout);

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	/* Has the RX channel been enabled? */
	if (!priv->rxenab) {
		i2serr("ERROR: I2S has no receiver\n");
		return -EAGAIN;
	}

	/* Check if the queue is full */
    if (priv->si_rx_handle.saiQueue[priv->si_rx_handle.queueUser].data) {
        return -EAGAIN;
    }

	/* Get exclusive access to the I2S driver data */
	i2s_exclsem_take(priv);
	i2sinfo("RX Exclusive Enter\n");

	priv->si_rx_handle.transferSize[priv->si_rx_handle.queueUser] = apb->nmaxbytes;
    priv->si_rx_handle.saiQueue[priv->si_rx_handle.queueUser].data = apb->samp;;
    priv->si_rx_handle.saiQueue[priv->si_rx_handle.queueUser].dataSize = apb->nmaxbytes;
	priv->si_rx_handle.callbackParam[priv->si_rx_handle.queueUser].dev_ptr = (void *)dev;
	priv->si_rx_handle.callbackParam[priv->si_rx_handle.queueUser].apb = apb;
	priv->si_rx_handle.callbackParam[priv->si_rx_handle.queueUser].arg = arg;
	priv->si_rx_handle.callbackParam[priv->si_rx_handle.queueUser].callback = callback;
	priv->si_rx_handle.callbackParam[priv->si_rx_handle.queueUser].timeout = timeout;
	priv->si_rx_handle.queueUser = (priv->si_rx_handle.queueUser + 1) % SAI_XFER_QUEUE_SIZE;

	/* Set state to busy */
    priv->si_rx_handle.state = kSAI_Busy;

	/* Enable interrupt */
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    /* Use FIFO request interrupt and fifo error*/
    i2s_rx_enableinterrupts(priv->i2s_base, kSAI_FIFOErrorInterruptEnable | kSAI_FIFORequestInterruptEnable);
#else
    i2s_rx_enableinterrupts(priv->i2s_base, kSAI_FIFOErrorInterruptEnable | kSAI_FIFOWarningInterruptEnable);
#endif /* FSL_FEATURE_SAI_FIFO_COUNT */

	/* Enable Rx transfer */
	i2s_rx_enable(priv->i2s_base, true);

	/* Exit this function */
	i2s_exclsem_give(priv);
	i2sinfo("RX Exclusive Exit\n");

	return OK;

#else
	i2serr("ERROR: I2S has no receiver\n");
	UNUSED(priv);
	return -ENOSYS;
#endif

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
#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	struct imxrt_i2s_s *priv = (struct imxrt_i2s_s *)dev;
	int ret;
	/* Support 16/32bits only */
	DEBUGASSERT(priv && (bits == I2S_BITS_PER_SAMPLE_16BIT || bits == I2S_BITS_PER_SAMPLE_32BIT));

	if (bits == I2S_BITS_PER_SAMPLE_16BIT || bits == I2S_BITS_PER_SAMPLE_32BIT) {
		priv->bits_per_sample = bits;

		if (bits == I2S_BITS_PER_SAMPLE_16BIT || bits == I2S_BITS_PER_SAMPLE_32BIT) {
			priv->bits_per_sample = bits;
			ret = i2s_set_clk(priv, priv->sample_rate, priv->bits_per_sample, priv->channel_num);
			if (ret == OK) {
				return priv->bits_per_sample * priv->sample_rate;
			}
		}
	}
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
	struct imxrt_i2s_s *priv = (struct imxrt_i2s_s *)dev;

	DEBUGASSERT(priv && apb);

	i2sinfo("[I2S TX] apb=%p nbytes=%d arg=%p samp=%p timeout=%d\n", apb, apb->nbytes - apb->curbyte, apb->samp, arg, timeout);

	i2s_dump_buffer("Sending", &apb->samp[apb->curbyte], apb->nbytes - apb->curbyte);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)

	/* Has the TX channel been enabled? */
	if (!priv->txenab) {
		i2serr("ERROR: I2S has no transmitter\n");
		return -EAGAIN;
	}

	/* Check if the queue is full */
    if (priv->si_tx_handle.saiQueue[priv->si_tx_handle.queueUser].data) {
        return kStatus_SAI_QueueFull;
    }

	/* Get exclusive access to the I2S driver data */
	i2s_exclsem_take(priv);
	i2sinfo("TX Exclusive Enter\n");

	/* Add into queue */
    priv->si_tx_handle.transferSize[priv->si_tx_handle.queueUser] = apb->nbytes - apb->curbyte;
    priv->si_tx_handle.saiQueue[priv->si_tx_handle.queueUser].data = &apb->samp[apb->curbyte];
    priv->si_tx_handle.saiQueue[priv->si_tx_handle.queueUser].dataSize = apb->nbytes - apb->curbyte;
	priv->si_tx_handle.callbackParam[priv->si_tx_handle.queueUser].dev_ptr = (void *)dev;
	priv->si_tx_handle.callbackParam[priv->si_tx_handle.queueUser].apb = apb;
	priv->si_tx_handle.callbackParam[priv->si_tx_handle.queueUser].arg = arg;
	priv->si_tx_handle.callbackParam[priv->si_tx_handle.queueUser].callback = callback;
	priv->si_tx_handle.callbackParam[priv->si_tx_handle.queueUser].timeout = timeout;
	priv->si_tx_handle.queueUser = (priv->si_tx_handle.queueUser + 1) % SAI_XFER_QUEUE_SIZE;

    /* Set the state to busy */
    priv->si_tx_handle.state = kSAI_Busy;

	/* Enable interrupt */
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    /* Use FIFO request interrupt and fifo error*/
	i2s_tx_enableinterrupts(priv->i2s_base, kSAI_FIFOErrorInterruptEnable | kSAI_FIFORequestInterruptEnable);
#else
    i2s_tx_enableinterrupts(priv->i2s_base, kSAI_FIFOErrorInterruptEnable | kSAI_FIFOWarningInterruptEnable);
#endif /* FSL_FEATURE_SAI_FIFO_COUNT */

    /* Enable Tx transfer */
	i2s_tx_enable(priv->i2s_base, true);

	i2s_exclsem_give(priv);
	i2sinfo("TX Exclusive Exit\n");

	return OK;
#else
	i2serr("ERROR: I2S has no transmitter\n");
	UNUSED(priv);
	return -ENOSYS;
#endif
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
	struct imxrt_i2s_s *priv = (struct imxrt_i2s_s *)dev;
	DEBUGASSERT(priv);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (dir == I2S_TX && priv->txenab) {
		i2s_tx_enable(priv->i2s_base, false);
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX && priv->rxenab) {
		i2s_rx_enable(priv->i2s_base, false);
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
	struct imxrt_i2s_s *priv = (struct imxrt_i2s_s *)dev;
	DEBUGASSERT(priv);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (dir == I2S_TX && priv->txenab) {
		i2s_tx_enable(priv->i2s_base, true);
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX && priv->rxenab) {
		i2s_rx_enable(priv->i2s_base, true);
	}
#endif

	return 0;
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
	struct imxrt_i2s_s *priv = (struct imxrt_i2s_s *)dev;
	DEBUGASSERT(priv);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (dir == I2S_TX) {
		i2s_transfer_abortsend(priv->i2s_base, &priv->si_tx_handle);
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX) {
		i2s_transfer_abortreceive(priv->i2s_base, &priv->si_rx_handle);
	}
#endif

	return 0;
}

static int i2s_stop(struct i2s_dev_s *dev, i2s_ch_dir_t dir)
{
	struct imxrt_i2s_s *priv = (struct imxrt_i2s_s *)dev;
	DEBUGASSERT(priv);

	i2s_exclsem_take(priv);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (dir == I2S_TX) {
		i2s_transferterminatesend(priv->i2s_base, &priv->si_tx_handle);
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX) {
		i2s_transferterminatereceive(priv->i2s_base, &priv->si_rx_handle);
	}
#endif

	i2s_exclsem_give(priv);

	return 0;
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
	struct imxrt_i2s_s *priv = (struct imxrt_i2s_s *)dev;
	DEBUGASSERT(priv);

	priv->err_cb = cb;
	priv->err_cb_arg = arg;

	return 0;
}

/****************************************************************************
* Name: i2s_set_clk
*
* Description:
*   Set the I2S clock.
*
* Input Parameters:
*   dev  - Device-specific state data
*   rate - The I2S sample rate in samples (not bits) per second
*   bits - bits in per sample
*   ch   - channel count
* Returned Value:
*   Returns the resulting bitrate
*
****************************************************************************/					
int i2s_set_clk(struct imxrt_i2s_s *priv, uint32_t rate, i2s_bits_per_sample_t bits, i2s_channel_t ch)
{
	sai_transfer_format_t format;
	uint32_t mclkSourceClockHz = 0U;

	if (bits > I2S_BITS_PER_SAMPLE_32BIT || bits < I2S_BITS_PER_SAMPLE_16BIT || (bits % 8) != 0) {
		i2sinfo("[I2S] Invalid bits per sample\n");
		return ERROR;
	}

	i2sinfo("[I2S] enter set clk:%d, %d, %d...\n", rate, bits, ch);
	if (NULL == priv) {
		i2sinfo("[I2S] g_i2sdevice Not initialized yet\n");
		return ERROR;
	}
	priv->sample_rate = rate;

	/* stop all transfer before configure I2S */
	i2s_stop((struct i2s_dev_s *)priv, I2S_RX);
	i2s_stop((struct i2s_dev_s *)priv, I2S_TX);

	// wait all on-going writing finish
	i2s_exclsem_take(priv);

	if (priv->channel_num != ch) {
		priv->channel_num = (ch == 2) ? 2 : 1;

		i2sinfo("[I2S] channel_num:%d tx_chan_mode:%d, tx_fifi_mode: %d; rx_chan_mode:%d, rx_fifi_mode: %d; \n", priv->channel_num, I2S[i2s_num]->conf_chan.tx_chan_mod, I2S[i2s_num]->fifo_conf.tx_fifo_mod, I2S[i2s_num]->conf_chan.rx_chan_mod, I2S[i2s_num]->fifo_conf.rx_fifo_mod);
	}

	if (bits != priv->bits_per_sample) {

		priv->bits_per_sample = bits;

		i2sinfo("[I2S] bits_per_sample:%d tx_chan_mode:%d, tx_fifi_mode: %d; rx_chan_mode:%d, rx_fifi_mode: %d; \n", priv->bits_per_sample, I2S[i2s_num]->conf_chan.tx_chan_mod, I2S[i2s_num]->fifo_conf.tx_fifo_mod, I2S[i2s_num]->conf_chan.rx_chan_mod, I2S[i2s_num]->fifo_conf.rx_fifo_mod);
	}

	/* Configure the audio format */
    format.bitWidth = priv->bits_per_sample;
    format.channel = priv->channel_num;
    format.sampleRate_Hz = priv->sample_rate;
    format.masterClockHz = OVER_SAMPLE_RATE * format.sampleRate_Hz;
    format.protocol = kSAI_BusI2S;
    format.stereo = kSAI_Stereo;
    format.isFrameSyncCompact = false;
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    format.watermark = FSL_FEATURE_SAI_FIFO_COUNT / 2U;
#endif

	/* Copy format to handle */
    priv->si_tx_handle.bitWidth = format.bitWidth;
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    priv->si_tx_handle.watermark = format.watermark;
#endif
    priv->si_tx_handle.channel = format.channel;
	mclkSourceClockHz = OVER_SAMPLE_RATE * format.sampleRate_Hz;
	i2s_tx_setformat(priv->i2s_base, &format, mclkSourceClockHz, format.masterClockHz);

    /* Copy format to handle */
    priv->si_rx_handle.bitWidth = format.bitWidth;
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    priv->si_rx_handle.watermark = format.watermark;
#endif
    priv->si_rx_handle.channel = format.channel;
	mclkSourceClockHz = OVER_SAMPLE_RATE * format.sampleRate_Hz;
	i2s_rx_setformat(priv->i2s_base, &format, mclkSourceClockHz, format.masterClockHz);


	// wait all writing on-going finish
	i2s_exclsem_give(priv);

	return OK;
}

/****************************************************************************
 * Name: esp32_i2s_isr_initialize
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
static int imxrt_i2s_isr_initialize(struct imxrt_i2s_s *priv)
{
	/* Attach the GPIO peripheral to the allocated CPU interrupt */
	up_disable_irq(priv->tx_isr_num);

	/* Attach and enable the IRQ */
	int ret = irq_attach(priv->tx_isr_num, priv->isr_handler, priv);
	if (ret == OK) {
		/* Enable the CPU interrupt */
		up_enable_irq(priv->tx_isr_num);
	}

	up_disable_irq(priv->rx_isr_num);

	/* Attach and enable the IRQ */
	ret = irq_attach(priv->rx_isr_num, priv->isr_handler, priv);
	if (ret == OK) {
		/* Enable the CPU interrupt */
		up_enable_irq(priv->rx_isr_num);
	}
	return ret;
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
	struct imxrt_i2s_s *priv = (struct imxrt_i2s_s *)dev;
	DEBUGASSERT(priv && rate > 0);
	return i2s_set_clk(priv, rate, priv->bits_per_sample, priv->channel_num);
}

/****************************************************************************
* Name: i2s_irq_handler
*
* Description:
*   ISR function for I2S.
*
* Input Parameters:
*   irq  - irq number
*   context - context
*   arg - Device-specific state data
*
* Returned Value:
*
*
****************************************************************************/

static int i2s_irq_handler(int irq, FAR void *context, FAR void *arg)
{
	struct imxrt_i2s_s *priv = (struct imxrt_i2s_s *)arg;

	if (priv->txenab) {
#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
		i2s_transfer_tx_handleirq(priv->i2s_base, &priv->si_tx_handle);
#endif
	}

	if (priv->rxenab) {
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
		i2s_transfer_rx_handleirq(priv->i2s_base, &priv->si_rx_handle);
#endif
	}

	return OK;
}

void imxrt_i2s_initpins(void)
{
    imxrt_iomuxc_setpinmux(
      IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL,        /* GPIO_AD_B1_00 is configured as LPI2C1_SCL */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_AD_B1_00 */
    imxrt_iomuxc_setpinmux(
        IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA,        /* GPIO_AD_B1_01 is configured as LPI2C1_SDA */
        1U);                                    /* Software Input On Field: Force input path of pad GPIO_AD_B1_01 */
    imxrt_iomuxc_setpinmux(
        IOMUXC_GPIO_AD_B1_09_SAI1_MCLK,         /* GPIO_AD_B1_09 is configured as SAI1_MCLK */
        1U);                                    /* Software Input On Field: Force input path of pad GPIO_AD_B1_09 */
    imxrt_iomuxc_setpinmux(
        IOMUXC_GPIO_AD_B1_12_SAI1_RX_DATA00,    /* GPIO_AD_B1_12 is configured as SAI1_RX_DATA00 */
        1U);                                    /* Software Input On Field: Force input path of pad GPIO_AD_B1_12 */
    imxrt_iomuxc_setpinmux(
        IOMUXC_GPIO_AD_B1_13_SAI1_TX_DATA00,    /* GPIO_AD_B1_13 is configured as SAI1_TX_DATA00 */
        1U);                                    /* Software Input On Field: Force input path of pad GPIO_AD_B1_13 */
    imxrt_iomuxc_setpinmux(
        IOMUXC_GPIO_AD_B1_14_SAI1_TX_BCLK,      /* GPIO_AD_B1_14 is configured as SAI1_TX_BCLK */
        1U);                                    /* Software Input On Field: Force input path of pad GPIO_AD_B1_14 */
    imxrt_iomuxc_setpinmux(
        IOMUXC_GPIO_AD_B1_15_SAI1_TX_SYNC,      /* GPIO_AD_B1_15 is configured as SAI1_TX_SYNC */
        1U);                                    /* Software Input On Field: Force input path of pad GPIO_AD_B1_15 */
    imxrt_iomuxc_setpinconfig(
        IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL,        /* GPIO_AD_B1_00 PAD functional properties : */
        0xD8B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                    Drive Strength Field: R0/6
                                                    Speed Field: medium(100MHz)
                                                    Open Drain Enable Field: Open Drain Enabled
                                                    Pull / Keep Enable Field: Pull/Keeper Enabled
                                                    Pull / Keep Select Field: Keeper
                                                    Pull Up / Down Config. Field: 22K Ohm Pull Up
                                                    Hyst. Enable Field: Hysteresis Disabled */
    imxrt_iomuxc_setpinconfig(
        IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA,        /* GPIO_AD_B1_01 PAD functional properties : */
        0xD8B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                    Drive Strength Field: R0/6
                                                    Speed Field: medium(100MHz)
                                                    Open Drain Enable Field: Open Drain Enabled
                                                    Pull / Keep Enable Field: Pull/Keeper Enabled
                                                    Pull / Keep Select Field: Keeper
                                                    Pull Up / Down Config. Field: 22K Ohm Pull Up
                                                    Hyst. Enable Field: Hysteresis Disabled */
    imxrt_iomuxc_setpinconfig(
        IOMUXC_GPIO_AD_B1_09_SAI1_MCLK,         /* GPIO_AD_B1_09 PAD functional properties : */
        0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                    Drive Strength Field: R0/6
                                                    Speed Field: medium(100MHz)
                                                    Open Drain Enable Field: Open Drain Disabled
                                                    Pull / Keep Enable Field: Pull/Keeper Enabled
                                                    Pull / Keep Select Field: Keeper
                                                    Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                    Hyst. Enable Field: Hysteresis Disabled */
    imxrt_iomuxc_setpinconfig(
        IOMUXC_GPIO_AD_B1_12_SAI1_RX_DATA00,    /* GPIO_AD_B1_12 PAD functional properties : */
        0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                    Drive Strength Field: R0/6
                                                    Speed Field: medium(100MHz)
                                                    Open Drain Enable Field: Open Drain Disabled
                                                    Pull / Keep Enable Field: Pull/Keeper Enabled
                                                    Pull / Keep Select Field: Keeper
                                                    Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                    Hyst. Enable Field: Hysteresis Disabled */
    imxrt_iomuxc_setpinconfig(
        IOMUXC_GPIO_AD_B1_13_SAI1_TX_DATA00,    /* GPIO_AD_B1_13 PAD functional properties : */
        0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                    Drive Strength Field: R0/6
                                                    Speed Field: medium(100MHz)
                                                    Open Drain Enable Field: Open Drain Disabled
                                                    Pull / Keep Enable Field: Pull/Keeper Enabled
                                                    Pull / Keep Select Field: Keeper
                                                    Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                    Hyst. Enable Field: Hysteresis Disabled */
    imxrt_iomuxc_setpinconfig(
        IOMUXC_GPIO_AD_B1_14_SAI1_TX_BCLK,      /* GPIO_AD_B1_14 PAD functional properties : */
        0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                    Drive Strength Field: R0/6
                                                    Speed Field: medium(100MHz)
                                                    Open Drain Enable Field: Open Drain Disabled
                                                    Pull / Keep Enable Field: Pull/Keeper Enabled
                                                    Pull / Keep Select Field: Keeper
                                                    Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                    Hyst. Enable Field: Hysteresis Disabled */
    imxrt_iomuxc_setpinconfig(
        IOMUXC_GPIO_AD_B1_15_SAI1_TX_SYNC,      /* GPIO_AD_B1_15 PAD functional properties : */
        0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                    Drive Strength Field: R0/6
                                                    Speed Field: medium(100MHz)
                                                    Open Drain Enable Field: Open Drain Disabled
                                                    Pull / Keep Enable Field: Pull/Keeper Enabled
                                                    Pull / Keep Select Field: Keeper
                                                    Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                    Hyst. Enable Field: Hysteresis Disabled */
}

void imxrt_i2s_clock_init(void)
{
    imxrt_clock_initaudiopll(&audioPllConfig);
    /*Clock setting for LPI2C*/
    imxrt_clock_setmux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
    imxrt_clock_setdiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);

    /*Clock setting for SAI1*/
    imxrt_clock_setmux(kCLOCK_Sai1Mux, DEMO_SAI1_CLOCK_SOURCE_SELECT);
    imxrt_clock_setdiv(kCLOCK_Sai1PreDiv, DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER);
    imxrt_clock_setdiv(kCLOCK_Sai1Div, DEMO_SAI1_CLOCK_SOURCE_DIVIDER);

    IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK;
}

#ifdef IMXRT_AUDIO_CODEC
void BOARD_Codec_I2C_Init(void)
{
    BOARD_LPI2C_Init(BOARD_CODEC_I2C_BASEADDR, BOARD_CODEC_I2C_CLOCK_FREQ);
}

status_t BOARD_Codec_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_CODEC_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize);
}

status_t BOARD_Codec_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_CODEC_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff, rxBuffSize);
}
#endif

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
struct i2s_dev_s *imxrt_i2s_initialize(uint16_t port)
{
	if (port >= I2S_NUM_MAX) {
		i2serr("ERROR: Port number outside the allowed port number range\n");
		return NULL;
	}
	if (g_i2sdevice[port] != NULL) {
		return &g_i2sdevice[port]->dev;
	}

	struct imxrt_i2s_s *priv;
	int ret;
	i2s_config_t i2s_config = { 0 };
	sai_config_t sai_config = { 0 };

	/* Allocate a new state structure for this chip select.  NOTE that there
	 * is no protection if the same chip select is used in two different
	 * chip select structures.
	 */
	priv = (struct imxrt_i2s_s *)zalloc(sizeof(struct imxrt_i2s_s));
	if (!priv) {
		i2serr("ERROR: Failed to allocate a chip select structure\n");
		return NULL;
	}

    imxrt_i2s_initpins();
    imxrt_i2s_clock_init();

	/* Initialize the I2S priv device structure  */
	sem_init(&priv->exclsem, 0, 1);
	priv->dev.ops = &g_i2sops;

	/* if port does not exist or is not enabled, return error */
	switch (port) {
	case I2S_NUM_0:
	{
		priv->i2s_base = s_saiBases[1];
		priv->tx_isr_num = SAI1_IRQn;
		priv->rx_isr_num = SAI1_IRQn;

		i2s_config = i2s0_default_config;
	}
	break;
	case I2S_NUM_1:
	{
		priv->i2s_base = s_saiBases[2];
		priv->tx_isr_num = SAI2_IRQn;
		priv->rx_isr_num = SAI2_IRQn;

		i2s_config = i2s1_default_config;
	}
	break;
	case I2S_NUM_2:
	{
		priv->i2s_base = s_saiBases[3];
		priv->tx_isr_num = SAI3_TX_IRQn;
		priv->rx_isr_num = SAI3_RX_IRQn;

		i2s_config = i2s2_default_config;
	}
	break;
	default:
		i2serr("ERROR: Port number outside the allowed port number range\n");
		goto errout_with_clocking;
	}

	/* Initialize buffering */
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	i2s_rx_getdefaultconfig(&sai_config);
	i2s_rx_init(priv->i2s_base, &sai_config);
	priv->rxenab = 1;
#endif

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	i2s_tx_getdefaultconfig(&sai_config);
	i2s_tx_init(priv->i2s_base, &sai_config);
	priv->txenab = 1;
#endif

	/* Basic settings */
	priv->i2s_num = port;
	priv->mode = i2s_config.mode;
	priv->bytes_per_sample = 0;
	priv->intrupt_level = i2s_config.int_alloc_level;

	/* configures IRQ */
	priv->isr_handler = &i2s_irq_handler;
	ret = imxrt_i2s_isr_initialize(priv);
	if (ret != OK) {
		i2serr("I2S initialize: isr fails\n");
		goto errout_with_alloc;
	}

	g_i2sdevice[port] = priv;

	i2s_stop((struct i2s_dev_s *)priv, I2S_RX);
	i2s_stop((struct i2s_dev_s *)priv, I2S_TX);

	priv->channel_num = 2;
	priv->bits_per_sample = 16;

	ret = i2s_set_clk(priv, i2s_config.sample_rate, i2s_config.bits_per_sample, (i2s_config.channel_format < I2S_CHANNEL_FMT_ONLY_RIGHT) ? 2 : 1);
	if (ret != OK) {
		i2serr("I2S initialize: i2s_set_clk fails\n");
		goto errout_with_alloc;
	}

	/* Success exit */
	return &priv->dev;

	/* Failure exits */
errout_with_alloc:
errout_with_clocking:
	sem_destroy(&priv->exclsem);
	kmm_free(priv);
	return NULL;
}
#endif							/* I2S_HAVE_RX || I2S_HAVE_TX */
