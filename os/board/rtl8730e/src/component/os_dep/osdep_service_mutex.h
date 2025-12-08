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

#ifndef __OSDEP_SERVICE_MUTEX_H_
#define __OSDEP_SERVICE_MUTEX_H_

#if( configSUPPORT_STATIC_ALLOCATION == 1 )
#ifdef CONFIG_WLAN
#define MAX_MUTEX_BUF_NUM		109
#else
#define MAX_MUTEX_BUF_NUM		5
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
typedef void	            *_mutex;

/*************************** Mutexes *******************************/
/**
 * @brief  This function implements a mutex semaphore by using the existing queue mechanism.
 * @param[in] pmutex: Pointer to the created mutex semaphore.
 * @return	  None
 */
void	rtw_mutex_init(_mutex *pmutex);

/**
 * @brief  This function deletes the mutex semaphore.
 * @param[in] pmutex: Pointer to the mutex semaphore to be deleted.
 * @return	  None
 */
void	rtw_mutex_free(_mutex *pmutex);

/**
 * @brief  This function releases a mutex semaphore.
 * @param[in] pmutex: Pointer to the mutex semaphore to be released.
 * @return	  None
 */
void	rtw_mutex_put(_mutex *pmutex);

/**
 * @brief  This function obtains a mutex semaphore.
 * @param[in] pmutex: Pointer to the mutex semaphore being taken - obtained when
 *			  the mutex semaphore was created.
 * @return	  None
 */
void	rtw_mutex_get(_mutex *pmutex);

/**
 * @brief  This function obtains a mutex semaphore with a timeout setting.
 * @param[in] pmutex: Pointer to the mutex semaphore being taken - obtained when
 *			  the mutex semaphore was created.
 * @param[in] timeout: The time in ms to wait for the semaphore to become available.
 * @return	0: The semaphore was obtained.
 * @return	-1: Timeout without the semaphore becoming available.
 */
int	rtw_mutex_get_timeout(_mutex *pmutex, uint32_t timeout_ms);
/*************************** End Mutexes *******************************/
#ifdef __cplusplus
}
#endif
#if( configSUPPORT_STATIC_ALLOCATION == 1 )

extern struct list_head wrapper_mutexbuf_list;
extern int mutexbuf_used_num, mutex_dynamic_num;
extern int max_mutexbuf_used_num;
extern int mutexpool_flag;

void *get_mutex_from_poll(struct list_head *phead, int *count);
void release_mutex_to_poll(unsigned char *buf, struct list_head *phead, int *count);
u8 rtw_get_mutex_dynamic_flag(_mutex sema);
#endif
void init_mutex_pool(void);

#endif
