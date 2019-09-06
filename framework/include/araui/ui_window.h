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
 * @defgroup WINDOW Window
 * @ingroup ARAUI
 * @{
 */

#ifndef __UI_WINDOW_H__
#define __UI_WINDOW_H__

#include <araui/ui_commons.h>
#include <araui/ui_widget.h>

/**
 * @brief Window Handle
 *
 * @see ui_window_create()
 * @see ui_window_destroy()
 */
typedef long ui_window_t;

/**
 * @brief Callback function type which is called when window is created successfully
 *
 * @see ui_window_create()
 */
typedef void (*created_callback)(ui_window_t window);

/**
 * @brief Callback function type which is called when window is destroyed successfully
 *
 * @see ui_window_create()
 */
typedef void (*destroyed_callback)(ui_window_t window);

/**
 * @brief Callback function type which is called when window is shown successfully
 *
 * @see ui_window_create()
 */
typedef void (*shown_callback)(ui_window_t window);

/**
 * @brief Callback function type which is called when window is hidden successfully
 *
 * @see ui_window_create()
 */
typedef void (*hidden_callback)(ui_window_t window);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create the window and register it to the core thread.
 *
 * @param[in] created_cb Callback function which is called when window is created successfully
 * @param[in] destroyed_cb Callback function which is called when window is destroyed successfully
 * @param[in] shown_cb Callback function which is called when window is shown successfully
 * @param[in] hidden_cb Callback function which is called when window is hidden successfully
 * @return On success, the window handle is returned. On failure, UI_NULL is returned.
 *
 * @see registered_callback
 */
ui_window_t ui_window_create(created_callback created_cb, destroyed_callback destroyed_cb, shown_callback shown_cb, hidden_callback hidden_cb);

/**
 * @brief Destroy the window and unregister it from the core thread.
 *
 * @param[in] window Handle of the registered window
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_window_create()
 */
ui_error_t ui_window_destroy(ui_window_t window);

/**
 * @brief Get the root of given window.
 *
 * @param[in] window Handle of the registered window
 * @return On success, the handle of root is returned. On failure, UI_NULL is returned.
 */
ui_widget_t ui_window_get_root(ui_window_t window);

/**
 * @brief Add an empty widget to the window.
 *
 * As widgets are organized in the form of tree, they are organized into parent-child relationships.
 * Therefore, if the parent widget is moved, child widgets are also moved correspondingly.
 * Furthermore, if the parent widget is removed (@ref ui_window_remove_widget), child widgets are also be removed automatically.
 * Accordingly the coordinates of the child widgets are the relative coordinate to the parent widget.
 *
 * @param[in] window Handle of the window which the widget will be added to
 * @param[in] widget Handle of the widget which wiil be added to the window
 * @param[in] x x coordinate of the widget. Relative coordinate to the parent widget
 * @param[in] y y coordinate of the widget. Relative coordinate to the parent widget
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 */
ui_error_t ui_window_add_widget(ui_window_t window, ui_widget_t widget, int32_t x, int32_t y);

#ifdef __cplusplus
}
#endif

#endif

/** @} */ // end of WINDOW group
