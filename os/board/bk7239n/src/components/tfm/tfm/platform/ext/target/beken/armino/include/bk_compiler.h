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

#define __BK_INLINE                     static inline

/// function returns struct in registers (4 words max, var with gnuarm)
#if !defined(__VIR)
#define __VIR                        __value_in_regs
#endif

/// function has no side effect and return depends only on arguments
#if !defined(__PURE)
#define __PURE                       __pure
#endif

/// Align instantiated lvalue or struct member on 4 bytes
#if !defined(__ALIGN4)
#define __ALIGN4                     __attribute__((aligned(4)))
#endif

#define __MODULE__                   __BASE_FILE__

/// define the BLE IRQ handler attribute for this compiler
#define __BLEIRQ

/// define size of an empty array (used to declare structure with an array size not defined)
#define __ARRAY_EMPTY

/// define the static keyword for this compiler
#define __STATIC static

/// Pack a structure field
#define __PACKED16                   __attribute__( ( packed ) )
#if !defined(__PACKED)
#define __PACKED                     __attribute__( ( packed ) )
#endif
#ifndef __packed
#define __packed                     __attribute__((packed))
#endif

#define __BK_SECTION(x)                 __attribute__((section(x)))

#ifndef likely
#define likely(x)                    __builtin_expect(!!(x), 1)
#endif
#ifndef unlikely
#define unlikely(x)                  __builtin_expect(!!(x), 0)
#endif

/* XXX: move to right place */
#if (CONFIG_FULLY_HOSTED)
/* host packed */
#undef __hpacked
#define __hpacked                    __attribute__((packed))
#else
#define __hpacked
#endif

#define __bk_deprecated                 __attribute__((deprecated))
#define __bk_weak                       __attribute__((weak))
#define __bk_must_check                 __attribute__((warn_unused_result))

/* Are two types/vars the same type (ignoring qualifiers)? */
#ifndef __same_type
# define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))
#endif

/* &a[0] degrades to a pointer: a different type from an array */
#define __must_be_array(a) BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))

