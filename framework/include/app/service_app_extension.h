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


#ifndef __TIZEN_APPFW_SERVICE_APP_EXTENSION_H__
#define __TIZEN_APPFW_SERVICE_APP_EXTENSION_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_SERVICE_APP_MODULE
 * @{
 */

/**
 * @brief Exits the main loop of the application without restart.
 *
 * @details The main loop of the application stops, service_app_terminate_cb() is invoked, and the a
 * pplication is not restarted.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @see service_app_main()
 * @see service_app_exit()
 * @see service_app_terminate_cb()
 */
void service_app_exit_without_restart(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_SERVICE_APP_EXTENSION_H__ */
