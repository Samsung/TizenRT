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

#ifdef __cplusplus
extern "C" {
#endif

//************************************************************//
//AON_RTC
//************************************************************//
#define BASEADDR_AON_RTC                                        (SOC_AON_RTC_REG_BASE)
//addAON_RTC_Reg0x0
#define addAON_RTC_Reg0x0                                       *((volatile unsigned long *) (BASEADDR_AON_RTC+0x0*4))
#define posAON_RTC_Reg0x0_rtc_clk_en                            6
#define bitAON_RTC_Reg0x0_rtc_clk_en                            0x40
#define set_AON_RTC_Reg0x0_rtc_clk_en(val)                      addAON_RTC_Reg0x0 = ((addAON_RTC_Reg0x0 & (~0x40)) | ((val) << 6))
#define setf_AON_RTC_Reg0x0_rtc_clk_en                          addAON_RTC_Reg0x0 |= 0x40
#define clrf_AON_RTC_Reg0x0_rtc_clk_en                          addAON_RTC_Reg0x0 &= ~0x40
#define get_AON_RTC_Reg0x0_rtc_clk_en                           ((addAON_RTC_Reg0x0 & 0x40) >> 6)

#define posAON_RTC_Reg0x0_rtc_tick_int                          5
#define bitAON_RTC_Reg0x0_rtc_tick_int                          0x20
#define set_AON_RTC_Reg0x0_rtc_tick_int(val)                    addAON_RTC_Reg0x0 = ((addAON_RTC_Reg0x0 & (~0x20)) | ((val) << 5))
#define setf_AON_RTC_Reg0x0_rtc_tick_int                        addAON_RTC_Reg0x0 |= 0x20
#define clrf_AON_RTC_Reg0x0_rtc_tick_int                        addAON_RTC_Reg0x0 &= ~0x20
#define get_AON_RTC_Reg0x0_rtc_tick_int                         ((addAON_RTC_Reg0x0 & 0x20) >> 5)

#define posAON_RTC_Reg0x0_rtc_aon_int                           4
#define bitAON_RTC_Reg0x0_rtc_aon_int                           0x10
#define set_AON_RTC_Reg0x0_rtc_aon_int(val)                     addAON_RTC_Reg0x0 = ((addAON_RTC_Reg0x0 & (~0x10)) | ((val) << 4))
#define setf_AON_RTC_Reg0x0_rtc_aon_int                         addAON_RTC_Reg0x0 |= 0x10
#define clrf_AON_RTC_Reg0x0_rtc_aon_int                         addAON_RTC_Reg0x0 &= ~0x10
#define get_AON_RTC_Reg0x0_rtc_aon_int                          ((addAON_RTC_Reg0x0 & 0x10) >> 4)

#define posAON_RTC_Reg0x0_rtc_tick_int_en                       3
#define bitAON_RTC_Reg0x0_rtc_tick_int_en                       0x8
#define set_AON_RTC_Reg0x0_rtc_tick_int_en(val)                 addAON_RTC_Reg0x0 = ((addAON_RTC_Reg0x0 & (~0x8)) | ((val) << 3))
#define setf_AON_RTC_Reg0x0_rtc_tick_int_en                     addAON_RTC_Reg0x0 |= 0x8
#define clrf_AON_RTC_Reg0x0_rtc_tick_int_en                     addAON_RTC_Reg0x0 &= ~0x8
#define get_AON_RTC_Reg0x0_rtc_tick_int_en                      ((addAON_RTC_Reg0x0 & 0x8) >> 3)

#define posAON_RTC_Reg0x0_rtc_aon_int_en                        2
#define bitAON_RTC_Reg0x0_rtc_aon_int_en                        0x4
#define set_AON_RTC_Reg0x0_rtc_aon_int_en(val)                  addAON_RTC_Reg0x0 = ((addAON_RTC_Reg0x0 & (~0x4)) | ((val) << 2))
#define setf_AON_RTC_Reg0x0_rtc_aon_int_en                      addAON_RTC_Reg0x0 |= 0x4
#define clrf_AON_RTC_Reg0x0_rtc_aon_int_en                      addAON_RTC_Reg0x0 &= ~0x4
#define get_AON_RTC_Reg0x0_rtc_aon_int_en                       ((addAON_RTC_Reg0x0 & 0x4) >> 2)

#define posAON_RTC_Reg0x0_rtc_cnt_stop                          1
#define bitAON_RTC_Reg0x0_rtc_cnt_stop                          0x2
#define set_AON_RTC_Reg0x0_rtc_cnt_stop(val)                    addAON_RTC_Reg0x0 = ((addAON_RTC_Reg0x0 & (~0x2)) | ((val) << 1))
#define setf_AON_RTC_Reg0x0_rtc_cnt_stop                        addAON_RTC_Reg0x0 |= 0x2
#define clrf_AON_RTC_Reg0x0_rtc_cnt_stop                        addAON_RTC_Reg0x0 &= ~0x2
#define get_AON_RTC_Reg0x0_rtc_cnt_stop                         ((addAON_RTC_Reg0x0 & 0x2) >> 1)

#define posAON_RTC_Reg0x0_rtc_cnt_reset                         0
#define bitAON_RTC_Reg0x0_rtc_cnt_reset                         0x1
#define set_AON_RTC_Reg0x0_rtc_cnt_reset(val)                   addAON_RTC_Reg0x0 = ((addAON_RTC_Reg0x0 & (~0x1)) | ((val) << 0))
#define setf_AON_RTC_Reg0x0_rtc_cnt_reset                       addAON_RTC_Reg0x0 |= 0x1
#define clrf_AON_RTC_Reg0x0_rtc_cnt_reset                       addAON_RTC_Reg0x0 &= ~0x1
#define get_AON_RTC_Reg0x0_rtc_cnt_reset                        (addAON_RTC_Reg0x0 & 0x1)

//addAON_RTC_Reg0x1
#define addAON_RTC_Reg0x1                                       *((volatile unsigned long *) (BASEADDR_AON_RTC+0x1*4))

//addAON_RTC_Reg0x2
#define addAON_RTC_Reg0x2                                       *((volatile unsigned long *) (BASEADDR_AON_RTC+0x2*4))

//addAON_RTC_Reg0x3
#define addAON_RTC_Reg0x3                                       *((volatile unsigned long *) (BASEADDR_AON_RTC+0x3*4))

//addAON_RTC_Reg0x4
#define addAON_RTC_Reg0x4                                       *((volatile unsigned long *) (BASEADDR_AON_RTC+0x4*4))

//addAON_RTC_Reg0x5
#define addAON_RTC_Reg0x5                                       *((volatile unsigned long *) (BASEADDR_AON_RTC+0x5*4))

#ifdef __cplusplus
}
#endif

