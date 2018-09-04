/*-*- Mode: C; c-basic-offset: 8; indent-tabs-mode: t -*-
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

#include <dlog.h>
#include <syslog.h>

int dlog_vprint(log_priority prio, const char *tag, const char *fmt, va_list ap)
{
	/* TODO : handle tag */

	int priority;
	switch (prio) {
	case DLOG_PRIO_MAX:
		priority = LOG_EMERG;
	case DLOG_SILENT:
		priority = LOG_ALERT;
		break;
	case DLOG_FATAL:
		priority = LOG_CRIT;
		break;
	case DLOG_ERROR:
		priority = LOG_ERR;
		break;
	case DLOG_WARN:
		priority = LOG_WARNING;
		break;
	case DLOG_INFO:
		priority = LOG_INFO;
		break;
	default:
		priority = LOG_DEBUG;
		break;
	}
	return vsyslog(priority, fmt, ap);
}

int dlog_print(log_priority prio, const char *tag, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	int ret = dlog_vprint(prio, tag, fmt, ap);
	va_end(ap);

	return ret;
}
