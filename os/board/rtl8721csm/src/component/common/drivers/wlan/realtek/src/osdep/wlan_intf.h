/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
#ifndef __WLAN_INTF_H__
#define __WLAN_INTF_H__

#ifdef	__cplusplus
extern "C" {
#endif
#include <autoconf.h>

#include <wireless.h>
#include "wifi_constants.h"

#ifndef WLAN0_IDX
	#define WLAN0_IDX	0
#endif
#ifndef WLAN1_IDX
	#define WLAN1_IDX	1
#endif
#ifndef WLAN_UNDEF
	#define WLAN_UNDEF	-1
#endif

/***********************************************************/
/* 
struct sk_buff {
	// These two members must be first.
	struct sk_buff		*next;		// Next buffer in list
	struct sk_buff		*prev;		// Previous buffer in list
	
	struct sk_buff_head	*list;			// List we are on	
	unsigned char		*head;		// Head of buffer
	unsigned char		*data;		// Data head pointer
	unsigned char		*tail;		// Tail pointer
	unsigned char		*end;		//End pointer
	struct net_device 	*dev;		//Device we arrived on/are leaving by 	
	unsigned int 		len;			// Length of actual data 
};
*/
/************************************************************/

//----- ------------------------------------------------------------------
// Wlan Interface opened for upper layer
//----- ------------------------------------------------------------------
int rltk_wlan_init(int idx_wlan, rtw_mode_t mode);				//return 0: success. -1:fail
void rltk_wlan_deinit(void);
void rltk_wlan_deinit_fastly(void);
int rltk_wlan_start(int idx_wlan);
void rltk_wlan_statistic(unsigned char idx);
unsigned char rltk_wlan_running(unsigned char idx);		// interface is up. 0: interface is down
int rltk_wlan_control(unsigned long cmd, void *data);
int rltk_wlan_handshake_done(void);
int rltk_wlan_rf_on(void);
int rltk_wlan_rf_off(void);
int rltk_wlan_check_bus(void);
int rltk_wlan_wireless_mode(unsigned char mode);
int rltk_wlan_get_wireless_mode(unsigned char *pmode);
int rltk_wlan_set_wpa_mode(const char *ifname, unsigned int wpa_mode);
int rltk_wlan_set_wps_phase(unsigned char is_trigger_wps);
int rtw_ps_enable(int enable);
int rltk_wlan_is_connected_to_ap(void);
void rltk_wlan_btcoex_set_bt_state(unsigned char state);
int rltk_wlan_change_channel_plan(unsigned char channel_plan);
int rltk_get_tx_pause(unsigned char *pause);
int rltk_set_tx_pause(unsigned char pause);

#ifdef CONFIG_IEEE80211W
void rltk_wlan_tx_sa_query(unsigned char key_type);
void rltk_wlan_tx_deauth(unsigned char b_broadcast, unsigned char key_type);
void rltk_wlan_tx_auth(void);
#endif

#ifdef	__cplusplus
}
#endif



#endif //#ifndef __WLAN_INTF_H__
