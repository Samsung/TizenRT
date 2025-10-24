// Copyright 2020-2021 Beken 
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
* This file is generated from BK7236_ADDR_Mapping_s.xlsm automatically                                
* Modify it manually is not recommended                                       
* CHIP ID:BK7258,GENARATE TIME:2023-09-21 10:12:45                                                 
************************************************************************************************************************************/

#pragma once                 
                            
#ifdef __cplusplus          
extern "C" {              
#endif                      

#include <soc/soc.h>          

#define AUD_LL_REG_BASE      (SOC_AUD_REG_BASE) //REG_BASE:0x47800000

/* REG_0x00 */
#define AUD_DEVICE_ID_ADDR  (AUD_LL_REG_BASE  + 0x0*4) //REG ADDR :0x47800000
#define AUD_DEVICE_ID_DEVICE_ID_POS (0) 
#define AUD_DEVICE_ID_DEVICE_ID_MASK (0xFFFFFFFF) 

static inline uint32_t aud_ll_get_device_id_value(void)
{
    return REG_READ(AUD_DEVICE_ID_ADDR);
}

/* REG_0x00:device_id->device_id:0x0[31:0],Device ID:ASCII Code "aud",0x00415544,R*/
static inline uint32_t aud_ll_get_device_id_device_id(void)
{
    return REG_READ(AUD_DEVICE_ID_ADDR);
}

/* REG_0x01 */
#define AUD_VERSION_ID_ADDR  (AUD_LL_REG_BASE  + 0x1*4) //REG ADDR :0x47800004
#define AUD_VERSION_ID_VERSION_ID_POS (0) 
#define AUD_VERSION_ID_VERSION_ID_MASK (0xFFFFFFFF) 

static inline uint32_t aud_ll_get_version_id_value(void)
{
    return REG_READ(AUD_VERSION_ID_ADDR);
}

/* REG_0x01:version_id->version_id:0x1[31:0],Version ID: 版本号 V1.1,0x00010001,R*/
static inline uint32_t aud_ll_get_version_id_version_id(void)
{
    return REG_READ(AUD_VERSION_ID_ADDR);
}

/* REG_0x02 */
#define AUD_CLK_CONTROL_ADDR  (AUD_LL_REG_BASE  + 0x2*4) //REG ADDR :0x47800008
#define AUD_CLK_CONTROL_SOFT_RESET_POS (0) 
#define AUD_CLK_CONTROL_SOFT_RESET_MASK (0x1) 

#define AUD_CLK_CONTROL_CLK_GATE_POS (1) 
#define AUD_CLK_CONTROL_CLK_GATE_MASK (0x1) 

#define AUD_CLK_CONTROL_RESERVED0_POS (2) 
#define AUD_CLK_CONTROL_RESERVED0_MASK (0x3FFFFFFF) 

static inline uint32_t aud_ll_get_clk_control_value(void)
{
    return REG_READ(AUD_CLK_CONTROL_ADDR);
}

static inline void aud_ll_set_clk_control_value(uint32_t value)
{
    REG_WRITE(AUD_CLK_CONTROL_ADDR,value);
}

/* REG_0x02:clk_control->soft_reset:0x2[0],软件写1复位SPI模块，需要软件清除,0x0,R/W*/
static inline uint32_t aud_ll_get_clk_control_soft_reset(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_CLK_CONTROL_ADDR);
    reg_value = ((reg_value >> AUD_CLK_CONTROL_SOFT_RESET_POS) & AUD_CLK_CONTROL_SOFT_RESET_MASK);
    return reg_value;
}

static inline void aud_ll_set_clk_control_soft_reset(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_CLK_CONTROL_ADDR);
    reg_value &= ~(AUD_CLK_CONTROL_SOFT_RESET_MASK << AUD_CLK_CONTROL_SOFT_RESET_POS);
    reg_value |= ((value & AUD_CLK_CONTROL_SOFT_RESET_MASK) << AUD_CLK_CONTROL_SOFT_RESET_POS);
    REG_WRITE(AUD_CLK_CONTROL_ADDR,reg_value);
}

/* REG_0x02:clk_control->clk_gate:0x2[1],0x0: 使能apb clock gate; 0x1: 不使能apb clock gate,0x0,R/W*/
static inline uint32_t aud_ll_get_clk_control_clk_gate(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_CLK_CONTROL_ADDR);
    reg_value = ((reg_value >> AUD_CLK_CONTROL_CLK_GATE_POS) & AUD_CLK_CONTROL_CLK_GATE_MASK);
    return reg_value;
}

static inline void aud_ll_set_clk_control_clk_gate(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_CLK_CONTROL_ADDR);
    reg_value &= ~(AUD_CLK_CONTROL_CLK_GATE_MASK << AUD_CLK_CONTROL_CLK_GATE_POS);
    reg_value |= ((value & AUD_CLK_CONTROL_CLK_GATE_MASK) << AUD_CLK_CONTROL_CLK_GATE_POS);
    REG_WRITE(AUD_CLK_CONTROL_ADDR,reg_value);
}

/* REG_0x03 */
#define AUD_GLOBAL_STATUS_ADDR  (AUD_LL_REG_BASE  + 0x3*4) //REG ADDR :0x4780000c
#define AUD_GLOBAL_STATUS_GLOBAL_STATUS_POS (0) 
#define AUD_GLOBAL_STATUS_GLOBAL_STATUS_MASK (0xFFFFFFFF) 

static inline uint32_t aud_ll_get_global_status_value(void)
{
    return REG_READ(AUD_GLOBAL_STATUS_ADDR);
}

/* REG_0x03:global_status->global_Status:0x3[31:0], ,0x0,R*/
static inline uint32_t aud_ll_get_global_status_global_status(void)
{
    return REG_READ(AUD_GLOBAL_STATUS_ADDR);
}

/* REG_0x04 */
#define AUD_ADC_CONFIG0_ADDR  (AUD_LL_REG_BASE  + 0x4*4) //REG ADDR :0x47800010
#define AUD_ADC_CONFIG0_ADC_HPF2_COEF_B2_POS (0) 
#define AUD_ADC_CONFIG0_ADC_HPF2_COEF_B2_MASK (0xFFFF) 

#define AUD_ADC_CONFIG0_ADC_HPF2_BYPASS_POS (16) 
#define AUD_ADC_CONFIG0_ADC_HPF2_BYPASS_MASK (0x1) 

#define AUD_ADC_CONFIG0_ADC_HPF1_BYPASS_POS (17) 
#define AUD_ADC_CONFIG0_ADC_HPF1_BYPASS_MASK (0x1) 

#define AUD_ADC_CONFIG0_ADC_SET_GAIN_POS (18) 
#define AUD_ADC_CONFIG0_ADC_SET_GAIN_MASK (0x3F) 

#define AUD_ADC_CONFIG0_ADC_SAMPE_EDGE_POS (24) 
#define AUD_ADC_CONFIG0_ADC_SAMPE_EDGE_MASK (0x1) 

#define AUD_ADC_CONFIG0_DIG_MIC_SEL_POS (25) 
#define AUD_ADC_CONFIG0_DIG_MIC_SEL_MASK (0x1) 

#define AUD_ADC_CONFIG0_RESERVED0_POS (26) 
#define AUD_ADC_CONFIG0_RESERVED0_MASK (0x3F) 

static inline uint32_t aud_ll_get_adc_config0_value(void)
{
    return REG_READ(AUD_ADC_CONFIG0_ADDR);
}

static inline void aud_ll_set_adc_config0_value(uint32_t value)
{
    REG_WRITE(AUD_ADC_CONFIG0_ADDR,value);
}

/* REG_0x04:adc_config0->adc_hpf2_coef_B2:0x4[15:0],ADC HPF2的系数 B2 ,0x0,R/W*/
static inline uint32_t aud_ll_get_adc_config0_adc_hpf2_coef_b2(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_CONFIG0_ADDR);
    reg_value = ((reg_value >> AUD_ADC_CONFIG0_ADC_HPF2_COEF_B2_POS) & AUD_ADC_CONFIG0_ADC_HPF2_COEF_B2_MASK);
    return reg_value;
}

static inline void aud_ll_set_adc_config0_adc_hpf2_coef_b2(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_CONFIG0_ADDR);
    reg_value &= ~(AUD_ADC_CONFIG0_ADC_HPF2_COEF_B2_MASK << AUD_ADC_CONFIG0_ADC_HPF2_COEF_B2_POS);
    reg_value |= ((value & AUD_ADC_CONFIG0_ADC_HPF2_COEF_B2_MASK) << AUD_ADC_CONFIG0_ADC_HPF2_COEF_B2_POS);
    REG_WRITE(AUD_ADC_CONFIG0_ADDR,reg_value);
}

/* REG_0x04:adc_config0->adc_hpf2_bypass:0x4[16],ADC HPF2禁能位，1：绕开HPF2,0x0,R/W*/
static inline uint32_t aud_ll_get_adc_config0_adc_hpf2_bypass(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_CONFIG0_ADDR);
    reg_value = ((reg_value >> AUD_ADC_CONFIG0_ADC_HPF2_BYPASS_POS) & AUD_ADC_CONFIG0_ADC_HPF2_BYPASS_MASK);
    return reg_value;
}

static inline void aud_ll_set_adc_config0_adc_hpf2_bypass(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_CONFIG0_ADDR);
    reg_value &= ~(AUD_ADC_CONFIG0_ADC_HPF2_BYPASS_MASK << AUD_ADC_CONFIG0_ADC_HPF2_BYPASS_POS);
    reg_value |= ((value & AUD_ADC_CONFIG0_ADC_HPF2_BYPASS_MASK) << AUD_ADC_CONFIG0_ADC_HPF2_BYPASS_POS);
    REG_WRITE(AUD_ADC_CONFIG0_ADDR,reg_value);
}

/* REG_0x04:adc_config0->adc_hpf1_bypass:0x4[17],ADC HPF1禁能位，1：绕开HPF1,0x0,R/W*/
static inline uint32_t aud_ll_get_adc_config0_adc_hpf1_bypass(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_CONFIG0_ADDR);
    reg_value = ((reg_value >> AUD_ADC_CONFIG0_ADC_HPF1_BYPASS_POS) & AUD_ADC_CONFIG0_ADC_HPF1_BYPASS_MASK);
    return reg_value;
}

static inline void aud_ll_set_adc_config0_adc_hpf1_bypass(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_CONFIG0_ADDR);
    reg_value &= ~(AUD_ADC_CONFIG0_ADC_HPF1_BYPASS_MASK << AUD_ADC_CONFIG0_ADC_HPF1_BYPASS_POS);
    reg_value |= ((value & AUD_ADC_CONFIG0_ADC_HPF1_BYPASS_MASK) << AUD_ADC_CONFIG0_ADC_HPF1_BYPASS_POS);
    REG_WRITE(AUD_ADC_CONFIG0_ADDR,reg_value);
}

/* REG_0x04:adc_config0->adc_set_gain:0x4[23:18],ADC 设置的增益 -45db ~18db  0x2d = 0db,0x0,R/W*/
static inline uint32_t aud_ll_get_adc_config0_adc_set_gain(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_CONFIG0_ADDR);
    reg_value = ((reg_value >> AUD_ADC_CONFIG0_ADC_SET_GAIN_POS) & AUD_ADC_CONFIG0_ADC_SET_GAIN_MASK);
    return reg_value;
}

static inline void aud_ll_set_adc_config0_adc_set_gain(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_CONFIG0_ADDR);
    reg_value &= ~(AUD_ADC_CONFIG0_ADC_SET_GAIN_MASK << AUD_ADC_CONFIG0_ADC_SET_GAIN_POS);
    reg_value |= ((value & AUD_ADC_CONFIG0_ADC_SET_GAIN_MASK) << AUD_ADC_CONFIG0_ADC_SET_GAIN_POS);
    REG_WRITE(AUD_ADC_CONFIG0_ADDR,reg_value);
}

/* REG_0x04:adc_config0->adc_sampe_edge:0x4[24],ADC数据采样的时钟边沿选择 1：下降沿，0：上升沿,0x0,R/W*/
static inline uint32_t aud_ll_get_adc_config0_adc_sampe_edge(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_CONFIG0_ADDR);
    reg_value = ((reg_value >> AUD_ADC_CONFIG0_ADC_SAMPE_EDGE_POS) & AUD_ADC_CONFIG0_ADC_SAMPE_EDGE_MASK);
    return reg_value;
}

static inline void aud_ll_set_adc_config0_adc_sampe_edge(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_CONFIG0_ADDR);
    reg_value &= ~(AUD_ADC_CONFIG0_ADC_SAMPE_EDGE_MASK << AUD_ADC_CONFIG0_ADC_SAMPE_EDGE_POS);
    reg_value |= ((value & AUD_ADC_CONFIG0_ADC_SAMPE_EDGE_MASK) << AUD_ADC_CONFIG0_ADC_SAMPE_EDGE_POS);
    REG_WRITE(AUD_ADC_CONFIG0_ADDR,reg_value);
}

/* REG_0x04:adc_config0->dig_mic_sel:0x4[25],用于调试，设为0,0x0,R/W*/
static inline uint32_t aud_ll_get_adc_config0_dig_mic_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_CONFIG0_ADDR);
    reg_value = ((reg_value >> AUD_ADC_CONFIG0_DIG_MIC_SEL_POS) & AUD_ADC_CONFIG0_DIG_MIC_SEL_MASK);
    return reg_value;
}

static inline void aud_ll_set_adc_config0_dig_mic_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_CONFIG0_ADDR);
    reg_value &= ~(AUD_ADC_CONFIG0_DIG_MIC_SEL_MASK << AUD_ADC_CONFIG0_DIG_MIC_SEL_POS);
    reg_value |= ((value & AUD_ADC_CONFIG0_DIG_MIC_SEL_MASK) << AUD_ADC_CONFIG0_DIG_MIC_SEL_POS);
    REG_WRITE(AUD_ADC_CONFIG0_ADDR,reg_value);
}

/* REG_0x05 */
#define AUD_ADC_CONFIG1_ADDR  (AUD_LL_REG_BASE  + 0x5*4) //REG ADDR :0x47800014
#define AUD_ADC_CONFIG1_ADC_HPF2_COEF_B0_POS (0) 
#define AUD_ADC_CONFIG1_ADC_HPF2_COEF_B0_MASK (0xFFFF) 

#define AUD_ADC_CONFIG1_ADC_HPF2_COEF_B1_POS (16) 
#define AUD_ADC_CONFIG1_ADC_HPF2_COEF_B1_MASK (0xFFFF) 

static inline uint32_t aud_ll_get_adc_config1_value(void)
{
    return REG_READ(AUD_ADC_CONFIG1_ADDR);
}

static inline void aud_ll_set_adc_config1_value(uint32_t value)
{
    REG_WRITE(AUD_ADC_CONFIG1_ADDR,value);
}

/* REG_0x05:adc_config1->adc_hpf2_coef_B0:0x5[15:0],ADC HPF2的系数 B2 ,0x0,R/W*/
static inline uint32_t aud_ll_get_adc_config1_adc_hpf2_coef_b0(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_CONFIG1_ADDR);
    reg_value = ((reg_value >> AUD_ADC_CONFIG1_ADC_HPF2_COEF_B0_POS) & AUD_ADC_CONFIG1_ADC_HPF2_COEF_B0_MASK);
    return reg_value;
}

static inline void aud_ll_set_adc_config1_adc_hpf2_coef_b0(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_CONFIG1_ADDR);
    reg_value &= ~(AUD_ADC_CONFIG1_ADC_HPF2_COEF_B0_MASK << AUD_ADC_CONFIG1_ADC_HPF2_COEF_B0_POS);
    reg_value |= ((value & AUD_ADC_CONFIG1_ADC_HPF2_COEF_B0_MASK) << AUD_ADC_CONFIG1_ADC_HPF2_COEF_B0_POS);
    REG_WRITE(AUD_ADC_CONFIG1_ADDR,reg_value);
}

/* REG_0x05:adc_config1->adc_hpf2_coef_B1:0x5[31:16],ADC HPF2的系数 B1 ,0x0,R/W*/
static inline uint32_t aud_ll_get_adc_config1_adc_hpf2_coef_b1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_CONFIG1_ADDR);
    reg_value = ((reg_value >> AUD_ADC_CONFIG1_ADC_HPF2_COEF_B1_POS) & AUD_ADC_CONFIG1_ADC_HPF2_COEF_B1_MASK);
    return reg_value;
}

static inline void aud_ll_set_adc_config1_adc_hpf2_coef_b1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_CONFIG1_ADDR);
    reg_value &= ~(AUD_ADC_CONFIG1_ADC_HPF2_COEF_B1_MASK << AUD_ADC_CONFIG1_ADC_HPF2_COEF_B1_POS);
    reg_value |= ((value & AUD_ADC_CONFIG1_ADC_HPF2_COEF_B1_MASK) << AUD_ADC_CONFIG1_ADC_HPF2_COEF_B1_POS);
    REG_WRITE(AUD_ADC_CONFIG1_ADDR,reg_value);
}

/* REG_0x06 */
#define AUD_ADC_CONFIG2_ADDR  (AUD_LL_REG_BASE  + 0x6*4) //REG ADDR :0x47800018
#define AUD_ADC_CONFIG2_ADC_HPF2_COEF_A0_POS (0) 
#define AUD_ADC_CONFIG2_ADC_HPF2_COEF_A0_MASK (0xFFFF) 

#define AUD_ADC_CONFIG2_ADC_HPF2_COEF_A1_POS (16) 
#define AUD_ADC_CONFIG2_ADC_HPF2_COEF_A1_MASK (0xFFFF) 

static inline uint32_t aud_ll_get_adc_config2_value(void)
{
    return REG_READ(AUD_ADC_CONFIG2_ADDR);
}

static inline void aud_ll_set_adc_config2_value(uint32_t value)
{
    REG_WRITE(AUD_ADC_CONFIG2_ADDR,value);
}

/* REG_0x06:adc_config2->adc_hpf2_coef_A0:0x6[15:0],ADC HPF2的系数 A0 ,0x0,R/W*/
static inline uint32_t aud_ll_get_adc_config2_adc_hpf2_coef_a0(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_CONFIG2_ADDR);
    reg_value = ((reg_value >> AUD_ADC_CONFIG2_ADC_HPF2_COEF_A0_POS) & AUD_ADC_CONFIG2_ADC_HPF2_COEF_A0_MASK);
    return reg_value;
}

static inline void aud_ll_set_adc_config2_adc_hpf2_coef_a0(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_CONFIG2_ADDR);
    reg_value &= ~(AUD_ADC_CONFIG2_ADC_HPF2_COEF_A0_MASK << AUD_ADC_CONFIG2_ADC_HPF2_COEF_A0_POS);
    reg_value |= ((value & AUD_ADC_CONFIG2_ADC_HPF2_COEF_A0_MASK) << AUD_ADC_CONFIG2_ADC_HPF2_COEF_A0_POS);
    REG_WRITE(AUD_ADC_CONFIG2_ADDR,reg_value);
}

/* REG_0x06:adc_config2->adc_hpf2_coef_A1:0x6[31:16],ADC HPF2的系数 A1 ,0x0,R/W*/
static inline uint32_t aud_ll_get_adc_config2_adc_hpf2_coef_a1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_CONFIG2_ADDR);
    reg_value = ((reg_value >> AUD_ADC_CONFIG2_ADC_HPF2_COEF_A1_POS) & AUD_ADC_CONFIG2_ADC_HPF2_COEF_A1_MASK);
    return reg_value;
}

static inline void aud_ll_set_adc_config2_adc_hpf2_coef_a1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_CONFIG2_ADDR);
    reg_value &= ~(AUD_ADC_CONFIG2_ADC_HPF2_COEF_A1_MASK << AUD_ADC_CONFIG2_ADC_HPF2_COEF_A1_POS);
    reg_value |= ((value & AUD_ADC_CONFIG2_ADC_HPF2_COEF_A1_MASK) << AUD_ADC_CONFIG2_ADC_HPF2_COEF_A1_POS);
    REG_WRITE(AUD_ADC_CONFIG2_ADDR,reg_value);
}

/* REG_0x07 */
#define AUD_DAC_CONFIG0_ADDR  (AUD_LL_REG_BASE  + 0x7*4) //REG ADDR :0x4780001c
#define AUD_DAC_CONFIG0_DAC_HPF2_COEF_B2_POS (0) 
#define AUD_DAC_CONFIG0_DAC_HPF2_COEF_B2_MASK (0xFFFF) 

#define AUD_DAC_CONFIG0_DAC_HPF2_BYPASS_POS (16) 
#define AUD_DAC_CONFIG0_DAC_HPF2_BYPASS_MASK (0x1) 

#define AUD_DAC_CONFIG0_DAC_HPF1_BYPASS_POS (17) 
#define AUD_DAC_CONFIG0_DAC_HPF1_BYPASS_MASK (0x1) 

#define AUD_DAC_CONFIG0_DAC_SET_GAIN_POS (18) 
#define AUD_DAC_CONFIG0_DAC_SET_GAIN_MASK (0x3F) 

#define AUD_DAC_CONFIG0_DAC_CLK_INVERT_POS (24) 
#define AUD_DAC_CONFIG0_DAC_CLK_INVERT_MASK (0x1) 

#define AUD_DAC_CONFIG0_DAC_PN_CONF_POS (25) 
#define AUD_DAC_CONFIG0_DAC_PN_CONF_MASK (0xF) 

#define AUD_DAC_CONFIG0_DAC_NOTCHEN_POS (29) 
#define AUD_DAC_CONFIG0_DAC_NOTCHEN_MASK (0x1) 

#define AUD_DAC_CONFIG0_RESERVED0_POS (30) 
#define AUD_DAC_CONFIG0_RESERVED0_MASK (0x3) 

static inline uint32_t aud_ll_get_dac_config0_value(void)
{
    return REG_READ(AUD_DAC_CONFIG0_ADDR);
}

static inline void aud_ll_set_dac_config0_value(uint32_t value)
{
    REG_WRITE(AUD_DAC_CONFIG0_ADDR,value);
}

/* REG_0x07:dac_config0->dac_hpf2_coef_B2:0x7[15:0],DAC HPF2的系数 B2 ,0x0,R/W*/
static inline uint32_t aud_ll_get_dac_config0_dac_hpf2_coef_b2(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG0_ADDR);
    reg_value = ((reg_value >> AUD_DAC_CONFIG0_DAC_HPF2_COEF_B2_POS) & AUD_DAC_CONFIG0_DAC_HPF2_COEF_B2_MASK);
    return reg_value;
}

static inline void aud_ll_set_dac_config0_dac_hpf2_coef_b2(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG0_ADDR);
    reg_value &= ~(AUD_DAC_CONFIG0_DAC_HPF2_COEF_B2_MASK << AUD_DAC_CONFIG0_DAC_HPF2_COEF_B2_POS);
    reg_value |= ((value & AUD_DAC_CONFIG0_DAC_HPF2_COEF_B2_MASK) << AUD_DAC_CONFIG0_DAC_HPF2_COEF_B2_POS);
    REG_WRITE(AUD_DAC_CONFIG0_ADDR,reg_value);
}

