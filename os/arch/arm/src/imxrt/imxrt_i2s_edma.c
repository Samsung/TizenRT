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
#include "imxrt_dmamux.h"
#include "imxrt_iomuxc.h"
#include "imxrt_edma.h"
#include "imxrt_i2s_edma.h"

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.sai_edma"
#endif

/*******************************************************************************
 * Definitations
 ******************************************************************************/
/* Used for 32byte aligned */
#define STCD_ADDR(address) (edma_tcd_t *)(((uint32_t)(address) + 32) & ~0x1FU)

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
//#define i2serr(format, ...)
#endif
#define i2serr(format, ...)   printf(format, ##__VA_ARGS__)

#if defined(I2S_HAVE_RX) || defined(I2S_HAVE_TX)

#ifndef MIN
#define MIN(a,b) a < b ? a : b
#endif

#define OVER_SAMPLE_RATE (384U)

/* DMA */
#define IMXRT_DMAMUX DMAMUX
#define IMXRT_DMA DMA0
#define IMXRT_I2S_TX_CHANNEL (0U)
#define IMXRT_I2S_TX_SOURCE kDmaRequestMuxSai1Tx
#define IMXRT_I2S_RX_CHANNEL (1U)
#define IMXRT_I2S_RX_SOURCE kDmaRequestMuxSai1Rx

#define BUFFER_SIZE (1024U)
#define BUFFER_NUMBER (4U)

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define IMXRT_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define IMXRT_LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define IMXRT_I2C_CLK_FREQ ((imxrt_clock_getfreq(kCLOCK_Usb1PllClk) / 8) / (IMXRT_LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

/* Select Audio/Video PLL (786.48 MHz) as sai1 clock source */
#define IMXRT_SAI1_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define IMXRT_SAI1_CLOCK_SOURCE_PRE_DIVIDER (0U)
/* Clock divider for sai1 clock source */
#define IMXRT_SAI1_CLOCK_SOURCE_DIVIDER (63U)
/* Get frequency of sai1 clock */
#define IMXRT_SAI_CLK_FREQ                                                        \
    (imxrt_clock_getfreq(kCLOCK_AudioPllClk) / (DEMO_SAI1_CLOCK_SOURCE_DIVIDER + 1U) / \
     (DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER + 1U))

#define BOARD_CODEC_I2C_BASEADDR LPI2C1
#define BOARD_CODEC_I2C_CLOCK_SOURCE_SELECT (0U)
#define BOARD_CODEC_I2C_CLOCK_SOURCE_DIVIDER (5U)
#define BOARD_CODEC_I2C_CLOCK_FREQ \
    ((imxrt_clock_getfreq(kCLOCK_Usb1PllClk) / 8) / (BOARD_CODEC_I2C_CLOCK_SOURCE_DIVIDER + 1U))

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t Buffer[BUFFER_NUMBER * BUFFER_SIZE], 4);
static uint32_t tx_index = 0U, rx_index = 0U;
volatile uint32_t emptyBlock = BUFFER_NUMBER;
#ifdef IMXRT_AUDIO_CODEC
codec_handle_t codecHandle = {0};
extern codec_config_t boardCodecConfig;
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/
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

    sai_config_t saiTxConfig;
    sai_transfer_format_t audioFormat;
	sai_edma_handle_t   si_tx_handle; 
    sai_edma_handle_t   si_rx_handle;     
    edma_handle_t  edma_txhandle; 
    edma_handle_t  edma_rxhandle;    
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

static void i2s_tx_setformat(I2S_Type *base, sai_transfer_format_t *format, uint32_t mclkSourceClockHz, uint32_t bclkSourceClockHz);
static void i2s_rx_setformat(I2S_Type *base, sai_transfer_format_t *format, uint32_t mclkSourceClockHz, uint32_t bclkSourceClockHz);

static void i2s_tx_enable(I2S_Type *base, bool enable);
static void i2s_rx_enable(I2S_Type *base, bool enable);

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
/* Base pointer array */
static I2S_Type *const s_saiBases[] = I2S_BASE_PTRS;
/*!@brief SAI handle pointer */
sai_handle_t *s_saiHandle[ARRAY_SIZE(s_saiBases)][2];

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/* Clock name array */
static const clock_ip_name_t s_saiClock[] = SAI_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*<! Structure definition for uart_edma_private_handle_t. The structure is private. */
typedef struct _sai_edma_private_handle {
    I2S_Type *base;
    sai_edma_handle_t *handle;
} sai_edma_private_handle_t;

enum _sai_edma_transfer_state {
    kSAI_Busy = 0x0U, /*!< SAI is busy */
    kSAI_Idle,        /*!< Transfer is done. */
};

/*<! Private handle only used for internally. */
static sai_edma_private_handle_t s_edmaPrivateHandle[ARRAY_SIZE(s_saiBases)][2];

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

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Get the instance number for SAI.
 *
 * @param base SAI base pointer.
 */
static uint32_t imxrt_i2s_getinstance(I2S_Type *base);

/*!
 * @brief SAI EDMA callback for send.
 *
 * @param handle pointer to sai_edma_handle_t structure which stores the transfer state.
 * @param userData Parameter for user callback.
 * @param done If the DMA transfer finished.
 * @param tcds The TCD index.
 */
static void imxrt_i2s_txedmacallback(edma_handle_t *handle, void *userData, bool done, uint32_t tcds);

/*!
 * @brief SAI EDMA callback for receive.
 *
 * @param handle pointer to sai_edma_handle_t structure which stores the transfer state.
 * @param userData Parameter for user callback.
 * @param done If the DMA transfer finished.
 * @param tcds The TCD index.
 */
static void imxrt_i2s_rxedmacallback(edma_handle_t *handle, void *userData, bool done, uint32_t tcds);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM)
 *                              = 24 * (32 + 77/100)
 *                              = 786.48 MHz
 */
