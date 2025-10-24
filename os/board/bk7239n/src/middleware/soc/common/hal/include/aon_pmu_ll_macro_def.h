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
* This file is generated from BK7256_ADDR Mapping_20211224_format_change_highlight_20220113_update.xlsm automatically                                
* Modify it manually is not recommended                                       
* CHIP ID:BK7256,GENARATE TIME:2022-01-17 15:35:03                                                 
************************************************************************************************************************************/

#pragma once                 
                            
#ifdef __cplusplus          
extern "C" {              
#endif                      

#include <soc/soc.h>          

#define AON_PMU_LL_REG_BASE      (SOC_AON_PMU_REG_BASE) //REG_BASE:0x44000000

/* REG_0x00 */
#define AON_PMU_REG0_ADDR  (AON_PMU_LL_REG_BASE  + 0x0*4) //REG ADDR :0x44000000
#define AON_PMU_REG0_MEMCHK_BPS_POS (0) 
#define AON_PMU_REG0_MEMCHK_BPS_MASK (0x1) 

#define AON_PMU_REG0_RESERVED_POS (1) 
#define AON_PMU_REG0_RESERVED_MASK (0x7FFFFFFF) 

static inline uint32_t aon_pmu_ll_get_reg0_value(void)
{
    return REG_READ(AON_PMU_REG0_ADDR);
}

static inline void aon_pmu_ll_set_reg0_value(uint32_t value)
{
    REG_WRITE(AON_PMU_REG0_ADDR,value);
}

/* REG_0x00:reg0->memchk_bps:0x0[0],memcheck bypass,1,R/W*/
static inline uint32_t aon_pmu_ll_get_reg0_memchk_bps(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG0_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG0_MEMCHK_BPS_POS) & AON_PMU_REG0_MEMCHK_BPS_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg0_memchk_bps(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG0_ADDR);
    reg_value &= ~(AON_PMU_REG0_MEMCHK_BPS_MASK << AON_PMU_REG0_MEMCHK_BPS_POS);
    reg_value |= ((value & AON_PMU_REG0_MEMCHK_BPS_MASK) << AON_PMU_REG0_MEMCHK_BPS_POS);
    REG_WRITE(AON_PMU_REG0_ADDR,reg_value);
}

/* REG_0x01 */
#define AON_PMU_REG1_ADDR  (AON_PMU_LL_REG_BASE  + 0x1*4) //REG ADDR :0x44000004
#define AON_PMU_REG1_TOUCH_SELECT_POS (0) 
#define AON_PMU_REG1_TOUCH_SELECT_MASK (0xF) 

#define AON_PMU_REG1_TOUCH_INT_EN_POS (4) 
#define AON_PMU_REG1_TOUCH_INT_EN_MASK (0xFFFF) 

#define AON_PMU_REG1_USBPLUG_INT_EN_POS (20) 
#define AON_PMU_REG1_USBPLUG_INT_EN_MASK (0x1) 

#define AON_PMU_REG1_RESERVED_POS (21) 
#define AON_PMU_REG1_RESERVED_MASK (0x7FF) 

static inline uint32_t aon_pmu_ll_get_reg1_value(void)
{
    return REG_READ(AON_PMU_REG1_ADDR);
}

static inline void aon_pmu_ll_set_reg1_value(uint32_t value)
{
	REG_WRITE(AON_PMU_REG1_ADDR,value);
}

/* REG_0x01:reg1->touch_select:0x1[3:0], ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg1_touch_select(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG1_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG1_TOUCH_SELECT_POS)&AON_PMU_REG1_TOUCH_SELECT_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg1_touch_select(uint32_t value)
{
	uint32_t reg_value;
	reg_value = REG_READ(AON_PMU_REG1_ADDR);
	reg_value &= ~(AON_PMU_REG1_TOUCH_SELECT_MASK << AON_PMU_REG1_TOUCH_SELECT_POS);
	reg_value |= ((value & AON_PMU_REG1_TOUCH_SELECT_MASK) << AON_PMU_REG1_TOUCH_SELECT_POS);
	REG_WRITE(AON_PMU_REG1_ADDR,reg_value);
}


/* REG_0x01:reg1->touch_int_en:0x1[19:4], ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg1_touch_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG1_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG1_TOUCH_INT_EN_POS) & AON_PMU_REG1_TOUCH_INT_EN_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg1_touch_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG1_ADDR);
    reg_value &= ~(AON_PMU_REG1_TOUCH_INT_EN_MASK << AON_PMU_REG1_TOUCH_INT_EN_POS);
    reg_value |= ((value & AON_PMU_REG1_TOUCH_INT_EN_MASK) << AON_PMU_REG1_TOUCH_INT_EN_POS);
    REG_WRITE(AON_PMU_REG1_ADDR,reg_value);
}

/* REG_0x01:reg1->usbplug_int_en:0x1[20], ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg1_usbplug_int_en(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG1_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG1_USBPLUG_INT_EN_POS) & AON_PMU_REG1_USBPLUG_INT_EN_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg1_usbplug_int_en(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG1_ADDR);
    reg_value &= ~(AON_PMU_REG1_USBPLUG_INT_EN_MASK << AON_PMU_REG1_USBPLUG_INT_EN_POS);
    reg_value |= ((value & AON_PMU_REG1_USBPLUG_INT_EN_MASK) << AON_PMU_REG1_USBPLUG_INT_EN_POS);
    REG_WRITE(AON_PMU_REG1_ADDR,reg_value);
}

/* REG_0x02 */
#define AON_PMU_REG2_ADDR  (AON_PMU_LL_REG_BASE  + 0x2*4) //REG ADDR :0x44000008
#define AON_PMU_REG2_WDT_RST_ANA_POS (0) 
#define AON_PMU_REG2_WDT_RST_ANA_MASK (0x1) 