/* REG_0x07:dac_config0->dac_hpf2_bypass:0x7[16],DAC HPF2禁能位，1：绕开HPF2,0x0,R/W*/
static inline uint32_t aud_ll_get_dac_config0_dac_hpf2_bypass(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG0_ADDR);
    reg_value = ((reg_value >> AUD_DAC_CONFIG0_DAC_HPF2_BYPASS_POS) & AUD_DAC_CONFIG0_DAC_HPF2_BYPASS_MASK);
    return reg_value;
}

static inline void aud_ll_set_dac_config0_dac_hpf2_bypass(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG0_ADDR);
    reg_value &= ~(AUD_DAC_CONFIG0_DAC_HPF2_BYPASS_MASK << AUD_DAC_CONFIG0_DAC_HPF2_BYPASS_POS);
    reg_value |= ((value & AUD_DAC_CONFIG0_DAC_HPF2_BYPASS_MASK) << AUD_DAC_CONFIG0_DAC_HPF2_BYPASS_POS);
    REG_WRITE(AUD_DAC_CONFIG0_ADDR,reg_value);
}

/* REG_0x07:dac_config0->dac_hpf1_bypass:0x7[17],DAC HPF1禁能位，1：绕开HPF1 ,0x0,R/W*/
static inline uint32_t aud_ll_get_dac_config0_dac_hpf1_bypass(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG0_ADDR);
    reg_value = ((reg_value >> AUD_DAC_CONFIG0_DAC_HPF1_BYPASS_POS) & AUD_DAC_CONFIG0_DAC_HPF1_BYPASS_MASK);
    return reg_value;
}

static inline void aud_ll_set_dac_config0_dac_hpf1_bypass(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG0_ADDR);
    reg_value &= ~(AUD_DAC_CONFIG0_DAC_HPF1_BYPASS_MASK << AUD_DAC_CONFIG0_DAC_HPF1_BYPASS_POS);
    reg_value |= ((value & AUD_DAC_CONFIG0_DAC_HPF1_BYPASS_MASK) << AUD_DAC_CONFIG0_DAC_HPF1_BYPASS_POS);
    REG_WRITE(AUD_DAC_CONFIG0_ADDR,reg_value);
}

/* REG_0x07:dac_config0->dac_set_gain:0x7[23:18],DAC 设置的增益 -45db ~18db  0x2d = 0db,0x0,R/W*/
static inline uint32_t aud_ll_get_dac_config0_dac_set_gain(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG0_ADDR);
    reg_value = ((reg_value >> AUD_DAC_CONFIG0_DAC_SET_GAIN_POS) & AUD_DAC_CONFIG0_DAC_SET_GAIN_MASK);
    return reg_value;
}

static inline void aud_ll_set_dac_config0_dac_set_gain(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG0_ADDR);
    reg_value &= ~(AUD_DAC_CONFIG0_DAC_SET_GAIN_MASK << AUD_DAC_CONFIG0_DAC_SET_GAIN_POS);
    reg_value |= ((value & AUD_DAC_CONFIG0_DAC_SET_GAIN_MASK) << AUD_DAC_CONFIG0_DAC_SET_GAIN_POS);
    REG_WRITE(AUD_DAC_CONFIG0_ADDR,reg_value);
}

/* REG_0x07:dac_config0->dac_clk_invert:0x7[24],DAC 输出时钟边沿选择 0：下降沿，1：上升沿,0x0,R/W*/
static inline uint32_t aud_ll_get_dac_config0_dac_clk_invert(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG0_ADDR);
    reg_value = ((reg_value >> AUD_DAC_CONFIG0_DAC_CLK_INVERT_POS) & AUD_DAC_CONFIG0_DAC_CLK_INVERT_MASK);
    return reg_value;
}

static inline void aud_ll_set_dac_config0_dac_clk_invert(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG0_ADDR);
    reg_value &= ~(AUD_DAC_CONFIG0_DAC_CLK_INVERT_MASK << AUD_DAC_CONFIG0_DAC_CLK_INVERT_POS);
    reg_value |= ((value & AUD_DAC_CONFIG0_DAC_CLK_INVERT_MASK) << AUD_DAC_CONFIG0_DAC_CLK_INVERT_POS);
    REG_WRITE(AUD_DAC_CONFIG0_ADDR,reg_value);
}

/* REG_0x07:dac_config0->dac_pn_conf:0x7[28:25], ,0x0,R/W*/
static inline uint32_t aud_ll_get_dac_config0_dac_pn_conf(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG0_ADDR);
    reg_value = ((reg_value >> AUD_DAC_CONFIG0_DAC_PN_CONF_POS) & AUD_DAC_CONFIG0_DAC_PN_CONF_MASK);
    return reg_value;
}

static inline void aud_ll_set_dac_config0_dac_pn_conf(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG0_ADDR);
    reg_value &= ~(AUD_DAC_CONFIG0_DAC_PN_CONF_MASK << AUD_DAC_CONFIG0_DAC_PN_CONF_POS);
    reg_value |= ((value & AUD_DAC_CONFIG0_DAC_PN_CONF_MASK) << AUD_DAC_CONFIG0_DAC_PN_CONF_POS);
    REG_WRITE(AUD_DAC_CONFIG0_ADDR,reg_value);
}

/* REG_0x07:dac_config0->dac_notchen:0x7[29], ,0x0,R/W*/
static inline uint32_t aud_ll_get_dac_config0_dac_notchen(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG0_ADDR);
    reg_value = ((reg_value >> AUD_DAC_CONFIG0_DAC_NOTCHEN_POS) & AUD_DAC_CONFIG0_DAC_NOTCHEN_MASK);
    return reg_value;
}

static inline void aud_ll_set_dac_config0_dac_notchen(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG0_ADDR);
    reg_value &= ~(AUD_DAC_CONFIG0_DAC_NOTCHEN_MASK << AUD_DAC_CONFIG0_DAC_NOTCHEN_POS);
    reg_value |= ((value & AUD_DAC_CONFIG0_DAC_NOTCHEN_MASK) << AUD_DAC_CONFIG0_DAC_NOTCHEN_POS);
    REG_WRITE(AUD_DAC_CONFIG0_ADDR,reg_value);
}

/* REG_0x08 */
#define AUD_DAC_CONFIG1_ADDR  (AUD_LL_REG_BASE  + 0x8*4) //REG ADDR :0x47800020
#define AUD_DAC_CONFIG1_DAC_HPF2_COEF_B0_POS (0) 
#define AUD_DAC_CONFIG1_DAC_HPF2_COEF_B0_MASK (0xFFFF) 

#define AUD_DAC_CONFIG1_DAC_HPF2_COEF_B1_POS (16) 
#define AUD_DAC_CONFIG1_DAC_HPF2_COEF_B1_MASK (0xFFFF) 

static inline uint32_t aud_ll_get_dac_config1_value(void)
{
    return REG_READ(AUD_DAC_CONFIG1_ADDR);
}

static inline void aud_ll_set_dac_config1_value(uint32_t value)
{
    REG_WRITE(AUD_DAC_CONFIG1_ADDR,value);
}

/* REG_0x08:dac_config1->dac_hpf2_coef_B0:0x8[15:0],DAC HPF2的系数 B0 ,0x0,R/W*/
static inline uint32_t aud_ll_get_dac_config1_dac_hpf2_coef_b0(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG1_ADDR);
    reg_value = ((reg_value >> AUD_DAC_CONFIG1_DAC_HPF2_COEF_B0_POS) & AUD_DAC_CONFIG1_DAC_HPF2_COEF_B0_MASK);
    return reg_value;
}

static inline void aud_ll_set_dac_config1_dac_hpf2_coef_b0(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG1_ADDR);
    reg_value &= ~(AUD_DAC_CONFIG1_DAC_HPF2_COEF_B0_MASK << AUD_DAC_CONFIG1_DAC_HPF2_COEF_B0_POS);
    reg_value |= ((value & AUD_DAC_CONFIG1_DAC_HPF2_COEF_B0_MASK) << AUD_DAC_CONFIG1_DAC_HPF2_COEF_B0_POS);
    REG_WRITE(AUD_DAC_CONFIG1_ADDR,reg_value);
}

/* REG_0x08:dac_config1->dac_hpf2_coef_B1:0x8[31:16],DAC HPF2的系数 B1 ,0x0,R/W*/
static inline uint32_t aud_ll_get_dac_config1_dac_hpf2_coef_b1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG1_ADDR);
    reg_value = ((reg_value >> AUD_DAC_CONFIG1_DAC_HPF2_COEF_B1_POS) & AUD_DAC_CONFIG1_DAC_HPF2_COEF_B1_MASK);
    return reg_value;
}

static inline void aud_ll_set_dac_config1_dac_hpf2_coef_b1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG1_ADDR);
    reg_value &= ~(AUD_DAC_CONFIG1_DAC_HPF2_COEF_B1_MASK << AUD_DAC_CONFIG1_DAC_HPF2_COEF_B1_POS);
    reg_value |= ((value & AUD_DAC_CONFIG1_DAC_HPF2_COEF_B1_MASK) << AUD_DAC_CONFIG1_DAC_HPF2_COEF_B1_POS);
    REG_WRITE(AUD_DAC_CONFIG1_ADDR,reg_value);
}

/* REG_0x09 */
#define AUD_DAC_CONFIG2_ADDR  (AUD_LL_REG_BASE  + 0x9*4) //REG ADDR :0x47800024
#define AUD_DAC_CONFIG2_DAC_HPF2_COEF_A1_POS (0) 
#define AUD_DAC_CONFIG2_DAC_HPF2_COEF_A1_MASK (0xFFFF) 

#define AUD_DAC_CONFIG2_DAC_HPF2_COEF_A2_POS (16) 
#define AUD_DAC_CONFIG2_DAC_HPF2_COEF_A2_MASK (0xFFFF) 

static inline uint32_t aud_ll_get_dac_config2_value(void)
{
    return REG_READ(AUD_DAC_CONFIG2_ADDR);
}

static inline void aud_ll_set_dac_config2_value(uint32_t value)
{
    REG_WRITE(AUD_DAC_CONFIG2_ADDR,value);
}

/* REG_0x09:dac_config2->dac_hpf2_coef_A1:0x9[15:0],DAC HPF2的系数 A1 ,0x0,R/W*/
static inline uint32_t aud_ll_get_dac_config2_dac_hpf2_coef_a1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG2_ADDR);
    reg_value = ((reg_value >> AUD_DAC_CONFIG2_DAC_HPF2_COEF_A1_POS) & AUD_DAC_CONFIG2_DAC_HPF2_COEF_A1_MASK);
    return reg_value;
}

static inline void aud_ll_set_dac_config2_dac_hpf2_coef_a1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG2_ADDR);
    reg_value &= ~(AUD_DAC_CONFIG2_DAC_HPF2_COEF_A1_MASK << AUD_DAC_CONFIG2_DAC_HPF2_COEF_A1_POS);
    reg_value |= ((value & AUD_DAC_CONFIG2_DAC_HPF2_COEF_A1_MASK) << AUD_DAC_CONFIG2_DAC_HPF2_COEF_A1_POS);
    REG_WRITE(AUD_DAC_CONFIG2_ADDR,reg_value);
}

/* REG_0x09:dac_config2->dac_hpf2_coef_A2:0x9[31:16],DAC HPF2的系数 A2 ,0x0,R/W*/
static inline uint32_t aud_ll_get_dac_config2_dac_hpf2_coef_a2(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG2_ADDR);
    reg_value = ((reg_value >> AUD_DAC_CONFIG2_DAC_HPF2_COEF_A2_POS) & AUD_DAC_CONFIG2_DAC_HPF2_COEF_A2_MASK);
    return reg_value;
}

static inline void aud_ll_set_dac_config2_dac_hpf2_coef_a2(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DAC_CONFIG2_ADDR);
    reg_value &= ~(AUD_DAC_CONFIG2_DAC_HPF2_COEF_A2_MASK << AUD_DAC_CONFIG2_DAC_HPF2_COEF_A2_POS);
    reg_value |= ((value & AUD_DAC_CONFIG2_DAC_HPF2_COEF_A2_MASK) << AUD_DAC_CONFIG2_DAC_HPF2_COEF_A2_POS);
    REG_WRITE(AUD_DAC_CONFIG2_ADDR,reg_value);
}

/* REG_0x0A */
#define AUD_FIFO_CONFIG_ADDR  (AUD_LL_REG_BASE  + 0xA*4) //REG ADDR :0x47800028
#define AUD_FIFO_CONFIG_DACR_RD_THRESHOLD_POS (0) 
#define AUD_FIFO_CONFIG_DACR_RD_THRESHOLD_MASK (0x1F) 

#define AUD_FIFO_CONFIG_DACL_RD_THRESHOLD_POS (5) 
#define AUD_FIFO_CONFIG_DACL_RD_THRESHOLD_MASK (0x1F) 

#define AUD_FIFO_CONFIG_DTMF_WR_THRESHOLD_POS (10) 
#define AUD_FIFO_CONFIG_DTMF_WR_THRESHOLD_MASK (0x1F) 

#define AUD_FIFO_CONFIG_ADCL_WR_THRESHOLD_POS (15) 
#define AUD_FIFO_CONFIG_ADCL_WR_THRESHOLD_MASK (0x1F) 

#define AUD_FIFO_CONFIG_DACR_INT_EN_POS (20) 
#define AUD_FIFO_CONFIG_DACR_INT_EN_MASK (0x1) 

#define AUD_FIFO_CONFIG_DACL_INT_EN_POS (21) 
#define AUD_FIFO_CONFIG_DACL_INT_EN_MASK (0x1) 

#define AUD_FIFO_CONFIG_DTMF_INT_EN_POS (22) 
#define AUD_FIFO_CONFIG_DTMF_INT_EN_MASK (0x1) 

#define AUD_FIFO_CONFIG_ADCL_INT_EN_POS (23) 
#define AUD_FIFO_CONFIG_ADCL_INT_EN_MASK (0x1) 

#define AUD_FIFO_CONFIG_LOOP_TON2DAC_POS (24) 
#define AUD_FIFO_CONFIG_LOOP_TON2DAC_MASK (0x1) 

#define AUD_FIFO_CONFIG_LOOP_ADC2DAC_POS (25) 
#define AUD_FIFO_CONFIG_LOOP_ADC2DAC_MASK (0x1) 

#define AUD_FIFO_CONFIG_LOOP_DMIC2DAC_POS (26) 
#define AUD_FIFO_CONFIG_LOOP_DMIC2DAC_MASK (0x1) 

#define AUD_FIFO_CONFIG_DMIC_WR_THRESHOLD_POS (27) 
#define AUD_FIFO_CONFIG_DMIC_WR_THRESHOLD_MASK (0xF) 

#define AUD_FIFO_CONFIG_DMIC_INT_EN_POS (31) 
#define AUD_FIFO_CONFIG_DMIC_INT_EN_MASK (0x1) 

static inline uint32_t aud_ll_get_fifo_config_value(void)
{
    return REG_READ(AUD_FIFO_CONFIG_ADDR);
}

static inline void aud_ll_set_fifo_config_value(uint32_t value)
{
    REG_WRITE(AUD_FIFO_CONFIG_ADDR,value);
}

/* REG_0x0a:fifo_config->dacr_rd_threshold:0xa[4:0],DAC右声道读FIFO时，数目低于该门限，发出中断请求,0x0,R/W*/
static inline uint32_t aud_ll_get_fifo_config_dacr_rd_threshold(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_CONFIG_DACR_RD_THRESHOLD_POS) & AUD_FIFO_CONFIG_DACR_RD_THRESHOLD_MASK);
    return reg_value;
}

static inline void aud_ll_set_fifo_config_dacr_rd_threshold(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value &= ~(AUD_FIFO_CONFIG_DACR_RD_THRESHOLD_MASK << AUD_FIFO_CONFIG_DACR_RD_THRESHOLD_POS);
    reg_value |= ((value & AUD_FIFO_CONFIG_DACR_RD_THRESHOLD_MASK) << AUD_FIFO_CONFIG_DACR_RD_THRESHOLD_POS);
    REG_WRITE(AUD_FIFO_CONFIG_ADDR,reg_value);
}

/* REG_0x0a:fifo_config->dacl_rd_threshold:0xa[9:5],DAC左声道读FIFO时，数目低于该门限，发出中断请求,0x0,R/W*/
static inline uint32_t aud_ll_get_fifo_config_dacl_rd_threshold(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_CONFIG_DACL_RD_THRESHOLD_POS) & AUD_FIFO_CONFIG_DACL_RD_THRESHOLD_MASK);
    return reg_value;
}

static inline void aud_ll_set_fifo_config_dacl_rd_threshold(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value &= ~(AUD_FIFO_CONFIG_DACL_RD_THRESHOLD_MASK << AUD_FIFO_CONFIG_DACL_RD_THRESHOLD_POS);
    reg_value |= ((value & AUD_FIFO_CONFIG_DACL_RD_THRESHOLD_MASK) << AUD_FIFO_CONFIG_DACL_RD_THRESHOLD_POS);
    REG_WRITE(AUD_FIFO_CONFIG_ADDR,reg_value);
}

/* REG_0x0a:fifo_config->dtmf_wr_threshold:0xa[14:10],DTMF自动写FIFO时，数目高于该门限，发出中断请求,0x0,R/W*/
static inline uint32_t aud_ll_get_fifo_config_dtmf_wr_threshold(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_CONFIG_DTMF_WR_THRESHOLD_POS) & AUD_FIFO_CONFIG_DTMF_WR_THRESHOLD_MASK);
    return reg_value;
}

static inline void aud_ll_set_fifo_config_dtmf_wr_threshold(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value &= ~(AUD_FIFO_CONFIG_DTMF_WR_THRESHOLD_MASK << AUD_FIFO_CONFIG_DTMF_WR_THRESHOLD_POS);
    reg_value |= ((value & AUD_FIFO_CONFIG_DTMF_WR_THRESHOLD_MASK) << AUD_FIFO_CONFIG_DTMF_WR_THRESHOLD_POS);
    REG_WRITE(AUD_FIFO_CONFIG_ADDR,reg_value);
}

/* REG_0x0a:fifo_config->adcl_wr_threshold:0xa[19:15],ADC自动写FIFO时，数目高于该门限，发出中断请求,0x0,R/W*/
static inline uint32_t aud_ll_get_fifo_config_adcl_wr_threshold(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_CONFIG_ADCL_WR_THRESHOLD_POS) & AUD_FIFO_CONFIG_ADCL_WR_THRESHOLD_MASK);
    return reg_value;
}

static inline void aud_ll_set_fifo_config_adcl_wr_threshold(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value &= ~(AUD_FIFO_CONFIG_ADCL_WR_THRESHOLD_MASK << AUD_FIFO_CONFIG_ADCL_WR_THRESHOLD_POS);
    reg_value |= ((value & AUD_FIFO_CONFIG_ADCL_WR_THRESHOLD_MASK) << AUD_FIFO_CONFIG_ADCL_WR_THRESHOLD_POS);
    REG_WRITE(AUD_FIFO_CONFIG_ADDR,reg_value);
}

/* REG_0x0a:fifo_config->dacr_int_en:0xa[20],DAC右声道中断请求使能位,0x0,R/W*/
static inline uint32_t aud_ll_get_fifo_config_dacr_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_CONFIG_DACR_INT_EN_POS) & AUD_FIFO_CONFIG_DACR_INT_EN_MASK);
    return reg_value;
}

static inline void aud_ll_set_fifo_config_dacr_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value &= ~(AUD_FIFO_CONFIG_DACR_INT_EN_MASK << AUD_FIFO_CONFIG_DACR_INT_EN_POS);
    reg_value |= ((value & AUD_FIFO_CONFIG_DACR_INT_EN_MASK) << AUD_FIFO_CONFIG_DACR_INT_EN_POS);
    REG_WRITE(AUD_FIFO_CONFIG_ADDR,reg_value);
}

/* REG_0x0a:fifo_config->dacl_int_en:0xa[21],DAC左声道中断请求使能位,0x0,R/W*/
static inline uint32_t aud_ll_get_fifo_config_dacl_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_CONFIG_DACL_INT_EN_POS) & AUD_FIFO_CONFIG_DACL_INT_EN_MASK);
    return reg_value;
}

static inline void aud_ll_set_fifo_config_dacl_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value &= ~(AUD_FIFO_CONFIG_DACL_INT_EN_MASK << AUD_FIFO_CONFIG_DACL_INT_EN_POS);
    reg_value |= ((value & AUD_FIFO_CONFIG_DACL_INT_EN_MASK) << AUD_FIFO_CONFIG_DACL_INT_EN_POS);
    REG_WRITE(AUD_FIFO_CONFIG_ADDR,reg_value);
}

/* REG_0x0a:fifo_config->dtmf_int_en:0xa[22],DTMF中断请求使能位,0x0,R/W*/
static inline uint32_t aud_ll_get_fifo_config_dtmf_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_CONFIG_DTMF_INT_EN_POS) & AUD_FIFO_CONFIG_DTMF_INT_EN_MASK);
    return reg_value;
}

static inline void aud_ll_set_fifo_config_dtmf_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value &= ~(AUD_FIFO_CONFIG_DTMF_INT_EN_MASK << AUD_FIFO_CONFIG_DTMF_INT_EN_POS);
    reg_value |= ((value & AUD_FIFO_CONFIG_DTMF_INT_EN_MASK) << AUD_FIFO_CONFIG_DTMF_INT_EN_POS);
    REG_WRITE(AUD_FIFO_CONFIG_ADDR,reg_value);
}

/* REG_0x0a:fifo_config->adcl_int_en:0xa[23],ADC中断请求使能位,0x0,R/W*/
static inline uint32_t aud_ll_get_fifo_config_adcl_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_CONFIG_ADCL_INT_EN_POS) & AUD_FIFO_CONFIG_ADCL_INT_EN_MASK);
    return reg_value;
}

static inline void aud_ll_set_fifo_config_adcl_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value &= ~(AUD_FIFO_CONFIG_ADCL_INT_EN_MASK << AUD_FIFO_CONFIG_ADCL_INT_EN_POS);
    reg_value |= ((value & AUD_FIFO_CONFIG_ADCL_INT_EN_MASK) << AUD_FIFO_CONFIG_ADCL_INT_EN_POS);
    REG_WRITE(AUD_FIFO_CONFIG_ADDR,reg_value);
}

/* REG_0x0a:fifo_config->loop_ton2dac:0xa[24],DTMF到DAC环回测试使能位,0x0,R/W*/
static inline uint32_t aud_ll_get_fifo_config_loop_ton2dac(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_CONFIG_LOOP_TON2DAC_POS) & AUD_FIFO_CONFIG_LOOP_TON2DAC_MASK);
    return reg_value;
}

