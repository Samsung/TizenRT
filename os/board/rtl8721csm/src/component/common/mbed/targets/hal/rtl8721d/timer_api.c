 /** mbed Microcontroller Library
  ******************************************************************************
  * @file    timer_api.c
  * @author 
  * @version V1.0.0
  * @date    2016-08-01
  * @brief   This file provides mbed API for gtimer.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ****************************************************************************** 
  */

#include "objects.h"
#include "timer_api.h"

/** @defgroup AmebaD_Mbed_API 
  * @{
  */

/** @defgroup MBED_TIMER
 *  @brief      MBED_TIMER driver modules.
 *  @{
 */

/** @defgroup MBED_TIMER_Exported_Functions MBED_TIMER Exported Functions
  * @{
  */

/**
  * @brief  gtimer interrupt handler function.
  * @param  data: timer IRQ callback parameter
  * @retval   none
  */
static void gtimer_timeout_handler (uint32_t data)
{
	gtimer_t *obj = (gtimer_t *)data;
	uint32_t tid = obj->timer_id;
	gtimer_irq_handler handler;

	RTIM_INTClear(TIM_x[tid]);

	if (obj->handler != NULL) {
		handler = (gtimer_irq_handler)obj->handler;
		handler(obj->hid);
	}

	if (!obj->is_periodcal) {
		gtimer_stop(obj);
	}
}

/**
  * @brief  Initializes the timer device, include timer registers and interrupt.
  * @param  obj: timer object define in application software.
  * @param  tid: general timer ID, which can be one of the following parameters:
  *              @arg TIMER0
  *              @arg TIMER1
  *              @arg TIMER2
  *              @arg TIMER3
  * @note KM4 TIMER0/1/2/3 are recommended.
  * @retval none  
  */
void gtimer_init (gtimer_t *obj, uint32_t tid)
{
	RTIM_TimeBaseInitTypeDef TIM_InitStruct;

	assert_param(tid < GTIMER_MAX);

	obj->timer_id = tid;

	RTIM_TimeBaseStructInit(&TIM_InitStruct);
	TIM_InitStruct.TIM_Idx = (u8)tid;

	TIM_InitStruct.TIM_UpdateEvent = ENABLE; /* UEV enable */
	TIM_InitStruct.TIM_UpdateSource = TIM_UpdateSource_Overflow;
	TIM_InitStruct.TIM_ARRProtection = ENABLE;
#ifndef CONFIG_PLATFORM_TIZENRT_OS
	RTIM_TimeBaseInit(TIM_x[tid], &TIM_InitStruct, TIMx_irq[tid], (IRQ_FUN) gtimer_timeout_handler, (u32)obj);
#else
	RTIM_TimeBaseInit(TIM_x[tid], &TIM_InitStruct, TIMx_irq[tid], NULL, 0);
	InterruptDis(TIMx_irq[tid]);
	InterruptRegister((IRQ_FUN) gtimer_timeout_handler, TIMx_irq[tid], (u32)obj, 14);
	InterruptEn(TIMx_irq[tid], 14);
#endif
}

/**
  * @brief  Deinitializes the timer, including clock/function/timer registers.
  * @param  obj: gtimer object defined in application software.
  * @retval none  
  */
void gtimer_deinit (gtimer_t *obj)
{
	uint32_t tid = obj->timer_id;

	assert_param(tid < GTIMER_MAX);
	
	RTIM_DeInit(TIM_x[tid]);
}

/**
  * @brief  Get counter value of the specified timer.
  * @param  obj: timer object define in application software.
  * @retval value: counter value
  */
uint32_t gtimer_read_tick (gtimer_t *obj)
{
	uint32_t tid = obj->timer_id;

	assert_param(tid < GTIMER_MAX);
	
	return (RTIM_GetCount(TIM_x[tid]));
}

/**
  * @brief  Read current timer tick in microsecond.
  * @param  obj: gtimer object defined in application software.
  * @retval 64 bit tick time  
  */
uint64_t gtimer_read_us (gtimer_t *obj)  //need to be test in IAR(64bit computing)
{
	assert_param(obj->timer_id < GTIMER_MAX);
	
	uint64_t time_us;
	time_us = gtimer_read_tick(obj) *1000000 /32768;

	return (time_us);
}

/**
  * @brief  Change period of the specified timer.
  * @param  obj: timer object define in application software.
  * @param  duration_us: the new period to be set in microseconds.
  * @retval none
  */
void gtimer_reload (gtimer_t *obj, uint32_t duration_us)
{
	uint32_t tid = obj->timer_id;
	uint32_t temp = (uint32_t)((float)duration_us  / 1000000 *32768);

	assert_param(tid < GTIMER_MAX);
	
    	RTIM_ChangePeriodImmediate(TIM_x[tid], temp);
}

/**
  * @brief  Start the specified timer and enable update interrupt.
  * @param  obj: timer object define in application software.
  * @retval none
  */
void gtimer_start (gtimer_t *obj)
{
	uint32_t tid = obj->timer_id;

	assert_param(tid < GTIMER_MAX);

	RTIM_INTConfig(TIM_x[tid], TIM_IT_Update, ENABLE);
	RTIM_Cmd(TIM_x[tid], ENABLE);
}

/**
  * @brief Start the specified timer in one-shot mode with specified period and interrupt handler.
  * @param  obj: timer object define in application software.
  * @param  duration_us: the new period to be set in microseconds.
  * @param  handler: user defined IRQ callback function
  * @param  hid: user defined IRQ callback parameter
  * @retval none
  * @note This function set the timer into one-shot mode which stops after the first time the counter overflows.
  */
void gtimer_start_one_shout (gtimer_t *obj, uint32_t duration_us, void* handler, uint32_t hid)
{
	assert_param(obj->timer_id < GTIMER_MAX);

	obj->is_periodcal = _FALSE;
	obj->handler = handler;
	obj->hid = hid;

	gtimer_reload(obj, duration_us);
	gtimer_start(obj);
}

/**
  * @brief Start the specified timer in periodical mode with specified period and interrupt handler.
  * @param  obj: timer object define in application software.
  * @param  duration_us: the new period to be set in microseconds.
  * @param  handler: user defined IRQ callback function
  * @param  hid: user defined IRQ callback parameter
  * @retval none
  * @note This functon set the timer into periodical mode which will restart to count from 0 each time the counter overflows.
  */
void gtimer_start_periodical (gtimer_t *obj, uint32_t duration_us, void* handler, uint32_t hid)
{
	assert_param(obj->timer_id < GTIMER_MAX);

	obj->is_periodcal = _TRUE;
	obj->handler = handler;
	obj->hid = hid;

	gtimer_reload(obj, duration_us);
	gtimer_start(obj);
}

/**
  * @brief Disable the specified timer peripheral.
  * @param  obj: timer object define in application software.
  * @retval none
  */
void gtimer_stop (gtimer_t *obj)
{
	uint32_t tid = obj->timer_id;

	assert_param(tid < GTIMER_MAX);
	
	RTIM_Cmd(TIM_x[tid], DISABLE);    
}
/** 
  * @}
  */

/** 
  * @}
  */

/** 
  * @}
  */
