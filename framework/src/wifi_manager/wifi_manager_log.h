/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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

#ifndef _WIFI_MANAGER_LOG_H__
#define _WIFI_MANAGER_LOG_H__
#include <tinyara/config.h>
/**
 * Logging
 */
#define WM_TAG "[WM]"

#if defined(CONFIG_WIFI_MANAGER_USE_PRINTF)
#define WM_LOG_VERBOSE printf
#define WM_LOG_DEBUG printf
#define WM_LOG_ERROR printf
#else
#define WM_LOG_VERBOSE nvdbg
#define WM_LOG_DEBUG ndbg
#define WM_LOG_ERROR ndbg
#endif


#define WM_ENTER WM_LOG_VERBOSE(WM_TAG"T%d\t%s:%d\n", getpid(), __FUNCTION__, __LINE__);

#define WM_LEAVE													\
	do {															\
		WM_LOG_VERBOSE(WM_TAG"<---%s:%d\n", __FILE__, __LINE__);	\
	} while (0)

#define WM_ERR												\
	do {													\
		WM_LOG_ERROR(WM_TAG"[ERR] %s: %d line err(%s)\n",	\
					 __FILE__, __LINE__, strerror(errno));	\
	} while (0)

#define WM_LOG_HANDLER_START WM_LOG_VERBOSE(WM_TAG "T%d %s:%d state(%s) evt(%s)\n", \
											getpid(), __FUNCTION__, __LINE__, \
											wifimgr_get_state_str(g_manager_info.state), \
											wifimgr_get_evt_str(msg->event));

#endif // _WIFI_MANAGER_LOG_H__
