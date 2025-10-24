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


#ifndef _rtos_ext_h_
#define _rtos_ext_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <common/bk_typedef.h>
#include <os/os.h>

typedef struct
{
	beken_semaphore_t   event_semaphore;  // will release from ISR.
	u32       event_flag;
	u32       event_inited;
} rtos_event_ext_t;

bk_err_t rtos_init_event_ex(rtos_event_ext_t * event);


/* this API may be called from ISR. */
bk_err_t rtos_set_event_ex(rtos_event_ext_t * event, u32 event_flag);


/* support only one task to wait on the event. */
u32 rtos_wait_event_ex(rtos_event_ext_t * event, u32 event_flag, u32 any_event, u32 timeout);


typedef void (* high_isr_t)(void * param);

typedef struct
{
	high_isr_t		hisr;
	void		  * hisr_param;
	u8				event_id;
	u8				inited;
	u8				hisr_id;
} rtos_hisr_cb_t;

//void rtos_hisr_handler( void *param )
void rtos_hisr_task( void *param );

/*
  * activate the HISR to be called in task context, to process bussiness can't be done in ISR.
  * input:
  *     hisr_cb  : hisr control block, initialized in rtos_create_hisr.
  *  
  * return:
  *     succeed : BK_OK;
  *     failed  : BK_FAIL, maybe hisr_cb has not been created.
  *
  */

bk_err_t rtos_activate_hisr(rtos_hisr_cb_t *hisr_cb);

/*
  * create the hisr control block.
  * input:
  *     hisr_cb  : hisr control block, will be initialized with following parameters.
  *     hisr     : callback function, runs in task context to process anything can't be done in interrupt context.
  *     param    : parameter to hisr callback.
  *     hisr_id  : function Id for debug purpose, maybe the IRQ number of the interrupt.
  *
  * return:
  *     succeed : BK_OK;
  *     failed  : BK_FAIL, maybe the same hisr & param pair have been registered in system,
  *                        or no resource for this hisr registery.
  *
  */

bk_err_t rtos_create_hisr(rtos_hisr_cb_t *hisr_cb, high_isr_t hisr, void *param, u32 hisr_id);

#ifdef __cplusplus
}
#endif

#endif /* _rtos_ext_h_ */