#define AON_PMU_REG2_WDT_RST_TOP_POS (1) 
#define AON_PMU_REG2_WDT_RST_TOP_MASK (0x1) 

#define AON_PMU_REG2_WDT_RST_AON_POS (2) 
#define AON_PMU_REG2_WDT_RST_AON_MASK (0x1) 

#define AON_PMU_REG2_WDT_RST_AWT_POS (3) 
#define AON_PMU_REG2_WDT_RST_AWT_MASK (0x1) 

#define AON_PMU_REG2_WDT_RST_GPIO_POS (4) 
#define AON_PMU_REG2_WDT_RST_GPIO_MASK (0x1) 

#define AON_PMU_REG2_WDT_RST_RTC_POS (5) 
#define AON_PMU_REG2_WDT_RST_RTC_MASK (0x1) 

#define AON_PMU_REG2_WDT_RST_WDT_POS (6) 
#define AON_PMU_REG2_WDT_RST_WDT_MASK (0x1) 

#define AON_PMU_REG2_WDT_RST_PMU_POS (7) 
#define AON_PMU_REG2_WDT_RST_PMU_MASK (0x1) 

#define AON_PMU_REG2_WDT_RST_PHY_POS (8) 
#define AON_PMU_REG2_WDT_RST_PHY_MASK (0x1) 

#define AON_PMU_REG2_RESERVED_POS (9)
#define AON_PMU_REG2_RESERVED_MASK (0x7FFFFF)

#define AON_PMU_REG2_WDT_RST_MASK (0x1FF)


static inline uint32_t aon_pmu_ll_get_reg2_value(void)
{
    return REG_READ(AON_PMU_REG2_ADDR);
}

static inline void aon_pmu_ll_set_reg2_value(uint32_t value)
{
    REG_WRITE(AON_PMU_REG2_ADDR,value);
}

/* REG_0x02:reg2->wdt_rst_ana:0x2[0],wdt rst of ana,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_ana(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG2_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG2_WDT_RST_ANA_POS) & AON_PMU_REG2_WDT_RST_ANA_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_ana(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG2_ADDR);
    reg_value &= ~(AON_PMU_REG2_WDT_RST_ANA_MASK << AON_PMU_REG2_WDT_RST_ANA_POS);
    reg_value |= ((value & AON_PMU_REG2_WDT_RST_ANA_MASK) << AON_PMU_REG2_WDT_RST_ANA_POS);
    REG_WRITE(AON_PMU_REG2_ADDR,reg_value);
}

/* REG_0x02:reg2->wdt_rst_top:0x2[1],wdt rst of top,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_top(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG2_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG2_WDT_RST_TOP_POS) & AON_PMU_REG2_WDT_RST_TOP_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_top(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG2_ADDR);
    reg_value &= ~(AON_PMU_REG2_WDT_RST_TOP_MASK << AON_PMU_REG2_WDT_RST_TOP_POS);
    reg_value |= ((value & AON_PMU_REG2_WDT_RST_TOP_MASK) << AON_PMU_REG2_WDT_RST_TOP_POS);
    REG_WRITE(AON_PMU_REG2_ADDR,reg_value);
}

/* REG_0x02:reg2->wdt_rst_aon:0x2[2],wdt rst of aon,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_aon(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG2_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG2_WDT_RST_AON_POS) & AON_PMU_REG2_WDT_RST_AON_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_aon(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG2_ADDR);
    reg_value &= ~(AON_PMU_REG2_WDT_RST_AON_MASK << AON_PMU_REG2_WDT_RST_AON_POS);
    reg_value |= ((value & AON_PMU_REG2_WDT_RST_AON_MASK) << AON_PMU_REG2_WDT_RST_AON_POS);
    REG_WRITE(AON_PMU_REG2_ADDR,reg_value);
}

/* REG_0x02:reg2->wdt_rst_awt:0x2[3],wdt rst of awt,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_awt(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG2_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG2_WDT_RST_AWT_POS) & AON_PMU_REG2_WDT_RST_AWT_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_awt(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG2_ADDR);
    reg_value &= ~(AON_PMU_REG2_WDT_RST_AWT_MASK << AON_PMU_REG2_WDT_RST_AWT_POS);
    reg_value |= ((value & AON_PMU_REG2_WDT_RST_AWT_MASK) << AON_PMU_REG2_WDT_RST_AWT_POS);
    REG_WRITE(AON_PMU_REG2_ADDR,reg_value);
}

/* REG_0x02:reg2->wdt_rst_gpio:0x2[4],wdt rst of gpio,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_gpio(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG2_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG2_WDT_RST_GPIO_POS) & AON_PMU_REG2_WDT_RST_GPIO_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_gpio(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG2_ADDR);
    reg_value &= ~(AON_PMU_REG2_WDT_RST_GPIO_MASK << AON_PMU_REG2_WDT_RST_GPIO_POS);
    reg_value |= ((value & AON_PMU_REG2_WDT_RST_GPIO_MASK) << AON_PMU_REG2_WDT_RST_GPIO_POS);
    REG_WRITE(AON_PMU_REG2_ADDR,reg_value);
}

/* REG_0x02:reg2->wdt_rst_rtc:0x2[5],wdt rst of rtc,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_rtc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG2_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG2_WDT_RST_RTC_POS) & AON_PMU_REG2_WDT_RST_RTC_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_rtc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG2_ADDR);
    reg_value &= ~(AON_PMU_REG2_WDT_RST_RTC_MASK << AON_PMU_REG2_WDT_RST_RTC_POS);
    reg_value |= ((value & AON_PMU_REG2_WDT_RST_RTC_MASK) << AON_PMU_REG2_WDT_RST_RTC_POS);
    REG_WRITE(AON_PMU_REG2_ADDR,reg_value);
}

/* REG_0x02:reg2->wdt_rst_wdt:0x2[6],wdt rst of wdt,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_wdt(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG2_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG2_WDT_RST_WDT_POS) & AON_PMU_REG2_WDT_RST_WDT_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_wdt(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG2_ADDR);
    reg_value &= ~(AON_PMU_REG2_WDT_RST_WDT_MASK << AON_PMU_REG2_WDT_RST_WDT_POS);
    reg_value |= ((value & AON_PMU_REG2_WDT_RST_WDT_MASK) << AON_PMU_REG2_WDT_RST_WDT_POS);
    REG_WRITE(AON_PMU_REG2_ADDR,reg_value);
}

/* REG_0x02:reg2->wdt_rst_pmu:0x2[7],wdt rst of pmu,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_pmu(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG2_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG2_WDT_RST_PMU_POS) & AON_PMU_REG2_WDT_RST_PMU_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_pmu(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG2_ADDR);
    reg_value &= ~(AON_PMU_REG2_WDT_RST_PMU_MASK << AON_PMU_REG2_WDT_RST_PMU_POS);
    reg_value |= ((value & AON_PMU_REG2_WDT_RST_PMU_MASK) << AON_PMU_REG2_WDT_RST_PMU_POS);
    REG_WRITE(AON_PMU_REG2_ADDR,reg_value);
}

/* REG_0x02:reg2->wdt_rst_phy:0x2[8],wdt rst of phy,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg2_wdt_rst_phy(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG2_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG2_WDT_RST_PHY_POS) & AON_PMU_REG2_WDT_RST_PHY_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg2_wdt_rst_phy(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG2_ADDR);
    reg_value &= ~(AON_PMU_REG2_WDT_RST_PHY_MASK << AON_PMU_REG2_WDT_RST_PHY_POS);
    reg_value |= ((value & AON_PMU_REG2_WDT_RST_PHY_MASK) << AON_PMU_REG2_WDT_RST_PHY_POS);
    REG_WRITE(AON_PMU_REG2_ADDR,reg_value);
}

/* REG_0x03 */
#define AON_PMU_REG3_ADDR  (AON_PMU_LL_REG_BASE  + 0x3*4) //REG ADDR :0x4400000c
#define AON_PMU_REG3_AON_REG3_POS (0) 
#define AON_PMU_REG3_AON_REG3_MASK (0xFFFFFFFF) 

