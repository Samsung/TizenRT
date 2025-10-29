// Copyright 2022-2025 Beken
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

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!

#pragma once

#include <soc/soc.h>
#include "hal_port.h"
#include "i2s_hw.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef I2S_LL_REG_BASE
#define I2S_LL_REG_BASE   SOC_I2S_REG_BASE
#endif

typedef enum {
	I2S_CHANN_0,
	I2S_CHANN_1,
	I2S_CHANN_2,
	I2S_CHANN_MAX
} i2s_channel_idx;

static inline uint32_t i2s_ll_get_dev_addr_by_idx(i2s_channel_idx idx) {
	uint32_t addr;

	if (idx == I2S_CHANN_0) {
		addr = I2S_LL_REG_BASE;
	} else {
		addr = 0;
	}

	return addr;
}
//reg :

static inline uint32_t i2s_ll_get_smb_devid_value(i2s_channel_idx idx) {
	i2s_smb_devid_t *r = (i2s_smb_devid_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x0 << 2));
	return r->v;
}

//reg :

static inline uint32_t i2s_ll_get_smb_verid_value(i2s_channel_idx idx) {
	i2s_smb_verid_t *r = (i2s_smb_verid_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x1 << 2));
	return r->v;
}

//reg :

static inline void i2s_ll_set_smb_clkrst_value(uint32_t v, i2s_channel_idx idx) {
	i2s_smb_clkrst_t *r = (i2s_smb_clkrst_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x2 << 2));
	r->v = v;
}

static inline uint32_t i2s_ll_get_smb_clkrst_value(i2s_channel_idx idx) {
	i2s_smb_clkrst_t *r = (i2s_smb_clkrst_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x2 << 2));
	return r->v;
}

//reg :

static inline void i2s_ll_set_smb_state_value(uint32_t v, i2s_channel_idx idx) {
	i2s_smb_state_t *r = (i2s_smb_state_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x3 << 2));
	r->v = v;
}

static inline uint32_t i2s_ll_get_smb_state_value(i2s_channel_idx idx) {
	i2s_smb_state_t *r = (i2s_smb_state_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x3 << 2));
	return r->v;
}

//reg :

static inline void i2s_ll_set_pcm_cfg_value(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	r->v = v;
}

static inline uint32_t i2s_ll_get_pcm_cfg_value(i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	return r->v;
}

static inline void i2s_ll_set_pcm_cfg_bitratio(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	r->bitratio = v & 0xFF;
}

static inline uint32_t i2s_ll_get_pcm_cfg_bitratio(i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	return r->bitratio & 0xFF;
}

static inline void i2s_ll_set_pcm_cfg_smpratio(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	r->smpratio = v & 0x1F;
}

static inline uint32_t i2s_ll_get_pcm_cfg_smpratio(i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	return r->smpratio & 0x1F;
}

static inline void i2s_ll_set_pcm_cfg_pcm_dlen(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t *)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	r->pcm_dlen = v & 0x7;
}

static inline uint32_t i2s_ll_get_pcm_cfg_pcm_dlen(i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t *)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	return r->pcm_dlen & 0x7;
}

static inline void i2s_ll_set_pcm_cfg_datalen(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t *)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	r->datalen = v & 0x1F;
}

static inline uint32_t i2s_ll_get_pcm_cfg_datalen(i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t *)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	return r->datalen & 0x1F;
}

static inline void i2s_ll_set_pcm_cfg_synclen(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	r->synclen = v & 0x7;
}

static inline uint32_t i2s_ll_get_pcm_cfg_synclen(i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	return r->synclen & 0x7;
}

static inline void i2s_ll_set_pcm_cfg_lsbfirst(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	r->lsbfirst = v & 0x1;
}

static inline uint32_t i2s_ll_get_pcm_cfg_lsbfirst(i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	return r->lsbfirst & 0x1;
}

static inline void i2s_ll_set_pcm_cfg_sclkinv(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	r->sclkinv = v & 0x1;
}

