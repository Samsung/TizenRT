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

#include <common/bk_typedef.h>
#include "bk_fake_clock.h"
#include "bk_icu.h"

#ifdef __cplusplus
extern "C" {
#endif

#define     CFG_MCU_PS_SELECT_120M      1

    typedef struct  mcu_ps {
        UINT8 mcu_ps_on;
        int peri_busy_count;
        UINT32 mcu_prevent;
    } MCU_PS_INFO;


#define     MCU_PS_CONNECT      CO_BIT(0)
#define     MCU_PS_ADD_KEY      CO_BIT(1)

#define CHIP_U_MCU_WKUP_USE_TIMER  1

#define  PS_USE_UART_WAKE_ARM   1

    extern void mcu_ps_init ( void );
    extern void mcu_ps_exit ( void );
    extern UINT32 mcu_power_save ( UINT32 );
    extern void mcu_prevent_clear ( UINT32 );
    extern void mcu_prevent_set ( UINT32 );
    extern void peri_busy_count_dec ( void );
    extern void peri_busy_count_add ( void );
    extern UINT32 peri_busy_count_get ( void );
    extern UINT32 mcu_prevent_get ( void );
    extern void mcu_ps_dump ( void );
    extern void ps_pwm_reconfig ( UINT32 , UINT8 );
    extern void ps_pwm_resume_tick ( void );
    extern void ps_pwm_suspend_tick ( UINT32 );
    extern void ps_pwm_disable ( void );
    extern void ps_pwm_enable ( void );
    extern uint32_t ps_timer3_disable ( void );
    extern void ps_timer3_enable ( uint32_t );
    extern UINT32 ps_pwm_int_status ( void );
    extern UINT32 rtt_update_tick ( UINT32 tick );

    extern UINT32 ps_timer3_measure_prepare ( void );
    extern UINT32 mcu_ps_tsf_cal ( UINT64 );
    extern UINT32 mcu_ps_machw_cal ( void );
    extern UINT32 mcu_ps_machw_reset ( void );
    extern UINT32 mcu_ps_machw_init ( void );
    extern UINT32 mcu_ps_is_on ( void );
    extern void mcu_ps_msg_process( UINT8 );

#define  PS_PWM_ID  FCLK_PWM_ID
#if (PS_PWM_ID == PWM0)
#define MCU_PS_PWM_COUNTER      PWM0_COUNTER
#define MCU_PS_PWM_DUTY_CYCLE   PWM0_DUTY_CYCLE
#define PWD_MCU_WAKE_PWM_BIT      PWD_PWM0_CLK_BIT
#elif(PS_PWM_ID == PWM1)
#define MCU_PS_PWM_COUNTER      PWM1_COUNTER
#define MCU_PS_PWM_DUTY_CYCLE   PWM1_DUTY_CYCLE
#define PWD_MCU_WAKE_PWM_BIT      PWD_PWM1_CLK_BIT
#elif(PS_PWM_ID == PWM2)
#define MCU_PS_PWM_COUNTER      PWM2_COUNTER
#define MCU_PS_PWM_DUTY_CYCLE   PWM2_DUTY_CYCLE
#define PWD_MCU_WAKE_PWM_BIT      PWD_PWM2_CLK_BIT
#elif(PS_PWM_ID == PWM3)
#define MCU_PS_PWM_COUNTER      PWM3_COUNTER
#define MCU_PS_PWM_DUTY_CYCLE   PWM3_DUTY_CYCLE
#define PWD_MCU_WAKE_PWM_BIT      PWD_PWM3_CLK_BIT
#elif(PS_PWM_ID == PWM4)
#define MCU_PS_PWM_COUNTER      PWM4_COUNTER
#define MCU_PS_PWM_DUTY_CYCLE   PWM4_DUTY_CYCLE
#define PWD_MCU_WAKE_PWM_BIT      PWD_PWM4_CLK_BIT
#elif(PS_PWM_ID == PWM5)
#define MCU_PS_PWM_COUNTER      PWM5_COUNTER
#define MCU_PS_PWM_DUTY_CYCLE   PWM5_DUTY_CYCLE
#define PWD_MCU_WAKE_PWM_BIT      PWD_PWM5_CLK_BIT
#endif

#ifdef __cplusplus
}
#endif
