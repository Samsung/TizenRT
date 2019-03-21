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

#ifndef __IMXRT_I2S_H
#define __IMXRT_I2S_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>
#include <tinyara/audio/i2s.h>

#include <arch/chip/irq.h>

#include <chip/imxrt105x_config.h>

#include "imxrt_config.h"

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/
/* SAI module features */

/* @brief Receive/transmit FIFO size in item count (register bit fields TCSR[FRDE], TCSR[FRIE], TCSR[FRF], TCR1[TFW], RCSR[FRDE], RCSR[FRIE], RCSR[FRF], RCR1[RFW], registers TFRn, RFRn). */
#define FSL_FEATURE_SAI_FIFO_COUNT (32)
/* @brief Receive/transmit channel number (register bit fields TCR3[TCE], RCR3[RCE], registers TDRn and RDRn). */
#define FSL_FEATURE_SAI_CHANNEL_COUNT (4)
/* @brief Maximum words per frame (register bit fields TCR3[WDFL], TCR4[FRSZ], TMR[TWM], RCR3[WDFL], RCR4[FRSZ], RMR[RWM]). */
#define FSL_FEATURE_SAI_MAX_WORDS_PER_FRAME (32)
/* @brief Has support of combining multiple data channel FIFOs into single channel FIFO (register bit fields TCR3[CFR], TCR4[FCOMB], TFR0[WCP], TFR1[WCP], RCR3[CFR], RCR4[FCOMB], RFR0[RCP], RFR1[RCP]). */
#define FSL_FEATURE_SAI_HAS_FIFO_COMBINE_MODE (1)
/* @brief Has packing of 8-bit and 16-bit data into each 32-bit FIFO word (register bit fields TCR4[FPACK], RCR4[FPACK]). */
#define FSL_FEATURE_SAI_HAS_FIFO_PACKING (1)
/* @brief Configures when the SAI will continue transmitting after a FIFO error has been detected (register bit fields TCR4[FCONT], RCR4[FCONT]). */
#define FSL_FEATURE_SAI_HAS_FIFO_FUNCTION_AFTER_ERROR (1)
/* @brief Configures if the frame sync is generated internally, a frame sync is only generated when the FIFO warning flag is clear or continuously (register bit fields TCR4[ONDEM], RCR4[ONDEM]). */
#define FSL_FEATURE_SAI_HAS_ON_DEMAND_MODE (1)
/* @brief Simplified bit clock source and asynchronous/synchronous mode selection (register bit fields TCR2[CLKMODE], RCR2[CLKMODE]), in comparison with the exclusively implemented TCR2[SYNC,BCS,BCI,MSEL], RCR2[SYNC,BCS,BCI,MSEL]. */
#define FSL_FEATURE_SAI_HAS_CLOCKING_MODE (0)
/* @brief Has register for configuration of the MCLK divide ratio (register bit fields MDR[FRACT], MDR[DIVIDE]). */
#define FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER (0)
/* @brief Interrupt source number */
#define FSL_FEATURE_SAI_INT_SOURCE_NUM (2)
/* @brief Has register of MCR. */
#define FSL_FEATURE_SAI_HAS_MCR (0)
/* @brief Has register of MDR */
#define FSL_FEATURE_SAI_HAS_MDR (0)

/************************************************************************************
 * Public Types
 ************************************************************************************/
/*! @brief SAI return status*/
enum _sai_status_t {
    kStatus_SAI_TxBusy = MAKE_STATUS(19, 0),    /*!< SAI Tx is busy. */
    kStatus_SAI_RxBusy = MAKE_STATUS(19, 1),    /*!< SAI Rx is busy. */
    kStatus_SAI_TxError = MAKE_STATUS(19, 2),   /*!< SAI Tx FIFO error. */
    kStatus_SAI_RxError = MAKE_STATUS(19, 3),   /*!< SAI Rx FIFO error. */
    kStatus_SAI_QueueFull = MAKE_STATUS(19, 4), /*!< SAI transfer queue is full. */
    kStatus_SAI_TxIdle = MAKE_STATUS(19, 5),    /*!< SAI Tx is idle */
    kStatus_SAI_RxIdle = MAKE_STATUS(19, 6)     /*!< SAI Rx is idle */
};

/*! @brief Define the SAI bus type */
typedef enum _sai_protocol {
    kSAI_BusLeftJustified = 0x0U, /*!< Uses left justified format.*/
    kSAI_BusRightJustified,       /*!< Uses right justified format. */
    kSAI_BusI2S,                  /*!< Uses I2S format. */
    kSAI_BusPCMA,                 /*!< Uses I2S PCM A format.*/
    kSAI_BusPCMB                  /*!< Uses I2S PCM B format. */
} sai_protocol_t;

