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

#pragma once

#define LOG_TAG "APP_CORE_BASE"

#include <stdio.h>
#include <stdbool.h>
#include <dlog.h>

typedef enum {
	TIZEN_PROFILE_UNKNOWN = 0,
	TIZEN_PROFILE_MOBILE = 0x1,
	TIZEN_PROFILE_WEARABLE = 0x2,
	TIZEN_PROFILE_TV = 0x4,
	TIZEN_PROFILE_IVI = 0x8,
	TIZEN_PROFILE_COMMON = 0x10,
} appcore_base_tizen_profile_t;

#ifndef EXPORT_API
#  define EXPORT_API __attribute__ ((visibility("default")))
#endif

#ifndef _DLOG_H_
#  define _ERR(fmt, arg...) \
	do { fprintf(stderr, "appcore: "fmt"\n", ##arg); } while (0)

#  define _INFO(fmt, arg...) \
	do { fprintf(stdout, fmt"\n", ##arg); } while (0)

#  define _DBG(fmt, arg...) \
	do { \
		if (getenv("APPCORE_DEBUG")) { \
			fprintf(stdout,	fmt"\n", ##arg); \
		} \
	} while (0)
#else
#  define _ERR(fmt, arg...) \
	do { \
		fprintf(stderr, "appcore: "fmt"\n", ##arg); \
		LOGE(fmt, ##arg); \
	} while (0)
#  define _INFO(...) LOGI(__VA_ARGS__)
#  define _DBG(...) LOGD(__VA_ARGS__)
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

appcore_base_tizen_profile_t appcore_base_get_tizen_profile(void);

#define TIZEN_FEATURE_BACKGROUND_MANAGEMENT \
	(!(appcore_base_get_tizen_profile() & TIZEN_PROFILE_TV))
#define TIZEN_FEATURE_CHARGER_STATUS \
	(appcore_base_get_tizen_profile() & TIZEN_PROFILE_WEARABLE)

extern void aul_finalize();

