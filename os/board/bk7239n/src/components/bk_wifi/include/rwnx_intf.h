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

////#include <os/os.h>

#if (CONFIG_SOC_BK7256XX)
#define __DTCM_RWNX_IF __attribute__((section(".dtcm_sec_data ")))
#else
#define __DTCM_RWNX_IF
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "generated/lmac_bus_msg.h"

#define CORE_QITEM_COUNT          (64)

typedef struct _wifi_core_ {
	uint32_t queue_item_count;
	void *io_queue;

	void *handle;
	uint32_t stack_size;
} WIFI_CORE_T;

struct sk_buff;
struct pbuf;

int bmsg_is_empty(void);
void core_thread_uninit(void);
int bmsg_tx_raw_cb_sender(uint8_t *buffer, int length, void *cb, void *param);
int bmsg_tx_mgmt_sender(struct sk_buff *skb);
int bmsg_tx_sender_ext(struct pbuf *p, uint32_t vif_idx, void *cb, void *param);
int bmsg_special_tx_sender_ext(struct pbuf *p, uint32_t vif_idx, void *cb, void *param);
int rwnx_intf_init(void);
int rwnx_intf_deinit(void);

#ifdef __cplusplus
}
#endif

