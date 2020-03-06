/** mbed Microcontroller Library
  ******************************************************************************
  * @file    lcdc_api.c
  * @author 
  * @version V1.0.0
  * @date    2019-04-19
  * @brief   This file provides mbed API for LCDC.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2019, Realtek Semiconductor Corporation. All rights reserved.
  ****************************************************************************** 
  */

#ifndef MBED_LCDC_API_H
#define MBED_LCDC_API_H

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif


/** @addtogroup lcdc LCDC
 *  @ingroup    hal
 *  @brief      lcdc functions
 *  @{
 */

#if ((defined (CONFIG_PLATFORM_8721D) && (CONFIG_PLATFORM_8721D == 1)))
///@name AmebaD Only
///@{


typedef struct lcdc_s lcdc_t;

/**
  * @brief    Initializes the LCDC peripheral.
  * @param  obj: lcdc object define in application software.
  * @retval None
  */	
void lcdc_init(lcdc_t * obj);

/**
  * @brief    Trigger the hardware to transfer one frame from DMA buffer based 
  *               on sync signal.
  * @param  obj: lcdc object define in application software.
  * @retval None
  */
void lcdc_mcu_dma_trigger(lcdc_t * obj);

/**
  * @brief     write data to MCU I/F bus.
  * @param  obj: lcdc object define in application software.
  * @param  Data: the data to transmit.
  * @retval   None
  */
void  lcdc_mcu_write_data(lcdc_t * obj, u16 data);

/**
  * @brief     read data from MCU I/F bus.
  * @param  obj: lcdc object define in application software.
  * @retval   the read value
  */
u16 lcdc_mcu_read_data(lcdc_t * obj);

/**
  * @brief     write command to MCU I/F bus.
  * @param  obj: lcdc object define in application software.
  * @param  Cmd: the command to transmit.
  * @retval   None
  */
void lcdc_mcu_write_cmd(lcdc_t * obj, u16 cmd);

/**
  * @brief    Configure LCDC DMA burst size .
  * @param  obj: lcdc object define in application software.
  * @param  BurstSize: DMA burst size; Unit 64 Bytes.
  * @note     If BurstSize=1, the actual burstsize = 1x64 Bytes; if the BurstSize=2, 
  *                the actual burstsize = 2x64 = 128 Bytes.... The parameter "BurstSize" is not more
  *                than 8.
  * @retval   None
  */
void lcdc_dma_burstsize_config(lcdc_t * obj, u32 BurstSize);

/**
  * @brief    Configure LCDC DMA under flow mode and under flow error data .
  * @param  obj: lcdc object define in application software.
  * @param  DmaUnFlwMode: DMA under flow mode, this parameter
  *                can be one of the following values:
  *     	    @arg LCDC_DMAUNFW_OUTPUT_LASTDATA:  output last data
  *                @arg LCDC_DMAUNFW_OUTPUT_ERRORDATA:  output error data
  * @param  ErrorData: the output data when  DMA FIFO underflow occurred. When under flow mode is configured as
  *                LCDC_DMAUNFW_OUTPUT_ERRORDATA, this parameter is needed, and otherwise it can be ignored.
  * @retval   None
  */
void lcdc_dma_underflow_config(lcdc_t * obj, u32 DmaUnFlwMode, u32 ErrorData);

/**
  * @brief    Configure image base address.
  * @param  obj: lcdc object define in application software.
  * @param  ImgBaseAddr: the image base address.
  * @retval   None
  */
void lcdc_dma_addr_config(lcdc_t * obj, u32 ImgBaseAddr);

/**
  * @brief    Set lcdc interrupt handler.
  * @param  obj: lcdc object define in application software.
  * @param  func: the interrupt handler function.
  * @retval   None
  */
void lcdc_set_irq_handler(lcdc_t * obj, void * func);

/**
  * @brief  Enables or disables the specified LCDC interrupts.
  * @param  obj: lcdc object define in application software.
  * @param  LCDC_IT: specifies the LCDC interrupts sources to be enabled or disabled. 
  *   This parameter can be any combination of the following values:
  *     @arg LCDC_IT_DMAUNDFW: DMA FIFO underflow interrupt
  *     @arg LCDC_IT_FRDN: LCD refresh done interrupt
  *     @arg LCDC_IT_LINE: line interrupt
  *     @arg LCDC_IT_IO_TIMEOUT: IO write/read timeout interrupt
  *     @arg LCDC_IT_FRM_START: Frame Start interrupt
  * @param  NewState: new state of the specified LCDC interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void lcdc_set_irq(lcdc_t * obj, u32 LCDC_IT,  u32 NewState);

/**
  * @brief  Clears the LCDC's interrupt pending bits.
  * @param  obj: lcdc object define in application software.
  * @param  LCDC_IT: specifies the interrupt to be cleared.
  *   This parameter can be any combination of the following values:
  *     @arg LCDC_IT_LINE:line interrupt
  *     @arg LCDC_IT_FRDN: refresh frame done interrupt
  *     @arg LCDC_IT_DMAUNDFW: DMA FIFO under flow interrupt
  *     @arg LCDC_IT_IO_TIMEOUT: IO write/read timeout interrupt
  *     @arg LCDC_IT_FRM_START: Frame Start interrupt
  * @retval None
  */
void lcdc_clear_irq(lcdc_t * obj, u32 LCDC_IT);

/**
  * @brief  Get lcdc interrupt status.
  * @param  obj: lcdc object define in application software.
  * @retval interrupt status
  */
u32  lcdc_get_irq_status(lcdc_t * obj);

/**
  * @brief  Enables the LCDC.
  * @param  obj: lcdc object define in application software.
  * @retval None
  */
void lcdc_enable(lcdc_t * obj);

/**
  * @brief  Disables the LCDC.
  * @param  obj: lcdc object define in application software.
  * @retval None
  */
void lcdc_disable(lcdc_t * obj);

/**
  * @brief  Deinitializes the LCDC.
  * @param  obj: lcdc object define in application software.
  * @note   Disable LCDC instantly, clear and disable all interrupts.
  * @retval None
  */
void lcdc_deinit(lcdc_t * obj);
///@}

/*\@}*/

///@}
#endif//CONFIG_PLATFORM_8721D

#ifdef __cplusplus
}
#endif

#endif/* MBED_LCDC_API_H */
