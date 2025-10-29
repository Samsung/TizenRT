/***************************************************************************//**
 * @file    iot_adc.c
 * @brief   Beken Corp implementation of Common I/O ADC API.
 *******************************************************************************
 * # License
 * <b>Copyright 2020-2025 Beken Corp Inc. </b>
 *******************************************************************************
 *  Licensed under the Apache License, Version 2.0 (the "License");           
 *  you may not use this file except in compliance with the License.            
 *  You may obtain a copy of the License at                                     
 *                                                                              
 *      http://www.apache.org/licenses/LICENSE-2.0                              
 *                                                                              
 *  Unless required by applicable law or agreed to in writing, software         
 *  distributed under the License is distributed on an "AS IS" BASIS,         
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.    
 *  See the License for the specific language governing permissions and         
 *  limitations under the License. 

 ******************************************************************************/

/*******************************************************************************
 *                               INCLUDES
 ******************************************************************************/

/* FreeRTOS kernel layer */
#include "FreeRTOS.h"
#include "semphr.h"

/* ADC driver layer */
#include "driver/hal/hal_adc_types.h"
#include "adc_driver.h"

/* ADC iot layer */
#include "iot_adc.h"

/*******************************************************************************
 *                              DESCRIPTOR
 ******************************************************************************/

typedef struct IotAdcDescriptor {
  /**************************************/
  /*            ADC CONFIG              */
  /**************************************/

  /* instance number */
  int32_t                     lInstNum;

  /* IotAdcConfig */
  IotAdcConfig_t              xIotAdcCfg;

  /**************************************/
  /*            ADC DRIVER              */
  /**************************************/
  /* adc configure */
  adc_config_t                *pxAdcCfg;

  /**************************************/
  /*             ADC HAL                */
  /**************************************/

  /* busy status */
  uint8_t                     ucBusyStatus;
  uint8_t                     ucBusyChannel;

  /* callback info */
  void                       *pvCallback;
  void                       *pvContext;

  /* buffer info */
  uint16_t                   *pusBufPtr;
  uint8_t                     ucBufMax;
  uint8_t                     ucBufCtr;
} IotAdcDescriptor_t;

IotAdcDescriptor_t g_IotAdcDescriptor;

/*******************************************************************************
 *                             iot_adc_open()
 ******************************************************************************/

/**
 * @brief Initializes ADC controller with default configuration.
 * init ADC controller, enable ADC clock, reset HW FIFO, set default
 * configuration parameters, etc. Also allocate all required resources
 * for ADC operation such as software data buffer etc.
 *
 * @warning Must be called prior to any other ADC api's so that a valid handle is obtained.
 * @warning Once opened, the same ADC instance must be closed before calling open again.
 *
 * @param[in] lAdc The instance of ADC controller to initialize.
 *
 * @return
 * - handle to the ADC controller on success
 * - NULL, if
 *    - invalid instance number
 *    - open same instance more than once before closing it.
 */
IotAdcHandle_t iot_adc_open(int32_t lAdc)
{
  /* local variables */
  IotAdcHandle_t    xAdcHandle   = &g_IotAdcDescriptor;

  /* xAdcHandle instance is valid? */
  if (BK_OK != bk_adc_is_valid_ch(lAdc)) {
    return NULL;
  }

  /* retrieve xAdcHandle by instance number */
  bk_adc_get_config(lAdc, &xAdcHandle->pxAdcCfg);

  if (xAdcHandle->pxAdcCfg->chan != lAdc) 
  {
     return NULL;
  }

  /* selected adc shouldn't be open */
  if (xAdcHandle->pxAdcCfg->is_open == pdTRUE) {
     return NULL;
  }

  /* initialize ADC driver */
  bk_adc_acquire();
  bk_adc_init(xAdcHandle->pxAdcCfg->chan);
  bk_adc_set_config(xAdcHandle->pxAdcCfg);

  /* enter critical section */
  portENTER_CRITICAL();

  /* reset ADC status */
  xAdcHandle->ucBusyStatus   = pdFALSE;
  xAdcHandle->ucBusyChannel  = 0;

  /* reset ADC callback config */
  xAdcHandle->pvCallback     = NULL;
  xAdcHandle->pvContext      = NULL;

  /* reset ADC buffer config */
  xAdcHandle->pusBufPtr      = NULL;
  xAdcHandle->ucBufMax       = 0;
  xAdcHandle->ucBufCtr       = 0;

  /* finally mark ADC as open */
  xAdcHandle->pxAdcCfg->is_open = pdTRUE;

  /* exit critical section */
  portEXIT_CRITICAL();

  /* done */
  return xAdcHandle;
}