/*! @brief Master or slave mode */
typedef enum _sai_master_slave {
    kSAI_Master = 0x0U, /*!< Master mode */
    kSAI_Slave = 0x1U   /*!< Slave mode */
} sai_master_slave_t;

/*! @brief Mono or stereo audio format */
typedef enum _sai_mono_stereo {
    kSAI_Stereo = 0x0U, /*!< Stereo sound. */
    kSAI_MonoRight,     /*!< Only Right channel have sound. */
    kSAI_MonoLeft       /*!< Only left channel have sound. */
} sai_mono_stereo_t;

/*! @brief Synchronous or asynchronous mode */
typedef enum _sai_sync_mode {
    kSAI_ModeAsync = 0x0U,    /*!< Asynchronous mode */
    kSAI_ModeSync,            /*!< Synchronous mode (with receiver or transmit) */
    kSAI_ModeSyncWithOtherTx, /*!< Synchronous with another SAI transmit */
    kSAI_ModeSyncWithOtherRx  /*!< Synchronous with another SAI receiver */
} sai_sync_mode_t;

/*! @brief Mater clock source */
typedef enum _sai_mclk_source {
    kSAI_MclkSourceSysclk = 0x0U, /*!< Master clock from the system clock */
    kSAI_MclkSourceSelect1,       /*!< Master clock from source 1 */
    kSAI_MclkSourceSelect2,       /*!< Master clock from source 2 */
    kSAI_MclkSourceSelect3        /*!< Master clock from source 3 */
} sai_mclk_source_t;

/*! @brief Bit clock source */
typedef enum _sai_bclk_source {
    kSAI_BclkSourceBusclk = 0x0U, /*!< Bit clock using bus clock */
    kSAI_BclkSourceMclkDiv,       /*!< Bit clock using master clock divider */
    kSAI_BclkSourceOtherSai0,     /*!< Bit clock from other SAI device  */
    kSAI_BclkSourceOtherSai1      /*!< Bit clock from other SAI device */
} sai_bclk_source_t;

/*! @brief The SAI interrupt enable flag */
enum _sai_interrupt_enable_t {
    kSAI_WordStartInterruptEnable =
        I2S_TCSR_WSIE_MASK, /*!< Word start flag, means the first word in a frame detected */
    kSAI_SyncErrorInterruptEnable = I2S_TCSR_SEIE_MASK,   /*!< Sync error flag, means the sync error is detected */
    kSAI_FIFOWarningInterruptEnable = I2S_TCSR_FWIE_MASK, /*!< FIFO warning flag, means the FIFO is empty */
    kSAI_FIFOErrorInterruptEnable = I2S_TCSR_FEIE_MASK,   /*!< FIFO error flag */
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    kSAI_FIFORequestInterruptEnable = I2S_TCSR_FRIE_MASK, /*!< FIFO request, means reached watermark */
#endif                                                    /* FSL_FEATURE_SAI_FIFO_COUNT */
};

/*! @brief The DMA request sources */
enum _sai_dma_enable_t {
    kSAI_FIFOWarningDMAEnable = I2S_TCSR_FWDE_MASK, /*!< FIFO warning caused by the DMA request */
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    kSAI_FIFORequestDMAEnable = I2S_TCSR_FRDE_MASK, /*!< FIFO request caused by the DMA request */
#endif                                              /* FSL_FEATURE_SAI_FIFO_COUNT */
};

/*! @brief The SAI status flag */
enum _sai_flags {
    kSAI_WordStartFlag = I2S_TCSR_WSF_MASK, /*!< Word start flag, means the first word in a frame detected */
    kSAI_SyncErrorFlag = I2S_TCSR_SEF_MASK, /*!< Sync error flag, means the sync error is detected */
    kSAI_FIFOErrorFlag = I2S_TCSR_FEF_MASK, /*!< FIFO error flag */
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    kSAI_FIFORequestFlag = I2S_TCSR_FRF_MASK, /*!< FIFO request flag. */
#endif                                        /* FSL_FEATURE_SAI_FIFO_COUNT */
    kSAI_FIFOWarningFlag = I2S_TCSR_FWF_MASK, /*!< FIFO warning flag */
};

/*! @brief The reset type */
typedef enum _sai_reset_type {
    kSAI_ResetTypeSoftware = I2S_TCSR_SR_MASK,          /*!< Software reset, reset the logic state */
    kSAI_ResetTypeFIFO = I2S_TCSR_FR_MASK,              /*!< FIFO reset, reset the FIFO read and write pointer */
    kSAI_ResetAll = I2S_TCSR_SR_MASK | I2S_TCSR_FR_MASK /*!< All reset. */
} sai_reset_type_t;

#if defined(FSL_FEATURE_SAI_HAS_FIFO_PACKING) && FSL_FEATURE_SAI_HAS_FIFO_PACKING
/*!
 * @brief The SAI packing mode
 * The mode includes 8 bit and 16 bit packing.
 */