static inline void aud_ll_set_fifo_config_loop_ton2dac(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value &= ~(AUD_FIFO_CONFIG_LOOP_TON2DAC_MASK << AUD_FIFO_CONFIG_LOOP_TON2DAC_POS);
    reg_value |= ((value & AUD_FIFO_CONFIG_LOOP_TON2DAC_MASK) << AUD_FIFO_CONFIG_LOOP_TON2DAC_POS);
    REG_WRITE(AUD_FIFO_CONFIG_ADDR,reg_value);
}

/* REG_0x0a:fifo_config->loop_adc2dac:0xa[25],ADC到DAC环回测试使能位,0x0,R/W*/
static inline uint32_t aud_ll_get_fifo_config_loop_adc2dac(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_CONFIG_LOOP_ADC2DAC_POS) & AUD_FIFO_CONFIG_LOOP_ADC2DAC_MASK);
    return reg_value;
}

static inline void aud_ll_set_fifo_config_loop_adc2dac(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value &= ~(AUD_FIFO_CONFIG_LOOP_ADC2DAC_MASK << AUD_FIFO_CONFIG_LOOP_ADC2DAC_POS);
    reg_value |= ((value & AUD_FIFO_CONFIG_LOOP_ADC2DAC_MASK) << AUD_FIFO_CONFIG_LOOP_ADC2DAC_POS);
    REG_WRITE(AUD_FIFO_CONFIG_ADDR,reg_value);
}

/* REG_0x0a:fifo_config->loop_dmic2dac:0xa[26],DMIC到DAC环回测试使能位,0x0,R/W*/
static inline uint32_t aud_ll_get_fifo_config_loop_dmic2dac(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_CONFIG_LOOP_DMIC2DAC_POS) & AUD_FIFO_CONFIG_LOOP_DMIC2DAC_MASK);
    return reg_value;
}

static inline void aud_ll_set_fifo_config_loop_dmic2dac(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value &= ~(AUD_FIFO_CONFIG_LOOP_DMIC2DAC_MASK << AUD_FIFO_CONFIG_LOOP_DMIC2DAC_POS);
    reg_value |= ((value & AUD_FIFO_CONFIG_LOOP_DMIC2DAC_MASK) << AUD_FIFO_CONFIG_LOOP_DMIC2DAC_POS);
    REG_WRITE(AUD_FIFO_CONFIG_ADDR,reg_value);
}

/* REG_0x0a:fifo_config->dmic_wr_threshold:0xa[30:27],DMIC自动写FIFO时，数目高于该门限，发出中断请求,0x0,R/W*/
static inline uint32_t aud_ll_get_fifo_config_dmic_wr_threshold(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_CONFIG_DMIC_WR_THRESHOLD_POS) & AUD_FIFO_CONFIG_DMIC_WR_THRESHOLD_MASK);
    return reg_value;
}

static inline void aud_ll_set_fifo_config_dmic_wr_threshold(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value &= ~(AUD_FIFO_CONFIG_DMIC_WR_THRESHOLD_MASK << AUD_FIFO_CONFIG_DMIC_WR_THRESHOLD_POS);
    reg_value |= ((value & AUD_FIFO_CONFIG_DMIC_WR_THRESHOLD_MASK) << AUD_FIFO_CONFIG_DMIC_WR_THRESHOLD_POS);
    REG_WRITE(AUD_FIFO_CONFIG_ADDR,reg_value);
}

/* REG_0x0a:fifo_config->dmic_int_en:0xa[31],DMIC中断请求使能位,0x0,R/W*/
static inline uint32_t aud_ll_get_fifo_config_dmic_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_CONFIG_DMIC_INT_EN_POS) & AUD_FIFO_CONFIG_DMIC_INT_EN_MASK);
    return reg_value;
}

static inline void aud_ll_set_fifo_config_dmic_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_CONFIG_ADDR);
    reg_value &= ~(AUD_FIFO_CONFIG_DMIC_INT_EN_MASK << AUD_FIFO_CONFIG_DMIC_INT_EN_POS);
    reg_value |= ((value & AUD_FIFO_CONFIG_DMIC_INT_EN_MASK) << AUD_FIFO_CONFIG_DMIC_INT_EN_POS);
    REG_WRITE(AUD_FIFO_CONFIG_ADDR,reg_value);
}

/* REG_0x0B */
#define AUD_AGC_CONFIG0_ADDR  (AUD_LL_REG_BASE  + 0xB*4) //REG ADDR :0x4780002c
#define AUD_AGC_CONFIG0_AGC_NOISE_THRD_POS (0) 
#define AUD_AGC_CONFIG0_AGC_NOISE_THRD_MASK (0x3FF) 

#define AUD_AGC_CONFIG0_AGC_NOISE_HIGH_POS (10) 
#define AUD_AGC_CONFIG0_AGC_NOISE_HIGH_MASK (0x3FF) 

#define AUD_AGC_CONFIG0_AGC_NOISE_LOW_POS (20) 
#define AUD_AGC_CONFIG0_AGC_NOISE_LOW_MASK (0x3FF) 

#define AUD_AGC_CONFIG0_RESERVED0_POS (30) 
#define AUD_AGC_CONFIG0_RESERVED0_MASK (0x3) 

static inline uint32_t aud_ll_get_agc_config0_value(void)
{
    return REG_READ(AUD_AGC_CONFIG0_ADDR);
}

static inline void aud_ll_set_agc_config0_value(uint32_t value)
{
    REG_WRITE(AUD_AGC_CONFIG0_ADDR,value);
}

/* REG_0x0b:agc_config0->agc_noise_thrd:0xb[9:0],noise gating 执行时刻的门限，noise gating要在audio值比较小的时候执行,0x0,R/W*/
static inline uint32_t aud_ll_get_agc_config0_agc_noise_thrd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG0_ADDR);
    reg_value = ((reg_value >> AUD_AGC_CONFIG0_AGC_NOISE_THRD_POS) & AUD_AGC_CONFIG0_AGC_NOISE_THRD_MASK);
    return reg_value;
}

static inline void aud_ll_set_agc_config0_agc_noise_thrd(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG0_ADDR);
    reg_value &= ~(AUD_AGC_CONFIG0_AGC_NOISE_THRD_MASK << AUD_AGC_CONFIG0_AGC_NOISE_THRD_POS);
    reg_value |= ((value & AUD_AGC_CONFIG0_AGC_NOISE_THRD_MASK) << AUD_AGC_CONFIG0_AGC_NOISE_THRD_POS);
    REG_WRITE(AUD_AGC_CONFIG0_ADDR,reg_value);
}

/* REG_0x0b:agc_config0->agc_noise_high:0xb[19:10],对应MIC_RSSI低10 bit,0x0,R/W*/
static inline uint32_t aud_ll_get_agc_config0_agc_noise_high(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG0_ADDR);
    reg_value = ((reg_value >> AUD_AGC_CONFIG0_AGC_NOISE_HIGH_POS) & AUD_AGC_CONFIG0_AGC_NOISE_HIGH_MASK);
    return reg_value;
}

static inline void aud_ll_set_agc_config0_agc_noise_high(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG0_ADDR);
    reg_value &= ~(AUD_AGC_CONFIG0_AGC_NOISE_HIGH_MASK << AUD_AGC_CONFIG0_AGC_NOISE_HIGH_POS);
    reg_value |= ((value & AUD_AGC_CONFIG0_AGC_NOISE_HIGH_MASK) << AUD_AGC_CONFIG0_AGC_NOISE_HIGH_POS);
    REG_WRITE(AUD_AGC_CONFIG0_ADDR,reg_value);
}

/* REG_0x0b:agc_config0->agc_noise_low:0xb[29:20],对应MIC_RSSI低10 bit，比如MIC_RSSI[15:10]任何一位为1，都说明信号电平超过NOISE_LOW,0x0,R/W*/
static inline uint32_t aud_ll_get_agc_config0_agc_noise_low(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG0_ADDR);
    reg_value = ((reg_value >> AUD_AGC_CONFIG0_AGC_NOISE_LOW_POS) & AUD_AGC_CONFIG0_AGC_NOISE_LOW_MASK);
    return reg_value;
}

static inline void aud_ll_set_agc_config0_agc_noise_low(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG0_ADDR);
    reg_value &= ~(AUD_AGC_CONFIG0_AGC_NOISE_LOW_MASK << AUD_AGC_CONFIG0_AGC_NOISE_LOW_POS);
    reg_value |= ((value & AUD_AGC_CONFIG0_AGC_NOISE_LOW_MASK) << AUD_AGC_CONFIG0_AGC_NOISE_LOW_POS);
    REG_WRITE(AUD_AGC_CONFIG0_ADDR,reg_value);
}

/* REG_0x0C */
#define AUD_AGC_CONFIG1_ADDR  (AUD_LL_REG_BASE  + 0xC*4) //REG ADDR :0x47800030
#define AUD_AGC_CONFIG1_AGC_NOISE_MIN_POS (0) 
#define AUD_AGC_CONFIG1_AGC_NOISE_MIN_MASK (0x7F) 

#define AUD_AGC_CONFIG1_AGC_NOISE_TOUT_POS (7) 
#define AUD_AGC_CONFIG1_AGC_NOISE_TOUT_MASK (0x7) 

#define AUD_AGC_CONFIG1_AGC_HIGH_DUR_POS (10) 
#define AUD_AGC_CONFIG1_AGC_HIGH_DUR_MASK (0x7) 

#define AUD_AGC_CONFIG1_AGC_LOW_DUR_POS (13) 
#define AUD_AGC_CONFIG1_AGC_LOW_DUR_MASK (0x7) 

#define AUD_AGC_CONFIG1_AGC_MIN_POS (16) 
#define AUD_AGC_CONFIG1_AGC_MIN_MASK (0x7F) 

#define AUD_AGC_CONFIG1_AGC_MAX_POS (23) 
#define AUD_AGC_CONFIG1_AGC_MAX_MASK (0x7F) 

#define AUD_AGC_CONFIG1_AGC_NG_METHOD_POS (30) 
#define AUD_AGC_CONFIG1_AGC_NG_METHOD_MASK (0x1) 

#define AUD_AGC_CONFIG1_AGC_NG_ENABLE_POS (31) 
#define AUD_AGC_CONFIG1_AGC_NG_ENABLE_MASK (0x1) 

static inline uint32_t aud_ll_get_agc_config1_value(void)
{
    return REG_READ(AUD_AGC_CONFIG1_ADDR);
}

static inline void aud_ll_set_agc_config1_value(uint32_t value)
{
    REG_WRITE(AUD_AGC_CONFIG1_ADDR,value);
}

/* REG_0x0c:agc_config1->agc_noise_min:0xc[6:0],{GAIN2[2:0] GAIN1[3:0]} when signal level below NOISE_LOW, when noise gating is enabled,0x0,R/W*/
static inline uint32_t aud_ll_get_agc_config1_agc_noise_min(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG1_ADDR);
    reg_value = ((reg_value >> AUD_AGC_CONFIG1_AGC_NOISE_MIN_POS) & AUD_AGC_CONFIG1_AGC_NOISE_MIN_MASK);
    return reg_value;
}

static inline void aud_ll_set_agc_config1_agc_noise_min(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG1_ADDR);
    reg_value &= ~(AUD_AGC_CONFIG1_AGC_NOISE_MIN_MASK << AUD_AGC_CONFIG1_AGC_NOISE_MIN_POS);
    reg_value |= ((value & AUD_AGC_CONFIG1_AGC_NOISE_MIN_MASK) << AUD_AGC_CONFIG1_AGC_NOISE_MIN_POS);
    REG_WRITE(AUD_AGC_CONFIG1_ADDR,reg_value);
}

/* REG_0x0c:agc_config1->agc_noise_tout:0xc[9:7],1: 4 (~0.5 ms), 2: 8, 3: 16, 4: 32, 5: 64, 6: 128, 7: 256 (~32 ms),0x0,R/W*/
static inline uint32_t aud_ll_get_agc_config1_agc_noise_tout(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG1_ADDR);
    reg_value = ((reg_value >> AUD_AGC_CONFIG1_AGC_NOISE_TOUT_POS) & AUD_AGC_CONFIG1_AGC_NOISE_TOUT_MASK);
    return reg_value;
}

static inline void aud_ll_set_agc_config1_agc_noise_tout(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG1_ADDR);
    reg_value &= ~(AUD_AGC_CONFIG1_AGC_NOISE_TOUT_MASK << AUD_AGC_CONFIG1_AGC_NOISE_TOUT_POS);
    reg_value |= ((value & AUD_AGC_CONFIG1_AGC_NOISE_TOUT_MASK) << AUD_AGC_CONFIG1_AGC_NOISE_TOUT_POS);
    REG_WRITE(AUD_AGC_CONFIG1_ADDR,reg_value);
}

/* REG_0x0c:agc_config1->agc_high_dur:0xc[12:10],0: 0 (0 ms，一旦发生MIC_RSSI超过NOISE_HIGH，立刻触发Noise gating，离开noise状态); 1: 8 (~1 ms), 2: 16, 3: 32, 4: 64, 5: 128, 6: 256, 7: 256 (~32 ms),0x0,R/W*/
static inline uint32_t aud_ll_get_agc_config1_agc_high_dur(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG1_ADDR);
    reg_value = ((reg_value >> AUD_AGC_CONFIG1_AGC_HIGH_DUR_POS) & AUD_AGC_CONFIG1_AGC_HIGH_DUR_MASK);
    return reg_value;
}

static inline void aud_ll_set_agc_config1_agc_high_dur(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG1_ADDR);
    reg_value &= ~(AUD_AGC_CONFIG1_AGC_HIGH_DUR_MASK << AUD_AGC_CONFIG1_AGC_HIGH_DUR_POS);
    reg_value |= ((value & AUD_AGC_CONFIG1_AGC_HIGH_DUR_MASK) << AUD_AGC_CONFIG1_AGC_HIGH_DUR_POS);
    REG_WRITE(AUD_AGC_CONFIG1_ADDR,reg_value);
}

/* REG_0x0c:agc_config1->agc_low_dur:0xc[15:13],0: 0 (0 ms，一旦发生MIC_RSSI小于NOISE_LOW，立刻触发noise gating); 1: 8 (~1 ms), 2: 16, 3: 32, 4: 64, 5: 128, 6: 256, 7: 256 (~32 ms),0x0,R/W*/
static inline uint32_t aud_ll_get_agc_config1_agc_low_dur(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG1_ADDR);
    reg_value = ((reg_value >> AUD_AGC_CONFIG1_AGC_LOW_DUR_POS) & AUD_AGC_CONFIG1_AGC_LOW_DUR_MASK);
    return reg_value;
}

static inline void aud_ll_set_agc_config1_agc_low_dur(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG1_ADDR);
    reg_value &= ~(AUD_AGC_CONFIG1_AGC_LOW_DUR_MASK << AUD_AGC_CONFIG1_AGC_LOW_DUR_POS);
    reg_value |= ((value & AUD_AGC_CONFIG1_AGC_LOW_DUR_MASK) << AUD_AGC_CONFIG1_AGC_LOW_DUR_POS);
    REG_WRITE(AUD_AGC_CONFIG1_ADDR,reg_value);
}

/* REG_0x0c:agc_config1->agc_min:0xc[22:16],Minimum value of {GAIN2[2:0] GAIN1[3:0]},0x0,R/W*/
static inline uint32_t aud_ll_get_agc_config1_agc_min(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG1_ADDR);
    reg_value = ((reg_value >> AUD_AGC_CONFIG1_AGC_MIN_POS) & AUD_AGC_CONFIG1_AGC_MIN_MASK);
    return reg_value;
}

static inline void aud_ll_set_agc_config1_agc_min(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG1_ADDR);
    reg_value &= ~(AUD_AGC_CONFIG1_AGC_MIN_MASK << AUD_AGC_CONFIG1_AGC_MIN_POS);
    reg_value |= ((value & AUD_AGC_CONFIG1_AGC_MIN_MASK) << AUD_AGC_CONFIG1_AGC_MIN_POS);
    REG_WRITE(AUD_AGC_CONFIG1_ADDR,reg_value);
}

/* REG_0x0c:agc_config1->agc_max:0xc[29:23],Maximum value of {GAIN2[2:0] GAIN1[3:0]}; Also the  default gain setting when AGC is disabled,0x0,R/W*/
static inline uint32_t aud_ll_get_agc_config1_agc_max(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG1_ADDR);
    reg_value = ((reg_value >> AUD_AGC_CONFIG1_AGC_MAX_POS) & AUD_AGC_CONFIG1_AGC_MAX_MASK);
    return reg_value;
}

static inline void aud_ll_set_agc_config1_agc_max(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG1_ADDR);
    reg_value &= ~(AUD_AGC_CONFIG1_AGC_MAX_MASK << AUD_AGC_CONFIG1_AGC_MAX_POS);
    reg_value |= ((value & AUD_AGC_CONFIG1_AGC_MAX_MASK) << AUD_AGC_CONFIG1_AGC_MAX_POS);
    REG_WRITE(AUD_AGC_CONFIG1_ADDR,reg_value);
}

/* REG_0x0c:agc_config1->agc_ng_method:0xc[30],0: 直接mute或者demute (default); 1: 逐渐降低增益或者增加增益,0x0,R/W*/
static inline uint32_t aud_ll_get_agc_config1_agc_ng_method(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG1_ADDR);
    reg_value = ((reg_value >> AUD_AGC_CONFIG1_AGC_NG_METHOD_POS) & AUD_AGC_CONFIG1_AGC_NG_METHOD_MASK);
    return reg_value;
}

static inline void aud_ll_set_agc_config1_agc_ng_method(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG1_ADDR);
    reg_value &= ~(AUD_AGC_CONFIG1_AGC_NG_METHOD_MASK << AUD_AGC_CONFIG1_AGC_NG_METHOD_POS);
    reg_value |= ((value & AUD_AGC_CONFIG1_AGC_NG_METHOD_MASK) << AUD_AGC_CONFIG1_AGC_NG_METHOD_POS);
    REG_WRITE(AUD_AGC_CONFIG1_ADDR,reg_value);
}

/* REG_0x0c:agc_config1->agc_ng_enable:0xc[31],使能 noise gating,0x0,R/W*/
static inline uint32_t aud_ll_get_agc_config1_agc_ng_enable(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG1_ADDR);
    reg_value = ((reg_value >> AUD_AGC_CONFIG1_AGC_NG_ENABLE_POS) & AUD_AGC_CONFIG1_AGC_NG_ENABLE_MASK);
    return reg_value;
}

static inline void aud_ll_set_agc_config1_agc_ng_enable(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG1_ADDR);
    reg_value &= ~(AUD_AGC_CONFIG1_AGC_NG_ENABLE_MASK << AUD_AGC_CONFIG1_AGC_NG_ENABLE_POS);
    reg_value |= ((value & AUD_AGC_CONFIG1_AGC_NG_ENABLE_MASK) << AUD_AGC_CONFIG1_AGC_NG_ENABLE_POS);
    REG_WRITE(AUD_AGC_CONFIG1_ADDR,reg_value);
}

/* REG_0x0D */
#define AUD_AGC_CONFIG2_ADDR  (AUD_LL_REG_BASE  + 0xD*4) //REG ADDR :0x47800034
#define AUD_AGC_CONFIG2_AGC_DECAY_TIME_POS (0) 
#define AUD_AGC_CONFIG2_AGC_DECAY_TIME_MASK (0x7) 

#define AUD_AGC_CONFIG2_AGC_ATTACK_TIME_POS (3) 
#define AUD_AGC_CONFIG2_AGC_ATTACK_TIME_MASK (0x7) 

#define AUD_AGC_CONFIG2_AGC_HIGH_THRD_POS (6) 
#define AUD_AGC_CONFIG2_AGC_HIGH_THRD_MASK (0x1F) 

#define AUD_AGC_CONFIG2_AGC_LOW_THRD_POS (11) 
#define AUD_AGC_CONFIG2_AGC_LOW_THRD_MASK (0x1F) 

#define AUD_AGC_CONFIG2_AGC_IIR_COEF_POS (16) 
#define AUD_AGC_CONFIG2_AGC_IIR_COEF_MASK (0x7) 

#define AUD_AGC_CONFIG2_AGC_ENABLE_POS (19) 
#define AUD_AGC_CONFIG2_AGC_ENABLE_MASK (0x1) 

#define AUD_AGC_CONFIG2_MANUAL_PGA_VALUE_POS (20) 
#define AUD_AGC_CONFIG2_MANUAL_PGA_VALUE_MASK (0x7F) 

#define AUD_AGC_CONFIG2_MANUAL_PGA_POS (27) 
#define AUD_AGC_CONFIG2_MANUAL_PGA_MASK (0x1) 

#define AUD_AGC_CONFIG2_RESERVED0_POS (28) 
#define AUD_AGC_CONFIG2_RESERVED0_MASK (0xF) 

static inline uint32_t aud_ll_get_agc_config2_value(void)
{
    return REG_READ(AUD_AGC_CONFIG2_ADDR);
}

static inline void aud_ll_set_agc_config2_value(uint32_t value)
{
    REG_WRITE(AUD_AGC_CONFIG2_ADDR,value);
}

/* REG_0x0d:agc_config2->agc_decay_time:0xd[2:0],0: 128 (~2.7 ms), 1: 256, 2: 512, 3: 1024, 4:2048, 5: 4096, 6: 8192, 7: 16384 (~340 ms),0x0,R/W*/
static inline uint32_t aud_ll_get_agc_config2_agc_decay_time(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG2_ADDR);
    reg_value = ((reg_value >> AUD_AGC_CONFIG2_AGC_DECAY_TIME_POS) & AUD_AGC_CONFIG2_AGC_DECAY_TIME_MASK);
    return reg_value;
}

static inline void aud_ll_set_agc_config2_agc_decay_time(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG2_ADDR);
    reg_value &= ~(AUD_AGC_CONFIG2_AGC_DECAY_TIME_MASK << AUD_AGC_CONFIG2_AGC_DECAY_TIME_POS);
    reg_value |= ((value & AUD_AGC_CONFIG2_AGC_DECAY_TIME_MASK) << AUD_AGC_CONFIG2_AGC_DECAY_TIME_POS);
    REG_WRITE(AUD_AGC_CONFIG2_ADDR,reg_value);
}

/* REG_0x0d:agc_config2->agc_attack_time:0xd[5:3],0: 8 (~0.1667ms), 1: 16, 2: 32, 3: 64, 4: 128, 5: 256, 6: 512, 7: 1024 (~21 ms),0x0,R/W*/
static inline uint32_t aud_ll_get_agc_config2_agc_attack_time(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG2_ADDR);
    reg_value = ((reg_value >> AUD_AGC_CONFIG2_AGC_ATTACK_TIME_POS) & AUD_AGC_CONFIG2_AGC_ATTACK_TIME_MASK);
    return reg_value;
}