/*******************************************************************************
 *                             iot_adc_close()
 ******************************************************************************/

/**
 * @brief Close ADC controller.
 * All pending operation will be cancelled, put ADC module in reset state or low
 * power state if possible. Release all resources claimed during open call
 *
 * @param[in] pxAdc handle to ADC controller returned from iot_adc_open() call.
 *
 * @return
 * - IOT_ADC_SUCCESS, on success;
 * - IOT_ADC_INVALID_VALUE, if pxAdc is NULL
 * - IOT_ADC_NOT_OPEN, if is not in open state (already closed).
 */
int32_t iot_adc_close(IotAdcHandle_t const pxAdc)
{
  /* pxAdc can't be null */
  if (pxAdc == NULL) {
    return IOT_ADC_INVALID_VALUE;
  }

  /* enter critical section */
  portENTER_CRITICAL();

  /* pxAdc must be already open */
  if (pxAdc->pxAdcCfg->is_open == pdFALSE) {
    portEXIT_CRITICAL();
    return IOT_ADC_NOT_OPEN;
  }

  /* any pending operation? */
  if (pxAdc->ucBusyStatus == pdTRUE) {
    bk_adc_stop();

    /* update status */
    pxAdc->ucBusyStatus = pdFALSE;
  }

  /* exit critical section */
  portEXIT_CRITICAL();

  bk_adc_deinit(pxAdc->pxAdcCfg->chan);
  bk_adc_release();

  /* enter critical section again */
  portENTER_CRITICAL();

  /* reset ADC status */
  pxAdc->ucBusyStatus    = pdFALSE;
  pxAdc->ucBusyChannel   = 0;

  /* reset ADC callback config */
  pxAdc->pvCallback      = NULL;
  pxAdc->pvContext       = NULL;

  bk_adc_unregister_isr_iot_callback();

  /* reset ADC buffer config */
  pxAdc->pusBufPtr       = NULL;
  pxAdc->ucBufMax        = 0;
  pxAdc->ucBufCtr        = 0;

  /* mark as closed */
  pxAdc->pxAdcCfg->is_open        = pdFALSE;

  /* exit critical section */
  portEXIT_CRITICAL();

  /* done */
  return IOT_ADC_SUCCESS;
}

/*******************************************************************************
 *                           iot_adc_set_callback()
 ******************************************************************************/

/**
 * @brief Sets channel callback on availability of channel scan data.
 * On availability of ADC scan data, the application is notified with a
 * function callback. The callback function and user context for callback
 * are set using iot_adc_set_callback.
 *
 * @note This callback is per AdcChannel for each handle.
 * @note If input handle or AdcChannel is invalid, or if callback function is NULL,
 *       this function silently takes no action.
 *
 * @param[in] pxAdc The Adc handle returned in the open() call.
 * @param[in] ucAdcChannel The Adc channel for which the callback is set
 * @param[in] pvCallback The callback function to be called on availability of ADC channel data.
 * @param[in] pvUserContext The user context to be passed when callback is called.
 *
 * <b>Example Callback Function</b>
 * For asychronous ADC calls, a callback function is used to signal when the async
 * task is complete.  This example uses a Semaphore to signal the completion.
 * @code{c}
 * static void prvAdcChCallback( uint16_t * pusConvertedData,
 *                               void * pvUserContext )
 * {
 *     BaseType_t xHigherPriorityTaskWoken;
 *     xSemaphoreGiveFromISR( xIotAdcSemaphore, &xHigherPriorityTaskWoken );
 *     portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
 * }
 * @endcode
 */
