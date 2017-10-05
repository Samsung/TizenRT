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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <tinyara/mm/mm.h>
#include <slsi_wifi/slsi_wifi_api.h>

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
#define MAX_SSID_LEN (4*32+1)

/* function is used from multiple places so dont rely on local static vars in this file */

slsi_security_config_t *getSecurityConfig(char *sec_type, char *psk, WiFi_InterFace_ID_t mode)
{

	slsi_security_config_t *ret = NULL;
	if (strncmp(SLSI_WIFI_SECURITY_OPEN, sec_type, sizeof(SLSI_WIFI_SECURITY_OPEN)) != 0) {
		ret = (slsi_security_config_t *)zalloc(sizeof(slsi_security_config_t));
		if (ret) {
			if (strncmp(SLSI_WIFI_SECURITY_WEP_OPEN, sec_type, sizeof(SLSI_WIFI_SECURITY_WEP_OPEN)) == 0) {
				ret->secmode = SLSI_SEC_MODE_WEP;
			} else if (strncmp(SLSI_WIFI_SECURITY_WEP_SHARED, sec_type, sizeof(SLSI_WIFI_SECURITY_WEP_SHARED)) == 0) {
				ret->secmode = SLSI_SEC_MODE_WEP_SHARED;
			} else if (strncmp(SLSI_WIFI_SECURITY_WPA_MIXED, sec_type, sizeof(SLSI_WIFI_SECURITY_WPA_MIXED)) == 0) {
				ret->secmode = SLSI_SEC_MODE_WPA_MIXED;
			} else if (strncmp(SLSI_WIFI_SECURITY_WPA_TKIP, sec_type, sizeof(SLSI_WIFI_SECURITY_WPA_TKIP)) == 0) {
				ret->secmode = SLSI_SEC_MODE_WPA_TKIP;
			} else if (strncmp(SLSI_WIFI_SECURITY_WPA_AES, sec_type, sizeof(SLSI_WIFI_SECURITY_WPA_AES)) == 0) {
				ret->secmode = SLSI_SEC_MODE_WPA_CCMP;
			} else if (strncmp(SLSI_WIFI_SECURITY_WPA2_MIXED, sec_type, sizeof(SLSI_WIFI_SECURITY_WPA2_MIXED)) == 0) {
				ret->secmode = SLSI_SEC_MODE_WPA2_MIXED;
			} else if (strncmp(SLSI_WIFI_SECURITY_WPA2_TKIP, sec_type, sizeof(SLSI_WIFI_SECURITY_WPA2_TKIP)) == 0) {
				ret->secmode = SLSI_SEC_MODE_WPA2_TKIP;
			} else if (strncmp(SLSI_WIFI_SECURITY_WPA2_AES, sec_type, sizeof(SLSI_WIFI_SECURITY_WPA2_AES)) == 0) {
				ret->secmode = SLSI_SEC_MODE_WPA2_CCMP;
			}
		}
		/* store the passphrase */
		if (psk && ret != NULL) {
			memcpy(ret->passphrase, psk, strlen(psk));
		} else {
			if (ret) {
				free(ret);
				ret = NULL;
			}
		}
	}
	return ret;
}

/*
 * Get the string representing the security mode in in a can result
 */

static void getSecurityModeString(slsi_security_config_t *sec, uint8_t count, char *modestring)
{
	memset(modestring, 0, MODE_STRING_MAX);
	char *pos = modestring;
	int x = 0;
	if (count == 0) {
		strncat(pos, "[NONE]", 6);
	} else {
		for (x = 0; x < count; x++) {
			strncat(pos, "[", 1);
			pos += 1;
			if (sec->secmode == SLSI_SEC_MODE_WEP) {
				strncat(pos, "WEP", 3);
				pos += 3;
			} else if (sec->secmode == SLSI_SEC_MODE_WEP_SHARED) {
				strncat(pos, "WEP_SHARED", 10);
				pos += 10;
			} else if (sec->secmode == (SLSI_SEC_MODE_WEP | SLSI_SEC_MODE_WEP_SHARED)) {
				strncat(pos, "WEP", 3);	// we dont really know if this is open or shared
				// let's just yse "WEP"
				pos += 3;
			} else if (sec->secmode == SLSI_SEC_MODE_WPA_TKIP) {
				strncat(pos, "WPA-PSK+TKIP", 12);
				pos += 12;
			} else if (sec->secmode == SLSI_SEC_MODE_WPA_CCMP) {
				strncat(pos, "WPA-PSK+AES", 11);
				pos += 11;
			} else if (sec->secmode == SLSI_SEC_MODE_WPA_MIXED) {
				strncat(pos, "WPA-PSK+AES+TKIP", 16);
				pos += 16;
			} else if (sec->secmode == SLSI_SEC_MODE_WPA2_TKIP) {
				strncat(pos, "WPA2-PSK+TKIP", 13);
				pos += 13;
			} else if (sec->secmode == SLSI_SEC_MODE_WPA2_CCMP) {
				strncat(pos, "WPA2-PSK+AES", 12);
				pos += 12;
			} else if (sec->secmode == SLSI_SEC_MODE_WPA2_MIXED) {
				strncat(pos, "WPA2-PSK+AES+TKIP", 17);
				pos += 17;
			} else if (sec->secmode == SLSI_SEC_MODE_EAP) {
				strncat(pos, "WPA-EAP", 7);
				pos += 7;
			}

			strncat(pos, "]", 1);
			pos += 1;
			sec++;				//go to next structure
		}
	}
	return;
}

static void resultStringEncode(char *txt, size_t maxlen, const u8 *data, size_t len)
{
	char *end = txt + maxlen;
	size_t i;

	for (i = 0; i < len; i++) {
		if (txt + 4 >= end) {
			break;
		}

		switch (data[i]) {
		case '\"':
			*txt++ = '\\';
			*txt++ = '\"';
			break;
		case '\\':
			*txt++ = '\\';
			*txt++ = '\\';
			break;
		case '\033':
			*txt++ = '\\';
			*txt++ = 'e';
			break;
		case '\n':
			*txt++ = '\\';
			*txt++ = 'n';
			break;
		case '\r':
			*txt++ = '\\';
			*txt++ = 'r';
			break;
		case '\t':
			*txt++ = '\\';
			*txt++ = 't';
			break;
		default:
			if (data[i] >= 32 && data[i] <= 127) {
				*txt++ = data[i];
			} else {
				txt += snprintf(txt, end - txt, "\\x%02x", data[i]);
			}
			break;
		}
	}

	*txt = '\0';
}

void printScanResult(slsi_scan_info_t *list)
{
	// if NULL scan finished
	int count = 0;
	char ssid[MAX_SSID_LEN];
	slsi_scan_info_t *current_element = list;
	printf("Scan Result - networks:\n");
	printf("          %-20.20s %-8.8s %-40.40s %-6.6s %s\n", "BSSID", "RSSI", "SECURITY", "CH", "SSID");
	while (current_element) {
		char modestring[MODE_STRING_MAX];
		count++;
		getSecurityModeString(current_element->sec_modes, current_element->num_sec_modes, modestring);

		resultStringEncode(ssid, MAX_SSID_LEN, current_element->ssid, current_element->ssid_len);
		printf("     %3.3d) %-20.20s %-8.8d %-40.40s %-6.6d %.32s \n", count, current_element->bssid, current_element->rssi, modestring, current_element->channel, ssid);
		current_element = current_element->next;
	}
}