static inline void aud_ll_set_agc_config2_agc_attack_time(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG2_ADDR);
    reg_value &= ~(AUD_AGC_CONFIG2_AGC_ATTACK_TIME_MASK << AUD_AGC_CONFIG2_AGC_ATTACK_TIME_POS);
    reg_value |= ((value & AUD_AGC_CONFIG2_AGC_ATTACK_TIME_MASK) << AUD_AGC_CONFIG2_AGC_ATTACK_TIME_POS);
    REG_WRITE(AUD_AGC_CONFIG2_ADDR,reg_value);
}

/* REG_0x0d:agc_config2->agc_high_thrd:0xd[10:6],对应到MIC_RSSI的最高5位,0x0,R/W*/
static inline uint32_t aud_ll_get_agc_config2_agc_high_thrd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG2_ADDR);
    reg_value = ((reg_value >> AUD_AGC_CONFIG2_AGC_HIGH_THRD_POS) & AUD_AGC_CONFIG2_AGC_HIGH_THRD_MASK);
    return reg_value;
}

static inline void aud_ll_set_agc_config2_agc_high_thrd(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG2_ADDR);
    reg_value &= ~(AUD_AGC_CONFIG2_AGC_HIGH_THRD_MASK << AUD_AGC_CONFIG2_AGC_HIGH_THRD_POS);
    reg_value |= ((value & AUD_AGC_CONFIG2_AGC_HIGH_THRD_MASK) << AUD_AGC_CONFIG2_AGC_HIGH_THRD_POS);
    REG_WRITE(AUD_AGC_CONFIG2_ADDR,reg_value);
}

/* REG_0x0d:agc_config2->agc_low_thrd:0xd[15:11],对应到MIC_RSSI的最高5位,0x0,R/W*/
static inline uint32_t aud_ll_get_agc_config2_agc_low_thrd(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG2_ADDR);
    reg_value = ((reg_value >> AUD_AGC_CONFIG2_AGC_LOW_THRD_POS) & AUD_AGC_CONFIG2_AGC_LOW_THRD_MASK);
    return reg_value;
}

static inline void aud_ll_set_agc_config2_agc_low_thrd(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG2_ADDR);
    reg_value &= ~(AUD_AGC_CONFIG2_AGC_LOW_THRD_MASK << AUD_AGC_CONFIG2_AGC_LOW_THRD_POS);
    reg_value |= ((value & AUD_AGC_CONFIG2_AGC_LOW_THRD_MASK) << AUD_AGC_CONFIG2_AGC_LOW_THRD_POS);
    REG_WRITE(AUD_AGC_CONFIG2_ADDR,reg_value);
}

/* REG_0x0d:agc_config2->agc_iir_coef:0xd[18:16],IIR系数选择 0:1/32 1:1/64 2:1/128 3:1/256 4:1/512 5:1/1024 6:1/2048 7:1/4096,0x0,R/W*/
static inline uint32_t aud_ll_get_agc_config2_agc_iir_coef(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG2_ADDR);
    reg_value = ((reg_value >> AUD_AGC_CONFIG2_AGC_IIR_COEF_POS) & AUD_AGC_CONFIG2_AGC_IIR_COEF_MASK);
    return reg_value;
}

static inline void aud_ll_set_agc_config2_agc_iir_coef(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG2_ADDR);
    reg_value &= ~(AUD_AGC_CONFIG2_AGC_IIR_COEF_MASK << AUD_AGC_CONFIG2_AGC_IIR_COEF_POS);
    reg_value |= ((value & AUD_AGC_CONFIG2_AGC_IIR_COEF_MASK) << AUD_AGC_CONFIG2_AGC_IIR_COEF_POS);
    REG_WRITE(AUD_AGC_CONFIG2_ADDR,reg_value);
}

/* REG_0x0d:agc_config2->agc_enable:0xd[19],AGC使能位,0x0,R/W*/
static inline uint32_t aud_ll_get_agc_config2_agc_enable(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG2_ADDR);
    reg_value = ((reg_value >> AUD_AGC_CONFIG2_AGC_ENABLE_POS) & AUD_AGC_CONFIG2_AGC_ENABLE_MASK);
    return reg_value;
}

static inline void aud_ll_set_agc_config2_agc_enable(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG2_ADDR);
    reg_value &= ~(AUD_AGC_CONFIG2_AGC_ENABLE_MASK << AUD_AGC_CONFIG2_AGC_ENABLE_POS);
    reg_value |= ((value & AUD_AGC_CONFIG2_AGC_ENABLE_MASK) << AUD_AGC_CONFIG2_AGC_ENABLE_POS);
    REG_WRITE(AUD_AGC_CONFIG2_ADDR,reg_value);
}

/* REG_0x0d:agc_config2->manual_pga_value:0xd[26:20],手动PGA的值,0x0,R/W*/
static inline uint32_t aud_ll_get_agc_config2_manual_pga_value(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG2_ADDR);
    reg_value = ((reg_value >> AUD_AGC_CONFIG2_MANUAL_PGA_VALUE_POS) & AUD_AGC_CONFIG2_MANUAL_PGA_VALUE_MASK);
    return reg_value;
}

static inline void aud_ll_set_agc_config2_manual_pga_value(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG2_ADDR);
    reg_value &= ~(AUD_AGC_CONFIG2_MANUAL_PGA_VALUE_MASK << AUD_AGC_CONFIG2_MANUAL_PGA_VALUE_POS);
    reg_value |= ((value & AUD_AGC_CONFIG2_MANUAL_PGA_VALUE_MASK) << AUD_AGC_CONFIG2_MANUAL_PGA_VALUE_POS);
    REG_WRITE(AUD_AGC_CONFIG2_ADDR,reg_value);
}

/* REG_0x0d:agc_config2->manual_pga:0xd[27],手动PGA模式,0x0,R/W*/
static inline uint32_t aud_ll_get_agc_config2_manual_pga(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG2_ADDR);
    reg_value = ((reg_value >> AUD_AGC_CONFIG2_MANUAL_PGA_POS) & AUD_AGC_CONFIG2_MANUAL_PGA_MASK);
    return reg_value;
}

static inline void aud_ll_set_agc_config2_manual_pga(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_CONFIG2_ADDR);
    reg_value &= ~(AUD_AGC_CONFIG2_MANUAL_PGA_MASK << AUD_AGC_CONFIG2_MANUAL_PGA_POS);
    reg_value |= ((value & AUD_AGC_CONFIG2_MANUAL_PGA_MASK) << AUD_AGC_CONFIG2_MANUAL_PGA_POS);
    REG_WRITE(AUD_AGC_CONFIG2_ADDR,reg_value);
}

/* REG_0x0E */
#define AUD_FIFO_STATUS_ADDR  (AUD_LL_REG_BASE  + 0xE*4) //REG ADDR :0x47800038
#define AUD_FIFO_STATUS_DACR_NEAR_FULL_POS (0) 
#define AUD_FIFO_STATUS_DACR_NEAR_FULL_MASK (0x1) 

#define AUD_FIFO_STATUS_DACL_NEAR_FULL_POS (1) 
#define AUD_FIFO_STATUS_DACL_NEAR_FULL_MASK (0x1) 

#define AUD_FIFO_STATUS_ADCL_NEAR_FULL_POS (2) 
#define AUD_FIFO_STATUS_ADCL_NEAR_FULL_MASK (0x1) 

#define AUD_FIFO_STATUS_DTMF_NEAR_FULL_POS (3) 
#define AUD_FIFO_STATUS_DTMF_NEAR_FULL_MASK (0x1) 

#define AUD_FIFO_STATUS_DACR_NEAR_EMPTY_POS (4) 
#define AUD_FIFO_STATUS_DACR_NEAR_EMPTY_MASK (0x1) 

#define AUD_FIFO_STATUS_DACL_NEAR_EMPTY_POS (5) 
#define AUD_FIFO_STATUS_DACL_NEAR_EMPTY_MASK (0x1) 

#define AUD_FIFO_STATUS_ADCL_NEAR_EMPTY_POS (6) 
#define AUD_FIFO_STATUS_ADCL_NEAR_EMPTY_MASK (0x1) 

#define AUD_FIFO_STATUS_DTMF_NEAR_EMPTY_POS (7) 
#define AUD_FIFO_STATUS_DTMF_NEAR_EMPTY_MASK (0x1) 

#define AUD_FIFO_STATUS_DACR_FIFO_FULL_POS (8) 
#define AUD_FIFO_STATUS_DACR_FIFO_FULL_MASK (0x1) 

#define AUD_FIFO_STATUS_DACL_FIFO_FULL_POS (9) 
#define AUD_FIFO_STATUS_DACL_FIFO_FULL_MASK (0x1) 

#define AUD_FIFO_STATUS_ADCL_FIFO_FULL_POS (10) 
#define AUD_FIFO_STATUS_ADCL_FIFO_FULL_MASK (0x1) 

#define AUD_FIFO_STATUS_DTMF_FIFO_FULL_POS (11) 
#define AUD_FIFO_STATUS_DTMF_FIFO_FULL_MASK (0x1) 

#define AUD_FIFO_STATUS_DACR_FIFO_EMPTY_POS (12) 
#define AUD_FIFO_STATUS_DACR_FIFO_EMPTY_MASK (0x1) 

#define AUD_FIFO_STATUS_DACL_FIFO_EMPTY_POS (13) 
#define AUD_FIFO_STATUS_DACL_FIFO_EMPTY_MASK (0x1) 

#define AUD_FIFO_STATUS_ADCL_FIFO_EMPTY_POS (14) 
#define AUD_FIFO_STATUS_ADCL_FIFO_EMPTY_MASK (0x1) 

#define AUD_FIFO_STATUS_DTMF_FIFO_EMPTY_POS (15) 
#define AUD_FIFO_STATUS_DTMF_FIFO_EMPTY_MASK (0x1) 

#define AUD_FIFO_STATUS_DACR_INT_FLAG_POS (16) 
#define AUD_FIFO_STATUS_DACR_INT_FLAG_MASK (0x1) 

#define AUD_FIFO_STATUS_DACL_INT_FLAG_POS (17) 
#define AUD_FIFO_STATUS_DACL_INT_FLAG_MASK (0x1) 

#define AUD_FIFO_STATUS_ADCL_INT_FLAG_POS (18) 
#define AUD_FIFO_STATUS_ADCL_INT_FLAG_MASK (0x1) 

#define AUD_FIFO_STATUS_DTMF_INT_FLAG_POS (19) 
#define AUD_FIFO_STATUS_DTMF_INT_FLAG_MASK (0x1) 

#define AUD_FIFO_STATUS_DMIC_NEAR_FULL_POS (20) 
#define AUD_FIFO_STATUS_DMIC_NEAR_FULL_MASK (0x1) 

#define AUD_FIFO_STATUS_DMIC_NEAR_EMPTY_POS (21) 
#define AUD_FIFO_STATUS_DMIC_NEAR_EMPTY_MASK (0x1) 

#define AUD_FIFO_STATUS_DMIC_FIFO_FULL_POS (22) 
#define AUD_FIFO_STATUS_DMIC_FIFO_FULL_MASK (0x1) 

#define AUD_FIFO_STATUS_DMIC_FIFO_EMPTY_POS (23) 
#define AUD_FIFO_STATUS_DMIC_FIFO_EMPTY_MASK (0x1) 

#define AUD_FIFO_STATUS_DMIC_INT_FLAG_POS (24) 
#define AUD_FIFO_STATUS_DMIC_INT_FLAG_MASK (0x1) 

#define AUD_FIFO_STATUS_RESERVED0_POS (25) 
#define AUD_FIFO_STATUS_RESERVED0_MASK (0x7F) 

static inline uint32_t aud_ll_get_fifo_status_value(void)
{
    return REG_READ(AUD_FIFO_STATUS_ADDR);
}

/* REG_0x0e:fifo_status->dacr_near_full:0xe[0],DAC右声道FIFO快要满  （默认大于3/4的容量）,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_dacr_near_full(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_DACR_NEAR_FULL_POS)&AUD_FIFO_STATUS_DACR_NEAR_FULL_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->dacl_near_full:0xe[1],DAC左声道FIFO快要满,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_dacl_near_full(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_DACL_NEAR_FULL_POS)&AUD_FIFO_STATUS_DACL_NEAR_FULL_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->adcl_near_full:0xe[2],ADC左声道FIFO快要满,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_adcl_near_full(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_ADCL_NEAR_FULL_POS)&AUD_FIFO_STATUS_ADCL_NEAR_FULL_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->dtmf_near_full:0xe[3],DTMF的FIFO快要满,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_dtmf_near_full(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_DTMF_NEAR_FULL_POS)&AUD_FIFO_STATUS_DTMF_NEAR_FULL_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->dacr_near_empty:0xe[4],DAC右声道FIFO快要空  （默认小于1/4的容量）,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_dacr_near_empty(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_DACR_NEAR_EMPTY_POS)&AUD_FIFO_STATUS_DACR_NEAR_EMPTY_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->dacl_near_empty:0xe[5],DAC左声道FIFO快要空,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_dacl_near_empty(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_DACL_NEAR_EMPTY_POS)&AUD_FIFO_STATUS_DACL_NEAR_EMPTY_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->adcl_near_empty:0xe[6],ADC左声道FIFO快要空,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_adcl_near_empty(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_ADCL_NEAR_EMPTY_POS)&AUD_FIFO_STATUS_ADCL_NEAR_EMPTY_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->dtmf_near_empty:0xe[7],DTMF的FIFO快要空,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_dtmf_near_empty(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_DTMF_NEAR_EMPTY_POS)&AUD_FIFO_STATUS_DTMF_NEAR_EMPTY_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->dacr_fifo_full:0xe[8],DAC右声道FIFO已满    (达到FIFO容量的最大值）,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_dacr_fifo_full(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_DACR_FIFO_FULL_POS)&AUD_FIFO_STATUS_DACR_FIFO_FULL_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->dacl_fifo_full:0xe[9],DAC左声道FIFO已满,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_dacl_fifo_full(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_DACL_FIFO_FULL_POS)&AUD_FIFO_STATUS_DACL_FIFO_FULL_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->adcl_fifo_full:0xe[10],ADC左声道FIFO已满,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_adcl_fifo_full(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_ADCL_FIFO_FULL_POS)&AUD_FIFO_STATUS_ADCL_FIFO_FULL_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->dtmf_fifo_full:0xe[11],DTMF的FIFO已满,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_dtmf_fifo_full(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_DTMF_FIFO_FULL_POS)&AUD_FIFO_STATUS_DTMF_FIFO_FULL_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->dacr_fifo_empty:0xe[12],DAC右声道FIFO已空   (达到FIFO容量的最小值）,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_dacr_fifo_empty(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_DACR_FIFO_EMPTY_POS)&AUD_FIFO_STATUS_DACR_FIFO_EMPTY_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->dacl_fifo_empty:0xe[13],DAC左声道FIFO已空,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_dacl_fifo_empty(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_DACL_FIFO_EMPTY_POS)&AUD_FIFO_STATUS_DACL_FIFO_EMPTY_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->adcl_fifo_empty:0xe[14],ADC左声道FIFO已空,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_adcl_fifo_empty(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_ADCL_FIFO_EMPTY_POS)&AUD_FIFO_STATUS_ADCL_FIFO_EMPTY_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->dtmf_fifo_empty:0xe[15],DTMF的FIFO已空,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_dtmf_fifo_empty(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_DTMF_FIFO_EMPTY_POS)&AUD_FIFO_STATUS_DTMF_FIFO_EMPTY_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->dacr_int_flag:0xe[16],DAC右声道中断标志,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_dacr_int_flag(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_DACR_INT_FLAG_POS)&AUD_FIFO_STATUS_DACR_INT_FLAG_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->dacl_int_flag:0xe[17],DAC左声道中断标志,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_dacl_int_flag(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_DACL_INT_FLAG_POS)&AUD_FIFO_STATUS_DACL_INT_FLAG_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->adcl_int_flag:0xe[18],ADC左声道中断标志,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_adcl_int_flag(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_ADCL_INT_FLAG_POS)&AUD_FIFO_STATUS_ADCL_INT_FLAG_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->dtmf_int_flag:0xe[19],DTMF的中断标志,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_dtmf_int_flag(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_DTMF_INT_FLAG_POS)&AUD_FIFO_STATUS_DTMF_INT_FLAG_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->dmic_near_full:0xe[20],DMIC的FIFO快要满,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_dmic_near_full(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_DMIC_NEAR_FULL_POS)&AUD_FIFO_STATUS_DMIC_NEAR_FULL_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->dmic_near_empty:0xe[21],DMIC的FIFO快要空,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_dmic_near_empty(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_DMIC_NEAR_EMPTY_POS)&AUD_FIFO_STATUS_DMIC_NEAR_EMPTY_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->dmic_fifo_full:0xe[22],DMIC的FIFO已满,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_dmic_fifo_full(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_DMIC_FIFO_FULL_POS)&AUD_FIFO_STATUS_DMIC_FIFO_FULL_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->dmic_fifo_empty:0xe[23],DMIC的FIFO已空,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_dmic_fifo_empty(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_DMIC_FIFO_EMPTY_POS)&AUD_FIFO_STATUS_DMIC_FIFO_EMPTY_MASK);
    return reg_value;
}

/* REG_0x0e:fifo_status->dmic_int_flag:0xe[24],DMIC的中断标志,0x0,R*/
static inline uint32_t aud_ll_get_fifo_status_dmic_int_flag(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FIFO_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_FIFO_STATUS_DMIC_INT_FLAG_POS)&AUD_FIFO_STATUS_DMIC_INT_FLAG_MASK);
    return reg_value;
}

/* REG_0x0F */
#define AUD_AGC_STATUS_ADDR  (AUD_LL_REG_BASE  + 0xF*4) //REG ADDR :0x4780003c
#define AUD_AGC_STATUS_RSSI_POS (0) 
#define AUD_AGC_STATUS_RSSI_MASK (0xFF) 

#define AUD_AGC_STATUS_MIC_PGA_POS (8) 
#define AUD_AGC_STATUS_MIC_PGA_MASK (0xFF) 

#define AUD_AGC_STATUS_MIC_RSSI_POS (16) 
#define AUD_AGC_STATUS_MIC_RSSI_MASK (0xFFFF) 

static inline uint32_t aud_ll_get_agc_status_value(void)
{
    return REG_READ(AUD_AGC_STATUS_ADDR);
}

/* REG_0x0f:agc_status->rssi:0xf[7:0],Absolutely microphone signal power in dB unit; Read only,0x0,R*/
static inline uint32_t aud_ll_get_agc_status_rssi(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_AGC_STATUS_RSSI_POS)&AUD_AGC_STATUS_RSSI_MASK);
    return reg_value;
}

/* REG_0x0f:agc_status->mic_pga:0xf[15:8],{1'b0, GAIN2[2:0] GAIN1[3:0]} (Read only),0x0,R*/
static inline uint32_t aud_ll_get_agc_status_mic_pga(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_AGC_STATUS_MIC_PGA_POS)&AUD_AGC_STATUS_MIC_PGA_MASK);
    return reg_value;
}

/* REG_0x0f:agc_status->mic_rssi:0xf[31:16],Microphone level; MIC_RSSI[15:0]; Read only,0x0,R*/
static inline uint32_t aud_ll_get_agc_status_mic_rssi(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AGC_STATUS_ADDR);
    reg_value = ((reg_value >> AUD_AGC_STATUS_MIC_RSSI_POS)&AUD_AGC_STATUS_MIC_RSSI_MASK);
    return reg_value;
}

/* REG_0x10 */
#define AUD_DTMF_FPORT_ADDR  (AUD_LL_REG_BASE  + 0x10*4) //REG ADDR :0x47800040
#define AUD_DTMF_FPORT_REG2TON_L_DI_POS (0) 
#define AUD_DTMF_FPORT_REG2TON_L_DI_MASK (0xFFFF) 

#define AUD_DTMF_FPORT_RESERVED0_POS (16) 
#define AUD_DTMF_FPORT_RESERVED0_MASK (0xFFFF) 

static inline uint32_t aud_ll_get_dtmf_fport_value(void)
{
    return REG_READ(AUD_DTMF_FPORT_ADDR);
}

/* REG_0x10:dtmf_fport->reg2ton_l_di:0x10[15:0],DTMF的FIFO读出地址,0x0,R*/
static inline uint32_t aud_ll_get_dtmf_fport_reg2ton_l_di(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DTMF_FPORT_ADDR);
    reg_value = ((reg_value >> AUD_DTMF_FPORT_REG2TON_L_DI_POS)&AUD_DTMF_FPORT_REG2TON_L_DI_MASK);
    return reg_value;
}

/* REG_0x11 */
#define AUD_ADC_FPORT_ADDR  (AUD_LL_REG_BASE  + 0x11*4) //REG ADDR :0x47800044
#define AUD_ADC_FPORT_REG2ADC_L_DI_POS (0) 
#define AUD_ADC_FPORT_REG2ADC_L_DI_MASK (0xFFFF) 

#define AUD_ADC_FPORT_REG2ADC_R_DI_POS (16) 
#define AUD_ADC_FPORT_REG2ADC_R_DI_MASK (0xFFFF) 

static inline uint32_t aud_ll_get_adc_fport_value(void)
{
    return REG_READ(AUD_ADC_FPORT_ADDR);
}

/* REG_0x11:adc_fport->reg2adc_l_di:0x11[15:0],ADC的FIFO读出地址，高16位是右声道，低16位是左声道,0x0,R*/
static inline uint32_t aud_ll_get_adc_fport_reg2adc_l_di(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_FPORT_ADDR);
    reg_value = ((reg_value >> AUD_ADC_FPORT_REG2ADC_L_DI_POS)&AUD_ADC_FPORT_REG2ADC_L_DI_MASK);
    return reg_value;
}

/* REG_0x11:adc_fport->reg2adc_r_di:0x11[31:16], ,0x0,R*/
static inline uint32_t aud_ll_get_adc_fport_reg2adc_r_di(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_ADC_FPORT_ADDR);
    reg_value = ((reg_value >> AUD_ADC_FPORT_REG2ADC_R_DI_POS)&AUD_ADC_FPORT_REG2ADC_R_DI_MASK);
    return reg_value;
}

/* REG_0x12 */
#define AUD_DAC_FPORT_ADDR  (AUD_LL_REG_BASE  + 0x12*4) //REG ADDR :0x47800048
#define AUD_DAC_FPORT_DACL_PORT_POS (0) 
#define AUD_DAC_FPORT_DACL_PORT_MASK (0xFFFF) 

#define AUD_DAC_FPORT_DACR_PORT_POS (16) 
#define AUD_DAC_FPORT_DACR_PORT_MASK (0xFFFF) 

/*write only reg:dac_fport:default value:0x0*/
static inline void aud_ll_set_dac_fport_value(uint32_t value)
{
    REG_WRITE(AUD_DAC_FPORT_ADDR,value);
}

/* REG_0x13 */
#define AUD_DMIC_FPORT_ADDR  (AUD_LL_REG_BASE  + 0x13*4) //REG ADDR :0x4780004c
#define AUD_DMIC_FPORT_REG2DMIC_L_DI_POS (0) 
#define AUD_DMIC_FPORT_REG2DMIC_L_DI_MASK (0xFFFF) 

