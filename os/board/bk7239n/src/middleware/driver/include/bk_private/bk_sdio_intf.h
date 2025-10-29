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

#include "bk_sdio.h"
//#include "ke_msg.h"
//#include "tx_swdesc.h"

#define SDIO_INTF_FAILURE        ((UINT32)-1)
#define SDIO_INTF_SUCCESS        (0)

/*******************************************************************************
* Function Declarations
*******************************************************************************/
UINT32 sdio_intf_init(void);
void sdio_emb_rxed_evt(int dummy);
#if (defined(CONFIG_FULLY_HOSTED) || defined(CONFIG_SEMI_HOSTED))
UINT32 outbound_upload_data(RW_RXIFO_PTR rx_info);
extern void inbound_cfm(void *cfm);
#else
UINT32 outbound_upload_data(UINT8 *buf_ptr, UINT32 len);
void inbound_cfm(void);
#endif
void sdio_emb_rxed_evt(int dummy);
//UINT32 sdio_emb_kmsg_fwd(struct ke_msg *msg); TODO fix it
void sdio_trans_evt(int dummy);
int sdio_trans_init(void);

#ifdef __cplusplus
}
#endif
