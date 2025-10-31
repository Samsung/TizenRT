// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <components/log.h>

#define CALENDAR_TAG "calendar"
#define CALENDAR_LOGI(...) BK_LOGI(CALENDAR_TAG, ##__VA_ARGS__)
#define CALENDAR_LOGW(...) BK_LOGW(CALENDAR_TAG, ##__VA_ARGS__)
#define CALENDAR_LOGE(...) BK_LOGE(CALENDAR_TAG, ##__VA_ARGS__)
#define CALENDAR_LOGD(...) BK_LOGD(CALENDAR_TAG, ##__VA_ARGS__)