#define AUD_DMIC_FPORT_REG2DMIC_R_DI_POS (16) 
#define AUD_DMIC_FPORT_REG2DMIC_R_DI_MASK (0xFFFF) 

static inline uint32_t aud_ll_get_dmic_fport_value(void)
{
    return REG_READ(AUD_DMIC_FPORT_ADDR);
}

/* REG_0x13:dmic_fport->reg2dmic_l_di:0x13[15:0],DMIC的FIFO读出地址，高16位是右声道，低16位是左声道,0x0,R*/
static inline uint32_t aud_ll_get_dmic_fport_reg2dmic_l_di(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DMIC_FPORT_ADDR);
    reg_value = ((reg_value >> AUD_DMIC_FPORT_REG2DMIC_L_DI_POS)&AUD_DMIC_FPORT_REG2DMIC_L_DI_MASK);
    return reg_value;
}

/* REG_0x13:dmic_fport->reg2dmic_r_di:0x13[31:16], ,0x0,R*/
static inline uint32_t aud_ll_get_dmic_fport_reg2dmic_r_di(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DMIC_FPORT_ADDR);
    reg_value = ((reg_value >> AUD_DMIC_FPORT_REG2DMIC_R_DI_POS)&AUD_DMIC_FPORT_REG2DMIC_R_DI_MASK);
    return reg_value;
}

/* REG_0x18 */
#define AUD_EXTEND_CFG_ADDR  (AUD_LL_REG_BASE  + 0x18*4) //REG ADDR :0x47800060
#define AUD_EXTEND_CFG_DAC_FRACMOD_MANUAL_POS (0) 
#define AUD_EXTEND_CFG_DAC_FRACMOD_MANUAL_MASK (0x1) 

#define AUD_EXTEND_CFG_ADC_FRACMOD_MANUAL_POS (1) 
#define AUD_EXTEND_CFG_ADC_FRACMOD_MANUAL_MASK (0x1) 

#define AUD_EXTEND_CFG_FILT_ENABLE_POS (2) 
#define AUD_EXTEND_CFG_FILT_ENABLE_MASK (0x1) 

#define AUD_EXTEND_CFG_RESERVED0_POS (3) 
#define AUD_EXTEND_CFG_RESERVED0_MASK (0x1FFFFFFF) 

static inline uint32_t aud_ll_get_extend_cfg_value(void)
{
    return REG_READ(AUD_EXTEND_CFG_ADDR);
}

static inline void aud_ll_set_extend_cfg_value(uint32_t value)
{
    REG_WRITE(AUD_EXTEND_CFG_ADDR,value);
}

/* REG_0x18:extend_cfg->dac_fracmod_manual:0x18[0],DAC小数分频手动设置使能,0x0,R/W*/
static inline uint32_t aud_ll_get_extend_cfg_dac_fracmod_manual(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_EXTEND_CFG_ADDR);
    reg_value = ((reg_value >> AUD_EXTEND_CFG_DAC_FRACMOD_MANUAL_POS) & AUD_EXTEND_CFG_DAC_FRACMOD_MANUAL_MASK);
    return reg_value;
}

static inline void aud_ll_set_extend_cfg_dac_fracmod_manual(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_EXTEND_CFG_ADDR);
    reg_value &= ~(AUD_EXTEND_CFG_DAC_FRACMOD_MANUAL_MASK << AUD_EXTEND_CFG_DAC_FRACMOD_MANUAL_POS);
    reg_value |= ((value & AUD_EXTEND_CFG_DAC_FRACMOD_MANUAL_MASK) << AUD_EXTEND_CFG_DAC_FRACMOD_MANUAL_POS);
    REG_WRITE(AUD_EXTEND_CFG_ADDR,reg_value);
}

/* REG_0x18:extend_cfg->adc_fracmod_manual:0x18[1],ADC小数分频手动设置使能,0x0,R/W*/
static inline uint32_t aud_ll_get_extend_cfg_adc_fracmod_manual(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_EXTEND_CFG_ADDR);
    reg_value = ((reg_value >> AUD_EXTEND_CFG_ADC_FRACMOD_MANUAL_POS) & AUD_EXTEND_CFG_ADC_FRACMOD_MANUAL_MASK);
    return reg_value;
}

static inline void aud_ll_set_extend_cfg_adc_fracmod_manual(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_EXTEND_CFG_ADDR);
    reg_value &= ~(AUD_EXTEND_CFG_ADC_FRACMOD_MANUAL_MASK << AUD_EXTEND_CFG_ADC_FRACMOD_MANUAL_POS);
    reg_value |= ((value & AUD_EXTEND_CFG_ADC_FRACMOD_MANUAL_MASK) << AUD_EXTEND_CFG_ADC_FRACMOD_MANUAL_POS);
    REG_WRITE(AUD_EXTEND_CFG_ADDR,reg_value);
}

/* REG_0x18:extend_cfg->filt_enable:0x18[2],DAC的频响滤波器使能,0x0,R/W*/
static inline uint32_t aud_ll_get_extend_cfg_filt_enable(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_EXTEND_CFG_ADDR);
    reg_value = ((reg_value >> AUD_EXTEND_CFG_FILT_ENABLE_POS) & AUD_EXTEND_CFG_FILT_ENABLE_MASK);
    return reg_value;
}

static inline void aud_ll_set_extend_cfg_filt_enable(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_EXTEND_CFG_ADDR);
    reg_value &= ~(AUD_EXTEND_CFG_FILT_ENABLE_MASK << AUD_EXTEND_CFG_FILT_ENABLE_POS);
    reg_value |= ((value & AUD_EXTEND_CFG_FILT_ENABLE_MASK) << AUD_EXTEND_CFG_FILT_ENABLE_POS);
    REG_WRITE(AUD_EXTEND_CFG_ADDR,reg_value);
}

/* REG_0x19 */
#define AUD_DAC_FRACMOD_ADDR  (AUD_LL_REG_BASE  + 0x19*4) //REG ADDR :0x47800064
#define AUD_DAC_FRACMOD_DAC_FRACCOEF_POS (0) 
#define AUD_DAC_FRACMOD_DAC_FRACCOEF_MASK (0xFFFFFFFF) 

static inline uint32_t aud_ll_get_dac_fracmod_value(void)
{
    return REG_READ(AUD_DAC_FRACMOD_ADDR);
}

static inline void aud_ll_set_dac_fracmod_value(uint32_t value)
{
    REG_WRITE(AUD_DAC_FRACMOD_ADDR,value);
}

/* REG_0x19:dac_fracmod->dac_fracCoef:0x19[31:0],DAC小数分频设置值 N * 2^24,0x0,R/W*/
static inline uint32_t aud_ll_get_dac_fracmod_dac_fraccoef(void)
{
    return REG_READ(AUD_DAC_FRACMOD_ADDR);
}

static inline void aud_ll_set_dac_fracmod_dac_fraccoef(uint32_t value)
{
    REG_WRITE(AUD_DAC_FRACMOD_ADDR,value);
}

/* REG_0x1A */
#define AUD_ADC_FRACMOD_ADDR  (AUD_LL_REG_BASE  + 0x1A*4) //REG ADDR :0x47800068
#define AUD_ADC_FRACMOD_ADC_FRACCOEF_POS (0) 
#define AUD_ADC_FRACMOD_ADC_FRACCOEF_MASK (0xFFFFFFFF) 

static inline uint32_t aud_ll_get_adc_fracmod_value(void)
{
    return REG_READ(AUD_ADC_FRACMOD_ADDR);
}

static inline void aud_ll_set_adc_fracmod_value(uint32_t value)
{
    REG_WRITE(AUD_ADC_FRACMOD_ADDR,value);
}

/* REG_0x1a:adc_fracmod->adc_fracCoef:0x1a[31:0],ADC小数分频设置值 N * 2^24,0x0,R/W*/
static inline uint32_t aud_ll_get_adc_fracmod_adc_fraccoef(void)
{
    return REG_READ(AUD_ADC_FRACMOD_ADDR);
}

static inline void aud_ll_set_adc_fracmod_adc_fraccoef(uint32_t value)
{
    REG_WRITE(AUD_ADC_FRACMOD_ADDR,value);
}

/* REG_0x1F */
#define AUD_HPF2_EXT_COEF_ADDR  (AUD_LL_REG_BASE  + 0x1F*4) //REG ADDR :0x4780007c
#define AUD_HPF2_EXT_COEF_HPF2_A1_L6BIT_POS (0) 
#define AUD_HPF2_EXT_COEF_HPF2_A1_L6BIT_MASK (0x3F) 

#define AUD_HPF2_EXT_COEF_HPF2_A2_L6BIT_POS (6) 
#define AUD_HPF2_EXT_COEF_HPF2_A2_L6BIT_MASK (0x3F) 

#define AUD_HPF2_EXT_COEF_HPF2_B0_L6BIT_POS (12) 
#define AUD_HPF2_EXT_COEF_HPF2_B0_L6BIT_MASK (0x3F) 

#define AUD_HPF2_EXT_COEF_HPF2_B1_L6BIT_POS (18) 
#define AUD_HPF2_EXT_COEF_HPF2_B1_L6BIT_MASK (0x3F) 

#define AUD_HPF2_EXT_COEF_HPF2_B2_L6BIT_POS (24) 
#define AUD_HPF2_EXT_COEF_HPF2_B2_L6BIT_MASK (0x3F) 

#define AUD_HPF2_EXT_COEF_RESERVED0_POS (30) 
#define AUD_HPF2_EXT_COEF_RESERVED0_MASK (0x3) 

static inline uint32_t aud_ll_get_hpf2_ext_coef_value(void)
{
    return REG_READ(AUD_HPF2_EXT_COEF_ADDR);
}

static inline void aud_ll_set_hpf2_ext_coef_value(uint32_t value)
{
    REG_WRITE(AUD_HPF2_EXT_COEF_ADDR,value);
}

/* REG_0x1f:hpf2_ext_coef->hpf2_A1_L6BIT:0x1f[5:0],DAC HPF2的系数A1低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_hpf2_ext_coef_hpf2_a1_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_HPF2_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_HPF2_EXT_COEF_HPF2_A1_L6BIT_POS) & AUD_HPF2_EXT_COEF_HPF2_A1_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_hpf2_ext_coef_hpf2_a1_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_HPF2_EXT_COEF_ADDR);
    reg_value &= ~(AUD_HPF2_EXT_COEF_HPF2_A1_L6BIT_MASK << AUD_HPF2_EXT_COEF_HPF2_A1_L6BIT_POS);
    reg_value |= ((value & AUD_HPF2_EXT_COEF_HPF2_A1_L6BIT_MASK) << AUD_HPF2_EXT_COEF_HPF2_A1_L6BIT_POS);
    REG_WRITE(AUD_HPF2_EXT_COEF_ADDR,reg_value);
}

/* REG_0x1f:hpf2_ext_coef->hpf2_A2_L6BIT:0x1f[11:6],DAC HPF2的系数A2低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_hpf2_ext_coef_hpf2_a2_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_HPF2_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_HPF2_EXT_COEF_HPF2_A2_L6BIT_POS) & AUD_HPF2_EXT_COEF_HPF2_A2_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_hpf2_ext_coef_hpf2_a2_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_HPF2_EXT_COEF_ADDR);
    reg_value &= ~(AUD_HPF2_EXT_COEF_HPF2_A2_L6BIT_MASK << AUD_HPF2_EXT_COEF_HPF2_A2_L6BIT_POS);
    reg_value |= ((value & AUD_HPF2_EXT_COEF_HPF2_A2_L6BIT_MASK) << AUD_HPF2_EXT_COEF_HPF2_A2_L6BIT_POS);
    REG_WRITE(AUD_HPF2_EXT_COEF_ADDR,reg_value);
}

/* REG_0x1f:hpf2_ext_coef->hpf2_B0_L6BIT:0x1f[17:12],DAC HPF2的系数B0低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_hpf2_ext_coef_hpf2_b0_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_HPF2_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_HPF2_EXT_COEF_HPF2_B0_L6BIT_POS) & AUD_HPF2_EXT_COEF_HPF2_B0_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_hpf2_ext_coef_hpf2_b0_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_HPF2_EXT_COEF_ADDR);
    reg_value &= ~(AUD_HPF2_EXT_COEF_HPF2_B0_L6BIT_MASK << AUD_HPF2_EXT_COEF_HPF2_B0_L6BIT_POS);
    reg_value |= ((value & AUD_HPF2_EXT_COEF_HPF2_B0_L6BIT_MASK) << AUD_HPF2_EXT_COEF_HPF2_B0_L6BIT_POS);
    REG_WRITE(AUD_HPF2_EXT_COEF_ADDR,reg_value);
}

/* REG_0x1f:hpf2_ext_coef->hpf2_B1_L6BIT:0x1f[23:18],DAC HPF2的系数B1低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_hpf2_ext_coef_hpf2_b1_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_HPF2_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_HPF2_EXT_COEF_HPF2_B1_L6BIT_POS) & AUD_HPF2_EXT_COEF_HPF2_B1_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_hpf2_ext_coef_hpf2_b1_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_HPF2_EXT_COEF_ADDR);
    reg_value &= ~(AUD_HPF2_EXT_COEF_HPF2_B1_L6BIT_MASK << AUD_HPF2_EXT_COEF_HPF2_B1_L6BIT_POS);
    reg_value |= ((value & AUD_HPF2_EXT_COEF_HPF2_B1_L6BIT_MASK) << AUD_HPF2_EXT_COEF_HPF2_B1_L6BIT_POS);
    REG_WRITE(AUD_HPF2_EXT_COEF_ADDR,reg_value);
}

/* REG_0x1f:hpf2_ext_coef->hpf2_B2_L6BIT:0x1f[29:24],DAC HPF2的系数B2低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_hpf2_ext_coef_hpf2_b2_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_HPF2_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_HPF2_EXT_COEF_HPF2_B2_L6BIT_POS) & AUD_HPF2_EXT_COEF_HPF2_B2_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_hpf2_ext_coef_hpf2_b2_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_HPF2_EXT_COEF_ADDR);
    reg_value &= ~(AUD_HPF2_EXT_COEF_HPF2_B2_L6BIT_MASK << AUD_HPF2_EXT_COEF_HPF2_B2_L6BIT_POS);
    reg_value |= ((value & AUD_HPF2_EXT_COEF_HPF2_B2_L6BIT_MASK) << AUD_HPF2_EXT_COEF_HPF2_B2_L6BIT_POS);
    REG_WRITE(AUD_HPF2_EXT_COEF_ADDR,reg_value);
}

/* REG_0x20 */
#define AUD_FLT0_COEF_ADDR  (AUD_LL_REG_BASE  + 0x20*4) //REG ADDR :0x47800080
#define AUD_FLT0_COEF_FLT0_A1_POS (0) 
#define AUD_FLT0_COEF_FLT0_A1_MASK (0xFFFF) 

#define AUD_FLT0_COEF_FLT0_A2_POS (16) 
#define AUD_FLT0_COEF_FLT0_A2_MASK (0xFFFF) 

static inline uint32_t aud_ll_get_flt0_coef_a1a2_value(void)
{
    return REG_READ(AUD_FLT0_COEF_ADDR);
}

static inline void aud_ll_set_flt0_coef_a1a2_value(uint32_t value)
{
    REG_WRITE(AUD_FLT0_COEF_ADDR,value);
}

/* REG_0x20:flt0_coef->flt0_A1:0x20[15:0],Filter 0 的IIR系数A1,0x0,R/W*/
static inline uint32_t aud_ll_get_flt0_coef_a1a2_flt0_a1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT0_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT0_COEF_FLT0_A1_POS) & AUD_FLT0_COEF_FLT0_A1_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt0_coef_a1a2_flt0_a1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT0_COEF_ADDR);
    reg_value &= ~(AUD_FLT0_COEF_FLT0_A1_MASK << AUD_FLT0_COEF_FLT0_A1_POS);
    reg_value |= ((value & AUD_FLT0_COEF_FLT0_A1_MASK) << AUD_FLT0_COEF_FLT0_A1_POS);
    REG_WRITE(AUD_FLT0_COEF_ADDR,reg_value);
}

/* REG_0x20:flt0_coef->flt0_A2:0x20[31:16],Filter 0 的IIR系数A2,0x0,R/W*/
static inline uint32_t aud_ll_get_flt0_coef_a1a2_flt0_a2(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT0_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT0_COEF_FLT0_A2_POS) & AUD_FLT0_COEF_FLT0_A2_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt0_coef_a1a2_flt0_a2(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT0_COEF_ADDR);
    reg_value &= ~(AUD_FLT0_COEF_FLT0_A2_MASK << AUD_FLT0_COEF_FLT0_A2_POS);
    reg_value |= ((value & AUD_FLT0_COEF_FLT0_A2_MASK) << AUD_FLT0_COEF_FLT0_A2_POS);
    REG_WRITE(AUD_FLT0_COEF_ADDR,reg_value);
}

/* REG_0x21 */
#define AUD_REG0X21_ADDR  (AUD_LL_REG_BASE  + 0x21*4) //REG ADDR :0x47800084
#define AUD_REG0X21_FLT0_B0_POS (0) 
#define AUD_REG0X21_FLT0_B0_MASK (0xFFFF) 

#define AUD_REG0X21_FLT0_B1_POS (16) 
#define AUD_REG0X21_FLT0_B1_MASK (0xFFFF) 

static inline uint32_t aud_ll_get_flt0_coef_b0b1_value(void)
{
    return REG_READ(AUD_REG0X21_ADDR);
}

static inline void aud_ll_set_flt0_coef_b0b1_value(uint32_t value)
{
    REG_WRITE(AUD_REG0X21_ADDR,value);
}

/* REG_0x21:reg0x21->flt0_B0:0x21[15:0],Filter 0 的IIR系数B0,0x0,R/W*/
static inline uint32_t aud_ll_get_flt0_coef_b0b1_flt0_b0(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X21_ADDR);
    reg_value = ((reg_value >> AUD_REG0X21_FLT0_B0_POS) & AUD_REG0X21_FLT0_B0_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt0_coef_b0b1_flt0_b0(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X21_ADDR);
    reg_value &= ~(AUD_REG0X21_FLT0_B0_MASK << AUD_REG0X21_FLT0_B0_POS);
    reg_value |= ((value & AUD_REG0X21_FLT0_B0_MASK) << AUD_REG0X21_FLT0_B0_POS);
    REG_WRITE(AUD_REG0X21_ADDR,reg_value);
}

/* REG_0x21:reg0x21->flt0_B1:0x21[31:16],Filter 0 的IIR系数B1,0x0,R/W*/
static inline uint32_t aud_ll_get_flt0_coef_b0b1_flt0_b1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X21_ADDR);
    reg_value = ((reg_value >> AUD_REG0X21_FLT0_B1_POS) & AUD_REG0X21_FLT0_B1_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt0_coef_b0b1_flt0_b1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X21_ADDR);
    reg_value &= ~(AUD_REG0X21_FLT0_B1_MASK << AUD_REG0X21_FLT0_B1_POS);
    reg_value |= ((value & AUD_REG0X21_FLT0_B1_MASK) << AUD_REG0X21_FLT0_B1_POS);
    REG_WRITE(AUD_REG0X21_ADDR,reg_value);
}

/* REG_0x22 */
#define AUD_REG0X22_ADDR  (AUD_LL_REG_BASE  + 0x22*4) //REG ADDR :0x47800088
#define AUD_REG0X22_FLT0_B2_POS (0) 
#define AUD_REG0X22_FLT0_B2_MASK (0xFFFF) 

#define AUD_REG0X22_RESERVED0_POS (16) 
#define AUD_REG0X22_RESERVED0_MASK (0xFFFF) 

static inline uint32_t aud_ll_get_flt0_coef_b2_value(void)
{
    return REG_READ(AUD_REG0X22_ADDR);
}

static inline void aud_ll_set_flt0_coef_b2_value(uint32_t value)
{
    REG_WRITE(AUD_REG0X22_ADDR,value);
}

/* REG_0x22:reg0x22->flt0_B2:0x22[15:0],Filter 0 的IIR系数B2,0x0,R/W*/
static inline uint32_t aud_ll_get_flt0_coef_b2_flt0_b2(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X22_ADDR);
    reg_value = ((reg_value >> AUD_REG0X22_FLT0_B2_POS) & AUD_REG0X22_FLT0_B2_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt0_coef_b2_flt0_b2(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X22_ADDR);
    reg_value &= ~(AUD_REG0X22_FLT0_B2_MASK << AUD_REG0X22_FLT0_B2_POS);
    reg_value |= ((value & AUD_REG0X22_FLT0_B2_MASK) << AUD_REG0X22_FLT0_B2_POS);
    REG_WRITE(AUD_REG0X22_ADDR,reg_value);
}

/* REG_0x23 */
#define AUD_FLT1_COEF_ADDR  (AUD_LL_REG_BASE  + 0x23*4) //REG ADDR :0x4780008c
#define AUD_FLT1_COEF_FLT1_A1_POS (0) 
#define AUD_FLT1_COEF_FLT1_A1_MASK (0xFFFF) 

#define AUD_FLT1_COEF_FLT1_A2_POS (16) 
#define AUD_FLT1_COEF_FLT1_A2_MASK (0xFFFF) 

static inline uint32_t aud_ll_get_flt1_coef_a1a2_value(void)
{
    return REG_READ(AUD_FLT1_COEF_ADDR);
}

static inline void aud_ll_set_flt1_coef_a1a2_value(uint32_t value)
{
    REG_WRITE(AUD_FLT1_COEF_ADDR,value);
}

/* REG_0x23:flt1_coef->flt1_A1:0x23[15:0],Filter 1 的IIR系数A1,0x0,R/W*/
static inline uint32_t aud_ll_get_flt1_coef_a1a2_flt1_a1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT1_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT1_COEF_FLT1_A1_POS) & AUD_FLT1_COEF_FLT1_A1_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt1_coef_a1a2_flt1_a1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT1_COEF_ADDR);
    reg_value &= ~(AUD_FLT1_COEF_FLT1_A1_MASK << AUD_FLT1_COEF_FLT1_A1_POS);
    reg_value |= ((value & AUD_FLT1_COEF_FLT1_A1_MASK) << AUD_FLT1_COEF_FLT1_A1_POS);
    REG_WRITE(AUD_FLT1_COEF_ADDR,reg_value);
}

/* REG_0x23:flt1_coef->flt1_A2:0x23[31:16],Filter 1 的IIR系数A2,0x0,R/W*/
static inline uint32_t aud_ll_get_flt1_coef_a1a2_flt1_a2(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT1_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT1_COEF_FLT1_A2_POS) & AUD_FLT1_COEF_FLT1_A2_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt1_coef_a1a2_flt1_a2(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT1_COEF_ADDR);
    reg_value &= ~(AUD_FLT1_COEF_FLT1_A2_MASK << AUD_FLT1_COEF_FLT1_A2_POS);
    reg_value |= ((value & AUD_FLT1_COEF_FLT1_A2_MASK) << AUD_FLT1_COEF_FLT1_A2_POS);
    REG_WRITE(AUD_FLT1_COEF_ADDR,reg_value);
}

