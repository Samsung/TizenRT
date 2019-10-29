/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
/**************************************************************************************
 * include/tinyara/lcd/ieg1120.h
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
 *   Authors: 
 *            
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 **************************************************************************************/

#ifndef __INCLUDE_TINYARA_LCD_STM32L4R9IDISCO_H
#define __INCLUDE_TINYARA_LCD_STM32L4R9IDISCO_H

/**************************************************************************************
 * Included Files
 **************************************************************************************/

#include <tinyara/config.h>
#include <sys/types.h>
#include <stdint.h>
#include <tinyara/video/fb.h>

/**************************************************************************************
 * Pre-processor Definitions
 **************************************************************************************/

/**************************************************************************************
 * Public Types
 **************************************************************************************/

struct stm32l4_lcd_s {
    /*  to control STM32L4R9I DISCOVERY Board Round LCD
     *
     *  - sendsparam    Send a multi length parameter for LCD configuration
     *  - sendlparam    Send a multi length parameter for LCD configuration
     */
    uint8_t (*sendsparam)(uint32_t Mode, uint32_t Param1, uint32_t Param2);
    uint8_t (*sendlparam)(uint32_t Mode, uint32_t NbParams, uint32_t Param1, uint8_t *ParametersTable);
    void    (*enablewap)(void);
    void    (*poweron)(void);
    void    (*poweroff)(void);
    void    (*refresh)(void);
};

/**************************************************************************************
 * Public Data
 **************************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/**************************************************************************************
 * Public Function Prototypes
 **************************************************************************************/

/**************************************************************************************
 * Name:  ili9341_initialize
 *
 * Description:
 *   Initialize the LCD video driver internal sturcture. Also initialize the
 *   lcd hardware if not done. The control of the LCD driver is depend on the
 *   selected MCU interface and part of the platform specific subdriver (see
 *   config/stm32f429i-disco/src/stm32_ili93414ws.c)
 *
 * Input Parameters:
 *
 *   lcd - A reference to the platform specific driver instance to control the
 *     ili9341 display driver.
 *   devno - A value in the range of 0 through CONFIG_ILI9341_NINTERFACES-1.
 *     This allows support for multiple LCD devices.
 *
 * Returned Value:
 *
 *   On success, this function returns a reference to the LCD driver object for
 *   the specified LCD driver. NULL is returned on any failure.
 *
 **************************************************************************************/

    FAR struct stm32l4_lcd_s *stm32l4_lcd_function(void);

/**************************************************************************************
 * Name:  ili9341_clear
 *
 * Description:
 *   This is a non-standard LCD interface.  Because of the various rotations, clearing
 *   the display in the normal way by writing a sequences of runs that covers the
 *   entire display can be very slow.  Here the display is cleared by simply setting
 *   all GRAM memory to the specified color.
 *
 * Input Parameters:
 *   dev   - A reference to the lcd driver structure
 *   color - The background color
 *
 * Returned Value:
 *
 *  On success - OK
 *  On error   - -EINVAL
 *
 **************************************************************************************/

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __INCLUDE_TINYARA_LCD_STM32L4R9IDISCO_H */

