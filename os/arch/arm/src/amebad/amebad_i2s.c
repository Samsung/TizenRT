/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
#include "amebad_i2s.h"

#include "mbed/targets/hal/rtl8721d/PinNames.h"

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

//#ifndef CONFIG_SCHED_WORKQUEUE
//#error Work queue support is required (CONFIG_SCHED_WORKQUEUE)
//#endif

//#ifndef CONFIG_AUDIO
//#error CONFIG_AUDIO required by this driver
//#endif

#ifndef CONFIG_AMEBAD_I2S_MAXINFLIGHT
#define CONFIG_AMEBAD_I2S_MAXINFLIGHT 32
#endif

/* Assume no RX/TX support until we learn better */
#undef I2S_HAVE_RX
#undef I2S_HAVE_TX

#if defined(CONFIG_AMEBAD_I2S)

/* Check for I2S RX support */
#if defined(CONFIG_AMEBAD_I2S_RX)
#define I2S_HAVE_RX     1
#endif

/* Check for I2S TX support */
#if defined(CONFIG_AMEBAD_I2S_TX)
#define I2S_HAVE_TX     1
#endif

#ifndef CONFIG_AMEBAD_I2S_DATALEN
#define CONFIG_AMEBAD_I2S_DATALEN        (I2S_BITS_PER_SAMPLE_16BIT)
#endif

#ifndef CONFIG_AMEBAD_I2S_SAMPLERATE
#define CONFIG_AMEBAD_I2S_SAMPLERATE     (SR_44p1KHZ)
#endif

#ifndef CONFIG_AMEBAD_I2S_CHANNEL_FMT
#define CONFIG_AMEBAD_I2S_CHANNEL_FMT    (I2S_CHANNEL_FMT_RIGHT_LEFT)
#endif

#ifndef CONFIG_AMEBAD_I2S_COMM_FMT
#define CONFIG_AMEBAD_I2S_COMM_FMT       I2S_COMM_FORMAT_I2S
#endif

#ifndef CONFIG_AMEBAD_I2S_INTR_LEVEL
#define CONFIG_AMEBAD_I2S_INTR_LEVEL    (1)
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

#define I2S_DMA_PAGE_SIZE	768   // 2 ~ 4096
#define I2S_DMA_PAGE_NUM    4   // Vaild number is 2~4

/* I2S configuration parameters for i2s_param_config function */
typedef struct {
	//i2s_mode_t mode;			/*!< I2S work mode */
	int sample_rate;			/*!< I2S sample rate */
	i2s_bits_per_sample_t bits_per_sample;	/*!< I2S bits per sample */

	uint8_t channel_num;
	uint8_t direction;
	uint8_t bits_per_sample;

	bool use_apll;				/*!< I2S using APLL as main I2S clock, enable it to get accurate clock */
	int fixed_mclk;				/*!< I2S using fixed MCLK output. If use_apll = true and fixed_mclk > 0, then the clock output for i2s is fixed and equal to the fixed_mclk value. */
} i2s_config_t;

/* The state of the one I2S peripheral */

struct amebad_i2s_s {
	struct i2s_dev_s dev;           /* Externally visible I2S interface, must the first element!! */


	i2s_t* i2s_object;
	uint32_t i2s_sclk_pin;
	uint32_t i2s_ws_pin;
	uint32_t i2s_sd_tx_pin;
	uint32_t i2s_sd_rx_pin;
	uint32_t i2s_mck_pin;

	uint8_t* i2s_tx_buf;
	uint8_t* i2s_rc_buf;

	i2s_irq_handler rx_isr_handler;
	i2s_irq_handler tx_isr_handler;

	uint8_t i2s_num;             /* I2S controller register base address */

	i2s_err_cb_t err_cb;            /* registered error callback function */
	void *err_cb_arg;               /* argiment to return with err cb call */

	sem_t exclsem;                  /* Assures mutually exclusive acess to I2S */

	uint8_t rxenab:1;               /* True: RX transfers enabled */
	uint8_t txenab:1;               /* True: TX transfers enabled */