const clock_audio_pll_config_t audioPllConfig = {
    .loopDivider = 32,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
    .numerator = 77,    /* 30 bit numerator of fractional loop divider. */
    .denominator = 100, /* 30 bit denominator of fractional loop divider */
};

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

static uint32_t imxrt_i2s_getinstance(I2S_Type *base)
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

static void imxrt_i2s_txedmacallback(edma_handle_t *handle, void *userData, bool done, uint32_t tcds)
{
    sai_edma_private_handle_t *privHandle = (sai_edma_private_handle_t *)userData;
    sai_edma_handle_t *saiHandle = privHandle->handle;

    /* If finished a blcok, call the callback function */
    memset(&saiHandle->saiQueue[saiHandle->queueDriver], 0, sizeof(sai_transfer_t));
    saiHandle->queueDriver = (saiHandle->queueDriver + 1) % SAI_XFER_QUEUE_SIZE;
    if (saiHandle->callback) {
        (saiHandle->callback)(privHandle->base, saiHandle, kStatus_SAI_TxIdle, saiHandle->userData);
    }

    /* If all data finished, just stop the transfer */
    if (saiHandle->saiQueue[saiHandle->queueDriver].data == NULL) {
        /* Disable DMA enable bit */
        i2s_tx_enabledma(privHandle->base, kSAI_FIFORequestDMAEnable, false);
        imxrt_edma_aborttransfer(handle);
    }
}

static void imxrt_i2s_rxedmacallback(edma_handle_t *handle, void *userData, bool done, uint32_t tcds)
{
    sai_edma_private_handle_t *privHandle = (sai_edma_private_handle_t *)userData;
    sai_edma_handle_t *saiHandle = privHandle->handle;

    /* If finished a blcok, call the callback function */
    memset(&saiHandle->saiQueue[saiHandle->queueDriver], 0, sizeof(sai_transfer_t));
    saiHandle->queueDriver = (saiHandle->queueDriver + 1) % SAI_XFER_QUEUE_SIZE;
    if (saiHandle->callback) {
        (saiHandle->callback)(privHandle->base, saiHandle, kStatus_SAI_RxIdle, saiHandle->userData);
    }

    /* If all data finished, just stop the transfer */
    if (saiHandle->saiQueue[saiHandle->queueDriver].data == NULL) {
        /* Disable DMA enable bit */
        i2s_tx_enabledma(privHandle->base, kSAI_FIFORequestDMAEnable, false);
        imxrt_edma_aborttransfer(handle);
    }
}

/*!
 * brief Initializes the SAI eDMA handle.
 *
 * This function initializes the SAI master DMA handle, which can be used for other SAI master transactional APIs.
 * Usually, for a specified SAI instance, call this API once to get the initialized handle.
 *
 * param base SAI base pointer.
 * param handle SAI eDMA handle pointer.
 * param base SAI peripheral base address.
 * param callback Pointer to user callback function.
 * param userData User parameter passed to the callback function.
 * param dmaHandle eDMA handle pointer, this handle shall be static allocated by users.
 */
void imxrt_i2s_transfertxcreatehandleedma(
    I2S_Type *base, sai_edma_handle_t *handle, sai_edma_callback_t callback, void *userData, edma_handle_t *dmaHandle)
{
    DEBUGASSERT(handle && dmaHandle);

    uint32_t instance = imxrt_i2s_getinstance(base);

    /* Zero the handle */
    memset(handle, 0, sizeof(*handle));

    /* Set sai base to handle */
    handle->dmaHandle = dmaHandle;
    handle->callback = callback;
    handle->userData = userData;

    /* Set SAI state to idle */
    handle->state = kSAI_Idle;

    s_edmaPrivateHandle[instance][0].base = base;
    s_edmaPrivateHandle[instance][0].handle = handle;

    /* Need to use scatter gather */
    imxrt_edma_installtcdmemory(dmaHandle, (edma_tcd_t *)(STCD_ADDR(handle->tcd)), SAI_XFER_QUEUE_SIZE);

    /* Install callback for Tx dma channel */
    imxrt_edma_setcallback(dmaHandle, imxrt_i2s_txedmacallback, &s_edmaPrivateHandle[instance][0]);
}

