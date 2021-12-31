/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License\n");
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
#pragma once

#ifndef __LINUX__
#define WT_LOG(tag, fmt, args...) \
	printf(tag "[T%d] " fmt "\t%s:%d\n", getpid(), ##args, __FUNCTION__, __LINE__)

#define WT_LOGE(tag, fmt, args...) \
	printf(tag "[ERR][T%d] " fmt "\t%s:%d\n", getpid(), ##args, __FUNCTION__, __LINE__)

#define WT_LOGP(tag, fmt, args...) \
	printf(fmt, ##args)
#else
#define WT_LOG(tag, fmt, args...) \
	printf(tag "[INFO] " fmt "\t%s:%d\n", ##args, __FUNCTION__, __LINE__)

#define WT_LOGE(tag, fmt, args...) \
	printf(tag "[ERR] " fmt "\t%s:%d\n", ##args, __FUNCTION__, __LINE__)

#define WT_LOGP(tag, fmt, args...) \
	printf(fmt, ##args)
#endif

#define WT_ENTER            \
	do {                    \
		WT_LOG(TAG, "-->"); \
	} while (0)

#define WT_LEAVE            \
	do {                    \
		WT_LOG(TAG, "<--"); \
	} while (0)
