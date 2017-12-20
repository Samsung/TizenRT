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
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <errno.h>

#include <apps/netutils/wifi/slsi_wifi_api.h>
#include <apps/netutils/dhcpc.h>
#include <apps/netutils/netlib.h>

#include "ocf_mylight.h"

static sem_t _sem_scan;
static sem_t _sem_join;

static int _wifi_start(void)
{
	WiFi_InterFace_ID_t mode = SLSI_WIFI_NONE;
	int8_t result;

	result = WiFiGetOpMode(&mode);
	if (result != SLSI_STATUS_SUCCESS) {
		DBG("WiFiGetOpMode() failed (%d)", result);
		return -1;
	}

	if (mode == SLSI_WIFI_STATION_IF)
		return 0;

	if (mode != SLSI_WIFI_NONE) {
		DBG("WiFiStop()");
		result = WiFiStop();
		if (result != SLSI_STATUS_SUCCESS) {
			DBG("WiFiStop() failed (%d)", result);
			return -1;
		}
	}

	DBG("WiFiStart(SLSI_WIFI_STATION_IF)");
	result = WiFiStart(SLSI_WIFI_STATION_IF, NULL);
	if (result != SLSI_STATUS_SUCCESS) {
		DBG("WiFiStart() failed (%d)", result);
		return -1;
	}

	DBG("WiFiStart() done");

	return 0;
}

static int8_t _scan_cb(slsi_reason_t *reason)
{
	if (reason->reason_code == 0)
		DBG("scan success");
	else
		DBG("scan failed (reason: %d)", reason->reason_code);

	sem_post(&_sem_scan);

	return SLSI_STATUS_SUCCESS;
}

static int _wifi_scan(const char *ssid, size_t ssid_len)
{
	struct timespec timeout;
	int8_t result;
	slsi_scan_info_t *scan_results;
	slsi_scan_info_t *cur;

	result = WiFiRegisterScanCallback(_scan_cb);
	if (result != SLSI_STATUS_SUCCESS) {
		DBG("WiFiRegisterScanCallback() failed. (ret=%d)", result);
		return -1;
	}

	sem_init(&_sem_scan, 0, 0);

	DBG("Scan start...");
	result = WiFiScanNetwork();
	if (result != SLSI_STATUS_SUCCESS) {
		DBG("WiFiScanNetwork() failed. (ret=%d)", result);
		sem_destroy(&_sem_scan);
		return -1;
	}

	clock_gettime(CLOCK_REALTIME, &timeout);
	timeout.tv_sec += 30;
	if (sem_timedwait(&_sem_scan, &timeout) == -1) {
		if (errno == ETIMEDOUT)
			DBG("scan timeout");
		else
			DBG("sem_timedwait failed");
	}
	DBG("Scan done");

	sem_destroy(&_sem_scan);

	result = WiFiGetScanResults(&scan_results);
	if (result != SLSI_STATUS_SUCCESS) {
		DBG("WiFiGetScanResults() failed. (ret=%d)", result);
		return -1;
	}

	result = -1;
	cur = scan_results;
	while (cur) {
		MSG("Security-type: 0x%08X, SSID: %s (ssid_len=%d)",
				cur->sec_modes->secmode, cur->ssid,
				cur->ssid_len);
		if (ssid) {
			if (ssid_len == cur->ssid_len
					&& !memcmp(ssid, cur->ssid, ssid_len)) {
				DBG("- found");
				result = 0;
				break;
			}
		}
		cur = cur->next;
	}

	WiFiFreeScanResults(&scan_results);

	if (result == -1 && ssid)
		MSG("Can't find SSID(%s)", ssid);

	return result;
}

static void _link_up_cb(slsi_reason_t *reason)
{
	if (reason->reason_code == 0)
		DBG("Join success");
	else
		DBG("Join failed (reason: %d)", reason->reason_code);

	sem_post(&_sem_join);
}

static void _link_down_cb(slsi_reason_t *reason)
{
	DBG("Link DOWN (reason: %d)", reason->reason_code);
}