static inline uint32_t aon_pmu_ll_get_reg3_value(void)
{
    return REG_READ(AON_PMU_REG3_ADDR);
}

static inline void aon_pmu_ll_set_reg3_value(uint32_t value)
{
    REG_WRITE(AON_PMU_REG3_ADDR,value);
}

/* REG_0x03:reg3->aon_reg3:0x3[31:0],aon_reg3,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg3_aon_reg3(void)
{
    return REG_READ(AON_PMU_REG3_ADDR);
}

static inline void aon_pmu_ll_set_reg3_aon_reg3(uint32_t value)
{
    REG_WRITE(AON_PMU_REG3_ADDR,value);
}

/* REG_0x40 */
#define AON_PMU_REG40_ADDR  (AON_PMU_LL_REG_BASE  + 0x40*4) //REG ADDR :0x44000100
#define AON_PMU_REG40_WAKE1_DELAY_POS (0) 
#define AON_PMU_REG40_WAKE1_DELAY_MASK (0xF) 

#define AON_PMU_REG40_WAKE2_DELAY_POS (4) 
#define AON_PMU_REG40_WAKE2_DELAY_MASK (0xF) 

#define AON_PMU_REG40_WAKE3_DELAY_POS (8) 
#define AON_PMU_REG40_WAKE3_DELAY_MASK (0xF) 

#define AON_PMU_REG40_HALT1_DELAY_POS (12) 
#define AON_PMU_REG40_HALT1_DELAY_MASK (0xF) 

#define AON_PMU_REG40_HALT2_DELAY_POS (16) 
#define AON_PMU_REG40_HALT2_DELAY_MASK (0xF) 

#define AON_PMU_REG40_HALT3_DELAY_POS (20) 
#define AON_PMU_REG40_HALT3_DELAY_MASK (0xF) 

#define AON_PMU_REG40_HALT_VOLT_POS (24) 
#define AON_PMU_REG40_HALT_VOLT_MASK (0x1) 

#define AON_PMU_REG40_HALT_XTAL_POS (25) 
#define AON_PMU_REG40_HALT_XTAL_MASK (0x1) 

#define AON_PMU_REG40_HALT_CORE_POS (26) 
#define AON_PMU_REG40_HALT_CORE_MASK (0x1) 

#define AON_PMU_REG40_HALT_FLASH_POS (27) 
#define AON_PMU_REG40_HALT_FLASH_MASK (0x1) 

#define AON_PMU_REG40_HALT_ROSC_POS (28) 
#define AON_PMU_REG40_HALT_ROSC_MASK (0x1) 

#define AON_PMU_REG40_HALT_RESTEN_POS (29) 
#define AON_PMU_REG40_HALT_RESTEN_MASK (0x1) 

#define AON_PMU_REG40_HALT_ISOLAT_POS (30) 
#define AON_PMU_REG40_HALT_ISOLAT_MASK (0x1) 

#define AON_PMU_REG40_HALT_CLKENA_POS (31) 
#define AON_PMU_REG40_HALT_CLKENA_MASK (0x1) 

static inline uint32_t aon_pmu_ll_get_reg40_value(void)
{
    return REG_READ(AON_PMU_REG40_ADDR);
}

static inline void aon_pmu_ll_set_reg40_value(uint32_t value)
{
    REG_WRITE(AON_PMU_REG40_ADDR,value);
}