static inline uint32_t i2s_ll_get_pcm_cfg_sclkinv(i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	return r->sclkinv & 0x1;
}

static inline void i2s_ll_set_pcm_cfg_lrckrp(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	r->lrckrp = v & 0x1;
}

static inline uint32_t i2s_ll_get_pcm_cfg_lrckrp(i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	return r->lrckrp & 0x1;
}

static inline void i2s_ll_set_pcm_cfg_modesel(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	r->modesel = v & 0x7;
}

static inline uint32_t i2s_ll_get_pcm_cfg_modesel(i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	return r->modesel & 0x7;
}

static inline void i2s_ll_set_pcm_cfg_msten(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	r->msten = v & 0x1;
}

static inline uint32_t i2s_ll_get_pcm_cfg_msten(i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	return r->msten & 0x1;
}

static inline void i2s_ll_set_pcm_cfg_i2spcmen(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	r->i2spcmen = v & 0x1;
}

static inline uint32_t i2s_ll_get_pcm_cfg_i2spcmen(i2s_channel_idx idx) {
	i2s_pcm_cfg_t *r = (i2s_pcm_cfg_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x4 << 2));
	return r->i2spcmen & 0x1;
}

//reg :

static inline void i2s_ll_set_pcm_cn_value(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	r->v = v;
}

static inline uint32_t i2s_ll_get_pcm_cn_value(i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	return r->v;
}

static inline void i2s_ll_set_pcm_cn_rxint_en(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	r->rxint_en = v & 0x1;
}

static inline uint32_t i2s_ll_get_pcm_cn_rxint_en(i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	return r->rxint_en & 0x1;
}

static inline void i2s_ll_set_pcm_cn_txint_en(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	r->txint_en = v & 0x1;
}

static inline uint32_t i2s_ll_get_pcm_cn_txint_en(i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	return r->txint_en & 0x1;
}

static inline void i2s_ll_set_pcm_cn_rxovf_en(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	r->rxovf_en = v & 0x1;
}

static inline uint32_t i2s_ll_get_pcm_cn_rxovf_en(i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	return r->rxovf_en & 0x1;
}

static inline void i2s_ll_set_pcm_cn_txudf_en(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	r->txudf_en = v & 0x1;
}

static inline uint32_t i2s_ll_get_pcm_cn_txudf_en(i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	return r->txudf_en & 0x1;
}

static inline void i2s_ll_set_pcm_cn_rxint_level(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	r->rxint_level = v & 0x3;
}

static inline uint32_t i2s_ll_get_pcm_cn_rxint_level(i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	return r->rxint_level & 0x3;
}

static inline void i2s_ll_set_pcm_cn_txint_level(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	r->txint_level = v & 0x3;
}

static inline uint32_t i2s_ll_get_pcm_cn_txint_level(i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	return r->txint_level & 0x3;
}

static inline void i2s_ll_set_pcm_cn_txfifo_clr(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	r->txfifo_clr = v & 0x1;
}

static inline uint32_t i2s_ll_get_pcm_cn_txfifo_clr(i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	return r->txfifo_clr & 0x1;
}

static inline void i2s_ll_set_pcm_cn_rxfifo_clr(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	r->rxfifo_clr = v & 0x1;
}

static inline uint32_t i2s_ll_get_pcm_cn_rxfifo_clr(i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	return r->rxfifo_clr & 0x1;
}

static inline void i2s_ll_set_pcm_cn_smpratio_h2b(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	r->smpratio_h2b = v & 0x3;
}

static inline uint32_t i2s_ll_get_pcm_cn_smpratio_h2b(i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	return r->smpratio_h2b & 0x3;
}

static inline void i2s_ll_set_pcm_cn_bitratio_h4b(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	r->bitratio_h4b = v & 0xF;
}

static inline uint32_t i2s_ll_get_pcm_cn_bitratio_h4b(i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	return r->bitratio_h4b & 0xF;
}