static int _wifi_join(const char *ssid, size_t ssid_len, const char *pwd,
		size_t pwd_len, int sec)
{
	struct timespec timeout;
	slsi_security_config_t config;
	int8_t result;

	if (pwd_len > SLSI_PASSPHRASE_LEN)
		pwd_len = SLSI_PASSPHRASE_LEN;

	result = WiFiRegisterLinkCallback(_link_up_cb, _link_down_cb);
	if (result != SLSI_STATUS_SUCCESS) {
		DBG("WiFiRegisterLinkCallback() failed. (ret=%d)", result);
		return -1;
	}

	sem_init(&_sem_join, 0, 0);

	memset(&config, 0, sizeof(slsi_security_config_t));
	config.secmode = sec;
	memcpy(config.passphrase, pwd, pwd_len);

	DBG("Join start... (ssid:%s, ssid_len:%d, passphrase:%s)", ssid,
			ssid_len, config.passphrase);

	result = WiFiNetworkJoin(ssid, ssid_len, NULL, &config);
	if (result != SLSI_STATUS_SUCCESS) {
		DBG("WiFiNetworkJoin() failed. (ret=%d)", result);
		return -1;
	}

	clock_gettime(CLOCK_REALTIME, &timeout);
	timeout.tv_sec += 40;
	if (sem_timedwait(&_sem_join, &timeout) == -1) {
		if (errno == ETIMEDOUT)
			DBG("timeout");
		else
			DBG("sem_timedwait failed");
	}

	DBG("Join done");

	sem_destroy(&_sem_join);

	return 0;
}

static int _dhcp_setup(void)
{
	void *handle;
	struct dhcpc_state state;
	int result;

	memset(&state, 0, sizeof(struct dhcpc_state));

	handle = dhcpc_open("wl1");
	result = dhcpc_request(handle, &state);
	dhcpc_close(handle);

	if (result != 0) {
		DBG("dhcpc_request() failed. (ret=%d)", result);
		return -1;
	}

	MSG("IP address\t%s", inet_ntoa(state.ipaddr));
	netlib_set_ipv4addr("wl1", &state.ipaddr);

	if (state.netmask.s_addr != 0) {
		MSG("Netmask\t\t%s", inet_ntoa(state.netmask));
		netlib_set_ipv4netmask("wl1", &state.netmask);
	}

	if (state.default_router.s_addr != 0) {
		MSG("Gateway\t\t%s", inet_ntoa(state.default_router));
		netlib_set_dripv4addr("wl1", &state.default_router);
	}

	return 0;
}

int ocf_mylight_wifi_scan(void)
{
	return _wifi_scan(NULL, 0);
}

int ocf_mylight_wifi_connect(void)
{
	const char *ssid;
	const char *pwd;
	int sec = 0;
	size_t ssid_len;
	size_t pwd_len;

	uint8_t count = 0;
	slsi_reason_t info;

	memset(&info, 0, sizeof(slsi_reason_t));
	if (WiFiIsConnected(&count, &info) == SLSI_STATUS_SUCCESS) {
		DBG("wifi already connected");
		return 0;
	}

	ssid = ocf_mylight_setup_get_wifi_ssid();
	ssid_len = strlen(ssid);
	if (ssid_len == 0) {
		DBG("no wifi information. skip wifi connection");
		return 0;
	}

	pwd = ocf_mylight_setup_get_wifi_pwd();
	pwd_len = strlen(pwd);

	sec = ocf_mylight_setup_get_wifi_security();
	if (sec == 0 && pwd_len > 0)
		sec = SLSI_SEC_MODE_WPA2_CCMP;

	DBG("ssid: %s, len: %d", ssid, ssid_len);
	DBG("pwd: %s: len: %d", pwd, pwd_len);
	DBG("sec: 0x%X", sec);

	if (_wifi_scan(ssid, ssid_len) < 0)
		return -1;

	if (_wifi_join(ssid, ssid_len, pwd, pwd_len, sec) < 0)
		return -1;

	if (_dhcp_setup() < 0)
		return -1;

	return 0;
}

int ocf_mylight_wifi_init(void)
{
	uint8_t count = 0;
	slsi_reason_t info;

	memset(&info, 0, sizeof(slsi_reason_t));
	if (WiFiIsConnected(&count, &info) == SLSI_STATUS_SUCCESS) {
		DBG("wifi already connected");
		return 0;
	}

	if (_wifi_start() < 0)
		return -1;

	return 0;
}

