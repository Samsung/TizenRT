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
 * @file artik_onboarding.h
 */

#ifndef _ARTIK_ONBOARDING_H_
#define _ARTIK_ONBOARDING_H_

#define ONBOARDING_VERSION "1.7.0"

/*
 * Service states
 */
enum ServiceState {
	STATE_IDLE,
	STATE_ONBOARDING,
	STATE_CONNECTING,
	STATE_CONNECTED
};

extern enum ServiceState current_service_state;

/*
 * LWM2M related exports
 */
struct Lwm2mState {
	char is_ota_update;
};

extern struct Lwm2mState lwm2m_state;

artik_error StartLwm2m(bool start);
void Lwm2mResetConfig(void);
/*
 * WiFi related exports
 */
#define SSID_MAX_LEN			64
#define PASSPHRASE_MAX_LEN		128
#define NTP_SERVER_MAX_LEN		64

struct WifiConfig {
	char ssid[SSID_MAX_LEN];
	char passphrase[PASSPHRASE_MAX_LEN];
	bool secure;
	char ntp_server[NTP_SERVER_MAX_LEN];
};

extern struct WifiConfig wifi_config;

char *WifiScanResult(void);
void WifiResetConfig(bool reset_ntp);
artik_error StartSoftAP(bool start);
artik_error StartStationConnection(bool start);
void StopWifi(void);
void StartMDNSService(bool start);

/*
 * ARTIK Cloud related exports
 */
#define AKC_DID_LEN		32
#define AKC_TOKEN_LEN		32
#define AKC_DTID_LEN		34
#define AKC_REG_ID_LEN		32
#define AKC_REG_NONCE_LEN	32
#define AKC_VDID_LEN		32

struct ArtikCloudConfig {
	char device_id[AKC_DID_LEN + 1];
	char device_token[AKC_TOKEN_LEN + 1];
	char device_type_id[AKC_DTID_LEN + 1];
	char reg_id[AKC_REG_ID_LEN + 1];
	char reg_nonce[AKC_REG_NONCE_LEN + 1];
};

extern struct ArtikCloudConfig cloud_config;
extern bool cloud_secure_dt;

void CloudResetConfig(bool reset_dtid);
artik_error StartCloudWebsocket(bool start);
artik_error SendMessageToCloud(char *message);
int StartSDRRegistration(char **resp);
int CompleteSDRRegistration(char **resp);
bool CloudIsSecureDeviceType(const char *dtid);

/*
 * Web server related exports
 */
enum ApiSet {
	API_SET_WIFI = (1 << 0),
	API_SET_CLOUD = (1 << 1)
};

artik_error StartWebServer(bool start, enum ApiSet);

/*
 * Configuration Web server related exports
 */
artik_error InitConfiguration(void);
artik_error SaveConfiguration(void);
artik_error ResetConfiguration(bool force);
void PrintConfiguration(void);

#define API_ERROR_OK				"0"
#define API_ERROR_INVALID_JSON		"100"
#define API_ERROR_INTERNAL			"102"
#define API_ERROR_INVALID_PARAMS	"106"
#define API_ERROR_INVALID_UUID		"108"
#define API_ERROR_MISSING_PARAM	"109"
#define API_ERROR_COMMUNICATION	"111"
#define API_ERROR_CLOUD_BASE		50000

#define RESP_ERROR_OK									\
	"{\"error\":false,"								\
	"\"error_code\":0,"								\
	"\"reason\":\"none\"}"

#define RESP_ERROR(code, reason)						\
	"{\"error\":true,"									\
	"\"error_code\":"code","							\
	"\"reason\":\""reason"\"}"

#define RESP_ERROR_EXTRA(code, reason, extra) "{\"error\":false,"	\
	"\"error_code\":"code","										\
	"\"reason\":\""reason"\","										\
	extra"}"

#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))

#endif /* _ARTIK_ONBOARDING_H_ */
