/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __BT_ADAPTATION_COMMON_H__
#define __BT_ADAPTATION_COMMON_H__

#include <bluetooth/bluetooth_type.h>
#include <bluetooth/bluetooth_type_internal.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define BT_UUID_STR_MAX 50
#define BT_ADDR_STR_MAX 18
#define BT_ADDR_HEX_LEN 6

int bt_adapt_init(void);

int bt_adapt_deinit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __BT_ADAPTATION_COMMON_H__ */
