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

/**
* @defgroup WiFi WiFi
* SLSI WiFi utility functions for TinyAra
* @{
*
* @brief S.LSI WiFi utility functions for TinyARA.
*
* This is the utility functions available for the Samsung LSI WiFi API for TinyARA OS.
*/

#ifndef __SLSI_WIFI_UTILS_H__
#define __SLSI_WIFI_UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <tinyara/mm/mm.h>
#include "slsi_wifi_api.h"

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/* Supported security modes as string */
#define SLSI_WIFI_SECURITY_OPEN          "open"
#define SLSI_WIFI_SECURITY_WEP_OPEN      "wep"
#define SLSI_WIFI_SECURITY_WEP_SHARED    "wep_shared"
#define SLSI_WIFI_SECURITY_WPA_MIXED     "wpa_mixed"
#define SLSI_WIFI_SECURITY_WPA_TKIP      "wpa_tkip"
#define SLSI_WIFI_SECURITY_WPA_AES       "wpa_aes"
#define SLSI_WIFI_SECURITY_WPA2_MIXED    "wpa2_mixed"
#define SLSI_WIFI_SECURITY_WPA2_TKIP     "wpa2_tkip"
#define SLSI_WIFI_SECURITY_WPA2_AES      "wpa2_aes"

#define MODE_STRING_MAX 100
#define MAX_SSID_LEN    (4 * 32 + 1)

/*return a security config struct depending on the mode, and the input security string (see above
 * defines.
 */
slsi_security_config_t *getSecurityConfig(char *sec_type, char *psk, WiFi_InterFace_ID_t mode);
/* prints a nicely ordered list of the scan results from the scan_info list
 */

void printScanResult(slsi_scan_info_t *list);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __SLSI_WIFI_UTILS_H */

