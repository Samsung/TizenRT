/****************************************************************************
*
* Copyright 2019 Samsung Electronics All Rights Reserved.
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

#ifndef _SAMPLE_LIGHT_LED_H_
#define _SAMPLE_LIGHT_LED_H_

// Internal RGB LED config
#define LED_RED_IO_NUM              0
#define LED_GREEN_IO_NUM            2
#define LED_BLUE_IO_NUM             4

// External RGB LED config
#define LED_EXT_RED_IO_NUM          21
#define LED_EXT_GREEN_IO_NUM        22
#define LED_EXT_BLUE_IO_NUM         23


/*
Several pins of JP1 can't be used as gpio:
GPIO16 & GPIO17: are not connected to the connectoer.
GPIO32 & GPIO33: are connected to XTAL 32khz.
There are several pins limited by hardware:
GPIO14 & GPIO15: are connected to SD card and pulled up to VDD33.
GPIO13: pulled up and down by both 10Kohm resistances; it is not limit.
*/
#define KEY_RESET_IO_NUM            18
#define KEY_SWITCH_IO_NUM           19

#define KEY_RESET_PRESSED           1
#define KEY_SWITCH_PRESSED          1

#define BUTTON_SCAN_PEROID          (20 * 1000)
#define BUTTON_PULL_SET             GPIO_DRIVE_PULLDOWN


int key_get_status(int key, int *status);

int led_open(void);
int led_set_switch(int status);
int led_set_dimming(int dimming);
int led_set_color_temprature(int color_temp);
int led_close(void);
int led_utc_ledonoff(void);

#endif //_SAMPLE_LIGHT_LED_H_
