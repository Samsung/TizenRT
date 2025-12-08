/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#ifndef __OSDEP_SERVICE_TIMER_H_
#define __OSDEP_SERVICE_TIMER_H_
#ifdef __cplusplus
extern "C" {
#endif
typedef void	            *_timerHandle;
typedef void (*TIMER_FUN)(void *context);

struct timer_list {
	struct list_head list;
	_timerHandle 	timer_hdl;
	unsigned long	data;
	unsigned char statically_alloc;	/*1: static allocate; 0: dynamic allocate */
	void (*function)(void *);
};

typedef struct timer_list	_timer;
#define rtw_timer_list timer_list

/*************************** Timers *******************************/

/**
 * @brief  This function creates a new software timer instance.
 * @param[in] pcTimerName:  A text name that is assigned to the timer.
 * @param[in] xTimerPeriodInTicks: The timer period which is defined in tick periods.
 * @param[in] uxAutoReload: If uxAutoReload is set to pdTRUE then the timer will
 * expire repeatedly with a frequency set by the xTimerPeriodInTicks parameter.  If
 * uxAutoReload is set to pdFALSE then the timer will be a one-shot timer and
 * enter the dormant state after it expires.
 * @param[in] pvTimerID: An identifier that is assigned to the timer being created.
 * @param[in] pxCallbackFunction: The function to call when the timer expires.
 * @return  If the timer is successfully create then a handle to the newly
 * created timer is returned.  If the timer cannot be created, then 0 is returned.
 */
_timerHandle rtw_timerCreate(const signed char *pcTimerName,
							 u32 xTimerPeriodInTicks,
							 uint32_t uxAutoReload,
							 void *pvTimerID,
							 TIMER_FUN pxCallbackFunction);

/**
 * @brief  This function deletes a timer that was previously created using rtw_timerCreate.
 * @param[in] xTimer:  The handle of the timer being deleted.
 * @param[in] xBlockTime: Specifies th etime, in ticks, that the calling task should be held in the Blocked
 *						  State to wait for the delete command to be successfully sent to the timer command queue,
 *						  should the queue already be full when rtw_timerDelete was called.
 * @return  pdFAIL will be returned if the delete command could not be sent to
 * the timer command queue even after xTicksToWait ticks had passed.  pdPASS will
 * be returned if the command was successfully sent to the timer command queue.
 * When the command is actually processed will depend on the priority of the
 * timer service/daemon task relative to other tasks in the system.
 */
uint32_t rtw_timerDelete(_timerHandle xTimer, u32 xBlockTime);

/**
 * @brief  This function queries a timer to see if it is active or dormant.
 * @param[in] xTimer:  The timer being queried.
 * @return  pdFALSE will be returned if the timer is dormant.  A value other than
 * pdFALSE will be returned if the timer is active.
 * @note  A timer will be dormant if:
 *     1) It has been created but not started, or
 *     2) It is an expired one-shot timer that has not been restarted.
 */
uint32_t rtw_timerIsTimerActive(_timerHandle xTimer);

/**
 * @brief  This function stops a timer that was previously started.
 * @param[in] xTimer:  The handle of the timer being stopped.
 * @param[in] xBlockTime:  Specifies the time, in ticks, that the calling task should
 * be held in the Blocked state to wait for the stop command to be successfully
 * sent to the timer command queue, should the queue already be full when
 * rtw_timerStop() was called.
 * @return  pdFAIL will be returned if the stop command could not be sent to
 * the timer command queue even after xTicksToWait ticks had passed.  pdPASS will
 * be returned if the command was successfully sent to the timer command queue.
 * When the command is actually processed will depend on the priority of the
 * timer service/daemon task relative to other tasks in the system.
 */
uint32_t rtw_timerStop(_timerHandle xTimer, u32 xBlockTime);

/**
 * @brief  This function changes the period of a timer that was previously created.
 * @param[in] xTimer:  The handle of the timer that is having its period changed.
 * @param[in] xNewPeriod:  The new period for xTimer.
 * @param[in] xBlockTime:  Specifies the time, in ticks, that the calling task should
 * be held in the Blocked state to wait for the change period command to be
 * successfully sent to the timer command queue, should the queue already be
 * full when rtw_timerChangePeriod() was called.
 * @return  pdFAIL will be returned if the change period command could not be
 * sent to the timer command queue even after xTicksToWait ticks had passed.
 * pdPASS will be returned if the command was successfully sent to the timer
 * command queue.  When the command is actually processed will depend on the
 * priority of the timer service/daemon task relative to other tasks in the
 * system.
 */
uint32_t rtw_timerChangePeriod(_timerHandle xTimer,
							   u32 xNewPeriod,
							   u32 xBlockTime);

#ifdef __cplusplus
}
#endif

/*************************** End Timers *******************************/
#endif
