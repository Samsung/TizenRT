// Copyright 2020-2025 Beken
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

#include "bk_wifi.h"
#include "os/os.h"

#define wifi_lock() \
	do{\
		GLOBAL_INT_DECLARATION();\
		GLOBAL_INT_DISABLE();
#define wifi_unlock() \
		GLOBAL_INT_RESTORE();\
	}while(0);

// bk_wifi_init() set the bit, bk_wifi_deinit() clear the bit
#define WIFI_INIT_BIT               (1)

// bk_wifi_sta_start() set the bit, bk_wifi_sta_stop() clear the bit
#define WIFI_STA_STARTED_BIT        (1<<1)
#define WIFI_STA_CONFIGURED_BIT     (1<<2)

// bk_wifi_sta_connect() set the bit, bk_wifi_sta_disconnect() clear the bit
// It doesn't indicate the STA is connected, it only means the API is called
// or NOT.
#define WIFI_STA_CONNECTED_BIT      (1<<3)

// bk_wifi_ap_start() set the bit, bk_wifi_ap_stop() clear the bit
#define WIFI_AP_STARTED_BIT         (1<<4)
#define WIFI_AP_CONFIGURED_BIT      (1<<5)
#define WIFI_MONITOR_STARTED_BIT    (1<<6)
#define WIFI_PURE_SCAN_STARTED_BIT  (1<<7)
#define WIFI_VIDEO_TRANSFER_STARTED_BIT  (1<<8)

#define WIFI_RESERVED_BYTE_VALUE    0

#define ENC_METHOD_NULL             1
#define ENC_METHOD_XOR              2
#define ENC_METHOD_AES              3

#define WIFI_TAG "wifi"
#define WIFI_LOGI(...) BK_WIFI_LOGI(WIFI_TAG, ##__VA_ARGS__)
#define WIFI_LOGW(...) BK_WIFI_LOGW(WIFI_TAG, ##__VA_ARGS__)
#define WIFI_LOGE(...) BK_WIFI_LOGE(WIFI_TAG, ##__VA_ARGS__)
#define WIFI_LOGD(...) BK_WIFI_LOGD(WIFI_TAG, ##__VA_ARGS__)
#define WIFI_LOG_RAW(...) BK_WIFI_LOG_RAW(WIFI_TAG, ##__VA_ARGS__)

#define WIFI_VIDEO_TRANSFER_TCP_RTO     1

const char *wifi_sec_type_string(wifi_security_t security);
void wifi_sta_reg_bcn_cb(void);
void bk_wlan_set_coexist_at_init_phase(uint8_t current_role);
void bk_wifi_media_dtim(void);
bool wifi_sta_is_started(void);
bool wifi_ap_is_started(void);
void bk_wifi_ota_dtim(bool is_open);
void rwnx_lowvol_enter_callback(void);
void rwnx_lowvol_exit_callback(void);

#ifdef __cplusplus
}
#endif