/*!
 * brief Initializes the SAI Rx eDMA handle.
 *
 * This function initializes the SAI slave DMA handle, which can be used for other SAI master transactional APIs.
 * Usually, for a specified SAI instance, call this API once to get the initialized handle.
 *
 * param base SAI base pointer.
 * param handle SAI eDMA handle pointer.
 * param base SAI peripheral base address.
 * param callback Pointer to user callback function.
 * param userData User parameter passed to the callback function.
 * param dmaHandle eDMA handle pointer, this handle shall be static allocated by users.
 */
void imxrt_i2s_tranferrxcreatehandleedma(
    I2S_Type *base, sai_edma_handle_t *handle, sai_edma_callback_t callback, void *userData, edma_handle_t *dmaHandle)
{
    DEBUGASSERT(handle && dmaHandle);

    uint32_t instance = imxrt_i2s_getinstance(base);

    /* Zero the handle */
    memset(handle, 0, sizeof(*handle));

    /* Set sai base to handle */
    handle->dmaHandle = dmaHandle;
    handle->callback = callback;
    handle->userData = userData;

    /* Set SAI state to idle */
    handle->state = kSAI_Idle;

    s_edmaPrivateHandle[instance][1].base = base;
    s_edmaPrivateHandle[instance][1].handle = handle;

    /* Need to use scatter gather */
    imxrt_edma_installtcdmemory(dmaHandle, STCD_ADDR(handle->tcd), SAI_XFER_QUEUE_SIZE);

    /* Install callback for Tx dma channel */
    imxrt_edma_setcallback(dmaHandle, imxrt_i2s_rxedmacallback, &s_edmaPrivateHandle[instance][1]);
}

/*!
 * brief Configures the SAI Tx audio format.
 *
 * The audio format can be changed at run-time. This function configures the sample rate and audio data
 * format to be transferred. This function also sets the eDMA parameter according to formatting requirements.
 *
 * param base SAI base pointer.
 * param handle SAI eDMA handle pointer.
 * param format Pointer to SAI audio data format structure.
 * param mclkSourceClockHz SAI master clock source frequency in Hz.
 * param bclkSourceClockHz SAI bit clock source frequency in Hz. If bit clock source is master
 * clock, this value should equals to masterClockHz in format.
 * retval kStatus_Success Audio format set successfully.
 * retval kStatus_InvalidArgument The input argument is invalid.
*/
void imxrt_i2s_transfertxsetformatedma(I2S_Type *base,
                                 sai_edma_handle_t *handle,
                                 sai_transfer_format_t *format,
                                 uint32_t mclkSourceClockHz,
                                 uint32_t bclkSourceClockHz)
{
    DEBUGASSERT(handle && format);

    /* Configure the audio format to SAI registers */
    i2s_tx_setformat(base, format, mclkSourceClockHz, bclkSourceClockHz);

    /* Get the tranfer size from format, this should be used in EDMA configuration */
    if (format->bitWidth == 24U) {
        handle->bytesPerFrame = 4U;
    } else {
        handle->bytesPerFrame = format->bitWidth / 8U;
    }

    /* Update the data channel SAI used */
    handle->channel = format->channel;

    /* Clear the channel enable bits unitl do a send/receive */
    base->TCR3 &= ~I2S_TCR3_TCE_MASK;
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    handle->count = FSL_FEATURE_SAI_FIFO_COUNT - format->watermark;
#else
    handle->count = 1U;
#endif /* FSL_FEATURE_SAI_FIFO_COUNT */
}

/*!
 * brief Configures the SAI Rx audio format.
 *
 * The audio format can be changed at run-time. This function configures the sample rate and audio data
 * format to be transferred. This function also sets the eDMA parameter according to formatting requirements.
 *
 * param base SAI base pointer.
 * param handle SAI eDMA handle pointer.
 * param format Pointer to SAI audio data format structure.
 * param mclkSourceClockHz SAI master clock source frequency in Hz.
 * param bclkSourceClockHz SAI bit clock source frequency in Hz. If a bit clock source is the master
 * clock, this value should equal to masterClockHz in format.
 * retval kStatus_Success Audio format set successfully.
 * retval kStatus_InvalidArgument The input argument is invalid.
*/
void imxrt_i2s_transferrxsetformatedma(I2S_Type *base,
                                 sai_edma_handle_t *handle,
                                 sai_transfer_format_t *format,
                                 uint32_t mclkSourceClockHz,
                                 uint32_t bclkSourceClockHz)
{
    DEBUGASSERT(handle && format);

    /* Configure the audio format to SAI registers */
    i2s_rx_setformat(base, format, mclkSourceClockHz, bclkSourceClockHz);

    /* Get the tranfer size from format, this should be used in EDMA configuration */
    if (format->bitWidth == 24U) {
        handle->bytesPerFrame = 4U;
    } else {
        handle->bytesPerFrame = format->bitWidth / 8U;
    }

    /* Update the data channel SAI used */
    handle->channel = format->channel;

    /* Clear the channel enable bits unitl do a send/receive */
    base->RCR3 &= ~I2S_RCR3_RCE_MASK;
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    handle->count = format->watermark;
#else
    handle->count = 1U;
#endif /* FSL_FEATURE_SAI_FIFO_COUNT */
}

