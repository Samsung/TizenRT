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

#include "rtos_ext.h"

#define port_enter_critical   rtos_enter_critical
#define port_exit_critical    rtos_exit_critical

bk_err_t rtos_init_event_ex(rtos_event_ext_t * event)
{
	if(event->event_inited != 0)
		return BK_OK;

	event->event_flag = 0;
	
	bk_err_t  result = rtos_init_semaphore(&event->event_semaphore, 1);
	
	if(result != BK_OK)
	{
		return result;
	}
	
	event->event_inited = 1;

	return BK_OK;
}

/* this API may be called from ISR. */
bk_err_t rtos_set_event_ex(rtos_event_ext_t * event, u32 event_flag)
{
	u32  int_mask;

	if(event->event_inited == 0)
		return BK_ERR_NOT_INIT;

	if(event_flag == 0)
		return BK_OK;

	int_mask = port_enter_critical();

	event->event_flag |= event_flag;

	port_exit_critical(int_mask);

	rtos_set_semaphore(&event->event_semaphore);

	return BK_OK;
}

/* support only one task to wait on the event. */
u32 rtos_wait_event_ex(rtos_event_ext_t * event, u32 event_flag, u32 any_event, u32 timeout)
{
	if(event->event_inited == 0)
		return 0;

	if(event_flag == 0)
		return 0;

	u32  int_mask;
	u32  ret_flag;

	u32  start_time = rtos_get_time();
	u32  cur_time;
	u32  elapsed_time = 0;

	bk_err_t  result;

	while(1)
	{
		int_mask = port_enter_critical();

		ret_flag = event->event_flag;

		if(any_event)
		{
			ret_flag &= event_flag;
		}
		else  // all events required.
		{
			if((ret_flag & event_flag) == event_flag)
			{
				ret_flag = event_flag;
			}
			else
			{
				ret_flag = 0;
			}
		}

		event->event_flag ^= ret_flag;  // clear read events.

		port_exit_critical(int_mask);

		if(ret_flag != 0)
		{
			return ret_flag;
		}

		if(timeout != BEKEN_WAIT_FOREVER)
		{
			cur_time = rtos_get_time();
			if(cur_time < start_time)
			{
				elapsed_time = (~cur_time) + 1 + start_time;
			}
			else
			{
				elapsed_time = cur_time - start_time;
			}
		}
		else
		{
			elapsed_time = 0;
		}

		if(elapsed_time >= timeout)
		{
			return 0;
		}
		else
		{
			result = rtos_get_semaphore(&event->event_semaphore, timeout - elapsed_time);

			if(result != BK_OK)
				return 0;
		}
	}

	return 0;

}

#define RTOS_HISR_MAX		32

static beken_semaphore_t   hisr_notify_sema;
static u8				   hisr_event_grp[(RTOS_HISR_MAX + 7) / 8];		// 64 interrupts.
static rtos_hisr_cb_t	  *hisr_list[RTOS_HISR_MAX];
static u8				   cur_hisr_id;   // for debug.

static u8 get_hisr_event_id(void)
{
	u8		event_id = 0xFF;
	u8		i, j;
	u32     int_mask;

	while(1)
	{
		event_id = 0xFF;

		int_mask = port_enter_critical();

		for(i = 0; i < ARRAY_SIZE(hisr_event_grp); i++)
		{
			if(hisr_event_grp[i] != 0)
				break;
		}

		if(i < ARRAY_SIZE(hisr_event_grp))
		{
			for(j = 0; j < 8; j++)
			{
				if(hisr_event_grp[i] & (0x01 << j))
					break;
			}

			hisr_event_grp[i] &= ~(0x01 << j);

			event_id = (i << 3) + j;
		}
		
		port_exit_critical(int_mask);

		if(event_id < RTOS_HISR_MAX)
			return event_id;

		rtos_get_semaphore(&hisr_notify_sema, BEKEN_WAIT_FOREVER);
		
	}

}

//void rtos_hisr_handler( void *param )
void rtos_hisr_task( void *param )
{
	u8  evt_id;

	// task init
	rtos_init_semaphore(&hisr_notify_sema, 1);

	while(1)
	{
		evt_id = get_hisr_event_id();

		cur_hisr_id = hisr_list[evt_id]->hisr_id;

		hisr_list[evt_id]->hisr(hisr_list[evt_id]->hisr_param);

		cur_hisr_id = -1;
	}
}

bk_err_t rtos_activate_hisr(rtos_hisr_cb_t *hisr_cb)
{
	if(hisr_cb->inited == 0)
	{
		return BK_FAIL;
	}

	if(hisr_cb->event_id >= RTOS_HISR_MAX)
	{
		return BK_FAIL;
	}

	u32 int_mask = port_enter_critical();
	hisr_event_grp[hisr_cb->event_id >> 3] |= 0x01 << (hisr_cb->event_id & 7);
	port_exit_critical(int_mask);

	rtos_set_semaphore(&hisr_notify_sema);

	return BK_OK;
}

bk_err_t rtos_create_hisr(rtos_hisr_cb_t *hisr_cb, high_isr_t hisr, void *param, u32 hisr_id)
{
	u8		i;

	for(i = 0; i < RTOS_HISR_MAX; i++)
	{
		if(hisr_list[i] == NULL)
			break;
		else 
		{
			if((hisr_list[i]->hisr == hisr) && (hisr_list[i]->hisr_param == param))
				return BK_FAIL;
		}
	}

	if(i >= RTOS_HISR_MAX)
		return BK_FAIL;
	
	hisr_cb->hisr = hisr;
	hisr_cb->hisr_param = param;
	hisr_cb->hisr_id = hisr_id;
	hisr_cb->event_id = i;
	hisr_cb->inited = 1;

	hisr_list[hisr_cb->event_id] = hisr_cb;

	return BK_OK;
}

