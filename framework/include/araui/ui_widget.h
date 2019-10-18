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
 * @defgroup WIDGET Widget
 * @ingroup ARAUI
 * @{
 */

#ifndef __UI_WIDGET_H__
#define __UI_WIDGET_H__

#include <tinyara/config.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <araui/ui_commons.h>
#include <araui/ui_asset.h>
#include <araui/ui_animation.h>

/**
 * @brief A handle type to control all kind of widgets in the UI Framework
 *
 * @see ui_window_add_widget()
 */
typedef long ui_widget_t;

/**
 * @brief Callback function type which is called when the tick callback is set to the widget.
 *
 * @param widget Widget which has the tick callback function
 * @param dt Delta Time (msec)
 *
 * @see ui_widget_set_tick_callback()
 */
typedef void (*tick_callback)(ui_widget_t widget, uint32_t dt);

/**
 * @brief Callback function type which is called when the interval callback is set to the widget.
 *
 * @param widget Widget which has the interval callback function
 *
 * @see ui_widget_set_interval_callback()
 */
typedef void (*interval_callback)(ui_widget_t widget);

/**
 * @brief Callback function type which is called when the animation is finished.
 *
 * @param widget Handle of the widget which performs animation
 *
 * @see ui_widget_play_anim()
 */
typedef void (*anim_finished_callback)(ui_widget_t widget, ui_anim_t anim);

#if defined(CONFIG_UI_ENABLE_TOUCH)

/**
 * @brief Callback function type which is called when a button widget is cliecked.
 *
 * @param widget Handle of the button widget which performs touched event
 *
 * @see ui_button_widget_set_touched_callback()
 */
typedef void (*button_touched_callback)(ui_widget_t widget);

#endif // CONFIG_UI_ENABLE_TOUCH

typedef void (*quick_panel_event_callback)(ui_widget_t widget,
	uint32_t dt, ui_touch_event_t touch_event, ui_coord_t touch_coord); 

typedef enum {
	UI_QUICK_PANEL_TOP_SWIPE = 0,
	UI_QUICK_PANEL_BOTTOM_SWIPE,
	UI_QUICK_PANEL_LEFT_SWIPE,
	UI_QUICK_PANEL_RIGHT_SWIPE,
	UI_QUICK_PANEL_BUTTON,
	UI_QUICK_PANEL_TYPE_NUM
} ui_quick_panel_event_type_t;

/**
 * @brief Enumeration that represents the type of the scroll widget reaches its offset
 */
typedef enum {
	UI_REACH_ACTION_NONE = 0,
	UI_REACH_ACTION_DELIVER_TO_PARENT
} ui_reach_offset_action_type_t;

#ifdef __cplusplus
extern "C" {
#endif

/********************************
 *
 * Widget commons
 *
 */

/**
 * @brief Create an empty widget.
 *
 * In order to operate properly, the created widget needs to set a parent (@ref ui_widget_add_child).
 * This empty widget can be used as the container widget which groups certain widgets.
 *
 * @param[in] width Width of the widget (Pixel)
 * @param[in] height Height of the widget (Pixel)
 * @return On success, the handle of widget is returned. On failure, UI_NULL is returned.
 *
 * @see ui_window_get_root()
 * @see ui_widget_add_child()
 */
ui_widget_t ui_widget_create(int32_t width, int32_t height);

/**
 * @brief Destroy the created widget and release the allocated memory.
 *
 * When the widget is destroyed, the child widgets of widget are also destroyed.
 * All widgets (including the image, text, scroll, and button widgets) can be destroyed with this function
 *
 * @param[in] widget Handle of the widget to be destroyed
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_widget_create()
 * @see ui_image_widget_create()
 * @see ui_text_widget_create()
 * @see ui_scroll_widget_create()
 * @see ui_button_widget_create()
 */
ui_error_t ui_widget_destroy(ui_widget_t widget);

/**
 * @brief Move the location(coordinate) of widget.
 *
 * All widgets (including the image widget and the text widget) can be moved with this function.
 *
 * @param[in] widget Handle of the widget to move
 * @param[in] x x coordinate of the moved widget
 * @param[in] y y coordinate of the moved widget
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 */
ui_error_t ui_widget_set_position(ui_widget_t widget, int32_t x, int32_t y);

/**
 * @brief Returns a rectangular region occupied by the widget.
 *
 * @param[in] widget Handle of the widget
 * @return On success, a rectangular region occupied by the widget is returned. On failure, an empty rectangular region is returned.
 *
 * @see ui_rect_t
 */
ui_rect_t ui_widget_get_rect(ui_widget_t widget);

/**
 * @brief Set the size of widget.
 *
 * All widgets (including the image widget and the text widget) can be resized with this function.
 *
 * @param[in] widget Handle of the widget to resize
 * @param[in] width New width of the wigdet (Pixel)
 * @param[in] height New height of the widget (Pixel)
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 */
ui_error_t ui_widget_set_size(ui_widget_t widget, int32_t width, int32_t height);

/**
 * @brief Set visible state of widget.
 *
 * If a widget is invisible, the widget is now showing in the display.
 *
 * @param[in] widget Handle of the widget to move
 * @param[in] visible visible state
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 */
ui_error_t ui_widget_set_visible(ui_widget_t widget, bool visible);

#if defined(CONFIG_UI_ENABLE_TOUCH)

/**
 * @brief Set the touchable value of the widget.
 *
 * If the touchable value of the widget is true, the widget can get the focus, and vice versa.
 *
 * @param[in] widget Handle of the widget which want to set the touchable value
 * @param[in] touchable Value which want to set as the touchable value
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 */
ui_error_t ui_widget_set_touchable(ui_widget_t widget, bool touchable);

#endif // CONFIG_UI_ENABLE_TOUCH

/**
 * @brief Set the tick callback function to the widget.
 *
 * When the tick callback function is set, the tick callback function is always called whenever the screen is updated.
 * Therefore, it is recommaned not to set the unnecessary tick callback function.
 * If user want to set a callback function called at specific interval, please use @ref ui_widget_set_interval_callback().
 * Only one tick callback function can be set to the widget.
 *
 * @param[in] widget Handle of the widget to set tick callback function
 * @param[in] tick_cb Tick callback function
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see tick_callback
 */
ui_error_t ui_widget_set_tick_callback(ui_widget_t widget, tick_callback tick_cb);

/**
 * @brief Set the interval callback function to the widget.
 *
 * When the interval callback function is set, the interval callback function is called at specific interval.
 * In order to remove the interval callback function, please insert interval_cb input as NULL.
 * Only one interval callback function can be set to the widget.
 *
 * @param[in] widget Handle of the widget to set interval callback function
 * @param[in] interval_cb Interval callback function
 * @param[in] timeout Interval of calling the interval callback function (msec)
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see interval_callback
 */
ui_error_t ui_widget_set_interval_callback(ui_widget_t widget, interval_callback interval_cb, uint32_t timeout);

/**
 * @brief Add the child widget to the parent widget and set the relative coordinates of child widget.
 *
 * As widgets are organized in the form of tree, they are organized into parent-child relationships.
 * Therefore, if the parent widget is moved, child widgets are also moved correspondingly.
 * Furthermore, if the parent widget is destroyed (@ref ui_widget_destroy), child widgets are also be destroyed automatically.
 * Accordingly the coordinates of the child widgets are the relative coordinate to the parent widget.
 *
 * @param[in] widget Handle of the widget which will be set as the parent
 * @param[in] child Handle of the widget which want to set the parent
 * @param[in] x x coordinate of the widget. Relative coordinate to the parent widget
 * @param[in] y y coordinate of the widget. Relative coordinate to the parent widget
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_window_get_root()
 */
ui_error_t ui_widget_add_child(ui_widget_t widget, ui_widget_t child, int32_t x, int32_t y);

/**
 * @brief Returns the parent widget of the given widget.
 *
 * @param[in] widget Handle of the widget
 * @return On success, the parent widget handle is returned. On failure, UI_NULL is returned.
 *
 * @see ui_rect_t
 */
ui_widget_t ui_widget_get_parent(ui_widget_t widget);

/**
 * @brief Remove the child widget from the parent widget.
 *
 * This function doesn't destroy the child widget. Only remove(deatch) from the parent.
 *
 * @param[in] widget Handle of the widget which will be set as the parent
 * @param[in] child Handle of the widget which want to be removed from the parent
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_widget_add_child()
 */
ui_error_t ui_widget_remove_child(ui_widget_t widget, ui_widget_t child);

/**
 * @brief Remove all children widgets from the parent widget.
 * 
 * This function doesn't destroy the child widgets. Only remove(deatch) from the parent.
 *
 * @param[in] widget Handle of the widget which will be set as the parent
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_widget_add_child()
 */
ui_error_t ui_widget_remove_all_children(ui_widget_t widget);

/**
 * @brief Set the pivot point of widget.
 *
 * Widgets will be rotated and scaled for this pivot points.
 * Default coordinates of the pivot point is (0, 0)
 *
 * @param[in] widget Handle of the widget
 * @param[in] x x coordinate of the pivot point
 * @param[in] y y coordinate of the pivot point
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_image_asset_set_anchor_point()
 * @see ui_error_t
 */
ui_error_t ui_widget_set_pivot_point(ui_widget_t widget, int32_t x, int32_t y);

/**
 * @brief Set the scaling factor of widget.
 *
 * Scaling factor must larger than 0.
 *
 * @param[in] widget Handle of the widget
 * @param[in] scale_x Scaling factor for x axis (i.e., horizontal scaling factor)
 * @param[in] scale_y Scaling factor for y axis (i.e., vertical scaling factor)
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_error_t
 */
ui_error_t ui_widget_set_scale(ui_widget_t widget, float scale_x, float scale_y);

/**
 * @brief Set the degree of rotated widget.
 *
 * @param[in] widget Handle of the widget
 * @param[in] degree Degree of the rotated widget (-360 ~ 360 degree)
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_error_t
 */
ui_error_t ui_widget_set_rotation(ui_widget_t widget, int32_t degree);

/**
 * @brief Set the userdata of the widget.
 *
 * Each widget can have its own data (void * type).
 *
 * @param[in] widget Handle of the widget
 * @param[in] userdata Userdata pointer
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_widget_get_userdata
 */
ui_error_t ui_widget_set_userdata(ui_widget_t widget, void *userdata);

/**
 * @brief Get the userdata of the widget.
 *
 * Each widget can have its own data (void * type).
 *
 * @param[in] widget Handle of the widget
 * @return On success, userdata pointer is returned. On failure, Null value is returned.
 *
 * @see ui_widget_set_userdata
 */
void *ui_widget_get_userdata(ui_widget_t widget);

/**
 * @brief Set the animation of the widget.
 *
 * Animation finished callback can be NULL.
 *
 * @param[in] widget Handle of the widget
 * @param[in] anim Handle of the widget
 * @param[in] anim_finished_cb Animation finished callback function
 * @param[in] loop Flag for animation loop
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_error_t
 */
ui_error_t ui_widget_play_anim(ui_widget_t widget, ui_anim_t anim, anim_finished_callback anim_finished_cb, bool loop);

/********************************
 *
 * Image widget
 *
 */

/**
 * @brief Create a image widget.
 *
 * In order to operate properly, the created widget needs to set a parent (@ref ui_widget_add_child).
 *
 * The input of this API (image) can be NULL value.
 * If user want to set the image later, please use ui_image_widget_set_image() (@ref ui_image_widget_set_image).
 *
 * Width and height are not specified when registering an image widget.
 * Width and height of the image widget are automatically calculated from the specified image asset.
 * If the width and height of image widget are different from the image asset, please resize the image widget by using @ref ui_widget_set_size.
 *
 * @param[in] image Handle of the image asset
 * @return On success, the handle of image widget is returned. On failure, UI_NULL is returned.
 *
 * @see ui_image_asset_create_from_file()
 * @see ui_widget_add_child()
 * @see ui_asset_t
 */
ui_widget_t ui_image_widget_create(ui_asset_t image);

/**
 * @brief Set the image asset to be used by the image widget.
 *
 * In order to prevent the memory leakage problem, the image assets which will not be used should be destroyed.
 * Please refer @ref ui_image_asset_destroy().
 *
 * @param[in] widget Handle of the image widget
 * @param[in] image Handle of the new image asset
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_image_asset_create_from_buffer()
 * @see ui_image_asset_destroy()
 */
ui_error_t ui_image_widget_set_image(ui_widget_t widget, ui_asset_t image);

/**
 * @brief Set the crop area of the image asset in the image widget.
 *
 * Crop the image asset which set to the image widget.
 * If the image asset of image widget is not set, UI_INVALID_PARAM is retured.
 * Furthermore, a crop area can not exceed the original image asset.
 *
 * @param[in] widget Handle of the image widget
 * @param[in] crop_rect Crop area to cut the image asset
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_image_widget_set_image()
 * @see ui_error_t
 */
ui_error_t ui_image_widget_set_crop_area(ui_widget_t widget, ui_rect_t crop_rect);



/********************************
 *
 * Text widget
 *
 */

/**
 * @brief Create a text widget.
 *
 * In order to operate properly, the created widget needs to set a parent (@ref ui_widget_add_child).
 *
 * Before creating a text widget, user must create a font asset for the text widget to use.
 * If the actual size of string is larger than the area of the text widget, the outer area is not presented.
 *
 * The input of this API (text) can be NULL value.
 * If user want to set the text later, please use ui_text_widget_set_text() (@ref ui_text_widget_set_text()).
 *
 * @param[in] width Width of the text widget (Pixel)
 * @param[in] height Height of the text widget (Pixel)
 * @param[in] font Handle of the font asset to use when representing string
 * @param[in] text Text string
 * @paran[in] font_size Size of the text (Pixel)
 * @return On success, the handle of text widget is returned. On failure, UI_NULL is returned.
 *
 * @see ui_font_asset_create_from_file()
 * @see ui_widget_add_child()
 * @see ui_asset_t
 */
ui_widget_t ui_text_widget_create(int32_t width, int32_t height, ui_asset_t font, const char *text, size_t font_size);

/**
 * @brief Set the string of the text widget.
 *
 * @param[in] widget Handle of the text widget
 * @param[in] text New text string
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_text_widget_create()
 */
ui_error_t ui_text_widget_set_text(ui_widget_t widget, const char *text);

/**
 * @brief Set the string of the text widget.
 *
 * @param[in] widget Handle of the text widget
 * @param[in] format New formatted text string
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_text_widget_create()
 */
ui_error_t ui_text_widget_set_text_format(ui_widget_t widget, const char *format, ...);

/**
 * @brief Set the align of the text widget.
 *
 * Default align option is ALIGN_LEFT | ALIGN_TOP.
 *
 * @param[in] widget Handle of the text widget
 * @param[in] align New align option
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_text_widget_create()
 * @see ui_align_t
 */
ui_error_t ui_text_widget_set_align(ui_widget_t widget, ui_align_t align);

/**
 * @brief Set color of the text in the text widget
 *
 * @param[in] widget Handle of the widget which will be set the color of text
 * @param[in] color Color of the text to set
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 */
ui_error_t ui_text_widget_set_color(ui_widget_t widget, ui_color_t color);

/**
 * @brief Set the text wrap option to the text widget
 *
 * @param[in] widget Handle of the widget which will be set the word wrap option
 * @param[in] word_wrap Option that want to be set as the word wrap
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 */
ui_error_t ui_text_widget_set_word_wrap(ui_widget_t widget, bool word_wrap);

/**
 * @brief Set the font size of the text widget
 *
 * @param[in] widget Handle of the widget which will be set the font size
 * @param[in] font_size Font size of the text (Pixel)
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 */
ui_error_t ui_text_widget_set_font_size(ui_widget_t widget, size_t font_size);


/********************************
 *
 * Button widget
 *
 */

/**
 * @brief Create a button widget.
 *
 * In order to operate properly, the created widget needs to set a parent (@ref ui_widget_add_child).
 *
 * @param[in] width Width of the button widget (Pixel)
 * @param[in] height Height of the button widget (Pixel)
 * @return On success, the handle of button widget is returned. On failure, UI_NULL is returned.
 *
 * @see ui_widget_add_child()
 */
ui_widget_t ui_button_widget_create(int32_t width, int32_t height);

#if defined(CONFIG_UI_ENABLE_TOUCH)

/**
 * @brief Set the touch callback to the button widget which will be called when the button widget is pressed.
 *
 * @param[in] widget Handle of the widget which want to set the touch callback
 * @param[in] touched_cb Function pointer of the touched callback
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 */
ui_error_t ui_button_widget_set_touched_callback(ui_widget_t widget, button_touched_callback touched_cb);

#endif // CONFIG_UI_ENABLE_TOUCH




/********************************
 *
 * Scroll widget
 *
 */

/**
 * @brief Create a scroll widget.
 *
 * In order to operate properly, the created widget needs to set a parent (@ref ui_widget_add_child).
 *
 * @param[in] width Width of the scroll widget (Pixel)
 * @param[in] height Height of the scroll widget (Pixel)
 * @return On success, the handle of scroll widget is returned. On failure, UI_NULL is returned.
 *
 * @see ui_widget_add_child()
 */
ui_widget_t ui_scroll_widget_create(int32_t width, int32_t height);

/**
 * @brief Set the scroll direction of scroll widget.
 *
 * @param[in] widget Handle of the scroll widget which want to set the scroll direction
 * @param[in] direction Scroll direction which want to set
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_direction_t
 */
ui_error_t ui_scroll_widget_set_direction(ui_widget_t widget, ui_direction_t direction);

/**
 * @brief Set the content size of scroll widget.
 *
 * Scroll widget can be scrolled up to the content size.
 *
 * @param[in] widget Handle of the scroll widget which want to set the content size
 * @param[in] width Width of the content size which will be set
 * @param[in] height Height of the content size which will be set
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 */
ui_error_t ui_scroll_widget_set_content_size(ui_widget_t widget, int32_t width, int32_t height);

/**
 * @brief Set an action of scroll widget when the scroll widget reaches the its minimum offset.
 *
 * @param[in] widget Handle of the scroll widget which want to set the action\n
					 when the scroll widget reaches the its minimum offset
 * @param[in] action_type Type of action when the scroll widget reaches its minimum offset
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 */
ui_error_t ui_scroll_widget_set_min_offset_reach_action(ui_widget_t widget, ui_reach_offset_action_type_t action_type);

/**
 * @brief Set an action of scroll widget when the scroll widget reaches the its maximum offset.
 *
 * @param[in] widget Handle of the scroll widget which want to set the action\n
					 when the scroll widget reaches the its maximum offset
 * @param[in] action_type Type of action when the scroll widget reaches its maximum offset
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 */
ui_error_t ui_scroll_widget_set_max_offset_reach_action(ui_widget_t widget, ui_reach_offset_action_type_t action_type);



/********************************
 *
 * Paginator widget
 *
 */

/**
 * @brief Create a paginator widget.
 *
 * In order to operate properly, the created widget needs to set a parent (@ref ui_widget_add_child).
 * Default direction is UI_DIRECTION_HORIZONTAL. To change the direction, @ref ui_paginator_widget_set_direction.
 *
 * @param[in] width Width of the paginator widget (Pixel)
 * @param[in] height Height of the paginator widget (Pixel)
 * @param[in] pages[] Array of ui_widget_t which means each pages
 * @param[in] page_count Count of the pages
 * @return On success, the handle of paginator widget is returned. On failure, UI_NULL is returned.
 *
 * @see ui_paginator_widget_set_direction()
 */
ui_widget_t ui_paginator_widget_create(int32_t width, int32_t height, ui_widget_t pages[], uint32_t page_count);

/**
 * @brief Set the scroll direction of paginator widget. Default direction is UI_DIRECTION_HORIZONTAL.
 *
 * @param[in] widget Handle of the paginator widget which want to set the scroll direction
 * @param[in] direction Scroll direction which want to set
 * @return On success, UI_OK is returned. On failure, the defined error type is returned.
 *
 * @see ui_direction_t
 */
ui_error_t ui_paginator_widget_set_direction(ui_widget_t widget, ui_direction_t direction);



/********************************
 *
 * Quick panel widget
 *
 */

/**
 * @brief Create a quick_panel widget widget
 *
 * In order to operate properly, the created widget needs to set a event type (@ref ui_core_set_quick_panel).
 *
 * @param[in] transition_type Type of the transition which will be applied to the created widget.
 * @return On success, the handle of widget is returned. On failure, UI_NULL is returned.
 *
 * @see ui_transition_type_t
 */
ui_widget_t ui_quick_panel_create(ui_transition_type_t transition_type);

#ifdef __cplusplus
}
#endif

#endif

/** @} */ // end of WIDGET group