/* REG_0x40:reg40->wake1_delay:0x40[3:0],wake delay of Xtal,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_wake1_delay(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG40_WAKE1_DELAY_POS) & AON_PMU_REG40_WAKE1_DELAY_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg40_wake1_delay(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value &= ~(AON_PMU_REG40_WAKE1_DELAY_MASK << AON_PMU_REG40_WAKE1_DELAY_POS);
    reg_value |= ((value & AON_PMU_REG40_WAKE1_DELAY_MASK) << AON_PMU_REG40_WAKE1_DELAY_POS);
    REG_WRITE(AON_PMU_REG40_ADDR,reg_value);
}

/* REG_0x40:reg40->wake2_delay:0x40[7:4],wake delay of Voltage,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_wake2_delay(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG40_WAKE2_DELAY_POS) & AON_PMU_REG40_WAKE2_DELAY_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg40_wake2_delay(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value &= ~(AON_PMU_REG40_WAKE2_DELAY_MASK << AON_PMU_REG40_WAKE2_DELAY_POS);
    reg_value |= ((value & AON_PMU_REG40_WAKE2_DELAY_MASK) << AON_PMU_REG40_WAKE2_DELAY_POS);
    REG_WRITE(AON_PMU_REG40_ADDR,reg_value);
}

/* REG_0x40:reg40->wake3_delay:0x40[11:8], ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_wake3_delay(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG40_WAKE3_DELAY_POS) & AON_PMU_REG40_WAKE3_DELAY_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg40_wake3_delay(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value &= ~(AON_PMU_REG40_WAKE3_DELAY_MASK << AON_PMU_REG40_WAKE3_DELAY_POS);
    reg_value |= ((value & AON_PMU_REG40_WAKE3_DELAY_MASK) << AON_PMU_REG40_WAKE3_DELAY_POS);
    REG_WRITE(AON_PMU_REG40_ADDR,reg_value);
}

/* REG_0x40:reg40->halt1_delay:0x40[15:12],halt delay of Voltage ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_halt1_delay(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG40_HALT1_DELAY_POS) & AON_PMU_REG40_HALT1_DELAY_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg40_halt1_delay(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value &= ~(AON_PMU_REG40_HALT1_DELAY_MASK << AON_PMU_REG40_HALT1_DELAY_POS);
    reg_value |= ((value & AON_PMU_REG40_HALT1_DELAY_MASK) << AON_PMU_REG40_HALT1_DELAY_POS);
    REG_WRITE(AON_PMU_REG40_ADDR,reg_value);
}

/* REG_0x40:reg40->halt2_delay:0x40[19:16],halt delay of Core and flash ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_halt2_delay(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG40_HALT2_DELAY_POS) & AON_PMU_REG40_HALT2_DELAY_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg40_halt2_delay(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value &= ~(AON_PMU_REG40_HALT2_DELAY_MASK << AON_PMU_REG40_HALT2_DELAY_POS);
    reg_value |= ((value & AON_PMU_REG40_HALT2_DELAY_MASK) << AON_PMU_REG40_HALT2_DELAY_POS);
    REG_WRITE(AON_PMU_REG40_ADDR,reg_value);
}

/* REG_0x40:reg40->halt3_delay:0x40[23:20],halt delay of Xtal,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_halt3_delay(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG40_HALT3_DELAY_POS) & AON_PMU_REG40_HALT3_DELAY_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg40_halt3_delay(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value &= ~(AON_PMU_REG40_HALT3_DELAY_MASK << AON_PMU_REG40_HALT3_DELAY_POS);
    reg_value |= ((value & AON_PMU_REG40_HALT3_DELAY_MASK) << AON_PMU_REG40_HALT3_DELAY_POS);
    REG_WRITE(AON_PMU_REG40_ADDR,reg_value);
}

/* REG_0x40:reg40->halt_volt:0x40[24],1:volt   halt ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_volt(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG40_HALT_VOLT_POS) & AON_PMU_REG40_HALT_VOLT_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg40_halt_volt(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value &= ~(AON_PMU_REG40_HALT_VOLT_MASK << AON_PMU_REG40_HALT_VOLT_POS);
    reg_value |= ((value & AON_PMU_REG40_HALT_VOLT_MASK) << AON_PMU_REG40_HALT_VOLT_POS);
    REG_WRITE(AON_PMU_REG40_ADDR,reg_value);
}

/* REG_0x40:reg40->halt_xtal:0x40[25],1:xtal   halt ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_xtal(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG40_HALT_XTAL_POS) & AON_PMU_REG40_HALT_XTAL_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg40_halt_xtal(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value &= ~(AON_PMU_REG40_HALT_XTAL_MASK << AON_PMU_REG40_HALT_XTAL_POS);
    reg_value |= ((value & AON_PMU_REG40_HALT_XTAL_MASK) << AON_PMU_REG40_HALT_XTAL_POS);
    REG_WRITE(AON_PMU_REG40_ADDR,reg_value);
}

/* REG_0x40:reg40->halt_core:0x40[26],1:core   halt ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_core(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG40_HALT_CORE_POS) & AON_PMU_REG40_HALT_CORE_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg40_halt_core(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value &= ~(AON_PMU_REG40_HALT_CORE_MASK << AON_PMU_REG40_HALT_CORE_POS);
    reg_value |= ((value & AON_PMU_REG40_HALT_CORE_MASK) << AON_PMU_REG40_HALT_CORE_POS);
    REG_WRITE(AON_PMU_REG40_ADDR,reg_value);
}

/* REG_0x40:reg40->halt_flash:0x40[27],1:flash  halt ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_flash(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG40_HALT_FLASH_POS) & AON_PMU_REG40_HALT_FLASH_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg40_halt_flash(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value &= ~(AON_PMU_REG40_HALT_FLASH_MASK << AON_PMU_REG40_HALT_FLASH_POS);
    reg_value |= ((value & AON_PMU_REG40_HALT_FLASH_MASK) << AON_PMU_REG40_HALT_FLASH_POS);
    REG_WRITE(AON_PMU_REG40_ADDR,reg_value);
}

/* REG_0x40:reg40->halt_rosc:0x40[28],1:rosc   halt ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_rosc(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG40_HALT_ROSC_POS) & AON_PMU_REG40_HALT_ROSC_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg40_halt_rosc(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value &= ~(AON_PMU_REG40_HALT_ROSC_MASK << AON_PMU_REG40_HALT_ROSC_POS);
    reg_value |= ((value & AON_PMU_REG40_HALT_ROSC_MASK) << AON_PMU_REG40_HALT_ROSC_POS);
    REG_WRITE(AON_PMU_REG40_ADDR,reg_value);
}

/* REG_0x40:reg40->halt_resten:0x40[29],1:resten halt ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_resten(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG40_HALT_RESTEN_POS) & AON_PMU_REG40_HALT_RESTEN_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg40_halt_resten(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value &= ~(AON_PMU_REG40_HALT_RESTEN_MASK << AON_PMU_REG40_HALT_RESTEN_POS);
    reg_value |= ((value & AON_PMU_REG40_HALT_RESTEN_MASK) << AON_PMU_REG40_HALT_RESTEN_POS);
    REG_WRITE(AON_PMU_REG40_ADDR,reg_value);
}

/* REG_0x40:reg40->halt_isolat:0x40[30],1:isolat halt ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_isolat(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG40_HALT_ISOLAT_POS) & AON_PMU_REG40_HALT_ISOLAT_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg40_halt_isolat(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value &= ~(AON_PMU_REG40_HALT_ISOLAT_MASK << AON_PMU_REG40_HALT_ISOLAT_POS);
    reg_value |= ((value & AON_PMU_REG40_HALT_ISOLAT_MASK) << AON_PMU_REG40_HALT_ISOLAT_POS);
    REG_WRITE(AON_PMU_REG40_ADDR,reg_value);
}

/* REG_0x40:reg40->halt_clkena:0x40[31],0:reg config clk halt ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg40_halt_clkena(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG40_HALT_CLKENA_POS) & AON_PMU_REG40_HALT_CLKENA_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg40_halt_clkena(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG40_ADDR);
    reg_value &= ~(AON_PMU_REG40_HALT_CLKENA_MASK << AON_PMU_REG40_HALT_CLKENA_POS);
    reg_value |= ((value & AON_PMU_REG40_HALT_CLKENA_MASK) << AON_PMU_REG40_HALT_CLKENA_POS);
    REG_WRITE(AON_PMU_REG40_ADDR,reg_value);
}

/* REG_0x41 */
#define AON_PMU_REG41_ADDR  (AON_PMU_LL_REG_BASE  + 0x41*4) //REG ADDR :0x44000104
#define AON_PMU_REG41_LPO_CONFIG_POS (0)
#define AON_PMU_REG41_LPO_CONFIG_MASK (0x3)