/* REG_0x24 */
#define AUD_REG0X24_ADDR  (AUD_LL_REG_BASE  + 0x24*4) //REG ADDR :0x47800090
#define AUD_REG0X24_FLT1_B0_POS (0) 
#define AUD_REG0X24_FLT1_B0_MASK (0xFFFF) 

#define AUD_REG0X24_FLT1_B1_POS (16) 
#define AUD_REG0X24_FLT1_B1_MASK (0xFFFF) 

static inline uint32_t aud_ll_get_flt1_coef_b0b1_value(void)
{
    return REG_READ(AUD_REG0X24_ADDR);
}

static inline void aud_ll_set_flt1_coef_b0b1_value(uint32_t value)
{
    REG_WRITE(AUD_REG0X24_ADDR,value);
}

/* REG_0x24:reg0x24->flt1_B0:0x24[15:0],Filter 1 的IIR系数B0,0x0,R/W*/
static inline uint32_t aud_ll_get_flt1_coef_b0b1_flt1_b0(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X24_ADDR);
    reg_value = ((reg_value >> AUD_REG0X24_FLT1_B0_POS) & AUD_REG0X24_FLT1_B0_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt1_coef_b0b1_flt1_b0(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X24_ADDR);
    reg_value &= ~(AUD_REG0X24_FLT1_B0_MASK << AUD_REG0X24_FLT1_B0_POS);
    reg_value |= ((value & AUD_REG0X24_FLT1_B0_MASK) << AUD_REG0X24_FLT1_B0_POS);
    REG_WRITE(AUD_REG0X24_ADDR,reg_value);
}

/* REG_0x24:reg0x24->flt1_B1:0x24[31:16],Filter 1 的IIR系数B1,0x0,R/W*/
static inline uint32_t aud_ll_get_flt1_coef_b0b1_flt1_b1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X24_ADDR);
    reg_value = ((reg_value >> AUD_REG0X24_FLT1_B1_POS) & AUD_REG0X24_FLT1_B1_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt1_coef_b0b1_flt1_b1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X24_ADDR);
    reg_value &= ~(AUD_REG0X24_FLT1_B1_MASK << AUD_REG0X24_FLT1_B1_POS);
    reg_value |= ((value & AUD_REG0X24_FLT1_B1_MASK) << AUD_REG0X24_FLT1_B1_POS);
    REG_WRITE(AUD_REG0X24_ADDR,reg_value);
}

/* REG_0x25 */
#define AUD_REG0X25_ADDR  (AUD_LL_REG_BASE  + 0x25*4) //REG ADDR :0x47800094
#define AUD_REG0X25_FLT1_B2_POS (0) 
#define AUD_REG0X25_FLT1_B2_MASK (0xFFFF) 

#define AUD_REG0X25_RESERVED0_POS (16) 
#define AUD_REG0X25_RESERVED0_MASK (0xFFFF) 

static inline uint32_t aud_ll_get_flt1_coef_b2_value(void)
{
    return REG_READ(AUD_REG0X25_ADDR);
}

static inline void aud_ll_set_flt1_coef_b2_value(uint32_t value)
{
    REG_WRITE(AUD_REG0X25_ADDR,value);
}

/* REG_0x25:reg0x25->flt1_B2:0x25[15:0],Filter 1 的IIR系数B2,0x0,R/W*/
static inline uint32_t aud_ll_get_flt1_coef_b2_flt1_b2(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X25_ADDR);
    reg_value = ((reg_value >> AUD_REG0X25_FLT1_B2_POS) & AUD_REG0X25_FLT1_B2_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt1_coef_b2_flt1_b2(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X25_ADDR);
    reg_value &= ~(AUD_REG0X25_FLT1_B2_MASK << AUD_REG0X25_FLT1_B2_POS);
    reg_value |= ((value & AUD_REG0X25_FLT1_B2_MASK) << AUD_REG0X25_FLT1_B2_POS);
    REG_WRITE(AUD_REG0X25_ADDR,reg_value);
}

/* REG_0x26 */
#define AUD_FLT2_COEF_ADDR  (AUD_LL_REG_BASE  + 0x26*4) //REG ADDR :0x47800098
#define AUD_FLT2_COEF_FLT2_A1_POS (0) 
#define AUD_FLT2_COEF_FLT2_A1_MASK (0xFFFF) 

#define AUD_FLT2_COEF_FLT2_A2_POS (16) 
#define AUD_FLT2_COEF_FLT2_A2_MASK (0xFFFF) 

static inline uint32_t aud_ll_get_flt2_coef_a1a2_value(void)
{
    return REG_READ(AUD_FLT2_COEF_ADDR);
}

static inline void aud_ll_set_flt2_coef_a1a2_value(uint32_t value)
{
    REG_WRITE(AUD_FLT2_COEF_ADDR,value);
}

/* REG_0x26:flt2_coef->flt2_A1:0x26[15:0],Filter 2 的IIR系数A1,0x0,R/W*/
static inline uint32_t aud_ll_get_flt2_coef_a1a2_flt2_a1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT2_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT2_COEF_FLT2_A1_POS) & AUD_FLT2_COEF_FLT2_A1_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt2_coef_a1a2_flt2_a1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT2_COEF_ADDR);
    reg_value &= ~(AUD_FLT2_COEF_FLT2_A1_MASK << AUD_FLT2_COEF_FLT2_A1_POS);
    reg_value |= ((value & AUD_FLT2_COEF_FLT2_A1_MASK) << AUD_FLT2_COEF_FLT2_A1_POS);
    REG_WRITE(AUD_FLT2_COEF_ADDR,reg_value);
}

/* REG_0x26:flt2_coef->flt2_A2:0x26[31:16],Filter 2 的IIR系数A2,0x0,R/W*/
static inline uint32_t aud_ll_get_flt2_coef_a1a2_flt2_a2(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT2_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT2_COEF_FLT2_A2_POS) & AUD_FLT2_COEF_FLT2_A2_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt2_coef_a1a2_flt2_a2(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT2_COEF_ADDR);
    reg_value &= ~(AUD_FLT2_COEF_FLT2_A2_MASK << AUD_FLT2_COEF_FLT2_A2_POS);
    reg_value |= ((value & AUD_FLT2_COEF_FLT2_A2_MASK) << AUD_FLT2_COEF_FLT2_A2_POS);
    REG_WRITE(AUD_FLT2_COEF_ADDR,reg_value);
}

/* REG_0x27 */
#define AUD_REG0X27_ADDR  (AUD_LL_REG_BASE  + 0x27*4) //REG ADDR :0x4780009c
#define AUD_REG0X27_FLT2_B0_POS (0) 
#define AUD_REG0X27_FLT2_B0_MASK (0xFFFF) 

#define AUD_REG0X27_FLT2_B1_POS (16) 
#define AUD_REG0X27_FLT2_B1_MASK (0xFFFF) 

static inline uint32_t aud_ll_get_flt2_coef_b0b1_value(void)
{
    return REG_READ(AUD_REG0X27_ADDR);
}

static inline void aud_ll_set_flt2_coef_b0b1_value(uint32_t value)
{
    REG_WRITE(AUD_REG0X27_ADDR,value);
}

/* REG_0x27:reg0x27->flt2_B0:0x27[15:0],Filter 2 的IIR系数B0,0x0,R/W*/
static inline uint32_t aud_ll_get_flt2_coef_b0b1_flt2_b0(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X27_ADDR);
    reg_value = ((reg_value >> AUD_REG0X27_FLT2_B0_POS) & AUD_REG0X27_FLT2_B0_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt2_coef_b0b1_flt2_b0(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X27_ADDR);
    reg_value &= ~(AUD_REG0X27_FLT2_B0_MASK << AUD_REG0X27_FLT2_B0_POS);
    reg_value |= ((value & AUD_REG0X27_FLT2_B0_MASK) << AUD_REG0X27_FLT2_B0_POS);
    REG_WRITE(AUD_REG0X27_ADDR,reg_value);
}

/* REG_0x27:reg0x27->flt2_B1:0x27[31:16],Filter 2 的IIR系数B1,0x0,R/W*/
static inline uint32_t aud_ll_get_flt2_coef_b0b1_flt2_b1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X27_ADDR);
    reg_value = ((reg_value >> AUD_REG0X27_FLT2_B1_POS) & AUD_REG0X27_FLT2_B1_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt2_coef_b0b1_flt2_b1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X27_ADDR);
    reg_value &= ~(AUD_REG0X27_FLT2_B1_MASK << AUD_REG0X27_FLT2_B1_POS);
    reg_value |= ((value & AUD_REG0X27_FLT2_B1_MASK) << AUD_REG0X27_FLT2_B1_POS);
    REG_WRITE(AUD_REG0X27_ADDR,reg_value);
}

/* REG_0x28 */
#define AUD_REG0X28_ADDR  (AUD_LL_REG_BASE  + 0x28*4) //REG ADDR :0x478000a0
#define AUD_REG0X28_FLT2_B2_POS (0) 
#define AUD_REG0X28_FLT2_B2_MASK (0xFFFF) 

#define AUD_REG0X28_RESERVED0_POS (16) 
#define AUD_REG0X28_RESERVED0_MASK (0xFFFF) 

static inline uint32_t aud_ll_get_flt2_coef_b2_value(void)
{
    return REG_READ(AUD_REG0X28_ADDR);
}

static inline void aud_ll_set_flt2_coef_b2_value(uint32_t value)
{
    REG_WRITE(AUD_REG0X28_ADDR,value);
}

/* REG_0x28:reg0x28->flt2_B2:0x28[15:0],Filter 2 的IIR系数B2,0x0,R/W*/
static inline uint32_t aud_ll_get_flt2_coef_b2_flt2_b2(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X28_ADDR);
    reg_value = ((reg_value >> AUD_REG0X28_FLT2_B2_POS) & AUD_REG0X28_FLT2_B2_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt2_coef_b2_flt2_b2(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X28_ADDR);
    reg_value &= ~(AUD_REG0X28_FLT2_B2_MASK << AUD_REG0X28_FLT2_B2_POS);
    reg_value |= ((value & AUD_REG0X28_FLT2_B2_MASK) << AUD_REG0X28_FLT2_B2_POS);
    REG_WRITE(AUD_REG0X28_ADDR,reg_value);
}

/* REG_0x29 */
#define AUD_FLT3_COEF_ADDR  (AUD_LL_REG_BASE  + 0x29*4) //REG ADDR :0x478000a4
#define AUD_FLT3_COEF_FLT3_A1_POS (0) 
#define AUD_FLT3_COEF_FLT3_A1_MASK (0xFFFF) 

#define AUD_FLT3_COEF_FLT3_A2_POS (16) 
#define AUD_FLT3_COEF_FLT3_A2_MASK (0xFFFF) 

static inline uint32_t aud_ll_get_flt3_coef_a1a2_value(void)
{
    return REG_READ(AUD_FLT3_COEF_ADDR);
}

static inline void aud_ll_set_flt3_coef_a1a2_value(uint32_t value)
{
    REG_WRITE(AUD_FLT3_COEF_ADDR,value);
}

/* REG_0x29:flt3_coef->flt3_A1:0x29[15:0],Filter 3 的IIR系数A1,0x0,R/W*/
static inline uint32_t aud_ll_get_flt3_coef_a1a2_flt3_a1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT3_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT3_COEF_FLT3_A1_POS) & AUD_FLT3_COEF_FLT3_A1_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt3_coef_a1a2_flt3_a1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT3_COEF_ADDR);
    reg_value &= ~(AUD_FLT3_COEF_FLT3_A1_MASK << AUD_FLT3_COEF_FLT3_A1_POS);
    reg_value |= ((value & AUD_FLT3_COEF_FLT3_A1_MASK) << AUD_FLT3_COEF_FLT3_A1_POS);
    REG_WRITE(AUD_FLT3_COEF_ADDR,reg_value);
}

/* REG_0x29:flt3_coef->flt3_A2:0x29[31:16],Filter 3 的IIR系数A2,0x0,R/W*/
static inline uint32_t aud_ll_get_flt3_coef_a1a2_flt3_a2(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT3_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT3_COEF_FLT3_A2_POS) & AUD_FLT3_COEF_FLT3_A2_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt3_coef_a1a2_flt3_a2(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT3_COEF_ADDR);
    reg_value &= ~(AUD_FLT3_COEF_FLT3_A2_MASK << AUD_FLT3_COEF_FLT3_A2_POS);
    reg_value |= ((value & AUD_FLT3_COEF_FLT3_A2_MASK) << AUD_FLT3_COEF_FLT3_A2_POS);
    REG_WRITE(AUD_FLT3_COEF_ADDR,reg_value);
}

/* REG_0x2A */
#define AUD_REG0X2A_ADDR  (AUD_LL_REG_BASE  + 0x2A*4) //REG ADDR :0x478000a8
#define AUD_REG0X2A_FLT3_B0_POS (0) 
#define AUD_REG0X2A_FLT3_B0_MASK (0xFFFF) 

#define AUD_REG0X2A_FLT3_B1_POS (16) 
#define AUD_REG0X2A_FLT3_B1_MASK (0xFFFF) 

static inline uint32_t aud_ll_get_flt3_coef_b0b1_value(void)
{
    return REG_READ(AUD_REG0X2A_ADDR);
}

static inline void aud_ll_set_flt3_coef_b0b1_value(uint32_t value)
{
    REG_WRITE(AUD_REG0X2A_ADDR,value);
}

/* REG_0x2a:reg0x2a->flt3_B0:0x2a[15:0],Filter 3 的IIR系数B0,0x0,R/W*/
static inline uint32_t aud_ll_get_flt3_coef_b0b1_flt3_b0(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X2A_ADDR);
    reg_value = ((reg_value >> AUD_REG0X2A_FLT3_B0_POS) & AUD_REG0X2A_FLT3_B0_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt3_coef_b0b1_flt3_b0(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X2A_ADDR);
    reg_value &= ~(AUD_REG0X2A_FLT3_B0_MASK << AUD_REG0X2A_FLT3_B0_POS);
    reg_value |= ((value & AUD_REG0X2A_FLT3_B0_MASK) << AUD_REG0X2A_FLT3_B0_POS);
    REG_WRITE(AUD_REG0X2A_ADDR,reg_value);
}

/* REG_0x2a:reg0x2a->flt3_B1:0x2a[31:16],Filter 3 的IIR系数B1,0x0,R/W*/
static inline uint32_t aud_ll_get_flt3_coef_b0b1_flt3_b1(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X2A_ADDR);
    reg_value = ((reg_value >> AUD_REG0X2A_FLT3_B1_POS) & AUD_REG0X2A_FLT3_B1_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt3_coef_b0b1_flt3_b1(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X2A_ADDR);
    reg_value &= ~(AUD_REG0X2A_FLT3_B1_MASK << AUD_REG0X2A_FLT3_B1_POS);
    reg_value |= ((value & AUD_REG0X2A_FLT3_B1_MASK) << AUD_REG0X2A_FLT3_B1_POS);
    REG_WRITE(AUD_REG0X2A_ADDR,reg_value);
}

/* REG_0x2B */
#define AUD_REG0X2B_ADDR  (AUD_LL_REG_BASE  + 0x2B*4) //REG ADDR :0x478000ac
#define AUD_REG0X2B_FLT3_B2_POS (0) 
#define AUD_REG0X2B_FLT3_B2_MASK (0xFFFF) 

#define AUD_REG0X2B_RESERVED0_POS (16) 
#define AUD_REG0X2B_RESERVED0_MASK (0xFFFF) 

static inline uint32_t aud_ll_get_flt3_coef_b2_value(void)
{
    return REG_READ(AUD_REG0X2B_ADDR);
}

static inline void aud_ll_set_flt3_coef_b2_value(uint32_t value)
{
    REG_WRITE(AUD_REG0X2B_ADDR,value);
}

/* REG_0x2b:reg0x2b->flt3_B2:0x2b[15:0],Filter 3 的IIR系数B2,0x0,R/W*/
static inline uint32_t aud_ll_get_flt3_coef_b2_flt3_b2(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X2B_ADDR);
    reg_value = ((reg_value >> AUD_REG0X2B_FLT3_B2_POS) & AUD_REG0X2B_FLT3_B2_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt3_coef_b2_flt3_b2(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_REG0X2B_ADDR);
    reg_value &= ~(AUD_REG0X2B_FLT3_B2_MASK << AUD_REG0X2B_FLT3_B2_POS);
    reg_value |= ((value & AUD_REG0X2B_FLT3_B2_MASK) << AUD_REG0X2B_FLT3_B2_POS);
    REG_WRITE(AUD_REG0X2B_ADDR,reg_value);
}

/* REG_0x2C */
#define AUD_FLT0_EXT_COEF_ADDR  (AUD_LL_REG_BASE  + 0x2C*4) //REG ADDR :0x478000b0
#define AUD_FLT0_EXT_COEF_FLT0_A1_L6BIT_POS (0) 
#define AUD_FLT0_EXT_COEF_FLT0_A1_L6BIT_MASK (0x3F) 

#define AUD_FLT0_EXT_COEF_FLT0_A2_L6BIT_POS (6) 
#define AUD_FLT0_EXT_COEF_FLT0_A2_L6BIT_MASK (0x3F) 

#define AUD_FLT0_EXT_COEF_FLT0_B0_L6BIT_POS (12) 
#define AUD_FLT0_EXT_COEF_FLT0_B0_L6BIT_MASK (0x3F) 

#define AUD_FLT0_EXT_COEF_FLT0_B1_L6BIT_POS (18) 
#define AUD_FLT0_EXT_COEF_FLT0_B1_L6BIT_MASK (0x3F) 

#define AUD_FLT0_EXT_COEF_FLT0_B2_L6BIT_POS (24) 
#define AUD_FLT0_EXT_COEF_FLT0_B2_L6BIT_MASK (0x3F) 

#define AUD_FLT0_EXT_COEF_RESERVED0_POS (30) 
#define AUD_FLT0_EXT_COEF_RESERVED0_MASK (0x3) 

static inline uint32_t aud_ll_get_flt0_ext_coef_value(void)
{
    return REG_READ(AUD_FLT0_EXT_COEF_ADDR);
}

static inline void aud_ll_set_flt0_ext_coef_value(uint32_t value)
{
    REG_WRITE(AUD_FLT0_EXT_COEF_ADDR,value);
}

/* REG_0x2c:flt0_ext_coef->flt0_A1_L6BIT:0x2c[5:0],Filter 0 的IIR系数A1低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_flt0_ext_coef_flt0_a1_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT0_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT0_EXT_COEF_FLT0_A1_L6BIT_POS) & AUD_FLT0_EXT_COEF_FLT0_A1_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt0_ext_coef_flt0_a1_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT0_EXT_COEF_ADDR);
    reg_value &= ~(AUD_FLT0_EXT_COEF_FLT0_A1_L6BIT_MASK << AUD_FLT0_EXT_COEF_FLT0_A1_L6BIT_POS);
    reg_value |= ((value & AUD_FLT0_EXT_COEF_FLT0_A1_L6BIT_MASK) << AUD_FLT0_EXT_COEF_FLT0_A1_L6BIT_POS);
    REG_WRITE(AUD_FLT0_EXT_COEF_ADDR,reg_value);
}

/* REG_0x2c:flt0_ext_coef->flt0_A2_L6BIT:0x2c[11:6],Filter 0 的IIR系数A2低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_flt0_ext_coef_flt0_a2_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT0_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT0_EXT_COEF_FLT0_A2_L6BIT_POS) & AUD_FLT0_EXT_COEF_FLT0_A2_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt0_ext_coef_flt0_a2_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT0_EXT_COEF_ADDR);
    reg_value &= ~(AUD_FLT0_EXT_COEF_FLT0_A2_L6BIT_MASK << AUD_FLT0_EXT_COEF_FLT0_A2_L6BIT_POS);
    reg_value |= ((value & AUD_FLT0_EXT_COEF_FLT0_A2_L6BIT_MASK) << AUD_FLT0_EXT_COEF_FLT0_A2_L6BIT_POS);
    REG_WRITE(AUD_FLT0_EXT_COEF_ADDR,reg_value);
}

/* REG_0x2c:flt0_ext_coef->flt0_B0_L6BIT:0x2c[17:12],Filter 0 的IIR系数B0低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_flt0_ext_coef_flt0_b0_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT0_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT0_EXT_COEF_FLT0_B0_L6BIT_POS) & AUD_FLT0_EXT_COEF_FLT0_B0_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt0_ext_coef_flt0_b0_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT0_EXT_COEF_ADDR);
    reg_value &= ~(AUD_FLT0_EXT_COEF_FLT0_B0_L6BIT_MASK << AUD_FLT0_EXT_COEF_FLT0_B0_L6BIT_POS);
    reg_value |= ((value & AUD_FLT0_EXT_COEF_FLT0_B0_L6BIT_MASK) << AUD_FLT0_EXT_COEF_FLT0_B0_L6BIT_POS);
    REG_WRITE(AUD_FLT0_EXT_COEF_ADDR,reg_value);
}

/* REG_0x2c:flt0_ext_coef->flt0_B1_L6BIT:0x2c[23:18],Filter 0 的IIR系数B1低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_flt0_ext_coef_flt0_b1_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT0_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT0_EXT_COEF_FLT0_B1_L6BIT_POS) & AUD_FLT0_EXT_COEF_FLT0_B1_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt0_ext_coef_flt0_b1_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT0_EXT_COEF_ADDR);
    reg_value &= ~(AUD_FLT0_EXT_COEF_FLT0_B1_L6BIT_MASK << AUD_FLT0_EXT_COEF_FLT0_B1_L6BIT_POS);
    reg_value |= ((value & AUD_FLT0_EXT_COEF_FLT0_B1_L6BIT_MASK) << AUD_FLT0_EXT_COEF_FLT0_B1_L6BIT_POS);
    REG_WRITE(AUD_FLT0_EXT_COEF_ADDR,reg_value);
}

/* REG_0x2c:flt0_ext_coef->flt0_B2_L6BIT:0x2c[29:24],Filter 0 的IIR系数B2低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_flt0_ext_coef_flt0_b2_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT0_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT0_EXT_COEF_FLT0_B2_L6BIT_POS) & AUD_FLT0_EXT_COEF_FLT0_B2_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt0_ext_coef_flt0_b2_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT0_EXT_COEF_ADDR);
    reg_value &= ~(AUD_FLT0_EXT_COEF_FLT0_B2_L6BIT_MASK << AUD_FLT0_EXT_COEF_FLT0_B2_L6BIT_POS);
    reg_value |= ((value & AUD_FLT0_EXT_COEF_FLT0_B2_L6BIT_MASK) << AUD_FLT0_EXT_COEF_FLT0_B2_L6BIT_POS);
    REG_WRITE(AUD_FLT0_EXT_COEF_ADDR,reg_value);
}

/* REG_0x2D */
#define AUD_FLT1_EXT_COEF_ADDR  (AUD_LL_REG_BASE  + 0x2D*4) //REG ADDR :0x478000b4
#define AUD_FLT1_EXT_COEF_FLT1_A1_L6BIT_POS (0) 
#define AUD_FLT1_EXT_COEF_FLT1_A1_L6BIT_MASK (0x3F) 