typedef enum _sai_fifo_packing {
    kSAI_FifoPackingDisabled = 0x0U, /*!< Packing disabled */
    kSAI_FifoPacking8bit = 0x2U,     /*!< 8 bit packing enabled */
    kSAI_FifoPacking16bit = 0x3U     /*!< 16bit packing enabled */
} sai_fifo_packing_t;
#endif /* FSL_FEATURE_SAI_HAS_FIFO_PACKING */

/*! @brief SAI user configuration structure */
typedef struct _sai_config {
    sai_protocol_t protocol;  /*!< Audio bus protocol in SAI */
    sai_sync_mode_t syncMode; /*!< SAI sync mode, control Tx/Rx clock sync */
#if defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)
    bool mclkOutputEnable;          /*!< Master clock output enable, true means master clock divider enabled */
#endif                              /* FSL_FEATURE_SAI_HAS_MCR */
    sai_mclk_source_t mclkSource;   /*!< Master Clock source */
    sai_bclk_source_t bclkSource;   /*!< Bit Clock source */
    sai_master_slave_t masterSlave; /*!< Master or slave */
} sai_config_t;

/*!@brief SAI transfer queue size, user can refine it according to use case. */
#define SAI_XFER_QUEUE_SIZE (30)

/*! @brief Audio sample rate */
typedef enum _sai_sample_rate {
    kSAI_SampleRate8KHz = 8000U,     /*!< Sample rate 8000 Hz */
    kSAI_SampleRate11025Hz = 11025U, /*!< Sample rate 11025 Hz */
    kSAI_SampleRate12KHz = 12000U,   /*!< Sample rate 12000 Hz */
    kSAI_SampleRate16KHz = 16000U,   /*!< Sample rate 16000 Hz */
    kSAI_SampleRate22050Hz = 22050U, /*!< Sample rate 22050 Hz */
    kSAI_SampleRate24KHz = 24000U,   /*!< Sample rate 24000 Hz */
    kSAI_SampleRate32KHz = 32000U,   /*!< Sample rate 32000 Hz */
    kSAI_SampleRate44100Hz = 44100U, /*!< Sample rate 44100 Hz */
    kSAI_SampleRate48KHz = 48000U,   /*!< Sample rate 48000 Hz */
    kSAI_SampleRate96KHz = 96000U    /*!< Sample rate 96000 Hz */
} sai_sample_rate_t;

/*! @brief Audio word width */
typedef enum _sai_word_width {
    kSAI_WordWidth8bits = 8U,   /*!< Audio data width 8 bits */
    kSAI_WordWidth16bits = 16U, /*!< Audio data width 16 bits */
    kSAI_WordWidth24bits = 24U, /*!< Audio data width 24 bits */
    kSAI_WordWidth32bits = 32U  /*!< Audio data width 32 bits */
} sai_word_width_t;

/*! @brief sai transfer format */
typedef struct _sai_transfer_format {
    uint32_t sampleRate_Hz;   /*!< Sample rate of audio data */
    uint32_t bitWidth;        /*!< Data length of audio data, usually 8/16/24/32 bits */
    sai_mono_stereo_t stereo; /*!< Mono or stereo */
    uint32_t masterClockHz;   /*!< Master clock frequency in Hz */
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    uint8_t watermark;       /*!< Watermark value */
#endif                       /* FSL_FEATURE_SAI_FIFO_COUNT */
    uint8_t channel;         /*!< Data channel used in transfer.*/
    sai_protocol_t protocol; /*!< Which audio protocol used */
    bool isFrameSyncCompact; /*!< True means Frame sync length is configurable according to bitWidth, false means frame
                                sync length is 64 times of bit clock. */
} sai_transfer_format_t;

/*! @brief SAI transfer structure */
typedef struct _sai_transfer {
    uint8_t *data;   /*!< Data start address to transfer. */
    size_t dataSize; /*!< Transfer size. */
} sai_transfer_t;

/* I2S buffer container */
typedef struct imxrt_buffer_s {
	void *dev_ptr;              /* The pointer for dev */
	i2s_callback_t callback;	/* Function to call when the transfer completes */
	void *arg;					/* The argument to be returned with the callback */
	uint32_t timeout;			/* The timeout value to use with DMA transfers */
	struct ap_buffer_s *apb;	/* The audio buffer */
	int result;					/* The result of the transfer */
} imxrt_buffer_t;

typedef struct _sai_handle sai_handle_t;

/*! @brief SAI transfer callback prototype */
typedef void (*sai_transfer_callback_t)(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData);