/*!
 * brief Performs a non-blocking SAI transfer using DMA.
 *
 * note This interface returns immediately after the transfer initiates. Call
 * SAI_GetTransferStatus to poll the transfer status and check whether the SAI transfer is finished.
 *
 * param base SAI base pointer.
 * param handle SAI eDMA handle pointer.
 * param xfer Pointer to the DMA transfer structure.
 * retval kStatus_Success Start a SAI eDMA send successfully.
 * retval kStatus_InvalidArgument The input argument is invalid.
 * retval kStatus_TxBusy SAI is busy sending data.
 */
status_t imxrt_i2s_transfersendedma(I2S_Type *base, sai_edma_handle_t *handle, sai_transfer_t *xfer)
{
    DEBUGASSERT(handle && xfer);

    edma_transfer_config_t config = {0};
    uint32_t destAddr = i2s_tx_getdataregisteraddress(base, handle->channel);

    /* Check if input parameter invalid */
    if ((xfer->data == NULL) || (xfer->dataSize == 0U)) {
        return kStatus_InvalidArgument;
    }

    if (handle->saiQueue[handle->queueUser].data) {
        return kStatus_SAI_QueueFull;
    }

    /* Change the state of handle */
    handle->state = kSAI_Busy;

    /* Update the queue state */
    handle->transferSize[handle->queueUser] = xfer->dataSize;
    handle->saiQueue[handle->queueUser].data = xfer->data;
    handle->saiQueue[handle->queueUser].dataSize = xfer->dataSize;
    handle->queueUser = (handle->queueUser + 1) % SAI_XFER_QUEUE_SIZE;

    /* Prepare edma configure */
    imxrt_edma_preparetransfer(&config, xfer->data, handle->bytesPerFrame, (void *)destAddr, handle->bytesPerFrame,
                         handle->count * handle->bytesPerFrame, xfer->dataSize, kEDMA_MemoryToPeripheral);

    /* Store the initially configured eDMA minor byte transfer count into the SAI handle */
    handle->nbytes = handle->count * handle->bytesPerFrame;

    imxrt_edma_submittransfer(handle->dmaHandle, &config);

    /* Start DMA transfer */
    imxrt_edma_starttransfer(handle->dmaHandle);

    /* Enable DMA enable bit */
    i2s_tx_enabledma(base, kSAI_FIFORequestDMAEnable, true);

    /* Enable SAI Tx clock */
    i2s_tx_enable(base, true);

    /* Enable the channel FIFO */
    base->TCR3 |= I2S_TCR3_TCE(1U << handle->channel);

    return kStatus_Success;
}

/*!
 * brief Performs a non-blocking SAI receive using eDMA.
 *
 * note This interface returns immediately after the transfer initiates. Call
 * the SAI_GetReceiveRemainingBytes to poll the transfer status and check whether the SAI transfer is finished.
 *
 * param base SAI base pointer
 * param handle SAI eDMA handle pointer.
 * param xfer Pointer to DMA transfer structure.
 * retval kStatus_Success Start a SAI eDMA receive successfully.
 * retval kStatus_InvalidArgument The input argument is invalid.
 * retval kStatus_RxBusy SAI is busy receiving data.
 */
status_t imxrt_i2s_transferreceiveedma(I2S_Type *base, sai_edma_handle_t *handle, sai_transfer_t *xfer)
{
    DEBUGASSERT(handle && xfer);

    edma_transfer_config_t config = {0};
    uint32_t srcAddr = i2s_rx_getdataregisteraddress(base, handle->channel);

    /* Check if input parameter invalid */
    if ((xfer->data == NULL) || (xfer->dataSize == 0U)) {
        return kStatus_InvalidArgument;
    }

    if (handle->saiQueue[handle->queueUser].data) {
        return kStatus_SAI_QueueFull;
    }

    /* Change the state of handle */
    handle->state = kSAI_Busy;

    /* Update queue state  */
    handle->transferSize[handle->queueUser] = xfer->dataSize;
    handle->saiQueue[handle->queueUser].data = xfer->data;
    handle->saiQueue[handle->queueUser].dataSize = xfer->dataSize;
    handle->queueUser = (handle->queueUser + 1) % SAI_XFER_QUEUE_SIZE;

    /* Prepare edma configure */
    imxrt_edma_preparetransfer(&config, (void *)srcAddr, handle->bytesPerFrame, xfer->data, handle->bytesPerFrame,
                         handle->count * handle->bytesPerFrame, xfer->dataSize, kEDMA_PeripheralToMemory);

    /* Store the initially configured eDMA minor byte transfer count into the SAI handle */
    handle->nbytes = handle->count * handle->bytesPerFrame;

    imxrt_edma_submittransfer(handle->dmaHandle, &config);

    /* Start DMA transfer */
    imxrt_edma_starttransfer(handle->dmaHandle);

    /* Enable DMA enable bit */
    i2s_rx_enabledma(base, kSAI_FIFORequestDMAEnable, true);

    /* Enable the channel FIFO */
    base->RCR3 |= I2S_RCR3_RCE(1U << handle->channel);

    /* Enable SAI Rx clock */
    i2s_rx_enable(base, true);

    return kStatus_Success;
}

