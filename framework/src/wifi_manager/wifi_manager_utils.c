/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>
#include <wifi_manager/wifi_manager.h>
#include <tinyara/net/if/wifi.h>
#include "wifi_manager_utils.h"

wifi_manager_result_e wifimgr_convert2wifimgr_res(trwifi_result_e tres)
{
	wifi_manager_result_e res = WIFI_MANAGER_FAIL;
	switch (tres) {
	case TRWIFI_SUCCESS:
		res = WIFI_MANAGER_SUCCESS;
		break;
	case TRWIFI_INVALID_ARGS:
		res = WIFI_MANAGER_INVALID_ARGS;
		break;
	case TRWIFI_TIMEOUT:
		res = WIFI_MANAGER_TIMEOUT;
		break;
	case TRWIFI_BUSY:
		res = WIFI_MANAGER_BUSY;
		break;
	case TRWIFI_ALREADY_CONNECTED:
		res = WIFI_MANAGER_ALREADY_CONNECTED;
		break;
	case TRWIFI_NOT_SUPPORTED:
		res = WIFI_MANAGER_NOT_AVAILABLE;
		break;
	case TRWIFI_FILE_ERROR:
	case TRWIFI_FAIL:
	default:
		res = WIFI_MANAGER_FAIL;
		break;
	}
	return res;
}

trwifi_ap_auth_type_e wifimgr_convert2trwifi_auth(wifi_manager_ap_auth_type_e atype)
{
	switch (atype) {
	case WIFI_MANAGER_AUTH_OPEN:
		return TRWIFI_AUTH_OPEN;
	case WIFI_MANAGER_AUTH_WEP_SHARED:
		return TRWIFI_AUTH_WEP_SHARED;
	case WIFI_MANAGER_AUTH_WPA_PSK:
		return TRWIFI_AUTH_WPA_PSK;
	case WIFI_MANAGER_AUTH_WPA2_PSK:
		return TRWIFI_AUTH_WPA2_PSK;
	case WIFI_MANAGER_AUTH_WPA3_PSK:
		return TRWIFI_AUTH_WPA3_PSK;
	case WIFI_MANAGER_AUTH_WPA_AND_WPA2_PSK:
		return TRWIFI_AUTH_WPA_AND_WPA2_PSK;
	case WIFI_MANAGER_AUTH_WPA2_AND_WPA3_PSK:
		return TRWIFI_AUTH_WPA2_AND_WPA3_PSK;
	case WIFI_MANAGER_AUTH_WPA_PSK_ENT:
		return TRWIFI_AUTH_WPA_PSK_ENT;
	case WIFI_MANAGER_AUTH_WPA2_PSK_ENT:
		return TRWIFI_AUTH_WPA2_PSK_ENT;
	case WIFI_MANAGER_AUTH_WPA_AND_WPA2_PSK_ENT:
		return TRWIFI_AUTH_WPA_AND_WPA2_PSK_ENT;
	case WIFI_MANAGER_AUTH_IBSS_OPEN:
		return TRWIFI_AUTH_IBSS_OPEN;
	case WIFI_MANAGER_AUTH_WPS:
		return TRWIFI_AUTH_WPS;
	case WIFI_MANAGER_AUTH_UNKNOWN:
		return TRWIFI_AUTH_UNKNOWN;
	}
	return TRWIFI_AUTH_UNKNOWN;
}

trwifi_ap_crypto_type_e wifimgr_convert2trwifi_crypto(wifi_manager_ap_crypto_type_e ctype)
{
	switch (ctype) {
	case WIFI_MANAGER_CRYPTO_NONE:
		return TRWIFI_CRYPTO_NONE;
	case WIFI_MANAGER_CRYPTO_WEP_64:
		return TRWIFI_CRYPTO_WEP_64;
	case WIFI_MANAGER_CRYPTO_WEP_128:
		return TRWIFI_CRYPTO_WEP_128;
	case WIFI_MANAGER_CRYPTO_AES:
		return TRWIFI_CRYPTO_AES;
	case WIFI_MANAGER_CRYPTO_TKIP:
		return TRWIFI_CRYPTO_TKIP;
	case WIFI_MANAGER_CRYPTO_TKIP_AND_AES:
		return TRWIFI_CRYPTO_TKIP_AND_AES;
	case WIFI_MANAGER_CRYPTO_AES_ENT:
		return TRWIFI_CRYPTO_AES_ENT;
	case WIFI_MANAGER_CRYPTO_TKIP_ENT:
		return TRWIFI_CRYPTO_TKIP_ENT;
	case WIFI_MANAGER_CRYPTO_TKIP_AND_AES_ENT:
		return TRWIFI_CRYPTO_TKIP_AND_AES_ENT;
	case WIFI_MANAGER_CRYPTO_UNKNOWN:
		return TRWIFI_CRYPTO_UNKNOWN;
	}
	return TRWIFI_CRYPTO_UNKNOWN;
}

