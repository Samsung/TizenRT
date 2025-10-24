// Copyright 2020-2022 Beken
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

#ifndef _amp_res_lock_h_
#define _amp_res_lock_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <common/bk_typedef.h>
#include <common/bk_err.h>

#if (CONFIG_CPU_CNT > 1)

typedef struct
{
	u16		self_req_cnt;
	u16		others_req_cnt;
} amp_res_req_cnt_t;

#ifdef AMP_RES_SERVER

/* call this API in interrupt disabled state. */
bk_err_t amp_res_acquire_cnt(u16 res_id, u16 cpu_id, amp_res_req_cnt_t *cnt_list);

/* call this API in interrupt disabled state. */
bk_err_t amp_res_release_cnt(u16 res_id, u16 cpu_id, amp_res_req_cnt_t *cnt_list);

#endif

/* Apps can't call this API, it's for IPC isr only. */
bk_err_t amp_res_available(u16 res_id);

#endif

#ifdef __cplusplus
}
#endif

#endif /* _amp_res_lock_h_ */

