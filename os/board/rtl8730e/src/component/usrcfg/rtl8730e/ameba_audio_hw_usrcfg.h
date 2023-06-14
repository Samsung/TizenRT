/*
 * Copyright (c) 2023 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef AMEBA_FWK_AUDIO_CONFIGS_HARDWARE_AUDIO_AUDIO_HW_CONFIG_H
#define AMEBA_FWK_AUDIO_CONFIGS_HARDWARE_AUDIO_AUDIO_HW_CONFIG_H

/*
 *Audio amplifier pin, if your board has no audio amplifier pin , please set it as -1.
 */
#define AUDIO_HW_AMPLIFIER_PIN          _PB_11

/*
 *After enable amplifer, it need some time to be steady. For d2 demo board, it's 90ms.
 */
#define AUDIO_HW_AMPLIFIER_ENABLE_TIME  90

/*
 *After disable amplifer, it need some time to be steady. For d2 demo board, it's 1ms.
 */
#define AUDIO_HW_AMPLIFIER_DISABLE_TIME 1

/*
 *When no sound playing, should amplifier mute? Set 1 if user wants amplifier mute, set
 *0 if user wants amplifier stay unmute.
 *For d2 and lite demo board, our hardware design makes sure aec noise acceptable, if
 *amplifier stays unmute, so it can be set 0.
 */
#define AUDIO_HW_AMPLIFIER_MUTE_ENABLE  0

/*
 *DMIC clock and data pins.
 */
#define AUDIO_HW_DMIC_CLK_PIN           _PB_22
#define AUDIO_HW_DMIC_DATA0_PIN         _PB_21
#define AUDIO_HW_DMIC_DATA1_PIN         _PB_18
#define AUDIO_HW_DMIC_DATA2_PIN         _PB_17
#define AUDIO_HW_DMIC_DATA3_PIN         _PB_16

/*
 *After dmic clock enable, it need some time to be steady. For d2 demo board, it's 100ms.
 *For customer's board, need to check with customer.
 */
#define AUDIO_HW_DMIC_STEADY_TIME       100

/*
 *Sport counter max irq X, when sport delivered X frames, there will be one interrupt.
 */
#define AUDIO_HW_MAX_SPORT_IRQ_X        134217727

#endif