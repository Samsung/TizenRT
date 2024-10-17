/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
 * @file csifw/csifw_log.h
 * @brief This file defines macros used to add debug logs in AI Framework and it's applications.
 *
 * Log levels are Error, Information and Verbose. Error is by default enabled.
 */

#pragma once

#include <stdio.h>
#include <string.h>

static const char *file_name = 0;
#define TAG "[CSIFW]"
#define TAG_CSIFW_UTILS "[CSIFW_UTILS]"
#define _FILE_NAME file_name ? file_name : (file_name = strrchr(__FILE__, '/') ? (char *)(strrchr(__FILE__, '/') + 1) : __FILE__)

#define PRINT_LOG(color, tag, ...)                                           \
	do {                                                                     \
		printf(color tag "[%s]::%s():%d: ", _FILE_NAME, __func__, __LINE__); \
		printf(__VA_ARGS__);                                                 \
		printf("\033[m\n");                                                  \
	} while (0)

#ifdef CONFIG_CSIFW_LOGE
#define CSIFW_LOGE(...) PRINT_LOG("\033[31mE", TAG, __VA_ARGS__)
#else
#define CSIFW_LOGE(...)
#endif

#ifdef CONFIG_CSIFW_LOGI
#define CSIFW_LOGI(...) PRINT_LOG("\033[32mI", TAG, __VA_ARGS__)
#else
#define CSIFW_LOGI(...)
#endif

#ifdef CONFIG_CSIFW_LOGV
#define CSIFW_LOGV(...) PRINT_LOG("\033[mV", TAG, __VA_ARGS__)
#else
#define CSIFW_LOGV(...)
#endif

#ifdef CONFIG_CSIFW_LOGD
#define CSIFW_LOGD(...) PRINT_LOG("\033[33mD", TAG, __VA_ARGS__)
#else
#define CSIFW_LOGD(...)
#endif