/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef _SECURE_STORAGE_DEVICE_H_
#define _SECURE_STORAGE_DEVICE_H_

/**
 * Callback to read device unique ID.
 *
 * @param    p_id_buf            [out] ID buffer
 * @param   p_id_buf_size      [in] byte length of p_id_buf size
 * @param    p_id_out_len        [out] Length of CPU ID
 * @return    SS_SUCCESS        if no error is occured
 */
typedef int (*get_device_unique_id)(unsigned char *p_id_buf, size_t p_id_buf_size, unsigned int *p_id_out_len);

/**
 * Set callback implementation to read device unique ID.
 * It will be invoked when generate master key of secure storage
 *
 * @param get_id_cb callback implementation to read device unique ID.
 */
extern void ss_set_device_unique_id_cb(get_device_unique_id get_id_cb);

/**
 * @brief    API to read device unique ID
 *
 * @param    p_id                [out] Device unique ID
 * @param   p_id_buf_size      [in] byte length of p_id size
 * @param    p_id_len            [out] Length of ID
 * @return    SS_SUCCESS        if no error is occured
 */
extern int ss_get_device_id(unsigned char *p_id, size_t p_id_buf_size, unsigned int *p_id_len);

#endif	/* 
 */