#define AON_PMU_REG41_FLSHSCK_IOCAP_POS (2)
#define AON_PMU_REG41_FLSHSCK_IOCAP_MASK (0x3)

#define AON_PMU_REG41_WAKEUP_ENA_POS (4)
#define AON_PMU_REG41_WAKEUP_ENA_MASK (0x1F)

#define AON_PMU_REG41_PSRAM_IODRV_POS (9)
#define AON_PMU_REG41_PSRAM_IODRV_MASK (0x3)

#define AON_PMU_REG41_RESERVED_POS (11)
#define AON_PMU_REG41_RESERVED_MASK (0x7FFFFF)

static inline uint32_t aon_pmu_ll_get_reg41_value(void)
{
    return REG_READ(AON_PMU_REG41_ADDR);
}

static inline void aon_pmu_ll_set_reg41_value(uint32_t value)
{
    REG_WRITE(AON_PMU_REG41_ADDR,value);
}

/* REG_0x41:reg41->lpo_config:0x41[1:0],lpo_src : 1x : clk_ROSC ; 1:clk_X32k ;     0:clk_DIVD,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg41_lpo_config(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG41_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG41_LPO_CONFIG_POS) & AON_PMU_REG41_LPO_CONFIG_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg41_lpo_config(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG41_ADDR);
    reg_value &= ~(AON_PMU_REG41_LPO_CONFIG_MASK << AON_PMU_REG41_LPO_CONFIG_POS);
    reg_value |= ((value & AON_PMU_REG41_LPO_CONFIG_MASK) << AON_PMU_REG41_LPO_CONFIG_POS);
    REG_WRITE(AON_PMU_REG41_ADDR,reg_value);
}

/* REG_0x41:reg41->flshsck_iocap:0x41[3:2],spiflash sck driver selection,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg41_flshsck_iocap(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG41_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG41_FLSHSCK_IOCAP_POS) & AON_PMU_REG41_FLSHSCK_IOCAP_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg41_flshsck_iocap(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG41_ADDR);
    reg_value &= ~(AON_PMU_REG41_FLSHSCK_IOCAP_MASK << AON_PMU_REG41_FLSHSCK_IOCAP_POS);
    reg_value |= ((value & AON_PMU_REG41_FLSHSCK_IOCAP_MASK) << AON_PMU_REG41_FLSHSCK_IOCAP_POS);
    REG_WRITE(AON_PMU_REG41_ADDR,reg_value);
}

/* REG_0x41:reg41->wakeup_ena:0x41[8:4],[8]:int_touched                                                                 [7]:int_usbplug                                                                    [6]:system_wake                                                                   [5]:int_rtc                                                                                 [4]:int_gpio,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg41_wakeup_ena(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG41_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG41_WAKEUP_ENA_POS) & AON_PMU_REG41_WAKEUP_ENA_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg41_wakeup_ena(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG41_ADDR);
    reg_value &= ~(AON_PMU_REG41_WAKEUP_ENA_MASK << AON_PMU_REG41_WAKEUP_ENA_POS);
    reg_value |= ((value & AON_PMU_REG41_WAKEUP_ENA_MASK) << AON_PMU_REG41_WAKEUP_ENA_POS);
    REG_WRITE(AON_PMU_REG41_ADDR,reg_value);
}

/* REG_0x41:reg41->psram_iodrv:0x41[10:9], psram clk driver selection,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg41_psram_iodrv(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG41_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG41_PSRAM_IODRV_POS) & AON_PMU_REG41_PSRAM_IODRV_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg41_psram_iodrv(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG41_ADDR);
    reg_value &= ~(AON_PMU_REG41_PSRAM_IODRV_MASK << AON_PMU_REG41_PSRAM_IODRV_POS);
    reg_value |= ((value & AON_PMU_REG41_PSRAM_IODRV_MASK) << AON_PMU_REG41_PSRAM_IODRV_POS);
    REG_WRITE(AON_PMU_REG41_ADDR,reg_value);
}


/* REG_0x42 */
#define AON_PMU_REG42_ADDR  (AON_PMU_LL_REG_BASE  + 0x42*4) //REG ADDR :0x44000108
#define AON_PMU_REG42_REG42_POS (0) 
#define AON_PMU_REG42_REG42_MASK (0xFFFFFFFF) 

