/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

// #include <Ecore_Wl2.h>
// #include <wayland-client.h>
// #include <wayland-tbm-client.h>
// #include <tizen-extension-client-protocol.h>

// #include <Ecore.h>
// #include <glib-object.h>
// #include <malloc.h>
#include <glib.h>
// #include <gio/gio.h>
#include <stdbool.h>
#include <aul.h>
#include <aul_svc.h>
#include <aul_rpc_port.h>
// #include <bundle_internal.h>
// #include <ttrace.h>

#include "appcore_base.h"
#include "appcore_ui_base.h"
#include "appcore_ui_base_private.h"
// #include "appcore_ui_plugin.h"

#include <araui/ui_core.h>
#include <eventloop/eventloop.h>

enum app_state {
	AS_NONE,
	AS_CREATED,
	AS_RUNNING,
	AS_PAUSED,
	AS_DYING,
};

enum win_status {
	WS_NONE,
	WS_PAUSE,
	WS_RESUME,
};

enum visibility_type {
	VT_NONE,
	VT_UNOBSCURED,
	VT_FULLY_OBSCURED,
};

typedef struct _appcore_ui_base_context {
	appcore_ui_base_ops ops;
	void *data;
	int argc;
	char **argv;
	unsigned int hint;
	char *below_app;
	char *appid;
	bool bg_state;
	bool resource_reclaiming;

	int state;
	// Ecore_Event_Handler *hshow;
	// Ecore_Event_Handler *hhide;
	// Ecore_Event_Handler *hvchange;
	// Ecore_Event_Handler *hlower;
	// Ecore_Event_Handler *hpvchange;
} appcore_ui_base_context;


static int w_status = WS_NONE;
static bool first_launch = true;

struct win_node {
	unsigned int win;
	unsigned int surf;
	int vis;
};

// static GSList *g_winnode_list;
static appcore_ui_base_context __context;
// static struct wl_display *dsp;
// static struct wl_registry *reg;
// static struct tizen_policy *tz_policy;

// static void _wl_cb_conformant(void *data,
// 		struct tizen_policy *tizen_policy,
// 		struct wl_surface *surface_resource,
// 		uint32_t is_conformant)
// {
// 	; // nothing to do.
// }

// static void _wl_cb_conformant_area(void *data,
// 		struct tizen_policy *tizen_policy,
// 		struct wl_surface *surface_resource,
// 		uint32_t conformant_part,
// 		uint32_t state,
// 		int32_t x, int32_t y, int32_t w, int32_t h)
// {
// 	; // nothing to do.
// }

// static void _wl_cb_notification_done(void *data,
// 		struct tizen_policy *tizen_policy,
// 		struct wl_surface *surface,
// 		int32_t level,
// 		uint32_t state)
// {
// 	; // nothing to do.
// }

// static void _wl_cb_transient_for_done(void *data,
// 		struct tizen_policy *tizen_policy,
// 		uint32_t child_id)
// {
// 	; // nothing to do.
// }

// static void _wl_cb_scr_mode_done(void *data,
// 		struct tizen_policy *tizen_policy,
// 		struct wl_surface *surface,
// 		uint32_t mode,
// 		uint32_t state)
// {
// 	; // nothing to do.
// }

// static void _wl_cb_iconify_state_changed(void *data,
// 		struct tizen_policy *tizen_policy,
// 		struct wl_surface *surface_resource,
// 		uint32_t iconified,
// 		uint32_t force)
// {
// 	; // nothing to do.
// }

// static void _wl_cb_supported_aux_hints(void *data,
// 		struct tizen_policy *tizen_policy,
// 		struct wl_surface *surface_resource,
// 		struct wl_array *hints,
// 		uint32_t num_hints)
// {
// 	; // nothing to do.
// }

// static void _wl_cb_allowed_aux_hint(void *data,
// 		struct tizen_policy *tizen_policy,
// 		struct wl_surface *surface_resource,
// 		int id)
// {
// 	; // nothing to do.
// }

// static void _wl_cb_aux_message(void *data,
// 		struct tizen_policy *tizen_policy,
// 		struct wl_surface *surface_resource,
// 		const char *key,
// 		const char *val,
// 		struct wl_array *options)
// {
// 	; // nothing to do.
// }

