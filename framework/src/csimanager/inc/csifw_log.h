/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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

#pragma once

#include <stdio.h>
#include <string.h>
#include <tinyara/config.h>

#define LOG_TAG "[CSIFW]"
#define _FILE (strrchr(__FILE__, '/') ? (char *)(strrchr(__FILE__, '/') + 1) : __FILE__)

#define PRINT_CSI_LOG(color, tag, ...)                        \
    do {                                        \
        printf(color tag "[%s]::%s():%d: ", _FILE, __func__, __LINE__);        \
        printf(__VA_ARGS__);                                                     \
        printf("\033[m\n");                            \
    } while (0)

#ifdef CONFIG_DEBUG_WIFICSI_ERROR
#define CSIFW_LOGE(...) PRINT_CSI_LOG("\033[31mE", LOG_TAG, __VA_ARGS__)
#else
#define CSIFW_LOGE(...)
#endif

#ifdef CONFIG_DEBUG_WIFICSI_WARN
#define CSIFW_LOGD(...) PRINT_CSI_LOG("\033[33mW", LOG_TAG, __VA_ARGS__)
#else
#define CSIFW_LOGD(...)
#endif

#ifdef CONFIG_DEBUG_WIFICSI_INFO
#define CSIFW_LOGI(...) PRINT_CSI_LOG("\033[32mI", LOG_TAG, __VA_ARGS__)
#define CSIFW_LOGV(...) PRINT_CSI_LOG("\033[32mI", LOG_TAG, __VA_ARGS__)
#else
#define CSIFW_LOGI(...)
#define CSIFW_LOGV(...)
#endif