void iot_adc_set_callback(IotAdcHandle_t const pxAdc,
                          uint8_t ucAdcChannel,
                          IotAdcCallback_t pvCallback,
                          void * pvUserContext)
{
  /* we only support one callback function */
  (void) ucAdcChannel;

  /* pxAdc can't be null */
  if (pxAdc == NULL) {
    return;
  }

  /* pvCallback can't be null */
  if (pvCallback == NULL) {
    return;
  }

  /* enter critical section */
  portENTER_CRITICAL();

  /* pxAdc must be already open */
  if (pxAdc->pxAdcCfg->is_open == pdFALSE) {
    portEXIT_CRITICAL();
    return;
  }

  /* update callback info */
  pxAdc->pvCallback = pvCallback;
  pxAdc->pvContext = pvUserContext;

  bk_adc_register_isr_iot_callback((void *)pvCallback, pvUserContext);

  /* exit critical section */
  portEXIT_CRITICAL();
}

/*******************************************************************************
 *                              iot_adc_start()
 ******************************************************************************/

/**
 * @brief Start data acquisition for ADC channel until iot_adc_stop API is called.
 *        data will be passed back to client using callback function.
 *        by default each callback will pass back one data sample, however if client has
 *        used ioctl to pass in data buffer, only when buffer is full will callback
 *        be triggered
 *
 * @warning iot_adc_set_callback() must be called prior to this in order to get notification
 *          when adc scan is complete and data is available
 *
 * @note iot_adc_set_callback() must be called prior to iot_adc_start().
 *
 * @param[in] pxAdc. The ADC handle returned in the open() call
 * @param[in] ucAdcChannel. The ADC channel to start data acquisition
 *
 * @return
 * - IOT_ADC_SUCCESS on success
 * - IOT_ADC_INVALID_VALUE, on NULL handle or invalid AdcChannel
 * - IOT_ADC_NOT_OPEN if ADC has not been opened yet.
 * - IOT_ADC_CH_BUSY if ADC operation has started but is not complete
 * - IOT_ADC_FAILED if not callback function has been set.
 * <b>Example Asynchronous read</b>
 * @code{c}
 * // ADC Instance to open
 * int32_t lAdcInstance = 0;
 *
 * // Return value of ADC functions
 * int32_t lRetVal;
 *
 * // ADC Channel to read
 * int32_t lAdcChannel = 0;
 *
 * // User/Driver context; if necessary
 * void xUserContext = NULL;
 *
 * // Open the ADC instance and get a handle.
 * xAdcHandle = iot_adc_open( lAdcInstance );
 * // assert(lRetVal == IOT_ADC_SUCCESS);
 *
 * // set the callback function
 * iot_adc_set_callback( xAdcHandle, lAdcChannel, prvAdcChCallback, &xUserCntx );
 *
 * // start channel data scan on channel
 * lRetVal = iot_adc_start( xAdcHandle, lAdcChannel );
 *  // assert( IOT_ADC_SUCCESS == lRetVal );
 *
 *  // wait for the Adc operation to complete
 *  lRetVal = xSemaphoreTake( xIotAdcSemaphore, lIotAdcChWaitTime );
 *  // assert ( pdTRUE == lRetVal );
 *
 *  // stop channel data scan
 *  lRetVal = iot_adc_stop( xAdcHandle, lAdcChannel );
 *  assert( IOT_ADC_SUCCESS == lRetVal );
 *
 *  // close ADC module
 *  lRetVal = iot_adc_close( xAdcHandle );
 *  // assert( IOT_ADC_SUCCESS == lRetVal );
 * @endcode
 */