static inline uint32_t aon_pmu_ll_get_reg42_value(void)
{
    return REG_READ(AON_PMU_REG42_ADDR);
}

static inline void aon_pmu_ll_set_reg42_value(uint32_t value)
{
    REG_WRITE(AON_PMU_REG42_ADDR,value);
}

/* REG_0x42:reg42->reg42:0x42[31:0], ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg42_reg42(void)
{
    return REG_READ(AON_PMU_REG42_ADDR);
}

static inline void aon_pmu_ll_set_reg42_reg42(uint32_t value)
{
    REG_WRITE(AON_PMU_REG42_ADDR,value);
}

/* REG_0x43 */
#define AON_PMU_REG43_ADDR  (AON_PMU_LL_REG_BASE  + 0x43*4) //REG ADDR :0x4400010c
#define AON_PMU_REG43_CLR_INT_TOUCHED_POS (0) 
#define AON_PMU_REG43_CLR_INT_TOUCHED_MASK (0xFFFF) 

#define AON_PMU_REG43_CLR_INT_USBPLUG_POS (16) 
#define AON_PMU_REG43_CLR_INT_USBPLUG_MASK (0x1) 

#define AON_PMU_REG43_WAKE_SRC_POS (17) 
#define AON_PMU_REG43_WAKE_SRC_MASK (0x1) 

#define AON_PMU_REG43_RESERVED_POS (18) 
#define AON_PMU_REG43_RESERVED_MASK (0x3FFF) 

static inline uint32_t aon_pmu_ll_get_reg43_value(void)
{
    return REG_READ(AON_PMU_REG43_ADDR);
}

static inline void aon_pmu_ll_set_reg43_value(uint32_t value)
{
    REG_WRITE(AON_PMU_REG43_ADDR,value);
}

/* REG_0x43:reg43->clr_int_touched:0x43[15:0],0:touch reset enable,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg43_clr_int_touched(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG43_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG43_CLR_INT_TOUCHED_POS) & AON_PMU_REG43_CLR_INT_TOUCHED_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg43_clr_int_touched(uint32_t value)
{
	/* 
	 * WARNING:This REG has many bits which attribute is write 1 to clear.
	 * If reads data from REG and write it back, maybe the other bits is 1
	 * and cleared by this function un-expectly.
	 */
#if 0
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG43_ADDR);
    reg_value &= ~(AON_PMU_REG43_CLR_INT_TOUCHED_MASK << AON_PMU_REG43_CLR_INT_TOUCHED_POS);
    reg_value |= ((value & AON_PMU_REG43_CLR_INT_TOUCHED_MASK) << AON_PMU_REG43_CLR_INT_TOUCHED_POS);
    REG_WRITE(AON_PMU_REG43_ADDR,reg_value);
#endif
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG43_ADDR);

	//clear all "W1C" bits to 0 to avoid clear other bits
    reg_value &= ~((AON_PMU_REG43_CLR_INT_TOUCHED_MASK << AON_PMU_REG43_CLR_INT_TOUCHED_POS) |
    				(AON_PMU_REG43_CLR_INT_USBPLUG_MASK << AON_PMU_REG43_CLR_INT_USBPLUG_POS) |
    				(AON_PMU_REG43_WAKE_SRC_MASK << AON_PMU_REG43_WAKE_SRC_POS));
    reg_value |= ((value & AON_PMU_REG43_CLR_INT_TOUCHED_MASK) << AON_PMU_REG43_CLR_INT_TOUCHED_POS);
    REG_WRITE(AON_PMU_REG43_ADDR,reg_value);
}

/* REG_0x43:reg43->clr_int_usbplug:0x43[16],0:usb reset enable,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg43_clr_int_usbplug(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG43_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG43_CLR_INT_USBPLUG_POS) & AON_PMU_REG43_CLR_INT_USBPLUG_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg43_clr_int_usbplug(uint32_t value)
{
	/* 
	 * WARNING:This REG has many bits which attribute is write 1 to clear.
	 * If reads data from REG and write it back, maybe the other bits is 1
	 * and cleared by this function un-expectly.
	 */
#if 0
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG43_ADDR);
    reg_value &= ~(AON_PMU_REG43_CLR_INT_USBPLUG_MASK << AON_PMU_REG43_CLR_INT_USBPLUG_POS);
    reg_value |= ((value & AON_PMU_REG43_CLR_INT_USBPLUG_MASK) << AON_PMU_REG43_CLR_INT_USBPLUG_POS);
    REG_WRITE(AON_PMU_REG43_ADDR,reg_value);