/*!
 * brief Aborts a SAI transfer using eDMA.
 *
 * This function only aborts the current transfer slots, the other transfer slots' information still kept
 * in the handler. If users want to terminate all transfer slots, just call SAI_TransferTerminateSendEDMA.
 *
 * param base SAI base pointer.
 * param handle SAI eDMA handle pointer.
 */
void imxrt_i2s_transferabortsendedma(I2S_Type *base, sai_edma_handle_t *handle)
{
    DEBUGASSERT(handle);

    /* Disable dma */
    imxrt_edma_aborttransfer(handle->dmaHandle);

    /* Disable the channel FIFO */
    base->TCR3 &= ~I2S_TCR3_TCE_MASK;

    /* Disable DMA enable bit */
    i2s_tx_enabledma(base, kSAI_FIFORequestDMAEnable, false);

    /* Disable Tx */
    i2s_tx_enable(base, false);

    /* Reset the FIFO pointer, at the same time clear all error flags if set */
    base->TCSR |= (I2S_TCSR_FR_MASK | I2S_TCSR_SR_MASK);
    base->TCSR &= ~I2S_TCSR_SR_MASK;

    /* Handle the queue index */
    memset(&handle->saiQueue[handle->queueDriver], 0, sizeof(sai_transfer_t));
    handle->queueDriver = (handle->queueDriver + 1) % SAI_XFER_QUEUE_SIZE;

    /* Set the handle state */
    handle->state = kSAI_Idle;
}

/*!
 * brief Aborts a SAI receive using eDMA.
 *
 * This function only aborts the current transfer slots, the other transfer slots' information still kept
 * in the handler. If users want to terminate all transfer slots, just call SAI_TransferTerminateReceiveEDMA.
 *
 * param base SAI base pointer
 * param handle SAI eDMA handle pointer.
 */
void imxrt_i2s_transferabortreceiveedma(I2S_Type *base, sai_edma_handle_t *handle)
{
    DEBUGASSERT(handle);

    /* Disable dma */
    imxrt_edma_aborttransfer(handle->dmaHandle);

    /* Disable the channel FIFO */
    base->RCR3 &= ~I2S_RCR3_RCE_MASK;

    /* Disable DMA enable bit */
    i2s_rx_enabledma(base, kSAI_FIFORequestDMAEnable, false);

    /* Disable Rx */
    i2s_rx_enable(base, false);

    /* Reset the FIFO pointer, at the same time clear all error flags if set */
    base->RCSR |= (I2S_RCSR_FR_MASK | I2S_RCSR_SR_MASK);
    base->RCSR &= ~I2S_RCSR_SR_MASK;

    /* Handle the queue index */
    memset(&handle->saiQueue[handle->queueDriver], 0, sizeof(sai_transfer_t));
    handle->queueDriver = (handle->queueDriver + 1) % SAI_XFER_QUEUE_SIZE;

    /* Set the handle state */
    handle->state = kSAI_Idle;
}

/*!
 * brief Terminate all SAI send.
 *
 * This function will clear all transfer slots buffered in the sai queue. If users only want to abort the
 * current transfer slot, please call SAI_TransferAbortSendEDMA.
 *
 * param base SAI base pointer.
 * param handle SAI eDMA handle pointer.
 */
void imxrt_i2s_transferterminatesendedma(I2S_Type *base, sai_edma_handle_t *handle)
{
    DEBUGASSERT(handle);

    /* Abort the current transfer */
    imxrt_i2s_transferabortsendedma(base, handle);

    /* Clear all the internal information */
    memset(handle->tcd, 0U, sizeof(handle->tcd));
    memset(handle->saiQueue, 0U, sizeof(handle->saiQueue));
    memset(handle->transferSize, 0U, sizeof(handle->transferSize));
    handle->queueUser = 0U;
    handle->queueDriver = 0U;
}

/*!
 * brief Terminate all SAI receive.
 *
 * This function will clear all transfer slots buffered in the sai queue. If users only want to abort the
 * current transfer slot, please call SAI_TransferAbortReceiveEDMA.
 *
 * param base SAI base pointer.
 * param handle SAI eDMA handle pointer.
 */
