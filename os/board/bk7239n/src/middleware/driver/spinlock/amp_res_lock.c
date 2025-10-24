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

#include <stdio.h>
#include <string.h>

#include <os/os.h>
#include <driver/mailbox_channel.h>
#include "mb_ipc_cmd.h"
#include "amp_res_lock.h"
#include "amp_lock_api.h"

#define AMP_CPU_CNT		SYSTEM_CPU_NUM

typedef struct
{
	u8					inited;
	beken_semaphore_t	res_sema;

#ifdef AMP_RES_SERVER
	u16		req_cnt[AMP_CPU_CNT];
#endif

} amp_res_sync_t;

static amp_res_sync_t	amp_res_sync[AMP_RES_ID_MAX];

#ifdef CONFIG_FREERTOS_SMP
#include "spinlock.h"
static volatile spinlock_t amp_spin_lock = SPIN_LOCK_INIT;
#endif // CONFIG_FREERTOS_SMP
static inline uint32_t amp_enter_critical()
{
	uint32_t flags = rtos_disable_int();

#ifdef CONFIG_FREERTOS_SMP
	spin_lock(&amp_spin_lock);
#endif // CONFIG_FREERTOS_SMP

	return flags;
}

static inline void amp_exit_critical(uint32_t flags)
{
#ifdef CONFIG_FREERTOS_SMP
	spin_unlock(&amp_spin_lock);
#endif // CONFIG_FREERTOS_SMP

	rtos_enable_int(flags);
}

#if (CONFIG_CPU_CNT > 1)
#ifdef AMP_RES_SERVER
/* call this API in interrupt disabled state. */
bk_err_t amp_res_acquire_cnt(u16 res_id, u16 cpu_id, amp_res_req_cnt_t *cnt_list)
{
	if(res_id >= AMP_RES_ID_MAX)
		return BK_ERR_PARAM;

	if(amp_res_sync[res_id].inited == 0)
		return BK_ERR_NOT_INIT;

	if(cpu_id >= AMP_CPU_CNT)
		return BK_ERR_PARAM;

	u16		i = 0;
	u16 	all_cnt = 0;
	
	for(i = 0; i < AMP_CPU_CNT; i++)
	{
		all_cnt += amp_res_sync[res_id].req_cnt[i];
	}

	cnt_list->self_req_cnt = amp_res_sync[res_id].req_cnt[cpu_id];
	cnt_list->others_req_cnt = all_cnt - cnt_list->self_req_cnt;

	amp_res_sync[res_id].req_cnt[cpu_id]++;

	return BK_OK;
	
}

/* call this API in interrupt disabled state. */
bk_err_t amp_res_release_cnt(u16 res_id, u16 cpu_id, amp_res_req_cnt_t *cnt_list)
{
	if(res_id >= AMP_RES_ID_MAX)
		return BK_ERR_PARAM;

	if(amp_res_sync[res_id].inited == 0)
		return BK_ERR_NOT_INIT;

	if(cpu_id >= AMP_CPU_CNT)
		return BK_ERR_PARAM;

	u16		i = 0;

	if(amp_res_sync[res_id].req_cnt[cpu_id] > 0)
	{
		amp_res_sync[res_id].req_cnt[cpu_id]--;

		u16 	all_cnt = 0;

		for(i = 0; i < AMP_CPU_CNT; i++)
		{
			all_cnt += amp_res_sync[res_id].req_cnt[i];
		}

		cnt_list->self_req_cnt = amp_res_sync[res_id].req_cnt[cpu_id];
		cnt_list->others_req_cnt = all_cnt - cnt_list->self_req_cnt;

		return BK_OK;
	}
	else
	{
		return BK_FAIL;
	}
	
}

#endif

/* Apps can't call this API, it's for IPC isr only. */
bk_err_t amp_res_available(u16 res_id)
{
	if(res_id >= AMP_RES_ID_MAX)
		return BK_ERR_PARAM;

	if(amp_res_sync[res_id].inited == 0)
		return BK_ERR_NOT_INIT;

	return rtos_set_semaphore(&amp_res_sync[res_id].res_sema);
}

bk_err_t amp_res_lock_acquire(u16 res_id, u32 timeout_ms, const char * func_name, int line_no)
{
	bk_err_t	ret_val = BK_FAIL;

	if (!rtos_is_scheduler_started()) {
		return BK_OK;
	}

	if( rtos_is_in_interrupt_context() )
	{
		BK_LOGE("AMP", "can't call in ISR %s,%d\r\n", func_name, line_no);

		return BK_FAIL;
	}

	if(res_id >= AMP_RES_ID_MAX)
		return BK_ERR_PARAM;

	if(amp_res_sync[res_id].inited == 0)
		return BK_ERR_NOT_INIT;

	amp_res_req_cnt_t	cnt_list;

#ifdef AMP_RES_SERVER

	u32  int_mask = amp_enter_critical();

	ret_val = amp_res_acquire_cnt(res_id, SRC_CPU, &cnt_list);

	amp_exit_critical(int_mask);

#else

	ret_val = ipc_send_res_acquire_cnt(res_id, SRC_CPU, &cnt_list);

#endif

	if(ret_val != BK_OK)
	{
		return ret_val;
	}

	if((cnt_list.self_req_cnt == 0) && (cnt_list.others_req_cnt == 0))
	{
		/* resource is free, so set semaphore state to available. */
		ret_val = rtos_set_semaphore(&amp_res_sync[res_id].res_sema);

		if(ret_val != BK_OK)
		{
			return ret_val;
		}
	}

	ret_val = rtos_get_semaphore(&amp_res_sync[res_id].res_sema, timeout_ms);

	return ret_val;

}

