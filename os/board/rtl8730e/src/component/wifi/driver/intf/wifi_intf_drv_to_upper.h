/**
  ******************************************************************************
  * @file    wifi_intf_drv_to_upper.h
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef __RTW_INTF_USER_H__
#define __RTW_INTF_USER_H__

#ifdef	__cplusplus
extern "C" {
#endif

#include "wifi_api.h"
#include "wifi_intf_drv_to_app_internal.h"

#define		_1M_RATE_		0
#define		_2M_RATE_		1
#define		_5M_RATE_		2
#define		_11M_RATE_		3
#define		_6M_RATE_		4
#define		_9M_RATE_		5
#define		_12M_RATE_		6
#define		_18M_RATE_		7
#define		_24M_RATE_		8
#define		_36M_RATE_		9
#define		_48M_RATE_		10
#define		_54M_RATE_		11

//----- ------------------------------------------------------------------
// Wlan Interface opened for upper layer
//----- ------------------------------------------------------------------
void wifi_if1_init(void);
int wifi_if2_init(void);
void wifi_if1_deinit(void);
void wifi_if2_deinit(void);
int wifi_if1_open(void);
void wifi_if2_open(void);
void wifi_stop_bss_network(void);
void wifi_set_platform_rom_func(void *(*calloc_func)(size_t, size_t),
								void (*free_func)(void *),
								int (*rand_func)(void *, unsigned char *, size_t));
void wifi_heap_statistics(u8 start);
void wifi_ram_statistic_print(u8 print_detail);
void wifi_set_rom2flash(void);
void wifi_set_rom2flash_user_config(void);

extern int rtw_disconnect(void);
extern int rtw_joinbss_start_api(struct rtw_network_info *connect_param);
extern int rtw_ap_start_api(struct rtw_softap_info *softAP_config);
extern int rtw_scan_start_api(struct rtw_scan_param *scan_param, unsigned char block);
extern void rtw_joinbss_by_roaming(struct rtw_network_info *connect_param);

extern int wifi_hal_iwpriv_command(unsigned char wlan_idx, char *cmd, int show_msg);
#define rtw_iwpriv_command(wlan_idx, cmd,show_msg)	wifi_hal_iwpriv_command(wlan_idx, cmd,show_msg)

#ifdef CONFIG_NAN
int rtw_nan_intfs_init(void);
void rtw_nan_intfs_deinit(void);
void rtw_nan_start_api(u8 master_pref, u8 band_support, u8 nan_cmd_type);
void rtw_nan_stop_api(void);
int rtw_nan_func_add(struct rtw_nan_func_t *func_param, u64 nan_func_pointer);
int rtw_nan_func_del(u64 cookie);
void rtw_cfgvendor_cmd_process(u16 vendor_cmd, void *data, u32 len);
#endif
#ifdef CONFIG_P2P
void rtw_p2p_set_role(enum rtw_p2p_role role);
int rtw_p2p_remain_on_ch(u8 wlan_idx, u8 enable);
#endif
#ifdef WHC_SKIP_NP_MSG_TASK
void rtw_single_thread_wakeup(void);
#endif

int rtw_wltunnel_command(char *cmd);
#ifdef CONFIG_WIFI_TUNNEL
void wifi_tunnelapi_joinbss_identity_key_get(u8 *identity_key);
#endif
#ifdef	__cplusplus
}
#endif

#endif //#ifndef __RTW_INTF_USER_H__