// static void _wl_cb_conformant_region(void *data,
// 		struct tizen_policy *tizen_policy,
// 		struct wl_surface *surface,
// 		uint32_t conformant_part,
// 		uint32_t state,
// 		int32_t x, int32_t y, int32_t w, int32_t h,
// 		uint32_t serial)
// {
// 	; // nothing to do.
// }

// static const struct tizen_policy_listener _tizen_policy_listener = {
// 	_wl_cb_conformant,
// 	_wl_cb_conformant_area,
// 	_wl_cb_notification_done,
// 	_wl_cb_transient_for_done,
// 	_wl_cb_scr_mode_done,
// 	_wl_cb_iconify_state_changed,
// 	_wl_cb_supported_aux_hints,
// 	_wl_cb_allowed_aux_hint,
// 	_wl_cb_aux_message,
// 	_wl_cb_conformant_region,
// };

// static const struct wl_registry_listener reg_listener = {
// 	__wl_listener_cb,
// 	__wl_listener_remove_cb
// };

// static Eina_Bool __stub_show_cb(void *data, int type, void *event)
// {
// 	if (__context.ops.window.show)
// 		__context.ops.window.show(type, event, __context.data);

// 	return ECORE_CALLBACK_RENEW;
// }

// static Eina_Bool __stub_hide_cb(void *data, int type, void *event)
// {
// 	if (__context.ops.window.hide)
// 		__context.ops.window.hide(type, event, __context.data);

// 	return ECORE_CALLBACK_RENEW;
// }

// static Eina_Bool __stub_visibility_cb(void *data, int type, void *event)
// {
// 	if (__context.ops.window.visibility)
// 		__context.ops.window.visibility(type, event, __context.data);

// 	return ECORE_CALLBACK_RENEW;
// }

// static Eina_Bool __stub_lower_cb(void *data, int type, void *event)
// {
// 	if (__context.ops.window.lower)
// 		__context.ops.window.lower(type, event, __context.data);

// 	return ECORE_CALLBACK_RENEW;
// }

// static Eina_Bool __stub_pre_visibility_cb(void *data, int type, void *event)
// {
// 	if (__context.ops.window.pre_visibility)
// 		__context.ops.window.pre_visibility(type, event, __context.data);

// 	return ECORE_CALLBACK_RENEW;
// }

static void __prepare_to_suspend(void)
{
	int suspend = APPCORE_BASE_SUSPENDED_STATE_WILL_ENTER_SUSPEND;

	if (appcore_base_is_bg_allowed() && !appcore_base_is_suspended()) {
		appcore_base_raise_event((void *)&suspend, APPCORE_BASE_EVENT_SUSPENDED_STATE_CHANGE);
		appcore_base_toggle_suspended_state();
	}
}

static void __exit_from_suspend(void)
{
	int suspend = APPCORE_BASE_SUSPENDED_STATE_DID_EXIT_FROM_SUSPEND;

	if (appcore_base_is_suspended()) {
		appcore_base_raise_event((void *)&suspend, APPCORE_BASE_EVENT_SUSPENDED_STATE_CHANGE);
		appcore_base_toggle_suspended_state();
	}
}

static void __do_pause(void)
{
	int r = -1;

	if (__context.state == AS_RUNNING) {
		if (__context.ops.pause) {
			// traceBegin(TTRACE_TAG_APPLICATION_MANAGER, "APPCORE:PAUSE");
			_DBG("Call pause callback");
			r = __context.ops.pause(__context.data);
			// traceEnd(TTRACE_TAG_APPLICATION_MANAGER);
		}

		if (r >= 0 && __context.resource_reclaiming)
			appcore_base_add_suspend_timer();

		__context.state = AS_PAUSED;
		__prepare_to_suspend();
	}
	// aul_status_update(STATUS_BG);
}

static void __do_resume(void)
{
	if (__context.state == AS_PAUSED || __context.state == AS_CREATED) {
		__exit_from_suspend();
		if (__context.ops.resume) {
			// LOG(LOG_DEBUG, "LAUNCH", "[%s:Application:resume:start]", __context.appid);
			// traceBegin(TTRACE_TAG_APPLICATION_MANAGER, "APPCORE:RESUME");
			_DBG("Call resume callback");
			__context.ops.resume(__context.data);
			// traceEnd(TTRACE_TAG_APPLICATION_MANAGER);
			// LOG(LOG_DEBUG, "LAUNCH", "[%s:Application:resume:done]", __context.appid);
		}
		__context.state = AS_RUNNING;
	}

	// aul_status_update(STATUS_VISIBLE);
}

