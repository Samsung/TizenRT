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


    typedef enum {
        PS_BMSG_IOCTL_RF_ENABLE = 0,
        PS_BMSG_IOCTL_RF_DISANABLE = 1,
        PS_BMSG_IOCTL_MCU_ENABLE = 2,
        PS_BMSG_IOCTL_MCU_DISANABLE = 3,
        PS_BMSG_IOCTL_RF_USER_WKUP = 4,
        PS_BMSG_IOCTL_RF_KP_SET = 5,
        PS_BMSG_IOCTL_RF_TD_SET = 6,
        PS_BMSG_IOCTL_RF_KP_HANDLER = 7,
        PS_BMSG_IOCTL_RF_TD_HANDLER = 8,
        PS_BMSG_IOCTL_RF_KP_STOP = 9,
        PS_BMSG_IOCTL_WAIT_TM_SET = 10,
        PS_BMSG_IOCTL_WAIT_TM_HANDLER = 11,
        PS_BMSG_IOCTL_RF_PS_TIMER_INIT = 12,
        PS_BMSG_IOCTL_RF_PS_TIMER_DEINIT = 13,
        PS_BMSG_IOCTL_AP_PS_STOP = 14,
        PS_BMSG_IOCTL_AP_PS_START = 15,
        PS_BMSG_IOCTL_AP_PS_RUN = 16,


    } PS_BMSG_IOCTL_CMD;

    typedef enum {
        PS_FORBID_NO_ON = 1,
        PS_FORBID_PREVENT = 2,
        PS_FORBID_VIF_PREVENT = 3,
        PS_FORBID_IN_DOZE = 4,
        PS_FORBID_KEEVT_ON = 5,
        PS_FORBID_BMSG_ON = 6,
        PS_FORBID_TXING = 7,
        PS_FORBID_HW_TIMER = 8,
        PS_FORBID_RXING = 9,
    } PS_FORBID_STATUS;

    typedef enum {
        PS_NO_PS_MODE = 0,
        PS_DTIM_PS_MODE = 3,
        PS_DTIM_PS_OPENING = 4,
        PS_DTIM_PS_CLOSING = 5,
    } PS_MODE_STATUS;

	//////use for BK7256
    typedef enum {
        PS_BMSG_IOCTL_PS_ENABLE = 0,
        PS_BMSG_IOCTL_PS_DISANABLE,
        PS_BMSG_IOCTL_EXC32K_START,
        PS_BMSG_IOCTL_EXC32K_STOP,
    } PS_CMD;

    UINT16 power_save_forbid_trace ( PS_FORBID_STATUS forbid );
    void mac_ps_bcn_callback ( uint8_t *data, int len );
    void mac_ps_dtim_wake ( uint32_t status );
    void mac_ps_msg_process ( UINT8 ps_msg );
    bool power_save_rf_sleep_check ( void );
    UINT8 power_save_if_ps_rf_dtim_enabled ( void );
    void power_save_wake_mac_rf_if_in_sleep ( void );
    void power_save_wake_mac_rf_end_clr_flag ( void );
    void mac_ps_inc_pck_cnt ( void );
    int mac_ps_wakeup_immediately ( void );
    int ps_need_pre_process ( UINT32 arg );
    void mac_ps_set_prevent ( UINT32 if_allow, UINT32 bit );
    UINT8 mac_sleeped( void );
    void mac_ps_bcn_callback ( uint8_t *data, int len );
    void power_save_delay_sleep_check ( void );
    bool  power_save_sleep ( void );
    UINT8 mac_ps_liston_interval_config ( void );
    PS_MODE_STATUS mac_ps_mode ( void );
    void mac_ps_mode_set ( PS_MODE_STATUS mode );
    int power_save_dtim_disable_handler ( void );
    int power_save_dtim_enable_handler ( void );
    UINT8 mac_ps_dtim_low_latency ( void );

void wifi_data_cache_semlist_set(void);
uint32_t ps_get_sleep_prevent(void);

UINT8 power_save_sm_set_all_bcmc(UINT8);
UINT8 power_save_set_dtim_multi(UINT8);
void power_save_dump(void);
UINT32 bk_unconditional_sleep_mode_get(void);

#ifdef __cplusplus
}
#endif

