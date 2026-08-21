/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#ifndef __EXTERNAL_LIBCXX_TEST_LIBCXX_TC_COMMON_H
#define __EXTERNAL_LIBCXX_TEST_LIBCXX_TC_COMMON_H
#include "tc_common.h"

/* TEST_STD_VER: C++ standard version macro */
#ifndef TEST_STD_VER
#if __cplusplus >= 202302L
#define TEST_STD_VER 23
#elif __cplusplus >= 202002L
#define TEST_STD_VER 20
#elif __cplusplus >= 201703L
#define TEST_STD_VER 17
#elif __cplusplus >= 201402L
#define TEST_STD_VER 14
#elif __cplusplus >= 201103L
#define TEST_STD_VER 11
#else
#define TEST_STD_VER 98
#endif
#endif

/* TEST_CONSTEXPR_CXX20: constexpr for C++20 and later */
#ifndef TEST_CONSTEXPR_CXX20
#if TEST_STD_VER >= 20
#define TEST_CONSTEXPR_CXX20 constexpr
#else
#define TEST_CONSTEXPR_CXX20
#endif
#endif


#ifndef _LIBCPP_HAS_GCC10_BUGS
#if defined(__arm__) && !defined(__clang__) && defined(__GNUC__) && __GNUC__ == 10
/* LIBCPP_ARM_EABI_GCC10_WORKAROUND: arm-none-eabi-gcc incomplete type traits */
#define LIBCPP_ARM_EABI_GCC10_WORKAROUND 1
/* _LIBCPP_HAS_GCC10_BUGS: GCC 10.x workaround flag */
#define _LIBCPP_HAS_GCC10_BUGS 1
#endif
#endif

/* Custom TC macros */

#define TC_ASSERT_EXPR(expr) \
do { \
	if (!(expr)) { \
		printf("\nTC Assertion FAIL at function: %s, file: %s line: %d\n", __func__, (const uint8_t *)__FILE__, (int)__LINE__); \
		total_fail++; \
	} \
} while(0)

#define TC_ASSERT_MSG(expr, msg) \
do { \
	if (!(expr)) { \
		printf("\nTC Assertion FAIL: %s\n  at function: %s, file: %s line: %d\n", msg, __func__, (const uint8_t *)__FILE__, (int)__LINE__); \
		total_fail++; \
	} \
} while(0)

#define TC_ASSERT_FUNC(funcret)\
{\
	if (0 != (funcret)) { \
		return -1; \
	} \
}

#endif /* __EXTERNAL_LIBCXX_TEST_LIBCXX_TC_COMMON_H */
