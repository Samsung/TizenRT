/* ***************************************************************************
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

#include "iot_bsp_nv_data.h"
#include "iot_debug.h"

const char *iot_bsp_nv_get_data_path(iot_nvd_t nv_type)
{
	HIT();
	IOT_WARN_CHECK((nv_type < 0 || nv_type > IOT_NVD_MAX), NULL, "Invalid args");

	switch (nv_type) {

	/* wifi prov data, smartfs based*/
	case IOT_NVD_WIFI_PROV_STATUS:
		return "/mnt/WifiProvStatus";
	case IOT_NVD_AP_SSID:
		return "/mnt/IotAPSSID";
	case IOT_NVD_AP_PASS:
		return "/mnt/IotAPPASS";
	case IOT_NVD_AP_BSSID:
		return "/mnt/IotAPBSSID";
	case IOT_NVD_AP_AUTH_TYPE:
		return "/mnt/IotAPAuthType";
	/* wifi prov data */

	/* cloud prov data, smartfs based */
	case IOT_NVD_CLOUD_PROV_STATUS:
		return "/mnt/CloudProvStatus";
	case IOT_NVD_SERVER_URL:
		return "/mnt/ServerURL";
	case IOT_NVD_SERVER_PORT:
		return "/mnt/ServerPort";
	case IOT_NVD_LOCATION_ID:
		return "/mnt/LocationID";
	case IOT_NVD_ROOM_ID:
		return "/mnt/RoomID";
	case IOT_NVD_LABEL:
		return "/mnt/Label";
	case IOT_NVD_DEVICE_ID:
		return "/mnt/DeviceID";

	/* stored in stnv partition (manufacturer data),romfs based */
	case IOT_NVD_PRIVATE_KEY:
		return "/rom/PrivateKey";
	case IOT_NVD_PUBLIC_KEY:
		return "/rom/PublicKey";
	case IOT_NVD_CA_CERT:
		return "/rom/CACert";
	case IOT_NVD_SUB_CERT:
		return "/rom/SubCert";
	case IOT_NVD_SERIAL_NUM:
		return "/rom/SerialNum";
	/* stored in stnv partition (manufacturer data) */

	default:
		return NULL;
	}
}
