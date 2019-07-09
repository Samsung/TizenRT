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

#include <tinyara/config.h>
#include <string.h>
#include <araui/ui_commons.h>
#include "ui_widget_internal.h"
#include "ui_core_internal.h"
#include "ui_view_internal.h"
#include "ui_log.h"

#define CONFIG_UI_GLOBAL_WIDGET_APPEAR_CONST 3
#define CONFIG_UI_GLOBAL_WIDGET_RETURN_CONST 4

#if defined(CONFIG_UI_TOUCH_INTERFACE)

static void ui_global_widget_disappear_tween_end_func(ui_widget_t widget)
{
	ui_global_widget_body_t *body;

	if (!widget) {
		UI_LOGE("error: widget is null!\n");
		return;
	}

	body = (ui_global_widget_body_t *)widget;
	body->base.visible = false;
	body->state = UI_GLOBAL_WIDGET_STATE_NONE;
}

static void ui_global_widget_appear_tween_end_func(ui_widget_t widget)
{
	ui_global_widget_body_t *body;

	if (!widget) {
		UI_LOGE("error: widget is null!\n");
		return;
	}

	body = (ui_global_widget_body_t *)widget;
	body->state = UI_GLOBAL_WIDGET_STATE_AT_SCREEN;
}

static void ui_global_widget_touch_func(ui_widget_body_t *widget, ui_touch_event_t event, ui_coord_t coord)
{
	ui_global_widget_body_t *body;

	if (!widget) {
		UI_LOGE("error: widget is null!\n");
		return;
	}

	body = (ui_global_widget_body_t *)widget;

	switch (event) {
		case UI_TOUCH_EVENT_CANCEL: {
		}
		break;

		case UI_TOUCH_EVENT_DOWN: {
			// If the state of given global widget is UI_GLOBAL_WIDGET_STATE_TRANSITION,
			// then a touch event is ignored
			if (body->state == UI_GLOBAL_WIDGET_STATE_TRANSITION) {
				break;
			}

			// Otherwise, information about the touch event is stored at the widget body for the future usage.
			body->base.visible = true;
			body->press = true;
			body->touch_down.x = coord.x;
			body->touch_down.y = coord.y;
		}
		break;

		case UI_TOUCH_EVENT_MOVE: {
			if (body->press && body->base.visible) {
				switch (body->event_type) {
					case UI_GLOBAL_WIDGET_TOP_SWIPE: {
						if (body->transit_type == UI_TRANSITION_SLIDE) {
							// If the state of given global widget is UI_GLOBAL_WIDGET_STATE_NONE,
							// it means that the given global widget just appears.
							// Therefore, the edge of global widget follows the touch coordinate.
							if (body->state == UI_GLOBAL_WIDGET_STATE_NONE) {
								body->base.local_rect.y = coord.y - CONFIG_UI_DISPLAY_HEIGHT;
							} else if (body->state == UI_GLOBAL_WIDGET_STATE_AT_SCREEN) {
							// If the state of given global widget is UI_GLOBAL_WIDGET_STATE_AT_SCREEN,
							// it means that the given global widget occupied the display.
							// Therefore, the given global widget moves as difference as touch occurs.
								body->base.local_rect.y = coord.y - body->touch_down.y;
								if (body->base.local_rect.y > 0) {
									body->base.local_rect.y = 0;
								}
							}
						}
						// other transit types are needed to be implemented.
					}
					break;

					case UI_GLOBAL_WIDGET_BOTTOM_SWIPE: {
						if (body->transit_type == UI_TRANSITION_SLIDE) {
							if (body->state == UI_GLOBAL_WIDGET_STATE_NONE) {
								body->base.local_rect.y = coord.y;
							} else if (body->state == UI_GLOBAL_WIDGET_STATE_AT_SCREEN) {
								body->base.local_rect.y = coord.y - body->touch_down.y;
								if (body->base.local_rect.y < 0) {
									body->base.local_rect.y = 0;
								}
							}
						}
						// other transit types are needed to be implemented.
					}
					break;

					case UI_GLOBAL_WIDGET_LEFT_SWIPE: {
						if (body->transit_type == UI_TRANSITION_SLIDE) {
							if (body->state == UI_GLOBAL_WIDGET_STATE_NONE) {
								body->base.local_rect.x = coord.x - CONFIG_UI_DISPLAY_WIDTH;
							} else if (body->state == UI_GLOBAL_WIDGET_STATE_AT_SCREEN) {
								body->base.local_rect.x = coord.x - body->touch_down.x;
								if (body->base.local_rect.x > 0) {
									body->base.local_rect.x = 0;
								}
							}
						}
						// other transit types are needed to be implemented.
					}
					break;

					case UI_GLOBAL_WIDGET_RIGHT_SWIPE: {
						if (body->transit_type == UI_TRANSITION_SLIDE) {
							if (body->state == UI_GLOBAL_WIDGET_STATE_NONE) {
								body->base.local_rect.x = coord.x;
							} else if (body->state == UI_GLOBAL_WIDGET_STATE_AT_SCREEN) {
								if (body->transit_type == UI_TRANSITION_SLIDE) {
									body->base.local_rect.x = coord.x - body->touch_down.x;
									if (body->base.local_rect.x < 0) {
										body->base.local_rect.x = 0;
									}
								}
							}
						}
						// other transit types are needed to be implemented.
					}
					break;

					case UI_GLOBAL_WIDGET_BUTTON: {
					}
					break;

					default: {
					}
					break;
				}
			}
		}
		break;

		case UI_TOUCH_EVENT_UP: {
			if (body->base.visible) {
				body->press = false;
				switch (body->event_type) {
					case UI_GLOBAL_WIDGET_TOP_SWIPE: {
						if (body->transit_type == UI_TRANSITION_SLIDE) {
							// If the state of given global widget is UI_GLOBAL_WIDGET_STATE_NONE,
							// it means that the given global widget just appears.
							// Therefore, the given global widget occupies the display when the edge of global widget beyonds the certain threshold.
							// Otherwise, the given global widget will be not presented at the display.
							if (body->state == UI_GLOBAL_WIDGET_STATE_NONE) {
								if (coord.y < CONFIG_UI_DISPLAY_HEIGHT / CONFIG_UI_GLOBAL_WIDGET_APPEAR_CONST) {
									body->state = UI_GLOBAL_WIDGET_STATE_TRANSITION;
									ui_widget_tween_moveto((ui_widget_t)widget, 0, -1 * CONFIG_UI_DISPLAY_HEIGHT, CONFIG_UI_GLOBAL_TRANSITION_TIME, TWEEN_EASE_OUT_QUAD, ui_global_widget_disappear_tween_end_func);
								} else {
									body->state = UI_GLOBAL_WIDGET_STATE_TRANSITION;
									ui_widget_tween_moveto((ui_widget_t)widget, 0, 0, CONFIG_UI_GLOBAL_TRANSITION_TIME, TWEEN_EASE_OUT_QUAD, ui_global_widget_appear_tween_end_func);
								}
							} else if (body->state == UI_GLOBAL_WIDGET_STATE_AT_SCREEN) {
								// If the state of given global widget is UI_GLOBAL_WIDGET_STATE_AT_SCREEN,
								// it means that the given global widget occupied the display.
								// Therefore, the given global widget will disappear
								// when the difference between a touch down coordinate and touch a up coordinate beyonds the certain threshold.
								// Otherwise, the given global widget occupies the display again.
								if ((body->touch_down.y - coord.y) > CONFIG_UI_DISPLAY_HEIGHT / CONFIG_UI_GLOBAL_WIDGET_RETURN_CONST) {
									body->state = UI_GLOBAL_WIDGET_STATE_TRANSITION;
									ui_widget_tween_moveto((ui_widget_t)widget, 0, -1 * CONFIG_UI_DISPLAY_HEIGHT, CONFIG_UI_GLOBAL_TRANSITION_TIME, TWEEN_EASE_OUT_QUAD, ui_global_widget_disappear_tween_end_func);
								} else {
									body->state = UI_GLOBAL_WIDGET_STATE_TRANSITION;
									ui_widget_tween_moveto((ui_widget_t)widget, 0, 0, CONFIG_UI_GLOBAL_TRANSITION_TIME, TWEEN_EASE_OUT_QUAD, ui_global_widget_appear_tween_end_func);
								}
							}
						}
						// other transit types are needed to be implemented.
					}
					break;

					case UI_GLOBAL_WIDGET_BOTTOM_SWIPE: {
						if (body->transit_type == UI_TRANSITION_SLIDE) {
							if (body->state == UI_GLOBAL_WIDGET_STATE_NONE) {
								if (coord.y > CONFIG_UI_DISPLAY_HEIGHT - CONFIG_UI_DISPLAY_HEIGHT / CONFIG_UI_GLOBAL_WIDGET_APPEAR_CONST) {
									body->state = UI_GLOBAL_WIDGET_STATE_TRANSITION;
									ui_widget_tween_moveto((ui_widget_t)widget, 0, CONFIG_UI_DISPLAY_HEIGHT, CONFIG_UI_GLOBAL_TRANSITION_TIME, TWEEN_EASE_OUT_QUAD, ui_global_widget_disappear_tween_end_func);
								} else {
									body->state = UI_GLOBAL_WIDGET_STATE_TRANSITION;
									ui_widget_tween_moveto((ui_widget_t)widget, 0, 0, CONFIG_UI_GLOBAL_TRANSITION_TIME, TWEEN_EASE_OUT_QUAD, ui_global_widget_appear_tween_end_func);
								}
							} else if (body->state == UI_GLOBAL_WIDGET_STATE_AT_SCREEN) {
								if ((coord.y - body->touch_down.y) > CONFIG_UI_DISPLAY_HEIGHT / CONFIG_UI_GLOBAL_WIDGET_RETURN_CONST) {
									body->state = UI_GLOBAL_WIDGET_STATE_TRANSITION;
									ui_widget_tween_moveto((ui_widget_t)widget, 0, CONFIG_UI_DISPLAY_HEIGHT, CONFIG_UI_GLOBAL_TRANSITION_TIME, TWEEN_EASE_OUT_QUAD, ui_global_widget_disappear_tween_end_func);
								} else {
									body->state = UI_GLOBAL_WIDGET_STATE_TRANSITION;
									ui_widget_tween_moveto((ui_widget_t)widget, 0, 0, CONFIG_UI_GLOBAL_TRANSITION_TIME, TWEEN_EASE_OUT_QUAD, ui_global_widget_appear_tween_end_func);
								}
							}
						}
						// other transit types are needed to be implemented.
					}
					break;

					case UI_GLOBAL_WIDGET_LEFT_SWIPE: {
						if (body->transit_type == UI_TRANSITION_SLIDE) {
							if (body->state == UI_GLOBAL_WIDGET_STATE_NONE) {
								if (coord.x < CONFIG_UI_DISPLAY_WIDTH / CONFIG_UI_GLOBAL_WIDGET_APPEAR_CONST) {
									body->state = UI_GLOBAL_WIDGET_STATE_TRANSITION;
									ui_widget_tween_moveto((ui_widget_t)widget, -1 * CONFIG_UI_DISPLAY_WIDTH, 0, CONFIG_UI_GLOBAL_TRANSITION_TIME, TWEEN_EASE_OUT_QUAD, ui_global_widget_disappear_tween_end_func);
								} else {
									body->state = UI_GLOBAL_WIDGET_STATE_TRANSITION;
									ui_widget_tween_moveto((ui_widget_t)widget, 0, 0, CONFIG_UI_GLOBAL_TRANSITION_TIME, TWEEN_EASE_OUT_QUAD, ui_global_widget_appear_tween_end_func);
								}
							} else if (body->state == UI_GLOBAL_WIDGET_STATE_AT_SCREEN) {
								if ((body->touch_down.x - coord.x) > CONFIG_UI_DISPLAY_WIDTH / CONFIG_UI_GLOBAL_WIDGET_RETURN_CONST) {
									body->state = UI_GLOBAL_WIDGET_STATE_TRANSITION;
									ui_widget_tween_moveto((ui_widget_t)widget, -1 * CONFIG_UI_DISPLAY_WIDTH, 0, CONFIG_UI_GLOBAL_TRANSITION_TIME, TWEEN_EASE_OUT_QUAD, ui_global_widget_disappear_tween_end_func);
								} else {
									body->state = UI_GLOBAL_WIDGET_STATE_TRANSITION;
									ui_widget_tween_moveto((ui_widget_t)widget, 0, 0, CONFIG_UI_GLOBAL_TRANSITION_TIME, TWEEN_EASE_OUT_QUAD, ui_global_widget_appear_tween_end_func);
								}
							}
						}
						// other transit types are needed to be implemented.
					}
					break;

					case UI_GLOBAL_WIDGET_RIGHT_SWIPE: {
						if (body->transit_type == UI_TRANSITION_SLIDE) {
							if (body->state == UI_GLOBAL_WIDGET_STATE_NONE) {
								if (coord.x > CONFIG_UI_DISPLAY_WIDTH - CONFIG_UI_DISPLAY_WIDTH / CONFIG_UI_GLOBAL_WIDGET_APPEAR_CONST) {
									body->state = UI_GLOBAL_WIDGET_STATE_TRANSITION;
									ui_widget_tween_moveto((ui_widget_t)widget, CONFIG_UI_DISPLAY_WIDTH, 0, CONFIG_UI_GLOBAL_TRANSITION_TIME, TWEEN_EASE_OUT_QUAD, ui_global_widget_disappear_tween_end_func);
								} else {
									body->state = UI_GLOBAL_WIDGET_STATE_TRANSITION;
									ui_widget_tween_moveto((ui_widget_t)widget, 0, 0, CONFIG_UI_GLOBAL_TRANSITION_TIME, TWEEN_EASE_OUT_QUAD, ui_global_widget_appear_tween_end_func);
								}
							} else if (body->state == UI_GLOBAL_WIDGET_STATE_AT_SCREEN) {
								if ((coord.x - body->touch_down.x) > CONFIG_UI_DISPLAY_WIDTH / CONFIG_UI_GLOBAL_WIDGET_RETURN_CONST) {
									body->state = UI_GLOBAL_WIDGET_STATE_TRANSITION;
									ui_widget_tween_moveto((ui_widget_t)widget, CONFIG_UI_DISPLAY_WIDTH, 0, CONFIG_UI_GLOBAL_TRANSITION_TIME, TWEEN_EASE_OUT_QUAD, ui_global_widget_disappear_tween_end_func);
								} else {
									body->state = UI_GLOBAL_WIDGET_STATE_TRANSITION;
									ui_widget_tween_moveto((ui_widget_t)widget, 0, 0, CONFIG_UI_GLOBAL_TRANSITION_TIME, TWEEN_EASE_OUT_QUAD, ui_global_widget_appear_tween_end_func);
								}
							}
						}
						// other transit types are needed to be implemented.
					}
					break;

					case UI_GLOBAL_WIDGET_BUTTON: {
					}
					break;

					default: {
					}
					break;
				}
			}
		}
		break;

		default:
		break;
	}
}

