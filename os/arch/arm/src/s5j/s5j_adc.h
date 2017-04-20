/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/s5j/s5j_adc.h
 *
 *   Copyright (C) 2017 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_S5J_S5J_ADC_H__
#define __ARCH_ARM_SRC_S5J_S5J_ADC_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#if defined(CONFIG_S5J_S5JT200)
#include "chip/s5jt200_adc.h"
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define S5J_ADC_MAX_CHANNELS	4

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/
typedef enum {
	adc_conversion_1time	= ADC_CON2_CTIME_1,
	adc_conversion_2time	= ADC_CON2_CTIME_2,
	adc_conversion_4time	= ADC_CON2_CTIME_4,
	adc_conversion_8time	= ADC_CON2_CTIME_8,
	adc_conversion_16time	= ADC_CON2_CTIME_16,
	adc_conversion_32time	= ADC_CON2_CTIME_32,
	adc_conversion_64time	= ADC_CON2_CTIME_64,
} adc_conversion_mode_t;

typedef enum {
	adc_channel_0		= ADC_CON2_ACHSEL_CHANNEL0,
	adc_channel_1		= ADC_CON2_ACHSEL_CHANNEL1,
	adc_channel_2		= ADC_CON2_ACHSEL_CHANNEL2,
	adc_channel_3		= ADC_CON2_ACHSEL_CHANNEL3,
} adc_channel_t;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: s5j_adc_initialize
 *
 * Description:
 *   Initialize the ADC. As the pins of each ADC channel are exported through
 *   configurable GPIO and it is board-specific, information on available
 *   ADC channels should be passed to s5j_adc_initialize().
 *
 * Input Parameters:
 *   chanlist  - The list of channels
 *   nchannels - Number of channels
 *
 * Returned Value:
 *   Valid can device structure reference on success; a NULL on failure
 *
 ****************************************************************************/
struct adc_dev_s *s5j_adc_initialize(FAR const uint8_t *chanlist,
				     int cchannels);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_S5J_S5J_ADC_H */
