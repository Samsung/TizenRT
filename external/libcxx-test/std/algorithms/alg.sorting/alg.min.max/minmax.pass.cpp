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
//   pair<const T&, const T&>
//   minmax(const T& a, const T& b);

#include <algorithm>
#include <cassert>

#include "test_macros.h"

template <class T>
void
test(const T& a, const T& b, const T& x, const T& y)
{
    std::pair<const T&, const T&> p = std::minmax(a, b);
    assert(&p.first == &x);
    assert(&p.second == &y);
}

int main()
{
    {
    int x = 0;
    int y = 0;
    test(x, y, x, y);
    test(y, x, y, x);
    }
    {
    int x = 0;
    int y = 1;
    test(x, y, x, y);
    test(y, x, x, y);
    }
    {
    int x = 1;
    int y = 0;
    test(x, y, y, x);
    test(y, x, y, x);
    }
#if TEST_STD_VER >= 14
    {
//  Note that you can't take a reference to a local var, since
//  its address is not a compile-time constant.
    constexpr static int x = 1;
    constexpr static int y = 0;
    constexpr auto p1 = std::minmax (x, y);
    static_assert(p1.first  == y, "");
    static_assert(p1.second == x, "");
    constexpr auto p2 = std::minmax (y, x);
    static_assert(p2.first  == y, "");
    static_assert(p2.second == x, "");
    }
#endif
}
