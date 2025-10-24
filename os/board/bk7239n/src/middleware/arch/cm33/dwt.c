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

#define TAG "dwt"

#include <common/bk_include.h>
#include "bk_arm_arch.h"
#include "armstar.h"
#include "dwt.h"
#include "components/log.h"

#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGV(...) BK_LOGV(TAG, ##__VA_ARGS__)

BIT_CAP_STR_T cap_str_array[CAP_MAX_CNT] = STR_CAP_TABLE;
ID_CAP_T id_cap_array[ID_MAX_CNT] = ID_CAP_TABLE;

void dwt_get_cap_info(uint32_t comparator_id)
{
    uint32_t i;
    uint32_t id = -1, cap = -1;

    if(comparator_id >= COMP_ID_MAX)
        return;

    /* DWT_FUNCTION bit[31:27]:identifies the capabilities of match for comparator n*/
    id = (DWT_Ex->COMPARATOR[comparator_id].FUNCTION & DWT_CTRL_NUMCOMP_Msk) >> DWT_FUNCTION_ID_Pos;
    for(i = 0; i < ID_MAX_CNT; i ++){
        if(id == id_cap_array[i].id){
            cap = id_cap_array[i].cap; 
            break;
        }
    }

    if(-1 == cap)
        return;

    LOGD("comparator id:%d, the following is the capability string:\r\n", comparator_id);
    for(i = 0; i < CAP_MAX_CNT; i ++){
        if(cap & (cap_str_array[i].bit_val)){
            LOGD("    %s\r\n", cap_str_array[i].cap_desc);
        }
    }
}

void dwt_show_comparator_cap(void)
{
    uint32_t i;

    for(i = 0; i < COMP_ID_MAX; i ++){
        dwt_get_cap_info(i);
    }
}

void dwt_enable_debug_monitor_exception(void)
{
    uint32_t dhcsr;

    dhcsr = DCB->DHCSR;
    /* the bit C_DEBUGEN is POR bit(reset by power on reset)
     *  1:halt mode debug; 0:debug monitor mode;
     */
    if(dhcsr & DCB_DHCSR_C_DEBUGEN_Msk){
        /* swd/jtag/gdb connected, maybe*/
        LOGE("Halting Debug Enabled, and You Cannot Enable Debug Monitor Mode\r\n");
        return;
    }

    DCB->DHCSR = (DHCSR_KEY << DCB_DHCSR_DBGKEY_Pos);
}

void dwt_enable_debug_monitor_mode(void)
{
    DCB->DEMCR |= DCB_DEMCR_TRCENA_Msk | DCB_DEMCR_MON_EN_Msk;
}

void dwt_disable_debug_monitor_mode(void)
{
    DCB->DEMCR &= ~(DCB_DEMCR_TRCENA_Msk | DCB_DEMCR_MON_EN_Msk);
}

