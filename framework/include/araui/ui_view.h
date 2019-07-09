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
 * @defgroup VIEW View
 * @ingroup ARAUI
 * @{
 */

#ifndef __UI_VIEW_H__
#define __UI_VIEW_H__

#include <araui/ui_commons.h>
#include <araui/ui_widget.h>

/**
 * @brief View Handle
 *
 * @see ui_view_create()
 * @see ui_view_destroy()
 */
typedef long ui_view_t;

/**
 * @brief Callback function type which is called when view is created successfully
 *
 * @see ui_view_create()
 */
typedef void (*created_callback)(ui_view_t view);

/**
 * @brief Callback function type which is called when view is destroyed successfully
 *
 * @see ui_view_create()
 */
typedef void (*destroyed_callback)(ui_view_t view);

/**
 * @brief Callback function type which is called when view is shown successfully
 *
 * @see ui_view_create()
 */
typedef void (*shown_callback)(ui_view_t view);

/**
 * @brief Callback function type which is called when view is hidden successfully
 *
 * @see ui_view_create()
 */
typedef void (*hidden_callback)(ui_view_t view);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Register the view.
 *
 * @param[in] created_cb Callback function which is called when view is created successfully
 * @param[in] destroyed_cb Callback function which is called when view is destroyed successfully
 * @param[in] shown_cb Callback function which is called when view is shown successfully
 * @param[in] hidden_cb Callback function which is called when view is hidden successfully
 * @return On success, the view handle is returned. On failure, UI_NULL is returned.
 *
 * @see registered_callback
 */
ui_view_t ui_view_create(created_callback created_cb, destroyed_callback destroyed_cb, shown_callback shown_cb, hidden_callback hidden_cb);

/**
 * @brief Unregister the registered view.
 *
 * @param[in] view Handle of the registered view
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_view_create()
 */
ui_error_t ui_view_destroy(ui_view_t view);

/**
 * @brief Register the view.
 *
 * @param[in] view Handle of the registered view
 * @return On success, the handle of widget is returned. On failure, UI_NULL is returned.
 *
 * @see registered_callback
 */
ui_widget_t ui_view_get_root(ui_view_t view);

/**
 * @brief Add an empty widget to the view.
 *
 * As widgets are organized in the form of tree, they are organized into parent-child relationships.
 * Therefore, if the parent widget is moved, child widgets are also moved correspondingly.
 * Furthermore, if the parent widget is removed (@ref ui_view_remove_widget), child widgets are also be removed automatically.
 * Accordingly the coordinates of the child widgets are the relative coordinate to the parent widget.
 *
 * @param[in] view Handle of the view which the widget will be added to
 * @param[in] widget Handle of the widget which wiil be added to the view
 * @param[in] x x coordinate of the widget. Relative coordinate to the parent widget
 * @param[in] y y coordinate of the widget. Relative coordinate to the parent widget
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 */
ui_error_t ui_view_add_widget(ui_view_t view, ui_widget_t widget, int32_t x, int32_t y);

/**
 * @brief Transit the view to the next view from the current.
 *
 * @param[in] next_view Handle of the view which transit to
 * @param[in] type Type of the transition effect
 * @param[in] duration Duration of the transition effect (msec)
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_transit_type_t
 */
ui_error_t ui_view_transit(ui_view_t next_view, ui_transit_type_t type, uint32_t duration);

#ifdef __cplusplus
}
#endif

#endif

/** @} */ // end of VIEW group