void imxrt_i2s_transferterminatereceiveedma(I2S_Type *base, sai_edma_handle_t *handle)
{
    DEBUGASSERT(handle);

    /* Abort the current transfer */
    imxrt_i2s_transferabortreceiveedma(base, handle);

    /* Clear all the internal information */
    memset(handle->tcd, 0U, sizeof(handle->tcd));
    memset(handle->saiQueue, 0U, sizeof(handle->saiQueue));
    memset(handle->transferSize, 0U, sizeof(handle->transferSize));
    handle->queueUser = 0U;
    handle->queueDriver = 0U;
}

/*!
 * brief Gets byte count sent by SAI.
 *
 * param base SAI base pointer.
 * param handle SAI eDMA handle pointer.
 * param count Bytes count sent by SAI.
 * retval kStatus_Success Succeed get the transfer count.
 * retval kStatus_NoTransferInProgress There is no non-blocking transaction in progress.
 */
status_t imxrt_i2s_transfergetsendcountedma(I2S_Type *base, sai_edma_handle_t *handle, size_t *count)
{
    DEBUGASSERT(handle);

    status_t status = kStatus_Success;

    if (handle->state != kSAI_Busy) {
        status = kStatus_NoTransferInProgress;
    } else {
        *count = (handle->transferSize[handle->queueDriver] -
                  (uint32_t)handle->nbytes *
                      imxrt_edma_getremainingmajorloopcount(handle->dmaHandle->base, handle->dmaHandle->channel));
    }

    return status;
}

/*!
 * brief Gets byte count received by SAI.
 *
 * param base SAI base pointer
 * param handle SAI eDMA handle pointer.
 * param count Bytes count received by SAI.
 * retval kStatus_Success Succeed get the transfer count.
 * retval kStatus_NoTransferInProgress There is no non-blocking transaction in progress.
 */
status_t imxrt_i2s_transfergetreceivecountedma(I2S_Type *base, sai_edma_handle_t *handle, size_t *count)
{
    DEBUGASSERT(handle);

    status_t status = kStatus_Success;

    if (handle->state != kSAI_Busy) {
        status = kStatus_NoTransferInProgress;
    } else {
        *count = (handle->transferSize[handle->queueDriver] -
                  (uint32_t)handle->nbytes *
                      imxrt_edma_getremainingmajorloopcount(handle->dmaHandle->base, handle->dmaHandle->channel));
    }

    return status;
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
    imxrt_clock_enableclock(s_saiClock[imxrt_i2s_getinstance(base)]);
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
    switch (config->syncMode)
    {
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
	imxrt_clock_enableclock(s_saiClock[imxrt_i2s_getinstance(base)]);
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
    i2s_tx_enabledma(base, kSAI_FIFORequestDMAEnable, false);
    i2s_rx_enabledma(base, kSAI_FIFORequestDMAEnable, false);
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
	imxrt_clock_disableclock(s_saiClock[imxrt_i2s_getinstance(base)]);
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
    sai_transfer_t xfer;

	i2sinfo("[I2S RX] apb=%p nmaxbytes=%d samp=%p arg=%p timeout=%d\n", apb, apb->nmaxbytes, apb->samp, arg, timeout);

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	/* Has the RX channel been enabled? */
	if (!priv->rxenab) {
		i2serr("ERROR: I2S has no receiver\n");
		return -EAGAIN;
	}

	/* Get exclusive access to the I2S driver data */
	i2s_exclsem_take(priv);
	i2sinfo("RX Exclusive Enter\n");

    xfer.data = apb->samp;
    xfer.dataSize = apb->nmaxbytes;
    if (kStatus_Success == imxrt_i2s_transferreceiveedma(priv->i2s_base, &priv->si_rx_handle, &xfer)) {
        rx_index++;
    }
    if (rx_index == BUFFER_NUMBER) {
        rx_index = 0U;
    }

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
    sai_transfer_t xfer;

	DEBUGASSERT(priv && apb);

	i2sinfo("[I2S TX] apb=%p nbytes=%d arg=%p samp=%p timeout=%d\n", apb, apb->nbytes - apb->curbyte, apb->samp, arg, timeout);

	i2s_dump_buffer("Sending", &apb->samp[apb->curbyte], apb->nbytes - apb->curbyte);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)

	/* Has the TX channel been enabled? */
	if (!priv->txenab) {
		i2serr("ERROR: I2S has no transmitter\n");
		return -EAGAIN;
	}

	/* Get exclusive access to the I2S driver data */
	i2s_exclsem_take(priv);
	i2sinfo("TX Exclusive Enter\n");

    xfer.data = apb->samp;
    xfer.dataSize = apb->nbytes - apb->curbyte;

    if (kStatus_Success == imxrt_i2s_transfersendedma(priv->i2s_base, &priv->si_tx_handle, &xfer)) {
        tx_index++;
    }
    if (tx_index == BUFFER_NUMBER) {
        tx_index = 0U;
    }

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
        i2s_tx_enabledma(priv->i2s_base, kSAI_FIFORequestDMAEnable, false);
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX && priv->rxenab) {
		i2s_rx_enable(priv->i2s_base, false);
        i2s_rx_enabledma(priv->i2s_base, kSAI_FIFORequestDMAEnable, false);
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
        i2s_tx_enabledma(priv->i2s_base, kSAI_FIFORequestDMAEnable, false);
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX && priv->rxenab) {
		i2s_rx_enable(priv->i2s_base, true);
        i2s_rx_enabledma(priv->i2s_base, kSAI_FIFORequestDMAEnable, false);
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
		imxrt_i2s_transferabortsendedma(priv->i2s_base, &priv->si_tx_handle);
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX) {
		imxrt_i2s_transferabortreceiveedma(priv->i2s_base, &priv->si_rx_handle);
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
		imxrt_i2s_transferterminatesendedma(priv->i2s_base, &priv->si_tx_handle);
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX) {
		imxrt_i2s_transferterminatereceiveedma(priv->i2s_base, &priv->si_rx_handle);
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
    if (format.bitWidth == 24U) {
        priv->si_tx_handle.bytesPerFrame = 4U;
    } else {
        priv->si_tx_handle.bytesPerFrame = format.bitWidth / 8U;
    }
    priv->si_tx_handle.channel = format.channel;
	mclkSourceClockHz = OVER_SAMPLE_RATE * format.sampleRate_Hz;
	i2s_tx_setformat(priv->i2s_base, &format, mclkSourceClockHz, format.masterClockHz);

    /* Copy format to handle */
    if (format.bitWidth == 24U) {
        priv->si_rx_handle.bytesPerFrame = 4U;
    } else {
        priv->si_rx_handle.bytesPerFrame = format.bitWidth / 8U;
    }
    priv->si_rx_handle.channel = format.channel;
	mclkSourceClockHz = OVER_SAMPLE_RATE * format.sampleRate_Hz;
	i2s_tx_setformat(priv->i2s_base, &format, mclkSourceClockHz, format.masterClockHz);

	// wait all writing on-going finish
	i2s_exclsem_give(priv);

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
	struct imxrt_i2s_s *priv = (struct imxrt_i2s_s *)dev;
	DEBUGASSERT(priv && rate > 0);
	return i2s_set_clk(priv, rate, priv->bits_per_sample, priv->channel_num);
}