// static GSList *__find_win(unsigned int win)
// {
// 	GSList *iter;
// 	struct win_node *t;

// 	for (iter = g_winnode_list; iter; iter = g_slist_next(iter)) {
// 		t = iter->data;
// 		if (t && t->win == win)
// 			return iter;
// 	}

// 	return NULL;
// }

static int __get_main_window(void)
{
	// struct win_node *entry = NULL;

	// if (g_winnode_list != NULL) {
	// 	entry = g_winnode_list->data;
	// 	return (unsigned int) entry->win;
	// }

	return 0;
}

static int __get_main_surface(void)
{
	// struct win_node *entry = NULL;

	// if (g_winnode_list != NULL) {
	// 	entry = g_winnode_list->data;
	// 	return (unsigned int) entry->surf;
	// }

	return 0;
}

static bool __add_win(unsigned int win, unsigned int surf)
{
	// struct win_node *t;
	// GSList *f;

	// _DBG("[EVENT_TEST][EVENT] __add_win WIN:%x\n", win);

	// f = __find_win(win);
	// if (f) {
	// 	errno = ENOENT;
	// 	_DBG("[EVENT_TEST][EVENT] ERROR There is already window : %x \n", win);
	// 	return FALSE;
	// }

	// t = calloc(1, sizeof(struct win_node));
	// if (t == NULL)
	// 	return FALSE;

	// t->win = win;
	// t->surf = surf;
	// t->vis = VT_NONE;

	// g_winnode_list = g_slist_append(g_winnode_list, t);

	return TRUE;
}

static bool __delete_win(unsigned int win)
{
	// GSList *f;

	// f = __find_win(win);
	// if (!f) {
	// 	errno = ENOENT;
	// 	_DBG("[EVENT_TEST][EVENT] ERROR There is no window : %x \n",
	// 			win);
	// 	return FALSE;
	// }

	// free(f->data);
	// g_winnode_list = g_slist_delete_link(g_winnode_list, f);

	return TRUE;
}

static bool __update_win(unsigned int win, unsigned int surf, int vis)
{
	// GSList *f;
	// struct win_node *t;

	// _DBG("[EVENT_TEST][EVENT] __update_win WIN:%x visibility %d\n",
	// 		win, vis);

	// f = __find_win(win);
	// if (!f) {
	// 	errno = ENOENT;
	// 	_DBG("[EVENT_TEST][EVENT] ERROR There is no window : %x \n", win);
	// 	return FALSE;
	// }

	// t = (struct win_node *)f->data;
	// t->win = win;
	// if (surf != 0)
	// 	t->surf = surf;
	// if (vis != VT_NONE)
	// 	t->vis = vis;

	return TRUE;
}

static void __raise_win(void)
{
	// Ecore_Wl2_Window *win;
	// unsigned int win_id;

	// if (!(__context.hint & APPCORE_UI_BASE_HINT_WINDOW_STACK_CONTROL))
	// 	return;

	// win_id = __get_main_window();

	// _DBG("Raise window: %d", win_id);
	// win = ecore_wl2_display_window_find(ecore_wl2_connected_display_get(NULL), win_id);
	// ecore_wl2_window_activate(win);
}

static void __pause_win(void)
{
	// Ecore_Wl2_Window *win;
	// GSList *wlist = g_winnode_list;
	// struct win_node *entry = NULL;

	// if (!(__context.hint & APPCORE_UI_BASE_HINT_WINDOW_STACK_CONTROL))
	// 	return;

	// _DBG("Pause window");

	// while (wlist) {
	// 	entry = wlist->data;

	// 	_DBG("Pause window: %d", entry->win);
	// 	win = ecore_wl2_display_window_find(ecore_wl2_connected_display_get(NULL), entry->win);
	// 	ecore_wl2_window_iconified_set(win, EINA_TRUE);

	// 	wlist = wlist->next;
	// }
}

static int __init_wl(void)
{
	return 0;
}

static void __finish_wl(void)
{
}

static void __set_bg_state(void)
{
	// if (!tz_policy && __init_wl() < 0)
	// 	return;

	// tizen_policy_set_background_state(tz_policy, getpid());
	// wl_display_roundtrip(dsp);
	__context.bg_state = true;
	_DBG("bg state: %d", __context.bg_state);
}

