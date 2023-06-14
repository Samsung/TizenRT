/*
 * Copyright (c) 2022 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "ameba_audio_stream.h"

#include <inttypes.h>

#include "audio_hw_debug.h"

typedef struct _AudioIpInfo {
	uint32_t idx;
	/*if specific bit set 1, means tx is using it, if set it 0, means no tx using it*/
	uint32_t tx_in_use;
	/*if specific bit set 1, means rx is using it, if set it 0, means no rx using it*/
	uint32_t rx_in_use;
} AudioIpInfo;

AudioIpInfo g_audio_ip_info[] = {
	{ AUDIOIP,      TX_AUDIO_IP_IN_USE, RX_AUDIO_IP_IN_USE},
	{ POWER,        TX_POWER_IN_USE,    RX_POWER_IN_USE},
	{ SPORT0,       TX_SPORT0_IN_USE,   RX_SPORT0_IN_USE},
	{ SPORT1,       TX_SPORT1_IN_USE,   RX_SPORT1_IN_USE},
	{ CODEC,        TX_CODEC_IN_USE,    RX_CODEC_IN_USE},
	{ IIS0,         TX_I2S0_IN_USE,     RX_I2S0_IN_USE},
	{ IIS1,         TX_I2S1_IN_USE,     RX_I2S1_IN_USE},
	{ LINEOUTLANA,  TX_ANAL_IN_USE,     0},
	{ LINEOUTRANA,  TX_ANAR_IN_USE,     0},
	{ HPOOUTLANA,   TX_HPO_ANAL_IN_USE, 0},
	{ HPOOUTRANA,   TX_HPO_ANAR_IN_USE, 0},
};

static uint32_t g_audio_ip_cnt = (sizeof((g_audio_ip_info)) / sizeof((g_audio_ip_info[0])));

static uint32_t g_audio_ip_status = 0;

void ameba_audio_set_audio_ip_use_status(uint32_t direction, uint32_t audio_ip, bool is_used)
{
	uint32_t i = 0;
	if (audio_ip > g_audio_ip_cnt) {
		return;
	}

	for (; i < g_audio_ip_cnt; i++) {
		if (g_audio_ip_info[i].idx == audio_ip) {
			break;
		}
	}

	if (i == g_audio_ip_cnt) {
		HAL_AUDIO_ERROR("no matching audio ip!");
		return;
	}

	if (direction == STREAM_OUT) {
		//HAL_AUDIO_INFO("tx_in_use: %lu, is_used: %d", g_audio_ip_info[i].tx_in_use, is_used);
		if (is_used) {
			g_audio_ip_status |= g_audio_ip_info[i].tx_in_use;
		} else {
			g_audio_ip_status &= ~g_audio_ip_info[i].tx_in_use;
		}
	} else {
		//HAL_AUDIO_INFO("rx_in_use: %lu, is_used: %d", g_audio_ip_info[i].tx_in_use, is_used);
		if (is_used) {
			g_audio_ip_status |= g_audio_ip_info[i].rx_in_use;
		} else {
			g_audio_ip_status &= ~g_audio_ip_info[i].rx_in_use;
		}
	}

}

bool ameba_audio_is_audio_ip_in_use(uint32_t audio_ip)
{
	uint32_t i = 0;
	if (audio_ip > g_audio_ip_cnt) {
		return false;
	}

	for (; i < g_audio_ip_cnt; i++) {
		if (g_audio_ip_info[i].idx == audio_ip) {
			break;
		}
	}

	if (i == g_audio_ip_cnt) {
		HAL_AUDIO_ERROR("no matching audio ip!");
		return false;
	}

	return (g_audio_ip_status & g_audio_ip_info[i].tx_in_use) != 0 || (g_audio_ip_status & g_audio_ip_info[i].rx_in_use) != 0;
}

void ameba_audio_periphclock_init(uint32_t sport_num)
{
	if (sport_num == 0 && !ameba_audio_is_audio_ip_in_use(SPORT0)) {
		RCC_PeriphClockCmd(APBPeriph_SPORT0, APBPeriph_SPORT0_CLOCK, ENABLE);
	} else if (sport_num == 1 && !ameba_audio_is_audio_ip_in_use(SPORT1)) {
		RCC_PeriphClockCmd(APBPeriph_SPORT1, APBPeriph_SPORT1_CLOCK, ENABLE);
	}

	RCC_PeriphClockSource_SPORT(sport_num, CKSL_I2S_XTAL40M);

	if (!ameba_audio_is_audio_ip_in_use(CODEC) && !ameba_audio_is_audio_ip_in_use(POWER)) {
		RCC_PeriphClockCmd(APBPeriph_AC, APBPeriph_AC_CLOCK, ENABLE);
		RCC_PeriphClockCmd(APBPeriph_AUDIO, NULL, ENABLE);
		RCC_PeriphClockSource_AUDIOCODEC(CKSL_AC_XTAL);
	}

}

void ameba_audio_reset_audio_ip_status(Stream *stream)
{
	if (!stream) {
		return;
	}

	if (stream->sport_dev_num == 0) {
		ameba_audio_set_audio_ip_use_status(stream->direction, SPORT0, false);
	} else if (stream->sport_dev_num == 1) {
		ameba_audio_set_audio_ip_use_status(stream->direction, SPORT1, false);
	}

	ameba_audio_set_audio_ip_use_status(stream->direction, CODEC, false);
	ameba_audio_set_audio_ip_use_status(stream->direction, AUDIOIP, false);

	//keep power on and lineout analog on, even if tx close.
	//ameba_audio_set_audio_ip_use_status(stream->direction, POWER, false);
	//ameba_audio_set_audio_ip_use_status(stream->direction, LINEOUTANA, false);
	if (!ameba_audio_is_audio_ip_in_use(CODEC) && !ameba_audio_is_audio_ip_in_use(POWER)) {
		RCC_PeriphClockCmd(APBPeriph_AUDIO, NULL, DISABLE);
		RCC_PeriphClockCmd(APBPeriph_AC, APBPeriph_AC_CLOCK, DISABLE);
	}

	if (stream->sport_dev_num == 0 && !ameba_audio_is_audio_ip_in_use(SPORT0)) {
		RCC_PeriphClockCmd(APBPeriph_SPORT0, APBPeriph_SPORT0_CLOCK, DISABLE);
	}

	if (stream->sport_dev_num == 1 && !ameba_audio_is_audio_ip_in_use(SPORT1)) {
		RCC_PeriphClockCmd(APBPeriph_SPORT1, APBPeriph_SPORT1_CLOCK, DISABLE);
	}

	HAL_AUDIO_INFO("audioIP status: %" PRIu32 "", g_audio_ip_status);
}
