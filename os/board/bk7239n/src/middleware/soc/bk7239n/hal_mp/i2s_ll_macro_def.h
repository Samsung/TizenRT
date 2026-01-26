// Copyright 2020-2025 Beken
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

/***********************************************************************************************************************************
* This file is generated from BK7256_ADDR Mapping_20211224_format_change_highlight_20220113_update.xlsm automatically
* Modify it manually is not recommended
* CHIP ID:BK7256,GENARATE TIME:2022-02-09 17:28:47
************************************************************************************************************************************/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <soc/soc.h>
#include "i2s_reg.h"

static inline uint32_t i2s_ll_get_pcmcfg_value(void)
{
    return REG_READ(I2S_PCM_CONF_ADDR);
}

static inline void i2s_ll_set_pcmcfg_value(uint32_t value)
{
    REG_WRITE(I2S_PCM_CONF_ADDR,value);
}

static inline uint32_t i2s_ll_get_pcmcfg_bitratio(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value = ((reg_value >> I2S_BITRATIO_POS) & I2S_BITRATIO_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcfg_bitratio(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value &= ~(I2S_BITRATIO_MASK << I2S_BITRATIO_POS);
    reg_value |= ((value & I2S_BITRATIO_MASK) << I2S_BITRATIO_POS);
    REG_WRITE(I2S_PCM_CONF_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcfg_smpratio(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value = ((reg_value >> I2S_SMPRATIO_POS) & I2S_SMPRATIO_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcfg_smpratio(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value &= ~(I2S_SMPRATIO_MASK << I2S_SMPRATIO_POS);
    reg_value |= ((value & I2S_SMPRATIO_MASK) << I2S_SMPRATIO_POS);
    REG_WRITE(I2S_PCM_CONF_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcfg_pcm_dlen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value = ((reg_value >> I2S_PCM_DLEN_POS) & I2S_PCM_DLEN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcfg_pcm_dlen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value &= ~(I2S_PCM_DLEN_MASK << I2S_PCM_DLEN_POS);
    reg_value |= ((value & I2S_PCM_DLEN_MASK) << I2S_PCM_DLEN_POS);
    REG_WRITE(I2S_PCM_CONF_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcfg_datalen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value = ((reg_value >> I2S_DATALEN_POS) & I2S_DATALEN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcfg_datalen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value &= ~(I2S_DATALEN_MASK << I2S_DATALEN_POS);
    reg_value |= ((value & I2S_DATALEN_MASK) << I2S_DATALEN_POS);
    REG_WRITE(I2S_PCM_CONF_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcfg_synclen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value = ((reg_value >> I2S_SYNCLEN_POS) & I2S_SYNCLEN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcfg_synclen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value &= ~(I2S_SYNCLEN_MASK << I2S_SYNCLEN_POS);
    reg_value |= ((value & I2S_SYNCLEN_MASK) << I2S_SYNCLEN_POS);
    REG_WRITE(I2S_PCM_CONF_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcfg_lsbfirst(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value = ((reg_value >> I2S_LSBFIRST_POS) & I2S_LSBFIRST_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcfg_lsbfirst(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value &= ~(I2S_LSBFIRST_MASK << I2S_LSBFIRST_POS);
    reg_value |= ((value & I2S_LSBFIRST_MASK) << I2S_LSBFIRST_POS);
    REG_WRITE(I2S_PCM_CONF_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcfg_sclkinv(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value = ((reg_value >> I2S_SCLKINV_POS) & I2S_SCLKINV_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcfg_sclkinv(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value &= ~(I2S_SCLKINV_MASK << I2S_SCLKINV_POS);
    reg_value |= ((value & I2S_SCLKINV_MASK) << I2S_SCLKINV_POS);
    REG_WRITE(I2S_PCM_CONF_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcfg_lrckrp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value = ((reg_value >> I2S_LRCKRP_POS) & I2S_LRCKRP_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcfg_lrckrp(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value &= ~(I2S_LRCKRP_MASK << I2S_LRCKRP_POS);
    reg_value |= ((value & I2S_LRCKRP_MASK) << I2S_LRCKRP_POS);
    REG_WRITE(I2S_PCM_CONF_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcfg_modesel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value = ((reg_value >> I2S_MODESEL_POS) & I2S_MODESEL_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcfg_modesel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value &= ~(I2S_MODESEL_MASK << I2S_MODESEL_POS);
    reg_value |= ((value & I2S_MODESEL_MASK) << I2S_MODESEL_POS);
    REG_WRITE(I2S_PCM_CONF_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcfg_msten(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value = ((reg_value >> I2S_MSTEN_POS) & I2S_MSTEN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcfg_msten(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value &= ~(I2S_MSTEN_MASK << I2S_MSTEN_POS);
    reg_value |= ((value & I2S_MSTEN_MASK) << I2S_MSTEN_POS);
    REG_WRITE(I2S_PCM_CONF_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcfg_i2spcmen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value = ((reg_value >> I2S_I2SPCMEN_POS) & I2S_I2SPCMEN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcfg_i2spcmen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CONF_ADDR);
    reg_value &= ~(I2S_I2SPCMEN_MASK << I2S_I2SPCMEN_POS);
    reg_value |= ((value & I2S_I2SPCMEN_MASK) << I2S_I2SPCMEN_POS);
    REG_WRITE(I2S_PCM_CONF_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcn_value(void)
{
    return REG_READ(I2S_PCM_CN_ADDR);
}

static inline void i2s_ll_set_pcmcn_value(uint32_t value)
{
    REG_WRITE(I2S_PCM_CN_ADDR,value);
}

static inline uint32_t i2s_ll_get_pcmcn_rxint_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value = ((reg_value >> I2S_RXINT_EN_POS) & I2S_RXINT_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcn_rxint_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value &= ~(I2S_RXINT_EN_MASK << I2S_RXINT_EN_POS);
    reg_value |= ((value & I2S_RXINT_EN_MASK) << I2S_RXINT_EN_POS);
    REG_WRITE(I2S_PCM_CN_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcn_txint_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value = ((reg_value >> I2S_TXINT_EN_POS) & I2S_TXINT_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcn_txint_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value &= ~(I2S_TXINT_EN_MASK << I2S_TXINT_EN_POS);
    reg_value |= ((value & I2S_TXINT_EN_MASK) << I2S_TXINT_EN_POS);
    REG_WRITE(I2S_PCM_CN_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcn_rxovf_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value = ((reg_value >> I2S_RXOVF_EN_POS) & I2S_RXOVF_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcn_rxovf_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value &= ~(I2S_RXOVF_EN_MASK << I2S_RXOVF_EN_POS);
    reg_value |= ((value & I2S_RXOVF_EN_MASK) << I2S_RXOVF_EN_POS);
    REG_WRITE(I2S_PCM_CN_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcn_txudf_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value = ((reg_value >> I2S_TXUDF_EN_POS) & I2S_TXUDF_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcn_txudf_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value &= ~(I2S_TXUDF_EN_MASK << I2S_TXUDF_EN_POS);
    reg_value |= ((value & I2S_TXUDF_EN_MASK) << I2S_TXUDF_EN_POS);
    REG_WRITE(I2S_PCM_CN_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcn_rxint_level(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value = ((reg_value >> I2S_RXINT_LEVEL_POS) & I2S_RXINT_LEVEL_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcn_rxint_level(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value &= ~(I2S_RXINT_LEVEL_MASK << I2S_RXINT_LEVEL_POS);
    reg_value |= ((value & I2S_RXINT_LEVEL_MASK) << I2S_RXINT_LEVEL_POS);
    REG_WRITE(I2S_PCM_CN_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcn_txint_level(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value = ((reg_value >> I2S_TXINT_LEVEL_POS) & I2S_TXINT_LEVEL_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcn_txint_level(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value &= ~(I2S_TXINT_LEVEL_MASK << I2S_TXINT_LEVEL_POS);
    reg_value |= ((value & I2S_TXINT_LEVEL_MASK) << I2S_TXINT_LEVEL_POS);
    REG_WRITE(I2S_PCM_CN_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcn_txfifo_clr(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value = ((reg_value >> I2S_TXFIFO_CLR_POS) & I2S_TXFIFO_CLR_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcn_txfifo_clr(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value &= ~(I2S_TXFIFO_CLR_MASK << I2S_TXFIFO_CLR_POS);
    reg_value |= ((value & I2S_TXFIFO_CLR_MASK) << I2S_TXFIFO_CLR_POS);
    REG_WRITE(I2S_PCM_CN_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcn_rxfifo_clr(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value = ((reg_value >> I2S_RXFIFO_CLR_POS) & I2S_RXFIFO_CLR_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcn_rxfifo_clr(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value &= ~(I2S_RXFIFO_CLR_MASK << I2S_RXFIFO_CLR_POS);
    reg_value |= ((value & I2S_RXFIFO_CLR_MASK) << I2S_RXFIFO_CLR_POS);
    REG_WRITE(I2S_PCM_CN_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcn_smpratio_h2b(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value = ((reg_value >> I2S_SMPRATIO_H2B_POS) & I2S_SMPRATIO_H2B_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcn_smpratio_h2b(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value &= ~(I2S_SMPRATIO_H2B_MASK << I2S_SMPRATIO_H2B_POS);
    reg_value |= ((value & I2S_SMPRATIO_H2B_MASK) << I2S_SMPRATIO_H2B_POS);
    REG_WRITE(I2S_PCM_CN_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcn_bitratio_h4b(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value = ((reg_value >> I2S_BITRATIO_H4B_POS) & I2S_BITRATIO_H4B_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcn_bitratio_h4b(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value &= ~(I2S_BITRATIO_H4B_MASK << I2S_BITRATIO_H4B_POS);
    reg_value |= ((value & I2S_BITRATIO_H4B_MASK) << I2S_BITRATIO_H4B_POS);
    REG_WRITE(I2S_PCM_CN_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcn_lrcom_store(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value = ((reg_value >> I2S_LRCOM_STORE_POS) & I2S_LRCOM_STORE_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcn_lrcom_store(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value &= ~(I2S_LRCOM_STORE_MASK << I2S_LRCOM_STORE_POS);
    reg_value |= ((value & I2S_LRCOM_STORE_MASK) << I2S_LRCOM_STORE_POS);
    REG_WRITE(I2S_PCM_CN_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcn_parallel_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value = ((reg_value >> I2S_PARALLEL_EN_POS) & I2S_PARALLEL_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcn_parallel_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_ADDR);
    reg_value &= ~(I2S_PARALLEL_EN_MASK << I2S_PARALLEL_EN_POS);
    reg_value |= ((value & I2S_PARALLEL_EN_MASK) << I2S_PARALLEL_EN_POS);
    REG_WRITE(I2S_PCM_CN_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmstat_value(void)
{
    return REG_READ(I2S_PCM_STAT_ADDR);
}

static inline uint32_t i2s_ll_get_pcmstat_rxint(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_ADDR);
    reg_value = ((reg_value >> I2S_RXINT_POS) & I2S_RXINT_MASK);
    return reg_value;
}

static inline uint32_t i2s_ll_get_pcmstat_txint(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_ADDR);
    reg_value = ((reg_value >> I2S_TXINT_POS) & I2S_TXINT_MASK);
    return reg_value;
}

static inline uint32_t i2s_ll_get_pcmstat_rxovf(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_ADDR);
    reg_value = ((reg_value >> I2S_RXOVF_POS) & I2S_RXOVF_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmstat_rxovf(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_ADDR);
    reg_value &= ~(I2S_RXOVF_MASK << I2S_RXOVF_POS);
    reg_value |= ((value & I2S_RXOVF_MASK) << I2S_RXOVF_POS);
    REG_WRITE(I2S_PCM_STAT_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmstat_txudf(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_ADDR);
    reg_value = ((reg_value >> I2S_TXUDF_POS) & I2S_TXUDF_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmstat_txudf(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_ADDR);
    reg_value &= ~(I2S_TXUDF_MASK << I2S_TXUDF_POS);
    reg_value |= ((value & I2S_TXUDF_MASK) << I2S_TXUDF_POS);
    REG_WRITE(I2S_PCM_STAT_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmstat_rxfifo_rd_ready(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_ADDR);
    reg_value = ((reg_value >> I2S_RXFIFO_RD_READY_POS) & I2S_RXFIFO_RD_READY_MASK);
    return reg_value;
}

static inline uint32_t i2s_ll_get_pcmstat_txfifo_wr_ready(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_ADDR);
    reg_value = ((reg_value >> I2S_TXFIFO_WR_READY_POS) & I2S_TXFIFO_WR_READY_MASK);
    return reg_value;
}

static inline uint32_t i2s_ll_get_pcmdat_value(void)
{
    return REG_READ(I2S_PCM_DAT_ADDR);
}

static inline void i2s_ll_set_pcmdat_value(uint32_t value)
{
    REG_WRITE(I2S_PCM_DAT_ADDR,value);
}

static inline uint32_t i2s_ll_get_pcmdat_i2s_dat(void)
{
    return REG_READ(I2S_PCM_DAT_ADDR);
}

static inline void i2s_ll_set_pcmdat_i2s_dat(uint32_t value)
{
    REG_WRITE(I2S_PCM_DAT_ADDR,value);
}

static inline uint32_t i2s_ll_get_pcmcnlt2_value(void)
{
    return REG_READ(I2S_PCM_CN_LT2_ADDR);
}

static inline void i2s_ll_set_pcmcnlt2_value(uint32_t value)
{
    REG_WRITE(I2S_PCM_CN_LT2_ADDR,value);
}

static inline uint32_t i2s_ll_get_pcmcnlt2_rx2int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value = ((reg_value >> I2S_RX2INT_EN_POS) & I2S_RX2INT_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcnlt2_rx2int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value &= ~(I2S_RX2INT_EN_MASK << I2S_RX2INT_EN_POS);
    reg_value |= ((value & I2S_RX2INT_EN_MASK) << I2S_RX2INT_EN_POS);
    REG_WRITE(I2S_PCM_CN_LT2_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcnlt2_tx2int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value = ((reg_value >> I2S_TX2INT_EN_POS) & I2S_TX2INT_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcnlt2_tx2int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value &= ~(I2S_TX2INT_EN_MASK << I2S_TX2INT_EN_POS);
    reg_value |= ((value & I2S_TX2INT_EN_MASK) << I2S_TX2INT_EN_POS);
    REG_WRITE(I2S_PCM_CN_LT2_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcnlt2_rx2ovf_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value = ((reg_value >> I2S_RX2OVF_EN_POS) & I2S_RX2OVF_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcnlt2_rx2ovf_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value &= ~(I2S_RX2OVF_EN_MASK << I2S_RX2OVF_EN_POS);
    reg_value |= ((value & I2S_RX2OVF_EN_MASK) << I2S_RX2OVF_EN_POS);
    REG_WRITE(I2S_PCM_CN_LT2_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcnlt2_tx2udf_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value = ((reg_value >> I2S_TX2UDF_EN_POS) & I2S_TX2UDF_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcnlt2_tx2udf_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value &= ~(I2S_TX2UDF_EN_MASK << I2S_TX2UDF_EN_POS);
    reg_value |= ((value & I2S_TX2UDF_EN_MASK) << I2S_TX2UDF_EN_POS);
    REG_WRITE(I2S_PCM_CN_LT2_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcnlt2_rx3int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value = ((reg_value >> I2S_RX3INT_EN_POS) & I2S_RX3INT_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcnlt2_rx3int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value &= ~(I2S_RX3INT_EN_MASK << I2S_RX3INT_EN_POS);
    reg_value |= ((value & I2S_RX3INT_EN_MASK) << I2S_RX3INT_EN_POS);
    REG_WRITE(I2S_PCM_CN_LT2_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcnlt2_tx3int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value = ((reg_value >> I2S_TX3NT_EN_POS) & I2S_TX3NT_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcnlt2_tx3int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value &= ~(I2S_TX3NT_EN_MASK << I2S_TX3NT_EN_POS);
    reg_value |= ((value & I2S_TX3NT_EN_MASK) << I2S_TX3NT_EN_POS);
    REG_WRITE(I2S_PCM_CN_LT2_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcnlt2_rx3ovf_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value = ((reg_value >> I2S_RX3OVF_EN_POS) & I2S_RX3OVF_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcnlt2_rx3ovf_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value &= ~(I2S_RX3OVF_EN_MASK << I2S_RX3OVF_EN_POS);
    reg_value |= ((value & I2S_RX3OVF_EN_MASK) << I2S_RX3OVF_EN_POS);
    REG_WRITE(I2S_PCM_CN_LT2_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcnlt2_tx3udf_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value = ((reg_value >> I2S_TX3UDF_EN_POS) & I2S_TX3UDF_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcnlt2_tx3udf_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value &= ~(I2S_TX3UDF_EN_MASK << I2S_TX3UDF_EN_POS);
    reg_value |= ((value & I2S_TX3UDF_EN_MASK) << I2S_TX3UDF_EN_POS);
    REG_WRITE(I2S_PCM_CN_LT2_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcnlt2_rx4int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value = ((reg_value >> I2S_RX4INT_EN_POS) & I2S_RX4INT_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcnlt2_rx4int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value &= ~(I2S_RX4INT_EN_MASK << I2S_RX4INT_EN_POS);
    reg_value |= ((value & I2S_RX4INT_EN_MASK) << I2S_RX4INT_EN_POS);
    REG_WRITE(I2S_PCM_CN_LT2_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcnlt2_tx4int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value = ((reg_value >> I2S_TX4NT_EN_POS) & I2S_TX4NT_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcnlt2_tx4int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value &= ~(I2S_TX4NT_EN_MASK << I2S_TX4NT_EN_POS);
    reg_value |= ((value & I2S_TX4NT_EN_MASK) << I2S_TX4NT_EN_POS);
    REG_WRITE(I2S_PCM_CN_LT2_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcnlt2_rx4ovf_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value = ((reg_value >> I2S_RX4OVF_EN_POS) & I2S_RX4OVF_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcnlt2_rx4ovf_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value &= ~(I2S_RX4OVF_EN_MASK << I2S_RX4OVF_EN_POS);
    reg_value |= ((value & I2S_RX4OVF_EN_MASK) << I2S_RX4OVF_EN_POS);
    REG_WRITE(I2S_PCM_CN_LT2_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmcnlt2_tx4udf_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value = ((reg_value >> I2S_TX4UDF_EN_POS) & I2S_TX4UDF_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmcnlt2_tx4udf_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_CN_LT2_ADDR);
    reg_value &= ~(I2S_TX4UDF_EN_MASK << I2S_TX4UDF_EN_POS);
    reg_value |= ((value & I2S_TX4UDF_EN_MASK) << I2S_TX4UDF_EN_POS);
    REG_WRITE(I2S_PCM_CN_LT2_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmstatlt2_value(void)
{
    return REG_READ(I2S_PCM_STAT_LT2_ADDR);
}

static inline uint32_t i2s_ll_get_pcmstatlt2_rx2int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_LT2_ADDR);
    reg_value = ((reg_value >> I2S_RX2INT_POS) & I2S_RX2INT_MASK);
    return reg_value;
}

static inline uint32_t i2s_ll_get_pcmstatlt2_tx2int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_LT2_ADDR);
    reg_value = ((reg_value >> I2S_TX2INT_POS) & I2S_TX2INT_MASK);
    return reg_value;
}

static inline uint32_t i2s_ll_get_pcmstatlt2_rx2ovf(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_LT2_ADDR);
    reg_value = ((reg_value >> I2S_RX2OVF_POS) & I2S_RX2OVF_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmstatlt2_rx2ovf(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_LT2_ADDR);
    reg_value &= ~(I2S_RX2OVF_MASK << I2S_RX2OVF_POS);
    reg_value |= ((value & I2S_RX2OVF_MASK) << I2S_RX2OVF_POS);
    REG_WRITE(I2S_PCM_STAT_LT2_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmstatlt2_tx2udf(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_LT2_ADDR);
    reg_value = ((reg_value >> I2S_TX2UDF_POS) & I2S_TX2UDF_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmstatlt2_tx2udf(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_LT2_ADDR);
    reg_value &= ~(I2S_TX2UDF_MASK << I2S_TX2UDF_POS);
    reg_value |= ((value & I2S_TX2UDF_MASK) << I2S_TX2UDF_POS);
    REG_WRITE(I2S_PCM_STAT_LT2_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmstatlt2_rx3int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_LT2_ADDR);
    reg_value = ((reg_value >> I2S_RX3INT_POS) & I2S_RX3INT_MASK);
    return reg_value;
}

static inline uint32_t i2s_ll_get_pcmstatlt2_tx3int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_LT2_ADDR);
    reg_value = ((reg_value >> I2S_TX3INT_POS) & I2S_TX3INT_MASK);
    return reg_value;
}

static inline uint32_t i2s_ll_get_pcmstatlt2_rx3ovf(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_LT2_ADDR);
    reg_value = ((reg_value >> I2S_RX3OVF_POS) & I2S_RX3OVF_EN_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmstatlt2_rx3ovf(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_LT2_ADDR);
    reg_value &= ~(I2S_RX3OVF_EN_MASK << I2S_RX3OVF_POS);
    reg_value |= ((value & I2S_RX3OVF_EN_MASK) << I2S_RX3OVF_POS);
    REG_WRITE(I2S_PCM_STAT_LT2_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmstatlt2_tx3udf(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_LT2_ADDR);
    reg_value = ((reg_value >> I2S_TX3UDF_POS) & I2S_TX3UDF_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmstatlt2_tx3udf(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_LT2_ADDR);
    reg_value &= ~(I2S_TX3UDF_MASK << I2S_TX3UDF_POS);
    reg_value |= ((value & I2S_TX3UDF_MASK) << I2S_TX3UDF_POS);
    REG_WRITE(I2S_PCM_STAT_LT2_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmstatlt2_rx4int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_LT2_ADDR);
    reg_value = ((reg_value >> I2S_RX4INT_POS) & I2S_RX4INT_MASK);
    return reg_value;
}

static inline uint32_t i2s_ll_get_pcmstatlt2_tx4int(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_LT2_ADDR);
    reg_value = ((reg_value >> I2S_TX4INT_POS) & I2S_TX4INT_MASK);
    return reg_value;
}

static inline uint32_t i2s_ll_get_pcmstatlt2_rx4ovf(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_LT2_ADDR);
    reg_value = ((reg_value >> I2S_RX4OVF_POS) & I2S_RX4OVF_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmstatlt2_rx4ovf(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_LT2_ADDR);
    reg_value &= ~(I2S_RX4OVF_MASK << I2S_RX4OVF_POS);
    reg_value |= ((value & I2S_RX4OVF_MASK) << I2S_RX4OVF_POS);
    REG_WRITE(I2S_PCM_STAT_LT2_ADDR,reg_value);
}

static inline uint32_t i2s_ll_get_pcmstatlt2_tx4udf(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_LT2_ADDR);
    reg_value = ((reg_value >> I2S_TX4UDF_POS) & I2S_TX4UDF_MASK);
    return reg_value;
}

static inline void i2s_ll_set_pcmstatlt2_tx4udf(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(I2S_PCM_STAT_LT2_ADDR);
    reg_value &= ~(I2S_TX4UDF_MASK << I2S_TX4UDF_POS);
    reg_value |= ((value & I2S_TX4UDF_MASK) << I2S_TX4UDF_POS);
    REG_WRITE(I2S_PCM_STAT_LT2_ADDR,reg_value);
}

static inline void i2s_ll_set_pcmdat2_value(uint32_t value)
{
    REG_WRITE(I2S_PCM_DAT2_ADDR,value);
}

static inline void i2s_ll_set_pcmdat3_value(uint32_t value)
{
    REG_WRITE(I2S_PCM_DAT3_ADDR,value);
}

static inline void i2s_ll_set_pcmdat4_value(uint32_t value)
{
    REG_WRITE(I2S_PCM_DAT4_ADDR,value);
}

#ifdef __cplusplus
}
#endif
