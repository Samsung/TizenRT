/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#ifndef __UI_DEBUG_INTERNAL_H__
#define __UI_DEBUG_INTERNAL_H__

#include <debug.h>
#include <assert.h>

#ifdef CONFIG_DEBUG_UI_INFO
#define UI_LOGD(format, ...) uivdbg("[Line:%d] "format, __LINE__, ##__VA_ARGS__)
#else
#define UI_LOGD(...)
#endif

#ifdef CONFIG_DEBUG_UI_WARN
#define UI_LOGW(format, ...) uiwdbg("\e[33m[Line:%d] "format "\e[m", __LINE__, ##__VA_ARGS__)
#else
#define UI_LOGW(...)
#endif

#ifdef CONFIG_DEBUG_UI_ERROR
#define UI_LOGE(format, ...) uidbg("\e[35m[Line:%d] "format "\e[m", __LINE__, ##__VA_ARGS__)
#else
#define UI_LOGE(...)
#endif

#define UI_ASSERT assert

#endif
