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
#ifndef _IMXRT_I2S_EDMA_H_
#define _IMXRT_I2S_EDMA_H_

#include <stddef.h>
#include <chip/imxrt105x_features.h>
#include <chip/imxrt105x_config.h>

#include "imxrt_config.h"

/*!
 * @addtogroup sai_edma
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
#define FSL_SAI_EDMA_DRIVER_VERSION (MAKE_VERSION(2, 1, 5)) /*!< Version 2.1.5 */
/*@}*/

typedef struct _sai_edma_handle sai_edma_handle_t;

/*! @brief SAI eDMA transfer callback function for finish and error */
typedef void (*sai_edma_callback_t)(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);

/*! @brief SAI DMA transfer handle, users should not touch the content of the handle.*/
struct _sai_edma_handle {
    edma_handle_t *dmaHandle;     /*!< DMA handler for SAI send */
    uint8_t nbytes;               /*!< eDMA minor byte transfer count initially configured. */
    uint8_t bytesPerFrame;        /*!< Bytes in a frame */
    uint8_t channel;              /*!< Which data channel */
    uint8_t count;                /*!< The transfer data count in a DMA request */
    uint32_t state;               /*!< Internal state for SAI eDMA transfer */
    sai_edma_callback_t callback; /*!< Callback for users while transfer finish or error occurs */
    void *userData;               /*!< User callback parameter */
    uint8_t tcd[(SAI_XFER_QUEUE_SIZE + 1U) * sizeof(edma_tcd_t)]; /*!< TCD pool for eDMA transfer. */
    sai_transfer_t saiQueue[SAI_XFER_QUEUE_SIZE];                 /*!< Transfer queue storing queued transfer. */
    size_t transferSize[SAI_XFER_QUEUE_SIZE];                     /*!< Data bytes need to transfer */
    volatile uint8_t queueUser;                                   /*!< Index for user to queue transfer. */
    volatile uint8_t queueDriver; /*!< Index for driver to get the transfer data and size */
};

/*******************************************************************************
 * APIs
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name eDMA Transactional
 * @{
 */

/****************************************************************************
 * Function: imxrt_i2s_transfertxcreatehandleedma
 *
 * Description:
 *   brief Initializes the SAI eDMA handle.
 * 
 *   This function initializes the SAI master DMA handle, which can be used for other SAI master transactional APIs.
 *   Usually, for a specified SAI instance, call this API once to get the initialized handle.
 * 
 * Input Parameters:
 *   base SAI base pointer.
 *   handle SAI eDMA handle pointer.
 *   base SAI peripheral base address.
 *   callback Pointer to user callback function.
 *   userData User parameter passed to the callback function.
 *   dmaHandle eDMA handle pointer, this handle shall be static allocated by users.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_i2s_transfertxcreatehandleedma(
    I2S_Type *base, sai_edma_handle_t *handle, sai_edma_callback_t callback, void *userData, edma_handle_t *dmaHandle);

/****************************************************************************
 * Function: imxrt_i2s_transfertxcreatehandleedma
 *
 * Description:
 *   brief Initializes the SAI Rx eDMA handle.
 * 
 *   This function initializes the SAI slave DMA handle, which can be used for other SAI master transactional APIs.
 *   Usually, for a specified SAI instance, call this API once to get the initialized handle.
 * 
 * Input Parameters:
 *   base SAI base pointer.
 *   handle SAI eDMA handle pointer.
 *   base SAI peripheral base address.
 *   callback Pointer to user callback function.
 *   userData User parameter passed to the callback function.
 *   dmaHandle eDMA handle pointer, this handle shall be static allocated by users.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_i2s_transferrxcreatehandleedma(
    I2S_Type *base, sai_edma_handle_t *handle, sai_edma_callback_t callback, void *userData, edma_handle_t *dmaHandle);

/****************************************************************************
 * Function: imxrt_i2s_transfertxsetformatedma
 *
 * Description:
 *   brief Configures the SAI Tx audio format.
 * 
 *   The audio format can be changed at run-time. This function configures the sample rate and audio data
 *   format to be transferred. This function also sets the eDMA parameter according to formatting requirements.
 * 
 * Input Parameters:
 *   base SAI base pointer.
 *   handle SAI eDMA handle pointer.
 *   format Pointer to SAI audio data format structure.
 *   mclkSourceClockHz SAI master clock source frequency in Hz.
 *   bclkSourceClockHz SAI bit clock source frequency in Hz. If bit clock source is master
 *     clock, this value should equals to masterClockHz in format.
 *
 * Returned Value:
 *   Returns kStatus_Success Audio format set successfully.
 *   Returns kStatus_InvalidArgument The input argument is invalid.
 *
 ****************************************************************************/