#endif
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG43_ADDR);

	//clear all "W1C" bits to 0 to avoid clear other bits
    reg_value &= ~((AON_PMU_REG43_CLR_INT_TOUCHED_MASK << AON_PMU_REG43_CLR_INT_TOUCHED_POS) |
    				(AON_PMU_REG43_CLR_INT_USBPLUG_MASK << AON_PMU_REG43_CLR_INT_USBPLUG_POS) |
    				(AON_PMU_REG43_WAKE_SRC_MASK << AON_PMU_REG43_WAKE_SRC_POS));
    reg_value |= ((value & AON_PMU_REG43_CLR_INT_USBPLUG_MASK) << AON_PMU_REG43_CLR_INT_USBPLUG_POS);
    REG_WRITE(AON_PMU_REG43_ADDR,reg_value);
}

/* REG_0x70 */
#define AON_PMU_REG70_ADDR  (AON_PMU_LL_REG_BASE  + 0x70*4) //REG ADDR :0x440001c0
#define AON_PMU_REG70_INT_TOUCHED_POS (0) 
#define AON_PMU_REG70_INT_TOUCHED_MASK (0xFFFF) 

#define AON_PMU_REG70_INT_USBPLUG_POS (16) 
#define AON_PMU_REG70_INT_USBPLUG_MASK (0x1) 

#define AON_PMU_REG70_RESERVED_POS (17) 
#define AON_PMU_REG70_RESERVED_MASK (0x7FFF) 

static inline uint32_t aon_pmu_ll_get_reg70_value(void)
{
    return REG_READ(AON_PMU_REG70_ADDR);
}

/* REG_0x70:reg70->int_touched:0x70[15:0],touch int state,None,RO*/
static inline uint32_t aon_pmu_ll_get_reg70_int_touched(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG70_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG70_INT_TOUCHED_POS)&AON_PMU_REG70_INT_TOUCHED_MASK);
    return reg_value;
}

/* REG_0x70:reg70->int_usbplug:0x70[16],usb int state,None,RO*/
static inline uint32_t aon_pmu_ll_get_reg70_int_usbplug(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG70_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG70_INT_USBPLUG_POS)&AON_PMU_REG70_INT_USBPLUG_MASK);
    return reg_value;
}

/* REG_0x71 */
#define AON_PMU_REG71_ADDR  (AON_PMU_LL_REG_BASE  + 0x71*4) //REG ADDR :0x440001c4
#define AON_PMU_REG71_TOUCH_STATE_POS (0) 
#define AON_PMU_REG71_TOUCH_STATE_MASK (0xFFFF) 

#define AON_PMU_REG71_USBPLUG_STATE_POS (16) 
#define AON_PMU_REG71_USBPLUG_STATE_MASK (0x1) 

#define AON_PMU_REG71_RESERVED0_POS (17) 
#define AON_PMU_REG71_RESERVED0_MASK (0x7) 

#define AON_PMU_REG71_WAKEUP_SOURCE_POS (20) 
#define AON_PMU_REG71_WAKEUP_SOURCE_MASK (0x1F) 

#define AON_PMU_REG71_RESERVED1_POS (25) 
#define AON_PMU_REG71_RESERVED1_MASK (0x7F) 

static inline uint32_t aon_pmu_ll_get_reg71_value(void)
{
    return REG_READ(AON_PMU_REG71_ADDR);
}

/* REG_0x71:reg71->touch_state:0x71[15:0],touch_state,None,RO*/
static inline uint32_t aon_pmu_ll_get_reg71_touch_state(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG71_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG71_TOUCH_STATE_POS)&AON_PMU_REG71_TOUCH_STATE_MASK);
    return reg_value;
}

/* REG_0x71:reg71->usbplug_state:0x71[16],usbplug_state,None,RO*/
static inline uint32_t aon_pmu_ll_get_reg71_usbplug_state(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG71_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG71_USBPLUG_STATE_POS)&AON_PMU_REG71_USBPLUG_STATE_MASK);
    return reg_value;
}

/* REG_0x71:reg71->wakeup_source:0x71[24:20],wakeup_source,None,RO*/
static inline uint32_t aon_pmu_ll_get_reg71_wakeup_source(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG71_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG71_WAKEUP_SOURCE_POS)&AON_PMU_REG71_WAKEUP_SOURCE_MASK);
    return reg_value;
}

/* REG_0x7C */
#define AON_PMU_REG7C_ADDR  (AON_PMU_LL_REG_BASE  + 0x7C*4) //REG ADDR :0x440001f0
#define AON_PMU_REG7C_ID_POS (0) 
#define AON_PMU_REG7C_ID_MASK (0xFFFFFFFF) 

static inline uint32_t aon_pmu_ll_get_reg7c_value(void)
{
    return REG_READ(AON_PMU_REG7C_ADDR);
}

/* REG_0x7c:reg7c->ID:0x7c[31:0], ,None,R*/
static inline uint32_t aon_pmu_ll_get_reg7c_id(void)
{
    return REG_READ(AON_PMU_REG7C_ADDR);
}

/* REG_0x7D */
#define AON_PMU_REG7D_ADDR  (AON_PMU_LL_REG_BASE  + 0x7D*4) //REG ADDR :0x440001f4
#define AON_PMU_REG7D_LCAL_POS (0) 
#define AON_PMU_REG7D_LCAL_MASK (0xFF) 

#define AON_PMU_REG7D_RCAL_POS (8) 
#define AON_PMU_REG7D_RCAL_MASK (0xFF) 

#define AON_PMU_REG7D_ADC_CAL_POS (16) 
#define AON_PMU_REG7D_ADC_CAL_MASK (0x7F) 

#define AON_PMU_REG7D_BGCAL_POS (23) 
#define AON_PMU_REG7D_BGCAL_MASK (0xFFFF) 

#define AON_PMU_REG7D_LTEMP_POS (29) 
#define AON_PMU_REG7D_LTEMP_MASK (0x1) 

