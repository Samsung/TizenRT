// Copyright 2022-2025 Beken
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

/**
 * Ideally soc should NOT depends on any other modules, but it's really helpful
 * if we can debug soc directly.
 * For ones who want to port soc to other platform, just need to implement following
 * interfaces.
 */
#include <components/log.h>
#include <sdkconfig.h>

#if CONFIG_SOC_BK7234
#include "soc/bk7234/soc.h"
#elif CONFIG_SOC_BK7236
#include "soc/bk7236/soc.h"
#elif CONFIG_SOC_BK7239
#include "soc/bk7239n/soc.h"
#endif