void imxrt_i2s_transfertxsetformatedma(I2S_Type *base,
                                 sai_edma_handle_t *handle,
                                 sai_transfer_format_t *format,
                                 uint32_t mclkSourceClockHz,
                                 uint32_t bclkSourceClockHz);

/****************************************************************************
 * Function: imxrt_i2s_transferrxsetformatedma
 *
 * Description:
 *   brief Configures the SAI Rx audio format.
 * 
 *   The audio format can be changed at run-time. This function configures the sample rate and audio data
 *   format to be transferred. This function also sets the eDMA parameter according to formatting requirements.
 * 
 * Input Parameters:
 *   base SAI base pointer.
 *   handle SAI eDMA handle pointer.
 *   format Pointer to SAI audio data format structure.
 *   mclkSourceClockHz SAI master clock source frequency in Hz.
 *   bclkSourceClockHz SAI bit clock source frequency in Hz. If bit clock source is master
 *     clock, this value should equals to masterClockHz in format.
 *
 * Returned Value:
 *   Returns kStatus_Success Audio format set successfully.
 *   Returns kStatus_InvalidArgument The input argument is invalid.
 *
 ****************************************************************************/
void imxrt_i2s_transferrxsetformatedma(I2S_Type *base,
                                 sai_edma_handle_t *handle,
                                 sai_transfer_format_t *format,
                                 uint32_t mclkSourceClockHz,
                                 uint32_t bclkSourceClockHz);

/****************************************************************************
 * Function: imxrt_i2s_transfersendedma
 *
 * Description:
 *   brief Performs a non-blocking SAI transfer using DMA.
 * 
 *   @note This interface returns immediately after the transfer initiates. Call
 *   SAI_GetTransferStatus to poll the transfer status and check whether the SAI transfer is finished.
 * 
 * Input Parameters:
 *   base SAI base pointer.
 *   handle SAI eDMA handle pointer.
 *   xfer Pointer to the DMA transfer structure.
 *
 * Returned Value:
 *   Returns kStatus_Success Start a SAI eDMA send successfully.
 *   Returns kStatus_InvalidArgument The input argument is invalid.
 *   Returns kStatus_TxBusy SAI is busy sending data.
 *
 ****************************************************************************/
status_t imxrt_i2s_transfersendedma(I2S_Type *base, sai_edma_handle_t *handle, sai_transfer_t *xfer);

/****************************************************************************
 * Function: imxrt_i2s_transferreceiveedma
 *
 * Description:
 *   brief Performs a non-blocking SAI receive using eDMA.
 * 
 *   @note This interface returns immediately after the transfer initiates. Call
 *   the SAI_GetReceiveRemainingBytes to poll the transfer status and check whether the SAI transfer is finished.
 * 
 * Input Parameters:
 *   base SAI base pointer.
 *   handle SAI eDMA handle pointer.
 *   xfer Pointer to the DMA transfer structure.
 *
 * Returned Value:
 *   Returns kStatus_Success Start a SAI eDMA send successfully.
 *   Returns kStatus_InvalidArgument The input argument is invalid.
 *   Returns kStatus_RxBusy SAI is busy receiving data.
 *
 ****************************************************************************/
