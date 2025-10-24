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

#pragma once

#include "hal_config.h"
#include <soc/soc.h>
#include "aud_ll_macro_def.h"

#include <driver/aud_adc_types.h>
#include <driver/aud_dtmf_types.h>
#include <driver/aud_dmic_types.h>
#include <driver/aud_dac_types.h>

#ifdef __cplusplus
extern "C" {
#endif


/* device_id */
#define aud_hal_get_device_id_value()                                   aud_ll_get_device_id_value()

/* version_id */
#define aud_hal_get_version_id_value()                                   aud_ll_get_version_id_value()

/* soft_reset */
#define aud_hal_get_clk_control_soft_reset()                                   aud_ll_get_clk_control_soft_reset()
#define aud_hal_set_clk_control_soft_reset(value)                            aud_ll_set_clk_control_soft_reset(value)

/* soft_reset */
#define aud_hal_get_clk_control_clk_gate()                                   aud_ll_get_clk_control_clk_gate()
#define aud_hal_set_clk_control_clk_gate(value)                            aud_ll_set_clk_control_clk_gate(value)

/* global_status */
#define aud_hal_get_global_status_value()                                   aud_ll_get_global_status_value()


/* samp_rate_ADC:ADC sample rate 0：8K  1：16K  2：44.1K  3：48K,RW,0x0[1:0]*/
#define aud_hal_get_audio_config_samp_rate_adc()                           aud_ll_get_audio_config_samp_rate_adc()
#define aud_hal_set_audio_config_samp_rate_adc(value)                    aud_ll_set_audio_config_samp_rate_adc(value)

/* dac_enable:DAC enable  1: enable  0: disable,RW,0x0[2]*/
#define aud_hal_get_audio_config_dac_enable()                              aud_ll_get_audio_config_dac_enable()
#define aud_hal_set_audio_config_dac_enable(value)                       aud_ll_set_audio_config_dac_enable(value);

/* adc_enable:ADC enable  1: enable  0: disable,RW,0x0[3]*/
#define aud_hal_get_audio_config_adc_enable()                              aud_ll_get_audio_config_adc_enable()
#define aud_hal_set_audio_config_adc_enable(value)                       aud_ll_set_audio_config_adc_enable(value);

/* dtmf_enable:DTMF enable 1: enable  0: disable,RW,0x0[4]*/
#define aud_hal_get_audio_config_dtmf_enable()                             aud_ll_get_audio_config_dtmf_enable()
#define aud_hal_set_audio_config_dtmf_enable(value)                      aud_ll_set_audio_config_dtmf_enable(value)

/* line_enable:LINE IN enable  1：enable  0:disable,RW,0x0[5]*/
#define aud_hal_get_audio_config_line_enable()                             aud_ll_get_audio_config_line_enable()
#define aud_hal_set_audio_config_line_enable(value)                      aud_ll_set_audio_config_line_enable(value)

/* samp_rate_DAC:DAC sample rate  0：8K  1：16K  2：44.1K  3：48K,RW,0x0[7:6]*/
#define aud_hal_get_audio_config_samp_rate_dac()                           aud_ll_get_audio_config_samp_rate_dac()
#define aud_hal_set_audio_config_samp_rate_dac(value)                    aud_ll_set_audio_config_samp_rate_dac(value)

/* REG_0x01 */
//#define aud_hal_get_dtmf_config0_value()                                   aud_ll_get_dtmf_config0_value()
//#define aud_hal_set_dtmf_config0_value(value)                            aud_ll_set_dtmf_config0_value(value)

/* REG_0x01:dtmf_config0->tone_pattern:1：Tone1(Active_Time) + Tone2(Pause_Time)              0: DTMF(Active_Time) + Zeros(Pause_Time),RW,0x1[0]*/
#define aud_hal_get_dtmf_config0_tone_pattern()                            aud_ll_get_dtmf_config0_tone_pattern()
#define aud_hal_set_dtmf_config0_tone_pattern(value)                     aud_ll_set_dtmf_config0_tone_pattern(value)

/* REG_0x01:dtmf_config0->tone_mode:1：continuous mode,  0：signal mode,RW,0x1[1]*/
#define aud_hal_get_dtmf_config0_tone_mode()                               aud_ll_get_dtmf_config0_tone_mode()
#define aud_hal_set_dtmf_config0_tone_mode(value)                        aud_ll_set_dtmf_config0_tone_mode(value)

/* REG_0x01:dtmf_config0->tone_pause_time:DTMF pause time  N*10 ms,RW,0x1[5:2]*/
#define aud_hal_get_dtmf_config0_tone_pause_time()                         aud_ll_get_dtmf_config0_tone_pause_time()
#define aud_hal_set_dtmf_config0_tone_pause_time(value)                  aud_ll_set_dtmf_config0_tone_pause_time(value)

/* REG_0x01:dtmf_config0->tone_active_time:DTMF active time  N*10 ms,RW,0x1[9:6]*/
#define aud_hal_get_dtmf_config0_tone_active_time()                        aud_ll_get_dtmf_config0_tone_active_time()
#define aud_hal_set_dtmf_config0_tone_active_time(value)                 aud_ll_set_dtmf_config0_tone_active_time(value)

/* REG_0x02 */
//#define aud_hal_get_dtmf_config1_value()                                   aud_ll_get_dtmf_config1_value()
//#define aud_hal_set_dtmf_config1_value(value)                            aud_ll_set_dtmf_config1_value(value)

/* REG_0x02:dtmf_config1->tone1_step:Tone1 frequency set  Step = F(KHz) * 8191 ,RW,0x2[15:0]*/
#define aud_hal_get_dtmf_config1_tone1_step()                              aud_ll_get_dtmf_config1_tone1_step()
#define aud_hal_set_dtmf_config1_tone1_step(value)                       aud_ll_set_dtmf_config1_tone1_step(value)

/* REG_0x02:dtmf_config1->tone1_attu:Tone1 damp set  0 ：-1 ：-15 dB ,RW,0x2[19:16]*/
#define aud_hal_get_dtmf_config1_tone1_attu()                              aud_ll_get_dtmf_config1_tone1_attu()
#define aud_hal_set_dtmf_config1_tone1_attu(value)                       aud_ll_set_dtmf_config1_tone1_attu(value)

/* REG_0x02:dtmf_config1->tone1_enable:Tone1 enable set,RW,0x2[20]*/
#define aud_hal_get_dtmf_config1_tone1_enable()                            aud_ll_get_dtmf_config1_tone1_enable()
#define aud_hal_set_dtmf_config1_tone1_enable(value)                     aud_ll_set_dtmf_config1_tone1_enable(value)

/* REG_0x03 */
//#define aud_hal_get_dtmf_config2_value()                                   aud_ll_get_dtmf_config2_value()
//#define aud_hal_set_dtmf_config2_value(value)                            aud_ll_set_dtmf_config2_value(value)

/* REG_0x03:dtmf_config2->tone2_step:Tone2 frequency set  Step = F(KHz) * 8191 ,RW,0x3[15:0]*/
#define aud_hal_get_dtmf_config2_tone2_step()                              aud_ll_get_dtmf_config2_tone2_step()
#define aud_hal_set_dtmf_config2_tone2_step(value)                       aud_ll_set_dtmf_config2_tone2_step(value)

/* REG_0x03:dtmf_config2->tone2_attu:Tone2 damp set  0 ：-1 ：-15 dB ,RW,0x3[19:16]*/
#define aud_hal_get_dtmf_config2_tone2_attu()                              aud_ll_get_dtmf_config2_tone2_attu()
#define aud_hal_set_dtmf_config2_tone2_attu(value)                       aud_ll_set_dtmf_config2_tone2_attu(value)

/* REG_0x03:dtmf_config2->tone2_enable:Tone2 enable set,RW,0x3[20]*/
#define aud_hal_get_dtmf_config2_tone2_enable()                            aud_ll_get_dtmf_config2_tone2_enable()
#define aud_hal_set_dtmf_config2_tone2_enable(value)                     aud_ll_set_dtmf_config2_tone2_enable(value)

/* REG_0x04 */
//#define aud_hal_get_adc_config0_value()                                    aud_ll_get_adc_config0_value()
//#define aud_hal_set_adc_config0_value(value)                             aud_ll_set_adc_config0_value(value)

/* REG_0x04:adc_config0->adc_hpf2_coef_B2:ADC HPF2 coefficient B2 ,RW,0x4[15:0]*/
#define aud_hal_get_adc_config0_adc_hpf2_coef_b2()                         aud_ll_get_adc_config0_adc_hpf2_coef_b2()
#define aud_hal_set_adc_config0_adc_hpf2_coef_b2(value)                  aud_ll_set_adc_config0_adc_hpf2_coef_b2(value)

/* REG_0x04:adc_config0->adc_hpf2_bypass:ADC HPF2 disable bit， 1：bypass HPF2,RW,0x4[16]*/
#define aud_hal_get_adc_config0_adc_hpf2_bypass()                          aud_ll_get_adc_config0_adc_hpf2_bypass()
#define aud_hal_set_adc_config0_adc_hpf2_bypass(value)                   aud_ll_set_adc_config0_adc_hpf2_bypass(value)

/* REG_0x04:adc_config0->adc_hpf1_bypass:ADC HPF1 disable bit， 1：bypass HPF1,RW,0x4[17]*/
#define aud_hal_get_adc_config0_adc_hpf1_bypass()                          aud_ll_get_adc_config0_adc_hpf1_bypass()
#define aud_hal_set_adc_config0_adc_hpf1_bypass(value)                   aud_ll_set_adc_config0_adc_hpf1_bypass(value)

/* REG_0x04:adc_config0->adc_set_gain:ADC gain set,RW,0x4[23:18]*/
#define aud_hal_get_adc_config0_adc_set_gain()                             aud_ll_get_adc_config0_adc_set_gain()
#define aud_hal_set_adc_config0_adc_set_gain(value)                      aud_ll_set_adc_config0_adc_set_gain(value)

/* REG_0x04:adc_config0->adc_sampe_edge:ADC data sampling clock edge select  1：falling edge，0：rising edge,RW,0x4[24]*/
#define aud_hal_get_adc_config0_adc_sampe_edge()                           aud_ll_get_adc_config0_adc_sampe_edge()
#define aud_hal_set_adc_config0_adc_sampe_edge(value)                    aud_ll_set_adc_config0_adc_sampe_edge(value)

/* REG_0x05 */
#define aud_hal_get_adc_config1_value()                                    aud_ll_get_adc_config1_value()
#define aud_hal_set_adc_config1_value(value)                             aud_ll_set_adc_config1_value(value)

/* REG_0x05:adc_config1->adc_hpf2_coef_B0:ADC HPF2 coefficient B2 ,RW,0x5[15: 0]*/
#define aud_hal_get_adc_config1_adc_hpf2_coef_b0()                         aud_ll_get_adc_config1_adc_hpf2_coef_b0()
#define aud_hal_set_adc_config1_adc_hpf2_coef_b0(value)                  aud_ll_set_adc_config1_adc_hpf2_coef_b0(value)

/* REG_0x05:adc_config1->adc_hpf2_coef_B1:ADC HPF2 coefficient B1 ,RW,0x5[31:16]*/
#define aud_hal_get_adc_config1_adc_hpf2_coef_b1()                         aud_ll_get_adc_config1_adc_hpf2_coef_b1()
#define aud_hal_set_adc_config1_adc_hpf2_coef_b1(value)                  aud_ll_set_adc_config1_adc_hpf2_coef_b1(value)

/* REG_0x06 */
#define aud_hal_get_adc_config2_value()                                    aud_ll_get_adc_config2_value()
#define aud_hal_set_adc_config2_value(value)                             aud_ll_set_adc_config2_value(value)

/* REG_0x06:adc_config2->adc_hpf2_coef_A0:ADC HPF2 coefficient A0 ,RW,0x6[15: 0]*/
#define aud_hal_get_adc_config2_adc_hpf2_coef_a0()                         aud_ll_get_adc_config2_adc_hpf2_coef_a0()
#define aud_hal_set_adc_config2_adc_hpf2_coef_a0(value)                  aud_ll_set_adc_config2_adc_hpf2_coef_a0(value)

/* REG_0x06:adc_config2->adc_hpf2_coef_A1:ADC HPF2 coefficient A1 ,RW,0x6[31:16]*/
#define aud_hal_get_adc_config2_adc_hpf2_coef_a1()                         aud_ll_get_adc_config2_adc_hpf2_coef_a1()
#define aud_hal_set_adc_config2_adc_hpf2_coef_a1(value)                  aud_ll_set_adc_config2_adc_hpf2_coef_a1(value)

/* REG_0x07 */
#define aud_hal_get_dac_config0_value()                                    aud_ll_get_dac_config0_value()
#define aud_hal_set_dac_config0_value(value)                             aud_ll_set_dac_config0_value(value)

/* REG_0x07:dac_config0->dac_hpf2_coef_B2:DAC HPF2 coefficient B2 ,RW,0x7[15:0]*/
#define aud_hal_get_dac_config0_dac_hpf2_coef_b2()                         aud_ll_get_dac_config0_dac_hpf2_coef_b2()
#define aud_hal_set_dac_config0_dac_hpf2_coef_b2(value)                  aud_ll_set_dac_config0_dac_hpf2_coef_b2(value)

/* REG_0x07:dac_config0->dac_hpf2_bypass:DAC HPF2 disable bit，1：bypass HPF2,RW,0x7[16]*/
#define aud_hal_get_dac_config0_dac_hpf2_bypass()                          aud_ll_get_dac_config0_dac_hpf2_bypass()
#define aud_hal_set_dac_config0_dac_hpf2_bypass(value)                   aud_ll_set_dac_config0_dac_hpf2_bypass(value)

/* REG_0x07:dac_config0->dac_hpf1_bypass:DAC HPF1 disable bit，1：bypass HPF1 ,RW,0x7[17]*/
#define aud_hal_get_dac_config0_dac_hpf1_bypass()                          aud_ll_get_dac_config0_dac_hpf1_bypass()
#define aud_hal_set_dac_config0_dac_hpf1_bypass(value)                   aud_ll_set_dac_config0_dac_hpf1_bypass(value)

/* REG_0x07:dac_config0->dac_set_gain:DAC gain set,RW,0x7[23:18]*/
#define aud_hal_get_dac_config0_dac_set_gain()                             aud_ll_get_dac_config0_dac_set_gain()
#define aud_hal_set_dac_config0_dac_set_gain(value)                      aud_ll_set_dac_config0_dac_set_gain(value)

/* REG_0x07:dac_config0->dac_clk_invert:DAC output clock edge select  0：falling edge， 1：rasing edge,RW,0x7[24]*/
#define aud_hal_get_dac_config0_dac_clk_invert()                           aud_ll_get_dac_config0_dac_clk_invert()
#define aud_hal_set_dac_config0_dac_clk_invert(value)                    aud_ll_set_dac_config0_dac_clk_invert(value)

/* REG_0x08 */
#define aud_hal_get_dac_config1_value()                                    aud_ll_get_dac_config1_value()
#define aud_hal_set_dac_config1_value(value)                             aud_ll_set_dac_config1_value(value)

/* REG_0x08:dac_config1->dac_hpf2_coef_B0:DAC HPF2 coefficient B0 ,RW,0x8[15: 0]*/
#define aud_hal_get_dac_config1_dac_hpf2_coef_b0()                         aud_ll_get_dac_config1_dac_hpf2_coef_b0()
#define aud_hal_set_dac_config1_dac_hpf2_coef_b0(value)                  aud_ll_set_dac_config1_dac_hpf2_coef_b0(value)

/* REG_0x08:dac_config1->dac_hpf2_coef_B1:DAC HPF2 coefficient B1 ,RW,0x8[31:16]*/
#define aud_hal_get_dac_config1_dac_hpf2_coef_b1()                         aud_ll_get_dac_config1_dac_hpf2_coef_b1()
#define aud_hal_set_dac_config1_dac_hpf2_coef_b1(value)                  aud_ll_set_dac_config1_dac_hpf2_coef_b1(value)

/* REG_0x09 */
#define aud_hal_get_dac_config2_value()                                    aud_ll_get_dac_config2_value()
#define aud_hal_set_dac_config2_value(value)                             aud_ll_set_dac_config2_value(value)

/* REG_0x09:dac_config2->dac_hpf2_coef_A1:DAC HPF2 coefficient A1 ,RW,0x9[15: 0]*/
#define aud_hal_get_dac_config2_dac_hpf2_coef_a1()                         aud_ll_get_dac_config2_dac_hpf2_coef_a1()
#define aud_hal_set_dac_config2_dac_hpf2_coef_a1(value)                  aud_ll_set_dac_config2_dac_hpf2_coef_a1(value)

/* REG_0x09:dac_config2->dac_hpf2_coef_A2:DAC HPF2 coefficient A2 ,RW,0x9[31:16]*/
#define aud_hal_get_dac_config2_dac_hpf2_coef_a2()                         aud_ll_get_dac_config2_dac_hpf2_coef_a2()
#define aud_hal_set_dac_config2_dac_hpf2_coef_a2(value)                  aud_ll_set_dac_config2_dac_hpf2_coef_a2(value)

/* REG_0x0a */
#define aud_hal_get_fifo_config_value()                                    aud_ll_get_fifo_config_value()
#define aud_hal_set_fifo_config_value(value)                             aud_ll_set_fifo_config_value(value)

/* REG_0x0a:fifo_config->dacr_rd_threshold:when DAC right channel read FIFO，if data number is lower than the threshold value, output interrupt request,RW,0xa[4:0]*/
#define aud_hal_get_fifo_config_dacr_rd_threshold()                        aud_ll_get_fifo_config_dacr_rd_threshold()
#define aud_hal_set_fifo_config_dacr_rd_threshold(value)                 aud_ll_set_fifo_config_dacr_rd_threshold(value)

/* REG_0x0a:fifo_config->dacl_rd_threshold:when DAC left channel read FIFO，if data number is lower than the threshold value, output interrupt request,RW,0xa[9:5]*/
#define aud_hal_get_fifo_config_dacl_rd_threshold()                        aud_ll_get_fifo_config_dacl_rd_threshold()
#define aud_hal_set_fifo_config_dacl_rd_threshold(value)                 aud_ll_set_fifo_config_dacl_rd_threshold(value)

/* REG_0x0a:fifo_config->dtmf_wr_threshold:when DTMF automatic write FIFO，if data number is higher than the threshold value, output interrupt request,RW,0xa[14:10]*/
#define aud_hal_get_fifo_config_dtmf_wr_threshold()                        aud_ll_get_fifo_config_dtmf_wr_threshold()
#define aud_hal_set_fifo_config_dtmf_wr_threshold(value)                 aud_ll_set_fifo_config_dtmf_wr_threshold(value)

/* REG_0x0a:fifo_config->adcl_wr_threshold:when ADC DTMF automatic write FIFO，if data number is higher than the threshold value, output interrupt request,RW,0xa[19:15]*/
#define aud_hal_get_fifo_config_adcl_wr_threshold()                        aud_ll_get_fifo_config_adcl_wr_threshold()
#define aud_hal_set_fifo_config_adcl_wr_threshold(value)                 aud_ll_set_fifo_config_adcl_wr_threshold(value)

/* REG_0x0a:fifo_config->dacr_int_en:DAC right channel interrupt request enable,RW,0xa[20]*/
#define aud_hal_get_fifo_config_dacr_int_en()                              aud_ll_get_fifo_config_dacr_int_en()
#define aud_hal_set_fifo_config_dacr_int_en(value)                       aud_ll_set_fifo_config_dacr_int_en(value)

/* REG_0x0a:fifo_config->dacl_int_en:DAC left channel interrupt request enable,RW,0xa[21]*/
#define aud_hal_get_fifo_config_dacl_int_en()                              aud_ll_get_fifo_config_dacl_int_en()
#define aud_hal_set_fifo_config_dacl_int_en(value)                       aud_ll_set_fifo_config_dacl_int_en(value)

/* REG_0x0a:fifo_config->dtmf_int_en:DTMF interrupt request enable,RW,0xa[22]*/
#define aud_hal_get_fifo_config_dtmf_int_en()                              aud_ll_get_fifo_config_dtmf_int_en()
#define aud_hal_set_fifo_config_dtmf_int_en(value)                       aud_ll_set_fifo_config_dtmf_int_en(value)

/* REG_0x0a:fifo_config->adcl_int_en:ADC interrupt request enable,RW,0xa[23]*/
#define aud_hal_get_fifo_config_adcl_int_en()                              aud_ll_get_fifo_config_adcl_int_en()
#define aud_hal_set_fifo_config_adcl_int_en(value)                       aud_ll_set_fifo_config_adcl_int_en(value)

/* REG_0x0a:fifo_config->loop_ton2dac:DTMF to DAC loop test enable,RW,0xa[24]*/
#define aud_hal_get_fifo_config_loop_ton2dac()                             aud_ll_get_fifo_config_loop_ton2dac()
#define aud_hal_set_fifo_config_loop_ton2dac(value)                      aud_ll_set_fifo_config_loop_ton2dac(value)

/* REG_0x0a:fifo_config->loop_adc2dac:ADC to DAC loop test enable,RW,0xa[25]*/
#define aud_hal_get_fifo_config_loop_adc2dac()                             aud_ll_get_fifo_config_loop_adc2dac()
#define aud_hal_set_fifo_config_loop_adc2dac(value)                      aud_ll_set_fifo_config_loop_adc2dac(value)

/* REG_0x0b */
#define aud_hal_get_agc_config0_value()                                    aud_ll_get_agc_config0_value()
#define aud_hal_set_agc_config0_value(value)                             aud_ll_set_agc_config0_value(value)

/* REG_0x0b:agc_config0->agc_noise_thrd:noise gating of excute，noise gating work when audio value is little,RW,0xb[9:0]*/
#define aud_hal_get_agc_config0_agc_noise_thrd()                           aud_ll_get_agc_config0_agc_noise_thrd()
#define aud_hal_set_agc_config0_agc_noise_thrd(value)                    aud_ll_set_agc_config0_agc_noise_thrd(value)

/* REG_0x0b:agc_config0->agc_noise_high:corresponding MIC_RSSI low 10 bit,RW,0xb[19:10]*/
#define aud_hal_get_agc_config0_agc_noise_high()                           aud_ll_get_agc_config0_agc_noise_high()
#define aud_hal_set_agc_config0_agc_noise_high(value)                    aud_ll_set_agc_config0_agc_noise_high(value)

/* REG_0x0b:agc_config0->agc_noise_low:corresponding MIC_RSSI low 10 bit，example if MIC_RSSI[15:10] any bit is '1'，signal level is than NOISE_LOW,RW,0xb[29:20]*/
#define aud_hal_get_agc_config0_agc_noise_low()                            aud_ll_get_agc_config0_agc_noise_low()
#define aud_hal_set_agc_config0_agc_noise_low(value)                     aud_ll_set_agc_config0_agc_noise_low(value)

/* REG_0x0b:agc_config0->agc_step:no used,RW,0xb[31:30]*/
#define aud_hal_get_agc_config0_agc_step()                                 aud_ll_get_agc_config0_agc_step()
#define aud_hal_set_agc_config0_agc_step(value)                          aud_ll_set_agc_config0_agc_step(value)

/* REG_0x0c */
#define aud_hal_get_agc_config1_value()                                    aud_ll_get_agc_config1_value()
#define aud_hal_set_agc_config1_value(value)                             aud_ll_set_agc_config1_value(value)

/* REG_0x0c:agc_config1->agc_noise_min:{GAIN2[2:0] GAIN1[3:0]} when signal level below NOISE_LOW, when noise gating is enabled,RW,0xc[6:0]*/
#define aud_hal_get_agc_config1_agc_noise_min()                            aud_ll_get_agc_config1_agc_noise_min()
#define aud_hal_set_agc_config1_agc_noise_min(value)                     aud_ll_set_agc_config1_agc_noise_min(value)

/* REG_0x0c:agc_config1->agc_noise_tout:1: 4 (~0.5 ms), 2: 8, 3: 16, 4: 32, 5: 64, 6: 128, 7: 256 (~32 ms),RW,0xc[9:7]*/
#define aud_hal_get_agc_config1_agc_noise_tout()                           aud_ll_get_agc_config1_agc_noise_tout()
#define aud_hal_set_agc_config1_agc_noise_tout(value)                    aud_ll_set_agc_config1_agc_noise_tout(value)

/* REG_0x0c:agc_config1->agc_high_dur:0: 0 (0 ms，as soon as MIC_RSSI value higher than NOISE_HIGH，Noise gating work，leave off noise status); 1: 8 (~1 ms), 2: 16, 3: 32, 4: 64, 5: 128, 6: 256, 7: 256 (~32 ms),RW,0xc[12:10]*/
#define aud_hal_get_agc_config1_agc_high_dur()                             aud_ll_get_agc_config1_agc_high_dur()
#define aud_hal_set_agc_config1_agc_high_dur(value)                      aud_ll_set_agc_config1_agc_high_dur(value)

/* REG_0x0c:agc_config1->agc_low_dur:0: 0 (0 ms，as soon as MIC_RSSI value lower than NOISE_LOW，Noise gating work); 1: 8 (~1 ms), 2: 16, 3: 32, 4: 64, 5: 128, 6: 256, 7: 256 (~32 ms),RW,0xc[15:13]*/
#define aud_hal_get_agc_config1_agc_low_dur()                              aud_ll_get_agc_config1_agc_low_dur()
#define aud_hal_set_agc_config1_agc_low_dur(value)                       aud_ll_set_agc_config1_agc_low_dur(value)

/* REG_0x0c:agc_config1->agc_min:Minimum value of {GAIN2[2:0] GAIN1[3:0]},RW,0xc[22:16]*/
#define aud_hal_get_agc_config1_agc_min()                                  aud_ll_get_agc_config1_agc_min()
#define aud_hal_set_agc_config1_agc_min(value)                           aud_ll_set_agc_config1_agc_min(value)

/* REG_0x0c:agc_config1->agc_max:Maximum value of {GAIN2[2:0] GAIN1[3:0]}; Also the  default gain setting when AGC is disabled,RW,0xc[29:23]*/
#define aud_hal_get_agc_config1_agc_max()                                  aud_ll_get_agc_config1_agc_max()
#define aud_hal_set_agc_config1_agc_max(value)                           aud_ll_set_agc_config1_agc_max(value)

/* REG_0x0c:agc_config1->agc_ng_method:0: mute or demute (default); 1: gradually increase or decrease gac,RW,0xc[30]*/
#define aud_hal_get_agc_config1_agc_ng_method()                            aud_ll_get_agc_config1_agc_ng_method()
#define aud_hal_set_agc_config1_agc_ng_method(value)                     aud_ll_set_agc_config1_agc_ng_method(value)

/* REG_0x0c:agc_config1->agc_ng_enable:enable noise gating,RW,0xc[31]*/
#define aud_hal_get_agc_config1_agc_ng_enable()                            aud_ll_get_agc_config1_agc_ng_enable()
#define aud_hal_set_agc_config1_agc_ng_enable(value)                     aud_ll_set_agc_config1_agc_ng_enable(value)

/* REG_0x0d */
#define aud_hal_get_agc_config2_value()                                    aud_ll_get_agc_config2_value()
#define aud_hal_set_agc_config2_value(value)                             aud_ll_set_agc_config2_value(value)

/* REG_0x0d:agc_config2->agc_decay_time:0: 128 (~2.7 ms), 1: 256, 2: 512, 3: 1024, 4:2048, 5: 4096, 6: 8192, 7: 16384 (~340 ms),RW,0xd[2:0]*/
#define aud_hal_get_agc_config2_agc_decay_time()                           aud_ll_get_agc_config2_agc_decay_time()
#define aud_hal_set_agc_config2_agc_decay_time(value)                    aud_ll_set_agc_config2_agc_decay_time(value)

/* REG_0x0d:agc_config2->agc_attack_time:0: 8 (~0.1667ms), 1: 16, 2: 32, 3: 64, 4: 128, 5: 256, 6: 512, 7: 1024 (~21 ms),RW,0xd[5:3]*/
#define aud_hal_get_agc_config2_agc_attack_time()                          aud_ll_get_agc_config2_agc_attack_time()
#define aud_hal_set_agc_config2_agc_attack_time(value)                   aud_ll_set_agc_config2_agc_attack_time(value)

/* REG_0x0d:agc_config2->agc_high_thrd:coefficient high five bit of MIC_RSSI,RW,0xd[10:6]*/
#define aud_hal_get_agc_config2_agc_high_thrd()                            aud_ll_get_agc_config2_agc_high_thrd()
#define aud_hal_set_agc_config2_agc_high_thrd(value)                     aud_ll_set_agc_config2_agc_high_thrd(value)

/* REG_0x0d:agc_config2->agc_low_thrd:coefficient low five bit of MIC_RSSI,RW,0xd[15:11]*/
#define aud_hal_get_agc_config2_agc_low_thrd()                             aud_ll_get_agc_config2_agc_low_thrd()
#define aud_hal_set_agc_config2_agc_low_thrd(value)                      aud_ll_set_agc_config2_agc_low_thrd(value)

/* REG_0x0d:agc_config2->agc_iir_coef:IIR系数选择 0:1/32 1:1/64 2:1/128 3:1/256 4:1/512 5:1/1024 6:1/2048 7:1/4096,RW,0xd[18:16]*/
#define aud_hal_get_agc_config2_agc_iir_coef()                             aud_ll_get_agc_config2_agc_iir_coef()
#define aud_hal_set_agc_config2_agc_iir_coef(value)                      aud_ll_set_agc_config2_agc_iir_coef(value)

/* REG_0x0d:agc_config2->agc_enable:AGC enable bit,RW,0xd[19]*/
#define aud_hal_get_agc_config2_agc_enable()                               aud_ll_get_agc_config2_agc_enable()
#define aud_hal_set_agc_config2_agc_enable(value)                        aud_ll_set_agc_config2_agc_enable(value)

/* REG_0x0d:agc_config2->manual_pga_value:manual set PGA value,RW,0xd[26:20]*/
#define aud_hal_get_agc_config2_manual_pga_value()                         aud_ll_get_agc_config2_manual_pga_value()
#define aud_hal_set_agc_config2_manual_pga_value(value)                  aud_ll_set_agc_config2_manual_pga_value(value)

/* REG_0x0d:agc_config2->manual_pga:manual set PGA mode,RW,0xd[27]*/
#define aud_hal_get_agc_config2_manual_pga()                               aud_ll_get_agc_config2_manual_pga()
#define aud_hal_set_agc_config2_manual_pga(value)                        aud_ll_set_agc_config2_manual_pga(value)

/* REG_0x0e */
#define aud_hal_get_fifo_status_value()                                    aud_ll_get_fifo_status_value()

/* REG_0x0e:fifo_status->dacr_near_full:DAC right channel FIFO near full: higher than 1/4 capacity of default,R,0xe[0]*/
#define aud_hal_get_fifo_status_dacr_near_full()                           aud_ll_get_fifo_status_dacr_near_full()

/* REG_0x0e:fifo_status->dacl_near_full:DAC left channel FIFO near full,R,0xe[1]*/
#define aud_hal_get_fifo_status_dacl_near_full()                           aud_ll_get_fifo_status_dacl_near_full()

/* REG_0x0e:fifo_status->adcl_near_full:ADC left channel FIFO near full,R,0xe[2]*/
#define aud_hal_get_fifo_status_adcl_near_full()                           aud_ll_get_fifo_status_adcl_near_full()

/* REG_0x0e:fifo_status->dtmf_near_full:DTMF FIFO near full,R,0xe[3]*/
#define aud_hal_get_fifo_status_dtmf_near_full()                           aud_ll_get_fifo_status_dtmf_near_full()

/* REG_0x0e:fifo_status->dacr_near_empty:DAC right channel FIFO near empty: lower than 1/4 capacity of default,R,0xe[4]*/
#define aud_hal_get_fifo_status_dacr_near_empty()                          aud_ll_get_fifo_status_dacr_near_empty()

/* REG_0x0e:fifo_status->dacl_near_empty:DAC left channel FIFO near empty,R,0xe[5]*/
#define aud_hal_get_fifo_status_dacl_near_empty()                          aud_ll_get_fifo_status_dacl_near_empty()

/* REG_0x0e:fifo_status->adcl_near_empty:ADC left channel FIFO near empty,R,0xe[6]*/
#define aud_hal_get_fifo_status_adcl_near_empty()                          aud_ll_get_fifo_status_adcl_near_empty()

/* REG_0x0e:fifo_status->dtmf_near_empty:DTMF FIFO near empty,R,0xe[7]*/
#define aud_hal_get_fifo_status_dtmf_near_empty()                          aud_ll_get_fifo_status_dtmf_near_empty()

/* REG_0x0e:fifo_status->dacr_fifo_full:DAC right channel FIFO full: the maxvalue of fifo,R,0xe[8]*/
#define aud_hal_get_fifo_status_dacr_fifo_full()                           aud_ll_get_fifo_status_dacr_fifo_full()

/* REG_0x0e:fifo_status->dacl_fifo_full:DAC left channel FIFO full,R,0xe[9]*/
#define aud_hal_get_fifo_status_dacl_fifo_full()                           aud_ll_get_fifo_status_dacl_fifo_full()

/* REG_0x0e:fifo_status->adcl_fifo_full:ADC left channel FIFO full,R,0xe[10]*/
#define aud_hal_get_fifo_status_adcl_fifo_full()                           aud_ll_get_fifo_status_adcl_fifo_full()

/* REG_0x0e:fifo_status->dtmf_fifo_full:DTMF FIFO full,R,0xe[11]*/
#define aud_hal_get_fifo_status_dtmf_fifo_full()                           aud_ll_get_fifo_status_dtmf_fifo_full()

/* REG_0x0e:fifo_status->dacr_fifo_empty:DAC right channel FIFO empty: the minvalue of fifo,R,0xe[12]*/
#define aud_hal_get_fifo_status_dacr_fifo_empty()                          aud_ll_get_fifo_status_dacr_fifo_empty()

/* REG_0x0e:fifo_status->dacl_fifo_empty:DAC left channel FIFO empty,R,0xe[13]*/
#define aud_hal_get_fifo_status_dacl_fifo_empty()                          aud_ll_get_fifo_status_dacl_fifo_empty()

/* REG_0x0e:fifo_status->adcl_fifo_empty:ADC left channel FIFO empty,R,0xe[14]*/
#define aud_hal_get_fifo_status_adcl_fifo_empty()                          aud_ll_get_fifo_status_adcl_fifo_empty()

/* REG_0x0e:fifo_status->dtmf_fifo_empty:DTMF FIFO empty,R,0xe[15]*/
#define aud_hal_get_fifo_status_dtmf_fifo_empty()                          aud_ll_get_fifo_status_dtmf_fifo_empty()

/* REG_0x0e:fifo_status->dacr_int_flag:DAC right channel interrupt flag,R,0xe[16]*/
#define aud_hal_get_fifo_status_dacr_int_flag()                            aud_ll_get_fifo_status_dacr_int_flag()

/* REG_0x0e:fifo_status->dacl_int_flag:DAC left channel interrupt flag,R,0xe[17]*/
#define aud_hal_get_fifo_status_dacl_int_flag()                            aud_ll_get_fifo_status_dacl_int_flag()

/* REG_0x0e:fifo_status->adcl_int_flag:ADC left channel interrupt flag,R,0xe[18]*/
#define aud_hal_get_fifo_status_adcl_int_flag()                            aud_ll_get_fifo_status_adcl_int_flag()

/* REG_0x0e:fifo_status->dtmf_int_flag:DTMF interrupt flag,R,0xe[19]*/
#define aud_hal_get_fifo_status_dtmf_int_flag()                            aud_ll_get_fifo_status_dtmf_int_flag()

/* REG_0x0f */
#define aud_hal_get_agc_status_value()                                     aud_ll_get_agc_status_value()

/* REG_0x0f:agc_status->rssi:Absolutely microphone signal power in dB unit; Read only,R,0xf[7:0]*/
#define aud_hal_get_agc_status_rssi()                                      aud_ll_get_agc_status_rssi()

/* REG_0x0f:agc_status->mic_pga:{1'b0, GAIN2[2:0] GAIN1[3:0]} (Read only),R,0xf[15:8]*/
#define aud_hal_get_agc_status_mic_pga()                                   aud_ll_get_agc_status_mic_pga()

/* REG_0x0f:agc_status->mic_rssi:Microphone level; MIC_RSSI[15:0]; Read only,R,0xf[31:16]*/
#define aud_hal_get_agc_status_mic_rssi()                                  aud_ll_get_agc_status_mic_rssi()

/* REG_0x10 */
#define aud_hal_get_dtmf_fport_value()                                     aud_ll_get_dtmf_fport_value()

/* REG_0x10:dtmf_fport->dtmf_port:DTMF FIFO read addr,R,0x10[15:0]*/
#define aud_hal_get_dtmf_fport_dtmf_port()                                 aud_ll_get_dtmf_fport_dtmf_port()

/* REG_0x11 */
#define aud_hal_get_adc_fport_value()                                      aud_ll_get_adc_fport_value()

/* REG_0x11:adc_fport->adc_port:ADC FIFO read addr,R,0x11[15:0]*/
#define aud_hal_get_adc_fport_adc_port()                                   aud_ll_get_adc_fport_adc_port()

/* REG_0x12 */
#define aud_hal_set_dac_fport_value(value)                               aud_ll_set_dac_fport_value(value)

/* REG_0x12:dac_fport->dacl_port:DACL FIFO write addr,W,0x12[15:0]*/
//#define aud_hal_set_dac_fport_dacl_port(value)                           aud_ll_set_dac_fport_dacl_port(value)

/* REG_0x12:dac_fport->dacr_port:DACR FIFO write addr,W,0x12[31:16]*/
//#define aud_hal_set_dac_fport_dacr_port(value)                           aud_ll_set_dac_fport_dacr_port(value)

/* REG_0x13 */
#define aud_hal_get_dmic_fport_value()                              aud_ll_get_dmic_fport_value()

/* REG_0x18 */
#define aud_hal_get_extend_cfg_value()                                     aud_ll_get_extend_cfg_value()
#define aud_hal_set_extend_cfg_value(value)                              aud_ll_set_extend_cfg_value(value)

/* REG_0x18:extend_cfg->dac_fracmod_manual:DAC fractional frequency division enable of manual set,RW,0x18[0]*/
#define aud_hal_get_extend_cfg_dac_fracmod_manual()                        aud_ll_get_extend_cfg_dac_fracmod_manual()
#define aud_hal_set_extend_cfg_dac_fracmod_manual(value)                 aud_ll_set_extend_cfg_dac_fracmod_manual(value)

/* REG_0x18:extend_cfg->adc_fracmod_manual:ADC fractional frequency division enable of manual set,RW,0x18[1]*/
#define aud_hal_get_extend_cfg_adc_fracmod_manual()                        aud_ll_get_extend_cfg_adc_fracmod_manual()
#define aud_hal_set_extend_cfg_adc_fracmod_manual(value)                 aud_ll_set_extend_cfg_adc_fracmod_manual(value)

/* REG_0x18:extend_cfg->filt_enable:DAC frequency response filter enable,RW,0x18[2]*/
#define aud_hal_get_extend_cfg_filt_enable()                               aud_ll_get_extend_cfg_filt_enable()
#define aud_hal_set_extend_cfg_filt_enable(value)                        aud_ll_set_extend_cfg_filt_enable(value)

/* REG_0x19 */
#define aud_hal_get_dac_fracmod_value()                                    aud_ll_get_dac_fracmod_value()
#define aud_hal_set_dac_fracmod_value(value)                             aud_ll_set_dac_fracmod_value(value)

/* REG_0x19:dac_fracmod->dac_fracCoef:DAC fractional frequency division value N * 2#define24,RW,0x19[31:0]*/
#define aud_hal_get_dac_fracmod_dac_fraccoef()                             aud_ll_get_dac_fracmod_dac_fraccoef()
#define aud_hal_set_dac_fracmod_dac_fraccoef(value)                      aud_ll_set_dac_fracmod_dac_fraccoef(value)

/* REG_0x1a */
#define aud_hal_get_adc_fracmod_value()                                    aud_ll_get_adc_fracmod_value()
#define aud_hal_set_adc_fracmod_value(value)                             aud_ll_set_adc_fracmod_value(value)

/* REG_0x1a:adc_fracmod->adc_fracCoef:ADC fractional frequency division value N * 2#define24,RW,0x1a[31:0]*/
#define aud_hal_get_adc_fracmod_adc_fraccoef()                             aud_ll_get_adc_fracmod_adc_fraccoef()
#define aud_hal_set_adc_fracmod_adc_fraccoef(value)                      aud_ll_set_adc_fracmod_adc_fraccoef(value)

/* REG_0x1f */
#define aud_hal_get_hpf2_ext_coef_value()                                  aud_ll_get_hpf2_ext_coef_value()
#define aud_hal_set_hpf2_ext_coef_value(value)                           aud_ll_set_hpf2_ext_coef_value(value)

/* REG_0x1f:hpf2_ext_coef->hpf2_A1_L6BIT:HPF2 coefficient A1 low six bit of DAC,RW,0x1f[5:0]*/
#define aud_hal_get_hpf2_ext_coef_hpf2_a1_l6bit()                          aud_ll_get_hpf2_ext_coef_hpf2_a1_l6bit()
#define aud_hal_set_hpf2_ext_coef_hpf2_a1_l6bit(value)                   aud_ll_set_hpf2_ext_coef_hpf2_a1_l6bit(value)

/* REG_0x1f:hpf2_ext_coef->hpf2_A2_L6BIT:HPF2 coefficient A2 low six bit of DAC,RW,0x1f[11:6]*/
#define aud_hal_get_hpf2_ext_coef_hpf2_a2_l6bit()                          aud_ll_get_hpf2_ext_coef_hpf2_a2_l6bit()
#define aud_hal_set_hpf2_ext_coef_hpf2_a2_l6bit(value)                   aud_ll_set_hpf2_ext_coef_hpf2_a2_l6bit(value)

/* REG_0x1f:hpf2_ext_coef->hpf2_B0_L6BIT:HPF2 coefficient B0 low six bit of DAC,RW,0x1f[17:12]*/
#define aud_hal_get_hpf2_ext_coef_hpf2_b0_l6bit()                          aud_ll_get_hpf2_ext_coef_hpf2_b0_l6bit()
#define aud_hal_set_hpf2_ext_coef_hpf2_b0_l6bit(value)                   aud_ll_set_hpf2_ext_coef_hpf2_b0_l6bit(value)

/* REG_0x1f:hpf2_ext_coef->hpf2_B1_L6BIT:HPF2 coefficient B1 low six bit of DAC,RW,0x1f[23:18]*/
#define aud_hal_get_hpf2_ext_coef_hpf2_b1_l6bit()                          aud_ll_get_hpf2_ext_coef_hpf2_b1_l6bit()
#define aud_hal_set_hpf2_ext_coef_hpf2_b1_l6bit(value)                   aud_ll_set_hpf2_ext_coef_hpf2_b1_l6bit(value)

/* REG_0x1f:hpf2_ext_coef->hpf2_B2_L6BIT:HPF2 coefficient B2 low six bit of DAC,RW,0x1f[29:24]*/
#define aud_hal_get_hpf2_ext_coef_hpf2_b2_l6bit()                          aud_ll_get_hpf2_ext_coef_hpf2_b2_l6bit()
#define aud_hal_set_hpf2_ext_coef_hpf2_b2_l6bit(value)                   aud_ll_set_hpf2_ext_coef_hpf2_b2_l6bit(value)

/* REG_0x20 */
#define aud_hal_get_flt0_coef_a1a2_value()                                 aud_ll_get_flt0_coef_a1a2_value()
#define aud_hal_set_flt0_coef_a1a2_value(value)                          aud_ll_set_flt0_coef_a1a2_value(value)

/* REG_0x20:flt0_coef_a1a2->flt0_A1:IIR coefficient A1 of Filter 0,RW,0x20[15:0]*/
#define aud_hal_get_flt0_coef_a1a2_flt0_a1()                               aud_ll_get_flt0_coef_a1a2_flt0_a1()
#define aud_hal_set_flt0_coef_a1a2_flt0_a1(value)                        aud_ll_set_flt0_coef_a1a2_flt0_a1(value)

/* REG_0x20:flt0_coef_a1a2->flt0_A2:IIR coefficient A2 of Filter 0,RW,0x20[31:16]*/
#define aud_hal_get_flt0_coef_a1a2_flt0_a2()                               aud_ll_get_flt0_coef_a1a2_flt0_a2()
#define aud_hal_set_flt0_coef_a1a2_flt0_a2(value)                        aud_ll_set_flt0_coef_a1a2_flt0_a2(value)

/* REG_0x21 */
#define aud_hal_get_flt0_coef_b0b1_value()                                 aud_ll_get_flt0_coef_b0b1_value()
#define aud_hal_set_flt0_coef_b0b1_value(value)                          aud_ll_set_flt0_coef_b0b1_value(value)

/* REG_0x21:flt0_coef_b0b1->flt0_B0:IIR coefficient B0 of Filter 0,RW,0x21[15:0]*/
#define aud_hal_get_flt0_coef_b0b1_flt0_b0()                               aud_ll_get_flt0_coef_b0b1_flt0_b0()
#define aud_hal_set_flt0_coef_b0b1_flt0_b0(value)                        aud_ll_set_flt0_coef_b0b1_flt0_b0(value)

/* REG_0x21:flt0_coef_b0b1->flt0_B1:IIR coefficient B1 of Filter 0,RW,0x21[31:16]*/
#define aud_hal_get_flt0_coef_b0b1_flt0_b1()                               aud_ll_get_flt0_coef_b0b1_flt0_b1()
#define aud_hal_set_flt0_coef_b0b1_flt0_b1(value)                        aud_ll_set_flt0_coef_b0b1_flt0_b1(value)

/* REG_0x22 */
#define aud_hal_get_flt0_coef_b2_value()                                   aud_ll_get_flt0_coef_b2_value()
#define aud_hal_set_flt0_coef_b2_value(value)                            aud_ll_set_flt0_coef_b2_value(value)

/* REG_0x22:flt0_coef_b2->flt0_B2:IIR coefficient B2 of Filter 0,RW,0x22[15:0]*/
#define aud_hal_get_flt0_coef_b2_flt0_b2()                                 aud_ll_get_flt0_coef_b2_flt0_b2()
#define aud_hal_set_flt0_coef_b2_flt0_b2(value)                          aud_ll_set_flt0_coef_b2_flt0_b2(value)

/* REG_0x23 */
#define aud_hal_get_flt1_coef_a1a2_value()                                 aud_ll_get_flt1_coef_a1a2_value()
#define aud_hal_set_flt1_coef_a1a2_value(value)                          aud_ll_set_flt1_coef_a1a2_value(value)

/* REG_0x23:flt1_coef_a1a2->flt1_A1:IIR coefficient A1 of Filter 1,RW,0x23[15:0]*/
#define aud_hal_get_flt1_coef_a1a2_flt1_a1()                               aud_ll_get_flt1_coef_a1a2_flt1_a1()
#define aud_hal_set_flt1_coef_a1a2_flt1_a1(value)                        aud_ll_set_flt1_coef_a1a2_flt1_a1(value)

/* REG_0x23:flt1_coef_a1a2->flt1_A2:IIR coefficient A2 of Filter 1,RW,0x23[31:16]*/
#define aud_hal_get_flt1_coef_a1a2_flt1_a2()                               aud_ll_get_flt1_coef_a1a2_flt1_a2()
#define aud_hal_set_flt1_coef_a1a2_flt1_a2(value)                        aud_ll_set_flt1_coef_a1a2_flt1_a2(value)

/* REG_0x24 */
#define aud_hal_get_flt1_coef_b0b1_value()                                 aud_ll_get_flt1_coef_b0b1_value()
#define aud_hal_set_flt1_coef_b0b1_value(value)                          aud_ll_set_flt1_coef_b0b1_value(value)

/* REG_0x24:flt1_coef_b0b1->flt1_B0:IIR coefficient B0 of Filter 1,RW,0x24[15:0]*/
#define aud_hal_get_flt1_coef_b0b1_flt1_b0()                               aud_ll_get_flt1_coef_b0b1_flt1_b0()
#define aud_hal_set_flt1_coef_b0b1_flt1_b0(value)                        aud_ll_set_flt1_coef_b0b1_flt1_b0(value)

/* REG_0x24:flt1_coef_b0b1->flt1_B1:IIR coefficient B1 of Filter 1,RW,0x24[31:16]*/
#define aud_hal_get_flt1_coef_b0b1_flt1_b1()                               aud_ll_get_flt1_coef_b0b1_flt1_b1()
#define aud_hal_set_flt1_coef_b0b1_flt1_b1(value)                        aud_ll_set_flt1_coef_b0b1_flt1_b1(value)

/* REG_0x25 */
#define aud_hal_get_flt1_coef_b2_value()                                   aud_ll_get_flt1_coef_b2_value()
#define aud_hal_set_flt1_coef_b2_value(value)                            aud_ll_set_flt1_coef_b2_value(value)

/* REG_0x25:flt1_coef_b2->flt1_B2:IIR coefficient B2 of Filter 1,RW,0x25[15:0]*/
#define aud_hal_get_flt1_coef_b2_flt1_b2()                                 aud_ll_get_flt1_coef_b2_flt1_b2()
#define aud_hal_set_flt1_coef_b2_flt1_b2(value)                          aud_ll_set_flt1_coef_b2_flt1_b2(value)

/* REG_0x26 */
#define aud_hal_get_flt2_coef_a1a2_value()                                 aud_ll_get_flt2_coef_a1a2_value()
#define aud_hal_set_flt2_coef_a1a2_value(value)                          aud_ll_set_flt2_coef_a1a2_value(value)

/* REG_0x26:flt2_coef_a1a2->flt2_A1:IIR coefficient A1 of Filter 2,RW,0x26[15:0]*/
#define aud_hal_get_flt2_coef_a1a2_flt2_a1()                               aud_ll_get_flt2_coef_a1a2_flt2_a1()
#define aud_hal_set_flt2_coef_a1a2_flt2_a1(value)                        aud_ll_set_flt2_coef_a1a2_flt2_a1(value)

/* REG_0x26:flt2_coef_a1a2->flt2_A2:IIR coefficient A2 of Filter 2,RW,0x26[31:16]*/
#define aud_hal_get_flt2_coef_a1a2_flt2_a2()                               aud_ll_get_flt2_coef_a1a2_flt2_a2()
#define aud_hal_set_flt2_coef_a1a2_flt2_a2(value)                        aud_ll_set_flt2_coef_a1a2_flt2_a2(value)

/* REG_0x27 */
#define aud_hal_get_flt2_coef_b0b1_value()                                 aud_ll_get_flt2_coef_b0b1_value()
#define aud_hal_set_flt2_coef_b0b1_value(value)                          aud_ll_set_flt2_coef_b0b1_value(value)

/* REG_0x27:flt2_coef_b0b1->flt2_B0:IIR coefficient B0 of Filter 2,RW,0x27[15:0]*/
#define aud_hal_get_flt2_coef_b0b1_flt2_b0()                               aud_ll_get_flt2_coef_b0b1_flt2_b0()
#define aud_hal_set_flt2_coef_b0b1_flt2_b0(value)                        aud_ll_set_flt2_coef_b0b1_flt2_b0(value)

/* REG_0x27:flt2_coef_b0b1->flt2_B1:IIR coefficient B1 of Filter 2,RW,0x27[31:16]*/
#define aud_hal_get_flt2_coef_b0b1_flt2_b1()                               aud_ll_get_flt2_coef_b0b1_flt2_b1()
#define aud_hal_set_flt2_coef_b0b1_flt2_b1(value)                        aud_ll_set_flt2_coef_b0b1_flt2_b1(value)

/* REG_0x28 */
#define aud_hal_get_flt2_coef_b2_value()                                   aud_ll_get_flt2_coef_b2_value()
#define aud_hal_set_flt2_coef_b2_value(value)                            aud_ll_set_flt2_coef_b2_value(value)

/* REG_0x28:flt2_coef_b2->flt2_B2:IIR coefficient B2 of Filter 2,RW,0x28[15:0]*/
#define aud_hal_get_flt2_coef_b2_flt2_b2()                                 aud_ll_get_flt2_coef_b2_flt2_b2()
#define aud_hal_set_flt2_coef_b2_flt2_b2(value)                          aud_ll_set_flt2_coef_b2_flt2_b2(value)

/* REG_0x29 */
#define aud_hal_get_flt3_coef_a1a2_value()                                 aud_ll_get_flt3_coef_a1a2_value()
#define aud_hal_set_flt3_coef_a1a2_value(value)                          aud_ll_set_flt3_coef_a1a2_value(value)

/* REG_0x29:flt3_coef_a1a2->flt3_A1:IIR coefficient A1 of Filter 3,RW,0x29[15:0]*/
#define aud_hal_get_flt3_coef_a1a2_flt3_a1()                               aud_ll_get_flt3_coef_a1a2_flt3_a1()
#define aud_hal_set_flt3_coef_a1a2_flt3_a1(value)                        aud_ll_set_flt3_coef_a1a2_flt3_a1(value)

/* REG_0x29:flt3_coef_a1a2->flt3_A2:IIR coefficient A2 of Filter 3,RW,0x29[31:16]*/
#define aud_hal_get_flt3_coef_a1a2_flt3_a2()                               aud_ll_get_flt3_coef_a1a2_flt3_a2()
#define aud_hal_set_flt3_coef_a1a2_flt3_a2(value)                        aud_ll_set_flt3_coef_a1a2_flt3_a2(value)

/* REG_0x2a */
#define aud_hal_get_flt3_coef_b0b1_value()                                 aud_ll_get_flt3_coef_b0b1_value()
#define aud_hal_set_flt3_coef_b0b1_value(value)                          aud_ll_set_flt3_coef_b0b1_value(value)

/* REG_0x2a:flt3_coef_b0b1->flt3_B0:IIR coefficient B0 of Filter 3,RW,0x2a[15:0]*/
#define aud_hal_get_flt3_coef_b0b1_flt3_b0()                               aud_ll_get_flt3_coef_b0b1_flt3_b0()
#define aud_hal_set_flt3_coef_b0b1_flt3_b0(value)                        aud_ll_set_flt3_coef_b0b1_flt3_b0(value)

/* REG_0x2a:flt3_coef_b0b1->flt3_B1:IIR coefficient B1 of Filter 3,RW,0x2a[31:16]*/
#define aud_hal_get_flt3_coef_b0b1_flt3_b1()                               aud_ll_get_flt3_coef_b0b1_flt3_b1()
#define aud_hal_set_flt3_coef_b0b1_flt3_b1(value)                        aud_ll_set_flt3_coef_b0b1_flt3_b1(value)

/* REG_0x2b */
#define aud_hal_get_flt3_coef_b2_value()                                   aud_ll_get_flt3_coef_b2_value()
#define aud_hal_set_flt3_coef_b2_value(value)                            aud_ll_set_flt3_coef_b2_value(value)

/* REG_0x2b:flt3_coef_b2->flt3_B2:IIR coefficient B2 of Filter 3,RW,0x2b[15:0]*/
#define aud_hal_get_flt3_coef_b2_flt3_b2()                                 aud_ll_get_flt3_coef_b2_flt3_b2()
#define aud_hal_set_flt3_coef_b2_flt3_b2(value)                          aud_ll_set_flt3_coef_b2_flt3_b2(value)

/* REG_0x2c */
#define aud_hal_get_flt0_ext_coef_value()                                  aud_ll_get_flt0_ext_coef_value()
#define aud_hal_set_flt0_ext_coef_value(value)                           aud_ll_set_flt0_ext_coef_value(value)

/* REG_0x2c:flt0_ext_coef->flt0_A1_L6BIT:IIR coefficient A1 low six bit of Filter 0,RW,0x2c[5:0]*/
#define aud_hal_get_flt0_ext_coef_flt0_a1_l6bit()                          aud_ll_get_flt0_ext_coef_flt0_a1_l6bit()
#define aud_hal_set_flt0_ext_coef_flt0_a1_l6bit(value)                   aud_ll_set_flt0_ext_coef_flt0_a1_l6bit(value)

/* REG_0x2c:flt0_ext_coef->flt0_A2_L6BIT:IIR coefficient A2 low six bit of Filter 0,RW,0x2c[11:6]*/
#define aud_hal_get_flt0_ext_coef_flt0_a2_l6bit()                          aud_ll_get_flt0_ext_coef_flt0_a2_l6bit()
#define aud_hal_set_flt0_ext_coef_flt0_a2_l6bit(value)                   aud_ll_set_flt0_ext_coef_flt0_a2_l6bit(value)

/* REG_0x2c:flt0_ext_coef->flt0_B0_L6BIT:IIR coefficient B0 low six bit of Filter 0,RW,0x2c[17:12]*/
#define aud_hal_get_flt0_ext_coef_flt0_b0_l6bit()                          aud_ll_get_flt0_ext_coef_flt0_b0_l6bit()
#define aud_hal_set_flt0_ext_coef_flt0_b0_l6bit(value)                   aud_ll_set_flt0_ext_coef_flt0_b0_l6bit(value)

/* REG_0x2c:flt0_ext_coef->flt0_B1_L6BIT:IIR coefficient B1 low six bit of Filter 0,RW,0x2c[23:18]*/
#define aud_hal_get_flt0_ext_coef_flt0_b1_l6bit()                          aud_ll_get_flt0_ext_coef_flt0_b1_l6bit()
#define aud_hal_set_flt0_ext_coef_flt0_b1_l6bit(value)                   aud_ll_set_flt0_ext_coef_flt0_b1_l6bit(value)

/* REG_0x2c:flt0_ext_coef->flt0_B2_L6BIT:IIR coefficient B2 low six bit of Filter 0,RW,0x2c[29:24]*/
#define aud_hal_get_flt0_ext_coef_flt0_b2_l6bit()                          aud_ll_get_flt0_ext_coef_flt0_b2_l6bit()
#define aud_hal_set_flt0_ext_coef_flt0_b2_l6bit(value)                   aud_ll_set_flt0_ext_coef_flt0_b2_l6bit(value)

/* REG_0x2d */
#define aud_hal_get_flt1_ext_coef_value()                                  aud_ll_get_flt1_ext_coef_value()
#define aud_hal_set_flt1_ext_coef_value(value)                           aud_ll_set_flt1_ext_coef_value(value)

/* REG_0x2d:flt1_ext_coef->flt1_A1_L6BIT:IIR coefficient A1 low six bit of Filter 1,RW,0x2d[5:0]*/
#define aud_hal_get_flt1_ext_coef_flt1_a1_l6bit()                          aud_ll_get_flt1_ext_coef_flt1_a1_l6bit()
#define aud_hal_set_flt1_ext_coef_flt1_a1_l6bit(value)                   aud_ll_set_flt1_ext_coef_flt1_a1_l6bit(value)

/* REG_0x2d:flt1_ext_coef->flt1_A2_L6BIT:IIR coefficient A2 low six bit of Filter 1,RW,0x2d[11:6]*/
#define aud_hal_get_flt1_ext_coef_flt1_a2_l6bit()                          aud_ll_get_flt1_ext_coef_flt1_a2_l6bit()
#define aud_hal_set_flt1_ext_coef_flt1_a2_l6bit(value)                   aud_ll_set_flt1_ext_coef_flt1_a2_l6bit(value)

/* REG_0x2d:flt1_ext_coef->flt1_B0_L6BIT:IIR coefficient B0 low six bit of Filter 1,RW,0x2d[17:12]*/
#define aud_hal_get_flt1_ext_coef_flt1_b0_l6bit()                          aud_ll_get_flt1_ext_coef_flt1_b0_l6bit()
#define aud_hal_set_flt1_ext_coef_flt1_b0_l6bit(value)                   aud_ll_set_flt1_ext_coef_flt1_b0_l6bit(value)

/* REG_0x2d:flt1_ext_coef->flt1_B1_L6BIT:IIR coefficient B1 low six bit of Filter 1,RW,0x2d[23:18]*/
#define aud_hal_get_flt1_ext_coef_flt1_b1_l6bit()                          aud_ll_get_flt1_ext_coef_flt1_b1_l6bit()
#define aud_hal_set_flt1_ext_coef_flt1_b1_l6bit(value)                   aud_ll_set_flt1_ext_coef_flt1_b1_l6bit(value)

/* REG_0x2d:flt1_ext_coef->flt1_B2_L6BIT:IIR coefficient B2 low six bit of Filter 1,RW,0x2d[29:24]*/
#define aud_hal_get_flt1_ext_coef_flt1_b2_l6bit()                          aud_ll_get_flt1_ext_coef_flt1_b2_l6bit()
#define aud_hal_set_flt1_ext_coef_flt1_b2_l6bit(value)                   aud_ll_set_flt1_ext_coef_flt1_b2_l6bit(value)

/* REG_0x2e */
#define aud_hal_get_flt2_ext_coef_value()                                  aud_ll_get_flt2_ext_coef_value()
#define aud_hal_set_flt2_ext_coef_value(value)                           aud_ll_set_flt2_ext_coef_value(value)

/* REG_0x2e:flt2_ext_coef->flt2_A1_L6BIT:IIR coefficient A1 low six bit of Filter 2,RW,0x2e[5:0]*/
#define aud_hal_get_flt2_ext_coef_flt2_a1_l6bit()                          aud_ll_get_flt2_ext_coef_flt2_a1_l6bit()
#define aud_hal_set_flt2_ext_coef_flt2_a1_l6bit(value)                   aud_ll_set_flt2_ext_coef_flt2_a1_l6bit(value)

/* REG_0x2e:flt2_ext_coef->flt2_A2_L6BIT:IIR coefficient A2 low six bit of Filter 2,RW,0x2e[11:6]*/
#define aud_hal_get_flt2_ext_coef_flt2_a2_l6bit()                          aud_ll_get_flt2_ext_coef_flt2_a2_l6bit()
#define aud_hal_set_flt2_ext_coef_flt2_a2_l6bit(value)                   aud_ll_set_flt2_ext_coef_flt2_a2_l6bit(value)

/* REG_0x2e:flt2_ext_coef->flt2_B0_L6BIT:IIR coefficient B0 low six bit of Filter 2,RW,0x2e[17:12]*/
#define aud_hal_get_flt2_ext_coef_flt2_b0_l6bit()                          aud_ll_get_flt2_ext_coef_flt2_b0_l6bit()
#define aud_hal_set_flt2_ext_coef_flt2_b0_l6bit(value)                   aud_ll_set_flt2_ext_coef_flt2_b0_l6bit(value)

/* REG_0x2e:flt2_ext_coef->flt2_B1_L6BIT:IIR coefficient B1 low six bit of Filter 2,RW,0x2e[23:18]*/
#define aud_hal_get_flt2_ext_coef_flt2_b1_l6bit()                          aud_ll_get_flt2_ext_coef_flt2_b1_l6bit()
#define aud_hal_set_flt2_ext_coef_flt2_b1_l6bit(value)                   aud_ll_set_flt2_ext_coef_flt2_b1_l6bit(value)

/* REG_0x2e:flt2_ext_coef->flt2_B2_L6BIT:IIR coefficient B2 low six bit of Filter 2,RW,0x2e[29:24]*/
#define aud_hal_get_flt2_ext_coef_flt2_b2_l6bit()                          aud_ll_get_flt2_ext_coef_flt2_b2_l6bit()
#define aud_hal_set_flt2_ext_coef_flt2_b2_l6bit(value)                   aud_ll_set_flt2_ext_coef_flt2_b2_l6bit(value)

/* REG_0x2f */
#define aud_hal_get_flt3_ext_coef_value()                                  aud_ll_get_flt3_ext_coef_value()
#define aud_hal_set_flt3_ext_coef_value(value)                           aud_ll_set_flt3_ext_coef_value(value)

/* REG_0x2f:flt3_ext_coef->flt3_A1_L6BIT:IIR coefficient A1 low six bit of Filter 3,RW,0x2f[5:0]*/
#define aud_hal_get_flt3_ext_coef_flt3_a1_l6bit()                          aud_ll_get_flt3_ext_coef_flt3_a1_l6bit()
#define aud_hal_set_flt3_ext_coef_flt3_a1_l6bit(value)                   aud_ll_set_flt3_ext_coef_flt3_a1_l6bit(value)

/* REG_0x2f:flt3_ext_coef->flt3_A2_L6BIT:IIR coefficient A2 low six bit of Filter 3,RW,0x2f[11:6]*/
#define aud_hal_get_flt3_ext_coef_flt3_a2_l6bit()                          aud_ll_get_flt3_ext_coef_flt3_a2_l6bit()
#define aud_hal_set_flt3_ext_coef_flt3_a2_l6bit(value)                   aud_ll_set_flt3_ext_coef_flt3_a2_l6bit(value)

/* REG_0x2f:flt3_ext_coef->flt3_B0_L6BIT:IIR coefficient B0 low six bit of Filter 3,RW,0x2f[17:12]*/
#define aud_hal_get_flt3_ext_coef_flt3_b0_l6bit()                          aud_ll_get_flt3_ext_coef_flt3_b0_l6bit()
#define aud_hal_set_flt3_ext_coef_flt3_b0_l6bit(value)                   aud_ll_set_flt3_ext_coef_flt3_b0_l6bit(value)

/* REG_0x2f:flt3_ext_coef->flt3_B1_L6BIT:IIR coefficient B1 low six bit of Filter 3,RW,0x2f[23:18]*/
#define aud_hal_get_flt3_ext_coef_flt3_b1_l6bit()                          aud_ll_get_flt3_ext_coef_flt3_b1_l6bit()
#define aud_hal_set_flt3_ext_coef_flt3_b1_l6bit(value)                   aud_ll_set_flt3_ext_coef_flt3_b1_l6bit(value)

/* REG_0x2f:flt3_ext_coef->flt3_B2_L6BIT:IIR coefficient B2 low six bit of Filter 3,RW,0x2f[29:24]*/
#define aud_hal_get_flt3_ext_coef_flt3_b2_l6bit()                          aud_ll_get_flt3_ext_coef_flt3_b2_l6bit()
#define aud_hal_set_flt3_ext_coef_flt3_b2_l6bit(value)                   aud_ll_set_flt3_ext_coef_flt3_b2_l6bit(value)

/* dac_pn_conf:0x7[28:25], ,0x0,R/W*/
#define aud_hal_get_dac_config0_dac_pn_conf()                          aud_ll_get_dac_config0_dac_pn_conf()
#define aud_hal_set_dac_config0_dac_pn_conf(value)                          aud_ll_set_dac_config0_dac_pn_conf(value)

/* dac_notchen:0x7[29], ,0x0,R/W*/
#define aud_hal_get_dac_config0_dac_notchen()                          aud_ll_get_dac_config0_dac_notchen()
#define aud_hal_set_dac_config0_dac_notchen(value)                          aud_ll_set_dac_config0_dac_notchen(value)

/* loop_dmic2dac:0xa[26],DMIC到DAC环回测试使能位,0x0,R/W*/
#define aud_hal_get_fifo_config_loop_dmic2dac()                            aud_ll_get_fifo_config_loop_dmic2dac()
#define aud_hal_set_fifo_config_loop_dmic2dac(value)                             aud_ll_set_fifo_config_loop_dmic2dac(value)

/* dmic_wr_threshold:0xa[30:27],DMIC自动写FIFO时，数目高于该门限，发出中断请求,0x0,R/W*/
#define aud_hal_get_fifo_config_dmic_wr_threshold()                         aud_ll_get_fifo_config_dmic_wr_threshold()
#define aud_hal_set_fifo_config_dmic_wr_threshold(value)                         aud_ll_set_fifo_config_dmic_wr_threshold(value)

/* dmic_int_en:0xa[31],DMIC中断请求使能位,0x0,R/W*/
#define aud_hal_get_fifo_config_dmic_int_en()                              aud_ll_get_fifo_config_dmic_int_en()
#define aud_hal_set_fifo_config_dmic_int_en(value)                              aud_ll_set_fifo_config_dmic_int_en(value)

/* dmic_near_full:0xe[20],DMIC的FIFO快要满,0x0,R*/
#define aud_hal_get_fifo_status_dmic_near_full()                           aud_ll_get_fifo_status_dmic_near_full()

/* dmic_near_empty:0xe[21],DMIC的FIFO快要空,0x0,R*/
#define aud_hal_get_fifo_status_dmic_near_empty()                          aud_ll_get_fifo_status_dmic_near_empty()

/* dmic_fifo_full:0xe[22],DMIC的FIFO已满,0x0,R*/
#define aud_hal_get_fifo_status_dmic_fifo_full()                           aud_ll_get_fifo_status_dmic_fifo_full()

/* dmic_fifo_empty:0xe[23],DMIC的FIFO已空,0x0,R*/
#define aud_hal_get_fifo_status_dmic_fifo_empty()                          aud_ll_get_fifo_status_dmic_fifo_empty()

/* dmic_int_flag:0xe[24],DMIC的中断标志,0x0,R*/
#define aud_hal_get_fifo_status_dmic_int_flag()                            aud_ll_get_fifo_status_dmic_int_flag()

/* reg2adc_l_di:0x11[15:0],ADC的FIFO读出地址，高16位是右声道，低16位是左声道,0x0,R*/
//#define aud_hal_get_adc_fport_reg2adc_l_di()                               aud_ll_get_adc_fport_reg2adc_l_di()

/* reg2adc_r_di:0x11[31:16], ,0x0,R*/
//#define aud_hal_get_adc_fport_reg2adc_r_di()                               aud_ll_get_adc_fport_reg2adc_r_di()

/* apll_sel:0x30[8],1: audio clk = 24.576MHz(APLL) 0: audio_clk = 26MHz(crystal),0x0,R/W*/
#define aud_hal_get_audio_config_apll_sel()                                aud_ll_get_audio_config_apll_sel()
#define aud_hal_set_audio_config_apll_sel(value)                                aud_ll_set_audio_config_apll_sel(value)

/* dmic_enable:0x30[9],digital mic path enable,0x0,R/W*/
#define aud_hal_get_audio_config_dmic_enable()                              aud_ll_get_audio_config_dmic_enable()
#define aud_hal_set_audio_config_dmic_enable(value)                              aud_ll_set_audio_config_dmic_enable(value)

/* dmic_cic_sel:0x30[11:10],digital mic down sample, 0: 16   1: 32: 2,3: 64,0x0,R/W*/
#define aud_hal_get_audio_config_dmic_cic_sel()                             aud_ll_get_audio_config_dmic_cic_sel()
#define aud_hal_set_audio_config_dmic_cic_sel(value)                             aud_ll_set_audio_config_dmic_cic_sel(value)

/* dig_mic_div:0x30[16:12],digital mic clk = apll/(dig_mic_div+1),0x0,R/W*/
#define aud_hal_get_audio_config_dig_mic_div()                              aud_ll_get_audio_config_dig_mic_div()
#define aud_hal_set_audio_config_dig_mic_div(value)                              aud_ll_set_audio_config_dig_mic_div(value)

/*write only reg:dac_fport:default value:0x0*/
//#define aud_hal_set_dac_fport_value(value)                                  aud_ll_set_dac_fport_value(value)


bk_err_t aud_hal_adc_hpf_config(aud_adc_hpf_config_t *config);
bk_err_t aud_hal_adc_agc_config(aud_adc_agc_config_t *config);
bk_err_t aud_hal_dac_hpf_config(aud_dac_hpf_config_t *config);
bk_err_t aud_hal_dac_filt_config(aud_dac_eq_config_t *config);
bk_err_t aud_hal_dtmf_config(aud_dtmf_config_t *config);

/* get adc fifo port address */
bk_err_t aud_hal_adc_get_fifo_addr(uint32_t *adc_fifo_addr);

/* get dac fifo port address */
bk_err_t aud_hal_dac_get_fifo_addr(uint32_t *dac_fifo_addr);

/* get dtmf fifo port address */
bk_err_t aud_hal_dtmf_get_fifo_addr(uint32_t *dtmf_fifo_addr);

/* get dmic fifo port address */
bk_err_t aud_hal_dmic_get_fifo_addr(uint32_t *dmic_fifo_addr);


#if CFG_HAL_DEBUG_AUD
void aud_struct_dump(void);
#else
#define aud_struct_dump()
#endif


#ifdef __cplusplus
}
#endif
