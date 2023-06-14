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

#ifndef __OSDEP_SERVICE_CRITICAL_H_
#define __OSDEP_SERVICE_CRITICAL_H_

#include "osdep_service_mutex.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef void	            *_lock;
typedef unsigned long		_irqL;

/*************************** SchedulerControl *******************************/
/**
 * @brief  This function marks the start of a critical code region.
 * 		   Preemptive context switches cannot occur when in a critical region.
 * @param[in] plock: Pointer to the spin lock semaphore.
 * @param[in] pirqL: Pointer to the IRQ.
 * @return	  None
 * @note: This may alter the stack (depending on the portable implementation)
 * so must be used with care!
 */
void	rtw_enter_critical(_lock *plock, _irqL *pirqL);

/**
 * @brief  This function marks end of a critical code region. Preemptive context
 * switches cannot occur when in a critical region.
 * @param[in] plock: Pointer to the spin lock semaphore.
 * @param[in] pirqL: Pointer to the IRQ.
 * @return	  None
 * @note: This may alter the stack (depending on the portable implementation)
 * so must be used with care!
 */
void	rtw_exit_critical(_lock *plock, _irqL *pirqL);

/**
 * @brief  This function obtains a spin lock semaphore.
 * @param[in] plock: Pointer to the spin lock semaphore being taken - obtained when
 *			  the mutex semaphore was created.
 * @param[in] pirqL: Pointer to the IRQ.
 * @return	  None
 */
void	rtw_enter_critical_bh(_lock *plock, _irqL *pirqL);

/**
 * @brief  This function releases a spin lock semaphore.
 * @param[in] plock: Pointer to the spin lock semaphore to be released.
 * @param[in] pirqL: Pointer to the IRQ.
 * @return	  None
 */
void	rtw_exit_critical_bh(_lock *plock, _irqL *pirqL);

/**
 * @brief  This function obtains a semaphore.
 * @param[in] pmutex: The handle to the mutex semaphore to be obtained.
 * @param[in] pirqL: Pointer to the IRQ.
 * @return	  None
 */
int		rtw_enter_critical_mutex(_mutex *pmutex, _irqL *pirqL);

/**
 * @brief  This function releases a semaphore.
 * @param[in] pmutex: The handle to the mutex semaphore to be released.
 * @param[in] pirqL: Pointer to the IRQ.
 * @return	  None
 */
void	rtw_exit_critical_mutex(_mutex *pmutex, _irqL *pirqL);

/**
 * @brief  This function will lock cpu. Can be used when xip active and want to do some flash operation.
 * @return	  None
 */
void	rtw_cpu_lock(void);

/**
* @brief  This function unlock cpu.
* @return	  None
*/
void	rtw_cpu_unlock(void);

/*************************** End SchedulerControl *******************************/

/*************************** Semaphores *******************************/

/**
 * @brief  This function implements a spin lock semaphore by using the existing queue mechanism.
 * @param[in] plock: Pointer to the created spin lock semaphore.
 * @return	  None
 */
void	rtw_spinlock_init(_lock *plock);

/**
 * @brief  This function deletes the spin lock semaphore.
 * @param[in] pmutex: Pointer to the spin lock semaphore to be deleted.
 * @return	  None
 */
void	rtw_spinlock_free(_lock *plock);

/**
 * @brief  This function obtains a spin lock semaphore.
 * @param[in] plock: Pointer to the spin lock semaphore being taken - obtained when
 *			  the mutex semaphore was created.
 * @return	  None
 */
void	rtw_spin_lock(_lock *plock);

/**
 * @brief  This function releases a spin lock semaphore.
 * @param[in] plock: Pointer to the spin lock semaphore to be released.
 * @return	  None
 */
void	rtw_spin_unlock(_lock *plock);

/*************************** End Semaphores *******************************/
#ifdef __cplusplus
}
#endif
// void save_and_cli(void);
// void restore_flags(void);
// void cli(void);
#endif
