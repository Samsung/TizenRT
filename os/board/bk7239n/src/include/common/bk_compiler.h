// Copyright 2020-2022 Beken
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

#define __BK_INLINE          static inline
#define __BK_STATIC          static
#define __BK_SECTION(x)      __attribute__((section(x)))
#define __bk_deprecated      __attribute__((deprecated))
#define __bk_weak            __attribute__((weak))
#define __bk_must_check      __attribute__((warn_unused_result))
#define __bk_packed          __attribute__((packed))
