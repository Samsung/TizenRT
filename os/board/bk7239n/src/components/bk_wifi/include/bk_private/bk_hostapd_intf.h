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

int hapd_intf_ioctl(unsigned long arg);
int hapd_intf_ke_rx_handle(INT32 dummy);
int hapd_intf_set_ap(void *beacon, int bcn_len, int head_len);
void wpa_buffer_scan_results(void);
void wpa_clear_scan_results(void);
void wpa_enable_traffic_port_at_opensystem(void);
typedef void (*bk_ap_no_password_cb_t)(void);
void bk_ap_no_password_connected_register_cb(bk_ap_no_password_cb_t func);
void wpa_hostapd_release_scan_rst(void);

#ifdef __cplusplus
}
#endif
