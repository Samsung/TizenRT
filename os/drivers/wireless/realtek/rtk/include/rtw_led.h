/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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
 ******************************************************************************/
#ifndef __RTW_LED_H_
#define __RTW_LED_H_

typedef enum _LED_CTL_MODE {
	LED_CTL_POWER_ON = 1,
	LED_CTL_LINK = 2,
	LED_CTL_NO_LINK = 3,
	LED_CTL_TX = 4,
	LED_CTL_RX = 5,
	LED_CTL_SITE_SURVEY = 6,
	LED_CTL_POWER_OFF = 7,
	LED_CTL_START_TO_LINK = 8,
	LED_CTL_START_WPS = 9,
	LED_CTL_STOP_WPS = 10,
	LED_CTL_START_WPS_BOTTON = 11,		//added for runtop
	LED_CTL_STOP_WPS_FAIL = 12,			//added for ALPHA
	LED_CTL_STOP_WPS_FAIL_OVERLAP = 13, //added for BELKIN
	LED_CTL_CONNECTION_NO_TRANSFER = 14,
} LED_CTL_MODE;

#ifdef CONFIG_SW_LED
#define rtw_led_control(adapter, LedAction)                               \
	do {                                                                  \
		if ((adapter)->ledpriv.LedControlHandler)                         \
			(adapter)->ledpriv.LedControlHandler((adapter), (LedAction)); \
	} while (0)
#else //CONFIG_SW_LED
#define rtw_led_control(adapter, LedAction)
#endif //CONFIG_SW_LED

#endif //__RTW_LED_H_