int32_t iot_adc_start(IotAdcHandle_t const pxAdc,
                      uint8_t ucAdcChannel)
{
  /* pxAdc can't be null */
  if (pxAdc == NULL) {
    return IOT_ADC_INVALID_VALUE;
  }

  /* ucAdcChannel valid? */
  if (BK_OK != bk_adc_is_valid_ch(ucAdcChannel)) {
    return IOT_ADC_INVALID_VALUE;
  }

  /* enter critical section */
  portENTER_CRITICAL();

  /* pxAdc must be already open */
  if (pxAdc->pxAdcCfg->is_open == pdFALSE) {
    portEXIT_CRITICAL();
    return IOT_ADC_NOT_OPEN;
  }

  /* pxAdc is busy? */
  #if 0
  if (pxAdc->ucBusyStatus == pdTRUE) {
    portEXIT_CRITICAL();
    return IOT_ADC_CH_BUSY;
  }
  #endif

  /* no callback function set? */
  if (pxAdc->pvCallback == NULL) {
    portEXIT_CRITICAL();
    return IOT_ADC_FAILED;
  }

  if(pxAdc->pxAdcCfg->chan != ucAdcChannel)
  {
      bk_adc_stop();
      /* mark current ADC channel as close */
      pxAdc->pxAdcCfg->is_open = pdFALSE;
      
      bk_adc_get_config(ucAdcChannel, &pxAdc->pxAdcCfg);
      bk_adc_init(pxAdc->pxAdcCfg->chan);
      bk_adc_set_config(pxAdc->pxAdcCfg);

      /*mark switched ADC channel as open */
      pxAdc->pxAdcCfg->is_open = pdTRUE;
  }

  ADC_LOGI("iot_adc_start:ch:%d\n",ucAdcChannel);

  /* update status */
  pxAdc->ucBusyStatus = pdTRUE;

  /* store active channel */
  pxAdc->ucBusyChannel = ucAdcChannel;

  /* exit critical section */
  portEXIT_CRITICAL();

  /* allocate analog bus */
  bk_adc_enable_bypass_clalibration();
  ADC_LOGI("bk_adc_enable_bypass_clalibration\n");
  bk_adc_start();
  ADC_LOGI("bk_adc_start\n");
  bk_adc_en();
  ADC_LOGI("bk_adc_en\n");
  /* done */
  return IOT_ADC_SUCCESS;
}

/*******************************************************************************
 *                             iot_adc_stop()
 ******************************************************************************/

/**
 * @brief Stop data acquisition for ADC channel
 *
 * @param[in] pxAdc. The ADC handle returned in the open() call
 * @param[in] ucAdcChannel. The ADC channel to stop data acquisition
 *
 * @return
 * - IOT_ADC_SCUCCESS on success
 * - IOT_ADC_INVALID_VALUE, on NULL handle or invalid AdcChannel
 * - IOT_ADC_NOT_OPEN if Adc has been closed without re-opening.
 */
int32_t iot_adc_stop(IotAdcHandle_t const pxAdc,
                     uint8_t ucAdcChannel)
{
  /* pxAdc can't be null */
  if (pxAdc == NULL) {
    return IOT_ADC_INVALID_VALUE;
  }

  /* ucAdcChannel valid? */
  if ((BK_OK != bk_adc_is_valid_ch(ucAdcChannel)) || (pxAdc->pxAdcCfg->chan != ucAdcChannel)) {
    return IOT_ADC_INVALID_VALUE;
  }

  /* enter critical section */
  portENTER_CRITICAL();

  /* pxAdc must be already open */
  if (pxAdc->pxAdcCfg->is_open == pdFALSE) {
    portEXIT_CRITICAL();
    return IOT_ADC_NOT_OPEN;
  }

  /* disable IRQs */
  bk_adc_stop();
  //bk_adc_deinit(ucAdcChannel);
  //bk_adc_release();

  /* update status */
  pxAdc->ucBusyStatus = pdFALSE;

  /* clear buffer info */
  pxAdc->pusBufPtr = NULL;
  pxAdc->ucBufMax = 0;
  pxAdc->ucBufCtr = 0;

  /* exit critical section */
  portEXIT_CRITICAL();

  /* done */
  return IOT_ADC_SUCCESS;
}