status_t imxrt_i2s_transferreceiveedma(I2S_Type *base, sai_edma_handle_t *handle, sai_transfer_t *xfer);

/****************************************************************************
 * Function: imxrt_i2s_transferterminatesendedma
 *
 * Description:
 *   brief Terminate all SAI send.
 * 
 *   This function will clear all transfer slots buffered in the sai queue. If users only want to abort the
 *   current transfer slot, please call SAI_TransferAbortSendEDMA.
 * 
 * Input Parameters:
 *   base SAI base pointer.
 *   handle SAI eDMA handle pointer.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_i2s_transferterminatesendedma(I2S_Type *base, sai_edma_handle_t *handle);

/****************************************************************************
 * Function: imxrt_i2s_transferterminatereceiveedma
 *
 * Description:
 *   brief Terminate all SAI receive.
 * 
 *   This function will clear all transfer slots buffered in the sai queue. If users only want to abort the
 *   current transfer slot, please call SAI_TransferAbortReceiveEDMA.
 * 
 * Input Parameters:
 *   base SAI base pointer.
 *   handle SAI eDMA handle pointer.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_i2s_transferterminatereceiveedma(I2S_Type *base, sai_edma_handle_t *handle);

/****************************************************************************
 * Function: imxrt_i2s_transferabortsendedma
 *
 * Description:
 *   brief Aborts a SAI transfer using eDMA.
 * 
 *   This function only aborts the current transfer slots, the other transfer slots' information still kept
 *   in the handler. If users want to terminate all transfer slots, just call SAI_TransferTerminateSendEDMA.
 * 
 * Input Parameters:
 *   base SAI base pointer.
 *   handle SAI eDMA handle pointer.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_i2s_transferabortsendedma(I2S_Type *base, sai_edma_handle_t *handle);

/****************************************************************************
 * Function: imxrt_i2s_transferabortreceiveedma
 *
 * Description:
 *   brief Aborts a SAI receive using eDMA.
 * 
 *   This function only aborts the current transfer slots, the other transfer slots' information still kept
 *   in the handler. If users want to terminate all transfer slots, just call SAI_TransferTerminateReceiveEDMA.
 * 
 * Input Parameters:
 *   base SAI base pointer.
 *   handle SAI eDMA handle pointer.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_i2s_transferabortreceiveedma(I2S_Type *base, sai_edma_handle_t *handle);

/****************************************************************************
 * Function: imxrt_i2s_transfergetsendcountedma
 *
 * Description:
 *   brief Gets byte count sent by SAI.
 * 
 * Input Parameters:
 *   base SAI base pointer.
 *   handle SAI eDMA handle pointer.
 *   count Bytes count sent by SAI.
 *
 * Returned Value:
 *   Returns kStatus_Success Succeed get the transfer count.
 *   Returns kStatus_NoTransferInProgress There is no non-blocking transaction in progress.
 *
 ****************************************************************************/
status_t imxrt_i2s_transfergetsendcountedma(I2S_Type *base, sai_edma_handle_t *handle, size_t *count);

/****************************************************************************
 * Function: imxrt_i2s_transfergetreceivecountedma
 *
 * Description:
 *   brief Gets byte count received by SAI.
 * 
 * Input Parameters:
 *   base SAI base pointer.
 *   handle SAI eDMA handle pointer.
 *   count Bytes count received by SAI.
 *
 * Returned Value:
 *   Returns kStatus_Success Succeed get the transfer count.
 *   Returns kStatus_NoTransferInProgress There is no non-blocking transaction in progress.
 *
 ****************************************************************************/
status_t imxrt_i2s_transfergetreceivecountedma(I2S_Type *base, sai_edma_handle_t *handle, size_t *count);

/*! @} */

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif
