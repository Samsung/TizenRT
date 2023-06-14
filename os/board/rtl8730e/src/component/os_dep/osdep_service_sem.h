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

#ifndef __OSDEP_SERVICE_SEM_H_
#define __OSDEP_SERVICE_SEM_H_
#ifdef __cplusplus
extern "C" {
#endif

#define RTW_MAX_DELAY			0xFFFFFFFF

typedef void	            *_sema;


/*************************** Semaphores *******************************/
/**
 * @brief  This function initializes the unnamed semaphore referred to by "sema" to the value "init_val".
 * @param[in] sema: Pointer to the semaphore handle to be initialized.
 * @param[in] init_val: Initial value for semaphore.
 * @return	  None
 */
void	rtw_init_sema(_sema *sema, int init_val);

/**
 * @brief  This function deletes the semaphore.
 * @param[in] sema: The semaphore to be deleted.
 * @return	  None
 */
void	rtw_free_sema(_sema *sema);

/**
 * @brief  This function releases the semaphore.
 *		   This macro must not be used from an ISR.
 * @param[in] sema: The semaphore to be released.
 * @return	  None
 */
void	rtw_up_sema(_sema	*sema);

/**
 * @brief  This function releases the semaphore.
 *		   This macro can be used from an ISR.
 * @param[in] sema: The semaphore to be released.
 * @return	  None
 */
void	rtw_up_sema_from_isr(_sema *sema);

/**
 * @brief  This function acquires the semaphore. If no more tasks are allowed to acquire the semaphore,
 *		   calling this function will put the task to sleep until the semaphore is up.
 * @param[in] sema: The semaphore to be acquired.
 * @return	pdTRUE: The semaphore was obtained.
 * @return	pdFALSE: Obtain the semaphore failed.
 */
uint32_t	rtw_down_sema(_sema *sema);

/**
 * @brief  This function acquires the semaphore. If no more tasks are allowed to acquire the semaphore,
 *		   calling this function will put the task to sleep until the semaphore is up.
 * @param[in] sema: The semaphore to be acquired.
 * @param[in] timeout: The time in ms to wait for the semaphore to become available.
 * @return	pdTRUE: The semaphore was obtained.
 * @return	pdFALSE: Timeout without the semaphore becoming available.
 */
uint32_t	rtw_down_timeout_sema(_sema *sema, uint32_t timeout);
/*************************** End Semaphores *******************************/
#ifdef __cplusplus
}
#endif

#endif