bk_err_t amp_res_lock_release(u16 res_id, const char * func_name, int line_no)
{
	bk_err_t	ret_val = BK_FAIL;

	if (!rtos_is_scheduler_started()) {
		return BK_OK;
	}

	if( rtos_is_in_interrupt_context() )
	{
		BK_LOGE("AMP", "can't call in ISR %s,%d\r\n", func_name, line_no);

		return BK_FAIL;
	}

	if(res_id >= AMP_RES_ID_MAX)
		return BK_ERR_PARAM;

	if(amp_res_sync[res_id].inited == 0)
		return BK_ERR_NOT_INIT;

	amp_res_req_cnt_t	cnt_list;

#ifdef AMP_RES_SERVER

	u32  int_mask = amp_enter_critical();

	ret_val = amp_res_release_cnt(res_id, SRC_CPU, &cnt_list);

	amp_exit_critical(int_mask);

#else

	ret_val = ipc_send_res_release_cnt(res_id, SRC_CPU, &cnt_list);

#endif

	if(ret_val != BK_OK)
	{
		return ret_val;
	}

	if(cnt_list.self_req_cnt > 0)
	{
		/* other CPU is waiting for the resource, so unblock the task. */
		ret_val = rtos_set_semaphore(&amp_res_sync[res_id].res_sema);
	}
	else if(cnt_list.others_req_cnt > 0)
	{
		/* other CPU is waiting for the resource, so inform CPU that it is available. */
		/* which CPU is selected in multi-cores? (over than 2 cores)*/
		ret_val = ipc_send_available_ind(res_id);
	}

	if(ret_val != BK_OK)
	{
		return ret_val;
	}

	return ret_val;

}

bk_err_t amp_res_lock_init(u16 res_id)
{
	bk_err_t	ret_val = BK_FAIL;

	if(res_id >= AMP_RES_ID_MAX)
		return BK_ERR_PARAM;

	if(amp_res_sync[res_id].inited != 0)
		return BK_OK;

#ifdef AMP_RES_SERVER

	u16 i = 0;

	for(i = 0; i < AMP_CPU_CNT; i++)
	{
		amp_res_sync[res_id].req_cnt[i] = 0;
	}

#endif

	ret_val = rtos_init_semaphore(&amp_res_sync[res_id].res_sema, 1);

	if(ret_val != BK_OK)
		return ret_val;

	amp_res_sync[res_id].inited = 1;

	return BK_OK;
}

bk_err_t amp_res_lock_reset_all(void)
{
#ifdef AMP_RES_SERVER

	u16 res_id = 0;

	for(res_id = 0; res_id < AMP_RES_ID_MAX; res_id++)
	{
		u16 i = 0;

		for(i = 0; i < AMP_CPU_CNT; i++)
		{
			amp_res_sync[res_id].req_cnt[i] = 0;
		}
	}

#endif

	return BK_OK;
}

#else

bk_err_t amp_res_lock_acquire(u16 res_id, u32 timeout_ms, const char * func_name, int line_no)
{
	bk_err_t	ret_val = BK_FAIL;

	if( rtos_is_in_interrupt_context() )
	{
		BK_LOGE("AMP", "can't call in ISR %s,%d\r\n", func_name, line_no);

		return BK_FAIL;
	}

	if(res_id >= AMP_RES_ID_MAX)
		return BK_ERR_PARAM;

	if(amp_res_sync[res_id].inited == 0)
		return BK_ERR_NOT_INIT;

	ret_val = rtos_get_semaphore(&amp_res_sync[res_id].res_sema, timeout_ms);

	return ret_val;

}

bk_err_t amp_res_lock_release(u16 res_id, const char * func_name, int line_no)
{
	bk_err_t	ret_val = BK_FAIL;

	if(res_id >= AMP_RES_ID_MAX)
		return BK_ERR_PARAM;

	if(amp_res_sync[res_id].inited == 0)
		return BK_ERR_NOT_INIT;

	ret_val = rtos_set_semaphore(&amp_res_sync[res_id].res_sema);

	return ret_val;

}

bk_err_t amp_res_lock_init(u16 res_id)
{
	bk_err_t	ret_val = BK_FAIL;

	if(res_id >= AMP_RES_ID_MAX)
		return BK_ERR_PARAM;

	if(amp_res_sync[res_id].inited != 0)
		return BK_OK;

	ret_val = rtos_init_semaphore_adv(&amp_res_sync[res_id].res_sema, 1, 1);

	if(ret_val != BK_OK)
		return ret_val;

	amp_res_sync[res_id].inited = 1;

	return BK_OK;
}

bk_err_t amp_res_lock_reset_all(void)
{
	return BK_OK;
}

#endif