void dwt_init_cycle_counter(void)
{
    /* when DWT_CTRL.CYCCNTENA = 1 and DEMCR.TRCENA = 1, cycle counter increments
     * when it overflows, it wraps to zero.
     */
    DCB->DEMCR |= DCB_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void dwt_reset_cycle_counter(void)
{
    DWT->CYCCNT = 0;
}

void dwt_enable_cycle_counter(void)
{
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void dwt_disable_cycle_counter(void)
{
    DWT->CTRL &= ~(DWT_CTRL_CYCCNTENA_Msk);
}

uint32_t dwt_get_cycle_counter_val(void)
{
    return DWT->CYCCNT;
}

void dwt_set_data_address_read(uint32_t data_address)
{
    dwt_disable_debug_monitor_mode();

    DWT->COMP0 = data_address;
    DWT->FUNCTION0 = (FUNC_DATA_SIZE_WORD << DWT_FUNCTION_DATAVSIZE_Pos)
                        | (DWT_ACTION_DEBUG_EVENT << DWT_FUNCTION_ACTION_Pos)
                        | (DWT_MATCH_DATA_ADDR_RD << DWT_FUNCTION_MATCH_Pos);

    dwt_enable_debug_monitor_mode();
}

void dwt_set_data_address_write(uint32_t data_address)
{
    dwt_disable_debug_monitor_mode();

    DWT->COMP0 = data_address;
    DWT->FUNCTION0 = (FUNC_DATA_SIZE_WORD << DWT_FUNCTION_DATAVSIZE_Pos)
                        | (DWT_ACTION_DEBUG_EVENT << DWT_FUNCTION_ACTION_Pos)
                        | (DWT_MATCH_DATA_ADDR_WR << DWT_FUNCTION_MATCH_Pos);

    dwt_enable_debug_monitor_mode();
}

void dwt_set_data_address_access(uint32_t data_address)
{
    dwt_disable_debug_monitor_mode();

    DWT->COMP0 = data_address;
    DWT->FUNCTION0 = (FUNC_DATA_SIZE_WORD << DWT_FUNCTION_DATAVSIZE_Pos)
                        | (DWT_ACTION_DEBUG_EVENT << DWT_FUNCTION_ACTION_Pos)
                        | (DWT_MATCH_DATA_ADDR << DWT_FUNCTION_MATCH_Pos);

    dwt_enable_debug_monitor_mode();
}

void dwt_set_instruction_address(uint32_t data_address)
{
    dwt_disable_debug_monitor_mode();

    DWT->COMP0 = data_address;
    DWT->FUNCTION0 = (FUNC_DATA_SIZE_WORD << DWT_FUNCTION_DATAVSIZE_Pos)
                        | (DWT_ACTION_DEBUG_EVENT << DWT_FUNCTION_ACTION_Pos)
                        | (DWT_MATCH_INSTRUCTION_ADDR << DWT_FUNCTION_MATCH_Pos);

    dwt_enable_debug_monitor_mode();
}

void dwt_set_data_address_range(uint32_t start_addr, uint32_t end_addr, ACCESS_T type)
{
    uint32_t access_type = DWT_MATCH_DATA_ADDR;

    dwt_disable_debug_monitor_mode();
    
    if(ACCESS_TYPE_WRITE == type)
        access_type = DWT_MATCH_DATA_ADDR_WR;
    if(ACCESS_TYPE_READ == type)
        access_type = DWT_MATCH_DATA_ADDR_RD;

    /* if the dwt comparator is being used to pair with another dwt comparator
     * for data address range checking, the datavsize should be set to 0
     */
    DWT->COMP2 = start_addr;
    DWT->FUNCTION2 = (FUNC_DATA_SIZE_NONE << DWT_FUNCTION_DATAVSIZE_Pos)
                        | (DWT_ACTION_DEBUG_EVENT << DWT_FUNCTION_ACTION_Pos)
                        | (access_type << DWT_FUNCTION_MATCH_Pos);
    DWT->COMP3 = end_addr;
    DWT->FUNCTION3 = (FUNC_DATA_SIZE_NONE << DWT_FUNCTION_DATAVSIZE_Pos)
                        | (DWT_ACTION_DEBUG_EVENT << DWT_FUNCTION_ACTION_Pos)
                        | (DWT_MATCH_DATA_ADDR_LIMIT << DWT_FUNCTION_MATCH_Pos);

    dwt_enable_debug_monitor_mode();
}

void dwt_set_instruction_address_range(uint32_t start_addr, uint32_t end_addr)
{
    dwt_disable_debug_monitor_mode();

    /* if used as an instruction address or an instruction address limit comparator
     * datavsize MUST be word type
     */
    DWT->COMP2 = start_addr;
    DWT->FUNCTION2 = (FUNC_DATA_SIZE_WORD << DWT_FUNCTION_DATAVSIZE_Pos)
                        | (DWT_ACTION_DEBUG_EVENT << DWT_FUNCTION_ACTION_Pos)
                        | (DWT_MATCH_INSTRUCTION_ADDR << DWT_FUNCTION_MATCH_Pos);
    DWT->COMP3 = end_addr;
    DWT->FUNCTION3 = (FUNC_DATA_SIZE_WORD << DWT_FUNCTION_DATAVSIZE_Pos)
                        | (DWT_ACTION_DEBUG_EVENT << DWT_FUNCTION_ACTION_Pos)
                        | (DWT_MATCH_INSTRUCTION_ADDR_LIMIT << DWT_FUNCTION_MATCH_Pos);

    dwt_enable_debug_monitor_mode();
}

void dwt_conditional_data_watchpoint(uint32_t addr, uint32_t data)
{
    dwt_disable_debug_monitor_mode();

    DWT->COMP2 = addr;
    DWT->FUNCTION2 = (FUNC_DATA_SIZE_WORD << DWT_FUNCTION_DATAVSIZE_Pos)
                        | (DWT_ACTION_TRIGGER_ONLY << DWT_FUNCTION_ACTION_Pos)
                        | (DWT_MATCH_DATA_ADDR_WR << DWT_FUNCTION_MATCH_Pos);
    DWT->COMP3 = data;
    DWT->FUNCTION3 = (FUNC_DATA_SIZE_WORD << DWT_FUNCTION_DATAVSIZE_Pos)
                        | (DWT_ACTION_DEBUG_EVENT << DWT_FUNCTION_ACTION_Pos)
                        | (DWT_MATCH_DATA_VAL_LINKED << DWT_FUNCTION_MATCH_Pos);

    dwt_enable_debug_monitor_mode();
}

void dwt_disable_watchpoint_comparator(void)
{
    dwt_disable_debug_monitor_mode();

    DWT->COMP0 = 0;
    DWT->COMP1 = 0;
    DWT->COMP2 = 0;
    DWT->COMP3 = 0;
    DWT->FUNCTION0 = 0;
    DWT->FUNCTION1 = 0;
    DWT->FUNCTION2 = 0;
    DWT->FUNCTION3 = 0;
}

void arch_dwt_trap_write(uint32_t addr, uint32_t data)
{
    dwt_conditional_data_watchpoint(addr, data);
}

void arch_dwt_trap_disable(void)
{
    dwt_disable_watchpoint_comparator();
}
// eof