static void __unset_bg_state(void)
{
	// if (!tz_policy)
	// 	return;

	// tizen_policy_unset_background_state(tz_policy, getpid());
	// wl_display_roundtrip(dsp);
	__context.bg_state = false;
	_DBG("bg state: %d", __context.bg_state);
}

static void __do_start(bundle *b)
{
	const char *bg_launch;
	const char *below_app;
	const char *rpc_port;

	if (__context.hint & APPCORE_UI_BASE_HINT_WINDOW_STACK_CONTROL) {
		if (__context.below_app) {
			free(__context.below_app);
			__context.below_app = NULL;
		}

		below_app = bundle_get_val(b, AUL_SVC_K_RELOCATE_BELOW);
		if (below_app)
			__context.below_app = strdup(below_app);
	}

	if (first_launch) {
		first_launch = FALSE;
		return;
	}

	if (__context.hint & APPCORE_UI_BASE_HINT_BG_LAUNCH_CONTROL) {
		bg_launch = bundle_get_val(b, AUL_SVC_K_BG_LAUNCH);
		if (bg_launch && strcmp(bg_launch, "enable") == 0) {
			if (!__context.bg_state &&
					__context.state != AS_RUNNING)
				__set_bg_state();
		} else {
			if (__context.bg_state)
				__unset_bg_state();
		}
	}

	if (__context.hint & APPCORE_UI_BASE_HINT_WINDOW_AUTO_CONTROL) {
		if (!__context.bg_state) {
			rpc_port = bundle_get_val(b, AUL_K_RPC_PORT);
			if (!rpc_port)
				__raise_win();
		}
	}
}

static int __is_legacy_lifecycle(void)
{
	return 0;
}

EXPORT_API int appcore_ui_base_on_receive(aul_type type, bundle *b)
{
	if (__context.state == AS_DYING) {
		_ERR("Skip the event in dying state");
		return 0;
	}

	if (type == AUL_TERMINATE_BGAPP && __context.state != AS_PAUSED)
		return 0;

	if (type == AUL_START)
		__exit_from_suspend();

	appcore_base_on_receive(type, b);

	switch (type) {
	case AUL_START:
		__do_start(b);
		if (__context.hint & APPCORE_UI_BASE_HINT_LEGACY_CONTROL) {
			if (!__context.bg_state && __is_legacy_lifecycle()) {
				_DBG("Legacy lifecycle");
				__do_resume();
			}
		}
		break;
	case AUL_RESUME:
		if (__context.bg_state)
			__unset_bg_state();
		__raise_win();
		break;
	case AUL_TERMINATE:
		break;
	case AUL_TERMINATE_BGAPP:
		_DBG("[APP %d] is paused. TERMINATE", getpid());
		__context.state = AS_DYING;
		// aul_status_update(STATUS_DYING);
		if (__context.ops.base.exit)
			__context.ops.base.exit(__context.data);
		break;
	case AUL_PAUSE:
		__pause_win();
		break;
	default:
		break;
	}

	return 0;
}

static void __add_ecore_events(void)
{
#if 0
	__context.hshow = ecore_event_handler_add(ECORE_WL2_EVENT_WINDOW_SHOW, __stub_show_cb, NULL);
	if (!__context.hshow)
		_ERR("Failed to add ECORE_WL_EVENT_WINDOW_SHOW event");

	__context.hhide = ecore_event_handler_add(ECORE_WL2_EVENT_WINDOW_HIDE, __stub_hide_cb, NULL);
	if (!__context.hhide)
		_ERR("Failed to add ECORE_WL_EVENT_WINDOW_HIDE event");

	__context.hvchange = ecore_event_handler_add(ECORE_WL2_EVENT_WINDOW_VISIBILITY_CHANGE, __stub_visibility_cb, NULL);
	if (!__context.hvchange)
		_ERR("Failed to add ECORE_WL_EVENT_WINDOW_VISIBILITY_CHANGE event");

	__context.hlower = ecore_event_handler_add(ECORE_WL2_EVENT_WINDOW_LOWER, __stub_lower_cb, NULL);
	if (!__context.hlower)
		_ERR("Failed to add ECORE_WL_EVENT_WINDOW_LOWER event");

	__context.hpvchange = ecore_event_handler_add(ECORE_WL2_EVENT_WINDOW_PRE_VISIBILITY_CHANGE, __stub_pre_visibility_cb, NULL);
	if (!__context.hpvchange)
		_ERR("Failed to add ECORE_WL_EVENT_WINDOW_PRE_VISIBILITY_CHANGE event");
#endif
}

