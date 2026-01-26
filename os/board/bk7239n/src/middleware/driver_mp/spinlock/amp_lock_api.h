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

#ifndef _amp_lock_api_h_
#define _amp_lock_api_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <common/bk_typedef.h>
#include <common/bk_err.h>

enum
{
	AMP_RES_ID_DMA = 0,
	AMP_RES_ID_GPIO,
	AMP_RES_ID_SYS_REG,

	AMP_RES_ID_MAX,
};

bk_err_t amp_res_lock_acquire(u16 res_id, u32 timeout_ms, const char * func_name, int line_no);
bk_err_t amp_res_lock_release(u16 res_id, const char * func_name, int line_no);
bk_err_t amp_res_lock_init(u16 res_id);
bk_err_t amp_res_lock_reset_all(void);

#if defined(CONFIG_MAILBOX)
#define amp_res_acquire(res_id, timeout_ms)  	amp_res_lock_acquire(res_id, timeout_ms, __FUNCTION__, __LINE__)
#define amp_res_release(res_id)					amp_res_lock_release(res_id, __FUNCTION__, __LINE__)
#define amp_res_init(res_id)					amp_res_lock_init(res_id)
#define amp_reset()								amp_res_lock_reset_all()
#else
#define amp_res_acquire(res_id, timeout_ms)  (BK_OK)
#define amp_res_release(res_id)              (BK_OK)
#define amp_res_init(res_id)
#define amp_reset()
#endif
#ifdef __cplusplus
}
#endif

#endif /* _amp_lock_api_h_ */