static void i2s_tx_callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_TxError == status) {
        /* Handle the error. */
    } else {
        emptyBlock++;
    }
}

static void i2s_rx_callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_RxError == status) {
        /* Handle the error. */
    } else {
        emptyBlock--;
    }
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
    imxrt_clock_setmux(kCLOCK_Lpi2cMux, IMXRT_LPI2C_CLOCK_SOURCE_SELECT);
    imxrt_clock_setdiv(kCLOCK_Lpi2cDiv, IMXRT_LPI2C_CLOCK_SOURCE_DIVIDER);

    /*Clock setting for SAI1*/
    imxrt_clock_setmux(kCLOCK_Sai1Mux, IMXRT_SAI1_CLOCK_SOURCE_SELECT);
    imxrt_clock_setdiv(kCLOCK_Sai1PreDiv, IMXRT_SAI1_CLOCK_SOURCE_PRE_DIVIDER);
    imxrt_clock_setdiv(kCLOCK_Sai1Div, IMXRT_SAI1_CLOCK_SOURCE_DIVIDER);

    IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK;
}

#ifdef IMXRT_AUDIO_CODEC
void imxrt_i2s_lpi2cinit(LPI2C_Type *base, uint32_t clkSrc_Hz)
{
    lpi2c_master_config_t lpi2cConfig = {0};

    /*
     * lpi2cConfig.debugEnable = false;
     * lpi2cConfig.ignoreAck = false;
     * lpi2cConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * lpi2cConfig.baudRate_Hz = 100000U;
     * lpi2cConfig.busIdleTimeout_ns = 0;
     * lpi2cConfig.pinLowTimeout_ns = 0;
     * lpi2cConfig.sdaGlitchFilterWidth_ns = 0;
     * lpi2cConfig.sclGlitchFilterWidth_ns = 0;
     */
    imxrt_lpi2c_mastergetdefaultconfig(&lpi2cConfig);
    imxrt_lpi2c_masterinit(base, &lpi2cConfig, clkSrc_Hz);
}