	//i2s_mode_t mode;                /*!< I2S work mode */
	int sample_rate;                /*!< I2S sample rate */
	int channel_num;                /*!< Number of channels */
	int bytes_per_sample;           /*!< Bytes per sample */
	int bits_per_sample;            /*!< Bits per sample */

	bool use_apll;                  /*!< I2S use APLL clock */
	int fixed_mclk;                 /*!< I2S fixed MLCK clock */

};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* Register helpers */
#define CONFIG_AMEBAD_I2S_DUMPBUFFERS   0

#if defined(CONFIG_AMEBAD_I2S_DUMPBUFFERS) && (0 < CONFIG_AMEBAD_I2S_DUMPBUFFERS)
#define       i2s_init_buffer(b, s)   memset(b, 0x55, s);
#define       i2s_dump_buffer(m, b, s) lib_dumpbuffer(m, b, s)
#else
#define       i2s_init_buffer(b, s)
#define       i2s_dump_buffer(m, b, s)
#endif

/* Semaphore helpers */
static void i2s_exclsem_take(struct amebad_i2s_s *priv);
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

int i2s_set_clk(struct amebad_i2s_s *priv, uint32_t rate, i2s_bits_per_sample_t bits, i2s_channel_t ch);

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

static const i2s_config_t i2s_default_config = {
	/*
#if defined(CONFIG_AMEBAD_I2S0_MODE_MASTER) && (0 < CONFIG_AMEBAD_I2S0_MODE_MASTER)
	.mode = I2S_MODE_MASTER,
#else
	.mode = I2S_MODE_SLAVE,
#endif
*/

	.sample_rate = CONFIG_AMEBAD_I2S_SAMPLERATE,
	.bits_per_sample = CONFIG_AMEBAD_I2S_DATALEN,

	.channel_num = CH_MONO;
#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	.direction = I2S_DIR_TX;
#endif
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	.direction = I2S_DIR_RX;
#endif
	.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
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

static struct amebad_i2s_s *g_i2sdevice[I2S_NUM_MAX] = {NULL};

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
	return 0;
}

/*!
 * @brief Gets the SAI Tx status flag state.
 *
 * @param base SAI base pointer
 * @return SAI Rx status flag value. Use the Status Mask to get the status value needed.
 */
static inline uint32_t i2s_rx_getstatusflag(I2S_Type *base)
{
	return 0;
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
	return 0;
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
	return 0;
}

/*!
 * @brief Get the instance number for SAI.
 *
 * @param base SAI base pointer.
 */
uint32_t i2s_getinstance(I2S_Type *base)
{
	return 0;
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
}

/*!
 * @brief Enables/disables the SAI Tx.
 *
 * @param base SAI base pointer
 * @param enable True means enable SAI Tx, false means disable.
 */
void i2s_tx_enable(I2S_Type *base, bool enable)
{
}

/*!
 * @brief Enables/disables the SAI Rx.
 *
 * @param base SAI base pointer
 * @param enable True means enable SAI Rx, false means disable.
 */
void i2s_rx_enable(I2S_Type *base, bool enable)
{
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
}

/*!
 * @brief Initializes the SAI Rx peripheral.
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
	return;
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
	return;
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
	return;
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
	return;
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
	return;
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
	return;
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
//	base->TDR[channel] = data;
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
	return;
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
	return 0;
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
	return kStatus_Success;
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
	return kStatus_Success;
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
	return;
}

/*!
 * @brief Aborts the current IRQ receive.
 *
 * @note This API can be called when an interrupt non-blocking transfer initiates
 * to abort the transfer early.
 *
 * @param base SAI base pointer
 * @param handle Pointer to the sai_handle_t structure which stores the transfer state.
 */
void i2s_transfer_abortreceive(I2S_Type *base, sai_handle_t *handle)
{
	return;
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
	return;
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
	return;
}

/*!
 * @brief Tx interrupt handler.
 *
 * @param base SAI base pointer.
 * @param handle Pointer to the sai_handle_t structure.
 */