#endif

static void ui_global_widget_update_func(ui_widget_t widget, uint32_t dt)
{
	ui_global_widget_body_t *body;

	if (!widget) {
		UI_LOGE("error: Invalid Parameter!\n");
		return;
	}

	body = (ui_global_widget_body_t *)widget;

	// If the state of given global widget is UI_GLOBAL_WIDGET_STATE_TRANSITION,
	// the position of given global widget is updated by the transition function.
	if (body->state != UI_GLOBAL_WIDGET_STATE_TRANSITION) {
		ui_widget_update_position_info((ui_widget_body_t *)widget);
	}
}


ui_widget_t ui_global_widget_create(ui_transit_type_t transit_type)
{
	ui_global_widget_body_t *body;

	if (!ui_is_running()) {
		UI_LOGE("error: UI framework is not running!\n");
		return UI_NULL;
	}

	body = (ui_global_widget_body_t *)UI_ALLOC(sizeof(ui_global_widget_body_t));
	if (!body) {
		UI_LOGE("error: out of memory!\n");
		return UI_NULL;
	}

	memset(body, 0, sizeof(ui_global_widget_body_t));
	ui_widget_init((ui_widget_body_t *)body, CONFIG_UI_DISPLAY_WIDTH, CONFIG_UI_DISPLAY_HEIGHT);
	((ui_widget_body_t *)body)->type = UI_GLOBAL_WIDGET;

	// default settings
#if defined(CONFIG_UI_TOUCH_INTERFACE)
	body->base.touchable = true;
	body->base.is_hooker = true;
	body->base.touch_cb = ui_global_widget_touch_func;
#endif
	body->base.update_cb = ui_global_widget_update_func;
	body->transit_type = transit_type;

	UI_LOGD("UI_OK.\n");

	return (ui_widget_t)body;
}

