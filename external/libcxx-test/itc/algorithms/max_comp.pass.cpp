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
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <algorithm>

// template<class T, StrictWeakOrder<auto, T> Compare>
//   requires !SameType<T, Compare> && CopyConstructible<Compare>
//   const T&
//   max(const T& a, const T& b, Compare comp);

#include <algorithm>
#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

bool comp(int a , int b)
{
	return b < a;
}



template <class T, class C>
static int
test(const T& a, const T& b, C c, const T& x)
{
    TC_ASSERT_EXPR(&std::max(a, b, c) == &x);
    return 0;
}
template <class T>
static int
test_comp(const T& a, const T& b, const T& x)
{
    TC_ASSERT_EXPR(&std::max(a, b, comp) == &x);
    return 0;
}



/**
* @testcase             itc_libcxx_algorithms_alg_min_max_max_comp
* @brief                To find min and max between given numbers
* @scenario             chekcing min and max of two variables on several scenarios
* @apicovered           greater<int>()
* @precondition         none
* @postcondition        none
*/

int itc_libcxx_algorithms_alg_min_max_max_comp(void)
{
    for (int ind = 0;ind <=1000 ;ind++)
    {
    	{
    	int x = 0;
    	int y = -1;
    	TC_ASSERT_FUNC((test(y, x, std::greater<int>(), y)));
    	TC_ASSERT_FUNC((test(x, y, std::greater<int>(), y)));
    	}
   	{
    	int x = -1;
    	int y = -1;
    	TC_ASSERT_FUNC((test(y, x, std::greater<int>(), x)));
    	TC_ASSERT_FUNC((test(x, y, std::greater<int>(), x)));
    	}
    	{
    	int x = -1;
    	int y = -2;
    	TC_ASSERT_FUNC((test_comp(x, y, y)));
    	TC_ASSERT_FUNC((test_comp(y, x, y)));
    	}
#if TEST_STD_VER >= 14
	{
    	constexpr int x = -1;
    	constexpr int y = -1;
    	static_assert(std::max(x, y, comp) == x, "" );
    	static_assert(std::max(y, x, std::greater<int>()) == y, "" );
    	}
#endif
	
     }
    TC_SUCCESS_RESULT();
    return 0;
}