#define AUD_FLT1_EXT_COEF_FLT1_A2_L6BIT_POS (6) 
#define AUD_FLT1_EXT_COEF_FLT1_A2_L6BIT_MASK (0x3F) 

#define AUD_FLT1_EXT_COEF_FLT1_B0_L6BIT_POS (12) 
#define AUD_FLT1_EXT_COEF_FLT1_B0_L6BIT_MASK (0x3F) 

#define AUD_FLT1_EXT_COEF_FLT1_B1_L6BIT_POS (18) 
#define AUD_FLT1_EXT_COEF_FLT1_B1_L6BIT_MASK (0x3F) 

#define AUD_FLT1_EXT_COEF_FLT1_B2_L6BIT_POS (24) 
#define AUD_FLT1_EXT_COEF_FLT1_B2_L6BIT_MASK (0x3F) 

#define AUD_FLT1_EXT_COEF_RESERVED0_POS (30) 
#define AUD_FLT1_EXT_COEF_RESERVED0_MASK (0x3) 

static inline uint32_t aud_ll_get_flt1_ext_coef_value(void)
{
    return REG_READ(AUD_FLT1_EXT_COEF_ADDR);
}

static inline void aud_ll_set_flt1_ext_coef_value(uint32_t value)
{
    REG_WRITE(AUD_FLT1_EXT_COEF_ADDR,value);
}

/* REG_0x2d:flt1_ext_coef->flt1_A1_L6BIT:0x2d[5:0],Filter 1 的IIR系数A1低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_flt1_ext_coef_flt1_a1_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT1_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT1_EXT_COEF_FLT1_A1_L6BIT_POS) & AUD_FLT1_EXT_COEF_FLT1_A1_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt1_ext_coef_flt1_a1_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT1_EXT_COEF_ADDR);
    reg_value &= ~(AUD_FLT1_EXT_COEF_FLT1_A1_L6BIT_MASK << AUD_FLT1_EXT_COEF_FLT1_A1_L6BIT_POS);
    reg_value |= ((value & AUD_FLT1_EXT_COEF_FLT1_A1_L6BIT_MASK) << AUD_FLT1_EXT_COEF_FLT1_A1_L6BIT_POS);
    REG_WRITE(AUD_FLT1_EXT_COEF_ADDR,reg_value);
}

/* REG_0x2d:flt1_ext_coef->flt1_A2_L6BIT:0x2d[11:6],Filter 1 的IIR系数A2低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_flt1_ext_coef_flt1_a2_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT1_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT1_EXT_COEF_FLT1_A2_L6BIT_POS) & AUD_FLT1_EXT_COEF_FLT1_A2_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt1_ext_coef_flt1_a2_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT1_EXT_COEF_ADDR);
    reg_value &= ~(AUD_FLT1_EXT_COEF_FLT1_A2_L6BIT_MASK << AUD_FLT1_EXT_COEF_FLT1_A2_L6BIT_POS);
    reg_value |= ((value & AUD_FLT1_EXT_COEF_FLT1_A2_L6BIT_MASK) << AUD_FLT1_EXT_COEF_FLT1_A2_L6BIT_POS);
    REG_WRITE(AUD_FLT1_EXT_COEF_ADDR,reg_value);
}

/* REG_0x2d:flt1_ext_coef->flt1_B0_L6BIT:0x2d[17:12],Filter 1 的IIR系数B0低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_flt1_ext_coef_flt1_b0_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT1_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT1_EXT_COEF_FLT1_B0_L6BIT_POS) & AUD_FLT1_EXT_COEF_FLT1_B0_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt1_ext_coef_flt1_b0_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT1_EXT_COEF_ADDR);
    reg_value &= ~(AUD_FLT1_EXT_COEF_FLT1_B0_L6BIT_MASK << AUD_FLT1_EXT_COEF_FLT1_B0_L6BIT_POS);
    reg_value |= ((value & AUD_FLT1_EXT_COEF_FLT1_B0_L6BIT_MASK) << AUD_FLT1_EXT_COEF_FLT1_B0_L6BIT_POS);
    REG_WRITE(AUD_FLT1_EXT_COEF_ADDR,reg_value);
}

/* REG_0x2d:flt1_ext_coef->flt1_B1_L6BIT:0x2d[23:18],Filter 1 的IIR系数B1低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_flt1_ext_coef_flt1_b1_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT1_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT1_EXT_COEF_FLT1_B1_L6BIT_POS) & AUD_FLT1_EXT_COEF_FLT1_B1_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt1_ext_coef_flt1_b1_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT1_EXT_COEF_ADDR);
    reg_value &= ~(AUD_FLT1_EXT_COEF_FLT1_B1_L6BIT_MASK << AUD_FLT1_EXT_COEF_FLT1_B1_L6BIT_POS);
    reg_value |= ((value & AUD_FLT1_EXT_COEF_FLT1_B1_L6BIT_MASK) << AUD_FLT1_EXT_COEF_FLT1_B1_L6BIT_POS);
    REG_WRITE(AUD_FLT1_EXT_COEF_ADDR,reg_value);
}

/* REG_0x2d:flt1_ext_coef->flt1_B2_L6BIT:0x2d[29:24],Filter 1 的IIR系数B2低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_flt1_ext_coef_flt1_b2_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT1_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT1_EXT_COEF_FLT1_B2_L6BIT_POS) & AUD_FLT1_EXT_COEF_FLT1_B2_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt1_ext_coef_flt1_b2_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT1_EXT_COEF_ADDR);
    reg_value &= ~(AUD_FLT1_EXT_COEF_FLT1_B2_L6BIT_MASK << AUD_FLT1_EXT_COEF_FLT1_B2_L6BIT_POS);
    reg_value |= ((value & AUD_FLT1_EXT_COEF_FLT1_B2_L6BIT_MASK) << AUD_FLT1_EXT_COEF_FLT1_B2_L6BIT_POS);
    REG_WRITE(AUD_FLT1_EXT_COEF_ADDR,reg_value);
}

/* REG_0x2E */
#define AUD_FLT2_EXT_COEF_ADDR  (AUD_LL_REG_BASE  + 0x2E*4) //REG ADDR :0x478000b8
#define AUD_FLT2_EXT_COEF_FLT2_A1_L6BIT_POS (0) 
#define AUD_FLT2_EXT_COEF_FLT2_A1_L6BIT_MASK (0x3F) 

#define AUD_FLT2_EXT_COEF_FLT2_A2_L6BIT_POS (6) 
#define AUD_FLT2_EXT_COEF_FLT2_A2_L6BIT_MASK (0x3F) 

#define AUD_FLT2_EXT_COEF_FLT2_B0_L6BIT_POS (12) 
#define AUD_FLT2_EXT_COEF_FLT2_B0_L6BIT_MASK (0x3F) 

#define AUD_FLT2_EXT_COEF_FLT2_B1_L6BIT_POS (18) 
#define AUD_FLT2_EXT_COEF_FLT2_B1_L6BIT_MASK (0x3F) 

#define AUD_FLT2_EXT_COEF_FLT2_B2_L6BIT_POS (24) 
#define AUD_FLT2_EXT_COEF_FLT2_B2_L6BIT_MASK (0x3F) 

#define AUD_FLT2_EXT_COEF_RESERVED0_POS (30) 
#define AUD_FLT2_EXT_COEF_RESERVED0_MASK (0x3) 

static inline uint32_t aud_ll_get_flt2_ext_coef_value(void)
{
    return REG_READ(AUD_FLT2_EXT_COEF_ADDR);
}

static inline void aud_ll_set_flt2_ext_coef_value(uint32_t value)
{
    REG_WRITE(AUD_FLT2_EXT_COEF_ADDR,value);
}

/* REG_0x2e:flt2_ext_coef->flt2_A1_L6BIT:0x2e[5:0],Filter 2 的IIR系数A1低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_flt2_ext_coef_flt2_a1_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT2_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT2_EXT_COEF_FLT2_A1_L6BIT_POS) & AUD_FLT2_EXT_COEF_FLT2_A1_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt2_ext_coef_flt2_a1_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT2_EXT_COEF_ADDR);
    reg_value &= ~(AUD_FLT2_EXT_COEF_FLT2_A1_L6BIT_MASK << AUD_FLT2_EXT_COEF_FLT2_A1_L6BIT_POS);
    reg_value |= ((value & AUD_FLT2_EXT_COEF_FLT2_A1_L6BIT_MASK) << AUD_FLT2_EXT_COEF_FLT2_A1_L6BIT_POS);
    REG_WRITE(AUD_FLT2_EXT_COEF_ADDR,reg_value);
}

/* REG_0x2e:flt2_ext_coef->flt2_A2_L6BIT:0x2e[11:6],Filter 2 的IIR系数A2低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_flt2_ext_coef_flt2_a2_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT2_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT2_EXT_COEF_FLT2_A2_L6BIT_POS) & AUD_FLT2_EXT_COEF_FLT2_A2_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt2_ext_coef_flt2_a2_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT2_EXT_COEF_ADDR);
    reg_value &= ~(AUD_FLT2_EXT_COEF_FLT2_A2_L6BIT_MASK << AUD_FLT2_EXT_COEF_FLT2_A2_L6BIT_POS);
    reg_value |= ((value & AUD_FLT2_EXT_COEF_FLT2_A2_L6BIT_MASK) << AUD_FLT2_EXT_COEF_FLT2_A2_L6BIT_POS);
    REG_WRITE(AUD_FLT2_EXT_COEF_ADDR,reg_value);
}

/* REG_0x2e:flt2_ext_coef->flt2_B0_L6BIT:0x2e[17:12],Filter 2 的IIR系数B0低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_flt2_ext_coef_flt2_b0_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT2_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT2_EXT_COEF_FLT2_B0_L6BIT_POS) & AUD_FLT2_EXT_COEF_FLT2_B0_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt2_ext_coef_flt2_b0_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT2_EXT_COEF_ADDR);
    reg_value &= ~(AUD_FLT2_EXT_COEF_FLT2_B0_L6BIT_MASK << AUD_FLT2_EXT_COEF_FLT2_B0_L6BIT_POS);
    reg_value |= ((value & AUD_FLT2_EXT_COEF_FLT2_B0_L6BIT_MASK) << AUD_FLT2_EXT_COEF_FLT2_B0_L6BIT_POS);
    REG_WRITE(AUD_FLT2_EXT_COEF_ADDR,reg_value);
}

/* REG_0x2e:flt2_ext_coef->flt2_B1_L6BIT:0x2e[23:18],Filter 2 的IIR系数B1低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_flt2_ext_coef_flt2_b1_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT2_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT2_EXT_COEF_FLT2_B1_L6BIT_POS) & AUD_FLT2_EXT_COEF_FLT2_B1_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt2_ext_coef_flt2_b1_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT2_EXT_COEF_ADDR);
    reg_value &= ~(AUD_FLT2_EXT_COEF_FLT2_B1_L6BIT_MASK << AUD_FLT2_EXT_COEF_FLT2_B1_L6BIT_POS);
    reg_value |= ((value & AUD_FLT2_EXT_COEF_FLT2_B1_L6BIT_MASK) << AUD_FLT2_EXT_COEF_FLT2_B1_L6BIT_POS);
    REG_WRITE(AUD_FLT2_EXT_COEF_ADDR,reg_value);
}

/* REG_0x2e:flt2_ext_coef->flt2_B2_L6BIT:0x2e[29:24],Filter 2 的IIR系数B2低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_flt2_ext_coef_flt2_b2_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT2_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT2_EXT_COEF_FLT2_B2_L6BIT_POS) & AUD_FLT2_EXT_COEF_FLT2_B2_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt2_ext_coef_flt2_b2_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT2_EXT_COEF_ADDR);
    reg_value &= ~(AUD_FLT2_EXT_COEF_FLT2_B2_L6BIT_MASK << AUD_FLT2_EXT_COEF_FLT2_B2_L6BIT_POS);
    reg_value |= ((value & AUD_FLT2_EXT_COEF_FLT2_B2_L6BIT_MASK) << AUD_FLT2_EXT_COEF_FLT2_B2_L6BIT_POS);
    REG_WRITE(AUD_FLT2_EXT_COEF_ADDR,reg_value);
}

/* REG_0x2F */
#define AUD_FLT3_EXT_COEF_ADDR  (AUD_LL_REG_BASE  + 0x2F*4) //REG ADDR :0x478000bc
#define AUD_FLT3_EXT_COEF_FLT3_A1_L6BIT_POS (0) 
#define AUD_FLT3_EXT_COEF_FLT3_A1_L6BIT_MASK (0x3F) 

#define AUD_FLT3_EXT_COEF_FLT3_A2_L6BIT_POS (6) 
#define AUD_FLT3_EXT_COEF_FLT3_A2_L6BIT_MASK (0x3F) 

#define AUD_FLT3_EXT_COEF_FLT3_B0_L6BIT_POS (12) 
#define AUD_FLT3_EXT_COEF_FLT3_B0_L6BIT_MASK (0x3F) 

#define AUD_FLT3_EXT_COEF_FLT3_B1_L6BIT_POS (18) 
#define AUD_FLT3_EXT_COEF_FLT3_B1_L6BIT_MASK (0x3F) 

#define AUD_FLT3_EXT_COEF_FLT3_B2_L6BIT_POS (24) 
#define AUD_FLT3_EXT_COEF_FLT3_B2_L6BIT_MASK (0x3F) 

#define AUD_FLT3_EXT_COEF_RESERVED0_POS (30) 
#define AUD_FLT3_EXT_COEF_RESERVED0_MASK (0x3) 

static inline uint32_t aud_ll_get_flt3_ext_coef_value(void)
{
    return REG_READ(AUD_FLT3_EXT_COEF_ADDR);
}

static inline void aud_ll_set_flt3_ext_coef_value(uint32_t value)
{
    REG_WRITE(AUD_FLT3_EXT_COEF_ADDR,value);
}

/* REG_0x2f:flt3_ext_coef->flt3_A1_L6BIT:0x2f[5:0],Filter 3 的IIR系数A1低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_flt3_ext_coef_flt3_a1_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT3_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT3_EXT_COEF_FLT3_A1_L6BIT_POS) & AUD_FLT3_EXT_COEF_FLT3_A1_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt3_ext_coef_flt3_a1_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT3_EXT_COEF_ADDR);
    reg_value &= ~(AUD_FLT3_EXT_COEF_FLT3_A1_L6BIT_MASK << AUD_FLT3_EXT_COEF_FLT3_A1_L6BIT_POS);
    reg_value |= ((value & AUD_FLT3_EXT_COEF_FLT3_A1_L6BIT_MASK) << AUD_FLT3_EXT_COEF_FLT3_A1_L6BIT_POS);
    REG_WRITE(AUD_FLT3_EXT_COEF_ADDR,reg_value);
}

/* REG_0x2f:flt3_ext_coef->flt3_A2_L6BIT:0x2f[11:6],Filter 3 的IIR系数A2低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_flt3_ext_coef_flt3_a2_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT3_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT3_EXT_COEF_FLT3_A2_L6BIT_POS) & AUD_FLT3_EXT_COEF_FLT3_A2_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt3_ext_coef_flt3_a2_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT3_EXT_COEF_ADDR);
    reg_value &= ~(AUD_FLT3_EXT_COEF_FLT3_A2_L6BIT_MASK << AUD_FLT3_EXT_COEF_FLT3_A2_L6BIT_POS);
    reg_value |= ((value & AUD_FLT3_EXT_COEF_FLT3_A2_L6BIT_MASK) << AUD_FLT3_EXT_COEF_FLT3_A2_L6BIT_POS);
    REG_WRITE(AUD_FLT3_EXT_COEF_ADDR,reg_value);
}

/* REG_0x2f:flt3_ext_coef->flt3_B0_L6BIT:0x2f[17:12],Filter 3 的IIR系数B0低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_flt3_ext_coef_flt3_b0_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT3_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT3_EXT_COEF_FLT3_B0_L6BIT_POS) & AUD_FLT3_EXT_COEF_FLT3_B0_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt3_ext_coef_flt3_b0_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT3_EXT_COEF_ADDR);
    reg_value &= ~(AUD_FLT3_EXT_COEF_FLT3_B0_L6BIT_MASK << AUD_FLT3_EXT_COEF_FLT3_B0_L6BIT_POS);
    reg_value |= ((value & AUD_FLT3_EXT_COEF_FLT3_B0_L6BIT_MASK) << AUD_FLT3_EXT_COEF_FLT3_B0_L6BIT_POS);
    REG_WRITE(AUD_FLT3_EXT_COEF_ADDR,reg_value);
}

/* REG_0x2f:flt3_ext_coef->flt3_B1_L6BIT:0x2f[23:18],Filter 3 的IIR系数B1低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_flt3_ext_coef_flt3_b1_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT3_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT3_EXT_COEF_FLT3_B1_L6BIT_POS) & AUD_FLT3_EXT_COEF_FLT3_B1_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt3_ext_coef_flt3_b1_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT3_EXT_COEF_ADDR);
    reg_value &= ~(AUD_FLT3_EXT_COEF_FLT3_B1_L6BIT_MASK << AUD_FLT3_EXT_COEF_FLT3_B1_L6BIT_POS);
    reg_value |= ((value & AUD_FLT3_EXT_COEF_FLT3_B1_L6BIT_MASK) << AUD_FLT3_EXT_COEF_FLT3_B1_L6BIT_POS);
    REG_WRITE(AUD_FLT3_EXT_COEF_ADDR,reg_value);
}

/* REG_0x2f:flt3_ext_coef->flt3_B2_L6BIT:0x2f[29:24],Filter 3 的IIR系数B2低6比特,0x0,R/W*/
static inline uint32_t aud_ll_get_flt3_ext_coef_flt3_b2_l6bit(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT3_EXT_COEF_ADDR);
    reg_value = ((reg_value >> AUD_FLT3_EXT_COEF_FLT3_B2_L6BIT_POS) & AUD_FLT3_EXT_COEF_FLT3_B2_L6BIT_MASK);
    return reg_value;
}

static inline void aud_ll_set_flt3_ext_coef_flt3_b2_l6bit(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_FLT3_EXT_COEF_ADDR);
    reg_value &= ~(AUD_FLT3_EXT_COEF_FLT3_B2_L6BIT_MASK << AUD_FLT3_EXT_COEF_FLT3_B2_L6BIT_POS);
    reg_value |= ((value & AUD_FLT3_EXT_COEF_FLT3_B2_L6BIT_MASK) << AUD_FLT3_EXT_COEF_FLT3_B2_L6BIT_POS);
    REG_WRITE(AUD_FLT3_EXT_COEF_ADDR,reg_value);
}

/* REG_0x30 */
#define AUD_AUDIO_CONFIG_ADDR  (AUD_LL_REG_BASE  + 0x30*4) //REG ADDR :0x478000c0
#define AUD_AUDIO_CONFIG_SAMP_RATE_ADC_POS (0) 
#define AUD_AUDIO_CONFIG_SAMP_RATE_ADC_MASK (0x3) 

#define AUD_AUDIO_CONFIG_DAC_ENABLE_POS (2) 
#define AUD_AUDIO_CONFIG_DAC_ENABLE_MASK (0x1) 

#define AUD_AUDIO_CONFIG_ADC_ENABLE_POS (3) 
#define AUD_AUDIO_CONFIG_ADC_ENABLE_MASK (0x1) 

#define AUD_AUDIO_CONFIG_DTMF_ENABLE_POS (4) 
#define AUD_AUDIO_CONFIG_DTMF_ENABLE_MASK (0x1) 

#define AUD_AUDIO_CONFIG_LINE_ENABLE_POS (5) 
#define AUD_AUDIO_CONFIG_LINE_ENABLE_MASK (0x1) 

#define AUD_AUDIO_CONFIG_SAMP_RATE_DAC_POS (6) 
#define AUD_AUDIO_CONFIG_SAMP_RATE_DAC_MASK (0x3) 

#define AUD_AUDIO_CONFIG_APLL_SEL_POS (8) 
#define AUD_AUDIO_CONFIG_APLL_SEL_MASK (0x1) 

#define AUD_AUDIO_CONFIG_DMIC_ENABLE_POS (9) 
#define AUD_AUDIO_CONFIG_DMIC_ENABLE_MASK (0x1) 

#define AUD_AUDIO_CONFIG_DMIC_CIC_SEL_POS (10) 
#define AUD_AUDIO_CONFIG_DMIC_CIC_SEL_MASK (0x3) 

#define AUD_AUDIO_CONFIG_DIG_MIC_DIV_POS (12) 
#define AUD_AUDIO_CONFIG_DIG_MIC_DIV_MASK (0x1F) 

#define AUD_AUDIO_CONFIG_RESERVED0_POS (17) 
#define AUD_AUDIO_CONFIG_RESERVED0_MASK (0x7FFF) 

static inline uint32_t aud_ll_get_audio_config_value(void)
{
    return REG_READ(AUD_AUDIO_CONFIG_ADDR);
}

static inline void aud_ll_set_audio_config_value(uint32_t value)
{
    REG_WRITE(AUD_AUDIO_CONFIG_ADDR,value);
}

/* REG_0x30:audio_config->samp_rate_ADC:0x30[1:0],ADC采样率配置 0：8K  1：16K  2：44.1K  3：48K,0x0,R/W*/
static inline uint32_t aud_ll_get_audio_config_samp_rate_adc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AUDIO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_AUDIO_CONFIG_SAMP_RATE_ADC_POS) & AUD_AUDIO_CONFIG_SAMP_RATE_ADC_MASK);
    return reg_value;
}

static inline void aud_ll_set_audio_config_samp_rate_adc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AUDIO_CONFIG_ADDR);
    reg_value &= ~(AUD_AUDIO_CONFIG_SAMP_RATE_ADC_MASK << AUD_AUDIO_CONFIG_SAMP_RATE_ADC_POS);
    reg_value |= ((value & AUD_AUDIO_CONFIG_SAMP_RATE_ADC_MASK) << AUD_AUDIO_CONFIG_SAMP_RATE_ADC_POS);
    REG_WRITE(AUD_AUDIO_CONFIG_ADDR,reg_value);
}

/* REG_0x30:audio_config->dac_enable:0x30[2],DAC使能  1：使能,0x0,R/W*/
static inline uint32_t aud_ll_get_audio_config_dac_enable(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AUDIO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_AUDIO_CONFIG_DAC_ENABLE_POS) & AUD_AUDIO_CONFIG_DAC_ENABLE_MASK);
    return reg_value;
}