#define AON_PMU_REG7D_HTEMP_POS (30) 
#define AON_PMU_REG7D_HTEMP_MASK (0x1) 

#define AON_PMU_REG7D_PLL26M_UNLOCK_POS (31) 
#define AON_PMU_REG7D_PLL26M_UNLOCK_MASK (0x1) 

static inline uint32_t aon_pmu_ll_get_reg7d_value(void)
{
    return REG_READ(AON_PMU_REG7D_ADDR);
}

/* REG_0x7d:reg7d->lcal:0x7d[7:0], ,None,R*/
static inline uint32_t aon_pmu_ll_get_reg7d_lcal(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG7D_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG7D_LCAL_POS)&AON_PMU_REG7D_LCAL_MASK);
    return reg_value;
}

/* REG_0x7d:reg7d->rcal:0x7d[15:8], ,None,R*/
static inline uint32_t aon_pmu_ll_get_reg7d_rcal(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG7D_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG7D_RCAL_POS)&AON_PMU_REG7D_RCAL_MASK);
    return reg_value;
}

/* REG_0x7d:reg7d->adc_cal:0x7d[22:16], ,None,R*/
static inline uint32_t aon_pmu_ll_get_reg7d_adc_cal(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG7D_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG7D_ADC_CAL_POS)&AON_PMU_REG7D_ADC_CAL_MASK);
    return reg_value;
}

/* REG_0x7d:reg7d->bgcal:0x7d[38:23], ,None,R*/
static inline uint32_t aon_pmu_ll_get_reg7d_bgcal(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG7D_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG7D_BGCAL_POS)&AON_PMU_REG7D_BGCAL_MASK);
    return reg_value;
}

/* REG_0x7d:reg7d->ltemp:0x7d[29], ,None,R*/
static inline uint32_t aon_pmu_ll_get_reg7d_ltemp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG7D_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG7D_LTEMP_POS)&AON_PMU_REG7D_LTEMP_MASK);
    return reg_value;
}

/* REG_0x7d:reg7d->htemp:0x7d[30], ,None,R*/
static inline uint32_t aon_pmu_ll_get_reg7d_htemp(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG7D_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG7D_HTEMP_POS)&AON_PMU_REG7D_HTEMP_MASK);
    return reg_value;
}

/* REG_0x7d:reg7d->pll26M_unlock:0x7d[31], ,None,R*/
static inline uint32_t aon_pmu_ll_get_reg7d_pll26m_unlock(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG7D_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG7D_PLL26M_UNLOCK_POS)&AON_PMU_REG7D_PLL26M_UNLOCK_MASK);
    return reg_value;
}

/* REG_0x7E */
#define AON_PMU_REG7E_ADDR  (AON_PMU_LL_REG_BASE  + 0x7E*4) //REG ADDR :0x440001f8
#define AON_PMU_REG7E_CAPCAL_POS (0) 
#define AON_PMU_REG7E_CAPCAL_MASK (0x1FF) 

#define AON_PMU_REG7E_BIAS_CAL_POS (9)
#define AON_PMU_REG7E_BIAS_CAL_MASK (0x1F)

#define AON_PMU_REG7E_DPLL_UNLOCK_POS (14)
#define AON_PMU_REG7E_DPLL_UNLOCK_MASK (0x1)

#define AON_PMU_REG7E_H_POS (15)
#define AON_PMU_REG7E_H_MASK (0x1FFFF)

static inline uint32_t aon_pmu_ll_get_reg7e_value(void)
{
    return REG_READ(AON_PMU_REG7E_ADDR);
}

static inline void aon_pmu_ll_set_reg7e_value(uint32_t value)
{
    REG_WRITE(AON_PMU_REG7E_ADDR,value);
}

/* REG_0x7e:reg7e->capcal:0x7e[8:0], ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg7e_capcal(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG7E_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG7E_CAPCAL_POS) & AON_PMU_REG7E_CAPCAL_MASK);
    return reg_value;
}

static inline void aon_pmu_ll_set_reg7e_capcal(uint32_t value)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG7E_ADDR);
    reg_value &= ~(AON_PMU_REG7E_CAPCAL_MASK << AON_PMU_REG7E_CAPCAL_POS);
    reg_value |= ((value & AON_PMU_REG7E_CAPCAL_MASK) << AON_PMU_REG7E_CAPCAL_POS);
    REG_WRITE(AON_PMU_REG7E_ADDR,reg_value);
}

/* REG_0x7e:reg7e->capcal:0x7e[13:9], ,None,R/W*/
static inline uint32_t aon_pmu_ll_get_reg7e_bias_cal(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG7E_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG7E_BIAS_CAL_POS) & AON_PMU_REG7E_BIAS_CAL_MASK);
    return reg_value;
}

/* REG_0x7e:reg7e->H:0x7e[31:15], ,None,R*/
static inline uint32_t aon_pmu_ll_get_reg7e_h(void)
{
    uint32_t reg_value;
    reg_value = REG_READ(AON_PMU_REG7E_ADDR);
    reg_value = ((reg_value >> AON_PMU_REG7E_H_POS)&AON_PMU_REG7E_H_MASK);
    return reg_value;
}

/* REG_0x7F */
#define AON_PMU_REG7F_ADDR  (AON_PMU_LL_REG_BASE  + 0x7F*4) //REG ADDR :0x440001fc
#define AON_PMU_REG7F_H_POS (0) 
#define AON_PMU_REG7F_H_MASK (0xFFFFFFFF) 

static inline uint32_t aon_pmu_ll_get_reg7f_value(void)
{
    return REG_READ(AON_PMU_REG7F_ADDR);
}

/* REG_0x7f:reg7f->H:0x7f[31:0], ,None,R*/
static inline uint32_t aon_pmu_ll_get_reg7f_h(void)
{
    return REG_READ(AON_PMU_REG7F_ADDR);
}

#ifdef __cplusplus 
}                  
#endif             
