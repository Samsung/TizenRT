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

// template<LessThanComparable T>
//   const T&
//   max(const T& a, const T& b);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"



template <class T>
static int
test(const T& a, const T& b, const T& x)
{
    TC_ASSERT_EXPR(&std::max(a, b) == &x);
    return 0;
}
/**
* @testcase             itc_libcxx_algorithms_alg_min_max_max
* @brief                To find min and max between given numbers with positive and negative values
* @scenario             chekcking min and max of two integers on several scenarios
* @apicovered          	max
* @precondition         none
* @postcondition        none
*/

int itc_libcxx_algorithms_alg_min_max_max(void)
{
    for(int ind = 0; ind < 100 ; ind++)
    {
	int* x = (int*)malloc(sizeof(int));
	int* y = (int*)malloc(sizeof(int));
	*x=25;
	*y=25;
    	TC_ASSERT_FUNC((test(*x, *y, *x)));
    	TC_ASSERT_FUNC((test(*y, *x, *y)));

	*x=0;
	*y=0;
	TC_ASSERT_FUNC((test(*x, *y, *x)));
	TC_ASSERT_FUNC((test(*y, *x, *y)));

	*x=0;
	*y=-1;
	TC_ASSERT_FUNC((test(*x, *y, *x)));
	TC_ASSERT_FUNC((test(*y, *x, *x)));

	free(x);
	free(y);
	x=NULL;
	y=NULL;
    }
    TC_SUCCESS_RESULT();
    return 0;
}
