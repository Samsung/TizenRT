/*
 * bundle
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Jayoun Lee <airjany@samsung.com>, Sewook Park <sewook7.park@samsung.com>,
 * Jaeho Lee <jaeho81.lee@samsung.com>
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


#include <debug.h>
#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "BUNDLE"

#ifdef _DEBUG_MODE_
#define BUNDLE_LOG_PRINT(FMT, ARG...)  do { printf("%5d", getpid()); printf
	("%s() : "FMT"\n", __FUNCTION__, ##ARG); } while (false)
#define BUNDLE_EXCEPTION_PRINT(FMT, ARG...)  do { printf("%5d", getpid()); 
	printf("%s() : "FMT"\n", __FUNCTION__, ##ARG); } while (false)
#define BUNDLE_ASSERT_PRINT(FMT, ARG...) do { printf("%5d", getpid()); printf
	("%s() : "FMT"\n", __FUNCTION__, ##ARG); } while (false)
#else
#define BUNDLE_LOG_PRINT(FMT, ARG...) dbg(FMT, ##ARG);
#define BUNDLE_EXCEPTION_PRINT(FMT, ARG...) dbg(FMT, ##ARG);
#define BUNDLE_ASSERT_PRINT(FMT, ARG...) dbg(FMT, ##ARG);
#endif
