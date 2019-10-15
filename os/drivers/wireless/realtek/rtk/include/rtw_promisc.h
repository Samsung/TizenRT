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

#ifndef _RTW_PROMISC_H_
#define _RTW_PROMISC_H_
#include <drv_types.h>
#ifdef CONFIG_PROMISC
void promisc_deinit(_adapter *padapter);
//void promisc_set_enable(_adapter *padapter, u8 enabled, u8 len_used);
int promisc_recv_func(_adapter *padapter, union recv_frame *rframe);
int promisc_recv_lens_func(void *padapter, u8 *payload, u8 plen);
int promisc_filter_with_len(u16 len);
#endif
int promisc_set(rtw_rcr_level_t enabled, void (*callback)(unsigned char *, unsigned int, void *), unsigned char len_used);
unsigned char is_promisc_enabled(void);
int promisc_get_fixed_channel(void *fixed_bssid, u8 *ssid, int *ssid_length);
void promisc_issue_probereq(void);
void promisc_issue_probersp(unsigned char *da);
void promisc_stop_tx_beacn(void);
void promisc_resume_tx_beacn(void);
void promisc_get_ap_info(rtw_result_t (*func)(char *ssid, u8 ssid_len, s16 rssi, char channel, char security));
#endif //_RTW_PROMISC_H_
