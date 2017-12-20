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
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>

#include "ocf_mylight.h"

#define DEFAULT_NAME "TestLight"
#define DEFAULT_TIMEZONE "KST"

struct setup_data {
	char name[64]; /* oic.wk.con:n */

	char wifi_ssid[32];
	char wifi_pwd[64];
	int wifi_sec;

	unsigned char macaddr[6];
	OCUUIdentity uuid;

	/**
	 * 05x do not have RTC battery, so time is reset on every reboot.
	 * if NTP is not available, use fallback_time to system time
	 */
	time_t failback_time;

	char timezone[10];
};

static struct setup_data _sd;

static int _get_mac_address(unsigned char *output)
{
	struct ifreq ifr;
	struct ifconf ifc;
	char buf[1024];
	int sock;
	struct ifreq *it;
	struct ifreq *end;

	if (!output)
		return -1;

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (sock < 0)
		return -1;

	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = buf;
	if (ioctl(sock, SIOCGIFCONF, &ifc) == -1)
		return -1;

	it = ifc.ifc_req;
	end = it + (ifc.ifc_len / sizeof(struct ifreq));

	for (; it != end; ++it) {
		strcpy(ifr.ifr_name, it->ifr_name);
		if (ioctl(sock, SIOCGIFFLAGS, &ifr) != 0)
			continue;

		if (ifr.ifr_flags & IFF_LOOPBACK)
			continue;

		if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
			memcpy(output, ifr.ifr_hwaddr.sa_data, 6);
			return 0;
		}
	}

	return -1;
}

const char *ocf_mylight_setup_get_name(void)
{
	return _sd.name;
}

const char *ocf_mylight_setup_get_wifi_ssid(void)
{
	return _sd.wifi_ssid;
}

const char *ocf_mylight_setup_get_wifi_pwd(void)
{
	return _sd.wifi_pwd;
}

int ocf_mylight_setup_get_wifi_security(void)
{
	return _sd.wifi_sec;
}

const OCUUIdentity *ocf_mylight_setup_get_uuid(void)
{
	return &_sd.uuid;
}

int ocf_mylight_setup_set_name(const char *name, size_t len)
{
	if (!name)
		return -1;

	OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DEVICE_NAME, name);

	memset(_sd.name, 0, 64);
	memcpy(_sd.name, name, len);

	return 0;
}

int ocf_mylight_setup_set_wifi_ssid(const char *ssid, size_t len)
{
	if (!ssid)
		return -1;

	memset(_sd.wifi_ssid, 0, 32);
	memcpy(_sd.wifi_ssid, ssid, len);

	return 0;
}

int ocf_mylight_setup_set_wifi_pwd(const char *pwd, size_t len)
{
	/* allow empty password */
	memset(_sd.wifi_pwd, 0, 64);

	if (!pwd)
		return 0;

	memcpy(_sd.wifi_pwd, pwd, len);

	return 0;
}

int ocf_mylight_setup_set_wifi_security(int security)
{
	_sd.wifi_sec = security;

	return 0;
}

int ocf_mylight_setup_set_failback_time(time_t value)
{
	_sd.failback_time = value;

	return 0;
}

time_t ocf_mylight_setup_get_failback_time(void)
{
	return _sd.failback_time;
}

int ocf_mylight_setup_set_timezone(const char *zone, size_t len)
{
	if (!zone)
		return -1;

	memset(_sd.timezone, 0, 10);
	memcpy(_sd.timezone, zone, len);

	return 0;
}

const char *ocf_mylight_setup_get_timezone(void)
{
	return _sd.timezone;
}

void ocf_mylight_setup_reset(void)
{
	OCUUIdentity uuid = {
		.id = {
			0xC0, 0xFF, 0xEE, 0x00, 0xBA, 0xB0, 0xFA, 0xCE, 0xB0,
			0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
		}
	};

	DBG("Reset setup info to default");
	memset(&_sd, 0, sizeof(struct setup_data));

	_get_mac_address(_sd.macaddr);

	/*
	 * Device UUID
	 *
	 * C0 FF EE 00 BA B0 FA CE B0 0C __ __ __ __ __ __
	 *                                  mac address
	 */
	memcpy(uuid.id + 10, _sd.macaddr, 6);
	memcpy(&_sd.uuid, &uuid, sizeof(OCUUIdentity));

	snprintf(_sd.name, 64, "%s_%02X%02X", DEFAULT_NAME, _sd.macaddr[4],
		_sd.macaddr[5]);

	memcpy(_sd.timezone, DEFAULT_TIMEZONE, strlen(DEFAULT_TIMEZONE));
}

static void _print_setup_info(void)
{
	printf(" - Device name: '%s'\n", _sd.name);
	printf(" - WiFi: SSID(%s), PWD(%s), SecurityType(0x%X)\n",
			_sd.wifi_ssid, _sd.wifi_pwd, _sd.wifi_sec);
	printf(" - Failback time: %zd\n", _sd.failback_time);
#ifdef CONFIG_LIBC_LOCALTIME
	printf(" - Timezone: %s\n", _sd.timezone);
#endif
}

int ocf_mylight_setup_load(void)
{
	FILE *fp;
	size_t nread;
	struct setup_data tmp;

	fp = fopen(SETUP_INFO_PATH, "rb");
	if (!fp) {
		ocf_mylight_setup_reset();
		return ocf_mylight_setup_save();
	}

	nread = fread(&tmp, 1, sizeof(struct setup_data), fp);
	if (nread != sizeof(struct setup_data)) {
		DBG("invalid data. reset to default");
		fclose(fp);
		ocf_mylight_setup_reset();
		return ocf_mylight_setup_save();
	}

	memcpy(&_sd, &tmp, sizeof(struct setup_data));

	fclose(fp);

	printf("Setup information loaded\n");
	_print_setup_info();

	return 0;
}

int ocf_mylight_setup_save(void)
{
	FILE *fp;
	size_t nwrite;

	fp = fopen(SETUP_INFO_PATH, "wb");
	if (!fp) {
		DBG("fopen(%s) failed.", SETUP_INFO_PATH);
		return -1;
	}

	nwrite = fwrite(&_sd, 1, sizeof(struct setup_data), fp);
	if (nwrite == 0) {
		DBG("fwrite() failed.");
		fclose(fp);
		return -1;
	}

	if (nwrite != sizeof(struct setup_data)) {
		DBG("invalid fwrite()");
		fclose(fp);
		return -1;
	}

	fclose(fp);

	printf("Setup information saved\n");
	_print_setup_info();

	return 0;
}
