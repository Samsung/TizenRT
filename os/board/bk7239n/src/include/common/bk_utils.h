// Copyright 2022-2023 Beken
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

#define DO_STRINGIFY(x) #x
#define BK_STRINGIFY(s) DO_STRINGIFY(s)

#define DO_CONCAT(x, y) x ## y
#define BK_CONCAT(x, y) DO_CONCAT(x, y)

//TODO need to fix the alignment
//On the X86, we can only align to power of 2, need to address the alignment
//in cm33!!!!
#define BK_DECL_ALIGN(type) __attribute__((aligned(sizeof(type)))) type

#define BK_BUILD_ASSERT(EXPR, MSG...) \
	enum BK_CONCAT(__build_assert_enum, __COUNTER__) { \
		BK_CONCAT(__build_assert, __COUNTER__) = 1 / !!(EXPR) \
	}

#define BK_IF_ENABLED(_flag, _code) \
        BK_COND_CODE_1(_flag, _code, ())

#define BK_COND_CODE_1(_flag, _if_1_code, _else_code) \
        A_COND_CODE_1(_flag, _if_1_code, _else_code)

#define _XXXX1 _YYYY,

/* Used internally by COND_CODE_1 and COND_CODE_0. */
#define A_COND_CODE_1(_flag, _if_1_code, _else_code) \
        __COND_CODE(_XXXX##_flag, _if_1_code, _else_code)
#define A_COND_CODE_0(_flag, _if_0_code, _else_code) \
        __COND_CODE(_ZZZZ##_flag, _if_0_code, _else_code)
#define _ZZZZ0 _YYYY,
#define __COND_CODE(one_or_two_args, _if_code, _else_code) \
        __GET_ARG2_DEBRACKET(one_or_two_args _if_code, _else_code)

#define __GET_ARG2_DEBRACKET(ignore_this, val, ...) __DEBRACKET val
#define __DEBRACKET(...) __VA_ARGS__

#define __used __attribute__((__used__))

