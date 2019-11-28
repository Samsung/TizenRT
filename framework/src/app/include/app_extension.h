/*
 * Copyright (c) 2011 - 2016 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_APPFW_APP_EXTENSION_H__
#define __TIZEN_APPFW_APP_EXTENSION_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	Gets the preinitialized window object.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks	This API only supports BASIC type window.
 *
 * @param[in] win_name The name to be set for the preinitialized window
 *
 * @return	A @a window object on success,
 *		otherwise @c NULL
 */
void *app_get_preinitialized_window(const char *win_name);

/**
 * @brief	Gets the preinitialized background object added to the preinitialized window.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks	This API should be called after calling app_get_preinitizlized_window().
 *
 * @return	A @a background object on success,
 *		otherwise @c NULL
 */
void *app_get_preinitialized_background(void);

/**
 * @brief	Gets the preinitialized conformant widget added to the preinitialized window.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @remarks	This API should be called after calling app_get_preinitizlized_window().
 *
 * @return	A conformant object on success,
 *		otherwise @c NULL
 */
void *app_get_preinitialized_conformant(void);

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_APP_EXTENSION_H__ */