static void __del_ecore_events(void)
{
#if 0
	if (__context.hshow) {
		ecore_event_handler_del(__context.hshow);
		__context.hshow = NULL;
	}

	if (__context.hhide) {
		ecore_event_handler_del(__context.hhide);
		__context.hhide = NULL;
	}

	if (__context.hvchange) {
		ecore_event_handler_del(__context.hvchange);
		__context.hvchange = NULL;
	}

	if (__context.hlower) {
		ecore_event_handler_del(__context.hlower);
		__context.hlower = NULL;
	}

	if (__context.hpvchange) {
		ecore_event_handler_del(__context.hpvchange);
		__context.hpvchange = NULL;
	}
#endif
}

EXPORT_API int appcore_ui_base_on_create(void)
{
	__add_ecore_events();
	appcore_base_on_create();
	__context.state = AS_CREATED;
	// LOG(LOG_DEBUG, "LAUNCH", "[%s:Application:create:done]", __context.appid);

	return 0;
}

EXPORT_API int appcore_ui_base_on_terminate(void)
{
	if (__context.state == AS_RUNNING) {
		if (__context.ops.pause) {
			_DBG("Call pause callback");
			__context.ops.pause(__context.data);
		}
	}

	__context.state = AS_DYING;

	appcore_base_on_terminate();

	return 0;
}

EXPORT_API int appcore_ui_base_on_pause(void)
{
	return 0;
}

EXPORT_API int appcore_ui_base_on_resume(void)
{
	return 0;
}

EXPORT_API int appcore_ui_base_on_control(bundle *b)
{
	// LOG(LOG_DEBUG, "LAUNCH", "[%s:Application:reset:start]", __context.appid);
	appcore_base_on_control(b);
	// LOG(LOG_DEBUG, "LAUNCH", "[%s:Application:reset:done]", __context.appid);

	return 0;
}

EXPORT_API int appcore_ui_base_on_trim_memory(void)
{
	return appcore_base_on_trim_memory();
}

static void __group_attach()
{
	if (!(__context.hint & APPCORE_UI_BASE_HINT_WINDOW_GROUP_CONTROL))
		return;

	appcore_ui_base_group_add();
}

static void __group_lower()
{
	if (!(__context.hint & APPCORE_UI_BASE_HINT_WINDOW_GROUP_CONTROL))
		return;
	appcore_ui_base_group_remove();
}

EXPORT_API void appcore_ui_base_group_add(void)
{
	static bool attached = false;

	_DBG("__group_attach");
	if (attached)
		return;

	int wid = __get_main_surface();
	if (wid == 0) {
		_ERR("window wasn't ready");
		return;
	}

	aul_app_group_set_window(wid);
	attached = true;
}

EXPORT_API void appcore_ui_base_group_remove(void)
{
	int exit = 0;

	_DBG("__group_lower");
	aul_app_group_lower(&exit);
	if (exit) {
		_DBG("__group_lower : sub-app!");
		if (__context.ops.base.exit)
			__context.ops.base.exit(__context.data);
	}
}

EXPORT_API void appcore_ui_base_window_on_show(int type, void *event)
{
	// Ecore_Wl2_Event_Window_Show *ev;

	// ev = event;
	// if (ev->parent_win != 0) {
	// 	/* This is child window. Skip!!! */
	// 	return;
	// }

	// _DBG("[EVENT_TEST][EVENT] GET SHOW EVENT!!!. WIN:%x, %d\n",
	// 		ev->win, ev->data[0]);

	// if (!__find_win((unsigned int)ev->win)) {
	// 	__add_win((unsigned int)ev->win, (unsigned int)ev->data[0]);
	// } else {
	// 	__update_win((unsigned int)ev->win, (unsigned int)ev->data[0],
	// 			VT_NONE);
	// }

	// if (ev->data[0] != 0)
	// 	__group_attach();
}

