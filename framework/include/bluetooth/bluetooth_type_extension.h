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


#ifndef __TIZEN_NETWORK_BLUETOOTH_TYPE_EXTENSION_H__
#define __TIZEN_NETWORK_BLUETOOTH_TYPE_EXTENSION_H__

 #ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @file bluetooth_type_extension.h
 */

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Called when the SCO(Synchronous Connection Oriented link) state is changed.
 * @since_tizen @if WEARABLE 2.3.1 @else 2.3 @endif
 *
 * @details  This callback is called when the SCO state is changed.
 * When you call bt_ag_open_sco() or bt_ag_close_sco(), this callback is also called with error result even though these functions failed.
 * @param[in] result  The result of changing the connection state
 * @param[in] opened  The state to be changed: (@c true = opened, @c  false = not opened)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_ag_set_sco_state_changed_cb()
 * @see bt_ag_unset_sco_state_changed_cb()
 * @see bt_ag_open_sco()
 * @see bt_ag_close_sco()
 */
typedef void (*bt_ag_sco_state_changed_cb) (int result, bool opened, void *user_data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_NETWORK_BLUETOOTH_TYPE_EXTENSION_H__ */
