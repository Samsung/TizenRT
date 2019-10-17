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

#ifndef __UI_CORE_INTERNAL_H__
#define __UI_CORE_INTERNAL_H__

#include <stdbool.h>
#include "ui_widget_internal.h"

#define CONFIG_UI_QUICK_PANEL_TRANSITION_TIME 300

typedef enum {
	UI_CORE_STATE_STOP,
	UI_CORE_STATE_STOPPING,
	UI_CORE_STATE_RUNNING
} ui_core_state_t;

#ifdef __cplusplus
extern "C" {
#endif

bool ui_is_running(void);

#if defined(CONFIG_UI_ENABLE_TOUCH)

/**
 * @brief If touch target is locked, all touch events are sent to the locked target only until it is unlocked.
 * !! Caution !! These functions don't support thread safety. Must be called as synchronous.
 */
void ui_core_lock_touch_event_target(ui_widget_body_t *target);

void ui_core_unlock_touch_event_target(void);

void ui_core_unlock_and_deliver_touch(ui_widget_body_t *body,ui_touch_event_t touch_event, ui_coord_t touch);

#endif // CONFIG_UI_ENABLE_TOUCH

#ifdef __cplusplus
}
#endif

#endif