static bool __check_visible(void)
{
	// GSList *iter = NULL;
	// struct win_node *entry = NULL;

	// _DBG("[EVENT_TEST][EVENT] __check_visible\n");

	// for (iter = g_winnode_list; iter != NULL; iter = g_slist_next(iter)) {
	// 	entry = iter->data;
	// 	_DBG("win : %x visibility : %d\n", entry->win, entry->vis);
	// 	if (entry->vis == VT_UNOBSCURED)
	// 		return true;
	// }

	return false;
}

EXPORT_API void appcore_ui_base_window_on_hide(int type, void *event)
{
	// Ecore_Wl2_Event_Window_Hide *ev;
	// int bvisibility;

	// ev = event;
	// _DBG("[EVENT_TEST][EVENT] GET HIDE EVENT!!!. WIN:%x\n", ev->win);

	// if (__find_win((unsigned int)ev->win)) {
	// 	__delete_win((unsigned int)ev->win);
	// 	bvisibility = __check_visible();
	// 	if (!bvisibility && w_status != WS_PAUSE) {
	// 		_DBG(" Go to Pasue state \n");
	// 		w_status = WS_PAUSE;
	// 		__do_pause();
	// 	}
	// }
}

EXPORT_API void appcore_ui_base_window_on_lower(int type, void *event)
{
	// Ecore_Wl2_Event_Window_Lower *ev;

	// ev = event;
	// if (!ev)
	// 	return;
	// _DBG("ECORE_WL2_EVENT_WINDOW_LOWER window id:%u\n", ev->win);
	// __group_lower();
}

EXPORT_API void appcore_ui_base_window_on_visibility(int type, void *event)
{
	// Ecore_Wl2_Event_Window_Visibility_Change *ev;
	// int bvisibility;

	// ev = event;
	// __update_win((unsigned int)ev->win, 0,
	// 		ev->fully_obscured ? VT_FULLY_OBSCURED : VT_UNOBSCURED);
	// bvisibility = __check_visible();

	// _DBG("bvisibility %d, w_status %d", bvisibility, w_status);

	// if (bvisibility && (__context.hint & APPCORE_UI_BASE_HINT_WINDOW_STACK_CONTROL) &&
	// 		__context.below_app) {
	// 	aul_app_group_activate_below(__context.below_app);
	// 	free(__context.below_app);
	// 	__context.below_app = NULL;
	// }

	// if (bvisibility && w_status != WS_RESUME) {
	// 	_DBG(" Go to Resume state\n");
	// 	w_status = WS_RESUME;
	// 	__do_resume();
	// } else if (!bvisibility && w_status != WS_PAUSE) {
	// 	_DBG(" Go to Pasue state \n");
	// 	w_status = WS_PAUSE;
	// 	__do_pause();
	// } else {
	// 	_DBG(" No change state \n");
	// }

}

EXPORT_API void appcore_ui_base_window_on_pre_visibility(int type, void *event)
{
	// Ecore_Wl2_Event_Window_Pre_Visibility_Change *ev = event;
	// bool bvisibility;

	// if (ev && ev->type == ECORE_WL2_WINDOW_VISIBILITY_TYPE_PRE_UNOBSCURED) {
	// 	__update_win((unsigned int)ev->win, 0, VT_UNOBSCURED);
	// 	bvisibility = __check_visible();

	// 	_DBG("bvisibility %d, w_status %d", bvisibility, w_status);
	// 	if (bvisibility && w_status != WS_RESUME) {
	// 		_DBG(" Go to Resume state\n");
	// 		w_status = WS_RESUME;
	// 		__do_resume();
	// 	}
	// }
}

EXPORT_API int appcore_ui_base_init(appcore_ui_base_ops ops, int argc, char **argv,
		void *data, unsigned int hint)
{
	const char *bg_launch;
	bundle *b;
	char appid[32] = {0, };
	int ret;

	// if (!ecore_wl2_init()) {
	// 	_ERR("could not wl2 init");
	// 	return -1;
	// }

	// ecore_wl2_display_connect(NULL);
	// appcore_ui_plugin_init(&ops, argc, argv, &hint);

	// ui_start(); ??

	snprintf(appid, sizeof(appid), "appid-%d", getpid());

	__context.ops = ops;
	__context.data = data;
	__context.argc = argc;
	__context.argv = argv;
	__context.hint = hint;
	__context.state = AS_NONE;
	__context.appid = "";//strdup(appid);
	__context.resource_reclaiming = true;

	// LOG(LOG_DEBUG, "LAUNCH", "[%s:Application:main:done]", appid);
	if (__context.hint & APPCORE_UI_BASE_HINT_BG_LAUNCH_CONTROL) {
		b = bundle_import_from_argv(argc, argv);
		if (b) {
			bg_launch = bundle_get_val(b, AUL_SVC_K_BG_LAUNCH);
			if (bg_launch && strcmp(bg_launch, "enable") == 0)
				__set_bg_state();

			bundle_free(b);
		}
	}

	return appcore_base_init(ops.base, argc, argv, data);
}