/*******************************************************************************
 *                           iot_adc_read_sample()
 ******************************************************************************/

/**
 * @brief read one ADC data sample. This API will return one ADC sample.
 *
 * @param[in] pxAdc. The ADC handle returned in the open() call
 * @param[in] ucAdcChannel. The ADC channel to read data from
 * @param[out] pusAdcSample. ADC channel read sample value
 *
 * @return
 * - IOT_ADC_SCUCCESS on success
 * - IOT_ADC_INVALID_VALUE, on NULL handle or invalid AdcChannel
 * - IOT_ADC_CH_BUSY if ADC operation not complete.
 * - IOT_ADC_NOT_OPEN if Adc has been closed without re-opening.
 * <b>Example Synchronous read</b>
 * @code{c}
 * // ADC Instance to open
 * int32_t lAdcInstance = 0;
 *
 * // ADC Channel to read
 * int32_t lAdcChannel = 0;
 *
 * // Declare and ADC handle
 * IotAdcHandle_t xAdcHandle;
 *
 * // Return value of ADC functions
 * int32_t lRetVal;
 *
 * // ADC value read
 * uint16_t usSample;
 *
 * // Open the ADC instance and get a handle.
 * xAdcHandle = iot_adc_open( lAdcInstance );
 * // assert(lRetVal == IOT_ADC_SUCCESS);
 *
 * // Read sample from ADC channel
 * lRetVal = iot_adc_read_sample( xAdcHandle, lAdcChannel, &usSample);
 * // assert(lRetVal == IOT_ADC_SUCCESS);
 *
 * // Close the ADC instance and get a handle.
 * lRetVal = iot_adc_close( xAdcHandle );
 * // assert(lRetVal == IOT_ADC_SUCCESS);
 * @endcode
 *
 */
int32_t iot_adc_read_sample(IotAdcHandle_t const pxAdc,
                            uint8_t ucAdcChannel,
                            uint16_t * pusAdcSample)
{
  /* pxAdc can't be null */
  if (pxAdc == NULL) {
    return IOT_ADC_INVALID_VALUE;
  }

  ADC_LOGI("desc_ch:%d,ch:%d \n",pxAdc->pxAdcCfg->chan, ucAdcChannel);

  /* ucAdcChannel valid? */
  if (BK_OK != bk_adc_is_valid_ch(ucAdcChannel)){
    return IOT_ADC_INVALID_VALUE;
  }

  /* pxAdc must be already open */
  if (pxAdc->pxAdcCfg->is_open == pdFALSE) {
    return IOT_ADC_NOT_OPEN;
  }

  if(pxAdc->pxAdcCfg->chan != ucAdcChannel)
  {
      bk_adc_stop();
      /* mark current ADC channel as close */
      pxAdc->pxAdcCfg->is_open = pdFALSE;
      
      bk_adc_get_config(ucAdcChannel, &pxAdc->pxAdcCfg);
      bk_adc_init(pxAdc->pxAdcCfg->chan);
      bk_adc_set_config(pxAdc->pxAdcCfg);

      /*mark switched ADC channel as open */
      pxAdc->pxAdcCfg->is_open = pdTRUE;
  }


  bk_adc_enable_bypass_clalibration();
  ADC_LOGI("bk_adc_enable_bypass_clalibration\n");
  bk_adc_start();
  ADC_LOGI("bk_adc_start\n");
  
  /* read samples */  
  if ((NULL == pxAdc->pusBufPtr) || (0 == pxAdc->ucBufMax)) 
  {
      ADC_LOGI("read sample no buf \n");
      bk_adc_read_raw(pusAdcSample,1,1000);
  }
  else
  {
      ADC_LOGI("read sample to buf \n");
      bk_adc_read_raw(pxAdc->pusBufPtr,pxAdc->ucBufMax,1000);
  }
  
  /* done */
  return IOT_ADC_SUCCESS;
}