/*! @brief SAI handle structure */
struct _sai_handle {
    uint32_t state;                                    /*!< Transfer status */
	uint32_t timeout;                                  /*!< Timeout value */
    uint8_t bitWidth;                                  /*!< Bit width for transfer, 8/16/24/32 bits */
    uint8_t channel;                                   /*!< Transfer channel */
    sai_transfer_t saiQueue[SAI_XFER_QUEUE_SIZE];      /*!< Transfer queue storing queued transfer */
	imxrt_buffer_t callbackParam[SAI_XFER_QUEUE_SIZE]; /*!< Callback Parameter need to transfer */
    size_t transferSize[SAI_XFER_QUEUE_SIZE];          /*!< Data bytes need to transfer */
    volatile uint8_t queueUser;                        /*!< Index for user to queue transfer */
    volatile uint8_t queueDriver;                      /*!< Index for driver to get the transfer data and size */
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    uint8_t watermark; /*!< Watermark value */
#endif
};

/************************************************************************************
 * Inline Functions
 ************************************************************************************/

/************************************************************************************
 * Public Data
 ************************************************************************************/
/**
* @brief I2S bit width per sample.
*
*/
typedef enum {
	I2S_BITS_PER_SAMPLE_8BIT = 8,	/*!< I2S bits per sample: 8-bits */
	I2S_BITS_PER_SAMPLE_16BIT = 16,	/*!< I2S bits per sample: 16-bits */
	I2S_BITS_PER_SAMPLE_24BIT = 24,	/*!< I2S bits per sample: 24-bits */
	I2S_BITS_PER_SAMPLE_32BIT = 32,	/*!< I2S bits per sample: 32-bits */
} i2s_bits_per_sample_t;

/**
* @brief I2S channel.
*
*/
typedef enum {
	I2S_CHANNEL_MONO = 1,		/*!< I2S 1 channel (mono) */
	I2S_CHANNEL_STEREO = 2		/*!< I2S 2 channel (stereo) */
} i2s_channel_t;

/**
* @brief I2S communication standard format
*
*/
typedef enum {
	I2S_COMM_FORMAT_I2S = 0x01,	/*!< I2S communication format I2S */
	I2S_COMM_FORMAT_I2S_MSB = 0x02,	/*!< I2S format MSB */
	I2S_COMM_FORMAT_I2S_LSB = 0x04,	/*!< I2S format LSB */
	I2S_COMM_FORMAT_PCM = 0x08,	/*!< I2S communication format PCM */
	I2S_COMM_FORMAT_PCM_SHORT = 0x10,	/*!< PCM Short */
	I2S_COMM_FORMAT_PCM_LONG = 0x20,	/*!< PCM Long */
} i2s_comm_format_t;

/**
* @brief I2S channel format type
*/
typedef enum {
	I2S_CHANNEL_FMT_RIGHT_LEFT = 0x00,
	I2S_CHANNEL_FMT_ALL_RIGHT,
	I2S_CHANNEL_FMT_ALL_LEFT,
	I2S_CHANNEL_FMT_ONLY_RIGHT,
	I2S_CHANNEL_FMT_ONLY_LEFT,
} i2s_channel_fmt_t;

/**
* @brief I2S Peripheral, 0 & 1.
*
*/
typedef enum {
	I2S_NUM_0 = 0x0,			/*!< I2S 0 */
	I2S_NUM_1 = 0x1,			/*!< I2S 1 */
	I2S_NUM_2 = 0x2,            /*!< I2S 2 */
	I2S_NUM_MAX,
} i2s_port_t;

/**
* @brief I2S Mode, defaut is I2S_MODE_MASTER | I2S_MODE_TX
*
* @note PDM and built-in DAC functions are only supported on I2S0 for current ESP32 chip.
*
*/
typedef enum {
	I2S_MODE_MASTER = 1,
	I2S_MODE_SLAVE = 2,
	I2S_MODE_TX = 4,
	I2S_MODE_RX = 8,
	I2S_MODE_DAC_BUILT_IN = 16,	/*!< Output I2S data to built-in DAC, no matter the data format is 16bit or 32 bit, the DAC module will only take the 8bits from MSB */
	I2S_MODE_ADC_BUILT_IN = 32,	/*!< Input I2S data from built-in ADC, each data can be 12-bit width at most */
	I2S_MODE_PDM = 64,
} i2s_mode_t;

/************************************************************************************
 * Public Function Prototypes
 ************************************************************************************/

/****************************************************************************
 * Name: imxrt_i2s_initialize
 *
 * Description:
 *   Initialize the selected I2S port.
 *
 * Input Parameter:
 *   Port number (for hardware that has mutiple I2S interfaces)
 *
 * Returned Value:
 *   Valid I2S device structure reference on succcess; a NULL on failure
 *
 ****************************************************************************/

FAR struct i2s_dev_s *imxrt_i2s_initialize(uint16_t port);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* __ASSEMBLY__ */
#endif							/* __IMXRT_I2S_H */