EXPORT_API void appcore_ui_base_fini(void)
{
	__del_ecore_events();
	// __finish_wl();

	free(__context.appid);
	__context.appid = NULL;

	appcore_base_fini();
	// appcore_ui_plugin_fini();
	// _ERR("disconnect wl2_display");
	// ecore_wl2_display_disconnect(ecore_wl2_connected_display_get(NULL));
	// ecore_wl2_shutdown();
}

EXPORT_API void appcore_ui_base_pause(void)
{
	__do_pause();
}

EXPORT_API void appcore_ui_base_resume(void)
{
	__do_resume();
}

EXPORT_API bool appcore_ui_base_is_resumed(void)
{
	return __context.state == AS_RUNNING;
}

EXPORT_API void appcore_ui_base_exit(void)
{
	appcore_base_exit();
}

EXPORT_API unsigned int appcore_ui_base_get_main_window(void)
{
	return __get_main_window();
}

EXPORT_API unsigned int appcore_ui_base_get_main_surface(void)
{
	return __get_main_surface();
}

EXPORT_API int appcore_ui_base_get_hint(void)
{
	return __context.hint;
}

EXPORT_API bool appcore_ui_base_get_bg_state(void)
{
	return __context.bg_state;
}

EXPORT_API void appcore_ui_base_set_bg_state(bool bg_state)
{
	__context.bg_state = bg_state;
}

EXPORT_API void appcore_ui_base_set_system_resource_reclaiming(bool enable)
{
	__context.resource_reclaiming = enable;
}

static int __on_receive(aul_type type, bundle *b, void *data)
{
	return appcore_ui_base_on_receive(type, b);
}

static int __on_create(void *data)
{
	return appcore_ui_base_on_create();
}

static int __on_terminate(void *data)
{
	return appcore_ui_base_on_terminate();
}

static void __on_init(int argc, char **argv, void *data)
{
	// Main loop / UI init
	ui_start();
}

static void __on_finish(void)
{
	// Main loop / UI finish
	ui_stop();
}

static void __on_run(void *data)
{
	// Main loop run
	eventloop_loop_run();
}

static void __on_exit(void *data)
{
	// Main loop exit
	eventloop_loop_stop();
}

static int __on_pause(void *data)
{
	return appcore_ui_base_on_pause();
}

static int __on_resume(void *data)
{
	return appcore_ui_base_on_resume();
}

static void __window_on_show(int type, void *event, void *data)
{
	appcore_ui_base_window_on_show(type, event);
}

static void __window_on_hide(int type, void *event, void *data)
{
	appcore_ui_base_window_on_hide(type, event);
}

static void __window_on_lower(int type, void *event, void *data)
{
	appcore_ui_base_window_on_lower(type, event);
}

static void __window_on_visibility(int type, void *event, void *data)
{
	appcore_ui_base_window_on_visibility(type, event);
}

static void __window_on_pre_visibility(int type, void *event, void *data)
{
	appcore_ui_base_window_on_pre_visibility(type, event);
}

EXPORT_API appcore_ui_base_ops appcore_ui_base_get_default_ops(void)
{
	appcore_ui_base_ops ops;

	ops.base = appcore_base_get_default_ops();

	/* override methods */
	ops.base.create = __on_create;
	ops.base.terminate = __on_terminate;
	ops.base.receive = __on_receive;

	ops.base.init = __on_init;
	ops.base.finish = __on_finish;
	ops.base.run = __on_run;
	ops.base.exit = __on_exit;

	ops.pause = __on_pause;
	ops.resume = __on_resume;
	ops.window.show = __window_on_show;
	ops.window.hide = __window_on_hide;
	ops.window.lower = __window_on_lower;
	ops.window.visibility = __window_on_visibility;
	ops.window.pre_visibility = __window_on_pre_visibility;

	return ops;
}