/*******************************************************************************
 *                             iot_adc_ioctl()
 ******************************************************************************/

/**
 * @brief Used for various ADC control functions.
 *
 * @param[in] pxAdc The Adc handle returned in the open() call.
 * @param[in] xRequest ioctl request defined by IotAdcIoctlRequest_s enums
 * @param[in/out] pvBuffer data buffer for ioctl request
 * @param[in] pvBuffer size
 *
 * @return
 * - IOT_ADC_SCUCCESS on success
 * - IOT_ADC_INVALID_VALUE, on NULL handle or invalid request or NULL or invalid buffer pointer
 * - IOT_ADC_CH_BUSY if ADC operation not complete.
 * - IOT_ADC_NOT_OPEN if Adc has been closed without re-opening.
 * - IOT_ADC_FAILED if invalid ADC chain is requested.
 * - IOT_ADC_FUNCTION_NOT_SUPPORTED only valid for eSetAdcChain, if feature not supported
 */
int32_t iot_adc_ioctl(IotAdcHandle_t const pxAdc,
                      IotAdcIoctlRequest_t xRequest,
                      void * const pvBuffer)
{
  /* local variables */
  uint8_t            ucAdcIsBusy  = pdFALSE;
  uint8_t            ucAdcBusyCh  = 0;
  uint8_t            ucAdcChannel = 0;
  IotAdcChStatus_t  *pxChStatus   = NULL;
  IotAdcConfig_t    *pxConfig     = NULL;
  IotAdcChBuffer_t  *pxChBuffer   = NULL;
  int32_t            lStatus      = IOT_ADC_SUCCESS;

  adc_config_t      *pxAdcCfgTemp;

  /* pxAdc can't be null */
  if (pxAdc == NULL) {
    return IOT_ADC_INVALID_VALUE;
  }

  /* buffer can't be null */
  if (pvBuffer == NULL) {
    return IOT_ADC_INVALID_VALUE;
  }

  /* enter critical section */
  portENTER_CRITICAL();

  /* pxAdc must be already open */
  if (pxAdc->pxAdcCfg->is_open == pdFALSE) {
    lStatus = IOT_ADC_NOT_OPEN;
  }

  /* exit critical section */
  portEXIT_CRITICAL();

  /* process the request */
  switch(xRequest) {
    /******************/
    /* set ADC config */
    /******************/
    case eSetAdcConfig:       
      /* set pxConfig to point to buffer */
      if (lStatus == IOT_ADC_SUCCESS) {
        pxConfig = (IotAdcConfig_t *) pvBuffer;
      }
      /* validate ulAdcSampleTime */
      if (lStatus == IOT_ADC_SUCCESS) {
        if ((79 < pxConfig->ulAdcSampleTime) || (16 > pxConfig->ulAdcSampleTime)) {  
          lStatus = IOT_ADC_INVALID_VALUE;
        }
      }

      pxAdc->pxAdcCfg->sample_rate = (pxConfig->ulAdcSampleTime - 16);
      
      /* validate ucAdcResolution */
      if (lStatus == IOT_ADC_SUCCESS) {
        switch (pxConfig->ucAdcResolution) {
          case 12:
            break;
          default:
            lStatus = IOT_ADC_INVALID_VALUE;
            break;
        }
      }
      /* set driver config */
      if (lStatus == IOT_ADC_SUCCESS) {
        bk_adc_set_config(pxAdc->pxAdcCfg);
      }
      /* done */
      break;

    /******************/
    /* get ADC config */
    /******************/
    case eGetAdcConfig:
      /* set pxConfig to point to buffer */
      if (lStatus == IOT_ADC_SUCCESS) {
        pxConfig = (IotAdcConfig_t *) pvBuffer;
      }
      /* get driver config */
      if (lStatus == IOT_ADC_SUCCESS) {
        bk_adc_get_config(pxAdc->lInstNum, &pxAdcCfgTemp);
        pxConfig->ucAdcResolution = 12;
        pxConfig->ulAdcSampleTime = (pxAdcCfgTemp->sample_rate + 16);
      }
      /* done */
      break;

    /**********************/
    /* get channel status */
    /**********************/
    case eGetChStatus:
      /* enter critical section */
      portENTER_CRITICAL();
      /* set pxChStatus to point to buffer */
      if (lStatus == IOT_ADC_SUCCESS) {
        pxChStatus = (IotAdcChStatus_t *) pvBuffer;
      }
      /* read channel number */
      if (lStatus == IOT_ADC_SUCCESS) {
        ucAdcChannel = pxChStatus->ucAdcChannel;
      }
      /* read bus status */
      if (lStatus == IOT_ADC_SUCCESS) {
        ucAdcIsBusy = pxAdc->ucBusyStatus;
        ucAdcBusyCh = pxAdc->ucBusyChannel;
      }
      /* store the result */
      if (lStatus == IOT_ADC_SUCCESS) {
        if (ucAdcIsBusy == pdTRUE && ucAdcBusyCh == ucAdcChannel) {
          pxChStatus->xAdcChState = eChStateBusy;
        } else {
          pxChStatus->xAdcChState = eChStateIdle;
        }
      }
      /* exit critical section */
      portEXIT_CRITICAL();
      /* done */
      break;

    /**********************/
    /* set channel buffer */
    /**********************/
    case eSetChBuffer:
      /* enter critical section */
      portENTER_CRITICAL();
      /* set pxChBuffer to point to buffer */
      if (lStatus == IOT_ADC_SUCCESS) {
        pxChBuffer = (IotAdcChBuffer_t  *) pvBuffer;
      }
      /* read channel number */
      if (lStatus == IOT_ADC_SUCCESS) {
        ucAdcChannel = pxChBuffer->ucAdcChannel;
      }
      /* validate channel number */
      if (lStatus == IOT_ADC_SUCCESS) {
        if (BK_OK != bk_adc_is_valid_ch(ucAdcChannel)) {
          lStatus = IOT_ADC_INVALID_VALUE;
        }
      }
      /* validate buffer pointer */
      if (lStatus == IOT_ADC_SUCCESS) {
        if (pxChBuffer->pvBuffer == NULL) {
          lStatus = IOT_ADC_INVALID_VALUE;
        }
      }
      /* validate buffer length */
      if (lStatus == IOT_ADC_SUCCESS) {
        if ( pxChBuffer->ucBufLen == 0) {
          lStatus = IOT_ADC_INVALID_VALUE;
        }
      }
      /* set buffer */
      if (lStatus == IOT_ADC_SUCCESS) {
        bk_adc_get_config(ucAdcChannel, &pxAdcCfgTemp);
        pxAdcCfgTemp->output_buf = pxChBuffer->pvBuffer;
        pxAdcCfgTemp->output_buf_len = pxChBuffer->ucBufLen;
        
        if(ucAdcChannel == pxAdc->pxAdcCfg->chan)
        {
            pxAdc->pusBufPtr = pxChBuffer->pvBuffer;
            pxAdc->ucBufMax = pxChBuffer->ucBufLen;
            pxAdc->ucBufCtr = 0;
        }
        
      }
      /* exit critical section */
      portEXIT_CRITICAL();
      /* done */
      break;

    /*****************/
    /* set ADC chain */
    /*****************/
    case eSetAdcChain:
      if (lStatus == IOT_ADC_SUCCESS) {
        lStatus = IOT_ADC_FUNCTION_NOT_SUPPORTED;
      }
      break;

    /***********/
    /* INVALID */
    /***********/
    default:
      if (lStatus == IOT_ADC_SUCCESS) {
        lStatus = IOT_ADC_INVALID_VALUE;
      }
      break;
  }

  /* done */
  return lStatus;
}
