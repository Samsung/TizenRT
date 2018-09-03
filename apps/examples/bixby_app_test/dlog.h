/*
 * DLOG
 * Copyright (c) 2005-2008, The Android Open Source Project
 * Copyright (c) 2012-2013 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef	_DLOG_H_
#define	_DLOG_H_

#include <string.h>
#include <stdarg.h>
#include <tizen_error.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define LOG_TAG "BIXBY"
#define LOGE(fmt, arg...) dlog_print(DLOG_ERROR, LOG_TAG, "%s (%d) %s : " fmt, __FILE__, __LINE__, __FUNCTION__, ##arg)

typedef enum {
	DLOG_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	DLOG_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	DLOG_ERROR_NOT_PERMITTED = TIZEN_ERROR_NOT_PERMITTED /**< Operation not permitted */
} dlog_error_e;

typedef enum {
	DLOG_UNKNOWN = 0, /**< Keep this always at the start */
	DLOG_DEFAULT, /**< Default */
	DLOG_VERBOSE, /**< Verbose */
	DLOG_DEBUG, /**< Debug */
	DLOG_INFO, /**< Info */
	DLOG_WARN, /**< Warning */
	DLOG_ERROR, /**< Error */
	DLOG_FATAL, /**< Fatal */
	DLOG_SILENT, /**< Silent */
	DLOG_PRIO_MAX /**< Keep this always at the end. */
} log_priority;

int dlog_print(log_priority prio, const char *tag, const char *fmt, ...);

int dlog_vprint(log_priority prio, const char *tag, const char *fmt, va_list ap);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _DLOG_H_*/
