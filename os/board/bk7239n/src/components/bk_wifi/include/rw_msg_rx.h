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

#ifndef _RW_MSG_RX_H_
#define _RW_MSG_RX_H_
#if CONFIG_AP_PROBEREQ_CB
#include "bk_rw.h"
#endif

#if CONFIG_RWNX_MULTIPLE_SSID_STA_MODE
#define GENMASK_ULL(h, l) (((~0ULL) << (l)) & (~0ULL >> (64 - 1 - (h))))
#endif

extern void mr_kmsg_init(void);
extern void rwnx_recv_msg(void);
extern void mhdr_assoc_cfm_cb(FUNC_2PARAM_PTR ind_cb, void *ctxt);
extern void mhdr_scanu_reg_cb(FUNC_2PARAM_PTR ind_cb, void *ctxt);
extern void mhdr_deauth_evt_cb(FUNC_2PARAM_PTR ind_cb, void *ctxt);
extern void mhdr_deassoc_evt_cb(FUNC_2PARAM_PTR ind_cb, void *ctxt);
extern void mhdr_scanu_reg_cb_for_wpa(FUNC_2PARAM_PTR ind_cb, void *ctxt);
extern uint32_t sr_get_scan_number(void);
#if CONFIG_AP_PROBEREQ_CB
extern void ap_vif_probe_req_frame_cb_hander(unsigned char *data, unsigned int length);
extern void ap_vif_probe_req_frame_cb_register(apvif_probereq_frame_t cb);
#endif
#endif // _RW_MSG_RX_H_
// eof

