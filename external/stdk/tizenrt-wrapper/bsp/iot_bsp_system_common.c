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

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#ifdef CONFIG_ARCH_BOARD_ESP32_FAMILY
#include "esp_system.h"
#include "soc/efuse_reg.h"
#else
#include "iot_bsp_wifi.h"
#endif
#include "iot_bsp_system.h"
#include "iot_debug.h"

void iot_bsp_system_reboot()
{
#ifdef CONFIG_ARCH_BOARD_ESP32_FAMILY
	//it's unsupported in current libcore.a, comment it temporarily
	//esp_restart();
	IOT_ERROR("esp_restart called, but currently not supported.");
#else

#ifdef CONFIG_BOARDCTL_RESET
	board_reset(0);
#endif

#endif
}

void iot_bsp_system_poweroff()
{
#ifdef CONFIG_ARCH_BOARD_ESP32_FAMILY
	//esp_restart(); // no poweroff feature.
#else

#endif
}

iot_error_t iot_bsp_system_get_time_in_sec(char *buf, unsigned int buf_len)
{
	IOT_WARN_CHECK(buf == NULL, IOT_ERROR_INVALID_ARGS, "buffer for time is NULL");

	struct timeval tv = {0,};

	gettimeofday(&tv, NULL);
	snprintf(buf, buf_len, "%ld", tv.tv_sec);

	return IOT_ERROR_NONE;
}

static int settimeofday(struct timeval *tv, FAR struct timezone *tz)
{
	struct timespec ts;
	int ret;

	ts.tv_sec = tv->tv_sec;
	ts.tv_nsec = tv->tv_usec * NSEC_PER_USEC;

	/* Let clock_settime do most of the work */
	ret = clock_settime(CLOCK_REALTIME, &ts);
	
	return ret;
}

iot_error_t iot_bsp_system_set_time_in_sec(const char *time_in_sec)
{
	IOT_WARN_CHECK(time_in_sec == NULL, IOT_ERROR_INVALID_ARGS, "time data is NULL");

	struct timeval tv = {0,};

	sscanf(time_in_sec, "%ld", &tv.tv_sec);
	settimeofday(&tv, NULL);

	return IOT_ERROR_NONE;
}

iot_error_t iot_bsp_system_get_uniqueid(unsigned char **uid, size_t *olen)
{
	unsigned int *buf;
	size_t chipid_len = 2 * sizeof(unsigned int);
#ifdef CONFIG_ARCH_BOARD_ESP32_FAMILY
	unsigned int chipid_reg = EFUSE_BLK0_RDATA1_REG; /* CRC of MAC */
#else
	struct iot_mac umac;
#endif

	buf = (unsigned int *)malloc(chipid_len);
	if (buf == NULL) {
		IOT_ERROR("malloc failed for uid");
		return IOT_ERROR_MEM_ALLOC;
	}

#ifdef CONFIG_ARCH_BOARD_ESP32_FAMILY
	buf[0] = REG_READ(chipid_reg + 0x0);
	buf[1] = REG_READ(chipid_reg + 0x4);
#else
	if (iot_bsp_wifi_get_mac(&umac) != IOT_ERROR_NONE) {
		return IOT_ERROR_READ_FAIL;
	}
	buf[0] = umac.addr[0] << 24 | umac.addr[1] << 16 | umac.addr[2] << 8 | umac.addr[3];
	buf[1] = umac.addr[4] << 8 | umac.addr[5];
#endif

	*uid = (unsigned char *)buf;
	*olen = chipid_len;

	return IOT_ERROR_NONE;
}