static inline void i2s_ll_set_pcm_cn_lrcom_store(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	r->lrcom_store = v & 0x1;
}

static inline uint32_t i2s_ll_get_pcm_cn_lrcom_store(i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	return r->lrcom_store & 0x1;
}

static inline void i2s_ll_set_pcm_cn_parallel_en(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	r->parallel_en = v & 0x1;
}

static inline uint32_t i2s_ll_get_pcm_cn_parallel_en(i2s_channel_idx idx) {
	i2s_pcm_cn_t *r = (i2s_pcm_cn_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x5 << 2));
	return r->parallel_en & 0x1;
}
//reg :

static inline void i2s_ll_set_pcm_stat_value(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_stat_t *r = (i2s_pcm_stat_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x6 << 2));
	r->v = v;
}

static inline uint32_t i2s_ll_get_pcm_stat_value(i2s_channel_idx idx) {
	i2s_pcm_stat_t *r = (i2s_pcm_stat_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x6 << 2));
	return r->v;
}

static inline uint32_t i2s_ll_get_pcm_stat_rxint(i2s_channel_idx idx) {
	i2s_pcm_stat_t *r = (i2s_pcm_stat_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x6 << 2));
	return r->rxint & 0x1;
}

static inline uint32_t i2s_ll_get_pcm_stat_txint(i2s_channel_idx idx) {
	i2s_pcm_stat_t *r = (i2s_pcm_stat_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x6 << 2));
	return r->txint & 0x1;
}

static inline void i2s_ll_clear_pcm_stat_rxovf(i2s_channel_idx idx) {
	i2s_pcm_stat_t *r = (i2s_pcm_stat_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x6 << 2));
	r->v = (I2S_RXOVF_MASK << I2S_RXOVF_POS);
}

static inline uint32_t i2s_ll_get_pcm_stat_rxovf(i2s_channel_idx idx) {
	i2s_pcm_stat_t *r = (i2s_pcm_stat_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x6 << 2));
	return r->rxovf & 0x1;
}

static inline void i2s_ll_clear_pcm_stat_txudf(i2s_channel_idx idx) {
	i2s_pcm_stat_t *r = (i2s_pcm_stat_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x6 << 2));
	r->v = (I2S_TXUDF_MASK << I2S_TXUDF_POS);
}

static inline uint32_t i2s_ll_get_pcm_stat_txudf(i2s_channel_idx idx) {
	i2s_pcm_stat_t *r = (i2s_pcm_stat_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x6 << 2));
	return r->txudf & 0x1;
}

static inline uint32_t i2s_ll_get_pcm_stat_rxfifo_rd_ready(i2s_channel_idx idx) {
	i2s_pcm_stat_t *r = (i2s_pcm_stat_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x6 << 2));
	return r->rxfifo_rd_ready & 0x1;
}

static inline uint32_t i2s_ll_get_pcm_stat_txfifo_wr_ready(i2s_channel_idx idx) {
	i2s_pcm_stat_t *r = (i2s_pcm_stat_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x6 << 2));
	return r->txfifo_wr_ready & 0x1;
}

//reg :

static inline void i2s_ll_set_pcm_dat_value(uint32_t v, i2s_channel_idx idx) {
	i2s_pcm_dat_t *r = (i2s_pcm_dat_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x7 << 2));
	r->v = v;
}

static inline uint32_t i2s_ll_get_pcm_dat_value(i2s_channel_idx idx) {
	i2s_pcm_dat_t *r = (i2s_pcm_dat_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x7 << 2));
	return r->v;
}

static inline uint32_t i2s_ll_get_pcm_dat_i2s_dat(i2s_channel_idx idx) {
	i2s_pcm_dat_t *r = (i2s_pcm_dat_t*)(i2s_ll_get_dev_addr_by_idx(idx) + (0x7 << 2));
	return r->i2s_dat;
}

#ifdef __cplusplus
}
#endif
