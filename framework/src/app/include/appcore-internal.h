/*
 *  app-core
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Jayoun Lee <airjany@samsung.com>, Sewook Park <sewook7.park@samsung.com>, Jaeho Lee <jaeho81.lee@samsung.com>
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
 *
 */



#ifndef __APPCORE_INTERNAL_H__
#define __APPCORE_INTERNAL_H__

#define LOG_TAG "APP_CORE"

#include <stdio.h>
#include <stdbool.h>
#include <dlog.h>
#include "appcore-common.h"


#ifndef EXPORT_API
#  define EXPORT_API __attribute__ ((visibility("default")))
#endif

#ifndef _DLOG_H_
#define _ERR(fmt, arg...) fprintf(stderr, "appcore: "fmt"\n", ##arg)
#define _INFO(fmt, arg...) fprintf(stdout, fmt"\n", ##arg)
#define _DBG(fmt, arg...) \
	do { \
		if (getenv("APPCORE_DEBUG")) { \
			fprintf(stdout,	fmt"\n", ##arg); \
		} \
	} while (0)
#else
#define _ERR(fmt, arg...) LOGE(fmt, ##arg)
#define _INFO(...) LOGI(__VA_ARGS__)
#define _DBG(...) LOGD(__VA_ARGS__)
#endif

#define _warn_if(expr, fmt, arg...) do { \
		if (expr) { \
			_ERR(fmt, ##arg); \
		} \
	} while (0)

#define _ret_if(expr) do { \
		if (expr) { \
			return; \
		} \
	} while (0)

#define _retv_if(expr, val) do { \
		if (expr) { \
			return (val); \
		} \
	} while (0)

#define _retm_if(expr, fmt, arg...) do { \
		if (expr) { \
			_ERR(fmt, ##arg); \
			return; \
		} \
	} while (0)

#define _retvm_if(expr, val, fmt, arg...) do { \
		if (expr) { \
			_ERR(fmt, ##arg); \
			return (val); \
		} \
	} while (0)

/**
 * Appcore internal state
 */
enum app_state {
	AS_NONE,
	AS_CREATED,
	AS_RUNNING,
	AS_PAUSED,
	AS_DYING,
};

/**
 * Appcore internal event
 */
enum app_event {
	AE_UNKNOWN,
	AE_CREATE,
	AE_TERMINATE,
	AE_TERMINATE_BGAPP,
	AE_PAUSE,
	AE_RESUME,
	AE_RAISE,
	AE_LOWER,
	AE_RESET,
	AE_LOWMEM_POST,
	AE_MEM_FLUSH,
	AE_UPDATE_REQUESTED,
	AE_MAX
};

/**
 * Appcore internal system event
 */
enum sys_event {
	SE_UNKNOWN,
	SE_LOWMEM,
	SE_LOWBAT,
	SE_LANGCHG,
	SE_REGIONCHG,
	SE_SUSPENDED_STATE,
	SE_UPDATE_REQUESTED,
	SE_MAX
};

/**
 * Appcore system event operation
 */
struct sys_op {
	int (*func) (void *, void *);
	void *data;
};

/**
 * Appcore internal structure
 */
struct appcore {
	int state;
	unsigned int tid;
	bool suspended_state;
	bool allowed_bg;

	const struct ui_ops *ops;
	struct sys_op sops[SE_MAX];
};

/**
 * Appcore UI operation
 */
struct ui_ops {
	void *data;
	void (*cb_app) (enum app_event evnt, void *data, bundle *b);
};

struct ui_wm_rotate {
	int (*set_rotation_cb) (int (*cb)(void *event_info, enum appcore_rm, void *), void *data);
	int (*unset_rotation_cb) (void);
	int (*get_rotation_state) (enum appcore_rm *curr);
	int (*pause_rotation_cb) (void);
	int (*resume_rotation_cb) (void);
};
void appcore_group_attach();
void appcore_group_lower();
unsigned int appcore_get_main_window(void);
unsigned int appcore_get_main_surface(void);

#define ENV_START "APP_START_TIME"

#endif				/* __APPCORE_INTERNAL_H__ */
