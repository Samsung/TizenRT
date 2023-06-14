/**
****************************************************************************************************
*               Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
****************************************************************************************************
* @file      kv.h
* @brief     key-value pair layer wraps littlefs or Fatfs for flash read/write
* @author    liang_cai
* @date      2022-6-20
* @version   v1.0
* **************************************************************************************************
*/

#ifndef _KV_H_
#define _KV_H_

#include <stdint.h>

#ifdef  __cplusplus
extern  "C" {
#endif  // __cplusplus

#define MAX_KEY_LENGTH 128

/*============================================================================*
  *                                Functions
  *============================================================================*/
int rt_kv_init(void);
int32_t rt_kv_set(const char *key, const void *val, int32_t len);
int32_t rt_kv_get(const char *key, void *buffer, int32_t len);
int32_t rt_kv_delete(const char *key);

/** @} */ /* End of group KV */

#ifdef  __cplusplus
}
#endif // __cplusplus

#endif // _KV_H_
