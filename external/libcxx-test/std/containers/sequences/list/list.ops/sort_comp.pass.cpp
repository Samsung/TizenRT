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

// <list>

// template <class Compare> sort(Compare comp);

#include <list>
#include <functional>
#include <algorithm>  // for is_permutation
#include <cassert>
#include "libcxx_tc_common.h"



#ifndef TEST_HAS_NO_EXCEPTIONS
template <typename T>
struct throwingLess {
    throwingLess() : num_(1) {}
    throwingLess(int num) : num_(num) {}

    bool operator() (const T& lhs, const T& rhs) const
    {
    if ( --num_ == 0) throw 1;
    return lhs < rhs;
    }

    mutable int num_;
    };
#endif


int tc_libcxx_containers_list_ops_sort_comp(void)
{
    {
    int a1[] = {4, 8, 1, 0, 5, 7, 2, 3, 6, 11, 10, 9};
    int a2[] = {11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    std::list<int> c1(a1, a1+sizeof(a1)/sizeof(a1[0]));
    c1.sort(std::greater<int>());
    TC_ASSERT_EXPR(c1 == std::list<int>(a2, a2+sizeof(a2)/sizeof(a2[0])));
    }

//  Test with throwing comparison; make sure that nothing is lost.
//  This is (sort of) LWG #2824
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
    int a1[] = {4, 8, 1, 0, 5, 7, 2, 3, 6, 11, 10, 9};
    const int sz = sizeof(a1)/sizeof(a1[0]);
    for (int i = 0; i < 10; ++i)
        {
        std::list<int> c1(a1, a1 + sz);
        try
            {
            throwingLess<int> comp(i);
            c1.sort(std::cref(comp));
            }
        catch (int) {}
        TC_ASSERT_EXPR((c1.size() == sz));
        TC_ASSERT_EXPR((std::is_permutation(c1.begin(), c1.end(), a1)));
        }
    }
#endif

    TC_SUCCESS_RESULT();
    return 0;
}
