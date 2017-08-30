/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
#ifndef SLSI_WIFI_NVMAP_H_
#define SLSI_WIFI_NVMAP_H_

#include <stdlib.h>

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/* struct containing all the values that needs to be read/written
 * from/to the non-volatile memory.
 * New values needs to be added to the end of the structure and needs to be
 * of byte aligned sizes.
 *
 * */

typedef struct slsi_wifi_nv_data {
	char	initialized[5];
	char	country_code[3];	//two asci chars + null
	uint8_t	tx_power;
} slsi_wifi_nv_data_t;

/* change default values in menu config or use sane defaults*/
#ifdef CONFIG_SLSI_WIFI_DEFAULT_WLAN_COUNTRY_CODE
#define SLSI_WIFI_NV_DEFAULT_COUNTRY_CODE	CONFIG_SLSI_WIFI_DEFAULT_WLAN_COUNTRY_CODE
#else
#define SLSI_WIFI_NV_DEFAULT_COUNTRY_CODE	"00"
#endif
#ifdef CONFIG_SLSI_WIFI_DEFAULT_WLAN_TX_POWER
#define SLSI_WIFI_NV_DEFAULT_TX_POWER		CONFIG_SLSI_WIFI_DEFAULT_WLAN_TX_POWER
#else
#define SLSI_WIFI_NV_DEFAULT_TX_POWER		(uint8_t)30
#endif

#define SLSI_WIFI_NV_DATA_SIZE				(sizeof(slsi_wifi_nv_data_t))

#undef EXTERN
#ifdef  __cplusplus
}
#endif
#endif							// SLSI_WIFI_NVMAP_H_
