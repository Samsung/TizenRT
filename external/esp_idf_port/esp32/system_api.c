/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

/****************************************************************************
 *
 *   Copyright (C) 2014-2016 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * The Samsung sample code has a BSD compatible license that requires this
 * copyright notice:
 *
 *   Copyright (c) 2016 Samsung Electronics, Inc.
 *   All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
// Copyright 2013-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <tinyara/sched.h>

#include "esp_attr.h"
#include <arch/chip/esp_err.h>
#include "chip/esp32_soc.h"
#include "soc/efuse_reg.h"
#include "esp_system.h"
#include "esp_log.h"

static const char* TAG = "system_api";

#define SHUTDOWN_HANDLERS_NO 2
static shutdown_handler_t shutdown_handlers[SHUTDOWN_HANDLERS_NO];
extern uint8_t esp_crc8(uint8_t *, int len);
extern uint32_t xthal_get_ccount(void);

static uint8_t base_mac_addr[6] = { 0 };

int smp_processor_id(void)
{
#ifdef CONFIG_SMP
	return up_cpu_index();
#else
	return 0;					// PRO CPU
#endif

}

esp_err_t esp_base_mac_addr_get(uint8_t *mac)
{
	uint8_t null_mac[6] = { 0 };

	if (memcmp(base_mac_addr, null_mac, 6) == 0) {
		ESP_LOGI(TAG, "Base MAC address is not set, read default base MAC address from BLK0 of EFUSE");
		return ESP_ERR_INVALID_MAC;
	}

	memcpy(mac, base_mac_addr, 6);

	return ESP_OK;
}

esp_err_t esp_efuse_mac_get_default(uint8_t *mac)
{
	uint32_t mac_low;
	uint32_t mac_high;
	uint8_t efuse_crc;
	uint8_t calc_crc;

	mac_low = REG_READ(EFUSE_BLK0_RDATA1_REG);
	mac_high = REG_READ(EFUSE_BLK0_RDATA2_REG);

	mac[0] = mac_high >> 8;
	mac[1] = mac_high;
	mac[2] = mac_low >> 24;
	mac[3] = mac_low >> 16;
	mac[4] = mac_low >> 8;
	mac[5] = mac_low;

	efuse_crc = mac_high >> 16;

	calc_crc = esp_crc8(mac, 6);

	if (efuse_crc != calc_crc) {
		// Small range of MAC addresses are accepted even if CRC is invalid.
		// These addresses are reserved for Espressif internal use.
		if ((mac_high & 0xFFFF) == 0x18fe) {
			if ((mac_low >= 0x346a85c7) && (mac_low <= 0x346a85f8)) {
				return ESP_OK;
			}
		} else {
			ESP_LOGE(TAG, "Base MAC address from BLK0 of EFUSE CRC error, efuse_crc = 0x%02x; calc_crc = 0x%02x", efuse_crc, calc_crc);
			abort();
		}
	}
	return ESP_OK;
}

esp_err_t system_efuse_read_mac(uint8_t *mac) __attribute__((alias("esp_efuse_mac_get_default")));
esp_err_t esp_efuse_read_mac(uint8_t *mac) __attribute__((alias("esp_efuse_mac_get_default")));

esp_err_t esp_derive_mac(uint8_t *local_mac, const uint8_t *universal_mac)
{
	uint8_t idx;

	if (local_mac == NULL || universal_mac == NULL) {
		ESP_LOGE(TAG, "mac address param is NULL");
		return ESP_ERR_INVALID_ARG;
	}

	memcpy(local_mac, universal_mac, 6);
	for (idx = 0; idx < 64; idx++) {
		local_mac[0] = universal_mac[0] | 0x02;
		local_mac[0] ^= idx << 2;

		if (memcmp(local_mac, universal_mac, 6)) {
			break;
		}
	}

	return ESP_OK;
}

esp_err_t esp_read_mac(uint8_t *mac, esp_mac_type_t type)
{
	uint8_t efuse_mac[6];

	if (mac == NULL) {
		ESP_LOGE(TAG, "mac address param is NULL");
		return ESP_ERR_INVALID_ARG;
	}

	if (type < ESP_MAC_WIFI_STA || type > ESP_MAC_ETH) {
		ESP_LOGE(TAG, "mac type is incorrect");
		return ESP_ERR_INVALID_ARG;
	}

	_Static_assert(UNIVERSAL_MAC_ADDR_NUM == FOUR_UNIVERSAL_MAC_ADDR || UNIVERSAL_MAC_ADDR_NUM == TWO_UNIVERSAL_MAC_ADDR, "incorrect NUM_MAC_ADDRESS_FROM_EFUSE value");

	if (esp_base_mac_addr_get(efuse_mac) != ESP_OK) {
		esp_efuse_mac_get_default(efuse_mac);
	}

	switch (type) {
	case ESP_MAC_WIFI_STA:
		memcpy(mac, efuse_mac, 6);
		break;
	case ESP_MAC_WIFI_SOFTAP:
		if (UNIVERSAL_MAC_ADDR_NUM == FOUR_UNIVERSAL_MAC_ADDR) {
			memcpy(mac, efuse_mac, 6);
			mac[5] += 1;
		} else if (UNIVERSAL_MAC_ADDR_NUM == TWO_UNIVERSAL_MAC_ADDR) {
			esp_derive_mac(mac, efuse_mac);
		}
		break;
	case ESP_MAC_BT:
		memcpy(mac, efuse_mac, 6);
		if (UNIVERSAL_MAC_ADDR_NUM == FOUR_UNIVERSAL_MAC_ADDR) {
			mac[5] += 2;
		} else if (UNIVERSAL_MAC_ADDR_NUM == TWO_UNIVERSAL_MAC_ADDR) {
			mac[5] += 1;
		}
		break;
	case ESP_MAC_ETH:
		if (UNIVERSAL_MAC_ADDR_NUM == FOUR_UNIVERSAL_MAC_ADDR) {
			memcpy(mac, efuse_mac, 6);
			mac[5] += 3;
		} else if (UNIVERSAL_MAC_ADDR_NUM == TWO_UNIVERSAL_MAC_ADDR) {
			efuse_mac[5] += 1;
			esp_derive_mac(mac, efuse_mac);
		}
		break;
	default:
		ESP_LOGW(TAG, "incorrect mac type");
		break;
	}

	return ESP_OK;
}

esp_err_t esp_register_shutdown_handler(shutdown_handler_t handler)
{
	int i;
	for (i = 0; i < SHUTDOWN_HANDLERS_NO; i++) {
		if (shutdown_handlers[i] == NULL) {
			shutdown_handlers[i] = handler;
			return ESP_OK;
		}
	}
	return ESP_FAIL;
}

#if 0
extern uint32_t g_ticks_per_us_pro;

uint32_t esp_log_early_timestamp(void)
{
	return xthal_get_ccount() / (g_ticks_per_us_pro * 1000);
}

uint32_t IRAM_ATTR esp_log_timestamp()
{
	int pid = getpid();
	FAR struct tcb_s *ctcb = sched_gettcb(pid);
	uint8_t state = ctcb->task_state;

	if (state == TSTATE_TASK_INVALID) {
		return esp_log_early_timestamp();
	}
	static uint32_t base = 0;
	if (base == 0) {
		base = esp_log_early_timestamp();
	}
	return base + clock_systimer() * (1000 / CLK_TCK);
}
#endif

#ifndef XTSTR
#define _XTSTR(x)   # x
#define XTSTR(x)    _XTSTR(x)
#endif

#define XCHAL_EXCM_LEVEL 3
#define XTOS_SET_INTLEVEL(intlevel)        ({ unsigned __tmp; \
            __asm__ __volatile__(   "rsil   %0, " XTSTR(intlevel) "\n" \
                        : "=a" (__tmp) : : "memory" ); \
            __tmp;})

#define XTOS_RESTORE_INTLEVEL(restoreval)  do{ unsigned __tmp = (restoreval); \
            __asm__ __volatile__(   "wsr    %0, " XTSTR(PS) " ; rsync\n" \
                        : : "a" (__tmp) : "memory" ); \
            }while(0)

unsigned portENTER_CRITICAL_NESTED(void)
{
	unsigned state = XTOS_SET_INTLEVEL(XCHAL_EXCM_LEVEL);
	up_irq_disable();
	return state;
}

void portEXIT_CRITICAL_NESTED(unsigned state)
{
	up_irq_enable();
	XTOS_RESTORE_INTLEVEL(state);
}
