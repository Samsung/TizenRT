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

#include <components/log.h>
#include "sdkconfig.h"

#if defined(CONFIG_SOC_BK7236N)
#include "partition_bk7236n.h"
#elif defined(CONFIG_SOC_BK7236XX)
#include "partition_bk7236xx.h"
#elif defined(CONFIG_SOC_BK7239XX)
#include "partition_bk7239xx.h"
#else
#include "partition_bk7236xx.h"
#endif
