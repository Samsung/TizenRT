/** mbed Microcontroller Library
  ******************************************************************************
  * @file    keyscan_api.h
  * @author 
  * @version V1.0.0
  * @brief   This file provides following mbed KEYSCAN API
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2006-2013 ARM Limited
  *
  * Licensed under the Apache License, Version 2.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  *     http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  ****************************************************************************** 
  */
#ifndef MBED_KEYSCAN_API_H
#define MBED_KEYSCAN_API_H

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup keyscan       KEYSCAN
 *  @ingroup    hal
 *  @brief      KEYSCAN functions
 *  @{
 */

#if ((defined (CONFIG_PLATFORM_8721D) && (CONFIG_PLATFORM_8721D == 1)))
///@name AmebaD Only
///@{

typedef struct keyscan_s keyscan_t;

/**
  * @brief  initialization of pinmux settings and pad settings.
  * @param  col: selected column num depending on KeyColumn(eg: col 0 & col 2, col is 5) .
  * @param  row: selected column num depending on KeyRow .
  * @return void
  */
void keyscan_array_pinmux(u32 col, u32 row);

/**
  * @brief  Get data number of keyscan FIFO.
  * @param  obj: keyscan object defined in application software.
  * @retval data number of keyscan FIFO
  */
u8 keyscan_getdatanum(keyscan_t *obj);

/**
  * @brief  Read data from keyscan FIFO.
  * @param  obj: keyscan object defined in application software.
  * @param  pBuf: buffer to save data read from KeyScan FIFO.
  * @param  num: number of data to be read.
  * @retval None
  */
void keyscan_read(keyscan_t *obj, u32* pbuf, u8 num);

/**
  * @brief  Initializes the KeyScan device.
  * @param  obj: keyscan object defined in application software.
  * @retval None
  */
void keyscan_init(keyscan_t *obj);

/**
  * @brief  Enables or disables the specified KeyScan interrupts mask.
  * @param  obj: keyscan object defined in application software.
  * @param  keyscan_IT: specifies the KeyScan interrupt sources to be enabled or masked.
  *          This parameter can be one or combinations of the following values:
  *            @arg BIT_KS_SCAN_EVENT_INT_MSK: Mask Scan event interrupt status
  *            @arg BIT_KS_FIFO_LIMIT_INT_MSK: Mask FIFO limit interrupt status
  *            @arg BIT_KS_FIFO_OVERFLOW_INT_MSK: Mask FIFO overflow interrupt
  *            @arg BIT_KS_FIFO_FULL_INT_MSK: Mask FIFO full interrupt
  *            @arg BIT_KS_SCAN_FINISH_INT_MSK: Mask scan finish interrupt
  *            @arg BIT_KS_FIFO_NOTEMPTY_INT_MSK: Mask FIFO nonempty interrupt
  *            @arg BIT_KS_ALL_RELEASE_INT_MSK: Mask All Release interrupt
  * @param  newstate: new state of the specified KeyScan interrupts mask.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void keyscan_set_irq(keyscan_t * obj, u32 keyscan_IT, u32 newstate);

/**
  * @brief  Clear the specified KeyScan interrupt pending bit.
  * @param  obj: keyscan object defined in application software.
  * @param  keyscan_IT: specifies the KeyScan interrupt to be cleared.
  *          This parameter can be one or combinations of the following values:
  *            @arg BIT_KS_FIFO_LIMIT_INT_STATUS: FIFO limit interrupt status
  *            @arg BIT_KS_FIFO_OVERFLOW_INT_STATUS: FIFO overflow interrupt status
  *            @arg BIT_KS_SCAN_FINISH_INT_STATUS: Scan finish interrupt status
  *            @arg BIT_KS_ALL_RELEASE_INT_STATUS: All Release interrupt status
  * @note   BIT_KS_SCAN_EVENT_INT_STATUS is automatically cleared by hardware when the data is read. 
  *         BIT_KS_FIFO_FULL_INT_STATUS is automatically cleared by hardware when the buffer level 
  *         goes below the BIT_KS_FIFO_THREHOLD_LEVEL threshold. 
  *         BIT_KS_FIFO_NOTEMPTY_INT_STATUS is automatically cleared by hardware when the FIFO is empty.
  * @retval None
  */
void keyscan_clear_irq(keyscan_t * obj, u32 keyscan_IT);

/**
  * @brief  Get KeyScan interrupt status.
  * @param  obj: keyscan object defined in application software.
  * @retval interrupt status
  */
u32 keyscan_get_irq_status(keyscan_t * obj);

/**
  * @brief  Set keyscan interrupt handler.
  * @param  obj: keyscan object define in application software.
  * @param  func: the interrupt handler function.
  * @retval None
  */
void keyscan_set_irq_handler(keyscan_t * obj, void * func);

/**
  * @brief  Enable the specified KeyScan peripheral.
  * @param  obj: keyscan object defined in application software.
  * @retval None
  */
void keyscan_enable(keyscan_t *obj);

/**
  * @brief  disable the specified KeyScan peripheral.
  * @param  obj: keyscan object defined in application software.
  * @retval None
  */
void keyscan_disale(keyscan_t *obj);

/**
  * @brief  Clears the FIFO data.
  * @param  obj: keyscan object defined in application software.
  * @retval None
  */
void keyscan_clearfifodata(keyscan_t *obj);

///@}

/*\@}*/

///@}
#endif //CONFIG_PLATFORM_8721D

#ifdef __cplusplus
}
#endif

#endif/* MBED_KEYSCAN_API_H */
