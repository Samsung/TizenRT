/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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

#ifndef _RREPORT_CONFIG_H_
#define _RREPORT_CONFIG_H_

#define RR_NUM_OF_RSSI 5
#define RR_INTERVAL 500 // milliseconds
#define RR_LOCATION "unknown"

/*NTPC*/
#define RR_NTPC_SERVER "time.google.com"
#define RR_NTPC_PORT 123
#define RR_NTPC_INTERVAL_SEC 3600 // 1 hour

/*  Wi-fi */
#define RR_AP_AUTH WIFI_MANAGER_AUTH_WPA2_PSK
#define RR_AP_CRYPTO WIFI_MANAGER_CRYPTO_AES

/*  HTTP */

/*  Debug */
#define RR_ERR											\
	do {												\
		printf("[ERROR] %s%d\n", __FILE__,  __LINE__);	\
	} while (0)

#endif // _RREPORT_CONFIG_H_