void i2s_transfer_tx_handleirq(void *data, char *pbuf)
{
}

/*!
 * @brief Tx interrupt handler.
 *
 * @param base SAI base pointer.
 * @param handle Pointer to the sai_handle_t structure.
 */
void i2s_transfer_rx_handleirq(void *data, char *pbuf)
{
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

static void i2s_exclsem_take(struct amebad_i2s_s *priv)
{
	return;
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
	struct amebad_i2s_s *priv = (struct amebad_i2s_s *)dev;

	/* Support 16, 24, 32 bits */
	DEBUGASSERT(priv && (bits == I2S_BITS_PER_SAMPLE_16BIT /
				|| bits == I2S_BITS_PER_SAMPLE_32BIT /
				|| bits == I2S_BITS_PER_SAMPLE_24BIT));

	priv->bits_per_sample = bits;

	/* amebad 16, 24, 32, bits setting */
	if (bits == I2S_BITS_PER_SAMPLE_16BIT) priv->i2s_object->word_length = WL_16b;
	else if (bits == I2S_BITS_PER_SAMPLE_24BIT) priv->i2s_object->word_length = WL_24b;
	else if (bits == I2S_BITS_PER_SAMPLE_32BIT) priv->i2s_object->word_length = WL_32b;

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
	struct amebad_i2s_s *priv = (struct amebad_i2s_s *)dev;

	i2sinfo("[I2S RX] apb=%p nmaxbytes=%d samp=%p arg=%p timeout=%d\n", apb, apb->nmaxbytes, apb->samp, arg, timeout);

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	/* Has the RX channel been enabled? */
	if (!priv->rxenab) {
		i2serr("ERROR: I2S has no receiver\n");
		return -EAGAIN;
	}

	i2s_exclsem_take(priv);
	i2sinfo("RX Exclusive Enter\n");

	i2s_set_direction(priv->i2s_object, I2S_DIR_RX);
	i2s_recv_page(priv->i2s_object);

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
	struct amebad_i2s_s *priv = (struct amebad_i2s_s *)dev;

	/* Support 16, 24, 32 bits */
	DEBUGASSERT(priv && (bits == I2S_BITS_PER_SAMPLE_16BIT /
				|| bits == I2S_BITS_PER_SAMPLE_32BIT /
				|| bits == I2S_BITS_PER_SAMPLE_24BIT));

	priv->bits_per_sample = bits;

	/* amebad 16, 24, 32, bits setting */
	if (bits == I2S_BITS_PER_SAMPLE_16BIT) priv->i2s_object->word_length = WL_16b;
	else if (bits == I2S_BITS_PER_SAMPLE_24BIT) priv->i2s_object->word_length = WL_24b;
	else if (bits == I2S_BITS_PER_SAMPLE_32BIT) priv->i2s_object->word_length = WL_32b;

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
	struct amebad_i2s_s *priv = (struct amebad_i2s_s *)dev;
	int* ptx_buf;

	DEBUGASSERT(priv && apb);

	i2sinfo("[I2S TX] apb=%p nbytes=%d arg=%p samp=%p timeout=%d\n", apb, apb->nbytes - apb->curbyte, apb->samp, arg, timeout);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)

	/* Has the TX channel been enabled? */
	if (!priv->txenab) {
		i2serr("ERROR: I2S has no transmitter\n");
		return -EAGAIN;
	}

	i2s_exclsem_take(priv);
	i2sinfo("TX Exclusive Enter\n");

	i2s_set_direction(priv->i2s_object, I2S_DIR_TX);
	ptx_buf = i2s_get_tx_page(priv->i2s_object);
	i2s_send_page(priv->i2s_object, (uint32_t*)ptx_buf);

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
	struct amebad_i2s_s *priv = (struct amebad_i2s_s *)dev;
	DEBUGASSERT(priv);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (dir == I2S_TX && priv->txenab) {
		i2s_disable(priv->i2s_object);
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX && priv->rxenab) {
		i2s_disable(priv->i2s_object);
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
	struct amebad_i2s_s *priv = (struct amebad_i2s_s *)dev;
	DEBUGASSERT(priv);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (dir == I2S_TX && priv->txenab) {
		i2s_enable(priv->i2s_object);
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX && priv->rxenab) {
		i2s_enable(priv->i2s_object);
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
	struct amebad_i2s_s *priv = (struct amebad_i2s_s *)dev;
	DEBUGASSERT(priv);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (dir == I2S_TX) {
		i2s_disable(priv->i2s_object);
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX) {
		i2s_disable(priv->i2s_object);
	}
#endif

	return OK;
}

static int i2s_stop(struct i2s_dev_s *dev, i2s_ch_dir_t dir)
{
	struct amebad_i2s_s *priv = (struct amebad_i2s_s *)dev;
	DEBUGASSERT(priv);

	i2s_exclsem_take(priv);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (dir == I2S_TX) {
		i2s_disable(priv->i2s_object);
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX) {
		i2s_disable(priv->i2s_object);
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
	struct amebad_i2s_s *priv = (struct amebad_i2s_s *)dev;
	DEBUGASSERT(priv);

	priv->err_cb = cb;
	priv->err_cb_arg = arg;

	return OK;
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
int i2s_set_clk(struct amebad_i2s_s *priv, uint32_t rate, i2s_bits_per_sample_t bits, i2s_channel_t ch)
{
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
static int amebad_i2s_isr_initialize(struct amebad_i2s_s *priv)
{
	struct amebad_i2s_s *priv = (struct amebad_i2s_s *)dev;

	/* Attach the GPIO peripheral to the allocated CPU interrupt */
	i2s_tx_irq_handler(priv->i2s_object, (i2s_irq_handler)priv->tx_isr_handler, (uint32_t)priv->i2s_object);
	i2s_rx_irq_handler(priv->i2s_object, (i2s_irq_handler)priv->rx_isr_handler, (uint32_t)priv->i2s_object);

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
	struct amebad_i2s_s *priv = (struct amebad_i2s_s *)dev;
	DEBUGASSERT(priv && rate > 0);

	priv->i2s_object->sampling_rate = rate;
	priv->sample_rate = rate;

	return priv->i2s_object->sampling_rate;
}

/****************************************************************************
* Name: amebad_i2s_initpins
*
* Description:
*   Set the I2S pins.
*
* Input Parameters:
*   dev  - Device-specific state data
*   	 - sclk Serial Clock
*   	 - ws Word Select
*   	 - sd_rx, sd_tx Serial Data rx tx
*   	 - mck Master Clock
*
****************************************************************************/

void amebad_i2s_initpins(struct i2s_dev_s *dev)
{
	struct amebad_i2s_s *priv = (struct amebad_i2s_s *)dev;
	priv->i2s_sclk_pin = PA_2;
	priv->i2s_ws_pin = PA_4;
	priv->i2s_sd_tx_pin = PB_26;
	priv->i2s_sd_rx_pin = PA_0;
	priv->i2s_mck_pin = PA_12;
}

/****************************************************************************
* Name: amebad_i2s_init_buffer
*
* Description:
*   Initialize serial buffers.
*
* Input Parameters:
*   dev  - Device-specific state data
*
****************************************************************************/

void amebad_i2s_init_buffer(struct i2s_dev_s *dev)
{
	struct amebad_i2s_s *priv = (struct amebad_i2s_s *)dev;
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	uint8_t* rx_buffer = (uint8_t *)kmm_malloc(I2S_DMA_PAGE_SIZE*I2S_DMA_PAGE_NUM);
	priv->i2s_rx_buf = rx_buffer;
	priv->rxenab = 1;
#endif

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	uint8_t* tx_buffer = (uint8_t *)kmm_malloc(I2S_DMA_PAGE_SIZE*I2S_DMA_PAGE_NUM);
	priv->i2s_tx_buf = tx_buffer;
	priv->txenab = 1;
#endif
}

/****************************************************************************
* Name: amebad_i2s_deinit_buffer
*
* Description:
*   Deinitialize serial buffers.
*
* Input Parameters:
*   dev  - Device-specific state data
*
****************************************************************************/

void amebad_i2s_deinit_buffer(struct i2s_dev_s* dev)
{
	struct amebad_i2s_s *priv = (struct amebad_i2s_s *)dev;
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (priv->rxenab && priv->i2s_rx_buf){
		kmm_free(priv->i2s_rx_buf);
		priv->i2s_rx_buf = NULL;
		priv->rxenab = 0;
	}
#endif

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (priv->txenab && priv->i2s_tx_buf){
		kmm_free(priv->i2s_tx_buf);
		priv->i2s_tx_buf = NULL;
		priv->txenab = 0;
	}
#endif
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

void i2s_getdefaultconfig(struct amebad_i2s_s *priv)
{
	priv->channel_num = &i2s_default_config->channel_num;
	priv->i2s_object->channel_num = &i2s_default_config->channel_num;

	priv->i2s_object->direction = &i2s_default_config->direction;

	priv->bits_per_sample = &i2s_default_config->bits_per_sample;
	if (priv->bits_per_sample == I2S_BITS_PER_SAMPLE_16BIT) priv->i2s_object->word_length = WL_16b;
        else if (priv->bits_per_sample == I2S_BITS_PER_SAMPLE_24BIT) priv->i2s_object->word_length = WL_24b;
        else if (priv->bits_per_sample == I2S_BITS_PER_SAMPLE_32BIT) priv->i2s_object->word_length = WL_32b;

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
struct i2s_dev_s *amebad_i2s_initialize(uint16_t port)
{
	if (port >= I2S_NUM_MAX) {
		i2serr("ERROR: Port number outside the allowed port number range\n");
		return NULL;
	}
	if (g_i2sdevice[port] != NULL) {
		return &g_i2sdevice[port]->dev;
	}

	struct amebad_i2s_s *priv;
	int ret;

	/* Allocate a new state structure for this chip select.  NOTE that there
	 * is no protection if the same chip select is used in two different
	 * chip select structures.
	 */
	priv = (struct amebad_i2s_s *)kmm_zalloc(sizeof(struct amebad_i2s_s));
	if (!priv) {
		i2serr("ERROR: Failed to allocate a chip select structure\n");
		return NULL;
	}

	/* Get default configuration */
	i2s_getdefaultconfig(priv);

	/* Config values initialization */
	amebad_i2s_initpins(priv);
	i2s_samplerate(priv, SR_44p1KHZ);
	amebad_i2s_init_buffer(priv);

	/* I2s object initialization */
	i2s_init(priv->i2s_object, priv->i2s_sclk_pin, priv->i2s_ws_pin, priv->i2s_sd_tx_pin, priv->i2s_sd_rx_pin, priv->i2s_mck_pin);

	/* Initialize buffering */
	i2s_set_dma_buffer(priv->i2s_object, priv->i2s_tx_buffer, priv->i2s_rx_buffer);

	/* configures IRQ */
	priv->rx_isr_handler = &i2s_transfer_rx_handleirq;
	priv->tx_isr_handler = &i2s_transfer_tx_handleirq;
	ret = amebad_i2s_isr_initialize(priv);
	if (ret != OK) {
		i2serr("I2S initialize: isr fails\n");
		goto errout_with_alloc;
	}

	/* Initialize the I2S priv device structure  */
	sem_init(&priv->exclsem, 0, 1);
	priv->dev.ops = &g_i2sops;

	/* Basic settings */
	priv->i2s_num = priv->i2s_object->i2s_idx;

	g_i2sdevice[port] = priv;

	i2s_disable(priv->i2s_object);

	/* Success exit */
	return &priv->dev;

	/* Failure exits */
errout_with_alloc:
errout_with_clocking:
	sem_destroy(&priv->exclsem);
	amebad_i2s_deinit_buffer(priv);
	kmm_free(priv);
	return NULL;
}
#endif							/* I2S_HAVE_RX || I2S_HAVE_TX */
