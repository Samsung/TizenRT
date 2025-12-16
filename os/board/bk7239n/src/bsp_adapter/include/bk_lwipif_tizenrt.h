// Copyright 2025 Beken
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

#define BK_SS_WIFI_DP 0
#define MAX_ETH_MTU_SIZE CONFIG_NET_ETH_MTU

struct pbuf *bk_wlan_buf_alloc(uint32_t len);
err_t bk_wlan_txdata_send(uint8_t * data,uint16_t len, uint32_t vif_idx);
void bk_wlan_buf_free(struct pbuf *p);