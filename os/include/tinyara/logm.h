/****************************************************************************
 *
 * Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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
  * @defgroup LOGM_KERNEL LOGM
  * @ingroup KERNEL
  */

///@file tinyara/logm.h
///@brief logm APIs

#ifndef __OS_INCLUDE_TINYARA_LOGM_H
#define __OS_INCLUDE_TINYARA_LOGM_H

#include <stdarg.h>

#define LOGM_DEF_PRIORITY (7)
/* Log priority levels in logm */

enum logm_loglevel_e {
	LOGM_EMR, /* Emergency */
	LOGM_ART, /* Alert */
	LOGM_CRT, /* Critical */
	LOGM_ERR, /* Error */
	LOGM_WRN, /* Warning */
	LOGM_NTCE,/* Notice */
	LOGM_INF, /* Info   */
	LOGM_DBG, /* Debug */
	LOGM_OFF  /* Is this needed? */
};

enum logm_param_type_e {
	LOGM_BUFSIZE,
	LOGM_INTERVAL,
	LOGM_PRIORITY
	/* This would grow later */
};

/* Log flags for somethings related to logging */
enum logm_logflag_e {
	LOGM_NORMAL,
	LOGM_LOWPUT
};

/* Log index means where messages are from */
enum logm_logindex_e {
	LOGM_UNKNOWN
	/* Not supported yet. This would be updated later */
};

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#  define EXTERN extern
#endif
/**
 * @cond
 * @internal
 */
void logm_start(void);
/**
 * @internal
 */
int logm_internal(int flag, int indx, int priority, const char *fmt, va_list valst);
/**
 * @internal
 */
int logm(int flag, int indx, int priority, const char *fmt, ...);
/**
 * @internal
 */
int logm_set_values(enum logm_param_type_e type, int value);
/**
 * @internal
 */
int logm_get_values(enum logm_param_type_e type, int* value);
/**
 * @endcond
 */
#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __OS_INCLUDE_TINYARA_LOGM_H */