static inline void aud_ll_set_audio_config_dac_enable(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AUDIO_CONFIG_ADDR);
    reg_value &= ~(AUD_AUDIO_CONFIG_DAC_ENABLE_MASK << AUD_AUDIO_CONFIG_DAC_ENABLE_POS);
    reg_value |= ((value & AUD_AUDIO_CONFIG_DAC_ENABLE_MASK) << AUD_AUDIO_CONFIG_DAC_ENABLE_POS);
    REG_WRITE(AUD_AUDIO_CONFIG_ADDR,reg_value);
}

/* REG_0x30:audio_config->adc_enable:0x30[3],ADC使能  1：使能,0x0,R/W*/
static inline uint32_t aud_ll_get_audio_config_adc_enable(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AUDIO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_AUDIO_CONFIG_ADC_ENABLE_POS) & AUD_AUDIO_CONFIG_ADC_ENABLE_MASK);
    return reg_value;
}

static inline void aud_ll_set_audio_config_adc_enable(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AUDIO_CONFIG_ADDR);
    reg_value &= ~(AUD_AUDIO_CONFIG_ADC_ENABLE_MASK << AUD_AUDIO_CONFIG_ADC_ENABLE_POS);
    reg_value |= ((value & AUD_AUDIO_CONFIG_ADC_ENABLE_MASK) << AUD_AUDIO_CONFIG_ADC_ENABLE_POS);
    REG_WRITE(AUD_AUDIO_CONFIG_ADDR,reg_value);
}

/* REG_0x30:audio_config->dtmf_enable:0x30[4],DTMF使能 1: 使能,0x0,R/W*/
static inline uint32_t aud_ll_get_audio_config_dtmf_enable(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AUDIO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_AUDIO_CONFIG_DTMF_ENABLE_POS) & AUD_AUDIO_CONFIG_DTMF_ENABLE_MASK);
    return reg_value;
}

static inline void aud_ll_set_audio_config_dtmf_enable(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AUDIO_CONFIG_ADDR);
    reg_value &= ~(AUD_AUDIO_CONFIG_DTMF_ENABLE_MASK << AUD_AUDIO_CONFIG_DTMF_ENABLE_POS);
    reg_value |= ((value & AUD_AUDIO_CONFIG_DTMF_ENABLE_MASK) << AUD_AUDIO_CONFIG_DTMF_ENABLE_POS);
    REG_WRITE(AUD_AUDIO_CONFIG_ADDR,reg_value);
}

/* REG_0x30:audio_config->line_enable:0x30[5],LINE IN使能  1：使能,0x0,R/W*/
static inline uint32_t aud_ll_get_audio_config_line_enable(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AUDIO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_AUDIO_CONFIG_LINE_ENABLE_POS) & AUD_AUDIO_CONFIG_LINE_ENABLE_MASK);
    return reg_value;
}

static inline void aud_ll_set_audio_config_line_enable(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AUDIO_CONFIG_ADDR);
    reg_value &= ~(AUD_AUDIO_CONFIG_LINE_ENABLE_MASK << AUD_AUDIO_CONFIG_LINE_ENABLE_POS);
    reg_value |= ((value & AUD_AUDIO_CONFIG_LINE_ENABLE_MASK) << AUD_AUDIO_CONFIG_LINE_ENABLE_POS);
    REG_WRITE(AUD_AUDIO_CONFIG_ADDR,reg_value);
}

/* REG_0x30:audio_config->samp_rate_DAC:0x30[7:6],DAC采样率配置 0：8K  1：16K  2：44.1K  3：48K,0x0,R/W*/
static inline uint32_t aud_ll_get_audio_config_samp_rate_dac(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AUDIO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_AUDIO_CONFIG_SAMP_RATE_DAC_POS) & AUD_AUDIO_CONFIG_SAMP_RATE_DAC_MASK);
    return reg_value;
}

static inline void aud_ll_set_audio_config_samp_rate_dac(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AUDIO_CONFIG_ADDR);
    reg_value &= ~(AUD_AUDIO_CONFIG_SAMP_RATE_DAC_MASK << AUD_AUDIO_CONFIG_SAMP_RATE_DAC_POS);
    reg_value |= ((value & AUD_AUDIO_CONFIG_SAMP_RATE_DAC_MASK) << AUD_AUDIO_CONFIG_SAMP_RATE_DAC_POS);
    REG_WRITE(AUD_AUDIO_CONFIG_ADDR,reg_value);
}

/* REG_0x30:audio_config->apll_sel:0x30[8],1: audio clk = 24.576MHz(APLL) 0: audio_clk = 26MHz(crystal),0x0,R/W*/
static inline uint32_t aud_ll_get_audio_config_apll_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AUDIO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_AUDIO_CONFIG_APLL_SEL_POS) & AUD_AUDIO_CONFIG_APLL_SEL_MASK);
    return reg_value;
}

static inline void aud_ll_set_audio_config_apll_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AUDIO_CONFIG_ADDR);
    reg_value &= ~(AUD_AUDIO_CONFIG_APLL_SEL_MASK << AUD_AUDIO_CONFIG_APLL_SEL_POS);
    reg_value |= ((value & AUD_AUDIO_CONFIG_APLL_SEL_MASK) << AUD_AUDIO_CONFIG_APLL_SEL_POS);
    REG_WRITE(AUD_AUDIO_CONFIG_ADDR,reg_value);
}

/* REG_0x30:audio_config->dmic_enable:0x30[9],digital mic path enable,0x0,R/W*/
static inline uint32_t aud_ll_get_audio_config_dmic_enable(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AUDIO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_AUDIO_CONFIG_DMIC_ENABLE_POS) & AUD_AUDIO_CONFIG_DMIC_ENABLE_MASK);
    return reg_value;
}

static inline void aud_ll_set_audio_config_dmic_enable(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AUDIO_CONFIG_ADDR);
    reg_value &= ~(AUD_AUDIO_CONFIG_DMIC_ENABLE_MASK << AUD_AUDIO_CONFIG_DMIC_ENABLE_POS);
    reg_value |= ((value & AUD_AUDIO_CONFIG_DMIC_ENABLE_MASK) << AUD_AUDIO_CONFIG_DMIC_ENABLE_POS);
    REG_WRITE(AUD_AUDIO_CONFIG_ADDR,reg_value);
}

/* REG_0x30:audio_config->dmic_cic_sel:0x30[11:10],digital mic down sample, 0: 16   1: 32: 2,3: 64,0x0,R/W*/
static inline uint32_t aud_ll_get_audio_config_dmic_cic_sel(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AUDIO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_AUDIO_CONFIG_DMIC_CIC_SEL_POS) & AUD_AUDIO_CONFIG_DMIC_CIC_SEL_MASK);
    return reg_value;
}

static inline void aud_ll_set_audio_config_dmic_cic_sel(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AUDIO_CONFIG_ADDR);
    reg_value &= ~(AUD_AUDIO_CONFIG_DMIC_CIC_SEL_MASK << AUD_AUDIO_CONFIG_DMIC_CIC_SEL_POS);
    reg_value |= ((value & AUD_AUDIO_CONFIG_DMIC_CIC_SEL_MASK) << AUD_AUDIO_CONFIG_DMIC_CIC_SEL_POS);
    REG_WRITE(AUD_AUDIO_CONFIG_ADDR,reg_value);
}

/* REG_0x30:audio_config->dig_mic_div:0x30[16:12],digital mic clk = apll/(dig_mic_div+1),0x0,R/W*/
static inline uint32_t aud_ll_get_audio_config_dig_mic_div(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AUDIO_CONFIG_ADDR);
    reg_value = ((reg_value >> AUD_AUDIO_CONFIG_DIG_MIC_DIV_POS) & AUD_AUDIO_CONFIG_DIG_MIC_DIV_MASK);
    return reg_value;
}

static inline void aud_ll_set_audio_config_dig_mic_div(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_AUDIO_CONFIG_ADDR);
    reg_value &= ~(AUD_AUDIO_CONFIG_DIG_MIC_DIV_MASK << AUD_AUDIO_CONFIG_DIG_MIC_DIV_POS);
    reg_value |= ((value & AUD_AUDIO_CONFIG_DIG_MIC_DIV_MASK) << AUD_AUDIO_CONFIG_DIG_MIC_DIV_POS);
    REG_WRITE(AUD_AUDIO_CONFIG_ADDR,reg_value);
}

/* REG_0x31 */
#define AUD_DTMF_CONFIG0_ADDR  (AUD_LL_REG_BASE  + 0x31*4) //REG ADDR :0x478000c4
#define AUD_DTMF_CONFIG0_TONE_PATTERN_POS (0) 
#define AUD_DTMF_CONFIG0_TONE_PATTERN_MASK (0x1) 

#define AUD_DTMF_CONFIG0_TONE_MODE_POS (1) 
#define AUD_DTMF_CONFIG0_TONE_MODE_MASK (0x1) 

#define AUD_DTMF_CONFIG0_TONE_PAUSE_TIME_POS (2) 
#define AUD_DTMF_CONFIG0_TONE_PAUSE_TIME_MASK (0xF) 

#define AUD_DTMF_CONFIG0_TONE_ACTIVE_TIME_POS (6) 
#define AUD_DTMF_CONFIG0_TONE_ACTIVE_TIME_MASK (0xF) 

#define AUD_DTMF_CONFIG0_RESERVED0_POS (10) 
#define AUD_DTMF_CONFIG0_RESERVED0_MASK (0x3FFFFF) 

static inline uint32_t aud_ll_get_dtmf_config0_value(void)
{
    return REG_READ(AUD_DTMF_CONFIG0_ADDR);
}

static inline void aud_ll_set_dtmf_config0_value(uint32_t value)
{
    REG_WRITE(AUD_DTMF_CONFIG0_ADDR,value);
}

/* REG_0x31:dtmf_config0->tone_pattern:0x31[0],1：Tone1(Active_Time) + Tone2(Pause_Time)              0: DTMF(Active_Time) + Zeros(Pause_Time),0x0,R/W*/
static inline uint32_t aud_ll_get_dtmf_config0_tone_pattern(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DTMF_CONFIG0_ADDR);
    reg_value = ((reg_value >> AUD_DTMF_CONFIG0_TONE_PATTERN_POS) & AUD_DTMF_CONFIG0_TONE_PATTERN_MASK);
    return reg_value;
}

static inline void aud_ll_set_dtmf_config0_tone_pattern(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DTMF_CONFIG0_ADDR);
    reg_value &= ~(AUD_DTMF_CONFIG0_TONE_PATTERN_MASK << AUD_DTMF_CONFIG0_TONE_PATTERN_POS);
    reg_value |= ((value & AUD_DTMF_CONFIG0_TONE_PATTERN_MASK) << AUD_DTMF_CONFIG0_TONE_PATTERN_POS);
    REG_WRITE(AUD_DTMF_CONFIG0_ADDR,reg_value);
}

/* REG_0x31:dtmf_config0->tone_mode:0x31[1],1：连续模式， 0：单次模式,0x0,R/W*/
static inline uint32_t aud_ll_get_dtmf_config0_tone_mode(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DTMF_CONFIG0_ADDR);
    reg_value = ((reg_value >> AUD_DTMF_CONFIG0_TONE_MODE_POS) & AUD_DTMF_CONFIG0_TONE_MODE_MASK);
    return reg_value;
}

static inline void aud_ll_set_dtmf_config0_tone_mode(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DTMF_CONFIG0_ADDR);
    reg_value &= ~(AUD_DTMF_CONFIG0_TONE_MODE_MASK << AUD_DTMF_CONFIG0_TONE_MODE_POS);
    reg_value |= ((value & AUD_DTMF_CONFIG0_TONE_MODE_MASK) << AUD_DTMF_CONFIG0_TONE_MODE_POS);
    REG_WRITE(AUD_DTMF_CONFIG0_ADDR,reg_value);
}

/* REG_0x31:dtmf_config0->tone_pause_time:0x31[5:2],DTMF的间歇时间  N*10 ms,0x0,R/W*/
static inline uint32_t aud_ll_get_dtmf_config0_tone_pause_time(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DTMF_CONFIG0_ADDR);
    reg_value = ((reg_value >> AUD_DTMF_CONFIG0_TONE_PAUSE_TIME_POS) & AUD_DTMF_CONFIG0_TONE_PAUSE_TIME_MASK);
    return reg_value;
}

static inline void aud_ll_set_dtmf_config0_tone_pause_time(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DTMF_CONFIG0_ADDR);
    reg_value &= ~(AUD_DTMF_CONFIG0_TONE_PAUSE_TIME_MASK << AUD_DTMF_CONFIG0_TONE_PAUSE_TIME_POS);
    reg_value |= ((value & AUD_DTMF_CONFIG0_TONE_PAUSE_TIME_MASK) << AUD_DTMF_CONFIG0_TONE_PAUSE_TIME_POS);
    REG_WRITE(AUD_DTMF_CONFIG0_ADDR,reg_value);
}

/* REG_0x31:dtmf_config0->tone_active_time:0x31[9:6],DTMF的工作时间  N*10 ms,0x0,R/W*/
static inline uint32_t aud_ll_get_dtmf_config0_tone_active_time(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DTMF_CONFIG0_ADDR);
    reg_value = ((reg_value >> AUD_DTMF_CONFIG0_TONE_ACTIVE_TIME_POS) & AUD_DTMF_CONFIG0_TONE_ACTIVE_TIME_MASK);
    return reg_value;
}

static inline void aud_ll_set_dtmf_config0_tone_active_time(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DTMF_CONFIG0_ADDR);
    reg_value &= ~(AUD_DTMF_CONFIG0_TONE_ACTIVE_TIME_MASK << AUD_DTMF_CONFIG0_TONE_ACTIVE_TIME_POS);
    reg_value |= ((value & AUD_DTMF_CONFIG0_TONE_ACTIVE_TIME_MASK) << AUD_DTMF_CONFIG0_TONE_ACTIVE_TIME_POS);
    REG_WRITE(AUD_DTMF_CONFIG0_ADDR,reg_value);
}

/* REG_0x32 */
#define AUD_DTMF_CONFIG1_ADDR  (AUD_LL_REG_BASE  + 0x32*4) //REG ADDR :0x478000c8
#define AUD_DTMF_CONFIG1_TONE1_STEP_POS (0) 
#define AUD_DTMF_CONFIG1_TONE1_STEP_MASK (0xFFFF) 

#define AUD_DTMF_CONFIG1_TONE1_ATTU_POS (16) 
#define AUD_DTMF_CONFIG1_TONE1_ATTU_MASK (0xF) 

#define AUD_DTMF_CONFIG1_TONE1_ENABLE_POS (20) 
#define AUD_DTMF_CONFIG1_TONE1_ENABLE_MASK (0x1) 

#define AUD_DTMF_CONFIG1_RESERVED0_POS (21) 
#define AUD_DTMF_CONFIG1_RESERVED0_MASK (0x7FF) 

static inline uint32_t aud_ll_get_dtmf_config1_value(void)
{
    return REG_READ(AUD_DTMF_CONFIG1_ADDR);
}

static inline void aud_ll_set_dtmf_config1_value(uint32_t value)
{
    REG_WRITE(AUD_DTMF_CONFIG1_ADDR,value);
}

/* REG_0x32:dtmf_config1->tone1_step:0x32[15:0],Tone1的频率设置  Step = F(KHz) * 8191 ,0x0,R/W*/
static inline uint32_t aud_ll_get_dtmf_config1_tone1_step(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DTMF_CONFIG1_ADDR);
    reg_value = ((reg_value >> AUD_DTMF_CONFIG1_TONE1_STEP_POS) & AUD_DTMF_CONFIG1_TONE1_STEP_MASK);
    return reg_value;
}

static inline void aud_ll_set_dtmf_config1_tone1_step(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DTMF_CONFIG1_ADDR);
    reg_value &= ~(AUD_DTMF_CONFIG1_TONE1_STEP_MASK << AUD_DTMF_CONFIG1_TONE1_STEP_POS);
    reg_value |= ((value & AUD_DTMF_CONFIG1_TONE1_STEP_MASK) << AUD_DTMF_CONFIG1_TONE1_STEP_POS);
    REG_WRITE(AUD_DTMF_CONFIG1_ADDR,reg_value);
}

/* REG_0x32:dtmf_config1->tone1_attu:0x32[19:16],Tone1的衰减设置  0 ：-1 ：-15 dB ,0x0,R/W*/
static inline uint32_t aud_ll_get_dtmf_config1_tone1_attu(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DTMF_CONFIG1_ADDR);
    reg_value = ((reg_value >> AUD_DTMF_CONFIG1_TONE1_ATTU_POS) & AUD_DTMF_CONFIG1_TONE1_ATTU_MASK);
    return reg_value;
}

static inline void aud_ll_set_dtmf_config1_tone1_attu(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DTMF_CONFIG1_ADDR);
    reg_value &= ~(AUD_DTMF_CONFIG1_TONE1_ATTU_MASK << AUD_DTMF_CONFIG1_TONE1_ATTU_POS);
    reg_value |= ((value & AUD_DTMF_CONFIG1_TONE1_ATTU_MASK) << AUD_DTMF_CONFIG1_TONE1_ATTU_POS);
    REG_WRITE(AUD_DTMF_CONFIG1_ADDR,reg_value);
}

/* REG_0x32:dtmf_config1->tone1_enable:0x32[20],Tone1的使能设置,0x0,R/W*/
static inline uint32_t aud_ll_get_dtmf_config1_tone1_enable(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DTMF_CONFIG1_ADDR);
    reg_value = ((reg_value >> AUD_DTMF_CONFIG1_TONE1_ENABLE_POS) & AUD_DTMF_CONFIG1_TONE1_ENABLE_MASK);
    return reg_value;
}

static inline void aud_ll_set_dtmf_config1_tone1_enable(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DTMF_CONFIG1_ADDR);
    reg_value &= ~(AUD_DTMF_CONFIG1_TONE1_ENABLE_MASK << AUD_DTMF_CONFIG1_TONE1_ENABLE_POS);
    reg_value |= ((value & AUD_DTMF_CONFIG1_TONE1_ENABLE_MASK) << AUD_DTMF_CONFIG1_TONE1_ENABLE_POS);
    REG_WRITE(AUD_DTMF_CONFIG1_ADDR,reg_value);
}

/* REG_0x33 */
#define AUD_DTMF_CONFIG2_ADDR  (AUD_LL_REG_BASE  + 0x33*4) //REG ADDR :0x478000cc
#define AUD_DTMF_CONFIG2_TONE2_STEP_POS (0) 
#define AUD_DTMF_CONFIG2_TONE2_STEP_MASK (0xFFFF) 

#define AUD_DTMF_CONFIG2_TONE2_ATTU_POS (16) 
#define AUD_DTMF_CONFIG2_TONE2_ATTU_MASK (0xF) 

#define AUD_DTMF_CONFIG2_TONE2_ENABLE_POS (20) 
#define AUD_DTMF_CONFIG2_TONE2_ENABLE_MASK (0x1) 

#define AUD_DTMF_CONFIG2_RESERVED0_POS (21) 
#define AUD_DTMF_CONFIG2_RESERVED0_MASK (0x7FF) 

static inline uint32_t aud_ll_get_dtmf_config2_value(void)
{
    return REG_READ(AUD_DTMF_CONFIG2_ADDR);
}

static inline void aud_ll_set_dtmf_config2_value(uint32_t value)
{
    REG_WRITE(AUD_DTMF_CONFIG2_ADDR,value);
}

/* REG_0x33:dtmf_config2->tone2_step:0x33[15:0],Tone2的频率设置  Step = F(KHz) * 8191 ,0x0,R/W*/
static inline uint32_t aud_ll_get_dtmf_config2_tone2_step(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DTMF_CONFIG2_ADDR);
    reg_value = ((reg_value >> AUD_DTMF_CONFIG2_TONE2_STEP_POS) & AUD_DTMF_CONFIG2_TONE2_STEP_MASK);
    return reg_value;
}

static inline void aud_ll_set_dtmf_config2_tone2_step(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DTMF_CONFIG2_ADDR);
    reg_value &= ~(AUD_DTMF_CONFIG2_TONE2_STEP_MASK << AUD_DTMF_CONFIG2_TONE2_STEP_POS);
    reg_value |= ((value & AUD_DTMF_CONFIG2_TONE2_STEP_MASK) << AUD_DTMF_CONFIG2_TONE2_STEP_POS);
    REG_WRITE(AUD_DTMF_CONFIG2_ADDR,reg_value);
}

/* REG_0x33:dtmf_config2->tone2_attu:0x33[19:16],Tone2的衰减设置  0 ：-1 ：-15 dB ,0x0,R/W*/
static inline uint32_t aud_ll_get_dtmf_config2_tone2_attu(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DTMF_CONFIG2_ADDR);
    reg_value = ((reg_value >> AUD_DTMF_CONFIG2_TONE2_ATTU_POS) & AUD_DTMF_CONFIG2_TONE2_ATTU_MASK);
    return reg_value;
}

static inline void aud_ll_set_dtmf_config2_tone2_attu(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DTMF_CONFIG2_ADDR);
    reg_value &= ~(AUD_DTMF_CONFIG2_TONE2_ATTU_MASK << AUD_DTMF_CONFIG2_TONE2_ATTU_POS);
    reg_value |= ((value & AUD_DTMF_CONFIG2_TONE2_ATTU_MASK) << AUD_DTMF_CONFIG2_TONE2_ATTU_POS);
    REG_WRITE(AUD_DTMF_CONFIG2_ADDR,reg_value);
}

/* REG_0x33:dtmf_config2->tone2_enable:0x33[20],Tone2的使能设置,0x0,R/W*/
static inline uint32_t aud_ll_get_dtmf_config2_tone2_enable(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DTMF_CONFIG2_ADDR);
    reg_value = ((reg_value >> AUD_DTMF_CONFIG2_TONE2_ENABLE_POS) & AUD_DTMF_CONFIG2_TONE2_ENABLE_MASK);
    return reg_value;
}

static inline void aud_ll_set_dtmf_config2_tone2_enable(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AUD_DTMF_CONFIG2_ADDR);
    reg_value &= ~(AUD_DTMF_CONFIG2_TONE2_ENABLE_MASK << AUD_DTMF_CONFIG2_TONE2_ENABLE_POS);
    reg_value |= ((value & AUD_DTMF_CONFIG2_TONE2_ENABLE_MASK) << AUD_DTMF_CONFIG2_TONE2_ENABLE_POS);
    REG_WRITE(AUD_DTMF_CONFIG2_ADDR,reg_value);
}

#if 0
uint32_t aud_ll_get_adc_fifo_addr(void)
{
	return AUD_ADC_FPORT_ADDR;
}

uint32_t aud_ll_get_dtmf_fifo_addr(void)
{
	return AUD_DTMF_FPORT_ADDR;
}

uint32_t aud_ll_get_dmic_fifo_addr(void)
{
	return AUD_DMIC_FPORT_ADDR;
}

uint32_t aud_ll_get_dac_fifo_addr(void)
{
	return AUD_DAC_FPORT_ADDR;
}
#endif

#ifdef __cplusplus 
}                  
#endif             
