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

#ifdef  BK_DEEP_SLEEP_DEBUG
#define BK_DEEP_SLEEP_PRT  bk_printf
#else
#define BK_DEEP_SLEEP_PRT   os_null_printf
#endif

    typedef enum {
        PS_DEEP_WAKEUP_GPIO = 1,
        PS_DEEP_WAKEUP_RTC = 2,
        PS_DEEP_WAKEUP_USB = 4,
    }
    PS_DEEP_WAKEUP_WAY;
    
    typedef struct  ps_deep_ctrl {
    
        /*deep_sleep wakeup mode */
        PS_DEEP_WAKEUP_WAY wake_up_way;
        
        /** @brief	Request deep sleep,and wakeup by gpio.
         *
         *	@param	gpio_index_map:The gpio bitmap which set 1 enable wakeup deep sleep.
         *				gpio_index_map is hex and every bits is map to gpio0-gpio31.
         *				ps:gpio1 as uart RX pin must be wake up from falling
         *			gpio_edge_map:The gpio edge bitmap for wakeup gpios,
         *				gpio_edge_map is hex and every bits is map to gpio0-gpio31.
         *				0:rising,1:falling.
         *		gpio_stay_lo_map:The gpio bitmap which need stay ,not change in deep sleep.
         * 			 gpio_index_lo_map is hex and every bits is map to gpio0-gpio31.
        			gpio_last_index_map:The gpio bitmap which set 1 enable wakeup deep sleep.
          * 			 gpio_index_map is hex and every bits is map to gpio32-gpio39.
          * 		gpio_last_edge_map:The gpio edge bitmap for wakeup gpios,
          * 			 gpio_edge_map is hex and every bits is map to gpio32-gpio39.
          * 			 0:rising,1:falling.
         *		gpio_stay_hi_map:The gpio bitmap which need stay ,not change in deep sleep.
         * 			 gpio_index_lo_map is hex and every bits is map to gpio32-gpio39.
         *
         *      sleep_time:the time secound when use PS_DEEP_WAKEUP_RTC wakeup.
         *      lpo_32k_src:the RTC wakeup source.LPO_SELECT_ROSC or LPO_SELECT_32K_XTAL.
         */
        
        UINT32 gpio_index_map;
        UINT32 gpio_edge_map;
        UINT32 gpio_stay_lo_map;
        UINT32 gpio_last_index_map;
        UINT32 gpio_last_edge_map;
        UINT32 gpio_stay_hi_map;
        
        UINT32 sleep_time;
        UINT32 lpo_32k_src;
    } PS_DEEP_CTRL_PARAM;
    
    
    typedef enum {
        MANUAL_MODE_NORMAL = 0,
        MANUAL_MODE_IDLE = 1,
    } MANUAL_MODE;
    
#define     PS_SUPPORT_MANUAL_SLEEP     0
    typedef void ( *ps_wakeup_cb ) ( void );
    extern void bk_enter_deep_sleep_mode ( PS_DEEP_CTRL_PARAM *deep_param );
    
#ifdef __cplusplus
}
#endif
