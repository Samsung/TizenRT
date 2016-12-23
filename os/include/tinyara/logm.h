/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

#ifndef __OS_INCLUDE_TINYARA_LOGM_H
#define __OS_INCLUDE_TINYARA_LOGM_H

#include <stdarg.h>

#define LOGM_DEF_PRIORITY (7)
/* Log priority levels in logm */

enum logm_loglevels_e {
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

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#  define EXTERN extern
#endif

void logm_start(void);
int logm_internal(int priority, const char *fmt, va_list valst);
int logm(int flag, int mod, int priority, const char *fmt, ...);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __OS_INCLUDE_TINYARA_LOGM_H */