wifi_manager_ap_auth_type_e wifimgr_convert2wifimgr_auth(trwifi_ap_auth_type_e atype)
{
	switch (atype) {
	case TRWIFI_AUTH_OPEN:
		return WIFI_MANAGER_AUTH_OPEN;
	case TRWIFI_AUTH_WEP_SHARED:
		return WIFI_MANAGER_AUTH_WEP_SHARED;
	case TRWIFI_AUTH_WPA_PSK:
		return WIFI_MANAGER_AUTH_WPA_PSK;
	case TRWIFI_AUTH_WPA2_PSK:
		return WIFI_MANAGER_AUTH_WPA2_PSK;
	case TRWIFI_AUTH_WPA3_PSK:
		return WIFI_MANAGER_AUTH_WPA3_PSK;
	case TRWIFI_AUTH_WPA_AND_WPA2_PSK:
		return WIFI_MANAGER_AUTH_WPA_AND_WPA2_PSK;
	case TRWIFI_AUTH_WPA2_AND_WPA3_PSK:
		return WIFI_MANAGER_AUTH_WPA2_AND_WPA3_PSK;
	case TRWIFI_AUTH_WPA_PSK_ENT:
		return WIFI_MANAGER_AUTH_WPA_PSK_ENT;
	case TRWIFI_AUTH_WPA2_PSK_ENT:
		return WIFI_MANAGER_AUTH_WPA2_PSK_ENT;
	case TRWIFI_AUTH_WPA_AND_WPA2_PSK_ENT:
		return WIFI_MANAGER_AUTH_WPA_AND_WPA2_PSK_ENT;
	case TRWIFI_AUTH_IBSS_OPEN:
		return WIFI_MANAGER_AUTH_IBSS_OPEN;
	case TRWIFI_AUTH_WPS:
		return WIFI_MANAGER_AUTH_WPS;
	case TRWIFI_AUTH_UNKNOWN:
		return WIFI_MANAGER_AUTH_UNKNOWN;
	}
	return WIFI_MANAGER_AUTH_UNKNOWN;
}

wifi_manager_ap_crypto_type_e wifimgr_convert2wifimgr_crypto(trwifi_ap_crypto_type_e ctype)
{
	switch (ctype) {
	case TRWIFI_CRYPTO_NONE:
		return WIFI_MANAGER_CRYPTO_NONE;
	case TRWIFI_CRYPTO_WEP_64:
		return WIFI_MANAGER_CRYPTO_WEP_64;
	case TRWIFI_CRYPTO_WEP_128:
		return WIFI_MANAGER_CRYPTO_WEP_128;
	case TRWIFI_CRYPTO_AES:
		return WIFI_MANAGER_CRYPTO_AES;
	case TRWIFI_CRYPTO_TKIP:
		return WIFI_MANAGER_CRYPTO_TKIP;
	case TRWIFI_CRYPTO_TKIP_AND_AES:
		return WIFI_MANAGER_CRYPTO_TKIP_AND_AES;
	case TRWIFI_CRYPTO_AES_ENT:
		return WIFI_MANAGER_CRYPTO_AES_ENT;
	case TRWIFI_CRYPTO_TKIP_ENT:
		return WIFI_MANAGER_CRYPTO_TKIP_ENT;
	case TRWIFI_CRYPTO_TKIP_AND_AES_ENT:
		return WIFI_MANAGER_CRYPTO_TKIP_AND_AES_ENT;
	case TRWIFI_CRYPTO_UNKNOWN:
		return WIFI_MANAGER_CRYPTO_UNKNOWN;
	}
	return WIFI_MANAGER_CRYPTO_UNKNOWN;
}