status_t imxrt_i2s_lpi2csend(LPI2C_Type *base,
                          uint8_t deviceAddress,
                          uint32_t subAddress,
                          uint8_t subAddressSize,
                          uint8_t *txBuff,
                          uint8_t txBuffSize)
{
    status_t reVal;

    /* Send master blocking data to slave */
    reVal = imxrt_lpi2c_masterstart(base, deviceAddress, kLPI2C_Write);
    if (kStatus_Success == reVal) {
        while (imxrt_lpi2c_mastergetstatusflags(base) & kLPI2C_MasterNackDetectFlag) {
        }

        reVal = imxrt_lpi2c_mastersend(base, &subAddress, subAddressSize);
        if (reVal != kStatus_Success) {
            return reVal;
        }

        reVal = imxrt_lpi2c_mastersend(base, txBuff, txBuffSize);
        if (reVal != kStatus_Success) {
            return reVal;
        }

        reVal = imxrt_lpi2c_masterstop(base);
        if (reVal != kStatus_Success) {
            return reVal;
        }
    }

    return reVal;
}

status_t imxrt_i2s_lpi2creceive(LPI2C_Type *base,
                             uint8_t deviceAddress,
                             uint32_t subAddress,
                             uint8_t subAddressSize,
                             uint8_t *rxBuff,
                             uint8_t rxBuffSize)
{
    status_t reVal;

    reVal = imxrt_lpi2c_masterstart(base, deviceAddress, kLPI2C_Write);
    if (kStatus_Success == reVal) {
        while (imxrt_lpi2c_mastergetstatusflags(base) & kLPI2C_MasterNackDetectFlag) {
        }

        reVal = imxrt_lpi2c_mastersend(base, &subAddress, subAddressSize);
        if (reVal != kStatus_Success) {
            return reVal;
        }

        reVal = imxrt_lpi2c_masterrepeatedstart(base, deviceAddress, kLPI2C_Read);

        if (reVal != kStatus_Success) {
            return reVal;
        }

        reVal = imxrt_lpi2c_masterreceive(base, rxBuff, rxBuffSize);
        if (reVal != kStatus_Success) {
            return reVal;
        }

        reVal = imxrt_lpi2c_masterstop(base);
        if (reVal != kStatus_Success) {
            return reVal;
        }
    }
    return reVal;
}
void imxrt_codec_i2cinit(void)
{
    imxrt_i2s_lpi2cinit(BOARD_CODEC_I2C_BASEADDR, BOARD_CODEC_I2C_CLOCK_FREQ);
}

status_t imxrt_codec_i2csend(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return imxrt_i2s_lpi2csend(BOARD_CODEC_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize);
}

status_t imxrt_codec_i2creceive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return imxrt_i2s_lpi2creceive(BOARD_CODEC_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff, rxBuffSize);
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
    edma_config_t dmaConfig = {0};

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

		i2s_config = i2s0_default_config;
	}
	break;
	case I2S_NUM_1:
	{
		priv->i2s_base = s_saiBases[2];

		i2s_config = i2s1_default_config;
	}
	break;
	case I2S_NUM_2:
	{
		priv->i2s_base = s_saiBases[3];

		i2s_config = i2s2_default_config;
	}
	break;
	default:
		i2serr("ERROR: Port number outside the allowed port number range\n");
		goto errout_with_clocking;
	}

    imxrt_edma_getdefaultconfig(&dmaConfig);
    imxrt_edma_init(IMXRT_DMA, &dmaConfig);
    imxrt_dmamux_init(IMXRT_DMAMUX);

	/* Initialize buffering */
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	i2s_rx_getdefaultconfig(&sai_config);
	i2s_rx_init(priv->i2s_base, &sai_config);
	priv->rxenab = 1;
    imxrt_edma_createhandle(&priv->edma_rxhandle, IMXRT_DMA, IMXRT_I2S_RX_CHANNEL);
    imxrt_dmamux_setsource(IMXRT_DMAMUX, IMXRT_I2S_RX_CHANNEL, (uint8_t)IMXRT_I2S_RX_SOURCE);
    imxrt_dmamux_enablechannel(IMXRT_DMAMUX, IMXRT_I2S_RX_CHANNEL);
    imxrt_i2s_transfertxcreatehandleedma(priv->i2s_base, &priv->si_rx_handle, i2s_rx_callback, NULL, &priv->edma_rxhandle);
#endif

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	i2s_tx_getdefaultconfig(&sai_config);
	i2s_tx_init(priv->i2s_base, &sai_config);
	priv->txenab = 1;
    imxrt_edma_createhandle(&priv->edma_txhandle, IMXRT_DMA, IMXRT_I2S_TX_CHANNEL);
    imxrt_dmamux_setsource(IMXRT_DMAMUX, IMXRT_I2S_TX_CHANNEL, (uint8_t)IMXRT_I2S_TX_SOURCE);
    imxrt_dmamux_enablechannel(IMXRT_DMAMUX, IMXRT_I2S_TX_CHANNEL);
    imxrt_i2s_transfertxcreatehandleedma(priv->i2s_base, &priv->si_tx_handle, i2s_tx_callback, NULL, &priv->edma_txhandle);
#endif

	/* Basic settings */
	priv->i2s_num = port;
	priv->mode = i2s_config.mode;
	priv->bytes_per_sample = 0;

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

