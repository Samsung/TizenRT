/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

/**
 * @defgroup ARAUI AraUI Framework
 * @{
 */
/**
 * @defgroup CORE Core
 * @ingroup ARAUI
 * @{
 */

#ifndef __UI_CORE_H__
#define __UI_CORE_H__

#include <araui/ui_commons.h>
#include <araui/ui_widget.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Start the AraUI Core Service.
 *
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 */
ui_error_t ui_start(void);

/**
 * @brief Stop the AraUI Core Service.
 *
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 */
ui_error_t ui_stop(void);

/**
 * @brief Set the quick panel to the certain event type.
 *
 * @param[in] event_type Type of the event to which the quick panel will be set.
 * @param[in] widget Handle of the quick panel.
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_quick_panel_event_type_t
 */
ui_error_t ui_core_set_quick_panel(ui_quick_panel_event_type_t event_type, ui_widget_t widget);

/**
 * @brief Unset the quick panel to the certain event type.
 *
 * @param[in] event_type Event type of the quick panel which want to be unset.
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_quick_panel_event_type_t
 */
ui_error_t ui_core_unset_quick_panel(ui_quick_panel_event_type_t event_type);

/**
 * @brief Make the quick panel of the certain event type appear.
 *
 * @param[in] event_type Event type of the quick panel which want to appear.
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_quick_panel_event_type_t
 */
ui_error_t ui_core_quick_panel_appear(ui_quick_panel_event_type_t event_type);

/**
 * @brief Make the quick panel of the certain event type disappear.
 *
 * @param[in] event_type Event type of the quick panel which want to disappear
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_quick_panel_event_type_t
 */
ui_error_t ui_core_quick_panel_disappear(ui_quick_panel_event_type_t event_type);

#ifdef __cplusplus
}
#endif

#endif

/** @} */ // end of CORE group

/** @} */ // end of ARAUI group
